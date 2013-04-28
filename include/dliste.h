#ifndef __DLIST__
#define __DLIST__

typedef struct DListElem {
  void * data ;
  struct DListElem * next ;
  struct DListElem * previous ;
} DListElem ;

#define DLISTE_ELEM_DATA(de) ((de)->data)
#define DLISTE_ELEM_NEXT(de) ((de)->next)
#define DLISTE_ELEM_PREVIOUS(de) ((de)->previous)

typedef struct DListe {
  DListElem * head ;
  DListElem * queue ;
  int size ;
  void (*remove) (void *) ;
} DListe ;

#define DLISTE_SIZE(dl) ((dl)->size)
#define DLISTE_REMOVE(dl) ((dl)->remove)
#define DLISTE_HEAD(dl) ((dl)->head)
#define DLISTE_QUEUE(dl) ((dl)->queue)

int dliste_init (DListe * , void (*)(void *));
int dliste_add_next (DListe * , DListElem * , const void *);
int dliste_add_previous (DListe * , DListElem * , const void *);
int dliste_rm (DListe * , DListElem * , void **);
int dliste_destroy(DListe *);


#endif /*__DLIST__*/
