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
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "common.h"

void usage(const char *);
char *tols_colors(char *);

int
main(int argc, char **argv)
{
	if (argc != 2)
		usage(argv[0]);

	char *input = NULL;
	if (strcmp(argv[1], "-") == 0) {
		char *buf = NULL;
		size_t linecap = 0;
		ssize_t linelen;
		getline(&buf, &linecap, stdin);
		if (buf[sizeof(buf) - 1] == '\n')
			buf[sizeof(buf) - 1] = '\0';
		input = strdup(buf);
		free(buf);
	} else
		input = argv[1];

	if (strchr(input, '='))
		fprintf(stdout, "%s\n", tolscolors(input));
	else
		fprintf(stdout, "%s\n", tols_colors(input));

	return (0);
}

char *
tols_colors(char *lscolors)
{
	char *ls_out = strdup("");

	for (int i = 0; i < 11; i++) {
		if (lscolors[2 * i] == 'x' && lscolors[2 * i + 1] == 'x')
			continue;
		sprintf(ls_out + strlen(ls_out), "%s=", types[i]);
		if (isupper(lscolors[2 * i]))
			sprintf(ls_out + strlen(ls_out), "01;");
		if (tolower(lscolors[2 * i]) == 'x')
			sprintf(ls_out + strlen(ls_out), "00");
		else if (tolower(lscolors[2 * i] != 'x'))
			sprintf(ls_out + strlen(ls_out), "3%i",
			    (int)(strchr(col, tolower(lscolors[2 * i])) - col));
		if (tolower(lscolors[2 * i + 1]) != 'x')
			sprintf(ls_out + strlen(ls_out), ";4%i",
			    (int)(strchr(col, tolower(lscolors[2 * i])) - col));
		sprintf(ls_out + strlen(ls_out), ":");
	}
	char *ret = strdup(ls_out);
	free(ls_out);
	return (ret);
}

void
usage(const char *progname)
{
	char *path;
	path = strdup(progname);

	(void)fprintf(stderr, "usage: %s LSCOLORS|LS_COLORS\n", basename(path));
	exit(EX_USAGE);
}
