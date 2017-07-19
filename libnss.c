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

/* /etc/passwd coverage */

char *password_see_shadow = "x";

char *empty_array = NULL;

enum nss_status _nss_mysql2017_setpwent (int stayopen) {
	syslog(LOG_WARNING, "%s: setpwent called", syslog_banner);
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getpwent_r (struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "%s: getpwent_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endpwent (void) {
	syslog(LOG_WARNING, "%s: endpwent called", syslog_banner);
	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_mysql2017_getpwuid_r (uid_t uid, struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "%s: getpwuid_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getpwnam_r (const char *name, struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "%s: getpwnam_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

/* /etc/group coverage */

enum nss_status _nss_mysql2017_setgrent (int stayopen) {
	syslog(LOG_WARNING, "%s: setgrent called", syslog_banner);
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getgrent_r (struct group *grp, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "%s: getgrent_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endgrent (void) {
	syslog(LOG_WARNING, "%s: endgrent called", syslog_banner);
	return NSS_STATUS_SUCCESS;
}

#define BOOL_TRUE (0 == 0)
#define BOOL_FALSE (0 != 0)

enum nss_status _nss_mysql2017_getgrgid_r (gid_t gid, struct group *grp, char *buffer, size_t buflen, int *errnop) {
	/*
	 * 0 - GID integer
	 * 1 - Name
	 * 2 - GID
	 * 3 - Members
	 */
	MYSQL_BIND binds[4];
	my_bool bind_nulls[4];
	unsigned long long int gid_in;
	unsigned long long int gid_out;
	char name_out[256];
	unsigned long name_out_length;
	char members_out[256];
	unsigned long members_out_length;

	syslog(LOG_DEBUG, "%s: getgrgid_r called with gid [%i]", syslog_banner, gid);

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

	binds[2].buffer_type = MYSQL_TYPE_LONGLONG;
	binds[2].buffer = &gid_out;
	binds[2].buffer_length = 8;
	binds[2].is_null = &bind_nulls[2];

	binds[3].buffer_type = MYSQL_TYPE_STRING;
	binds[3].buffer = &members_out;
	binds[3].buffer_length = 256;
	binds[3].length = &members_out_length;

	switch ( db_read(options.nss.getgrgid, &binds[0], &binds[1]) ) {
		case 1:
			*errnop = 0;
			grp->gr_name = strndup(name_out, sizeof(name_out));
			grp->gr_passwd = password_see_shadow;
			grp->gr_gid = gid_out;
			grp->gr_mem = &empty_array;
			return NSS_STATUS_SUCCESS;
			break;
		case 0:
			*errnop = 0;
			return NSS_STATUS_NOTFOUND;
			break;
		default:
			*errnop = ENOENT;
			return NSS_STATUS_NOTFOUND;
			break;
	}

}

enum nss_status _nss_mysql2017_getgrnam_r (const char *name, struct group *grp, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "%s: getgrnam_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

/* /etc/shadow coverage */

enum nss_status _nss_mysql2017_setspent (int stayopen) {
	syslog(LOG_WARNING, "%s: setspent called", syslog_banner);
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getspent_r (struct spwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "%s: getspent_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endspent (void) {
	syslog(LOG_WARNING, "%s: endspent called", syslog_banner);
	return NSS_STATUS_SUCCESS;
}

enum nss_status _nss_mysql2017_getspnam_r (const char *name, struct spwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "%s: getspnam_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}
