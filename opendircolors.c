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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "common.h"

void usage(void);

#define MAXKEYLEN 21

static const struct option long_options[] = {	    /* no clang-format */
	{ "help", no_argument, NULL, 'h' },	    /**/
	{ "bourne-shell", no_argument, NULL, 'b' }, /**/
	{ "sh", no_argument, NULL, 'b' },	    /**/
	{ "csh", no_argument, NULL, 'c' },	    /**/
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

int
main(int argc, char **argv)
{
	int ch;
	char *prefix = "LS_COLORS='";
	char *suffix = "';\nexport LS_COLORS";
	char *lsprefix = "LSCOLORS='";
	char *lssuffix = "';\nexport LSCOLORS";

	while (
	    (ch = getopt_long(argc, argv, "hbc", long_options, NULL)) != -1) {
		switch (ch) {
		case 'b':
			prefix = "LS_COLORS='";
			suffix = "';\nexport LS_COLORS";
			lsprefix = "LSCOLORS='";
			lssuffix = "';\nexport LSCOLORS";
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
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	char *path = *argv != NULL ? *argv++ : "-";
	FILE *fd = fopen(path, "r");

	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	char *out = strdup("");
	while ((linelen = getline(&line, &linecap, fd)) > 0) {
		if (*line == '#' || *line == '\n')
			continue;
		char fmttype[MAXKEYLEN] = "", val[MAXKEYLEN] = "";
		sscanf(line, "%s %s\n", fmttype, val);
		if (*line == '.') {
			sprintf(out + strlen(out), "*%s=%s:", fmttype, val);
			continue;
			;
		} else if (*line == '*') {
			sprintf(out + strlen(out), "%s=%s:", fmttype, val);
			continue;
		}
		for (int i = 0; i < 37; i++) {
			if (strcmp(fmttype, long_types[i]) == 0) {
				sprintf(out + strlen(out),
				    "%s=%s:", short_types[i], val);
				break;
			}
		}
	}
	free(line);
	printf("%s%s%s\n", prefix, out, suffix);
	printf("%s%s%s\n", lsprefix, tolscolors(out), lssuffix);
	free(out);

	/* NOTREACHED */
	return (0);
}

void
usage(void)
{
	(void)fprintf(stderr, "usage: %s [-bch] [FILE]\n", getprogname());
	exit(EX_USAGE);
}
