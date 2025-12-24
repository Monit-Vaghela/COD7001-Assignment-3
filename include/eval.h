#ifndef EVAL_H
#define EVAL_H

#include "ast.h"

int eval_ast(ast_node *node);
void eval_print_vars(void);

#endif
