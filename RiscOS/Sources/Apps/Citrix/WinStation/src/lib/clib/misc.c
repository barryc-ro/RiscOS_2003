/* > misc.c

 *

 */


#include "windows.h"

#include <string.h>

#include "../../inc/client.h"

#include "../../inc/wdapi.h"
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
    
void *write_rect(void *out, const RECT *rect)
{
    WDRCL rcl;
    char *p = out;
 
    // do the first point
    rcl.x = rect->left;
    rcl.y = rect->top;
    memcpy(p, &rcl, sizeof_WDRCL);
    
    // do the second point
    rcl.x = rect->right;
    rcl.y = rect->bottom;
    memcpy(p + 3, &rcl, sizeof_WDRCL);

    return p + 6;
}

void *read_rect(const void *in, RECT *rect)
{
    WDRCL rcl;
    const char *p = in;

    // do the first point
    memcpy(&rcl, p, sizeof_WDRCL);
    rect->left = rcl.x;
    rect->top = rcl.y;
    
    // do the second point
    memcpy(&rcl, p + 3, sizeof_WDRCL);
    rect->right = rcl.x;
    rect->bottom = rcl.y;

    return (void *)(p + 6);
}

/* eof misc.c */
