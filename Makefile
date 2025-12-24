CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Iparser

TARGET = myparser

SRC = \
    src/main.c \
    src/ast.c \
    src/symbol_table.c \
    src/eval.c

LEX_SRC  = lexer/lexer.l
YACC_SRC = parser/parser.y

all: $(TARGET)

$(TARGET): parser/parser.tab.c lex.yy.c $(SRC)
	$(CC) $(CFLAGS) $(SRC) parser/parser.tab.c lex.yy.c -o $(TARGET)

parser/parser.tab.c parser/parser.tab.h: $(YACC_SRC)
	bison -d -o parser/parser.tab.c $(YACC_SRC)

lex.yy.c: $(LEX_SRC) parser/parser.tab.h
	flex $(LEX_SRC)

clean:
	rm -f $(TARGET) lex.yy.c parser/parser.tab.c parser/parser.tab.h

.PHONY: all clean