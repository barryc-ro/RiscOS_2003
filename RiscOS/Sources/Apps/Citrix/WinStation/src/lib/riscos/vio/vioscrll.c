
/*************************************************************************
*
*  VIOSCRLL.C
*
*  Video scrolling routines.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry (3/28/1994)
*
*  vioscrll.c,v
*  Revision 1.1  1998/01/12 11:37:40  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.7   15 Apr 1997 18:51:42   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   28 Jul 1995 14:48:16   bradp
*  update
*  
*     Rev 1.5   03 May 1995 11:16:10   butchd
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

#include "vio.h"

/*=============================================================================
 ==   Functions Used
 ============================================================================*/

// int _CGA_WRITE( char *, char *, unsigned int );
// int _CGA_READ( char *, char *, unsigned int );

/*=============================================================================
 ==   Global Variables
 ============================================================================*/

// extern int far * _CRT_START;
// extern int fMONO;
// extern int fCGA;
extern unsigned int usMaxRow;
extern unsigned int usMaxCol;

/*****************************************************************************
*
****************************************************************************/


/*****************************************************************************
*
*  FUNCTION: Scroll Down
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioScrollDn (USHORT usTopRow, USHORT usLeftCol,
                             USHORT usBotRow, USHORT usRightCol,
                             USHORT cbLines, LPBYTE pCell, HVIO hvio)
{
   PCH    pLVB;
   register USHORT i, j, k;
   USHORT lo, lw, cb, cb2;

   /*
    * Check for null scroll
    */
   if ( cbLines == 0 )
      return( CLIENT_STATUS_SUCCESS );

   /*
    * Check for out of bounds values.
    */
   if ( usTopRow >= usMaxRow)
      usTopRow = usMaxRow - 1;

   if ( usBotRow >= usMaxRow)
      usBotRow = usMaxRow - 1;

   if ( usLeftCol >= usMaxCol )
      usLeftCol = usMaxCol-1;

   if ( usRightCol >= usMaxCol )
      usRightCol = usMaxCol-1;

   if ( cbLines > (usBotRow - usTopRow + 1) )
      cbLines = usBotRow - usTopRow + 1;

   textwindow(usTopRow, usLeftCol, usBotRow, usRightCol);
   scrollwindow(cbLines, SCROLL_DOWN);
   resettextwindow();
   
   /*
    * Fill the remainder with the requested character.
    * -- only if char/attr 
    */
   if ( pCell[0] || pCell[1] ) {
       for ( i=usTopRow; i<usTopRow+cbLines; i++ ) {
	   WriteCounted( pCell[0], pCell[1], usRightCol - usLeftCol + 1, i, usLeftCol);
      }
   }

  
   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Scroll Up
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioScrollUp (USHORT usTopRow, USHORT usLeftCol,
                             USHORT usBotRow, USHORT usRightCol,
                             USHORT cbLines, LPBYTE pCell, HVIO hvio)
{
   PCH    pLVB;
   register USHORT i, j, k;
   USHORT lo, lw, cb, cb2;

   /*
    * Check for null scroll
    */
   if ( cbLines == 0 )
      return( CLIENT_STATUS_SUCCESS );

   /*
    * Check for out of bounds values.
    */
   if ( usTopRow >= usMaxRow)
      usTopRow = usMaxRow - 1;

   if ( usBotRow >= usMaxRow)
      usBotRow = usMaxRow - 1;

   if ( usLeftCol >= usMaxCol )
      usLeftCol = usMaxCol-1;

   if ( usRightCol >= usMaxCol )
      usRightCol = usMaxCol-1;

   if ( cbLines > (usBotRow - usTopRow + 1) )
      cbLines = usBotRow - usTopRow + 1;

   textwindow(usTopRow, usLeftCol, usBotRow, usRightCol);
   scrollwindow(cbLines, SCROLL_UP);
   resettextwindow();
   /*
    * Fill the remainder with the requested character.
    */
   if ( pCell[0] || pCell[1] ) {
      for ( i=(usBotRow-cbLines+1); i<=usBotRow; i++ ) {
	  WriteCounted( pCell[0], pCell[1], usRightCol - usLeftCol + 1, i, usLeftCol);
      }
   }

   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Scroll Left
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioScrollLf (USHORT usTopRow, USHORT usLeftCol,
                        USHORT usBotRow, USHORT usRightCol,
                        USHORT cbCol, LPBYTE pCell, HVIO hvio)
{
   PCH    pLVB;
   register USHORT i, j, k;
   USHORT l, lo, lw, cb, cb3;

   /*
    * Check for null scroll
    */
   if ( cbCol == 0 )
      return( CLIENT_STATUS_SUCCESS );

   /*
    * Check for out of bounds values.
    */
   if ( usTopRow >= usMaxRow)
      usTopRow = usMaxRow - 1;

   if ( usBotRow >= usMaxRow)
      usBotRow = usMaxRow - 1;

   if ( usLeftCol >= usMaxCol )
      usLeftCol = usMaxCol-1;

   if ( usRightCol >= usMaxCol )
      usRightCol = usMaxCol-1;

   if ( cbCol > (usRightCol - usLeftCol + 1) )
      cbCol = usRightCol - usLeftCol + 1;

   textwindow(usTopRow, usLeftCol, usBotRow, usRightCol);
   scrollwindow(cbCol, SCROLL_LEFT);
   resettextwindow();

   /*
    * Fill the remainder with the requested character.
    */
   if ( pCell[0] || pCell[1] ) {
//    lo = (usRightCol - cbCol + 1) << 1;
      for ( i=usTopRow; i<=usBotRow; i++ ) {
	  WriteCounted(pCell[0], pCell[1], usRightCol - cbCol + 1, i, cbCol);
      }
   }

   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Scroll Right
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioScrollRt (USHORT usTopRow, USHORT usLeftCol,
                        USHORT usBotRow, USHORT usRightCol,
                        USHORT cbCol, LPBYTE pCell, HVIO hvio)
{
   PCH    pLVB;
   register USHORT i, j, k;
   USHORT l, lo, lw, cb, cb3;
   BYTE Buffer[256];

   /*
    * Check for null scroll
    */
   if ( cbCol == 0 )
      return( CLIENT_STATUS_SUCCESS );

   /*
    * Check for out of bounds values.
    */
   if ( usTopRow >= usMaxRow)
      usTopRow = usMaxRow - 1;

   if ( usBotRow >= usMaxRow)
      usBotRow = usMaxRow - 1;

   if ( usLeftCol >= usMaxCol )
      usLeftCol = usMaxCol-1;

   if ( usRightCol >= usMaxCol )
      usRightCol = usMaxCol-1;

   if ( cbCol > (usRightCol - usLeftCol + 1) )
      cbCol = usRightCol - usLeftCol + 1;

   textwindow(usTopRow, usLeftCol, usBotRow, usRightCol);
   scrollwindow(cbCol, SCROLL_RIGHT);
   resettextwindow();
   
   /*
    * Fill the remainder with the requested character.
    */
   if ( pCell[0] || pCell[1] ) {
      for ( i=usTopRow; i<=usBotRow; i++ ) {
	  WriteCounted(pCell[0], pCell[1], usLeftCol, i, cbCol);
      }
   }

   return( CLIENT_STATUS_SUCCESS );
}
