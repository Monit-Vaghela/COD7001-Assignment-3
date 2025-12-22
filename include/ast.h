#ifndef AST_H
#define AST_H

typedef enum {
    AST_PROGRAM,
    AST_BLOCK,

    /* Statements */
    AST_VAR_DECL,
    AST_ASSIGN,
    AST_IF,
    AST_IF_ELSE,
    AST_WHILE,

    /* Expressions */
    AST_BINOP,
    AST_INT,
    AST_IDENT
} ast_node_type;

/* =========================
   AST Node Structure
   ========================= */
typedef struct ast_node {
    ast_node_type type;

    /* Common children */
    struct ast_node *left;
    struct ast_node *right;

    /* Extra pointer (for else block, statement lists, etc.) */
    struct ast_node *extra;

    /* Data */
    int int_value;       /* for AST_INT */
    char *name;          /* for AST_IDENT */
    char op;             /* for AST_BINOP (+ - * / etc.) */

} ast_node;

/* =========================
   AST Construction Helpers
   ========================= */
ast_node *ast_make_int(int value);
ast_node *ast_make_ident(char *name);
ast_node *ast_make_binop(char op, ast_node *left, ast_node *right);

ast_node *ast_make_assign(char *name, ast_node *expr);
ast_node *ast_make_var_decl(char *name, ast_node *expr);

ast_node *ast_make_if(ast_node *cond, ast_node *then_block);
ast_node *ast_make_if_else(ast_node *cond,
                           ast_node *then_block,
                           ast_node *else_block);

ast_node *ast_make_while(ast_node *cond, ast_node *body);

ast_node *ast_make_block(ast_node *stmt_list);
ast_node *ast_append_statement(ast_node *list, ast_node *stmt);

/* =========================
   AST Utilities
   ========================= */
void ast_print(ast_node *node, int indent);
void ast_free(ast_node *node);

#endif /* AST_H */
