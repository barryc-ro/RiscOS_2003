/* -*-c-*- */

#include <string.h>
#include <stdio.h>

#include "debug.h"
#include "version.h"

static char versionstr[] = VERSION_NUMBER " (aa-bbb-cc (xxyy)) " VERSION_QUALIFIER;

char *program_name = PROGRAM_NAME;
char *program_title = PROGRAM_TITLE;

char *version_number = VERSION_NUMBER;
char *version_qual = VERSION_QUALIFIER;

extern char *fresco_time;
extern char *fresco_date;

char *fresco_getversion( void )
{
    char *pos = strchr( versionstr, '(' ) + 1;

#if DEBUG
    fprintf( stderr, "Built %s %s\n", fresco_date, fresco_time );
#endif

    /* fresco_date is "Mmm dd yyyy" (mandated by ANSI)
     * we want dd-Mmm-yy
     */

    memcpy( pos, fresco_date+4, 2 );
    memcpy( pos+3, fresco_date, 3 );
    memcpy( pos+7, fresco_date+9, 2 );

    memcpy( pos+11, fresco_time, 2 );
    memcpy( pos+13, fresco_time+3, 2 );

    return versionstr;
}
