#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


static ast_node *ast_create_empty_node(ast_node_type type) {
    /*
    Inputs : Type of ast node
    Output : returns a pointer to the new node created
    */

    // sizeof(ast_node) will ne size of all of its
    // components combined, and that much uninitiallized 
    // memory will be allocated to the code
    ast_node *node = malloc(sizeof(ast_node));
    if (!node)
    {
        perror("malloc");
        exit(1);
    }

    // We will initialize everyting to either
    // NULL or ZERO execpt type
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->extra = NULL;
    node->int_value = 0;
    node->name = NULL;
    node->op = 0;

    return node;
}

ast_node *ast_make_int(int value) {
    /*
    Input : The value of the integer
    Output : pointer to the new INT type AST node
    */
    ast_node *node = ast_create_empty_node(AST_INT);
    node->int_value = value;
    return node;
}

ast_node *ast_make_ident(char *name) {
    /*
    Input : The name of the identifier
    Output : pointer to the new IDENT type AST node
    */
    ast_node *node = ast_create_empty_node(AST_IDENT);
    node->name = strdup(name);
    return node;
}

ast_node *ast_make_binop(char op, ast_node *left, ast_node *right) {
    /*
    Inputs : 
        1) Binary operator which is used in this statement
        2) left AST node
        3) right AST node
    Output : pointer to the new BINOP type AST node
    */
    ast_node *node = ast_create_empty_node(AST_BINOP);
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ast_node *ast_make_assign(char *name, ast_node *expr) {
    /*
    Inputs : 
        1) name of the variable which this value is being assigned
        2) expression which has been assigned to the variable
    Output : pointer to the new ASSIGN type AST node
    */
    ast_node *node = ast_create_empty_node(AST_ASSIGN);
    node->name = strdup(name);
    // as assign only has a single child we choose the left 
    // child
    node->left = expr;
    return node;
}

ast_node *ast_make_var_decl(char *name, ast_node *expr) {
    /*
    Inputs : 
        1) name of the variable which this value is being assigned
        2) expression which has been assigned to the variable
    Output : pointer to the new VAR_DECL type AST node
    */
    ast_node *node = ast_create_empty_node(AST_VAR_DECL);
    node->name = strdup(name);

    // Can be Null
    /*
    example : var x; v/s var x = 10;
    */
    node->left = expr;  
    return node;
}

ast_node *ast_make_if(ast_node *cond, ast_node *then_block) {
    /*
    Inputs : 
        1) Condition for the IF block
        2) Pointer for the child AST i,e the then block (may or may not be NULL)
    Output : pointer to the new if type AST node
    */
    ast_node *node = ast_create_empty_node(AST_IF);
    node->left = cond;
    node->right = then_block;
    return node;
}

ast_node *ast_make_if_else(ast_node *cond, ast_node *then_block, ast_node *else_block) {
    /*
    Inputs : 
        1) Condition for the IF Block
        2) Pointer to the then block
        3) Poiner to else block.
    Output : pointer to the new IF_ELSE type AST node
    */
    ast_node *node = ast_create_empty_node(AST_IF_ELSE);
    node->left = cond;
    node->right = then_block;
    node->extra = else_block;
    return node;
}

ast_node *ast_make_while(ast_node *cond, ast_node *body) {
    /*
    Inputs : 
        1) Condition for the while
        2) Pointer to the body
    Output : pointer to the new WHILE type AST node
    */
    ast_node *node = ast_create_empty_node(AST_WHILE);
    node->left = cond;
    node->right = body;
    return node;
}

ast_node *ast_make_block(ast_node *stmt_list) {
    ast_node *node = ast_create_empty_node(AST_BLOCK);
    node->left = stmt_list;
    return node;
}

ast_node *ast_append_statement(ast_node *list, ast_node *stmt)
{
    if (!stmt)
        return list;

    if (!list)
        return stmt;

    ast_node *temp = list;

    while (temp->right)
    {
        if (temp->type == AST_IF ||
            temp->type == AST_IF_ELSE ||
            temp->type == AST_WHILE ||
            temp->type == AST_BLOCK)
            break;

        temp = temp->right;
    }

    temp->right = stmt;
    return list;
}

void ast_print(ast_node *node, int indent)
{
    if (!node)
        return;
    
    for (int i = 0; i < indent; i++) printf("  ");

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