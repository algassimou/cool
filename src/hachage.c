#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hachage.h"


HashTable *hash_table_create () {
  HashTable *ht = malloc (sizeof(HashTable));
  assert(ht);

  ht -> table = malloc (HASH_TABLE_SIZE_MAX * sizeof (DListe*)) ;
  assert (ht -> table);

  ht -> liste_listes_collision = malloc (sizeof(Liste));
  assert (ht -> liste_listes_collision);

  return ht ;
}


int hash_table_init (HashTable *h, 
		    uint32_t (*hash) (const void *),
		    int (*cmp) (const void *, const void *)
		    ) {

  assert (h);
  assert (hash);
  assert (cmp);
  
  if (liste_init (h->liste_listes_collision, NULL) == -1)
    return -1;

  h->size = HASH_TABLE_SIZE_MAX ;
  h->compare = cmp ;
  h->count = 0 ;
  h->hash_function = hash ;

  return 0 ;
}

int hash_table_add (HashTable *h, void *cle, void *val) {
  assert (h);
  assert (cle);

  uint32_t key = h->hash_function (cle) ;
  DListe * collision = h->table[key % h->size] ;
  if (collision == NULL) {

    collision = malloc (sizeof(DListe)) ;
    assert(collision);

    dliste_init (collision, NULL) ;
    if (liste_add (h->liste_listes_collision, 
		   LISTE_QUEUE(h->liste_listes_collision), collision) == -1 )
      return -1 ;

    h->table [key % h->size] = collision ;
  }

  Couple * c = NULL ;
  c = malloc (sizeof(Couple));
  assert(c);

  couple_init (c, cle, val);

  if (dliste_add_previous (collision, DLISTE_HEAD(collision), c) == -1)
    return -1 ;

  h->count++ ;
  return 0 ;
}

int hash_table_put (HashTable *h, void *cle, void *val, void **old_val) {
  assert (h != NULL);
  assert (cle != NULL);

  uint32_t key = h->hash_function (cle) ;
  DListe * collision = h->table[key % h->size] ;
  Couple * c = NULL ;

  if (collision == NULL) {

    if ((collision = malloc (sizeof(DListe))) == NULL)
      return -1 ;

    dliste_init (collision, NULL) ;
    if (liste_add (h->liste_listes_collision, 
		   LISTE_QUEUE(h->liste_listes_collision), collision) == -1 )
      return -1 ;

    h->table [key % h->size] = collision ;

    if ((c = malloc (sizeof(Couple))) == NULL)
      return -1 ;
    
    couple_init (c, cle, val);
    
    if (dliste_add_previous (collision, DLISTE_HEAD(collision), c) == -1)
      return -1 ;

    return 0 ;
  }

  DListElem * elt = NULL ;
  elt = DLISTE_HEAD (collision) ;
    
  while (elt) {
    c = (Couple *) DLISTE_ELEM_DATA(elt) ;
    if (h->compare(COUPLE_FIRST(c), cle) == 0)
      {
	if (old_val != NULL)
	  *old_val = COUPLE_SECOND(c);

	COUPLE_SECOND(c) = val ;
	return 1 ;
      }

    elt = DLISTE_ELEM_NEXT(elt);
  } 
  
  if ((c = malloc (sizeof(Couple))) == NULL)
    return -1 ;
    
  couple_init (c, cle, val);
    
  if (dliste_add_previous (collision, DLISTE_HEAD(collision), c) == -1)
    return -1 ;

  h->count++ ;
  return 0 ;
}

Couple *hash_table_search (HashTable *h, void *cle){
  assert (h != NULL);
  assert (cle != NULL);

  uint32_t key = h->hash_function (cle) ;
  DListe * collision = h->table[key % h->size] ;
  if (collision == NULL) 
    return NULL ;
  
  DListElem * elt = NULL ;
  elt = DLISTE_HEAD (collision) ;
  Couple * c = NULL ;
    
  while (elt) {
    c = (Couple *) DLISTE_ELEM_DATA(elt) ;
    if (h->compare(COUPLE_FIRST(c), cle) == 0)
      return c ;

    elt = DLISTE_ELEM_NEXT(elt);
  } 
  return NULL ;
}

int hash_table_destroy(HashTable *h, 
		      void (*destroy_cle) (void*), 
		      void (*destroy_val) (void*)
		      ){ 
  assert(h != NULL);
  
  ListElem *liste_elt = LISTE_TETE(h->liste_listes_collision);

  while (liste_elt) {
    DListe *liste = (DListe *) LISTE_ELEM_DATA(liste_elt) ;
    DListElem *elt = DLISTE_HEAD(liste) ;

    while (elt) {
      Couple *c = (Couple *) DLISTE_ELEM_DATA(elt) ;
      if (destroy_cle)
	destroy_cle(COUPLE_FIRST(c));

      if (destroy_val)
	destroy_val(COUPLE_SECOND(c));

      free(c);
      elt = DLISTE_ELEM_NEXT(elt);
      h->count -- ;
    }
    liste_elt = LISTE_ELEM_NEXT(liste_elt);
    free(liste);
  }

  return 0 ;
}

int hash_table_rm (HashTable *h, void *cle, void **data) {
  assert (h != NULL);
  assert (cle != NULL);

  if (h->count == 0)
    return 1 ;

  uint32_t key = h->hash_function (cle) ;
  DListe * collision = h->table[key % h->size] ;
  if (collision == NULL) 
    return 1 ;
  
  DListElem *elt = NULL ;
  elt = DLISTE_HEAD (collision) ;
  Couple * c = NULL ;
    
  while (elt) {
    c = (Couple *) DLISTE_ELEM_DATA(elt) ;
    if (h->compare(COUPLE_FIRST(c), cle) == 0)
      {
	if (data != NULL)
	  *data = c ;
	
	h->count -- ;
	return dliste_rm (collision, elt, NULL);
      }
    elt = DLISTE_ELEM_NEXT(elt);
  } 
  return 1 ;
}
