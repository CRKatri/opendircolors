#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

const char *types[11] = { "di", "ln", "so", "pi", "ex", "bd", "cd", "su", "sg",
	"tw", "ow" };
const char col[8] = "abcdefgh";

int tolscolors(char *);
int tols_colors(char *);
void usage(void);
char numtocol(char, bool);

int
main(int argc, char **argv)
{
	if (argc != 2)
		usage();

	if (strchr(argv[1], '='))
		return tolscolors(argv[1]);
	else
		return tols_colors(argv[1]);

	/* NOTREACHED */
	return 0;
}

int
tolscolors(char *dircolor)
{
	char *color, *ent, *buf, *val, *val2;
	char out[22] = "xxxxxxxxxxxxxxxxxxxxxx";
	bool bold = false;

	while ((ent = buf = strsep(&dircolor, ":")) != NULL) {
		for (int i = 0; i < 11; i++) {
			if (strncmp(ent, types[i], strlen(types[i])) == 0) {
				bold = false;
				while ((val = strsep(&buf, "=")) != NULL) {
					while ((val2 = strsep(&val, ";")) !=
					    NULL) {
						if (strcmp(val2, "01") == 0) {
							bold = true;
						} else if (val2[0] == '3') {
							out[2 * i] = numtocol(
							    val2[1], bold);
						} else if (val2[0] == '4') {
							out[2 * i + 1] =
							    numtocol(
								val2[1], NULL);
						}
					}
				}
			}
		}
	}
	printf("%s\n", out);
	return 0;
}

char
numtocol(char c, bool bold)
{
	char buf = col[strtol(&c, NULL, 10)];
	if (bold)
		return toupper(buf);
	else
		return buf;
}

int
tols_colors(char *lscolors)
{
	char *ls_out;
	strcpy(ls_out, "");

	for (int i = 0; i < 11; i++) {
		if (lscolors[2 * i] == 'x' && lscolors[2 * i + 1] == 'x')
			continue;
		sprintf(ls_out + strlen(ls_out), "%s=", types[i]);
		if (isupper(lscolors[2 * i]))
			sprintf(ls_out + strlen(ls_out), "01;");
		if (tolower(lscolors[2 * i])  == 'x')
			sprintf(ls_out + strlen(ls_out), "00");
		else if (tolower(lscolors[2 * i] != 'x'))
			sprintf(ls_out + strlen(ls_out), "3%i", (int)(strchr(col, tolower(lscolors[2 * i])) - col));
		if (tolower(lscolors[2 * i + 1])  != 'x')
			sprintf(ls_out + strlen(ls_out), ";4%i", (int)(strchr(col, tolower(lscolors[2 * i])) - col));
		sprintf(ls_out + strlen(ls_out), ":");
	}
	printf("%s\n", ls_out);
	return 0;
}

void
usage(void)
{
	(void)fprintf(stderr, "%s\n", "usage: lscolors format_code ");
	exit(EX_USAGE);
}
