
#include "windows.h"
#include "mem.h"

#include <stdio.h>
#include <string.h>

#include "../../inc/wfengapi.h"

#include "../../app/utils.h"

int LoadString( const char *base, int idResource, char *szBuffer, int cbBuffer )
{
    char tag[12], *s;
    int success = 0;

    sprintf(tag, "E%s%d:", base ? base : "", idResource);
    
    s = utils_msgs_lookup(tag);

    if (s)
    {
	MEMCHECK_PUSH();

	if (s[0])
	{
	    strncpy(szBuffer, s, cbBuffer);
	    szBuffer[cbBuffer-1] = '\0';

	    success = 1;
	}

	MEMCHECK_POP();
    }

    return success;
}

