/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021
 *	Cameron Katri.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY CAMERON KATRI AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL CAMERON KATRI OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "common.h"

static void usage(const char *);

static const struct option long_options[] = {	    /* no clang-format */
	{ "help", no_argument, NULL, 'h' },	    /**/
	{ "bourne-shell", no_argument, NULL, 'b' }, /**/
	{ "sh", no_argument, NULL, 'b' },	    /**/
	{ "csh", no_argument, NULL, 'c' },	    /**/
	{ "c-shell", no_argument, NULL, 'c' },	    /**/
	{ NULL, no_argument, NULL, 0 }
};

static const char *long_types[38] = { "NORMAL", "NORM", "FILE", "RESET", "DIR",
	"LNK", "LINK", "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK",
	"BLK", "BLOCK", "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE",
	"RIGHT", "RIGHTCODE", "END", "ENDCODE", "SUID", "SETUID", "SGID",
	"SETGID", "STICKY", "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE",
	"OWT", "CAPABILITY", "MULTIHARDLINK", "CLRTOEOL", NULL };

static const char *short_types[38] = { "no", "no", "fi", "rs", "di", "ln", "ln",
	"ln", "or", "mi", "pi", "pi", "so", "bd", "bd", "cd", "cd", "do", "ex",
	"lc", "lc", "rc", "rc", "ec", "ec", "su", "su", "sg", "sg", "st", "ow",
	"ow", "tw", "tw", "ca", "mh", "cl", NULL };

static const int indexes[37] = { -1, -1, -1, -1, 0, 1, 1,
	1, -1, -1, 3, 3, 2, 5, 5, 6, 6, -1, 4,
	-1, -1, -1, -1, -1, -1, 7, 7, 8, 8, -1, 10,
	10, 9, 9, -1, -1, -1 };

int
main(int argc, char **argv)
{
	int ch;
	FILE *fd;
	char *prefix = "LS_COLORS='";
	char *suffix = "';\nexport LS_COLORS;";
	char *lsprefix = "LSCOLORS='";
	char *lssuffix = "';\nexport LSCOLORS;";

	while (
	    (ch = getopt_long(argc, argv, "hbc", long_options, NULL)) != -1) {
		switch (ch) {
		case 'b':
			prefix = "LS_COLORS='";
			suffix = "';\nexport LS_COLORS;";
			lsprefix = "LSCOLORS='";
			lssuffix = "';\nexport LSCOLORS;";
			break;
		case 'c':
			prefix = "setenv LS_COLORS '";
			suffix = "'";
			lsprefix = "setenv LSCOLORS '";
			lssuffix = "'";
			break;
		case 'h':
		case '?':
		default:
			usage(argv[0]);
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 1)
		usage(argv[1]);

	char *path = *argv;
	if (strcmp(path, "-") == 0)
		fd = stdin;
	else if ((fd = fopen(path, "r")) == NULL) {
		warnx("%s: %s\n", path, strerror(errno));
		return (errno);
	}

	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	char *ls_out = strdup("");
	char lsout[22] = "xxxxxxxxxxxxxxxxxxxxxx";
	struct color color;

	while ((linelen = getline(&line, &linecap, fd)) > 0) {
		if (*line == '#' || *line == '\n')
			continue;
		char fmttype[MAXKEYLEN] = "", val[MAXKEYLEN] = "";
		sscanf(line, "%s %s\n", fmttype, val);
		if (*line == '.') {
			sprintf(ls_out + strlen(ls_out), "*%s=%s:", fmttype, val);
			continue;
		} else if (*line == '*') {
			sprintf(ls_out + strlen(ls_out), "%s=%s:", fmttype, val);
			continue;
		}
		for (int i = 0; i < 37; i++) {
			if (strcmp(fmttype, long_types[i]) == 0) {
				sprintf(ls_out + strlen(ls_out), "%s=%s:", short_types[i], val);
				parseansi(val, &color);
				if (indexes[i] >= 0) {
					lsout[2 * indexes[i]] = color.fg;
					lsout[2 * indexes[i] + 1] = color.bg;
				}
				break;
			}
		}
	}
	fclose(fd);

	fprintf(stdout, "%s%s%s\n", prefix, ls_out, suffix);
	fprintf(stdout, "%s%s%s\n", lsprefix, lsout, lssuffix);

	free(line);
	free(ls_out);

	return (0);
}

static void
usage(const char *progname)
{
	char *path;
	path = strdup(progname);

	(void)fprintf(stderr, "usage: %s [-bch] [FILE]\n", basename(path));
	exit(EX_USAGE);
}
