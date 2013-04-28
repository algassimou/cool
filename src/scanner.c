#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "compile.h"

Scanner *scanner_create () {
  Scanner *s;

  s = malloc(sizeof(Scanner));
  assert (s);

  s -> string_buffer = malloc (sizeof(StringBuffer));
  assert (s -> string_buffer);

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
  if (scanner) {
    if (scanner -> buffers) {
      free (scanner -> buffers [0]);
      free (scanner -> buffers [1]);
    }

    if (scanner -> string_buffer) {
      string_buffer_del(scanner -> string_buffer);
    }

    free(scanner);
  }
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
    case '=' : return EQUAL ; // a fixer = ou ==
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
	printf ("comment single line");
	while ((c != -1) && (c != '\n')) {
	  c = scanner_next_char(scanner);
	}
	(scanner -> pre)-- ;
	continue ;
      } else {
	(scanner -> pre)-- ;
	return MOINS;
      }
      break;
    case '(' :
      c = scanner_next_char(scanner);
      if (c == '*') { // comment multi lines
	printf ("comment multi line");
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

    if (isalpha (c) || c == '_') {
      etat = 200;
    }

    while (1) {
      string_buffer_append (scanner -> string_buffer, c);
      c = scanner_next_char(scanner);

      switch (etat) {
      case 200 : /* identifier */
	if (isIdent(c)) {
	  continue;
	} 
      
	(scanner -> pre)-- ;
	char *key = string_buffer_data(scanner -> string_buffer);       
	Symbol *sym = st_find_symbol(tableSymbole, key);

	if (sym == NULL) { 
	  //printf("%s new id \n", key);
	  yylval.id = key ;
	  return ID ;
	} else if (sym -> scope == keywords_scope) {
	  //printf("%s keyword \n", key);
	  return  * ((int *) sym -> val);
	} else {
	  //printf("%s old id\n", key);
	  yylval.id = key ;
	  return ID ;
	}
	break;
      }
    }
  }
}

/*
int yylex (void *s) {
  assert (s);
  Scanner *scanner = (Scanner *) s;
  scanner -> string_buffer -> len = 0;

  int etat ;
  int c = scanner_next_char(scanner);

  if (c == -1) {
    return 0;
  }

  if (isspace(c)) {
    c = scanner_skip_space (scanner, c);
  }  

  // comment's gestion
  switch (c) {
  case '-' : 
    c = scanner_next_char(scanner);
    if (c == '-') { // comment single line
      printf ("comment");
      while ((c != -1) && (c != '\n')) {
	c = scanner_next_char(scanner);
      }
      (scanner -> pre)-- ;
    } else {
      (scanner -> pre)-- ;
      return MOINS;
    }
    break;
  case '(' : return LPAREN ;
  }

  c = scanner_next_char(scanner);

  if (c == -1) {
    return 0;
  }
  
  if (isspace(c)) {
    c = scanner_skip_space (scanner, c);
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
  case '=' : return EQUAL ; // a fixer = ou ==
  }

  if (isalpha (c) || c == '_') {
    etat = 200;
  }

  while (1) {
    string_buffer_append (scanner -> string_buffer, c);
    c = scanner_next_char(scanner);

    switch (etat) {
    case 200 : /* identifier 
      if (isIdent(c)) {
	continue;
      } 
      
      (scanner -> pre)-- ;
      char *key = string_buffer_data(scanner -> string_buffer); 
      
      Symbol *sym = st_find_symbol(tableSymbole, key);
      if (sym == NULL) { 
	//printf("%s new id \n", key);
	yylval.id = key ;
	return ID ;
      } else if (sym -> scope == keywords_scope) {
	//printf("%s keyword \n", key);
	return  * ((int *) sym -> val);
      } else {
	//printf("%s old id\n", key);
	yylval.id = key ;
	return ID ;
      }
 
      break;
    }
  }
}
*/
