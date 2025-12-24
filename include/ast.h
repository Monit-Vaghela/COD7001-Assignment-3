#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_BLOCK,

    AST_VAR_DECL,
    AST_ASSIGN,
    AST_IF,
    AST_IF_ELSE,

    AST_WHILE,

    AST_BINOP,
    AST_INT,
    AST_IDENT
} ast_node_type;


typedef struct ast_node {
    ast_node_type type;

    struct ast_node *left;
    struct ast_node *right;

    // To indicate a block statement
    // such as loops, if, while etc
    struct ast_node *extra;


    int int_value;
    int lineno;       
    char *name;          
    char op;            

} ast_node;


ast_node *ast_make_int(int value, int line);
ast_node *ast_make_ident(char *name, int line);
ast_node *ast_make_binop(char op, ast_node *left, ast_node *right, int line);

ast_node *ast_make_assign(char *name, ast_node *expr, int line);
ast_node *ast_make_var_decl(char *name, ast_node *expr, int line);

ast_node *ast_make_if(ast_node *cond, ast_node *then_block, int line);
ast_node *ast_make_if_else(ast_node *cond, ast_node *then_block, ast_node *else_block, int line);

ast_node *ast_make_while(ast_node *cond, ast_node *body, int line);

ast_node *ast_make_block(ast_node *stmt_list, int line);
ast_node *ast_append_statement(ast_node *list, ast_node *stmt);


void ast_print(ast_node *node, int indent);
void ast_free(ast_node *node);

#endif /* AST_H */