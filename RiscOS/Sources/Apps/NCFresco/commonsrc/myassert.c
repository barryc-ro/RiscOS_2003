
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wimp.h"
#include "dbox.h"
#include "menu.h"
#include "interface.h"
#include "myassert.h"


extern void __ASSERT(int v, char *expr, char *file, int line)
{
        if (!v)
        {
                os_error e;
                memset(&e, 0, sizeof(e));
                sprintf(e.errmess, "Internal error: %s: line %d of %s", expr, line, file);
                frontend_fatal_error(&e);
                /* Never reaches here */
                exit(-1);
        }
}

/* eof myassert.c */
