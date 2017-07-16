#include <nss.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <syslog.h>

/* /etc/passwd coverage */

enum nss_status _nss_mysql2017_setpwent (int stayopen) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - setpwent called");
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getpwent_r (struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getpwent_r called");
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endpwent (void) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - endpwent called");
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getpwuid_r (uid_t uid, struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getpwuid_r called");
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getpwnam_r (const char *name, struct passwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getpwnam_r called");
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

/* /etc/group coverage */

enum nss_status _nss_mysql2017_setgrent (int stayopen) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - setgrent called");
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getgrent_r (struct group *grp, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getgrent_r called");
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endgrent (void) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - endgrent called");
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getgrgid_r (gid_t gid, struct group *grp, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getgrgid_r called");
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getgrnam_r (const char *name, struct group *grp, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getgrnam_r called");
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

/* /etc/shadow coverage */

enum nss_status _nss_mysql2017_setspent (int stayopen) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - setspent called");
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getspent_r (struct spwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getspent_r called");
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_endspent (void) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - endspent called");
	return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_mysql2017_getspnam_r (const char *name, struct spwd *pwd, char *buffer, size_t buflen, int *errnop) {
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getspnam_r called");
	*errnop = 0;
	return NSS_STATUS_NOTFOUND;
}
