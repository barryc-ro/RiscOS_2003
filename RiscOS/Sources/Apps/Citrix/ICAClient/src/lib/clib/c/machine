/* > machine.c

 *

 */


#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "swis.h"

#include "../../inc/client.h"
#include "../../inc/clib.h"

/*
 * Read the configured keyboard (country) number
 * Convert to a country name
 * Return success
 */

int GetLocalKeyboard(char *s, int len)
{
    int layout, out, outlen;
    if (LOGERR(_swix(OS_Byte, _INR(0,1) | _OUT(1), 71, 255, &layout)) != NULL)
	return CLIENT_ERROR;

    if (LOGERR(_swix(OS_ServiceCall, _INR(1,5) | _OUT(1) | _OUT(5), 0x43, 2, layout, s, len, &out, &outlen)) != NULL)
	return CLIENT_ERROR;

    s[outlen] = '\0';

    return out == 0 ? CLIENT_STATUS_SUCCESS : CLIENT_ERROR;
}

static BOOL check_front_and_back(const char *path, const char *match)
{
    int plen = strlen(path);
    int mlen = strlen(match);
    return plen > mlen &&
	(strnicmp(path, match, mlen) == 0 || stricmp(path + plen - mlen, match));
}

int GetPrinterType( void )
{
    int n;
    char buffer[32], *s;
    int ptype = printer_OTHER;

    if (_swix(OS_Byte, _INR(0,2) | _OUT(1), 245, 0, 255, &n) == NULL)
    {
	sprintf(buffer, "PrinterType$%d", n);
	s = getenv(buffer);
	if (s)
	{
	    if (check_front_and_back(s, "null"))
		ptype = printer_NONE;
	    else if (check_front_and_back(s, "parallel") || check_front_and_back(s, "fastparallel"))
		ptype = printer_PARALLEL;
	    else if (check_front_and_back(s, "serial"))
		ptype = printer_SERIAL;
	}
	else 
	{
	    ptype = n >= 3 ? printer_OTHER : n;
	}
    }

    return ptype;
}

/* eof machine.c */
