
#include "windows.h"

#include <stdio.h>
#include <string.h>

#include "../../inc/wfengapi.h"

#include "../../app/utils.h"

int LoadString( const char *base, int idResource, char *szBuffer, int cbBuffer )
{
    char tag[12], *s;
    sprintf(tag, "E%s%d:", base ? base : "", idResource);
    s = utils_msgs_lookup(tag);
    if (s && s[0])
    {
	strncpy(szBuffer, s, cbBuffer);
	szBuffer[cbBuffer-1] = '\0';
	return 1;
    }
    return 0;
}

