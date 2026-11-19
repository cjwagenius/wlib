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
 * Written and made public by C.J.Wagenius - 2023-01-09
 * ----------------------------------------------------------------------------
 *
 * A tiny ANSI C opt/arg parser
 *
 *     Parses options and arguments in a POSIXLY_CORRECT manner, (option- and
 *     nonoption arguments can not be mixed).
 *
 *
 * Instructions:
 *
 *     This is a single header-file to include in projects. In one source file
 *     per project, e.g. main.c, define WOPT_IMPL and include this header after
 *     that. The code will then be compiled with main.c. When that's done, you
 *     can include this file as usual in any other file where the functions are
 *     needed.
 *
 *         Example:
 *
 *              main.c:
 *             ----------------------------------------
 *              #define WOPT_IMPL
 *              #include "wopt.h"
 *
 *
 * Misc. macros:
 *
 *     WOPT_DONE	- Parsing is done
 *     WOPT_ENOARG	- An option argument is missing
 *     WOPT_ENOOPT	- An invalid option found
 *
 *
 * Global variables:
 *
 *     wopt_idx		- While parsing, points to current argv-index. After
 *                        scanning is done, it points to the first non-option
 *                        argument in argv.
 *     wopt_opt		- Current option value beeing parsed.
 *     wopt_arg		- While parsing, points to an option argument if one
 *                        is expected.
 *     wopt_prg		- If this variable is not already set, and if argv[0]
 *                        is not NULL, it's set to the last path component
 *                        in argv[0].
 *
 *
 * Functions:
 *
 *     int wopt(const char** argv, const char* opts)
 *
 *         Parses [argv] after options, and option arguments, specified by
 *         [opts]. If argv[0] is not NULL, [wopt_prg] is set to point to the
 *         last path component of its string value. Option parsing stops when
 *         when the first non-option agument is found, and it's not allowed
 *         to mix option- and non-option arguments.
 *
 *         [opts] is a string with option characters, followed with a colon
 *         (':') if the option requires an argument.
 *
 *         At use, multiple options that do not require arguments may be given
 *         at once, (-afg). And an option argument may, or may not, follow the
 *         the option character directly (-omyfile).
 *
 *         Returns the current option character or WOPT_DONE (0) on success.
 *         WOPT_NOOPT is returned if a character that's not in [opts] was
 *         found. WOPT_NOARG is returned if a valid option was found, but its
 *         required argument was missing.
 *
 *
 * Example:
 *
 *     int opt;
 *     char* fn_out;
 *
 *     while ((opt = wopt(argv, "ho:")) != WOPT_DONE) {
 *             switch (opt) {
 *             case 'h':
 *                     print_help();
 *                     break;
 *             case 'o':
 *                     fn_out = wopt_arg;
 *                     break;
 *             default:
 *                     if (wopt_opt == '?')
 *                             fprintf(stderr, "invalid option, -%c", wopt_opt);
 *                     else
 *                             fprintf(stderr, "missing argument, -%c", wopt_opt);
 *                     exit(EXIT_FAILURE);
 *             }
 *     }
 *
 **/

#ifndef WOPT_H
#define WOPT_H

#define WOPT_DONE   0x00
#define WOPT_ENOARG 0x01
#define WOPT_ENOOPT 0x02

extern int   wopt_idx;
extern int   wopt_opt;
extern char* wopt_arg;
extern char* wopt_prg;

extern int wopt(char **argv, const char *opts);

#ifdef WOPT_IMPL
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#  define WOPT_PATHSEP '\\'
#else
#  define WOPT_PATHSEP '/'
#endif

int   wopt_idx = 0;
int   wopt_opt = 0;
char* wopt_arg = NULL;
char* wopt_prg = NULL;

int wopt(char** argv, const char* opts)
{
	char* opt;
	static char* arg = "";

	assert(opts != NULL);
	assert(argv != NULL);

	if (!wopt_prg && *argv) {
		if ((wopt_prg = strrchr(*argv, WOPT_PATHSEP)))
			wopt_prg ++;
		else
			wopt_prg = *argv;
	}

	if (!*arg) {
		wopt_idx ++;
		if (!(arg = argv[wopt_idx]))
			return (arg = "", WOPT_DONE);
		if (arg[0] != '-' || arg[1] == '-') {
			wopt_idx += (arg[1] == '-');
			return (arg = "", WOPT_DONE);
		}
		arg ++;
	}
	wopt_opt = *(arg ++);
	if (!(opt = strchr(opts, wopt_opt)))
		return WOPT_ENOOPT;
	if (*++opt != ':')
		return (wopt_arg = NULL, wopt_opt);
	if (*arg)
		return (wopt_arg = arg, arg = "", wopt_opt);
	if (!argv[wopt_idx + 1])
		return WOPT_ENOARG;
	wopt_arg = argv[++ wopt_idx];

	return wopt_opt;
}

#endif /* WOPT_IMPL */
#endif /* WOPT_H */
