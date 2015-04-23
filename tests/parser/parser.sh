#!/bin/bash
EXE=tests/parser_coverage
for CFG in tests/parser/*.cfg ./; do
	${EXE} ${CFG} > /dev/null
done
OLDUMASK=$(umask)
umask -- -r
TEMPFILE=$(mktemp)
umask ${OLDUMASK}
${EXE} ${TEMPFILE}
gcov *.c
