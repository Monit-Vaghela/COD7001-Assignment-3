%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

struct ast_node;

int semantic_error = 0;
int parse_error = 0;

ast_node *ast_root = NULL;
extern int yylex(void);
extern int yylineno;

void yyerror(const char *s);
%}

%define parse.error detailed

%union {
    int ival;               
    char *sval;             
    struct ast_node *node;  
}

// Keywords
%token VAR IF ELSE WHILE FOR DO

// Identifiers
%token <sval> IDENTIFIER

// literals
%token <ival> INTEGER

// Operators
%token PLUS MINUS MUL DIV
%token ASSIGN
%token EQ NE LT GT LE GE

// Other Syntax Stuff
%token LPAREN RPAREN
%token LBRACE RBRACE
%token SEMI

// Non terminal
%type <node> program
%type <node> stmt stmt_list block
%type <node> variable_decl assignment
%type <node> if_stmt while_stmt for_stmt do_while_stmt
%type <node> for_init for_update
%type <node> expression equality comparison term factor unary primary

%start program

%%

// program ::= statement_list ;
program:
    stmt_list
    {
        ast_root = ast_make_block($1);
    }
    ;

// statement_list ::= statement | epsilon;
stmt_list:
    /* empty */
    {
        $$ = NULL;
    }
    | stmt_list stmt
    {
        $$ = ast_append_statement($1, $2);
    }
    ;

// statement ::= variable_decl | assignment | if | while | for | do-while | block ;
stmt:
    variable_decl
    {
        $$ = $1;
    }
    | assignment
    {
        $$ = $1;
    }
    | if_stmt
    {
        $$ = $1;
    }
    | while_stmt
    {
        $$ = $1;
    }
    | for_stmt
    {
        $$ = $1;
    }
    | do_while_stmt
    {
        $$ = $1;
    }
    // as block is non terminal
    // we just assign what we get from block
    | block
    {
        $$ = $1;
    }
    // if it is just ';' then we just move on
    | SEMI
    {
        $$ = NULL;
    }
    ;

// block (anything enclosed in { })
block:
    LBRACE
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

/* variable_decl ::= VAR IDENTIFIER SEMI 
                   | VAR IDENTIFIER assignment SEMI */
variable_decl:
    // declaration only
    // var x;
    VAR IDENTIFIER SEMI
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

    // declararation and assignment
    // var x = 10 + 5;
    | VAR IDENTIFIER ASSIGN expression SEMI
    {
        if (symtab_insert($2, 0) != 0)
        {
            fprintf(stderr,
                    "Error at line %d: redeclaration of variable '%s'\n",
                    yylineno, $2);
            semantic_error = 1;
        }
        $$ = ast_make_var_decl($2, $4);
    }
    ;

// assignment only (must be declared before)
// x = 10 + 5;
assignment:
    IDENTIFIER ASSIGN expression SEMI
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
    ;

// IF only and IF with ELSE
if_stmt:
    // IF only
    IF LPAREN expression RPAREN stmt
    {
        $$ = ast_make_if($3, $5);
    }

    // IF with ELSE
    | IF LPAREN expression RPAREN stmt ELSE stmt
    {
        $$ = ast_make_if_else($3, $5, $7);
    }
    ;

// While
while_stmt:
    WHILE LPAREN expression RPAREN stmt
    {
        $$ = ast_make_while($3, $5);
    }
    ;

// For loop
for_stmt:
    FOR LPAREN for_init expression for_update RPAREN stmt
    {
        $$ = ast_make_for($3, $4, $5, $7);
    }
    ;

for_init:
    variable_decl
    {
        $$ = $1;
    }
    | assignment
    {
        $$ = $1;
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

for_update:
    assignment
    {
        $$ = $1;
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

// Do while loop
do_while_stmt:
    DO stmt WHILE LPAREN expression RPAREN SEMI
    {
        $$ = ast_make_do_while($2, $5);
    }
    ;

// expression ::= equality ;
expression:
    equality
    {
        $$ = $1;
    }
    ;

/* equality ::= comparison ;
              | equality EQ comparison ;
              | equality NE comparison;
*/
equality:
    comparison
    {
        $$ = $1;
    }
    | equality EQ comparison
    {
        $$ = ast_make_binop('E', $1, $3);
    }
    | equality NE comparison
    {
        $$ = ast_make_binop('N', $1, $3);
    }
    ;

comparison:
    term
    {
        $$ = $1;
    }
    | comparison LT term
    {
        $$ = ast_make_binop('<', $1, $3);
    }
    | comparison GT term
    {
        $$ = ast_make_binop('>', $1, $3);
    }
    | comparison LE term
    {
        $$ = ast_make_binop('L', $1, $3); /* <= */
    }
    | comparison GE term
    {
        $$ = ast_make_binop('G', $1, $3); /* >= */
    }
    ;

term:
    factor
    {
        $$ = $1;
    }
    | term PLUS factor
    {
        $$ = ast_make_binop('+', $1, $3);
    }
    | term MINUS factor
    {
        $$ = ast_make_binop('-', $1, $3);
    }
    ;

factor:
    unary
    {
        $$ = $1;
    }
    | factor MUL unary
    {
        $$ = ast_make_binop('*', $1, $3);
    }
    | factor DIV unary
    {
        $$ = ast_make_binop('/', $1, $3);
    }
    ;

unary:
    PLUS unary
    {
        $$ = $2;
    }
    | MINUS unary
    {
        $$ = ast_make_binop('-', ast_make_int(0), $2);
    }
    | primary
    {
        $$ = $1;
    }
    ;

primary:
    // integer Only (Primary)
    INTEGER
    {
        $$ = ast_make_int($1);
    }

    // Identifier only (Primary)
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

    // taking care of opening and closing of
    // parenthesis
    | LPAREN expression RPAREN
    {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s)
{
    parse_error = 1;
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}
