
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "debug.h"

#define DBGPROTO(x) extern void x(const char *fmt, ...)
#define DBGFNDEF(x,y) extern void x(const char *fmts, ...) \
{ if (dbg_conf[y].present) \
{ va_list arglist; va_start(arglist, fmts); vfprintf(stderr, fmts, arglist); \
fflush(stderr);	va_end(arglist); } }

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
DBGPROTO(htsdbg);
DBGPROTO(tabdbgn);
DBGPROTO(prsdbgn);
DBGPROTO(rendbgn);
DBGPROTO(dicdbgn);
DBGPROTO(cnfdbgn);
DBGPROTO(imgdbgn);
DBGPROTO(ckidbgn);
DBGPROTO(accdbgn);
DBGPROTO(htsdbgn);
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
    { "HTSDBG", 0 },
    { "HTSDBGN", 0 }
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
    hts,
    htsn
};

extern void dbginit(void)
{
    dbg_conf_item *ptr = dbg_conf;
    dbg_conf_item *end = ptr + sizeof(dbg_conf) / sizeof(dbg_conf_item);

    while (ptr < end)
    {
	ptr->present = getenv(ptr->name) != NULL;
	ptr++;
    }

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
    HTSDBG(("HTTPSave debugging present\n"));
    HTSDBGN(("Excessive HTTPSave debugging present\n"));
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
DBGFNDEF(htsdbg, hts)
DBGFNDEF(htsdbgn, htsn)

#else	/* DEBUG */

extern void dbginit(void)
{

}

#endif /* DEBUG */

/* eof debug.c */
