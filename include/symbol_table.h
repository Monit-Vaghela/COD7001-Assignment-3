#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct symbol {
    char *name;              // variable name 
    int value;               // integer value 
    int scope;               // scope level 
    struct symbol *next;     // next symbol in list 
} symbol_t;

void symtab_init();
void symtab_cleanup();
void enter_scope();
void exit_scope();
int symtab_insert(const char *name, int value);
int symtab_update(const char *name, int value);
void symtab_print();
symbol_t *lookup_symbol(const char *name);

#endif
