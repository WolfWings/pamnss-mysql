#!/bin/bash
chmod a-r tests/coverage/parser/invalid-unreadable.cfg
for CFG in tests/coverage/parser/*.cfg ./; do
	./test_parser ${CFG} > /dev/null
done
chmod a+r tests/coverage/parser/invalid-unreadable.cfg
gcov *.c
