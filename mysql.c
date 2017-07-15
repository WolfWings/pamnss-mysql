#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

static void finish_with_error(MYSQL *con) {
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);
}
