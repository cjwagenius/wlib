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
 *	void wlog_init(const char *name, enum wlogt lvl, FILE *out, int local);
 *
 *		Initializes wlog and sets required values.
 *
 *		[name]   is the application name that should be mentioned in
 *			 the logfile. (REQUIRED)
 *
 *		[level]  Which level of logging should be written.
 *			 WLOG_OFF	- nothing is printed
 *			 WLOG_ERR	- only errors are printed.
 *			 WLOG_WRN	- errors & warnings are printed
 *			 WLOG_NFO	- errors, warnings & infos are printed
 *			 WLOG_DBG	- all messages are printed
 *
 *		[out]    Stream to print messages to. (REQUIRED)
 *
 *		[local]  If 1, use local timestamps, otherwise GMT is used.
 *
 *	void wlog_level(enum wlogt level)
 *
 * 		Sets the log-level.
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
extern void wlog_init(const char *name, enum wlogt lvl, FILE *out, int local);
extern void wlog_set_level(enum wlogt lvl);
extern void wlog_set_out(FILE *out);

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

static FILE       *wlog_out    = NULL;
static const char *wlog_name   = NULL;
static int         wlog_loglvl = WLOG_ERR;
static int         wlog_local  = 0;

static char *wlog_strtime(time_t t)
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
	time_t t;
	static char *types[] = { NULL, "ERR", "WRN", "INF", "DBG" };

	assert((wlog_name && wlog_out) && "wlog.h not initialized correctly");

	if (!wlog_loglvl || (type > wlog_loglvl)) return;
	t = time(NULL);
#ifdef WLOG_PTHREADS
	pthread_mutex_lock(&wlog_mux);
#endif /* WLOG_PTHREADS */
	fputs(wlog_strtime(t), wlog_out);
	fprintf(wlog_out, " - %s (%s): ", wlog_name, types[type]);
	vfprintf(wlog_out, fmt, a);
	fputc('\n', wlog_out);
	fflush(wlog_out);
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
void wlog_init(const char *name, enum wlogt lvl, FILE *out, int local)
{
	assert(name && "[name] must not be NULL");

	wlog_name = name;
	wlog_local = local;
	wlog_set_out(out);
	wlog_set_level(lvl);
} 
void wlog_set_level(enum wlogt lvl)
{
	wlog_loglvl = lvl;
}
void wlog_set_out(FILE *out)
{
	assert(out && "[out] must not be NULL");

	wlog_out = out;
}

#endif /* WLOG_IMPL */
#endif /* WLOG_H */
