#ifndef __COUPLE__
#define __COUPLE__

/**
 * definit un couple de valeur 
 * first la première valeur 
 * second la seconde valeur 
 */
struct _Couple {
  void *first ;
  void *second ;
} ;

#define COUPLE_FIRST(c) ((c)->first)
#define COUPLE_SECOND(c) ((c)->second)

typedef struct _Couple Couple ;

int couple_init (Couple *c, void *f, void *s);

#endif
