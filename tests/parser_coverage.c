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

static void _free(void **p) {
	if (*p != NULL) {
		free(*p);
		*p = NULL;
	}
}

int main(int argc, char **argv) {
	int i;

	for (i = 1; i < argc; i++) {
		config_parse(argv[i]);
	}

	config_load();

	_free((void *)&options.db.host);
	_free((void *)&options.db.database);

	_free((void *)&options.db.pam.username);
	_free((void *)&options.db.pam.password);

	_free((void *)&options.db.nss.username);
	_free((void *)&options.db.nss.password);

	_free((void *)&options.nss.setpwent);
	_free((void *)&options.nss.getpwent);
	_free((void *)&options.nss.endpwent);
	_free((void *)&options.nss.getpwuid);
	_free((void *)&options.nss.getpwnam);

	_free((void *)&options.nss.setspent);
	_free((void *)&options.nss.getspent);
	_free((void *)&options.nss.endspent);
	_free((void *)&options.nss.getspnam);

	_free((void *)&options.nss.setgrent);
	_free((void *)&options.nss.getgrent);
	_free((void *)&options.nss.endgrent);
	_free((void *)&options.nss.getgrgid);
	_free((void *)&options.nss.getgrnam);

	_free((void *)&options.pam.table);
	_free((void *)&options.pam.usernamecolumn);
	_free((void *)&options.pam.passwordcolumn);

	return 0;
}
