#include <nss.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <syslog.h>
#include <mysql.h>
#include <string.h>
#include <errno.h>

#include "database.h"
#include "banners.h"
#include "config.h"
#include "parser.h"

/* /etc/passwd coverage */

char *password_see_shadow = "x";

char *empty_array = NULL;

static int pwent_line = 0;

enum nss_status _nss_mysql2017_setpwent (int stayopen) {
	syslog(LOG_DEBUG, "%s: setpwent called", syslog_banner);

	pwent_line = 0;

	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_mysql2017_getpwent_r (struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_DEBUG, "%s: getpwent_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endpwent (void) {
	syslog(LOG_DEBUG, "%s: endpwent called", syslog_banner);
	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_mysql2017_getpwuid_r (uid_t uid, struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_DEBUG, "%s: getpwuid_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getpwnam_r (const char *name, struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_DEBUG, "%s: getpwnam_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

/* /etc/group coverage */

static int grent_cursor = 0;

enum nss_status _nss_mysql2017_setgrent (int stayopen) {
	syslog(LOG_DEBUG, "%s: setgrent called", syslog_banner);

	grent_cursor = 0;

	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_mysql2017_getgrent_r (struct group *grp, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_DEBUG, "%s: getgrent_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endgrent (void) {
	syslog(LOG_DEBUG, "%s: endgrent called", syslog_banner);
	return NSS_STATUS_SUCCESS;
}

#define BOOL_TRUE (0 == 0)
#define BOOL_FALSE (0 != 0)

enum nss_status _nss_mysql2017_getgrgid_r (gid_t gid, struct group *grp, char *buffer, size_t buflen, int *errnop) {
	/*
	 * 0 - GID integer
	 * 1 - Name
	 * 2 - Members
	 */
	MYSQL_BIND binds[3];
	my_bool bind_nulls[3];
	unsigned long long int gid_in;
	char name_out[256];
	unsigned long name_out_length;
	char members_out[256];
	unsigned long members_out_length;

	syslog(LOG_DEBUG, "%s: getgrgid_r called with gid [%i]", syslog_banner, gid);

	/* Make sure config files have been loaded */
	config_load();

	if (options.nss.getgrgid == NULL) {
		*errnop = 0;
		return NSS_STATUS_NOTFOUND;
	}

	memset(binds, 0, sizeof(binds));

	binds[0].buffer_type = MYSQL_TYPE_LONGLONG;
	binds[0].buffer = &gid_in;
	binds[0].buffer_length = 8;
	bind_nulls[0] = BOOL_FALSE;
	binds[0].is_null = &bind_nulls[0];
	binds[0].is_unsigned = BOOL_TRUE;

	binds[1].buffer_type = MYSQL_TYPE_STRING;
	binds[1].buffer = &name_out;
	binds[1].buffer_length = 256;
	binds[1].length = &name_out_length;

	binds[2].buffer_type = MYSQL_TYPE_STRING;
	binds[2].buffer = &members_out;
	binds[2].buffer_length = 256;
	binds[2].length = &members_out_length;

	switch ( db_read_single(options.nss.getgrgid, &binds[0], &binds[1]) ) {
		case -1:
			*errnop = ENOENT;
			return NSS_STATUS_NOTFOUND;
			break;
		case 0:
			*errnop = 0;
			return NSS_STATUS_NOTFOUND;
			break;
		default:
			*errnop = 0;
			grp->gr_name = strndup(name_out, sizeof(name_out));
			grp->gr_passwd = password_see_shadow;
			grp->gr_gid = gid;
			grp->gr_mem = &empty_array;
			return NSS_STATUS_SUCCESS;
			break;
	}
}

enum nss_status _nss_mysql2017_getgrnam_r (const char *name, struct group *grp, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_DEBUG, "%s: getgrnam_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

/* /etc/shadow coverage */

static int spent_line = 0;

enum nss_status _nss_mysql2017_setspent (int stayopen) {
	syslog(LOG_DEBUG, "%s: setspent called", syslog_banner);

	spent_line = 0;

	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_mysql2017_getspent_r (struct spwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_DEBUG, "%s: getspent_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endspent (void) {
	syslog(LOG_DEBUG, "%s: endspent called", syslog_banner);
	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_mysql2017_getspnam_r (const char *name, struct spwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_DEBUG, "%s: getspnam_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}
