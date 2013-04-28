#include "dliste.h"
#include <stdlib.h>
#include <string.h>

int dliste_init (DListe * dl , void (*rm) (void *))
{
  if (dl == NULL) return -1 ;
  DLISTE_SIZE(dl) = 0 ;
  DLISTE_HEAD(dl) = NULL ;
  DLISTE_QUEUE(dl) = NULL ;
  DLISTE_REMOVE(dl) = rm ;
  return 0;
}

int dliste_add_next(DListe * dl , DListElem * elt , const void * data)
{
  DListElem * new = NULL ;

  if (elt == NULL && DLISTE_SIZE(dl) != 0) return -1 ;

  new = malloc (sizeof(DListElem));
  if (new == NULL) return -1 ;

  DLISTE_ELEM_DATA(new) = (void *) data ;
  DLISTE_ELEM_NEXT(new) = NULL ;
  DLISTE_ELEM_PREVIOUS(new) = NULL ;

  if (DLISTE_SIZE(dl) == 0)
    {
      DLISTE_HEAD(dl) = new ;
      DLISTE_QUEUE(dl) = new ;
    }
  else 
    {
      DLISTE_ELEM_PREVIOUS(new) = elt ;
      DLISTE_ELEM_NEXT(new) = DLISTE_ELEM_NEXT(elt) ;
      DLISTE_ELEM_NEXT(elt) = new ;

      if (DLISTE_ELEM_NEXT(new) == NULL) /* ajout en fin */
	DLISTE_QUEUE(dl) = new ;
      else
	DLISTE_ELEM_PREVIOUS(DLISTE_ELEM_NEXT(new)) = new ;
    }
  DLISTE_SIZE(dl) ++ ;
  return 0 ;
}

int dliste_add_previous(DListe * dl , DListElem * elt , const void * data)
{
  DListElem * new = NULL ;

  if (elt == NULL && DLISTE_SIZE(dl) != 0) return -1 ;

  new = malloc (sizeof(DListElem));
  if (new == NULL) return -1 ;

  DLISTE_ELEM_DATA(new) = (void *) data ;
  DLISTE_ELEM_NEXT(new) = NULL ;
  DLISTE_ELEM_PREVIOUS(new) = NULL ;

  if (DLISTE_SIZE(dl) == 0)
    {
      DLISTE_HEAD(dl) = new ;
      DLISTE_QUEUE(dl) = new ;
    }
  else 
    {
      DLISTE_ELEM_NEXT(new) = elt ;
      DLISTE_ELEM_PREVIOUS(new) = DLISTE_ELEM_PREVIOUS(elt) ;
      DLISTE_ELEM_PREVIOUS(elt) = new ;

      if (DLISTE_ELEM_PREVIOUS(new) == NULL)
	DLISTE_HEAD(dl) = new ;
      else 
	DLISTE_ELEM_NEXT(DLISTE_ELEM_PREVIOUS(new)) = new ;
    }
  DLISTE_SIZE(dl) ++ ;
  return 0 ;
}

int dliste_rm(DListe * dl , DListElem * elt , void **data)
{
  if (elt == NULL) return -1 ;
  if (DLISTE_SIZE(dl) == 0) return -1 ;

  if (DLISTE_ELEM_NEXT(elt))
    DLISTE_ELEM_PREVIOUS(DLISTE_ELEM_NEXT(elt)) = DLISTE_ELEM_PREVIOUS(elt);
  
  if (DLISTE_ELEM_PREVIOUS(elt))
    DLISTE_ELEM_NEXT(DLISTE_ELEM_PREVIOUS(elt)) = DLISTE_ELEM_NEXT(elt);

  if (DLISTE_HEAD(dl) == elt)
    DLISTE_HEAD(dl) = DLISTE_ELEM_NEXT(elt);
  
  if (DLISTE_QUEUE(dl) == elt)
    DLISTE_QUEUE(dl) = DLISTE_ELEM_PREVIOUS(elt);

  if (data)
    *data = DLISTE_ELEM_DATA(elt);

  free(elt);
  DLISTE_SIZE(dl) -- ;
  return 0 ;
}

int dliste_destroy(DListe *dl)
{
  void * data = NULL ;

  while (DLISTE_SIZE(dl))
    {
      dliste_rm(dl , DLISTE_HEAD(dl) , &data);
      if (DLISTE_REMOVE(dl))
	DLISTE_REMOVE(dl)(data);
    }
  memset(dl , 0 , sizeof(DListe));
  return 0 ;
}
