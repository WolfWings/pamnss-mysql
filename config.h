void option_set(const char *name, const char *value);

typedef struct {
	char *username;
	char *password;
} _logincreds;

typedef struct {
	struct {
		char *host;
		char *database;

		_logincreds pam;
		_logincreds nssuser;
		_logincreds nssroot;
	} db;

	struct {
		char *setpwent;
		char *getpwent;
		char *endpwent;
		char *getpwuid;
		char *getpwnam;

		char *setspent;
		char *getspent;
		char *endspent;
		char *getspnam;

		char *setgrent;
		char *getgrent;
		char *endgrent;
		char *getgrgid;
		char *getgrnam;
	} nss;

	struct {
		char *table;
		char *usernamecolumn;
		char *passwordcolumn;
	} pam;
} _options;

extern _options options;
