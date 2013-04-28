%{
#include <stdio.h>
#include <stdlib.h>
#include "compile.h"
%}

%union {
  char *id ;
  int ival;
}


%token <id> TYPE ID
%token FDN PLUS MOINS MULT DIV COLON PERIOD LPAREN RPAREN COMMA
%token LBRACKET RBRACKET EQUAL SEMI LBRACE RBRACE
%token CLASS CASE ELSE ESAC FALSE FI IF IN INHERITS ISVOID
%token LET LOOP NEW NOT OF POOL THEN TRUE WHILE 
%start program

%%
program : classes

classes : def_class classes | def_class 

def_class : class SEMI {
  printf ("def class\n");
 } // | error SEMI {printf (".");} 

class : CLASS ID {
  if (st_check_scope(tableSymbole, yylval.id)) {
    // error 
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", yylval.id) ;
  } else {
    add_symbole(yylval.id);
  }
 } inherits block_class
 
 inherits : INHERITS ID | empty
 
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

feature : ID {
  if (st_check_scope(tableSymbole, yylval.id)) {
    // error 
    compile_error(scanner -> filename, scanner -> line_num, 
		  "%s is already on use\n", yylval.id) ;
  } else {
    add_symbole(yylval.id);
  }
} COLON ID

%%

int yyerror (char *s) {
  printf ("%s\n", s);
  return -1;
}

