#CFLAGS=-I. -Os -march=native
#LDFLAGS=-s
CFLAGS=-I. -O0 -Wall -pedantic -fprofile-arcs -ftest-coverage
LDFLAGS=

default: test_parser

test_parser: parser.o config.o test_parser.c

parser.c: config.h

config.c: config.h

clean:
	$(RM) *.o *.gcda *.gcno *.gcov test_parser
