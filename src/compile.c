#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "compile.h"

Scanner *scanner ;
SymboleTable *tableSymbole = NULL;

void *keywords_scope = NULL;
void *types_scope = NULL;

static char * keywords [] = {
  "class", "case", "else", "esac", "false", "fi",
  "if", "in", "inherits", "isvoid", "let", "loop",
  "new", "not", "of", "pool", "then", "true", "while",
  NULL
};

static char * basic_type [] = {
  "Bool", "Int", "IO", "Object", "String", NULL
};

static void keyword_del (void *key, void *val){
  free(val);
}

static void compile_end () {
  // end type scope
  st_end_scope(tableSymbole);

  //end keywords scope
  st_end_scope(tableSymbole);
}

static int keywords_init () {
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
    case 14: /*of*/ *val = OF; break ;
    case 15: /*pool*/ *val = POOL; break ;
    case 16: /*then*/ *val = THEN; break ;
    case 17: /*true*/ *val = TRUE; break ;
    case 18: /*while*/ *val = WHILE; break ;
    }
 
    // create and init symbole
    sym = symbol_create();
    symbol_init(sym, keyword_del);
    sym -> id = key;
    sym -> val = val;
    sym -> scope = keywords_scope;

    if (st_add_symbol(tableSymbole, sym) == -1)
      return -1;

    key = keywords[++cpt];
  }

  return 0;
}

static int basic_types_init () {
  // add a type scope
  if (st_create_scope(tableSymbole) == -1){
    compile_panic("not enable to create a new scope");
  }
  
  types_scope = st_current_scope(tableSymbole);

  int cpt = 0;
  char *key = basic_type[cpt] ;
  Symbol *sym;

  while(key != NULL) {
    // create and init symbole
    sym = symbol_create();
    symbol_init(sym, NULL);
    sym -> id = key;
    sym -> val = key;
    sym -> scope = types_scope ;

    if (st_add_symbol(tableSymbole, sym) == -1)
      return -1;

    key = basic_type[++cpt];
  }

  return 0;
}

static int compile_init () {  
  if (keywords_init() == -1)
    return -1;

  if (basic_types_init() == -1)
    return -1;

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

  if (argc < 2) { 
    fprintf (stderr, "Usage %s file1 file2 ... fileN\n", argv[0]);
    return -1;
  }

  FILE *input = NULL;
  int cpt = 0;

  // creations
  tableSymbole = symbole_table_create ();
  scanner = scanner_create ();

  // initializations
  symbole_table_init(tableSymbole);
  compile_init ();

  for (cpt = 1; cpt < argc ; cpt++) {

    if ((input = fopen(argv[cpt], "r")) == NULL)  {
      return -1;
    }
    
    scanner_init(scanner, argv[cpt], input);

    // compilation
    fprintf(stdout, "============== start compilation %s ===============\n", argv[cpt]);
    yyparse();
    fprintf(stdout, "==============  end conpilation  %s ==============\n", argv[cpt]);
  }

  // destructions
  compile_end();
  symbole_table_del(tableSymbole);
  scanner_del(scanner);
  fclose(input);

  return 0;
}
