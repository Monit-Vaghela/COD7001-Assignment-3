#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static symbol_t *symtab_head;
static int curr_scope;

void symtab_init()
{
    symtab_head = NULL; 
    curr_scope = 0;
}

void symtab_cleanup()
{
    while(symtab_head!=NULL)
    {
        symbol_t *after = symtab_head->next;
        free(symtab_head->name);
        free(symtab_head);
        symtab_head = after;
    }
    curr_scope = 0;
}


void enter_scope()
{
    curr_scope = curr_scope + 1;
}
void exit_scope()
{
    while(symtab_head!=NULL && symtab_head->scope == curr_scope)
    {
        symbol_t *after = symtab_head->next;
        free(symtab_head->name);
        free(symtab_head);
        symtab_head = after;
    }
    if(curr_scope)
    {
        curr_scope = curr_scope - 1;
    }
}


int symtab_insert(const char *name, int value)
{
    symbol_t *symbol = lookup_symbol(name);
    if(symbol && symbol->scope == curr_scope)
    {
        printf("Variable already exists\n");
        return -1;
    }
    symbol_t *new_symbol = malloc(sizeof(symbol_t));
    
    if (!new_symbol)   // if malloc return NULL
    {
        perror("malloc");
        return -1;
    }
    new_symbol->next = NULL;
    
    new_symbol->name = strdup(name);
    new_symbol->value = value;
    new_symbol->scope = curr_scope;

    new_symbol->next = symtab_head;
    symtab_head = new_symbol;
    
    return 0;
}


int symtab_update(const char *name, int value)
{
    symbol_t *temp = lookup_symbol(name);
    if(!temp)
    {
        printf("Variable not declared\n");
        return -1;
    }  
    temp->value = value;
    return 0;
}


void symtab_print()
{
    printf("SYMBOL TABLE\n");
    symbol_t *temp = symtab_head;
    while(temp)
    {
        printf("Symbol name = %s\n", temp->name);
        printf("Symbol value = %d\n", temp->value);
        printf("Symbol scope = %d\n", temp->scope);
        temp = temp->next;
    }
}


symbol_t *lookup_symbol(const char *name)
{
    symbol_t *temp = symtab_head;
    while(temp)
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;   // nearest scope matches
        }
        temp = temp->next;
    }
    return NULL;  // Symbol not found
}

symbol_t *get_symbol_table(void)
{
    return symtab_head;
}

int get_current_scope(void){
    return curr_scope;
}