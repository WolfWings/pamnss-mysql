#CFLAGS=-I. -Os -march=native
#LDFLAGS=-s
CFLAGS=-I. -O0 -Wall -pedantic -fprofile-arcs -ftest-coverage
LDFLAGS=

default: tests/parser_coverage

tests/parser_coverage: parser.o config.o tests/parser_coverage.c

parser.c: config.h

config.c: config.h

clean:
	$(RM) *.o *.gcda *.gcno *.gcov tests/*_coverage
