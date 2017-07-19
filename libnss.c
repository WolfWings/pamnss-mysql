#include <nss.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <syslog.h>

#include "banners.h"

/* /etc/passwd coverage */

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

enum nss_status _nss_mysql2017_getgrgid_r (gid_t gid, struct group *grp, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "%s: getgrgid_r called", syslog_banner);
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
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
