/* > misc.c

 *

 */

#include "../../inc/clib.h"

int read_word(void *a)
{
    const char *s = a;
    return s[0] + (s[1] << 8);
}

void write_word(void *a, int b)
{
    char *s = a;
    s[0] = b;
    s[1] = b >> 8;
}

int read_long(void *a)
{
    const char *s = a;
    return s[0] + (s[1] << 8) + (s[2] << 16) + (s[3] << 24);
}

void write_long(void *a, int b)
{
    char *s = a;
    s[0] = b;
    s[1] = b >> 8;
    s[2] = b >> 16;
    s[3] = b >> 24;
}
    
/* eof misc.c */
