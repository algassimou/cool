#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "compile.h"

Scanner *scanner_create () {
  Scanner *s;

  s = malloc(sizeof(Scanner));
  assert (s);

  char * cp ;
  cp = malloc (BUFFER_SIZE + 1);
  assert(cp);
  s -> buffers[0] = cp;

  cp = malloc (BUFFER_SIZE + 1);
  assert(cp);
  s -> buffers[1] = cp;

  s -> string_buffer = string_buffer_create() ;

  return s;
}

int scanner_init (Scanner *scanner, const char *filename, FILE *file) {
  assert(scanner);
  assert (file);

  scanner -> filename = (char *) filename;
  scanner -> input = file;
  scanner -> line_num = 1;
  scanner -> current_buffer_index = 0;
  scanner -> pre = 0;
  scanner -> eof = 0;

  size_t buff_size = fread (scanner -> buffers[0], CHARACTER_SIZE,
			    BUFFER_SIZE, file);  

  scanner -> buffers [0][buff_size] = -1;
  return 0;
}

void scanner_del (Scanner *scanner) {
  assert(scanner);
  assert(scanner -> string_buffer);

  if (scanner -> buffers) {
    free (scanner -> buffers [0]);
    free (scanner -> buffers [1]);
  }

  string_buffer_del(scanner -> string_buffer);
  free(scanner);
}

int scanner_next_char (Scanner *scanner) {
  if (scanner -> eof) {
    return -1;
  }

  int idx = scanner -> current_buffer_index;
  int c = scanner -> buffers [idx] [scanner -> pre] ;

  if (c == -1) {

    // end of buffer
    if (scanner -> pre == BUFFER_SIZE) {
      idx = (idx + 1) % 2;

      size_t buff_size = fread (scanner -> buffers[idx], CHARACTER_SIZE,
				BUFFER_SIZE, scanner -> input);        
      scanner -> buffers [idx][buff_size] = -1;
      
      c = scanner -> buffers [idx] [0] ;
      scanner -> pre = 0;   
      scanner -> current_buffer_index = idx ;
    } else {
      scanner -> eof = 1;
    }
  }

  (scanner -> pre)++;
  // printf ("(%d : %c)", scanner -> pre - 1 , (char) c);
  return c;
} 

static void scanner_skip_space (Scanner *scanner, int c) {
  assert(scanner);

  while (isspace(c)) {
    //printf ("*%c*\n", (char)c);
    if (c == '\n') {
      (scanner -> line_num) ++;
    }
      
    c = scanner_next_char(scanner);
  }   

  (scanner -> pre) -- ;
  //printf ("ret %c", (char)c);
}

static int isOperator (int c) {
  return (c == '+' || c == '-' || 
	  c == '/' || c == '*' || 
	  c == '>' || c == '<' || 
	  c == '=');
}


int yylex (void *s) {
  assert (s);
  Scanner *scanner = (Scanner *) s;
  scanner -> string_buffer -> len = 0;

  int etat ;
  int c;

  while (1) {
    c = scanner_next_char(scanner);
    
    if (c == -1) {
      return -1;
    }
    
    switch (c) {
    case '+' : return PLUS;
    case '*' : return MULT;
    case '/' : return DIV;
    case ')' : return RPAREN; 
    case ';' : return SEMI;
    case ':' : return COLON;
    case ',' : return COMMA;
    case '{' : return LBRACE;
    case '}' : return RBRACE;
    case '[' : return LBRACKET;
    case ']' : return RBRACKET;
    case '.' : return PERIOD;
    case '@' : return AT;
    case '~' : return TILDE;
    case '=' : 
      c = scanner_next_char(scanner);
      if (c != -1) {
	if (c == '>')
	  return MATCH;
      }

      (scanner -> pre) -- ;
      return EQUAL ;
    case '<' :
      c = scanner_next_char(scanner);
      if (c != -1) {
	if (c == '-')
	  return AFF;
	else if (c == '=')
	  return LESS_EQ;
      }

      (scanner -> pre) -- ;
      return LESS ;      
    }

    if (isspace(c)) {
      scanner_skip_space (scanner, c);
      continue ;
    }   
    
    // comment's gestion
    switch (c) {
    case '-' : 
      c = scanner_next_char(scanner);
      if (c == '-') { // comment single line
	//printf ("comment single line");
	while ((c != -1) && (c != '\n')) {
	  c = scanner_next_char(scanner);
	}
	(scanner -> pre)-- ;
	continue ;
      } else {
	(scanner -> pre)-- ;
	return MINUS;
      }
      break;
    case '(' :
      c = scanner_next_char(scanner);
      if (c == '*') { // comment multi lines
	//printf ("comment multi line");
	int stop = 0, star = 0;
	while (!stop){
	  c = scanner_next_char(scanner);
	  switch (c) {
	  case '*' :
	    star = 1;
	    break ;
	  case ')' :
	    if (star) {
	      stop = 1;
	    } else {
	      star = 0;
	    }
	    break ;
	  case '\n' :
	    (scanner -> line_num)++ ;
	  default :
	    if (star) {
	      star = 0;
	    }
	    break ;
	  }
	}
	continue ;
      } else {
	(scanner -> pre)-- ;
	return LPAREN ;
      }
    }

    if (c == '\"') /* String constant*/ {
      c = scanner_next_char (scanner);
      if (c == '\"') {
	yylval.id = malloc (1);
	yylval.id[0] = '\0';
	return STRING;
      }
      etat = 300 ;
    }
    else if (isdigit(c)) {
      etat = 100;
    }
    else if (isalpha (c) || c == '_') {
      etat = 200;
    }

    char *key = NULL ;
    Symbol *sym = NULL ;
    while (1) {
      string_buffer_append (scanner -> string_buffer, c);
      c = scanner_next_char(scanner);

      switch (etat) {
      case 100 : /* integer */
	if (isdigit(c)){
	  continue;
	}

	(scanner -> pre)-- ;
	key = string_buffer_look_data(scanner -> string_buffer);       
	yylval.ival = atoi(key);
	return INTEGER;

      case 200 : /* identifier */
	if (isIdent(c)) {
	  continue;
	} 
      
	(scanner -> pre)-- ;
	key = string_buffer_look_data(scanner -> string_buffer);       
	sym = st_find_symbol(tableSymbole, key);

	if (sym == NULL) { 
	  //printf("%s new id \n", key);
	  yylval.id = string_buffer_get_data(scanner -> string_buffer) ;
	  return ID ;

	} else if (sym -> scope == keywords_scope) {
	  //printf("%s keyword \n", key);
	  return  * ((int *) sym -> val);
	} else {
	  //printf("%s old id\n", key);
	  yylval.id = sym -> id ;
	  return ID ;
	}
	break;
      case 300 : // string constant
	if (c != '\"') {
	  continue;
	}

	yylval.id = string_buffer_get_data(scanner -> string_buffer) ;
	return STRING ;
	break ;
      }
    }
  }
}
