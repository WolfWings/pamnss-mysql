#include <nss.h>
#include <pwd.h>
#include <grp.h>
#include <syslog.h>

/* /etc/passwd coverage */

int _nss_mysql2017_setpwent (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - setpwent called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_getpwent_r (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getpwent_r called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_endpwent (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - endpwent called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_getpwuid_r (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getpwuid_r called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_getpwnam_r (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getpwnam_r called");
	return NSS_STATUS_NOTFOUND;
}

/* /etc/group coverage */

int _nss_mysql2017_setgrent (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - setgrent called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_getgrent_r (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getgrent_r called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_endgrent (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - endgrent called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_getgrgid_r (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getgrgid_r called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_getgrnam_r (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getgrnam_r called");
	return NSS_STATUS_NOTFOUND;
}

/* /etc/shadow coverage */

int _nss_mysql2017_setspent (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - setspent called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_getspent_r (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getspent_r called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_endspent (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - endspent called");
	return NSS_STATUS_NOTFOUND;
}

int _nss_mysql2017_getspnam_r (void) { // FIXME
	syslog(LOG_WARNING, "[PAMNSS-MySQL-2017] - getspnam_r called");
	return NSS_STATUS_NOTFOUND;
}
