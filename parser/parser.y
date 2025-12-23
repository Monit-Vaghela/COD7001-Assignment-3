%{
/* =========================
   C Declarations
   ========================= */
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

/* Forward declaration */
struct ast_node;

int semantic_error = 0;
int parse_error = 0;

ast_node *ast_root = NULL;

/* Interface to lexer */
extern int yylex(void);
extern int yylineno;

/* Error handler */
void yyerror(const char *s);
%}

%define parse.error detailed


/* =========================
   Semantic Value Union
   ========================= */
%union {
    int ival;                /* integer literals */
    char *sval;              /* identifiers */
    struct ast_node *node;   /* AST nodes - use 'struct' keyword */
}

/* =========================
   Token Declarations
   ========================= */

/* Keywords */
%token VAR IF ELSE WHILE

/* Identifiers and literals */
%token <sval> IDENTIFIER
%token <ival> INTEGER

/* Operators */
%token PLUS MINUS MUL DIV
%token ASSIGN
%token EQ NE LT GT LE GE

/* Punctuation */
%token LPAREN RPAREN
%token LBRACE RBRACE
%token SEMI

/* =========================
   Non-terminal Types
   ========================= */
%type <node> program
%type <node> stmt stmt_list block
%type <node> expr

/* =========================
   Operator Precedence
   ========================= */
%left PLUS MINUS
%left MUL DIV
%nonassoc LT GT LE GE EQ NE

/* =========================
   Start Symbol
   ========================= */
%start program

%%
/* =========================
   Program
   ========================= */
program
    : stmt_list
      {
          ast_root = ast_make_block($1);   /* implicit global block */
      }
      ;

/* =========================
   Statement List
   ========================= */
stmt_list
    : stmt
      {
          $$ = $1;
      }
    | stmt_list stmt
      {
          $$ = ast_append_statement($1, $2);
      }
    ;

/* =========================
   Statements
   ========================= */
stmt
    : block
      {
          $$ = $1;
      }
    | SEMI
      {
          $$ = NULL;
      }

    /* Variable declaration */
    | VAR IDENTIFIER SEMI
      {
          if (symtab_insert($2, 0) != 0)
          {
              fprintf(stderr,
                      "Error at line %d: redeclaration of variable '%s'\n",
                      yylineno, $2);
              semantic_error = 1;
          }
          $$ = ast_make_var_decl($2, NULL);
      }

    | VAR IDENTIFIER ASSIGN expr SEMI
      {
          if (symtab_insert($2, 0) != 0)
              fprintf(stderr,
                      "Error at line %d: redeclaration of variable '%s'\n",
                      yylineno, $2);
          $$ = ast_make_var_decl($2, $4);
      }

    /* Assignment */
    | IDENTIFIER ASSIGN expr SEMI
      {
          if (symtab_update($1, 0) != 0)
          {
              fprintf(stderr,
                      "Error at line %d: assignment to undeclared variable '%s'\n",
                      yylineno, $1);
              semantic_error = 1;
          }
          $$ = ast_make_assign($1, $3);
      }

    /* If */
    | IF LPAREN expr RPAREN stmt
      {
          $$ = ast_make_if($3, $5);
      }

    /* If-Else */
    | IF LPAREN expr RPAREN stmt ELSE stmt
      {
          $$ = ast_make_if_else($3, $5, $7);
      }

    /* While */
    | WHILE LPAREN expr RPAREN stmt
      {
          $$ = ast_make_while($3, $5);
      }
    ;

/* =========================
   Block (Scope)
   ========================= */
block
    : LBRACE
      {
          enter_scope();
      }
      stmt_list
      RBRACE
      {
          exit_scope();
          $$ = ast_make_block($3);
      }
    ;

/* =========================
   Expressions
   ========================= */
expr
    : INTEGER
      {
          $$ = ast_make_int($1);
      }
    | IDENTIFIER
      {
          if (!lookup_symbol($1))
          {
              fprintf(stderr,
                      "Error at line %d: variable '%s' not declared\n",
                      yylineno, $1);
              semantic_error = 1;
          }
          $$ = ast_make_ident($1);
      }
    | expr PLUS expr
      {
          $$ = ast_make_binop('+', $1, $3);
      }
    | expr MINUS expr
      {
          $$ = ast_make_binop('-', $1, $3);
      }
    | expr MUL expr
      {
          $$ = ast_make_binop('*', $1, $3);
      }
    | expr DIV expr
      {
          $$ = ast_make_binop('/', $1, $3);
      }
    | expr LT expr
      {
          $$ = ast_make_binop('<', $1, $3);
      }
    | expr GT expr
      {
          $$ = ast_make_binop('>', $1, $3);
      }
    | expr LE expr
      {
          $$ = ast_make_binop('L', $1, $3); /* <= */
      }
    | expr GE expr
      {
          $$ = ast_make_binop('G', $1, $3); /* >= */
      }
    | expr EQ expr
      {
          $$ = ast_make_binop('E', $1, $3); /* == */
      }
    | expr NE expr
      {
          $$ = ast_make_binop('N', $1, $3); /* != */
      }
    | LPAREN expr RPAREN
      {
          $$ = $2;
      }
    ;

%%
/* =========================
   Error Handling
   ========================= */
void yyerror(const char *s)
{
    parse_error = 1;
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}