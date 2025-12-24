#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"

#define MAX_VARS 128

typedef struct {
    char *name;
    int value;
} runtime_var;

static runtime_var vars[MAX_VARS];
static int var_count = 0;

static int lookup_runtime(const char *name)
{
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    }

    fprintf(stderr, "Runtime error: variable '%s' not initialized\n", name);
    exit(1);
}

static void update_runtime(const char *name, int value)
{
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name, name) == 0)
        {
            vars[i].value = value;
            return;
        }
    }

    vars[var_count].name = strdup(name);
    vars[var_count].value = value;
    var_count++;
}

int eval_ast(ast_node *node)
{
    if (!node)
        return 0;

    switch (node->type)
    {
        case AST_INT:
            return node->int_value;

        case AST_IDENT:
            return lookup_runtime(node->name);

        case AST_BINOP:
        {
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
                        fprintf(stderr, "Runtime error: division by zero\n");
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

            fprintf(stderr, "Runtime error: unknown operator\n");
            exit(1);
        }

        case AST_VAR_DECL:
        {
            int value = 0;
            if (node->left)
                value = eval_ast(node->left);

            update_runtime(node->name, value);
            return value;
        }

        case AST_ASSIGN:
        {
            int value = eval_ast(node->left);
            update_runtime(node->name, value);
            return value;
        }

        case AST_BLOCK:
        {
            ast_node *stmt = node->left;
            while (stmt)
            {
                eval_ast(stmt);

                /* 🚨 STOP if this is a control-flow node */
                if (stmt->type == AST_IF ||
                    stmt->type == AST_IF_ELSE ||
                    stmt->type == AST_WHILE)
                    break;

                stmt = stmt->right;
            }
            return 0;
        }

        case AST_IF:
        {
            int cond = eval_ast(node->left);

            if (cond)
                eval_ast(node->right);

            return 0;
        }

        case AST_IF_ELSE:
        {
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
            fprintf(stderr, "Runtime error: unknown AST node\n");
            exit(1);
    }
}

void eval_print_vars(void)
{
    printf("\n--- Runtime Values ---\n");

    if (var_count == 0)
    {
        printf("(no variables)\n");
        return;
    }

    for (int i = 0; i < var_count; i++)
    {
        printf("%s = %d\n", vars[i].name, vars[i].value);
    }
}
