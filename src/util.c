#include <stdio.h>
#include <stdlib.h>
#include "compile.h"

void compile_panic (char *s) {
  fprintf(stderr, "**PANIC** %s\n", s);
  exit(-1);
}

static void id_del (void *key, void *val) {
  free(key);
  //free(val);
}

void add_symbole (char *key) {
  Symbol *sym ;
  sym = symbol_create();
  symbol_init(sym, id_del);
  sym -> id = key;
  sym -> val = key;
  sym -> scope = st_current_scope(tableSymbole);
    
  if (st_add_symbol(tableSymbole, sym) == -1) {
    compile_panic ("Unable to add symbol in the symbol table") ; 
  }
}
