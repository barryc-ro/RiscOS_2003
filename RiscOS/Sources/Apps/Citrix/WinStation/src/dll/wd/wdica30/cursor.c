
/*************************************************************************
*
*   cursor.c
*  
*   ICA 3.0 WinStation Driver - cursor packets
*  
*    PACKET_SETCUR_POSITION   2  set cursor position (row,column)
*    PACKET_SETCUR_ROW        1  set cursor row
*    PACKET_SETCUR_COLUMN     1  set cursor column
*    PACKET_SETCUR_SIZE       1  set cursor size
*  
*  
*   Copyright 1994, Citrix Systems Inc.
*  
*   Author: Brad Pedersen (4/9/94)
*  
*   $Log$
*  
*     Rev 1.10   15 Apr 1997 18:17:48   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.9   11 Jul 1995 13:56:08   kurtp
*  update
*  
*     Rev 1.8   07 Jul 1995 11:39:46   kurtp
*  update
*  
*     Rev 1.7   04 May 1995 18:28:34   kurtp
*  update
*  
*     Rev 1.6   03 May 1995 11:45:38   kurtp
*  update
*    
*************************************************************************/


/*
 *  Includes
 */
#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"
#include "citrix/ica30.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

void IcaSetCursorPosition( PWD, LPBYTE, USHORT ); 
void IcaSetCursorRow( PWD, LPBYTE, USHORT );      
void IcaSetCursorColumn( PWD, LPBYTE, USHORT );   
void IcaSetCursorSize( PWD, LPBYTE, USHORT );     


/*=============================================================================
==   Local Data
=============================================================================*/

//unsigned char far * pCursorMode = (unsigned char far *) 0x00400060;


/*******************************************************************************
 *
 *  IcaSetCursorPosition 
 *
 *  PACKET_SETCUR_POSITION
 *  P1 - row
 *  P2 - column
 *  
 *  This command moves the cursor to row P1 and column P2.
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaSetCursorPosition( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    USHORT Row;
    USHORT Col;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    Row = (USHORT) *pInputBuffer++;
    Col = (USHORT) *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "SETCURPOS: row=%u, col=%u", Row, Col ));

    rc = VioSetCurPos( Row, Col, pIca->hVio );
    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaSetCursorRow
 *
 *  PACKET_SETCUR_ROW
 *  P1 - row
 *  
 *  This command moves the cursor to row P1 without changing the column.
 *
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaSetCursorRow( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    USHORT Row;
    USHORT Col;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    rc = VioGetCurPos( &Row, &Col, pIca->hVio );
    ASSERT( rc == 0, rc );

    Row = (USHORT) *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "SETCURROW: row=%u, col=%u", Row, Col ));

    rc = VioSetCurPos( Row, Col, pIca->hVio );
    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaSetCursorColumn 
 *
 *  PACKET_SETCUR_COLUMN
 *  P1 - column
 *  
 *  This command moves the cursor to column P1 without changing the row.
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaSetCursorColumn( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    USHORT Row;
    USHORT Col;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    rc = VioGetCurPos( &Row, &Col, pIca->hVio );
    ASSERT( rc == 0, rc );

    Col = (USHORT) *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "SETCURCOL: row=%u, col=%u", Row, Col ));

    rc = VioSetCurPos( Row, Col, pIca->hVio );
    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaSetCursorSize  (PACKET_SETCUR_SIZE)
 *
 *  PACKET_SETCUR_SIZE
 *  P1 - cursor size in percent (0-100)
 *  
 *  This command set the cursor size.  A value of 0 for P1 denotes a 
 *  hidden cursor.
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaSetCursorSize( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
#if 0
    PWDICA pIca;
    VIOCURSORINFO CursorInfo;
    VIOMODEINFO ModeInfo;
    USHORT CellHeight;
    USHORT CursorHeight;
    USHORT Size;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    Size = (USHORT) *pInputBuffer;    // 0-100 percent
    
    ModeInfo.cb = 12;
    rc = VioGetMode( &ModeInfo, pIca->hVio );
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

    //  firewall to keep divide by zero out
    if ( rc ) 
        return;
    
    CellHeight = ModeInfo.vres / ModeInfo.row;
    
    /*
     * if video bios does not support this vres
     * figure it out by looking in bios data area
     */
    if ( CellHeight == 0 )
       CellHeight = (unsigned char) ((*pCursorMode) + 1);

    CursorHeight = (CellHeight * Size) / 100;

    CursorInfo.yStart = CellHeight - CursorHeight;  // top
    CursorInfo.cEnd   = CellHeight - 1;             // bottom
    CursorInfo.cx     = 1;
    CursorInfo.attr   = (Size>0) ? 0x0000 : 0xffff;

    TRACE(( TC_WD, TT_API1, 
           "SETCURSIZE: size %u, cellh %u, curh %u, start %u, end %u, attr %u",
            Size, CellHeight, CursorHeight, CursorInfo.yStart, 
            CursorInfo.cEnd, CursorInfo.attr ));

    rc = (int) VioSetCurType( &CursorInfo, pIca->hVio );
    ASSERT( rc == 0, rc );
#endif
}
