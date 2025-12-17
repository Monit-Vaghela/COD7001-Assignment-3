CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGET = parser

SRC = \
    src/main.c \
    src/parser_driver.c \
    src/parser.c \
    src/lexer.c \
    src/ast.c \
    src/symbol_table.c

LEX_SRC  = lexer/lexer.l
YACC_SRC = parser/parser.y

all: $(TARGET)

$(TARGET): parser.tab.c lex.yy.c $(SRC)
	$(CC) $(CFLAGS) $(SRC) parser.tab.c lex.yy.c -o $(TARGET)

parser.tab.c parser.tab.h: $(YACC_SRC)
	bison -d $(YACC_SRC)

lex.yy.c: $(LEX_SRC) parser.tab.h
	flex $(LEX_SRC)

clean:
	rm -f $(TARGET) lex.yy.c parser.tab.c parser.tab.h

.PHONY: all clean
