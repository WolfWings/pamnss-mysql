# Modify these to change overall compiler options
#MY_CFLAGS=-Os -march=native
#MY_LDFLAGS=-s
MY_CFLAGS=-O0 -Wall -pedantic -fprofile-arcs -ftest-coverage
MY_LDFLAGS=
MY_LDLIBS=

# Build final "FLAGS" with required add-ons:
# MySQL includes and libraries
# Enforce "gnu99" C99 mode
# Include local directory
CFLAGS=$(MY_CFLAGS) $(shell mysql_config --cflags) -I. -std=gnu99
LDLIBS=$(MY_LDLIBS) $(shell mysql_config --libs)
LDFLAGS=$(MY_LDFLAGS)

default: libnss_mysql2017.so.2

tests: tests/parser_coverage

tests/parser_coverage: parser.o config.o tests/parser_coverage.c

parser.c: config.h

config.c: config.h

mysql.c: mysql.h

nss.c: nss.h

libnss_mysql2017.so.2: nss.c config.c parser.c mysql.c
	gcc -fPIC -shared -o libnss_mysql2017.so.2 -Wl,-soname,libnss_mysql2017.so.2 nss.c

clean:
	$(RM) *.o *.gcda *.gcno *.gcov tests/*_coverage *.so.*
