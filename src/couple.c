#include <stdio.h>
#include <assert.h>
#include "couple.h"

int couple_init (Couple *c, void *f, void *s) {
  assert(c != NULL);

  COUPLE_FIRST(c) = f;
  COUPLE_SECOND(c) = s ;
  return 0 ;
}
