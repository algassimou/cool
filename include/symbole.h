#ifndef __TABLE_SYMBOLE__
#define __TABLE_SYMBOLE__

#include "hachage.h"
#include "liste.h"

typedef struct {
  char *id;
  void *val;
  void *scope;

  void (*val_del) (void *);
} Symbol ;

// life cycle
extern Symbol *symbol_create ();
extern int symbol_init (Symbol *s, void (*val_del) (void *)) ;
extern void symbol_del (void *s);

typedef struct {
  HashTable *table ;
  Liste *scopes ; 
} SymboleTable  ;

// life cycle
extern SymboleTable *symbole_table_create ();
extern int symbole_table_init (SymboleTable *st);
extern void symbole_table_del (SymboleTable *st);

// scope's gestion
extern int st_create_scope(SymboleTable *st); 
extern int st_end_scope(SymboleTable *st); 
extern int st_check_scope (SymboleTable *st, char *s);
extern void *st_current_scope (SymboleTable *st);

// symbole operations
extern int st_add_symbol (SymboleTable *st, Symbol *s);
extern Symbol *st_find_symbol (SymboleTable *st, char *id);

// operations

/*extern int symbole_table_insert (SymboleTable *st, void *sym, void *val);
extern Couple *symbole_table_search (SymboleTable *st, void *sym);
//extern int symbole_table_rm (SymboleTable *st, void *sym);
extern int symbole_table_add_block (SymboleTable *st);
extern int symbole_table_rm_block (SymboleTable *st);
*/

#endif /* __TABLE_SYMBOLE__ */
