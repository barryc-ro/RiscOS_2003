/* -*-C-*- commonsrc/rdebug.c */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

#ifndef BUILDERS
#include "swis.h"
#include "wimp.h"
#endif

#include "version.h"

/* ----------------------------------------------------------------------------- */

#ifdef REMOTE_DEBUG

#include "debug/remote.h"

void *db_sess = NULL;

static void cleanup(void)
{
    remote_debug_close((debug_session *)db_sess);
}

static int debug_cmd_handler(int argc, char *argv[], void *handle)
{
    int handled = -1;

    if (strcasecomp(argv[0], "show") == 0)
    {
	if (argc == 2)
	{
#if MEMLIB
	    extern char   *flexptr__base;
	    extern char   *flexptr__free;
	    extern char   *flexptr__slot;
	    extern void   *heap__base;
	    extern void malloc_stats(void);
	    extern int malloc_size, malloc_da, heap__size, heap__da, flex__da;
#endif /* MEMLIB */
	    if (strcasecomp(argv[1], "dbg") == 0)
	    {
		dbglist();
		handled = 1;
	    }
#ifdef STBWEB
	    else if (strcasecomp(argv[1], "heap") == 0)
	    {
		heap__dump(NULL);
		handled = 1;
	    }
#endif /* STBWEB */
	    else if (strcasecomp(argv[1], "flex") == 0)
	    {
#if MEMLIB && defined(STBWEB)
 		MemFlex_Dump(NULL);
#endif /* MEMLIB */
		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "mm") == 0)
	    {
		mm__dump(NULL);
#if MEMLIB
		malloc_stats();
		DBG(("mall: msize %dK\n", malloc_size/1024));
#endif /* MEMLIB */
		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "mem") == 0)
	    {
		int us = -1, next = -1, free;

		wimp_slotsize(&us, &next, &free);
		DBG(("slot: size %dK total free %dK\n", us/1024, free/1024));

#if MEMLIB
		DBG(("flex: area %dK size %dK top %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), flex__da)/1024, (flexptr__slot - flexptr__base)/1024, (flexptr__free - flexptr__base)/1024));
		DBG(("heap: area %dK size %dK top %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), heap__da)/1024, heap__size/1024, ((int *)heap__base)[3]/1024));
		DBG(("mall: area %dK size %dK top -\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), malloc_da)/1024, malloc_size/1024));
#endif /* MEMLIB */

		handled = 1;
	    }
	}
    }
    else if (strcasecomp(argv[0], "openurl") == 0)
    {
	if (argc == 2)
	{
	    frontend_open_url(argv[1], NULL, NULL, NULL, 0);
	    handled = 1;
	}
    }
    else if (argc == 2)
	handled = debug_set(argv[0], atoi(argv[1]));

    return handled;
    handle = handle;
}

void rdebug_open(void)
{
    if (!db_sess)
    {
	remote_debug_open(PROGRAM_NAME, (debug_session **)&db_sess);
	if (db_sess)
	    remote_debug_register_cmd_handler((debug_session *)db_sess, debug_cmd_handler, NULL);
	atexit(cleanup);
    }
}

void rdebug_poll(void)
{
    debug_poll((debug_session *)db_sess);
}

/* ----------------------------------------------------------------------------- */

#else

void rdebug_open(void)
{
}


void rdebug_poll(void)
{
}

#endif

/* ----------------------------------------------------------------------------- */

/* eof rdebug.c */
