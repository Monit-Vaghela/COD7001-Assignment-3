#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"
#include "symbol_table.h"

#define MAX_VARS 128

static symbol_t runtime_vars[MAX_VARS];
static int var_count = 0;


void push_runtime_vars() {
    /*
    This function is used to push the variables created at 
    runtime to a local pointer
    */

    symbol_t *current = get_symbol_table();
    
    while (current != NULL) {
        int curr_scope = get_current_scope();
        if (current->scope == curr_scope) { 
            // Only push variables from the current scope
            // Add variable to the runtime_vars array
            if (var_count < MAX_VARS) {
                runtime_vars[var_count].name = strdup(current->name);
                runtime_vars[var_count].value = current->value;
                runtime_vars[var_count].scope = current->scope;
                var_count++;
            }
            else {
                fprintf(stderr, "Error: Runtime variables array is full!\n");
                exit(1);
            }
        }
        current = current->next;
    }
}

int eval_ast(ast_node *node) {
    if (!node) {
        fprintf(stderr, "Internal error: NULL AST node\n");
        exit(1);
    }

    switch (node->type) {
        case AST_INT:{
            // printf("INTEGER -> Name %s, Value %d\n", node->name, node->int_value);
            return node->int_value;
        }

        case AST_IDENT: {
            symbol_t *sym = lookup_symbol(node->name);
            if (!sym)
            {
                fprintf(stderr, "Runtime error: variable '%s' not found at line %d\n", node->name, node->lineno);
                exit(1);
            }
            // printf("IDENTIFIER-> Name %s, Value %d\n", node->name, node->int_value);
            return sym->value;
        }

        case AST_BINOP: {
            int lhs = eval_ast(node->left);
            int rhs = eval_ast(node->right);

            switch (node->op)
            {
                case '+': return lhs + rhs;
                case '-': return lhs - rhs;
                case '*': return lhs * rhs;

                case '/':
                    if (rhs == 0)
                    {
                        fprintf(stderr, "Runtime error: division by zero at line %d\n", node->lineno);
                        exit(1);
                    }
                    return lhs / rhs;

                case '<': return lhs < rhs;
                case '>': return lhs > rhs;
                case 'L': return lhs <= rhs;
                case 'G': return lhs >= rhs;
                case 'E': return lhs == rhs;
                case 'N': return lhs != rhs;
            }

            fprintf(stderr, "Runtime error: unknown operator at line %d\n", node->lineno);
            exit(1);
        }

        case AST_VAR_DECL: {
            int value = 0;
            if (node->left)
                value = eval_ast(node->left);

            if (symtab_insert(node->name, value) != 0)
            {
                fprintf(stderr, "Runtime error: redeclaration of '%s' at line %d\n", node->name, node->lineno);
                exit(1);
            }
            // printf("DECLARATION -> Name %s, Value %d\n", node->name, node->int_value);
            return value;
        }

        case AST_ASSIGN: {
            int value = eval_ast(node->left);

            if (symtab_update(node->name, value) != 0)
            {
                fprintf(stderr, "Runtime error: assignment to undeclared variable '%s' at line %d\n", node->name, node->lineno);
                exit(1);
            }
            
            // printf("ASSIGN -> Name %s, Value %d\n", node->name, node->int_value);
            return value;
        }

        case AST_BLOCK: {
            enter_scope();
            ast_node *stmt = node->left;
            while (stmt)
            {
                eval_ast(stmt);
                if (stmt->type == AST_IF ||
                    stmt->type == AST_IF_ELSE ||
                    stmt->type == AST_WHILE)
                    break;

                stmt = stmt->right;
            }
            // before exiting this scope we stored every variable
            // created in this block
            push_runtime_vars();
            exit_scope();
            return 0;
        }

        case AST_IF: {
            int cond = eval_ast(node->left);

            if (cond)
                eval_ast(node->right);

            return 0;
        }

        case AST_IF_ELSE: {
            int cond = eval_ast(node->left);

            if (cond)
                eval_ast(node->right);
            else
                eval_ast(node->extra);

            return 0;
        }

        case AST_WHILE:
            fprintf(stderr, "Runtime error: control flow not implemented yet\n");
            exit(1);

        default:
            fprintf(stderr, "Runtime error: unknown AST node at line %d\n", node->lineno);
            exit(1);
    }
}

void eval_print_vars(void) {
    printf("\n--- Runtime Values ---\n");

    if (var_count == 0)
    {
        printf("(no variables)\n");
        return;
    }

    // Print all variables with indentation
    for (int i = 0; i < var_count; i++)
    {
        printf("Name: %s\n", runtime_vars[i].name);  // Indented Name
        printf("Value: %d\n", runtime_vars[i].value);  // Indented Value
        printf("Scope: %d\n\n", runtime_vars[i].scope);  // Indented Scope
    }
}


