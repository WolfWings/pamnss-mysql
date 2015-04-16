#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "parser.h"

char *empty = "[NULL]";

char *string(char *input) {
	if (input == NULL) {
		return empty;
	} else {
		return input;
	}
}

int main(int argc, char **argv) {
	parse_config(argv[1]);
	printf("db.host = %s\n", string(options.db.host));
	printf("db.database = %s\n", string(options.db.database));
	printf("db.pam = %s:%s\n", string(options.db.pam.username), string(options.db.pam.password));
	printf("db.nssuser = %s:%s\n", string(options.db.nssuser.username), string(options.db.nssuser.password));
	printf("db.nssroot = %s:%s\n", string(options.db.nssroot.username), string(options.db.nssroot.password));
	printf("nss.getpwent = %s\n", string(options.nss.getpwent));
	printf("nss.getpwnam = %s\n", string(options.nss.getpwnam));
	printf("nss.getpwuid = %s\n", string(options.nss.getpwuid));
	printf("nss.getspent = %s\n", string(options.nss.getspent));
	printf("nss.getspnam = %s\n", string(options.nss.getspnam));
	printf("nss.getgrent = %s\n", string(options.nss.getgrent));
	printf("nss.getgrnam = %s\n", string(options.nss.getgrnam));
	printf("nss.getgrgid = %s\n", string(options.nss.getgrgid));
	printf("nss.memsbygid = %s\n", string(options.nss.memsbygid));
	printf("nss.gidsbymem = %s\n", string(options.nss.gidsbymem));
	printf("pam.table = %s\n", string(options.pam.table));
	printf("pam.usernamecolumn = %s\n", string(options.pam.usernamecolumn));
	printf("pam.passwordcolumn = %s\n", string(options.pam.passwordcolumn));
	return 0;
}
