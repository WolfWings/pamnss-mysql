#!/bin/bash
for CFG in tests/coverage/parser/*.cfg ./; do
	./test_parser ${CFG} > /dev/null
done
chmod a-r tests/coverage/parser/coverage.cfg
./test_parser tests/coverage/parser/coverage.cfg
chmod a+r tests/coverage/parser/coverage.cfg
gcov *.c
