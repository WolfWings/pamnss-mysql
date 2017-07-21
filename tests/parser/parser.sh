#!/bin/bash
EXE=tests/parser_coverage
if [ -x $(which valgrind) ]; then
	EXE="$(which valgrind) --leak-check=yes ${EXE}"
fi

# Create a file we can't read...
OLDUMASK=$(umask)
umask -- -r
TEMPFILE=$(mktemp)
umask ${OLDUMASK}

# ...and a file that doesn't exist...
MISSINGFILE=$(mktemp)
rm -f ${MISSINGFILE}

# ...and run all the tests.
LD_PRELOAD=tests/preload_silence_syslog.so ${EXE} tests/parser/*.cfg ${TEMPFILE} ${MISSINGFILE} 2>&1 > /dev/null

# Safe to purge the unreadable file now.
rm -f ${TEMPFILE}

# Output the GCOV counts.
gcov config.c parser.c
