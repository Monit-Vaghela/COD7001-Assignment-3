#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

void symtab_init(void);
void symtab_insert(const char *name);
void symtab_cleanup(void);

#endif
