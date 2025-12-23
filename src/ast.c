#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* =========================
   Internal helper
   ========================= */
static ast_node *ast_new_node(ast_node_type type)
{
    ast_node *node = malloc(sizeof(ast_node));
    if (!node)
    {
        perror("malloc");
        exit(1);
    }

    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->extra = NULL;
    node->int_value = 0;
    node->name = NULL;
    node->op = 0;

    return node;
}

/* =========================
   Expression nodes
   ========================= */
ast_node *ast_make_int(int value)
{
    ast_node *node = ast_new_node(AST_INT);
    node->int_value = value;
    return node;
}

ast_node *ast_make_ident(char *name)
{
    ast_node *node = ast_new_node(AST_IDENT);
    node->name = strdup(name);
    return node;
}

ast_node *ast_make_binop(char op, ast_node *left, ast_node *right)
{
    ast_node *node = ast_new_node(AST_BINOP);
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

/* =========================
   Statement nodes
   ========================= */
ast_node *ast_make_assign(char *name, ast_node *expr)
{
    ast_node *node = ast_new_node(AST_ASSIGN);
    node->name = strdup(name);
    node->left = expr;
    return node;
}

ast_node *ast_make_var_decl(char *name, ast_node *expr)
{
    ast_node *node = ast_new_node(AST_VAR_DECL);
    node->name = strdup(name);
    node->left = expr;   /* may be NULL if no initializer */
    return node;
}

ast_node *ast_make_if(ast_node *cond, ast_node *then_block)
{
    ast_node *node = ast_new_node(AST_IF);
    node->left = cond;
    node->right = then_block;
    return node;
}

ast_node *ast_make_if_else(ast_node *cond,
                           ast_node *then_block,
                           ast_node *else_block)
{
    ast_node *node = ast_new_node(AST_IF_ELSE);
    node->left = cond;
    node->right = then_block;
    node->extra = else_block;
    return node;
}

ast_node *ast_make_while(ast_node *cond, ast_node *body)
{
    ast_node *node = ast_new_node(AST_WHILE);
    node->left = cond;
    node->right = body;
    return node;
}

/* =========================
   Block & statement list
   ========================= */
ast_node *ast_make_block(ast_node *stmt_list)
{
    ast_node *node = ast_new_node(AST_BLOCK);
    node->left = stmt_list;
    return node;
}

ast_node *ast_append_statement(ast_node *list, ast_node *stmt)
{
    if (!stmt)
        return list;   /* ignore empty statements */

    if (!list)
        return stmt;

    ast_node *temp = list;
    while (temp->right)
        temp = temp->right;

    temp->right = stmt;
    return list;
}

/* =========================
   AST Printing (for debugging)
   ========================= */
static void print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
        printf("  ");
}

void ast_print(ast_node *node, int indent)
{
    if (!node)
        return;

    print_indent(indent);

    switch (node->type)
    {
        case AST_INT:
            printf("INT(%d)\n", node->int_value);
            break;

        case AST_IDENT:
            printf("IDENT(%s)\n", node->name);
            break;

        case AST_BINOP:
            printf("BINOP(%c)\n", node->op);
            ast_print(node->left, indent + 1);
            ast_print(node->right, indent + 1);
            break;

        case AST_ASSIGN:
            printf("ASSIGN(%s)\n", node->name);
            ast_print(node->left, indent + 1);
            break;

        case AST_VAR_DECL:
            printf("VAR_DECL(%s)\n", node->name);
            if (node->left)
                ast_print(node->left, indent + 1);
            break;

        case AST_IF:
            printf("IF\n");
            ast_print(node->left, indent + 1);
            ast_print(node->right, indent + 1);
            break;

        case AST_IF_ELSE:
            printf("IF_ELSE\n");
            ast_print(node->left, indent + 1);
            ast_print(node->right, indent + 1);
            ast_print(node->extra, indent + 1);
            break;

        case AST_WHILE:
            printf("WHILE\n");
            ast_print(node->left, indent + 1);
            ast_print(node->right, indent + 1);
            break;

        case AST_BLOCK:
            printf("BLOCK\n");
            ast_print(node->left, indent + 1);
            break;

        default:
            printf("UNKNOWN NODE\n");
    }

/* Traverse statement list ONLY for simple statements */
if ((node->type == AST_VAR_DECL ||
     node->type == AST_ASSIGN) &&
    node->right)
{
    ast_print(node->right, indent);
}



}

/* =========================
   AST Freeing
   ========================= */
void ast_free(ast_node *node)
{
    if (!node)
        return;

    ast_free(node->left);
    ast_free(node->right);
    ast_free(node->extra);

    if (node->name)
        free(node->name);

    free(node);
}
