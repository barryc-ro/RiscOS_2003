/******************************************************************************
*
*  CLIB.H
*
*     Header file for C runtime library routines
*
*
*  Copyright Citrix Systems Inc. 1990
*
*  $Author:      Brad Pedersen
*
*   clib.h,v
*   Revision 1.1  1998/01/12 11:36:46  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.36   15 Apr 1997 18:44:50   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.35   06 May 1996 19:06:50   jeffm
*  update
*  
*     Rev 1.34   04 Dec 1995 16:11:14   bradp
*  update
*  
*     Rev 1.33   22 Jul 1995 17:18:06   richa
*  Added define memicmp.
*
*     Rev 1.32   19 Jul 1995 12:48:18   richa
*  Added Reserver memory buffer and check in malloc.
*
*     Rev 1.31   27 Jun 1995 21:19:32   bradp
*  update
*
*     Rev 1.30   24 May 1995 09:22:38   marcb
*  update
*
*     Rev 1.29   03 May 1995 12:16:16   butchd
*  update
*
*     Rev 1.28   02 May 1995 16:02:32   butchd
*  update
*
*     Rev 1.27   02 May 1995 13:38:58   butchd
*  update
*
*******************************************************************************/

#ifndef __CLIB_H__
#define __CLIB_H__

#include <ctype.h>
#include <time.h>

extern const char *strsafe(const char *s);
extern char *strdup(const char *s);
extern int strnicmp(const char *s1, const char *s2, int n);
extern int stricmp(const char *s1, const char *s2);
extern char *strupr(char *s);


#define strcmpi(a,b) stricmp(a,b)
#define memicmp(a,b,n) strncmp(a,b,n)

#define lstrcpy(a,b) strcpy(a,b)
#define lstrlen(a) strlen(a)
#define wsprintf sprintf
#define AnsiUpper(a) strupr(a)

#define Getmsec() (clock()*10)

extern void Delay(int t);

#define     ERROR_DEFAULT           0xffff
extern int LoadString( const char *base, int idResource, char *szBuffer, int cbBuffer );

#define GlobalAlloc(flags, size)	((HGLOBAL)malloc(size))
#define GlobalFree(ptr)			(free((void *)(ptr)), (HGLOBAL)0)
#define GlobalLock(ptr)			((void *)ptr)
#define GlobalUnlock(ptr)		

extern int read_word(void *a);
extern void write_word(void *a, int b);
extern int read_long(void *a);
extern void write_long(void *a, int b);

extern void *write_rect(void *out, const RECT *r);
extern void *read_rect(const void *in, RECT *r);

#endif //__CLIB_H__
