#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "compile.h"

Scanner *scanner ;
SymboleTable *tableSymbole = NULL;

void *keywords_scope = NULL;

static char * keywords [] = {
  "class", "case", "else", "esac", "false", "fi",
  "if", "in", "inherits", "isvoid", "let", "loop",
  "new", "not", "of", "pool", "then", "true", "while",
  NULL
} ;

static int keywords_scope_init () {

  if (st_create_scope(tableSymbole) == -1)
    return -1 ;

  keywords_scope = st_current_scope(tableSymbole);

  int cpt = 0;
  char *key = keywords[cpt] ;
  Symbol *sym;
  int *val;

  while(key != NULL) {
    val = malloc (sizeof(int));
    assert(val);

    switch (cpt) {
    case 0: /*class*/ *val = CLASS;  break ;
    case 1: /*case*/  *val = CASE; break ;
    case 2: /*else*/  *val = ELSE; break ;
    case 3: /*esac*/ *val = ESAC;  break ;
    case 4: /*false*/ *val = FALSE; break ;
    case 5: /*fi*/  *val = FI; break ;
    case 6: /*if*/ *val = IF; break ;
    case 7: /*in*/ *val = IN; break ;
    case 8: /*inherits*/ *val = INHERITS; break ;
    case 9: /*isvoid*/ *val = ISVOID; break ;
    case 10: /*let*/ *val = LET; break ;
    case 11: /*loop*/ *val = LOOP; break ;
    case 12: /*new*/ *val = NEW; break ;
    case 13: /*not*/ *val = NOT; break ;
    case 14: /*pool*/ *val = POOL; break ;
    case 15: /*then*/ *val = THEN; break ;
    case 16: /*true*/ *val = TRUE; break ;
    case 17: /*while*/ *val = WHILE; break ;
    }
 
    // create and init symbole
    sym = symbol_create();
    symbol_init(sym, free);
    sym -> id = key;
    sym -> val = val;
    sym -> scope = keywords_scope;

    if (st_add_symbol(tableSymbole, sym) == -1)
      return -1;

    key = keywords[++cpt];
  }
}

static int compile_init () {  
  keywords_scope_init();
  // add a global scope
  if (st_create_scope(tableSymbole) == -1)
    return -1 ;

  nerrors = 0;
  silence = 0;
  /*
  int cpt = 0;
  char *key = keywords[cpt];
  while (key != NULL) {
    Symbol *sym = st_find_symbol(tableSymbole, key);

    printf ("key (%s) val (%d) scope (%p) global %d\n", 
	    (char *) sym -> id,
	    *((int *) sym -> val),  
	    sym -> scope, st_check_scope(tableSymbole, sym));
    key = keywords[++cpt];
  }
  
  printf ("==========================================================\n"); 
  
  /*
  Liste *scope = st_current_scope(tableSymbole);
  ListElem *elt = LISTE_TETE(scope);
  while (elt != NULL) {
    Couple * couple = DLISTE_ELEM_DATA((DListElem *) LISTE_ELEM_DATA(elt));
    Symbol *sym = COUPLE_SECOND(couple);

    printf ("key (%s) val (%s) scope (%p) global %d\n", 
	    (char *) sym -> id,
	    (char *) sym -> val,  
	    sym -> scope, st_check_scope(tableSymbole, sym));
    key = keywords[++cpt];
    elt = LISTE_ELEM_NEXT(elt);
  }

  printf ("==========================================================\n");

  if (st_end_scope (tableSymbole) == -1)
  return -1 ;
  
  cpt = 0;
  key = keywords[cpt];
  while (key != NULL) {
  Symbol *sym = st_find_symbol(tableSymbole, key);
  printf ("%p %p\n",sym, st_current_scope(tableSymbole));
  key = keywords[++cpt];
  }
  printf ("==========================================================\n");
  
  */
  
  return 0;
}

int main(int argc, char *argv[])
{

  if (argc != 2) 
    return -1;

  FILE *input = NULL;


  if ((input = fopen(argv[1], "r")) == NULL)  {
    return -1;
  }

  // creations
  tableSymbole = symbole_table_create ();
  scanner = scanner_create ();

  // initializations
  symbole_table_init(tableSymbole);
  scanner_init(scanner, argv[1], input);
  compile_init ();

  // compilation
  fprintf(stdout, "============== start compilation ==============\n");
  yyparse();
  fprintf(stdout, "============== end conpilation ==============\n");

  // destructions
  symbole_table_del(tableSymbole);
  scanner_del(scanner);
  fclose(input);

  return 0;
}
