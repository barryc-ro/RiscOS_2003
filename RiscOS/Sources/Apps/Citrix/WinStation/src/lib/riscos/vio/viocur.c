
/*************************************************************************
*
*  VIOCUR.C
*
*  Video cursor routines.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry (3/28/1994)
*
*  $Log$
*  
*     Rev 1.6   15 Apr 1997 18:51:32   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   03 May 1995 11:15:46   butchd
*  clib.h now standard
*
*************************************************************************/

#include "windows.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <string.h>

/*  Get CLIB includes */
#include "../../../inc/client.h"
#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/vioapi.h"

#include "swis.h"

/*=============================================================================
 ==   Functions Used
 ============================================================================*/

/*=============================================================================
 ==   Local Variables
 ============================================================================*/

static USHORT yStart;
static USHORT cEnd;
static USHORT attr = 0x0000;

/*=============================================================================
 ==   Global Variables
 ============================================================================*/

extern unsigned int usMaxRow;
extern unsigned int usMaxCol;

/*****************************************************************************
*
*  FUNCTION: Get Cursor Position
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioGetCurPos (PUSHORT pusRow, PUSHORT pusColumn, HVIO hvio)
{
    int x, y;
    _swix(OS_Byte, _IN(0) | _OUTR(1,2), 134, &x, &y);
    *pusRow = y;
    *pusColumn = x;

#if 0
    union REGS    regs;

   regs.x.ax = 0x0300;
   regs.x.bx = 0x00;
   int86(0x10, &regs, &regs );

   *pusRow    = regs.h.dh;
   *pusColumn = regs.h.dl;
#endif
   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Get Cursor Position
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioSetCurPos (USHORT usRow, USHORT usColumn, HVIO hvio)
{
    char s[3];
    s[0] = 31;
    s[1] = usColumn;
    s[2] = usRow;
    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));

#if 0
    union REGS    regs;

   regs.x.ax = 0x0200;
   regs.x.bx = 0x00;
   regs.h.dh = (unsigned char) usRow;
   regs.h.dl = (unsigned char) usColumn;

   int86(0x10, &regs, &regs );
#endif
   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Get Cursor Type
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioGetCurType (PVIOCURSORINFO pvioCursorInfo, HVIO hvio)
{
    pvioCursorInfo->yStart = yStart;
    pvioCursorInfo->cEnd   = cEnd;
#if 0
    PCH pbROM = (LPBYTE) 0x00400060;

   if ( attr == 0xffff ) {
      pvioCursorInfo->yStart = yStart;
      pvioCursorInfo->cEnd   = cEnd;
   }
   else {
      pvioCursorInfo->cEnd   = (USHORT) ((BYTE) *(pbROM));
      pvioCursorInfo->yStart = (USHORT) ((BYTE) *(pbROM+1));
   }
#endif
   pvioCursorInfo->cx     = 1;
   pvioCursorInfo->attr   = attr;

   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Set Cursor Type
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioSetCurType (PVIOCURSORINFO pvioCursorInfo, HVIO hvio)
{
#if 0
    union REGS regs;
   PCH        pbROM = (LPBYTE) 0x00400060;
   USHORT     lattr=attr;
#endif
   
   /* save the requested start/end */
   yStart = pvioCursorInfo->yStart;
   cEnd   = pvioCursorInfo->cEnd;
   attr   = pvioCursorInfo->attr;

#if 0
   /* hide cursor and note state */
   if ( attr == 0xffff ) {
      regs.x.cx = 0x2000;
   }
   else {
      regs.h.ch = (BYTE) (pvioCursorInfo->yStart & 0x0f);
      regs.h.cl = (BYTE) (pvioCursorInfo->cEnd   & 0x0f);
   }

   /* do int 10h function 01h for cursor setting */
   regs.x.ax = 0x0100;
   int86( 0x10, &regs, &regs );
#endif
   
   /* no return from int 10h */
   return( CLIENT_STATUS_SUCCESS );
}

