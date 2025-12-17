%option noyywrap

%{
#include <stdlib.h>
#include "parser.tab.h"
%}

%%
[0-9]+  { yylval = atoi(yytext); return INTEGER; }
[ \t\n]+ ;
.        ;
%%
