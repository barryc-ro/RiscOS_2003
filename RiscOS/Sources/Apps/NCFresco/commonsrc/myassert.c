
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "wimp.h"
#include "dbox.h"
#include "menu.h"
#include "interface.h"
#include "myassert.h"
#include "debug.h"

#include "guarded.h"

extern void __ASSERT(int v, const char *expr, const char *file, int line)
{
        if (!v)
        {
                os_error e;
		fflush(stderr);
		DBG(("Assertion failure: %s, %s, %d\n", expr, file, line));
		fflush(stderr);

                memset(&e, 0, sizeof(e));
                sprintf(e.errmess, "Internal error: %s: line %d of %s", expr, line, file);

#if CATCHTYPE5
		if ( sig_guarded() )
		{
		    frontend_complain( &e );
		    raise( SIGUSR2 );
		}
#endif
                frontend_fatal_error(&e);
                /* Never reaches here */
                exit(-1);
        }
}

/* eof myassert.c */
