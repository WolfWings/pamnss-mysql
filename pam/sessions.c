#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>

#define PAM_SM_SESSION
#include <security/pam_modules.h>
#include <security/pam_modutil.h>
#include <security/pam_ext.h>

#include "pam_mysql.h"

static int pam_sm_session(pam_handle_t *pamh, int closing) {
	int          results;
	char         *service;
	char         *username;
	const char   *loginname;
	int          response;

	response = PAM_SUCCESS;

	results = pam_get_item(pamh, PAM_SERVICE, (void *) &service);
	if (results != PAM_SUCCESS || service == NULL || service[0] == 0) {
		response = PAM_SESSION_ERR;
		service = "[UNKNOWN]";
	}

	results = pam_get_item(pamh, PAM_USER, (void *) &username);
	if (results != PAM_SUCCESS || username == NULL || username[0] == 0) {
		response = PAM_SESSION_ERR;
		username = "[UNKNOWN]";
	}

	loginname = pam_modutil_getlogin(pamh);
	if (loginname == NULL) {
	    loginname = "[UNKNOWN]";
	}

	pam_syslog(pamh, (response == PAM_SUCCESS) ? LOG_INFO : LOG_CRIT,
		"session %s for user %s by %s(uid=%lu) for %s",
		closing ? "closed" : "opened",
		username, loginname, (unsigned long)getuid(), service);

	return response;
}

int pam_sm_open_session(pam_handle_t *pamh, int flags UNUSED, int argc UNUSED, const char **argv UNUSED) {
	return pam_sm_session(pamh, 0);
}

int pam_sm_close_session(pam_handle_t *pamh, int flags UNUSED, int argc UNUSED, const char **argv UNUSED) {
	return pam_sm_session(pamh, !0);
}
