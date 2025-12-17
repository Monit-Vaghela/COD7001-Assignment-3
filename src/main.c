#include <stdio.h>
#include "parser_driver.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    parse_file(argv[1]);
    return 0;
}
