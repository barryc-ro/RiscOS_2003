/* > wdemul.h

 * This file contains the extra defines required of all WD's for RISCOS use.

 */

#ifndef __WDEMUL_H
#define __WDEMUL_H

/* This structure is to provide access to transport and protocl specific functions */

#define WDEMUL__OPEN		0
#define WDEMUL__CLOSE		1
#define WDEMUL__INFO		2
#define WDEMUL__PROCESSINPUT	3
#define WDEMUL__POLL		4
#define WDEMUL__SETINFORMATION	5
#define WDEMUL__SETINFO		6
#define WDEMUL__QUERYINFO	7
#define WDEMUL__OUTBUFALLOC	8
#define WDEMUL__OUTBUFERROR	9
#define WDEMUL__OUTBUFFREE	10
#define WDEMUL__WRITETTY	11
#define WDEMUL__COUNT		12

typedef int FNWDEMULOPEN( PWD, PWDOPEN );
typedef FNWDEMULOPEN *PFNWDEMULOPEN;

typedef int FNWDEMULCLOSE( PWD );
typedef FNWDEMULCLOSE *PFNWDEMULCLOSE;

typedef int FNWDEMULINFO( PWD, PDLLINFO );
typedef FNWDEMULINFO *PFNWDEMULINFO;

typedef int FNWDEMULPROCESSINPUT( PWD );
typedef FNWDEMULPROCESSINPUT *PFNWDEMULPROCESSINPUT;
typedef int FNWDEMULPOLL( PWD, PDLLPOLL, int );
typedef FNWDEMULPOLL *PFNWDEMULPOLL;

typedef int FNWDEMULSETINFORMATION( PWD, PWDSETINFORMATION );
typedef FNWDEMULSETINFORMATION *PFNWDEMULSETINFORMATION;

typedef int FNWDEMULSETINFO( PWD, SETINFOCLASS, LPBYTE, USHORT );
typedef FNWDEMULSETINFO *PFNWDEMULSETINFO;
typedef int FNWDEMULQUERYINFO( PWD, QUERYINFOCLASS, LPBYTE, USHORT );
typedef FNWDEMULQUERYINFO *PFNWDEMULQUERYINFO;

typedef int FNWDEMULOUTBUFALLOC( PWD, POUTBUF * );
typedef FNWDEMULOUTBUFALLOC *PFNWDEMULOUTBUFALLOC;
typedef void FNWDEMULOUTBUFERROR( PWD, POUTBUF );
typedef FNWDEMULOUTBUFERROR *PFNWDEMULOUTBUFERROR;
typedef void FNWDEMULOUTBUFFREE( PWD, POUTBUF );
typedef FNWDEMULOUTBUFFREE *PFNWDEMULOUTBUFFREE;

typedef void FNWDEMULWRITETTY( PWD, LPBYTE, USHORT );
typedef FNWDEMULWRITETTY *PFNWDEMULWRITETTY;

#ifndef NO_WDEMUL_DEFINES

#define EmulOpen(a,b)		((PFNWDEMULOPEN)((a)->pEmulProcedures[WDEMUL__OPEN]))(a,b)
#define EmulClose(a)		((PFNWDEMULCLOSE)((a)->pEmulProcedures[WDEMUL__CLOSE]))(a)
#define EmulInfo(a,b)		((PFNWDEMULINFO)((a)->pEmulProcedures[WDEMUL__INFO]))(a,b)

#define EmulProcessInput(a)	((PFNWDEMULPROCESSINPUT)((a)->pEmulProcedures[WDEMUL__PROCESSINPUT]))(a)
#define EmulPoll(a,b,c)		((PFNWDEMULPOLL)((a)->pEmulProcedures[WDEMUL__POLL]))(a,b,c)

#define EmulSetInformation(a,b)	((PFNWDEMULSETINFORMATION)((a)->pEmulProcedures[WDEMUL__SETINFORMATION]))(a,b)

#define EmulSetInfo(a,b,c,d)	((PFNWDEMULSETINFO)((a)->pEmulProcedures[WDEMUL__SETINFO]))(a,b,c,d)
#define EmulQueryInfo(a,b,c,d)	((PFNWDEMULQUERYINFO)((a)->pEmulProcedures[WDEMUL__QUERYINFO]))(a,b,c,d)

#define EmulOutBufAlloc(a,b)	((PFNWDEMULOUTBUFALLOC)((a)->pEmulProcedures[WDEMUL__OUTBUFALLOC]))(a,b)
#define EmulOutBufError(a,b)	((PFNWDEMULOUTBUFERROR)((a)->pEmulProcedures[WDEMUL__OUTBUFERROR]))(a,b)
#define EmulOutBufFree(a,b)	((PFNWDEMULOUTBUFFREE)((a)->pEmulProcedures[WDEMUL__OUTBUFFREE]))(a,b)

#define EmulWriteTTY(a,b,c)	((PFNWDEMULWRITETTY)((a)->pEmulProcedures[WDEMUL__WRITETTY]))(a,b,c)

#endif

/*---------------------------------------------------------------------------------------------------- */

#endif
