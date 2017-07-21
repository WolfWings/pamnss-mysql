#include <stdio.h>
#include <stdarg.h>

int syslog(int priority, const char *format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);
}
