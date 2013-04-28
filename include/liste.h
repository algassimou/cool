#ifndef __LISTE__
#define __LISTE__

typedef struct ListElem {
  void * data ;
  struct ListElem * next ;
} ListElem ;

#define LISTE_ELEM_DATA(le) ((le)->data)
#define LISTE_ELEM_NEXT(le) ((le)->next)

typedef struct {
  int size ;
  ListElem * tete ;
  ListElem * queue ;
  void (*remove) (void*);
} Liste ;

#define LISTE_SIZE(l) ((l)->size)
#define LISTE_TETE(l) ((l)->tete)
#define LISTE_QUEUE(l) ((l)->queue)
#define LISTE_REMOVE(l) ((l)->remove)

// life cycle
Liste *liste_create ();
int liste_init (Liste * , void (*)(void *));
int liste_del(Liste *);

// operations
int liste_add(Liste * , ListElem * , const void *);
int liste_rm (Liste * , ListElem * , void **);


#endif /*__LISTE__*/
