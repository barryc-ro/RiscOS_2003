/* -*-C-*- commonsrc/debug.c */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

#define DBGPROTO(x) extern void x(const char *fmt, ...)

#ifdef REMOTE_DEBUG

static void dbglist(void);

#include "debug/remote.h"

static debug_session *db_sess = NULL;

static void cleanup(void)
{
    remote_debug_close(db_sess);
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
#endif
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
#endif
	    else if (strcasecomp(argv[1], "flex") == 0)
	    {
#if MEMLIB
		DBG(("flex: free %p slot %p usage %dK\n", flexptr__free, flexptr__slot, (flexptr__slot - flexptr__base)/1024 - 32));
#endif
		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "mm") == 0)
	    {
		mm__dump(NULL);
		handled = 1;
	    }
	    else if (strcasecomp(argv[1], "mem") == 0)
	    {
		int us = -1, next = -1, free;
#if MEMLIB
		DBG(("flex: da usage %dK\n", (flexptr__slot - flexptr__base)/1024 - 32));
#endif

		wimp_slotsize(&us, &next, &free);
		DBG(("slot: us %dK next %dK free %dK\n", us/1024, next/1024, free/1024));

		heap__info(NULL);
		
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

#define DBGFNDEF(x,y) extern void x(const char *fmts, ...) \
{ if (dbg_conf[y].present) \
{ va_list arglist; va_start(arglist, fmts); if (db_sess) debug_vprintf(db_sess, fmts, arglist); else vfprintf(stderr, fmts, arglist);\
va_end(arglist); } }

void dbg(const char *fmts, ...)
{
    va_list arglist;
    va_start(arglist, fmts);
    if (db_sess)
	debug_vprintf(db_sess, fmts, arglist);
    else
	vfprintf(stderr, fmts, arglist);
    va_end(arglist);
}

void fdbg(void *f, const char *fmts, ...)
{
    va_list arglist;
    va_start(arglist, fmts);
    if (f == NULL && db_sess)
	debug_vprintf(db_sess, fmts, arglist);
    else
	vfprintf(f ? f : stderr, fmts, arglist);
    va_end(arglist);
}

#else

#define DBGFNDEF(x,y) extern void x(const char *fmts, ...) \
{ if (dbg_conf[y].present) \
{ va_list arglist; va_start(arglist, fmts); vfprintf(stderr, fmts, arglist); \
fflush(stderr);	va_end(arglist); } }

void dbg(const char *fmts, ...)
{
    va_list arglist;
    va_start(arglist, fmts);
    vfprintf(stderr, fmts, arglist);
    va_end(arglist);
}

#endif

#if DEBUG

#if 0
#pragma -v1
DBGPROTO(tabdbg);
DBGPROTO(prsdbg);
DBGPROTO(rendbg);
DBGPROTO(dicdbg);
DBGPROTO(cnfdbg);
DBGPROTO(imgdbg);
DBGPROTO(ckidbg);
DBGPROTO(accdbg);
DBGPROTO(bendbg);
DBGPROTO(lnkdbg);
DBGPROTO(tabdbgn);
DBGPROTO(prsdbgn);
DBGPROTO(rendbgn);
DBGPROTO(dicdbgn);
DBGPROTO(cnfdbgn);
DBGPROTO(imgdbgn);
DBGPROTO(ckidbgn);
DBGPROTO(accdbgn);
DBGPROTO(bendbgn);
DBGPROTO(lnkdbgn);
DBGPROTO(laydbgn);
#pragma -v0
#endif

typedef struct { char *name; int present; } dbg_conf_item;

static dbg_conf_item dbg_conf[]=
{
    { "TABDBG", 0 },
    { "TABDBGN", 0 },
    { "OBJDBG", 0 },
    { "OBJDBGN", 0 },
    { "PRSDBG", 0 },
    { "PRSDBGN", 0 },
    { "PPDBG", 0 },
    { "PPDBGN", 0 },
    { "RENDBG", 0 },
    { "RENDBGN", 0 },
    { "DICDBG", 0 },
    { "DICDBGN", 0 },
    { "CNFDBG", 0 },
    { "CNFDBGN", 0 },
    { "FMTDBG", 0 },
    { "FMTDBGN", 0 },
    { "IMGDBG", 0 },
    { "IMGDBGN", 0 },
    { "CKIDBG", 0 },
    { "CKIDBGN", 0 },
    { "ACCDBG", 0 },
    { "ACCDBGN", 0 } ,
    { "STBDBG", 0 },
    { "STBDBGN", 0 },
    { "BENDBG", 0 },
    { "BENDBGN", 0 },
    { "LNKDBG", 0 },
    { "LNKDBGN", 0 },
    { "LAYDBG", 0 },
    { "LAYDBGN", 0 }
};

enum
{
    tab,
    tabn,
    obj,
    objn,
    prs,
    prsn,
    pp,
    ppn,
    ren,
    renn,
    dic,
    dicn,
    cnf,
    cnfn,
    fmt,
    fmtn,
    img,
    imgn,
    cki,
    ckin,
    acc,
    accn,
    stb,
    stbn,
    ben,
    benn,
    lnk,
    lnkn,
    lay,
    layn
};

static void dbglist(void)
{
    TABDBG(("Table debugging present\n"));
    TABDBGN(("Excessive Table debugging present\n"));
    OBJDBG(("Object debugging present\n"));
    OBJDBGN(("Excessive Object debugging present\n"));
    PRSDBG(("Parser debugging present\n"));
    PRSDBGN(("Excessive Parser debugging present\n"));
    PPDBG(("Parser progress debugging present\n"));
    PPDBGN(("Excessive Parser progress debugging present\n"));
    RENDBG(("Rendering image debugging present\n"));
    RENDBGN(("Excessive Rendering image debugging present\n"));
    DICDBG(("Debug image change debugging present\n"));
    DICDBGN(("Excessive Debug image change debugging present\n"));
    CNFDBG(("Configuration debugging present\n"));
    CNFDBGN(("Excessive configuration debugging present\n"));
    FMTDBG(("Formatting debugging present\n"));
    FMTDBGN(("Excessive formatting debugging present\n"));
    IMGDBG(("Image debugging present\n"));
    IMGDBGN(("Excessive image debugging present\n"));
    CKIDBG(("Cookie debugging present\n"));
    CKIDBGN(("Excessive cookie debugging present\n"));
    ACCDBG(("Access debugging present\n"));
    ACCDBGN(("Excessive access debugging present\n"));
    STBDBG(("NCFresco debugging present\n"));
    STBDBGN(("Excessive NCFresco debugging present\n"));
    BENDBG(("HTTPSave debugging present\n"));
    BENDBGN(("Excessive HTTPSave debugging present\n"));
    LNKDBG(("Link debugging present\n"));
    LNKDBGN(("Excessive link debugging present\n"));
    LAYDBG(("Frame debugging present\n"));
    LAYDBGN(("Excessive frame debugging present\n"));
}

extern void dbginit(void)
{
    dbg_conf_item *ptr = dbg_conf;
    dbg_conf_item *end = ptr + sizeof(dbg_conf) / sizeof(dbg_conf_item);

    while (ptr < end)
    {
	ptr->present = getenv(ptr->name) != NULL;
	ptr++;
    }

    dbglist();
    
#ifdef REMOTE_DEBUG
    if (!db_sess)
    {
	remote_debug_open("NCFresco", &db_sess);
	if (db_sess)
	    remote_debug_register_cmd_handler(db_sess, debug_cmd_handler, NULL);
	atexit(cleanup);
    }
#endif
}

extern int debug_set(const char *feature, int enable)
{
    int ix;

    for (ix = 0; ix < sizeof(dbg_conf) / sizeof(dbg_conf_item); ix++)
    {
	/*if ( strcasecmp(dbg_conf[ix].name, feature) == 0 )*/
	if ( strcmp(dbg_conf[ix].name, feature) == 0 )
	{
	    dbg_conf[ix].present = enable;
	    break;
	}
    }

    return ix != sizeof(dbg_conf) / sizeof(dbg_conf_item);
}

extern int debug_get(const char *feature)
{
    int ix;

    for (ix = 0; ix < sizeof(dbg_conf) / sizeof(dbg_conf_item); ix++)
    {
	/*if ( strcasecmp(dbg_conf[ix].name, feature) == 0 )*/
	if ( strcmp(dbg_conf[ix].name, feature) == 0 )
	{
	    return dbg_conf[ix].present;
	}
    }

    return 0;
}

extern void dbgpoll(void)
{
#ifdef REMOTE_DEBUG
    debug_poll(db_sess);
#endif
}

/* **** N.B.  These don't need semi-colons at the end as they define functions */
DBGFNDEF(tabdbg, tab)
DBGFNDEF(tabdbgn, tabn)
DBGFNDEF(objdbg, obj)
DBGFNDEF(objdbgn, objn)
DBGFNDEF(prsdbg, prs)
DBGFNDEF(prsdbgn, prsn)
DBGFNDEF(ppdbg, pp)
DBGFNDEF(ppdbgn, ppn)
DBGFNDEF(rendbg, ren)
DBGFNDEF(rendbgn, renn)
DBGFNDEF(dicdbg, dic)
DBGFNDEF(dicdbgn, dicn)
DBGFNDEF(cnfdbg, cnf)
DBGFNDEF(cnfdbgn, cnfn)
DBGFNDEF(fmtdbg, fmt)
DBGFNDEF(fmtdbgn, fmtn)
DBGFNDEF(imgdbg, img)
DBGFNDEF(imgdbgn, imgn)
DBGFNDEF(ckidbg, cki)
DBGFNDEF(ckidbgn, ckin)
DBGFNDEF(accdbg, acc)
DBGFNDEF(accdbgn, accn)
DBGFNDEF(stbdbg, stb)
DBGFNDEF(stbdbgn, stbn)
DBGFNDEF(bendbg, ben)
DBGFNDEF(bendbgn, benn)
DBGFNDEF(lnkdbg, lnk)
DBGFNDEF(lnkdbgn, lnkn)
DBGFNDEF(laydbg, lay)
DBGFNDEF(laydbgn, layn)
    
#else	/* DEBUG */

extern void dbginit(void)
{

}

extern void dbgpoll(void)
{
}

#endif /* DEBUG */

/* eof debug.c */
