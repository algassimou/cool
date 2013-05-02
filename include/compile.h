#ifndef _COMPILE_H_
#define _COMPILE_H_

#include "string_buffer.h"
#include "scanner.h"

#define YYLEX_PARAM scanner
extern Scanner *scanner;

#include "symbole.h"
extern SymboleTable *tableSymbole ;
extern void * keywords_scope ;
extern void * types_scope ;

#include "parser.h"

extern void add_symbole (char *key);
extern void compile_panic (char *key);

#include "error.h"
#define print_error(msg) (compile_error((scanner -> filename), (scanner -> line_num), (msg)))

#endif /* _COMPILE_H_ */
