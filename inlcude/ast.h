#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_STATEMENT,
    AST_EXPRESSION
} ast_node_type_t;

typedef struct ast_node {
    ast_node_type_t type;
    struct ast_node *left;
    struct ast_node *right;
    void *data;
} ast_node_t;

ast_node_t *ast_create_node(ast_node_type_t type);
void ast_free(ast_node_t *node);

#endif
