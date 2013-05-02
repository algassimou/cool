%{
#include <stdio.h>
#include <stdlib.h>
#include "compile.h"

  static int check_type (char *type){
    Symbol *sym = st_find_symbol(tableSymbole, type);
    if (sym == NULL) /* not ever occur*/
      return 0;
    else {
      return sym -> scope == types_scope ;
    }
  }

%}

%union {
  char *id ;
  int ival;
}

%token <id> ID STRING
%token <ival> INTEGER 
%token FDN COLON LPAREN RPAREN COMMA
%token MATCH LBRACKET RBRACKET SEMI LBRACE RBRACE
%token CLASS CASE ELSE ESAC FALSE FI IF IN INHERITS
%token LET LOOP OF POOL THEN TRUE WHILE NEW

%right AFF 
%nonassoc NOT 
%nonassoc LESS LESS_EQ EQUAL
%left PLUS MINUS
%left MULT DIV
%nonassoc ISVOID
%nonassoc TILDE
%nonassoc AT
%nonassoc PERIOD

 //%type<ival> expr
%start program

%%

program : classes

classes : def_class classes | def_class 

def_class : class SEMI {
  printf ("def class\n");
 } // | error SEMI {printf (".");} 

class : CLASS ID {
  if (st_check_scope(tableSymbole, $2)) {
    // error 
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", $2) ;
  } else {
    add_symbole(yylval.id);
  }
 } inherits block_class
 
 inherits : INHERITS ID {
  if (check_type($2) == 0) {
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s must be a defined type \n", $2);

    free($2);
  }
} | empty
 
empty : ;

block_class : LBRACE {
  if (st_create_scope(tableSymbole) == -1) {
    compile_panic("not enable to create a new scope");
  }
} def_features RBRACE {
  st_end_scope(tableSymbole);
} // | LBRACE error {printf ("*******************");} RBRACE 
 

def_features : features | empty

features : def_feature features | def_feature 

def_feature : feature SEMI 

feature : feature_method | feature_attribute

feature_attribute : ID COLON ID {
 if (st_check_scope(tableSymbole, $1)) {
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", $1) ;
    free($1);
    free($3);
  } else {
    add_symbole($1);
    if (check_type($3) == 0) {
      compile_error(scanner -> filename, scanner -> line_num, 
		    "%s must be a defined type\n", $3);
    
      free($3);
    }
  }
}
| ID COLON ID AFF expr {

  if (st_check_scope(tableSymbole, $1)) {
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", $1) ;
    free($1);
    free($3);
  } else {
    add_symbole($1);
    if (check_type($3) == 0) {
      compile_error(scanner -> filename, scanner -> line_num, 
		    "%s must be a defined type\n", $3);
    
      free($3);
    }
  }

  //printf (" init %d\n", $5);

}
| /*ID error {
  free($1);
  compile_error(scanner -> filename, scanner -> line_num, 
		  "maybe forget :\n");
    yyclearin ;
    yyerrok ;
} ID {
  printf("id %s\n", $4);
  if (st_find_symbol(tableSymbole, $4) == NULL)
    free($4);
  }
  */

//init : AFF expr {printf ("aff init");} | empty 

feature_method : ID formals COLON ID block_method {
  if (st_check_scope(tableSymbole, $1)) {
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", $1) ;
    free($1);
    free($4);
  } else {
    add_symbole($1);
    if (check_type($4) == 0) {
      compile_error(scanner -> filename, scanner -> line_num, 
		    "%s must be a defined type \n", $4);
      
      free($4);
    }
  }
} 

formals : LPAREN RPAREN | LPAREN {
  if (st_create_scope(tableSymbole) == -1) {
    compile_panic("not enable to create a new scope");
  }
} formal_list  RPAREN {
  st_end_scope(tableSymbole);
}

formal_list : formal COMMA formal_list | formal 

formal : ID COLON ID {
 if (st_check_scope(tableSymbole, $1)) {
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", $1) ;
    free($1);
    free($3);
  } else {
    add_symbole($1);
    if (check_type($3) == 0) {
      compile_error(scanner -> filename, scanner -> line_num, 
		  "%s must be a defined type \n", $3);
    
      free($3);
    }
  }
}

block_method : LBRACE {
  if (st_create_scope(tableSymbole) == -1) {
    compile_panic("not enable to create a new scope");
  }
} expr {printf("expr\n");} RBRACE {
  st_end_scope(tableSymbole);
} // | LBRACE error {printf ("*******************");} RBRACE 

expr : ID AFF expr
| expr typed PERIOD ID LPAREN effectives RPAREN
| ID LPAREN effectives RPAREN
| IF expr THEN expr ELSE expr FI 
| WHILE expr LOOP expr POOL
| LBRACE exprs RBRACE
| LET {  
  if (st_create_scope(tableSymbole) == -1) {
    compile_panic("not enable to create a new scope");
  }
} let_vars IN expr {
  st_end_scope(tableSymbole);
}

| CASE expr OF case_block ESAC 
| NEW ID // type 
| ISVOID expr 
| expr PLUS expr //{printf ("(%d + %d)", $1, $3) ; $$ = $1 + $3;}
| expr MINUS expr //{printf ("(%d - %d)", $1, $3) ; $$ = $1 - $3}
| expr MULT expr //{printf ("(%d * %d)", $1, $3) ; $$ = $1 * $3;}
| expr DIV expr //{printf ("(%d / %d)", $1, $3) ; $$ = $1 / $3}
| TILDE expr
| expr LESS expr
| expr LESS_EQ expr
| expr EQUAL expr
| NOT expr
| LPAREN expr RPAREN //{$$ = $2;}
| ID
| INTEGER //{ printf ("******** const integer (%d) **********\n", $1);}
| STRING //{printf ("******** const string (%s) **********\n", $1); free($1); }
| TRUE 
| FALSE

typed : AT ID | empty 

effectives : expr COMMA effectives | expr 

exprs : expr SEMI exprs | expr SEMI
case_block : case_line case_block | case_line
case_line : ID COLON ID MATCH expr SEMI {
  // type
}

let_vars : let_var COMMA let_vars | let_var 
let_var : ID COLON ID {
 if (st_check_scope(tableSymbole, $1)) {
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", $1) ;
    free($1);
    free($3);
  } else {
    add_symbole($1);
    if (check_type($3) == 0) {
      compile_error(scanner -> filename, scanner -> line_num, 
		  "%s must be a defined type \n", $3);
    
      free($3);
    }
  }
}
| ID COLON ID AFF expr {
 if (st_check_scope(tableSymbole, $1)) {
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", $1) ;
    free($1);
    free($3);
  } else {
    add_symbole($1);
    if (check_type($3) == 0) {
      compile_error(scanner -> filename, scanner -> line_num, 
		  "%s must be a defined type \n", $3);
    
      free($3);
    }
  }
}

%%

int yyerror (char *s) {
  printf ("%s", s);
  return -1;
}

