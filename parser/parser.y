%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
int yyerror(const char *s);
%}

/* Define semantic values */
%union {
    int intval;
    char *strval;
}

/* Tokens */
%token VAR IF ELSE WHILE
%token EQ NE LE GE
%token ASSIGN LT GT
%token PLUS MINUS MUL DIV
%token SEMI LPAREN RPAREN LBRACE RBRACE

%token <intval> INTEGER
%token <strval> IDENTIFIER

%%

program:
    INTEGER
  | IDENTIFIER
  ;

%%

int yyerror(const char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
    return 0;
}
