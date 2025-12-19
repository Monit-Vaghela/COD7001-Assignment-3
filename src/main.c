#include <stdio.h>
#include "parser_driver.h"
#include "symbol_table.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    symtab_init();

    if (parse_file(argv[1]) != 0) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }

    symtab_cleanup();  

    return 0;
}
