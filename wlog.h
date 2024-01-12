/**
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 *
 * Written and made public by C.J.Wagenius - 2024-01-12
 *
 * ---------------------------------------------------------------------------
 *
 * Pthread environment:
 *
 * 	If being used with Pthreads, define WLOG_PTHREADS before including
 * 	this file.
 *
 * Global variables:
 *
 *	int wlog_local
 *		If wlog_local print local time else GMT, (default: GMT)
 *
 *	enum wlogt wlog_level
 *		Log level. Levels greater than this will be ignored,
 *		(default: WLOG_ERR).
 *
 *	char* wlog_name
 *		Output this name in every message, (default: nothing).
 *
 *	FILE* wlog_out
 *		Where to write log messages, (default: stderr).
 *
 *
 * Log-levels:
 *
 *	WLOG_OFF
 *	WLOG_ERR
 *	WLOG_WRN
 *	WLOG_NFO
 *	WLOG_DBG
 *
 *
 * Functions:
 *
 *	void wlog(enum wlogt type, const char *fmt, ...)
 *	void wlogv(enum wlogt type, const char *fmt, va_list a)
 *
 *             Writes a message to the log, if the loglevel of the message
 *             allowes it.
 *
 *
 **/

#ifndef WLOG_H
#define WLOG_H

#include <stdio.h>
#include <stdarg.h>

enum wlogt {
	WLOG_OFF,
	WLOG_ERR,
	WLOG_WRN,
	WLOG_NFO,
	WLOG_DBG
};

extern void wlog(enum wlogt type, const char *fmt, ...);
extern void wlogv(enum wlogt type, const char *fmt, va_list a);

#ifdef WLOG_IMPL
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifdef WLOG_PTHREADS
#include <pthread.h>

static pthread_mutex_t wlog_mux = PTHREAD_MUTEX_INITIALIZER;
#endif /* WLOG_PTHREADS */

FILE       *wlog_out   = NULL;
const char *wlog_name  = NULL;
enum wlog   wlog_level = WLOG_ERR;
int         wlog_local = 0;

static char *_wlog_strtime(time_t t)
{
	struct tm *tm;
	static char buf[20];

	tm = wlog_local ? localtime(&t) : gmtime(&t);
	sprintf(buf, "%i-%02i-%02i %02i:%02i:%02i",
	    tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
	    tm->tm_hour, tm->tm_min, tm->tm_sec);

	return buf;
}
void wlogv(enum wlogt type, const char *fmt, va_list a)
{
	FILE* o;
	time_t t;
	static char *types[] = { NULL, "ERR", "WRN", "INF", "DBG" };

	if (!wlog_level || (type > wlog_level))
		return;
	t = time(NULL);
	o = wlog_out ? wlog_out : stderr;
#ifdef WLOG_PTHREADS
	pthread_mutex_lock(&wlog_mux);
#endif /* WLOG_PTHREADS */
	fputs(_wlog_strtime(t), o);
	if (wlog_name)
		fprintf(o, " - %s (%s): ", wlog_name, types[type]);
	else
		fprintf(o, " - (%s): ", types[type]);
	vfprintf(o, fmt, a);
	fputc('\n', o);
	fflush(o);
#ifdef WLOG_PTHREADS
	pthread_mutex_unlock(&wlog_mux);
#endif /* WLOG_PTHREADS */
}
void wlog(enum wlogt type, const char *fmt, ...)
{
	va_list a;

	va_start(a, fmt);
	wlogv(type, fmt, a);
	va_end(a);
}

#endif /* WLOG_IMPL */
#endif /* WLOG_H */
