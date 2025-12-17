#include "parser.h"

int yyparse(void);

void parser_init(void) {
    /* future: init AST, symbol table */
}

int parser_run(void) {
    return yyparse();
}

void parser_cleanup(void) {
    /* future: free AST, symbol table */
}
