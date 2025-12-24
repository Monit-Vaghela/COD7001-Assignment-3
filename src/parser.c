#include <stdio.h>
#include "parser.h"
#include "ast.h"
#include "eval.h"

/* from parser.y */
extern ast_node *ast_root;
extern int semantic_error;
extern int parse_error;

int yyparse(void);

void parser_init(void)
{
    semantic_error = 0;
    parse_error = 0;
    ast_root = NULL;
}

int parser_run(void)
{
    int ret = yyparse();

    /* Print AST only if parsing fully succeeded */
    if (ret == 0 && !semantic_error && !parse_error)
    {
        ast_print(ast_root, 0);
    }

    if (!parse_error && !semantic_error) {
        eval_ast(ast_root);
        eval_print_vars();
    }
    
    /* Always free AST if it was built */
    if (ast_root)
    {
        ast_free(ast_root);
        ast_root = NULL;
    }

    /* Non-zero return means failure */
    if (ret != 0 || semantic_error || parse_error)
        return 1;


    return 0;
}

void parser_cleanup(void)
{
    /* nothing extra needed for now */
}