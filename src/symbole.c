#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "symbole.h"

#define DEBUG

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

#ifdef DEBUG
  printf ("DEL key (%s) val (%p) scope (%p)\n",
  	  (char *) sym -> id,
  	  sym -> val,
  	  sym -> scope);


  if (sym -> symbol_del) {
    sym -> symbol_del (sym -> id, sym -> val);
  }
#endif

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

  if (LISTE_SIZE(st -> scopes) != 0) {
    while (LISTE_SIZE(st -> scopes) != 0) {
      st_end_scope(st);
    }
  }
  
  liste_del(st -> scopes);
  hash_table_del(st -> table, NULL, NULL);
  free (st);
}


int st_create_scope(SymboleTable *st) {
  assert (st);

  Liste *scope =  liste_create();

  if (liste_init(scope, NULL) == -1)
    return -1 ;

#ifdef DEBUG
  printf ("================ create scope %p ==================\n", scope);
#endif

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

#ifdef DEBUG
  printf ("================ end scope %p ==================\n", scope);
#endif

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

#ifdef DEBUG
  printf ("ADD key (%s) val (%p) scope (%p)\n",
  	  (char *) s -> id,
  	  s -> val,
  	  s -> scope);
#endif

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
