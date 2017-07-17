#!/bin/bash
EXE=tests/parser_coverage

# Run through the itemized test files first
for CFG in tests/parser/*.cfg ./; do
	${EXE} ${CFG} 2>&1 > /dev/null
done

# Now create a file we can't read...
OLDUMASK=$(umask)
umask -- -r
TEMPFILE=$(mktemp)
umask ${OLDUMASK}
${EXE} ${TEMPFILE} 2>&1 > /dev/null
rm -f ${TEMPFILE}

# ...and test again for the missing option
${EXE} ${TEMPFILE} 2>&1 > /dev/null

# Output the GCOV counts.
gcov config.c parser.c
