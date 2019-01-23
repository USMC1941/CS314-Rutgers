#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "Utils.h"

int err(const char *level, const char *func, int line, const char *fmt, ...)
{
	char s[1024];
	va_list va;
	int r;

	va_start(va, fmt);
	vsprintf(s, fmt, va);
	va_end(va);
	r = fprintf(stderr, "[%s:%s:%d] %s", level, func, line, s);
	return r;
}
