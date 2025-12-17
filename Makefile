CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

all: parser

parser:
	flex lexer/lexer.l
	bison -d parser/parser.y
	$(CC) $(CFLAGS) \
	    src/*.c lex.yy.c parser.tab.c \
	    -o parser

clean:
	rm -f parser lex.yy.c parser.tab.c parser.tab.h
