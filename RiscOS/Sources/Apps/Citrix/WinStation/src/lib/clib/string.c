/* > string.c

 * Extra string functions

 */

#include "windows.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/clib.h"

/* --------------------------------------------------------------------------------------------- */

const char *strsafe(const char *s)
{
    return s ? s : "";      
}

int strnicmp (const char *a, const char *b, int n)
{
    const char *p =a;
    const char *q =b;
    int i;
    for(p=a, q=b, i=0; *p && *q && i < n; p++, q++, i++) {
	int diff = tolower(*p) - tolower(*q);
	if (diff) return diff;
    }
    if (*p) return 1;	/* p was longer than q */
    if (*q) return -1;	/* p was shorter than q */
    return 0;		/* Exact match */
}

int stricmp (const char *a, const char *b)
{
    const char *p =a;
    const char *q =b;
    for(p=a, q=b; *p && *q; p++, q++) {
	int diff = tolower(*p) - tolower(*q);
	if (diff) return diff;
    }
    if (*p) return 1;	/* p was longer than q */
    if (*q) return -1;	/* p was shorter than q */
    return 0;		/* Exact match */
}

char *strdup(const char *s)
{
    char *ss;

    if (s == NULL)
	return NULL;

    ss = malloc(strlen(s)+1);
    if (ss)
	strcpy(ss, s);

    return ss;
}

char *strupr(char *s)
{
    int c;
    char *ss = s;
    while ((c = *ss) != 0)
	*ss++ = toupper(c);
    return s;
}

/* --------------------------------------------------------------------------------------------- */

/* eof string.c */
