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
		_logincreds nss;
	} db;

	struct {
		char *getpwent;
		char *getpwnam;
		char *getpwuid;

		char *getspent;
		char *getspnam;

		char *getgrent;
		char *getgrnam;
		char *getgrgid;
	} nss;

	struct {
		char *table;
		char *usernamecolumn;
		char *passwordcolumn;
	} pam;
} _options;

extern _options options;
