#ifndef UTILS_H
#define UTILS_H

// Lexer Functions
int lexer_init(const char *filename);
void lexer_cleanup(void);

// Parser Functions
int parse_file(const char *filename);
void parser_init(void);
int  parser_run(void);
void parser_cleanup(void);

#endif