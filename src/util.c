#include <stdio.h>
#include <stdlib.h>
#include "compile.h"

void compile_panic (char *s) {
  fprintf(stderr, "**PANIC** %s\n", s);
  exit(-1);
}

void add_symbole (char *key) {
  Symbol *sym ;
  sym = symbol_create();
  symbol_init(sym, NULL);
  sym -> id = key;
  sym -> val = key;
  sym -> scope = st_current_scope(tableSymbole);

  /* printf ("ADD key (%s) val (%p) scope (%p)\n",  */
  /* 	  (char *) sym -> id, */
  /* 	  sym -> val,   */
  /* 	  sym -> scope); */
  
  
  if (st_add_symbol(tableSymbole, sym) == -1) {
    compile_panic ("Unable to add symbol in the symbol table") ; 
  }
}
