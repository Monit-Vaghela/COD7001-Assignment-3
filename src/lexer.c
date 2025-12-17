#include <stdio.h>
#include "lexer.h"

extern FILE *yyin;

int lexer_init(const char *filename) {
    yyin = fopen(filename, "r");
    return yyin ? 0 : 1;
}

void lexer_cleanup(void) {
    if (yyin) fclose(yyin);
}
