/* -*-c-*- */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "os.h"
#include "msgs.h"
#include "makeerror.h"
#include "swis.h"

/**********************************************************************/

os_error *makeerror(int n)
{
    os_error err;
    char *m;

    err.errnum = n + ANTWEB_ERROR_BASE;
    sprintf(err.errmess, "err%03d", n);
    m = msgs_lookup(err.errmess);
    if (m != err.errmess)
	strncpy(err.errmess, m, 251);
    err.errmess[251] = 0;

    return os_swi1(os_X | MessageTrans_CopyError, (int)&err);
}

/* SJM */
os_error *makeerrorf(int n, ...)
{
    os_error err;
    va_list ap;

    err.errnum = n + ANTWEB_ERROR_BASE;
    sprintf(err.errmess, "err%03d", n);

    va_start(ap, n);
    vsprintf(err.errmess, msgs_lookup(err.errmess), ap);
    va_end(ap);

    return os_swi1(os_X | MessageTrans_CopyError, (int)&err);
}
