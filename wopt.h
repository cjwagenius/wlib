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
 * --------------------------------------------------------------------------
 * Usage:
 *
 * #include <stdio.h>
 *
 * #define WOPT_IMPL
 * #include "wopt.h"
 *
 * int main(int argc, char* argv[])
 * {
 *     int o;
 *     int h;
 *     char *s;
 *
 *     while ((o = wopt(argv, "hs:")) != WOPT_DONE) {
 *         switch (o) {
 *	   case 's':
 *	       s = wopt_arg;
 *	       break;
 *	   case 'h':
 *	       h = 1;
 *	       break;
 *         default:
 *             if (o == WOPT_ENOARG)
 *                 fprintf(stderr, "missing argument for -%c\n", wopt_opt);
 *             else
 *                 fprintf(stderr, "invalid option -%c\n", wopt_opt);
 *     }
 *
 *     if (!(argv[wopt_idx]))
 *         /* no unnamed arguments */;
 *
 *     return 0;
 **/

#ifndef WOPT_H
#define WOPT_H

#define WOPT_DONE -1
#define WOPT_ENOARG ':'
#define WOPT_ENOOPT '?'

extern int   wopt_idx;
extern int   wopt_opt;
extern char *wopt_arg;

extern int wopt(char **argv, const char *opts);

#ifdef WOPT_IMPL
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int   wopt_idx = 0;
int   wopt_opt = 0;
char *wopt_arg = NULL;

int wopt(char **argv, const char *opts)
{
	char *opt;
	static char *arg = "";

	assert(opts != NULL);
	assert(argv != NULL && *argv != NULL);

	if (!*arg) {
		wopt_idx ++;
		if (!(arg = argv[wopt_idx])) {
			return WOPT_DONE;
		}
		if (arg[0] != '-' || arg[1] == '-') {
			wopt_idx += (arg[1] == '-');
			arg = "";
			return WOPT_DONE;
		}
		arg ++;
	}
	wopt_opt = *(arg++);
	if (!(opt = strchr(opts, wopt_opt))) {
		return WOPT_ENOOPT;
	}
	if (*++opt != ':') {
		wopt_arg = NULL;
		return wopt_opt;
	}
	if (*arg) {
		wopt_arg = arg;
		arg = "";
		return wopt_opt;
	}
	if (!argv[wopt_idx + 1]) {
		return WOPT_ENOARG;
	}
	wopt_arg = argv[++wopt_idx];

	return wopt_opt;
}

#endif /* WOPT_IMPL */
#endif /* WOPT_H */
