
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
*  viocur.c,v
*  Revision 1.1  1998/01/12 11:37:37  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
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

#include "vio.h"

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
    cursor_get(&x, &y);
    if (pusColumn)
	*pusColumn = x;
    if (pusRow)
	*pusRow = y;
    return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Set Cursor Position
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioSetCurPos (USHORT usRow, USHORT usColumn, HVIO hvio)
{
    cursor_to(usColumn, usRow);
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
    char s[10];
   
    /* save the requested start/end */
    yStart = pvioCursorInfo->yStart;
    cEnd   = pvioCursorInfo->cEnd;
    attr   = pvioCursorInfo->attr;
   
    memset(s, 0, sizeof(s));
    s[0] = 23;

    /* hide cursor and note state */
    if ( attr == 0xffff ) {
	s[2] = 10;
	s[3] = 1<<5;
	_swix(OS_WriteI, _INR(0,1), s, sizeof(s));
    }
    else {
	s[2] = 10;
	s[3] = yStart;
	_swix(OS_WriteI, _INR(0,1), s, sizeof(s));

	s[2] = 11;
	s[3] = cEnd;
	_swix(OS_WriteI, _INR(0,1), s, sizeof(s));
    }
   
    /* no return from int 10h */
    return( CLIENT_STATUS_SUCCESS );
}

