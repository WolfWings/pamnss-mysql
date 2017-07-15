#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "config.h"

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

/* C99 style initialization */
_options options = {
	.db.host = NULL
,	.db.database = NULL
,	.db.pam.username = NULL
,	.db.pam.password = NULL
,	.db.nssuser.username = NULL
,	.db.nssuser.password = NULL
,	.db.nssroot.username = NULL
,	.db.nssroot.password = NULL
,	.nss.getpwent = NULL
,	.nss.getpwnam = NULL
,	.nss.getpwuid = NULL
,	.nss.getspent = NULL
,	.nss.getspnam = NULL
,	.nss.getgrent = NULL
,	.nss.getgrnam = NULL
,	.nss.getgrgid = NULL
,	.nss.memsbygid = NULL
,	.nss.gidsbymem = NULL
,	.pam.table = NULL
,	.pam.usernamecolumn = NULL
,	.pam.passwordcolumn = NULL
};

/*
 * When importing a new config-state limit any value to at most 4k.
 * Also means we free the old strings once replaced.
 */

static void updatestring(void *old, const char *new) {
	char *temp = *((char **)old);

	*((char **)old) = strndup(new, 4095);

	if (temp != NULL) {
		free(temp);
	}
}

static void updatecredentials(void *old, const char *new) {
	_logincreds temp;
	char *split = index(new, ':');

	if (split == NULL) {
		return;
	}

	temp = *((_logincreds *)old);
	((_logincreds *)old)->username = strndup(new, min(split - new, 4095));
	((_logincreds *)old)->password = strndup(split + 1, 4095);

	if (temp.username != NULL) {
		free(temp.username);
	}

	if (temp.password != NULL) {
		free(temp.password);
	}
}

/*
 * A very basic 'linear search' to populate the options-structure.
 * Could be sped up greatly with a Finite Automata/Radix-Tree based
 * string matching algorithm but this is much more maintainable.
 */
typedef struct {
	char *name;
	void *destination;
	void (*function)(void *old, const char *new);
} _stringsearch;

static _stringsearch stringsearch[] = {
	{"db.host", (void *)&options.db.host, updatestring}
,	{"db.database", (void *)&options.db.database, updatestring}
,	{"db.pam", (void *)&options.db.pam, updatecredentials}
,	{"db.nssuser", (void *)&options.db.nssuser, updatecredentials}
,	{"db.nssroot", (void *)&options.db.nssroot, updatecredentials}
,	{"nss.getpwent", (void *)&options.nss.getpwent, updatestring}
,	{"nss.getpwnam", (void *)&options.nss.getpwnam, updatestring}
,	{"nss.getpwuid", (void *)&options.nss.getpwuid, updatestring}
,	{"nss.getspent", (void *)&options.nss.getspent, updatestring}
,	{"nss.getspnam", (void *)&options.nss.getspnam, updatestring}
,	{"nss.getgrent", (void *)&options.nss.getgrent, updatestring}
,	{"nss.getgrnam", (void *)&options.nss.getgrnam, updatestring}
,	{"nss.getgrgid", (void *)&options.nss.getgrgid, updatestring}
,	{"nss.memsbygid", (void *)&options.nss.memsbygid, updatestring}
,	{"nss.gidsbymem", (void *)&options.nss.gidsbymem, updatestring}
,	{"pam.table", (void *)&options.pam.table, updatestring}
,	{"pam.usernamecolumn", (void *)&options.pam.usernamecolumn, updatestring}
,	{"pam.passwordcolumn", (void *)&options.pam.passwordcolumn, updatestring}
};

void option_set(const char *name, const char *value) {
	int i;
	for (i = 0; i < (sizeof(stringsearch) / sizeof(_stringsearch)); i++) {
		if (strcasecmp(name, stringsearch[i].name) == 0) {
			stringsearch[i].function(stringsearch[i].destination, value);
			return;
		}
	}
}
