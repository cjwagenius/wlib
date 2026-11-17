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
 * Written and made public by C.J.Wagenius - 2026-11-17
 * ----------------------------------------------------------------------------
 *
 * A convenient ANSI C ISO8601 library for timespec (POSIX.1b-1993)
 *
 *     Output ISO8601-formated strings of timespec:s with ease.
 *
 *
 * Instructions:
 *
 *     This is a single header-file to include in projects. In one source file
 *     per project, e.g. misc.c, define WISO_IMPL and include this header after
 *     that. The code will then be compiled with misc.c. When that's done, you
 *     can include this file as usual in any other file where the functions are
 *     needed. The following example would make the code compile with misc.c
 *     and make the functions/macros available for all source files that
 *     includes the misc.h header.
 *
 *         Example:
 *
 *              misc.c:
 *             ----------------------------------------
 *              #define WISO_IMPL
 *              #include "misc.h"
 *
 *              misc.h:
 *             ----------------------------------------
 *              #include "wiso.h"
 *
 *
 *     The macro-functions are named after which format they produce. There
 *     are three groups of formats as macro-functions, date, time & iso.
 *     They have certain prefixes to specify which format that should be
 *     expected. wiso_'date' will produce a UTC date-string ("1988-12-24"),
 *     and wiso_'ldate' will produce the local time counter part.
 *     The same goes for time and iso. wiso_'time' will produce the UTC time,
 *     ("10:15:08") and wiso_'liso' will produce the local time ISO8601 format
 *     ("1988-12-24T11:15:08+01:00").
 *     The time and iso macros also have a precision prefix for seconds and it
 *     follows the local-prefix ('l'). 'm', 'u' and 'n', produces mili-, micro-
 *     and nano seconds respectively. To produce a ISO8601 formated string with
 *     micro seconds in local time ("1988-12-24T11:15:08.141303+01:00"), one
 *     would use the macro wiso_luiso(). If no macro suits your need, check
 *     instructions for the function wiso_str() below.
 *
 *         Example:
 *
 *             struct timespec ts;
 *             char buf[WISO_LUISO_LEN + 1];
 *
 *             wiso_luiso(buf, sizeof(buf), wiso_now(&ts));
 *
 *     
 * Length macros:
 *
 * 	   macro		format-flag	    example
 *    -------------------------------------------------------------------------
 *     WISO_DATE_LEN		WISO_DATE	"1988-12-24"
 *     WISO_TIME_LEN		WISO_TIME	"10:15:08"
 *     WISO_MSEC_LEN		WISO_MSEC	".141"
 *     WISO_USEC_LEN		WISO_USEC	".141303"
 *     WISO_NSEC_LEN		WISO_NSEC	".141303120"
 *     WISO_GOFF_LEN		WISO_OFFS	"Z"
 *     WISO_LOFF_LEN		WISO_OFFS	"+01:00"
 *     WISO_MTIME_LEN		WISO_MTIME	"10:15:08.141"
 *     WISO_UTIME_LEN		WISO_UTIME	"10:15:08.141303"
 *     WISO_NTIME_LEN		WISO_NTIME	"10:15:08.141303120"
 *     WISO_DATETIME_LEN	WISO_DATETIME	"1988-12-24T10:15:08"
 *     WISO_DATEMTIME_LEN	WISO_DATEMTIME	"1988-12-24T10:15:08.141"
 *     WISO_DATEUTIME_LEN	WISO_DATEUTIME	"1988-12-24T10:15:08.141303"
 *     WISO_DATENTIME_LEN	WISO_DATENTIME	"1988-12-24T10:15:08.141303120"
 *     WISO_ISO_LEN		WISO_ISO	"1988-12-24T10:15:08+01:00"
 *     WISO_MISO_LEN		WISO_MISO	"1988-12-24T10:15:08.141Z"
 *     WISO_UISO_LEN		WISO_UISO	"1988-12-24T10:15:08.141303Z"
 *     WISO_NISO_LEN		WISO_NISO	"1988-12-24T10:15:08.141303120Z"
 *     WISO_LMISO_LEN		WISO_MISO	"1988-12-24T10:15:08.141+01:00"
 *     WISO_LUISO_LEN		WISO_UISO	"1988-12-24T10:15:08.141303+01:00"
 *     WISO_LNISO_LEN		WISO_NISO	"1988-12-24T10:15:08.141303120+01:00"
 *
 *
 * Function macros:
 *
 *	     macros		  format specifiers
 *    -------------------------------------------------------------------------
 *    UTC:
 *     wiso_datestr(b, l, n)	WISO_DATE
 *     wiso_timestr(b, l, n)	WISO_TIME
 *     wiso_mtimestr(b, l, n)	WISO_TIME | WISO_MSEC
 *     wiso_utimestr(b, l, n)	WISO_TIME | WISO_USEC
 *     wiso_ntimestr(b, l, n)	WISO_TIME | WISO_NSEC
 *     wiso_isostr(b, l, n)	WISO_GOFF
 *     wiso_misostr(b, l, n)	WISO_GOFF | WISO_MSEC
 *     wiso_uisostr(b, l, n)	WISO_GOFF | WISO_USEC
 *     wiso_nisostr(b, l, n)	WISO_GOFF | WISO_NSEC
 *
 *    Local time:
 *     wiso_ldatestr(b, l, n)	WISO_DATE | WISO_LOCL 
 *     wiso_ltimestr(b, l, n)	WISO_LOFF
 *     wiso_lmtimestr(b, l, n)	WISO_LOFF | WISO_LOCL | WISO_MSEC
 *     wiso_lutimestr(b, l, n)	WISO_LOFF | WISO_LOCL | WISO_USEC
 *     wiso_lntimestr(b, l, n)	WISO_LOFF | WISO_LOCL | WISO_NSEC
 *     wiso_lisostr(b, l, n)	WISO_LISO
 *     wiso_lmisostr(b, l, n)	WISO_LISO | WISO_LOCL | WISO_MSEC
 *     wiso_luisostr(b, l, n)	WISO_LISO | WISO_LOCL | WISO_USEC
 *     wiso_lnisostr(b, l, n)	WISO_LISO | WISO_LOCL | WISO_NSEC
 *
 *
 * Functions:
 *
 *     struct timespec* wiso_now(struct timespec* [ts | NULL])
 *
 *         Returns a timespec-pointer to a structure holding current time
 *         from epoch. If [ts] is given its memory is used and [ts] is
 *         returned. Otherwise a pointer to an internal structure is returned.
 *
 *
 *     size_t wiso_str(char* buf, size_t len, int fmt, struct timespec* ts)
 *
 *         Formats the time [ts] to a string at [buf] with a maximum size of
 *         [len]. Which format to be used is given with format-flags to [fmt].
 *         The function returns bytes required to store the formated string,
 *         not including a terminating zero.
 *         Macros exists to dynamically allocate memory for the different
 *         format options, see Length macros. There are also less complicated,
 *         flag-free, function-macros to use instead of calling this
 *         function directly, see Function macros above. If no date or time
 *         format is chosen in [fmt], WISO_DATETIME and WISO_OFFS are or:ed in.
 *
 *
 * Compile options:
 *
 *     USE_THREADS	- Enables thread-safety (gmtime_r & localtime_r).
 *                        Beware of handing wiso_now() a NULL-pointer.
 *
 * Requirements:
 *
 *     ANSI C '98
 *
 *     POSIX.1-1988|1990
 *         extern long timezone
 *
 *     POSIX.1b-1993
 *         struct timespec
 *         clock_gettime
 *
 *
 **/

#ifndef WISO_H
#define WISO_H

#include <time.h>
#include <stdlib.h>

#define WISO_NSPERS 1000000000ULL
#define WISO_DATE_LEN  10
#define WISO_TIME_LEN   8
#define WISO_MSEC_LEN   4
#define WISO_USEC_LEN   7
#define WISO_NSEC_LEN  10
#define WISO_GOFF_LEN   1
#define WISO_LOFF_LEN   6
#define WISO_MTIME_LEN  (WISO_TIME_LEN + WISO_MSEC_LEN)
#define WISO_UTIME_LEN  (WISO_TIME_LEN + WISO_USEC_LEN)
#define WISO_NTIME_LEN  (WISO_TIME_LEN + WISO_NSEC_LEN)
#define WISO_DATETIME_LEN (WISO_DATE_LEN + 1 + WISO_TIME_LEN)
#define WISO_DATEMTIME_LEN (WISO_DATETIME_LEN + WISO_MSEC)
#define WISO_DATEUTIME_LEN (WISO_DATETIME_LEN + WISO_USEC)
#define WISO_DATENTIME_LEN (WISO_DATETIME_LEN + WISO_NSEC)
#define WISO_ISO_LEN  (WISO_DATETIME_LEN + WISO_GOFF_LEN)
#define WISO_MISO_LEN (WISO_DATETIME_LEN + WISO_MSEC_LEN + WISO_GOFF_LEN)
#define WISO_UISO_LEN (WISO_DATETIME_LEN + WISO_USEC_LEN + WISO_GOFF_LEN)
#define WISO_NISO_LEN (WISO_DATETIME_LEN + WISO_NSEC_LEN + WISO_GOFF_LEN)
#define WISO_LISO_LEN (WISO_DATETIME_LEN + WISO_LOFF_LEN)
#define WISO_LMISO_LEN (WISO_DATETIME_LEN + WISO_MSEC_LEN + WISO_LOFF_LEN)
#define WISO_LUISO_LEN (WISO_DATETIME_LEN + WISO_USEC_LEN + WISO_LOFF_LEN)
#define WISO_LNISO_LEN (WISO_DATETIME_LEN + WISO_NSEC_LEN + WISO_LOFF_LEN)

enum wisofmt {
	WISO_DATE     = (1 << 0),
	WISO_TIME     = (1 << 1),
	WISO_MSEC     = (1 << 2),
	WISO_USEC     = (1 << 3),
	WISO_NSEC     = (1 << 4),
	WISO_OFFS     = (1 << 5),
	WISO_LOCL     = (1 << 6),
	WISO_DATETIME = (WISO_DATE | WISO_TIME)
};

#define wiso_date(b, l, n)  \
    wiso_str(b, l, WISO_DATE, n)
#define wiso_time(b, l, n)  \
    wiso_str(b, l, WISO_TIME, n)
#define wiso_mtime(b, l, n) \
    wiso_str(b, l, (WISO_TIME | WISO_MSEC), n)
#define wiso_utime(b, l, n) \
    wiso_str(b, l, (WISO_TIME | WISO_USEC), n)
#define wiso_ntime(b, l, n) \
    wiso_str(b, l, (WISO_TIME | WISO_NSEC), n)
#define wiso_iso(b, l, n) \
    wiso_str(b, l, 0, n)
#define wiso_miso(b, l, n) \
    wiso_str(b, l, WISO_MSEC, n)
#define wiso_uiso(b, l, n)  \
    wiso_str(b, l, WISO_USEC, n)
#define wiso_niso(b, l, n)  \
    wiso_str(b, l, WISO_NSEC, n)
#define wiso_ltime(b, l, n)  \
    wiso_str(b, l, WISO_TIME | WISO_LOCL, n)
#define wiso_lmtime(b, l, n) \
    wiso_str(b, l, (WISO_TIME | WISO_LOCL | WISO_MSEC), n)
#define wiso_lutime(b, l, n) \
    wiso_str(b, l, (WISO_TIME | WISO_LOCL | WISO_USEC), n)
#define wiso_lntime(b, l, n) \
    wiso_str(b, l, (WISO_TIME | WISO_LOCL | WISO_NSEC), n)
#define wiso_liso(b, l, n) \
    wiso_str(b, l, WISO_LOCL, n)
#define wiso_lmiso(b, l, n) \
    wiso_str(b, l, WISO_MSEC | WISO_LOCL, n)
#define wiso_luiso(b, l, n)  \
    wiso_str(b, l, WISO_USEC | WISO_LOCL, n)
#define wiso_lniso(b, l, n)  \
    wiso_str(b, l, WISO_NSEC | WISO_LOCL, n)

#define wiso_split(s, ts) (*(s) = (ts)->tv_sec, ts->tv_nsec)

extern struct timespec* wiso_now(struct timespec* ts);
extern size_t wiso_str(char* buf, size_t len, int fmt, const struct timespec* ts);

#ifdef WISO_IMPL
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#if !defined(_POSIX_VERSION) || _POSIX_VERSION < 199309L
#  error POSIX.1b-1993 support is needed (-D_POSIX_C_SOURCE=199309)
#endif
#ifdef USE_THREADS
#  define GMTIME gmtime_r
#  define LOCALTIME localtime_r
typedef struct tm* (*TFUNC)(const time_t*, struct tm*);
#else
#  define GMTIME gmtime
#  define LOCALTIME localtime
typedef struct tm* (*TFUNC)(const time_t*);
#endif /* USE_THREADS */

#define WISO_FRAC (WISO_MSEC | WISO_USEC | WISO_NSEC)
#define WISO_FRAC_ISSET(f) (f & WISO_FRAC)
#define WISO_MSCHK(f) ((f & WISO_FRAC) == WISO_MSEC)
#define WISO_USCHK(f) ((f & WISO_FRAC) == WISO_USEC)
#define WISO_NSCHK(f) ((f & WISO_FRAC) == WISO_NSEC)
#define WISO_FRAC_CHK(f) (WISO_MSCHK(f) || WISO_USCHK(f) || WISO_NSCHK(f))
#define WISO_BUF (buf ? buf + tb : NULL)

static size_t _wiso_vspf(char* buf, size_t len, const char* fmt, ...)
{
	va_list arg;
	size_t wb = 0;
	char tmp[WISO_DATE_LEN + 2];	/* largest buffer + 'T' & '\0' */
	
	va_start(arg, fmt);
	wb = vsprintf(tmp, fmt, arg);
	va_end(arg);

	if (buf && len) {
		int l = len < wb ? len : wb;
		strncpy(buf, tmp, l)[l] = '\0';
	}

	return wb;
}
struct timespec* wiso_now(struct timespec* ts)
{
	static struct timespec _ts = { -1, -1 };

	if (!ts)
		ts = &_ts;

	if (clock_gettime(CLOCK_REALTIME, ts))
		return NULL;

	return _ts.tv_nsec == -1 ? ts : &_ts;
}
size_t wiso_str(char* buf, size_t len, int fmt, const struct timespec* ts)
{
	int i;
	size_t tb = 0;
	struct tm* tp;
	struct timespec t = *ts;
	TFUNC tfunc[] = { GMTIME, LOCALTIME };
	int mask[] = { WISO_DATE, WISO_TIME, WISO_FRAC, WISO_OFFS };
#ifdef USE_THREADS
	struct tm tm;
#endif

	if (!(fmt & WISO_DATETIME))
		fmt |= (WISO_DATETIME | WISO_OFFS);

	assert((!(fmt & WISO_FRAC) || (fmt & WISO_TIME)) && "fractions but no time");
	assert((!(fmt & WISO_FRAC) || WISO_FRAC_CHK(fmt)) && "fmt clash ms/us/ns");

#ifdef USE_THREADS
	if (!(tp = tfunc[!!(fmt & WISO_LOCL)](&t.tv_sec, &tm)))
#else
	if (!(tp = tfunc[!!(fmt & WISO_LOCL)](&t.tv_sec)))
#endif
		return -1;
	tp->tm_year += 1900;
	tp->tm_mon  +=    1;

	for (i = 0; i < (sizeof(mask) / sizeof(*mask)); i ++) {
		char* o;	/* offset string  */
		div_t z;	/* tz calculation */
		int w = 0;	/* fraction width */
		size_t wb = 0;	/* written bytes  */
		extern long timezone;
		switch (fmt & mask[i]) {
		case 0:
			continue;
		case WISO_DATE:
			wb = _wiso_vspf(WISO_BUF, len, "%04i-%02i-%02i",
			    tp->tm_year, tp->tm_mon, tp->tm_mday);
			break;
		case WISO_TIME:
			wb = _wiso_vspf(WISO_BUF, len, "%s%02i:%02i:%02i",
			    tb ? "T" : "", tp->tm_hour, tp->tm_min, tp->tm_sec);
			break;
		case WISO_MSEC:
			w += !!(fmt & WISO_MSEC) * 3;
			t.tv_nsec /= 1000;
			/* fall-through */
		case WISO_USEC:
			w += !!(fmt & WISO_USEC) * 6;
			t.tv_nsec /= 1000;
			/* fall-through */
		case WISO_NSEC:
			w += !!(fmt & WISO_NSEC) * 9;
			wb = _wiso_vspf(WISO_BUF, len, ".%0*li", (int)w, t.tv_nsec);
			break;
		case WISO_OFFS:
			o = (fmt & WISO_LOCL) ? "%+03i:%02i" : "Z";
			z = div(-timezone, 3600);
			wb = _wiso_vspf(WISO_BUF, len, o, z.quot, z.rem / 60);
			break;
		}
		tb += wb;
		len -= wb < tb ? wb : 0;
	}

	return tb;
}

#ifdef UNITTEST
#if !defined(_POSIX_VERSION) || _POSIX_VERSION < 200112
#  error "POSIX.1-2001 is needed for unittesting (-D_POSIX_C_SOURCE=200112)"
#endif
/**
 * gcc -O0 -g -o utest -xc -D_POSIX_C_SOURCE=200112 -DWISO_IMPL -DUNITTEST wiso.h && ./utest
 **/
int main(int argc, char** argv)
{
	char buf[WISO_LNISO_LEN + 1];
	const struct timespec ts = { 598961708, 141303120 };

	assert(wiso_niso(NULL, 0, &ts) == WISO_NISO_LEN);
	assert(wiso_date(buf, sizeof(buf), &ts)  == WISO_DATE_LEN);
	assert(!strcmp(buf, "1988-12-24"));
	assert(wiso_time(buf, sizeof(buf), &ts)  == WISO_TIME_LEN);
	assert(!strcmp(buf, "10:15:08"));
	assert(wiso_mtime(buf, sizeof(buf), &ts) == WISO_MTIME_LEN);
	assert(!strcmp(buf, "10:15:08.141"));
	assert(wiso_utime(buf, sizeof(buf), &ts) == WISO_UTIME_LEN);
	assert(!strcmp(buf, "10:15:08.141303"));
	assert(wiso_ntime(buf, sizeof(buf), &ts) == WISO_NTIME_LEN);
	assert(!strcmp(buf, "10:15:08.141303120"));
	assert(wiso_iso(buf, sizeof(buf), &ts) == WISO_ISO_LEN);
	assert(!strcmp(buf, "1988-12-24T10:15:08Z"));

	setenv("TZ", "GMT-1", 1);
	tzset();
	assert(wiso_liso(buf, sizeof(buf), &ts) == WISO_LISO_LEN);
	assert(!strcmp(buf, "1988-12-24T11:15:08+01:00"));
	assert(wiso_lmiso(buf, sizeof(buf), &ts) == WISO_LMISO_LEN);
	assert(!strcmp(buf, "1988-12-24T11:15:08.141+01:00"));
	assert(wiso_luiso(buf, sizeof(buf), &ts) == WISO_LUISO_LEN);
	assert(!strcmp(buf, "1988-12-24T11:15:08.141303+01:00"));
	assert(wiso_lniso(buf, sizeof(buf), &ts) == WISO_LNISO_LEN);
	assert(!strcmp(buf, "1988-12-24T11:15:08.141303120+01:00"));

	return EXIT_SUCCESS;
}
#endif /* UNITTEST */

#undef WISO_BUF
#undef WISO_FRAC_CHK
#undef WISO_NSCHK
#undef WISO_USCHK
#undef WISO_MSCHK
#undef WISO_FRAC_ISSET
#undef WISO_FRAC

#endif /* WISO_IMPL */
#endif /* WISO_H */

