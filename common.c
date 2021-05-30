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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "common.h"

struct color *
parseansi(char *ansi, struct color *color)
{
	long long num;
	char *token;
	const char *errstr;
	bool bold = false, underline = false;

	color->fg = 'x';
	color->bg = 'x';

	while ((token = strsep(&ansi, ";")) != NULL) {
		num = strtonum(token, 0, 47, NULL);
		switch (num) {
			case 1:
				bold = true;
				break;
			case 4:
				underline = true;
				break;
			default:
				if (num <= 37)
					color->fg = (char)((num - 30) + 97);
				else if (num <= 47)
					color->bg = (char)((num - 40) + 97);
		}
	}
	if (bold)
		color->fg = toupper(color->fg);
	if (underline)
		color->bg = toupper(color->bg);

	return color;
}
