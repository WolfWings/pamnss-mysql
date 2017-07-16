#include <stdio.h>
#include <stdlib.h>

#include "../config.h"
#include "../parser.h"

char *empty = "[NULL]";

char *string(char *input) {
	if (input == NULL) {
		return empty;
	} else {
		return input;
	}
}

int main(int argc, char **argv) {
	config_parse(argv[1]);

	config_parse(argv[1]);

	return 0;
}
