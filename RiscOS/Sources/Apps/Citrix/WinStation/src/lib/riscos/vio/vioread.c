
/*************************************************************************
*
*  VIOREAD.C
*
*  Video read routines.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry (3/28/1994)
*
*  vioread.c,v
*  Revision 1.1  1998/01/12 11:37:39  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.6   15 Apr 1997 18:51:40   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   03 May 1995 11:16:02   butchd
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

/*=============================================================================
 ==   Functions Used
 ============================================================================*/

int _CGA_WRITE( char *, char *, unsigned int );
int _CGA_READ( char *, char *, unsigned int );

/*=============================================================================
 ==   Global Variables
 ============================================================================*/

extern int far * _CRT_START;
extern int fMONO;
extern int fCGA;
extern unsigned int usMaxRow;
extern unsigned int usMaxCol;


/*****************************************************************************
*
*  FUNCTION: Read Cell String
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioReadCellStr (PCH pchCellStr, PUSHORT pcb, USHORT usRow,
                           USHORT usColumn, HVIO hvio)
{
   PCH pLVB;

   /*
    * Check for out of bounds values.
    */
   if ( usRow >= usMaxRow )
      usRow = usMaxRow-1;
   if ( usColumn >= usMaxCol )
      usColumn = usMaxCol-1;

#if 0
   /*
    * Get the phys address.
    */
   pLVB = fMONO ? (LPBYTE) 0xB0000000 : (LPBYTE) 0xB8000000;

   /*
    * Add in regen offset.
    */
   pLVB += *_CRT_START;

   /*
    * Add in row/colomn offset
    */
   pLVB += ( (usRow * usMaxCol) + usColumn) << 1;

   /*
    * Move data to user buffer from screen.
    */
   if ( fCGA )
      (void) _CGA_READ( pchCellStr, pLVB, *pcb );
   else
      memcpy( pchCellStr, pLVB, *pcb );
#endif
   
   return( CLIENT_STATUS_SUCCESS );
}

#if 0
/*****************************************************************************
*
*  FUNCTION: Read Character String
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioReadCharStr (PCH pchCharStr, PUSHORT pcb, USHORT usRow,
                           USHORT usColumn, HVIO hvio)
{
   register USHORT i,j;
   PCH pLVB;

   /*
    * Check for out of bounds values.
    */
   if ( usRow >= usMaxRow )
      usRow = usMaxRow-1;
   if ( usColumn >= usMaxCol )
      usColumn = usMaxCol-1;

   for (i = 
   
#if 0
   /*
    * Get the phys address.
    */
   pLVB = fMONO ? (LPBYTE) 0xB0000000 : (LPBYTE) 0xB8000000;

   /*
    * Add in regen offset.
    */
   pLVB += *_CRT_START;

   /*
    * Add in row/colomn offset
    */
   pLVB += ( (usRow * usMaxCol) + usColumn) << 1;
   
   /*
    * Move data to user buffer from screen.
    */
   for ( j=0, i=0; i < *pcb; j+=2, i++ ) {
      if ( fCGA )
         (void) _CGA_READ( pchCharStr + i, pLVB + j, 1 );
      else
         pchCharStr[ i ] = pLVB[ j ];
   }
#endif
   
   return( CLIENT_STATUS_SUCCESS );
}
#endif

