%{
#include <stdio.h>
#include "parser.h"

int yylex(void);
int yyerror(const char *s);
%}

%token INTEGER

%%
program:
    INTEGER
    ;
%%

int yyerror(const char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
    return 0;
}
