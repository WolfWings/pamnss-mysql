#!/bin/bash
EXE=tests/parser_coverage
for CFG in tests/parser/*.cfg ./; do
	${EXE} ${CFG} 2>&1 > /dev/null
done
OLDUMASK=$(umask)
umask -- -r
TEMPFILE=$(mktemp)
umask ${OLDUMASK}
${EXE} ${TEMPFILE} 2>&1 > /dev/null
rm -f ${TEMPFILE}
gcov config.c parser.c
