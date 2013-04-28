#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <stdio.h>
#include "string_buffer.h"

#define BUFFER_SIZE 4096
#define CHARACTER_SIZE 1

typedef struct {
  char *filename;
  FILE *input;
  int line_num;
  int eof;
  int current_buffer_index;
  int pre;
  char* buffers [2];
  StringBuffer *string_buffer ;
} Scanner ;

extern Scanner *scanner_create () ;
extern int scanner_init (Scanner *scanner, const char *filename, FILE *file) ;
extern void scanner_del (Scanner *scanner);
extern int scanner_next_char (Scanner *scanner);
extern int yylex () ;

#define isIdent(c) (isalpha((c)) || isdigit((c)) || c == '_') 

#endif /* _SCANNER_H_ */
