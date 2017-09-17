#include <mysql.h>

int db_read_single(const char *query, MYSQL_BIND *params, MYSQL_BIND *results);
int db_read_open(const char *query, MYSQL_BIND *params, MYSQL_BIND *results);
int db_read_fetch();
int db_read_close();
