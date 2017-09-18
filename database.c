#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include <mysql.h>

#include "database.h"
#include "config.h"
#include "parser.h"
#include "banners.h"

static MYSQL *_mysql_con = NULL;
static MYSQL_STMT *_mysql_stmt = NULL;
static int _fetch;

static void finish_with_error(int priority, const char *message) {
	if (_mysql_con == NULL) {
		syslog(priority, "%s: %s", syslog_banner, message);
	} else {
		syslog(priority, "%s: %s: %s", syslog_banner, message, mysql_error(_mysql_con));
		mysql_close(_mysql_con);
		free(_mysql_con);
		_mysql_con = NULL;
	}
}

int db_read_open(const char *query, MYSQL_BIND *params, MYSQL_BIND *results) {
	/* Make sure we have a valid database connection. */
	if (_mysql_con == NULL) {
		_mysql_con = mysql_init(NULL);
		if (_mysql_con == NULL) {
			finish_with_error(LOG_ERR, "Failed to initialize MySQL connection structure");
			return -1;
		}

		if (mysql_real_connect(_mysql_con, options.db.host, options.db.nss.username, options.db.nss.password, options.db.database, 0, NULL, 0) == NULL) {
			finish_with_error(LOG_ERR, "Failed to connect to MySQL");
			return -1;
		}
	}

	_mysql_stmt = mysql_stmt_init(_mysql_con);

	if (_mysql_stmt == NULL) {
		finish_with_error(LOG_ERR, "Failed to allocate MySQL prepared statement structure");
		return -1;
	}

	if (mysql_stmt_prepare(_mysql_stmt, query, strlen(query) + 1) != 0) {
		mysql_stmt_close(_mysql_stmt);
		finish_with_error(LOG_ERR, "Failed to prepare MySQL statement");
		return -1;
	}

	if (mysql_stmt_bind_param(_mysql_stmt, params) != 0) {
		mysql_stmt_close(_mysql_stmt);
		finish_with_error(LOG_ERR, "Failed to bind parameters to prepared MySQL statement");
		return -1;
	}

	if (mysql_stmt_bind_result(_mysql_stmt, results) != 0) {
		mysql_stmt_close(_mysql_stmt);
		finish_with_error(LOG_ERR, "Failed to bind results for prepared MySQL statement");
		return -1;
	}

	if (mysql_stmt_execute(_mysql_stmt) != 0) {
		mysql_stmt_close(_mysql_stmt);
		finish_with_error(LOG_ERR, "Failed to execute prepared MySQL statement");
		return -1;
	}

	return 0;
}

int db_read_fetch() {
	_fetch = mysql_stmt_fetch(_mysql_stmt);
	switch(_fetch) {
		case 1:
			mysql_stmt_close(_mysql_stmt);
			finish_with_error(LOG_ERR, "Failed to fetch prepared MySQL statement result");
			return -1;
			break;
		case MYSQL_NO_DATA:
			return 0;
		default:
			return 1;
	}
}

int db_read_close() {
	int rows;

	rows = mysql_stmt_affected_rows(_mysql_stmt);
	mysql_stmt_close(_mysql_stmt);

	if (rows < 0) {
		/* Only LOG_WARNING as we have successfully fetched SOMETHING. */
		finish_with_error(LOG_WARNING, "Problem fetching number of rows from prepared MySQL statement");

		if (_fetch == MYSQL_NO_DATA) {
			return 0;
		} else {
			return 1;
		}
	}

	return rows;
}

int db_read_single(const char *query, MYSQL_BIND *params, MYSQL_BIND *results) {
	if (db_read_open(query, params, results) == -1) {
		return -1;
	}
	if (db_read_fetch() == -1) {
		return -1;
	}
	return db_read_close();
}
