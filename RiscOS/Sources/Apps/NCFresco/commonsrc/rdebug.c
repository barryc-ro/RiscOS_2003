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

static void *db_sess = NULL;
static BOOL suspended = FALSE;

static void cleanup(void)
{
    remote_debug_close((debug_session *)db_sess);
}

static void dump_dir(const char *dir, int level)
{
    char buffer[512];
    int nread, offset = 0;

    do
    {
	int i;
	char *s;

	if ((os_error *)_swix(OS_GBPB, _INR(0,6) | _OUTR(3,4),
			      12, dir, buffer, 75, offset, sizeof(buffer), NULL,
			      &nread, &offset) != NULL)
	    break;
	
	for (i = 0, s = buffer; i < nread; i++, s += ((24 + strlen(s+24)+1 + 3) &~ 3))
	{
	    int size = ((int *)s)[2];
	    int attr = ((int *)s)[3];
	    int type = ((int *)s)[4];
	    char *file = s + 24;
	    
	    if (type == 2)
	    {
		char newdir[256];
		strcpy(newdir, dir);
		strcat(newdir, ".");
		strcat(newdir, file);

		DBG((" directory: %s\n", newdir));

		dump_dir(newdir, level+1);
	    }
	    else
	    {
		DBG(("%10d%c%c %s\n", size, type == 1 ? ':' : '*', attr & (1<<3) ? 'L' : ' ', file));
	    }
	}
    }
    while (offset != -1);
}

#define HELP_INFO_1	"\n" \
	"close <handle>   close given file handle\n" \
	"help             show this information\n" \
	"openurl <url>    fetch and display the given URL\n" \
	"ramdisc <size>   change ramdisc size by given amount (in Kbytes)\n" \
	"suspend 0|1      turn debugging off and on\n" \
	"show dbg         show debugging options enabled\n" \
	"show heap        dump image heap\n" \
	"show flex        dump flex info to stderr\n" \
	"show mm          dump mm_malloc heap\n" \
	"show mem         show memory summaries\n" \
	"show da          show dynamic areas\n" \
	"show files       show open files\n" \
	"show cache       show cache structures\n" \
	"show scrap       show scrap directory\n" \
	"show history     show history list\n" \
	"show plugins     list plugins\n" \
	"show view <name> show info on named frame (__top for main)\n"

#define HELP_INFO_2	"\n" \
	"ACCDBG[N] 1|0    access debugging\n" \
	"DICDBG[N] 1|0    image change debugging\n" \
	"TABDBG[N] 1|0    tables debugging\n" \
	"OBJDBG[N] 1|0    plugin debugging\n" \
	"PRSDBG[N] 1|0    parser debugging\n" \
	"PPDBG[N]  1|0    parser debugging\n" \
	"RENDBG[N] 1|0    renderer debugging\n" \
	"CNFDBG[N] 1|0    configure debugging\n" \
	"FMTDBG[N] 1|0    formatter debugging\n" \
	"IMGDBG[N] 1|0    image debugging\n" \
	"CKIDBG[N] 1|0    cookie debugging\n" \
	"STBDBG[N] 1|0    frontend debugging\n" \
	"BENDBG[N] 1|0    backend debugging\n" \
	"LNKDBG[N] 1|0    highlight debugging\n" \
	"LAYDBG[N] 1|0    frames debugging\n"

static int debug_cmd_handler(int argc, char *argv[], void *handle)
{
    int handled = -1;

    if (strcasecomp(argv[0], "help") == 0)
    {
	DBG(( HELP_INFO_1 ));
	DBG(( HELP_INFO_2 ));
    }
    else if (strcasecomp(argv[0], "show") == 0)
    {
	if (argc >= 2)
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
		int us, free;

		_swix(Wimp_SlotSize, _INR(0,1) | _OUT(0) | _OUT(2), -1, -1, &us, &free);
		DBG(("slot: size %dK total free %dK\n", us/1024, free/1024));

#if MEMLIB
		DBG(("flex: area %dK size %dK top %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), flex__da)/1024, (flexptr__slot - flexptr__base)/1024, (flexptr__free - flexptr__base)/1024));
		DBG(("heap: area %dK size %dK top %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), heap__da)/1024, heap__size/1024, ((int *)heap__base)[3]/1024));
		DBG(("mall: area %dK size %dK top -\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), malloc_da)/1024, malloc_size/1024));
#endif /* MEMLIB */

		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "da") == 0)
	    {
		int area = -1;
		do
		{
		    _swix(OS_DynamicArea, _INR(0,1) | _OUT(1), 3, area, &area);
		    if (area != -1)
		    {
			int size;
			char *name;
			_swix(OS_DynamicArea, _INR(0,1) | _OUT(2) | _OUT(8), 2, area, &size, &name);
			DBG(("da: size %10d name '%s'\n", size, name));
		    }
		}
		while (area != -1);
	    }
	    else if (strcasecomp(argv[1], "files") == 0)
	    {
		int fh;
		for (fh = 255; fh > 0; fh--)
		{
		    char buffer[256];
		    int left;
		    if (_swix(OS_Args, _INR(0,2)|_IN(5) | _OUT(5), 7, fh, buffer, sizeof(buffer), &left) == NULL &&
			left != sizeof(buffer))
		    {
			DBG(("file: %3d '%s'\n", fh, buffer));
		    }
		}
		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "cache") == 0)
	    {
		extern void cache_debug(void);
		cache_debug();
		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "scrap") == 0)
	    {
		char *s = getenv("Wimp$ScrapDir");
		dump_dir(s, 0);
		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "plugins") == 0)
	    {
		extern void plugin_dump(void);
		plugin_dump();
		handled = 1;
	    }
#ifdef STBWEB
	    else if (strcasecomp(argv[1], "history") == 0)
	    {
		extern void history_dump(BOOL global);
		history_dump(FALSE);
		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "view") == 0)
	    {
		if (argc >= 3)
		{
		    extern void view_dump(const char *name);
		    view_dump(argv[2]);
		    handled = 1;
		}
	    }
#endif
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
    else if (strcasecomp(argv[0], "suspend") == 0)
    {
	if (argc == 2)
	{
	    suspended = atoi(argv[1]) != 0;
	    handled = 1;
	}
    }
    else if (strcasecomp(argv[0], "close") == 0)
    {
	if (argc == 2)
	{
	    int fh = atoi(argv[1]);
	    handled = _swix(OS_File, _INR(0,1), 0, fh) == NULL;
	}
    }
    else if (strcasecomp(argv[0], "ramdisc") == 0)
    {
	if (argc == 2)
	{
	    int size = atoi(argv[1])*1024;
	    int out = -1;
	    _swix(OS_ChangeDynamicArea, _INR(0,1) | _OUT(1), 5, size, &out);
	    handled = out;
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
