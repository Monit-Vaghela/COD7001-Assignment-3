#include <stdio.h>
#include "parser_driver.h"
#include "lexer.h"
#include "parser.h"

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
