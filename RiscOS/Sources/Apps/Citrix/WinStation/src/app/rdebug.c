/* -*-C-*- commonsrc/rdebug.c */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "rdebug.h"

#include "version.h"
#include "utils.h"

#ifdef REMOTE_DEBUG
#include "debug/remote.h"
#endif

#include "../inc/clib.h"

/* ----------------------------------------------------------------------------- */

/* debugging routines that only depend on DEBUG state */

#ifdef DEBUG

static int dbg__priority = 0, dbg__line = 0;
static const char *dbg__file = NULL;

void vfdbg(void *f, const char *fmts, void *arglist)
{
#ifdef REMOTE_DEBUG
    void *r;
    if (f == NULL && (r = rdebug_session()) != 0)
	debug_dvprintf(r, dbg__priority, dbg__file, dbg__line, fmts, arglist);
    else
#endif
	vfprintf(f ? f : stderr, fmts, arglist);
}

void fdbg(void *f, const char *fmts, ...)
{
    va_list arglist;
    va_start(arglist, fmts);

    vfdbg(f, fmts, arglist);

    va_end(arglist);
}

void dbg(const char *fmts, ...)
{
    va_list arglist;
    va_start(arglist, fmts);

    vfdbg(NULL, fmts, arglist);

    va_end(arglist);
}

void dbghdr(int priority, const char *file, int line)
{
    dbg__priority = priority;
    dbg__file = file;
    dbg__line = line;
}

#endif

/* ----------------------------------------------------------------------------- */

#if defined(REMOTE_DEBUG) && defined(DEBUG)

static debug_session *db_sess = NULL;
static BOOL suspended = FALSE;

static void cleanup(void)
{
    remote_debug_close((debug_session *)db_sess);
}

#define HELP_INFO_1	"\n"

static int debug_cmd_handler(int argc, char *argv[], void *handle)
{
    int handled = -1;

    if (stricmp(argv[0], "help") == 0)
    {
	DBG(( HELP_INFO_1 ));
    }
    else if (stricmp(argv[0], "show") == 0)
    {
    }
    
    return handled;
    NOT_USED(handle);
}

void rdebug_open(void)
{
    if (!db_sess)
    {
	remote_debug_open(APP_NAME, &db_sess);
	if (db_sess)
	    remote_debug_register_cmd_handler(db_sess, debug_cmd_handler, NULL);
	atexit(cleanup);
    }
}

void rdebug_poll(void)
{
    debug_poll(db_sess);
}

void *rdebug_session(void)
{
    return suspended ? NULL : db_sess;
}

/* ----------------------------------------------------------------------------- */

#else

void rdebug_open(void)
{
}


void rdebug_poll(void)
{
}


void *rdebug_session(void)
{
    return NULL;
}

#endif

/* ----------------------------------------------------------------------------- */

/* eof rdebug.c */
