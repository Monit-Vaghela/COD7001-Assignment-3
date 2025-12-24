
#include <stdio.h>
#include "utils.h"
#include "ast.h"
#include "symbol_table.h"
#include "eval.h"

extern FILE *yyin;
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
        symtab_cleanup();
        symtab_init();
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

int lexer_init(const char *filename) {
    yyin = fopen(filename, "r");
    return yyin ? 0 : 1;
}

void lexer_cleanup(void) {
    if (yyin) fclose(yyin);
}

int parse_file(const char *filename) {
    if (lexer_init(filename) != 0) {
        return 1;
    }

    parser_init();

    if (parser_run() != 0) {
        return 1;
    }

    parser_cleanup();
    lexer_cleanup();

    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    symtab_init();

    if (parse_file(argv[1]) != 0) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }
    symtab_cleanup();  

    return 0;
}