
/***************************************************************************
*
*  CLIENT.H
*
*  This module contains common client defines and structures
*
*  Copyright 1994, Citrix Systems Inc.
*
*  Author: Brad Pedersen  (3/16/94)
*
*  $Log$
*  
*     Rev 1.55   11 Mar 1998 12:54:44   kalyanv
*  added the ifdefs around define _HAS_ALIGNED
*  
*     Rev 1.54   01 Mar 1998 14:35:18   TOMA
*  ce merge
*
*     Rev 1.52   08 Jan 1998 16:56:18   brada
*  Added typedefs
*
*     Rev 1.51   15 Apr 1997 18:44:52   TOMA
*  autoput for remove source 4/12/97
*
*     Rev 1.50   16 May 1996 13:10:40   marcb
*  update
*
*     Rev 1.49   16 May 1996 12:25:20   marcb
*  update
*
*     Rev 1.48   10 May 1996 13:48:30   marcb
*  update
*
*     Rev 1.47   06 May 1996 19:14:38   thanhl
*  update
*
*     Rev 1.45   20 Dec 1995 09:52:26   bradp
*  update
*
*     Rev 1.44   12 Jun 1995 11:25:24   marcb
*  update
*
*     Rev 1.43   30 May 1995 08:26:52   butchd
*  update
*
****************************************************************************/

#ifndef __CLIENT_H__
#define __CLIENT_H__

/* disable inline assembly warning */
/* #pragma warning(disable:4704) */

/*=============================================================================
 ==   defines
 ============================================================================*/

#define MSB(x)     ((unsigned char) (((x) >> 8) & 0x00ff))
#define LSB(x)     ((unsigned char) ((x) & 0x00ff))

#ifndef STATIC
#define STATIC
#endif

#if defined(WIN32) || defined(WINCE)
// Used for alignment purposes.
// When using UNALIGNED RISC
// imlementations will break access
// down to byte stores and fetches.

#define _HAS_UNALIGNED_
#endif

#ifdef _HAS_UNALIGNED_

typedef WORD far UNALIGNED *LPUNALIGNED_WORD;

#define UNALIGNED_MOVE_SHORT(p, q)     \
   *(LPUNALIGNED_WORD)(p) = *(LPUNALIGNED_WORD)(q);

#else //#ifdef _HAS_UNALIGNED_

#define BYTE_PTR(p)        \
   ((char *)(p))

#define UNALIGNED_MOVE_SHORT(p, q)     \
   BYTE_PTR(p)[0] = BYTE_PTR(q)[0]; \
   BYTE_PTR(p)[1] = BYTE_PTR(q)[1];

#endif

#define UNALIGNED_ASSIGN_SHORT(p, q)   \
   {                          \
      short i = q;               \
      UNALIGNED_MOVE_SHORT(p, &i);  \
   }


// hook procedure structure
typedef struct _FNHOOK {
   void * pProcedure;
   struct _FNHOOK * pNext;
} FNHOOK, far * PFNHOOK, far * far *PPFNHOOK;


/*=============================================================================
 ==   Include externalized (WinFrame ENGINE) defines and structures
 ============================================================================*/

#include "wfengapi.h"


/*=============================================================================
 ==   DLL Emulation
 ============================================================================*/

#include "dll.h"


/*=============================================================================
 ==   Include debug ASSERT and TRACE macros for all internal client modules
 ============================================================================*/

#include "debug.h"

#endif //__CLIENT_H__
