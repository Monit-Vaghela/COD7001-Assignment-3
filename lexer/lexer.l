%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.tab.h"

extern int yylineno;
extern int lexical_error;

%}

%option noyywrap
%option yylineno

DIGIT [0-9]
LETTER [a-zA-Z_]
IDENTIFIER {LETTER}({LETTER}|{DIGIT})*
INTEGER {DIGIT}+
WHITESPACE [ \t\r\n]+

%%

"var" { return VAR; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }

"==" { return EQ; }
"!=" { return NE; }
"<=" { return LE; }
">=" { return GE; }

"=" { return ASSIGN; }
"<" { return LT; }
">" { return GT; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MUL; }
"/" { return DIV; }

";" { return SEMI; }
"(" { return LPAREN; }
")" { return RPAREN; }
"{" { return LBRACE; }
"}" { return RBRACE; }

{INTEGER} {
    yylval.ival = atoi(yytext);
    return INTEGER;
}

{IDENTIFIER} {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

{WHITESPACE} { /* ignore */ }

"//".*\n { /* ignore single-line comments */ }

"/*"([^*]|(\*+[^*/]))*\*+"/" { /* ignore multi-line comments */ }

. {
    printf("Lexical error at line %d: unknown character '%s'\n", yylineno, yytext);
    exit(1);
}

%%