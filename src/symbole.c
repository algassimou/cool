#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "symbole.h"

Symbol *symbol_create () {
  Symbol *s ;
  s = malloc (sizeof (Symbol));
  assert(s);
  
  return s;
}

int symbol_init (Symbol *s, void (*symbol_del) (void *, void *)) {
  assert(s);
  s -> symbol_del = symbol_del ;
  return 0;
}


void symbol_del (void *s) {
  assert(s);
  Symbol * sym = (Symbol *)s; 

  printf ("DEL key (%s) val (%p) scope (%p)\n",
  	  (char *) sym -> id,
  	  sym -> val,
  	  sym -> scope);


  if (sym -> symbol_del) {
    sym -> symbol_del (sym -> id, sym -> val);
  }

  free(sym);
}

static uint32_t sdbm(const void *s)
{
  unsigned char *str = (unsigned char *) s ;
  uint32_t hash = 0;
  uint32_t c;

  while (c = *str++)
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

static int cmp (const void *c1, const void *c2) {
  char *s1 = (char *) c1 ;
  char *s2 = (char *) c2;

  return strcmp(s1,s2);
}

SymboleTable *symbole_table_create () {
  SymboleTable *st = malloc(sizeof(SymboleTable));
  assert(st);
  
  st -> table = hash_table_create () ;
  st -> scopes = liste_create() ;

  return st ;
}

int symbole_table_init (SymboleTable *st) {
  assert(st);

  if (hash_table_init (st->table, sdbm, cmp) == -1)
    return -1 ;

  return liste_init(st->scopes, NULL) ;
}

void symbole_table_del(SymboleTable *st) {
  assert (st);
  hash_table_destroy(st -> table, NULL, NULL);
  liste_destroy(st -> scopes);
  free (st);
}


int st_create_scope(SymboleTable *st) {
  assert (st);

  Liste *scope =  liste_create();

  if (liste_init(scope, NULL) == -1)
    return -1 ;

  return liste_add(st->scopes, NULL, scope) ;
}

int st_end_scope(SymboleTable *st) {
  assert(st);

  Liste *scope = NULL;

  if (liste_rm(st -> scopes, NULL, (void **)&scope) == -1)
    return -1;

  if (scope == NULL)
    return -1;

  DListElem *val = NULL ;
  DListe *collision = NULL ;
  Couple *c = NULL ;

  while(LISTE_SIZE(scope) != 0) {
    if (liste_rm(scope, NULL, (void **)&val) == -1)
      return -1 ;

    c = (Couple *) DLISTE_ELEM_DATA(val) ;

    uint32_t hash = st->table->hash_function(COUPLE_FIRST(c)) ;
    collision = st->table->table[hash % st->table->size] ;
    if (dliste_rm(collision, val, NULL) == -1)
      return -1 ;

    symbol_del(COUPLE_SECOND(c));
    free(c);
  }

  free(scope);
  return 0 ;
}

void *st_current_scope (SymboleTable *st) {
  assert(st);
  ListElem * head = LISTE_TETE(st->scopes) ;
  if (head == NULL)
    return NULL ;

  return LISTE_ELEM_DATA(head);
}

int st_check_scope (SymboleTable *st, char *s) {
  assert (st);
  assert (s);

  Symbol *sym = st_find_symbol(st, s);
  if (sym == NULL)
    return 0;

  return sym -> scope == LISTE_ELEM_DATA(LISTE_TETE(st->scopes)) ;
}

int st_add_symbol (SymboleTable *st, Symbol *s) {
  assert(st);
  assert(s);

  printf ("ADD key (%s) val (%p) scope (%p)\n",
  	  (char *) s -> id,
  	  s -> val,
  	  s -> scope);


  // insertion dans la table de hachage
  if (hash_table_add(st->table, s->id, s) == -1)
    return -1 ;
  
  // gestion de la portée
  Liste *scope = (Liste *) LISTE_ELEM_DATA(LISTE_TETE(st -> scopes)) ;
  uint32_t hash = st->table->hash_function(s -> id) ;
  DListe *collision = st->table->table[hash % st->table->size] ;
  DListElem *elt = DLISTE_HEAD(collision) ;

  return liste_add(scope, LISTE_QUEUE(scope), elt) ;
}

Symbol *st_find_symbol (SymboleTable *st, char *id) {
  assert(st);
  assert(id);

  Couple *c = (Couple *) hash_table_search (st->table, id) ;
  if (c == NULL)
    return NULL ;

  return (Symbol *) COUPLE_SECOND(c);
}


/*
int symbole_table_init_general_scope (SymboleTable *st, char **keys) {
  assert(st);
  assert(keys);

  if (symbole_table_add_block(st) == -1)
    return -1;

  int cpt = 0;
  char *key = keys[cpt] ;
  while(key != NULL) {
    if (symbole_table_insert(st, key, key) == -1)
      return -1;

    key = keys[++cpt];
  }
  return 0;
}


int symbole_table_insert (SymboleTable *st, void *key, void *val) {
  assert(st != NULL);
  assert(key != NULL);

  Liste *scope = (Liste *) LISTE_ELEM_DATA(LISTE_TETE(st->block_stack)) ;

  SymboleVal *sym = malloc (sizeof(SymboleVal));
  assert(sym);
  sym -> val = val ;
  sym -> scope = scope ;

  // insertion dans la table de hachage
  if (hash_table_add(st->table, key, sym) == -1)
    return -1 ;
  
  // gestion de la portée
  uint32_t hash = st->table->hash_function(key) ;
  DListe *collision = st->table->table[hash % st->table->size] ;
  DListElem *elt = DLISTE_HEAD(collision) ;

  return liste_add(scope, LISTE_QUEUE(scope), elt) ;
}

Couple *symbole_table_search (SymboleTable *st, void *key){
  assert(st);
  assert(key);
  
  return hash_table_search (st->table, key) ;
}

int symbole_table_rm_block (SymboleTable *st){
  Liste *scope = (Liste *) LISTE_ELEM_DATA(LISTE_TETE(st->block_stack)) ;
  DListElem *val = NULL ;
  DListe *collision = NULL ;
  Couple *c = NULL ;

  while(LISTE_SIZE(scope) != 0) {
    if (liste_rm(scope, NULL, (void **)&val) == -1)
      return -1 ;

    c = (Couple *) DLISTE_ELEM_DATA(val) ;

    uint32_t hash = st->table->hash_function(COUPLE_FIRST(c)) ;
    collision = st->table->table[hash % st->table->size] ;
    if (dliste_rm(collision, val, NULL) == -1)
      return -1 ;

    free(c);
  }

  return 0 ;
}

int symbole_table_add_block (SymboleTable *st) {
  assert(st);

  Liste *scope =  malloc (sizeof(Liste)) ;
  assert(scope);

  if (liste_init(scope, NULL) == -1)
    return -1 ;

  return liste_add(st->block_stack, NULL, scope) ;
}

*/
