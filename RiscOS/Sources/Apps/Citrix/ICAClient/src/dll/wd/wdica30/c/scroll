
/*************************************************************************
*
*   scroll.c
*   
*   ICA 3.0 WinStation Driver - scroll packets
*   
*    PACKET_SCROLL_SCREEN   1  scroll screen up 1 line
*    PACKET_SCROLLUP        7  VioScrollUp
*    PACKET_SCROLLUP1       0  VioScrollUp - 1 line using previous
*    PACKET_SCROLLUP2       1  VioScrollUp - n lines using previous
*    PACKET_SCROLLDN        7  VioScrollDn
*    PACKET_SCROLLDN1       0  VioScrollDn - 1 line using previous
*    PACKET_SCROLLDN2       1  VioScrollDn - n lines using previous
*    PACKET_SCROLLLF        7  VioScrollLf
*    PACKET_SCROLLLF1       0  VioScrollLf - 1 line using previous
*    PACKET_SCROLLLF2       1  VioScrollLf - n lines using previous
*    PACKET_SCROLLRT        7  VioScrollRt
*    PACKET_SCROLLRT1       0  VioScrollRt - 1 line using previous
*    PACKET_SCROLLRT2       1  VioScrollRt - n lines using previous
*   
*   Copyright 1994, Citrix Systems Inc.
*   
*   Author: Brad Pedersen (4/9/94)
*   
*   $Log$
*  
*     Rev 1.9   15 Apr 1997 18:18:00   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.8   04 May 1995 18:29:28   kurtp
*  update
*  
*     Rev 1.7   03 May 1995 11:46:20   kurtp
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
#include "../../../inc/vioapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

void IcaScrollScreen( PWD, LPBYTE, USHORT );      

void IcaScrollUp( PWD, LPBYTE, USHORT );          
void IcaScrollUp1( PWD, LPBYTE, USHORT );         
void IcaScrollUpN( PWD, LPBYTE, USHORT );         

void IcaScrollDown( PWD, LPBYTE, USHORT );        
void IcaScrollDown1( PWD, LPBYTE, USHORT );       
void IcaScrollDownN( PWD, LPBYTE, USHORT );       

void IcaScrollLeft( PWD, LPBYTE, USHORT );        
void IcaScrollLeft1( PWD, LPBYTE, USHORT );       
void IcaScrollLeftN( PWD, LPBYTE, USHORT );       

void IcaScrollRight( PWD, LPBYTE, USHORT );       
void IcaScrollRight1( PWD, LPBYTE, USHORT );      
void IcaScrollRightN( PWD, LPBYTE, USHORT );      


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

void ScrollGetParams( PWD, LPBYTE );
void ScrollRect( PWD, USHORT );


/*******************************************************************************
 *
 *  IcaScrollScreen 
 *
 *  PACKET_SCROLL_SCREEN
 *  P1 - attribute
 *
 *  scroll entire screen up 1 line filling with specified attribute
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
IcaScrollScreen( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    BYTE Cell[2];
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    Cell[0] = ' ';
    Cell[1] = *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "SCROLL_SCREEN: attr=%x", Cell[1] ));

    rc = VioScrollUp( 0, 0, 0xFFFF, 0xFFFF, 1, Cell, pIca->hVio );
    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaScrollUp  
 *
 *  PACKET_SCROLLUP
 *  P1 - top row
 *  P2 - left column
 *  P3 - bottom row
 *  P4 - right column
 *  P5 - character
 *  P6 - attribute
 *  P7 - number of lines
 *  
 *  This command scrolls the data contained within the scroll window 
 *  P1,P2,P3,P4 upward P7 lines.  The area left blank is filled by the 
 *  character/attribute pair P5/P6. If a value is specified that is greater 
 *  than the maximum for P1, P2, P3, P4, or P7, the maximum value for that 
 *  parameter is used.  Maximum values depend upon the dimensions of the
 *  screen being used.
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
IcaScrollUp( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    ScrollGetParams( pWd, pInputBuffer );
    ScrollRect( pWd, VIO__SCROLLUP );
}


/*******************************************************************************
 *
 *  IcaScrollUp1
 *
 *  PACKET_SCROLLUP1
 *
 *  This command scrolls the data contained within the scroll window, as 
 *  defined by the last scroll command, upward one line.  The area left 
 *  blank is filled by the character/attribute pair that was used in the 
 *  last scroll command.
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
IcaScrollUp1( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollLines = 1;
    ScrollRect( pWd, VIO__SCROLLUP );
}


/*******************************************************************************
 *
 *  IcaScrollUpN 
 *
 *  PACKET_SCROLLUP2
 *  P1 - number of lines
 *  
 *  This command scrolls the data contained within the scroll window, as 
 *  defined by the last scroll command, upward P1 lines.  The area left 
 *  blank is filled by the character/attribute pair that was used in the 
 *  last scroll command.
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
IcaScrollUpN( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollLines = *pInputBuffer;
    ScrollRect( pWd, VIO__SCROLLUP );
}


/*******************************************************************************
 *
 *  IcaScrollDown
 *
 *  PACKET_SCROLLDN
 *  P1 - top row
 *  P2 - left column
 *  P3 - bottom row
 *  P4 - right column
 *  P5 - character
 *  P6 - attribute
 *  P7 - number of lines
 *  
 *  This command scrolls the data contained within the scroll window 
 *  P1,P2,P3,P4 downward P7 lines.  The area left blank is filled by the 
 *  character/attribute pair P5/P6. If a value is specified that is greater 
 *  than the maximum for P1, P2, P3, P4, or P7, the maximum value for that 
 *  parameter is used.  Maximum values depend upon the dimensions of the
 *  screen being used.
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
IcaScrollDown( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    ScrollGetParams( pWd, pInputBuffer );
    ScrollRect( pWd, VIO__SCROLLDN );
}


/*******************************************************************************
 *
 *  IcaScrollDown1 
 *
 *  PACKET_SCROLLDN1
 *
 *  This command scrolls the data contained within the scroll window, as 
 *  defined by the last scroll command, downward one line.  The area left 
 *  blank is filled by the character/attribute pair that was used in the 
 *  last scroll command.
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
IcaScrollDown1( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollLines = 1;
    ScrollRect( pWd, VIO__SCROLLDN );
}


/*******************************************************************************
 *
 *  IcaScrollDownN 
 *
 *  PACKET_SCROLLDN2
 *  P1 - number of lines
 *  
 *  This command scrolls the data contained within the scroll window, as 
 *  defined by the last scroll command, downward P1 lines.  The area left 
 *  blank is filled by the character/attribute pair that was used in the 
 *  last scroll command.
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
IcaScrollDownN( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollLines = *pInputBuffer;
    ScrollRect( pWd, VIO__SCROLLDN );
}


/*******************************************************************************
 *
 *  IcaScrollLeft 
 *
 *  PACKET_SCROLLLF
 *  P1 - top row
 *  P2 - left column
 *  P3 - bottom row
 *  P4 - right column
 *  P5 - character
 *  P6 - attribute
 *  P7 - number of lines
 *  
 *  This command scrolls the data contained within the scroll window 
 *  P1,P2,P3,P4 left P7 lines.  The area left blank is filled by the 
 *  character/attribute pair P5/P6. If a value is specified that is greater 
 *  than the maximum for P1, P2, P3, P4, or P7, the maximum value for that 
 *  parameter is used.  Maximum values depend upon the dimensions of the
 *  screen being used.
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
IcaScrollLeft( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    ScrollGetParams( pWd, pInputBuffer );
    ScrollRect( pWd, VIO__SCROLLLF );
}


/*******************************************************************************
 *
 *  IcaScrollLeft1 
 *
 *  PACKET_SCROLLLF1
 *
 *  This command scrolls the data contained within the scroll window, as 
 *  defined by the last scroll command, left one line.  The area left 
 *  blank is filled by the character/attribute pair that was used in the 
 *  last scroll command.
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
IcaScrollLeft1( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollLines = 1;
    ScrollRect( pWd, VIO__SCROLLLF );
}


/*******************************************************************************
 *
 *  IcaScrollLeftN 
 *
 *  PACKET_SCROLLLF2
 *  P1 - number of lines
 *  
 *  This command scrolls the data contained within the scroll window, as 
 *  defined by the last scroll command, left P1 lines.  The area left 
 *  blank is filled by the character/attribute pair that was used in the 
 *  last scroll command.
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
IcaScrollLeftN( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollLines = *pInputBuffer;
    ScrollRect( pWd, VIO__SCROLLLF );
}


/*******************************************************************************
 *
 *  IcaScrollRight 
 *
 *  PACKET_SCROLLRT
 *  P1 - top row
 *  P2 - left column
 *  P3 - bottom row
 *  P4 - right column
 *  P5 - character
 *  P6 - attribute
 *  P7 - number of lines
 *  
 *  This command scrolls the data contained within the scroll window 
 *  P1,P2,P3,P4 right P7 lines.  The area left blank is filled by the 
 *  character/attribute pair P5/P6. If a value is specified that is greater 
 *  than the maximum for P1, P2, P3, P4, or P7, the maximum value for that 
 *  parameter is used.  Maximum values depend upon the dimensions of the
 *  screen being used.
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
IcaScrollRight( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    ScrollGetParams( pWd, pInputBuffer );
    ScrollRect( pWd, VIO__SCROLLRT );
}


/*******************************************************************************
 *
 *  IcaScrollRight1
 *
 *  PACKET_SCROLLRT1
 *
 *  This command scrolls the data contained within the scroll window, as 
 *  defined by the last scroll command, right one line.  The area left 
 *  blank is filled by the character/attribute pair that was used in the 
 *  last scroll command.
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
IcaScrollRight1( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollLines = 1;
    ScrollRect( pWd, VIO__SCROLLRT );
}


/*******************************************************************************
 *
 *  IcaScrollRightN
 *
 *  PACKET_SCROLLRT2
 *  P1 - number of lines
 *  
 *  This command scrolls the data contained within the scroll window, as 
 *  defined by the last scroll command, right P1 lines.  The area left 
 *  blank is filled by the character/attribute pair that was used in the 
 *  last scroll command.
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
IcaScrollRightN( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollLines = *pInputBuffer;
    ScrollRect( pWd, VIO__SCROLLRT );
}


/*******************************************************************************
 *
 *  ScrollGetParams
 *
 *  get parameters for scroll command
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
ScrollGetParams( PWD pWd, LPBYTE pInputBuffer )
{
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ScrollTopRow   = (USHORT) *pInputBuffer++;
    pIca->ScrollLeftCol  = (USHORT) *pInputBuffer++; 
    pIca->ScrollBotRow   = (USHORT) *pInputBuffer++; 
    pIca->ScrollRightCol = (USHORT) *pInputBuffer++; 
    pIca->ScrollCell[0]  = *pInputBuffer++;
    pIca->ScrollCell[1]  = *pInputBuffer++;
    pIca->ScrollLines    = *pInputBuffer;
}


/*******************************************************************************
 *
 *  ScrollRect
 *
 *  scroll specified rectangle
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    Type (input)
 *       type of scroll  (VIO__SCROLLUP, ...)
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
ScrollRect( PWD pWd, USHORT Type )
{
    PWDICA pIca;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    TRACE(( TC_WD, TT_API1, 
           "SCROLLRECT(%u): %u, %u, %u, %u, lines=%u, cell 0x%02x 0x%02x",
           Type, pIca->ScrollTopRow, pIca->ScrollLeftCol, pIca->ScrollBotRow, 
           pIca->ScrollRightCol, pIca->ScrollLines, pIca->ScrollCell[0], 
           pIca->ScrollCell[1]));

#if 1
    rc = CLIENT_STATUS_SUCCESS;
    switch (Type)
    {
    case VIO__SCROLLLF:
	rc = VioScrollLf(pIca->ScrollTopRow, pIca->ScrollLeftCol, pIca->ScrollBotRow, pIca->ScrollRightCol, pIca->ScrollLines, pIca->ScrollCell, pIca->hVio );
	break;
    case VIO__SCROLLRT:
	rc = VioScrollRt(pIca->ScrollTopRow, pIca->ScrollLeftCol, pIca->ScrollBotRow, pIca->ScrollRightCol, pIca->ScrollLines, pIca->ScrollCell, pIca->hVio );
	break;
    case VIO__SCROLLUP:
	rc = VioScrollUp(pIca->ScrollTopRow, pIca->ScrollLeftCol, pIca->ScrollBotRow, pIca->ScrollRightCol, pIca->ScrollLines, pIca->ScrollCell, pIca->hVio );
	break;
    case VIO__SCROLLDN:
	rc = VioScrollDn(pIca->ScrollTopRow, pIca->ScrollLeftCol, pIca->ScrollBotRow, pIca->ScrollRightCol, pIca->ScrollLines, pIca->ScrollCell, pIca->hVio );
	break;
    }

#else

    rc = (int)(pVioProcedures[ Type ])( pIca->ScrollTopRow, 
                                        pIca->ScrollLeftCol, 
                                        pIca->ScrollBotRow, 
                                        pIca->ScrollRightCol, 
                                        pIca->ScrollLines, 
                                        pIca->ScrollCell, 
                                        pIca->hVio );
#endif
    ASSERT( rc == 0, rc );
}


