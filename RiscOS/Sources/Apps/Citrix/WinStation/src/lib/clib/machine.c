/* > machine.c

 *

 */


#include "windows.h"

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
    int layout, out;
    return LOGERR(_swix(OS_Byte, _INR(0,1) | _OUT(1), 71, 255, &layout)) == NULL &&
	LOGERR(_swix(OS_ServiceCall, _INR(1,5) | _OUT(1), 0x43, 2, layout, s, len, &out)) == NULL &&
	out == 0 ? CLIENT_STATUS_SUCCESS : CLIENT_ERROR;
}


/* eof machine.c */
