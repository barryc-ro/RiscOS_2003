
/*************************************************************************
*
*   text.c
*
*   ICA 3.0 WinStation Driver - text packets
*
*   PACKET_CLEAR_SCREEN      1  clear screen
*   PACKET_CLEAR_EOL         3  clear to end of line
*   PACKET_SET_GLOBAL_ATTR   1  set attribute for raw packets
*   PACKET_RAW_WRITE0        1  write 1 bytes of raw data
*   PACKET_RAW_WRITE1        n  write n bytes of raw data (short)
*   PACKET_RAW_WRITE2        nn write n bytes of raw data (long)
*   PACKET_WRTCHARSTRATTR1   n  VioWrtCharStrAttr (short)
*   PACKET_WRTCHARSTRATTR2   nn VioWrtCharStrAttr (long)
*   PACKET_WRTNCELL1         5  VioWrtNCell (short)
*   PACKET_WRTNCELL2         6  VioWrtNCell (long)
*
*
*   Copyright 1994, Citrix Systems Inc.
*
*   Author: Brad Pedersen (4/9/94)
*
*   $Log$
*  
*     Rev 1.12   15 Apr 1997 18:18:02   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.12   21 Mar 1997 16:09:54   bradp
*  update
*  
*     Rev 1.11   04 May 1995 18:29:36   kurtp
*  update
*  
*     Rev 1.10   03 May 1995 11:46:38   kurtp
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
#include "citrix/ica.h"
#include "citrix/ica30.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

void IcaClearScreen( PWD, LPBYTE, USHORT );       
void IcaClearEol( PWD, LPBYTE, USHORT );          
void IcaSetGlobalAttr( PWD, LPBYTE, USHORT );     
void IcaRawWrite( PWD, LPBYTE, USHORT );          
void IcaWrtCharStrAttr( PWD, LPBYTE, USHORT );    
void IcaWrtNCell1( PWD, LPBYTE, USHORT );          
void IcaWrtNCell2( PWD, LPBYTE, USHORT );          


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

void WrtNCell( PWD, USHORT, LPBYTE, USHORT );


/*=============================================================================
==   Global Data
=============================================================================*/

extern WDTEXTMODE G_TextModes[];


/*******************************************************************************
 *
 *  IcaClearScreen
 *
 *  PACKET_CLEAR_SCREEN
 *  P1 - attribute
 *  
 *  This command clears the screen to spaces using the attribute P1.
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
IcaClearScreen( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    BYTE Cell[2];
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    Cell[0] = ' ';
    Cell[1] = (BYTE) *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "CLEAR_SCREEN, attr %x", Cell[1] ));

    rc = VioScrollUp( 0, 0, 0xFFFF, 0xFFFF, 0xFFFF, Cell, pIca->hVio );
    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaClearEol 
 *
 *  PACKET_CLEAR_EOL
 *  P1 - row
 *  P2 - column
 *  P3 - attribute
 *  
 *  This command clears to spaces from row P1 and column P2 to the end of 
 *  the line using the attribute P3.
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
IcaClearEol( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    BYTE Cell[2];
    USHORT Row;
    USHORT Col;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    Row = (USHORT) *pInputBuffer++;
    Col = (USHORT) *pInputBuffer++;

    Cell[0] = ' ';
    Cell[1] = (BYTE) *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "CLEAR_EOL: row=%u, col=%u, attr=%x", Row, Col, Cell[1] ));

    rc = VioWrtNCell( Cell,
                      (USHORT)(G_TextModes[pIca->TextIndex].Columns - Col),
                      Row, Col, 
                      pIca->hVio );

    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaSetGlobalAttr
 *
 *  PACKET_SET_GLOBAL_ATTR
 *  P1 - attribute
 *  
 *  This command sets the global attribute to P1.  This attribute is used 
 *  as the attribute for PACKET_RAW_WRITE0, PACKET_RAW_WRITE1 and 
 *  PACKET_RAW_WRITE2 commands.  The global attribute defaults to 0x07.
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
IcaSetGlobalAttr( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->GlobalAttr = *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "SET_GLOBAL_ATTR: 0x%02x", pIca->GlobalAttr ));
}


/*******************************************************************************
 *
 *  IcaRawWrite
 *
 *  PACKET_RAW_WRITE
 *  Pn - characters
 *  
 *  This command writes 'InputCount' bytes of character string Pn to the 
 *  screen, starting at the current cursor position. This command advances 
 *  the cursor as it writes each character, using the attribute that was set 
 *  by the PACKET_SET_GLOBAL_ATTR command for each character.  If the command 
 *  reaches the end of the line, it continues writing at the beginning of the 
 *  next line. It does not advance past the end of the screen.
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
IcaRawWrite( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    USHORT Row;
    USHORT Col;
    USHORT Offset;
    PWDICA pIca;
    int rc;
    USHORT MaxRow;
    USHORT MaxCol;

    pIca = (PWDICA) pWd->pPrivate;

    MaxRow = G_TextModes[pIca->TextIndex].Rows;
    MaxCol = G_TextModes[pIca->TextIndex].Columns;

    TRACE(( TC_WD, TT_API1, "RAW_WRITE0: bc=%u", InputCount ));

    rc = VioGetCurPos( &Row, &Col, pIca->hVio );
    ASSERT( rc == 0, rc );
    
    /* write the data */
    rc = VioWrtCharStrAtt( pInputBuffer, InputCount, Row, Col, &pIca->GlobalAttr, pIca->hVio );
    ASSERT( rc == 0, rc );
    
    /* position the cursor */
    Offset = (Row * MaxCol) + Col + InputCount;  // new screen offset
    if ( Offset >= (USHORT)(MaxCol * MaxRow) )
       Offset = (MaxCol * MaxRow) - 1;

    rc = VioSetCurPos( (USHORT)(Offset / MaxCol), (USHORT)(Offset % MaxCol), pIca->hVio );
    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaWrtCharStrAttr
 *
 *  PACKET_WRTCHARSTRATTR
 *  P1 - row
 *  P2 - column
 *  P3 - attribute
 *  Pn - character 
 *  
 *  This command writes 'InputCount-3' bytes of character string Pn to the 
 *  screen starting at row P1 and column P2 without moving the cursor, using 
 *  the attribute P3. If the string is longer than the current line, the 
 *  command continues writing it at the beginning of the next line. It does 
 *  not write past the end of the screen.  
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
IcaWrtCharStrAttr( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    USHORT Row;
    USHORT Col;
    BYTE Attr;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    Row = (USHORT) *pInputBuffer++;
    Col = (USHORT) *pInputBuffer++;
    Attr = *pInputBuffer++;
    InputCount -= 3;

    TRACE(( TC_WD, TT_API1, "WRTCHARSTRATTR1: bc=%u, row=%u, col=%u, attr=0x%02x",
            InputCount, Row, Col, Attr ));

    rc = VioWrtCharStrAtt( pInputBuffer, InputCount, Row, Col, &Attr, pIca->hVio );
    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaWrtNCell1 
 *
 *  PACKET_WRTNCELL1
 *  P1 - repeat count
 *  P2 - row
 *  P3 - column
 *  P4 - character
 *  P5 - attribute
 *  
 *  This command writes the cell P4/P5 to the screen P1 times starting 
 *  at row P2 and column P3 without moving the cursor. If the cell is 
 *  repeated more times than can fit on the current line, the command 
 *  continues writing it at the beginning of the next line. It does not 
 *  write past the end of the screen.  
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
IcaWrtNCell1( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    USHORT RepeatCount;

    RepeatCount = (USHORT) *pInputBuffer++;

    WrtNCell( pWd, RepeatCount, pInputBuffer, InputCount );
}


/*******************************************************************************
 *
 *  IcaWrtNCell2  (PACKET_WRTNCELL2)
 *
 *  PACKET_WRTNCELL2
 *  P1 - repeat count (high byte)
 *  P2 - repeat count (low byte)
 *  P3 - row
 *  P4 - column
 *  P5 - character
 *  P6 - attribute
 *  
 *  This command writes the cell P5/P6 to the screen P1:P2 times starting 
 *  at row P3 and column P4 without moving the cursor. If the cell is 
 *  repeated more times than can fit on the current line, the command 
 *  continues writing it at the beginning of the next line. It does not 
 *  write past the end of the screen.  
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
IcaWrtNCell2( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    USHORT RepeatCount;

    RepeatCount = (USHORT) *((PUSHORT)pInputBuffer);
    pInputBuffer += 2;

    WrtNCell( pWd, RepeatCount, pInputBuffer, InputCount );
}


/*******************************************************************************
 *
 *  WrtNCell
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    RepeatCount (input)
 *       repeat count
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
WrtNCell( PWD pWd, USHORT RepeatCount, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    USHORT Row;
    USHORT Col;
    BYTE Cell[2];
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    Row = (USHORT) *pInputBuffer++;
    Col = (USHORT) *pInputBuffer++;

    Cell[0]  = *pInputBuffer++;
    Cell[1]  = *pInputBuffer;

    TRACE(( TC_WD, TT_API1, 
            "WRTNCELL: repeat=%u, row=%u, col=%u, cell 0x%02x 0x%02x",
            RepeatCount, Row, Col, Cell[0], Cell[1] ));

    rc = VioWrtNCell( Cell, RepeatCount, Row, Col, pIca->hVio );
    ASSERT( rc == 0, rc );
}


