/* > string.c

 * Extra string functions

 */

#include "windows.h"

#include <ctype.h>
#include <limits.h>
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
    if (i == n) return 0;
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

char *strndup(const char *s, int maxlen)
{
    char *s1 = NULL;
    if (s)
    {
        const char *end = memchr(s, 0, maxlen);
        int len = end ? end - s : maxlen;
        s1 = malloc(len + 1);
        if (s1)
        {
            memcpy(s1, s, len);
            s1[len] = '\0';
        }
    }
    return s1;
}

char *strdup(const char *s)
{
    return strndup(s, INT_MAX);
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
