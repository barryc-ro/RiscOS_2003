
/*************************************************************************
*
*  VIOWRT.C
*
*  Video write routines.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry (3/28/1994)
*
*  $Log$
*  
*     Rev 1.7   15 Apr 1997 18:52:12   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   03 May 1995 11:16:28   butchd
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

//  Get private includes
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

extern PVIOHOOK pVioRootHook;

/*****************************************************************************
*
*  FUNCTION: Write Cell String
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioWrtCellStr (PCH pchCellStr, USHORT cb, USHORT usRow,
                               USHORT usColumn, HVIO hvio)
{
   USHORT i;
   PCH pLVB;
   PVIOHOOK pVioHook;

   /*
    * Check for out of bounds values.
    */
   if ( usRow >= usMaxRow )
      usRow = usMaxRow-1;
   if ( usColumn >= usMaxCol )
      usColumn = usMaxCol-1;

   WriteCells(pchCellStr, cb, usRow, usColumn);
   
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
    * Move data from user buffer to screen buffer.
    */
   if ( fCGA )
      (void) _CGA_WRITE( pLVB, pchCellStr, cb );
   else
      memcpy( pLVB, pchCellStr, cb );
#endif
   
   // call hook routines
   for ( pVioHook=pVioRootHook; pVioHook != NULL; pVioHook=pVioHook->pNext ) {
      for ( i=0; i<cb; i++ ) {
         (pVioHook->pProcedure)( usRow, usColumn+i, *(pchCellStr+(i*2)), 1 );
      }
   }

   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Write Character String
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioWrtCharStr (PCH pchStr, USHORT cb, USHORT usRow,
                               USHORT usColumn, HVIO hvio)
{
   register USHORT i,j;
   PCH pLVB;
   PVIOHOOK pVioHook;

   /*
    * Check for out of bounds values.
    */
   if ( usRow >= usMaxRow )
      usRow = usMaxRow-1;
   if ( usColumn >= usMaxCol )
      usColumn = usMaxCol-1;

   WriteString(pchStr, cb, usRow, usColumn, 0);
   
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
    * Move data from user buffer to screen.
    */
   for ( j=0, i=0; i<cb; j+=2, i++ ) {
      if ( fCGA )
         (void) _CGA_WRITE( pLVB+j, pchStr+i, 1 );
      else
         pLVB[j] = pchStr[i];
   }
#endif
   
   // call hook routines
   for ( pVioHook=pVioRootHook; pVioHook != NULL; pVioHook=pVioHook->pNext ) {
      (pVioHook->pProcedure)( usRow, usColumn, pchStr, cb );
   }

   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Write N Attributes
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioWrtNAttr (LPBYTE pAttr, USHORT cb, USHORT usRow,
                             USHORT usColumn, HVIO hvio)
{
   register USHORT i;
   PCH pLVB;

   /*
    * Check for out of bounds values.
    */
   if ( usRow >= usMaxRow )
      usRow = usMaxRow-1;
   if ( usColumn >= usMaxCol )
      usColumn = usMaxCol-1;

   WriteCounted(-1, *pAttr, cb, usRow, usColumn);
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
    *  Write attribute to screen
    */
   for ( i=1, cb<<=1; i<cb; i+=2 ) {
      if ( fCGA )
         (void) _CGA_WRITE( pLVB+i, pAttr, 1 );
      else
         pLVB[i] = (char) *pAttr;
   }
#endif
   
   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Write N Cells
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioWrtNCell (LPBYTE pCell, USHORT cb, USHORT usRow,
                             USHORT usColumn, HVIO hvio)
{
   register USHORT i;
   PCH pLVB;
   PVIOHOOK pVioHook;

   /*
    * Check for out of bounds values.
    */
   if ( usRow >= usMaxRow )
      usRow = usMaxRow-1;
   if ( usColumn >= usMaxCol )
      usColumn = usMaxCol-1;

   WriteCounted(pCell[0], pCell[1], cb, usRow, usColumn);

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
    * Write Char/Attr pair to screen
    */
   for ( i=0, cb<<=1; i<cb; i+=2 ) {
      if ( fCGA )
         (void) _CGA_WRITE( pLVB+i, pCell, 2 );
      else
         memcpy( pLVB+i, pCell, 2 );
   }
#endif
   
   // call hook routines
   for ( pVioHook=pVioRootHook; pVioHook != NULL; pVioHook=pVioHook->pNext ) {
      for ( i=0; i<cb; i++ ) {
         (pVioHook->pProcedure)( usRow, usColumn+i, pCell, 1 );
      }
   }

   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Write N Characters
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioWrtNChar (PCH pchChar, USHORT cb, USHORT usRow,
                             USHORT usColumn, HVIO hvio)
{
   register USHORT i;
   PCH pLVB;
   PVIOHOOK pVioHook;

   /*
    * Check for out of bounds values.
    */
   if ( usRow >= usMaxRow )
      usRow = usMaxRow-1;
   if ( usColumn >= usMaxCol )
      usColumn = usMaxCol-1;

   WriteString(pchChar, cb, usRow, usColumn, 0);

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
    * Write character to screen
    */
   for ( i=0, cb<<=1; i<cb; i+=2 ) {
      if ( fCGA )
         (void) _CGA_WRITE( pLVB+i, pchChar, 1 );
      else
         pLVB[i] = *pchChar;
   }
#endif
   
   // call hook routines
   for ( pVioHook=pVioRootHook; pVioHook != NULL; pVioHook=pVioHook->pNext ) {
      for ( i=0; i<cb; i++ ) {
         (pVioHook->pProcedure)( usRow, usColumn+i, pchChar, 1 );
      }
   }

   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Write Character String with Attributes
*
*  ENTRY:
*
****************************************************************************/
int WFCAPI VioWrtCharStrAtt (PCH pch, USHORT cb, USHORT usRow,
                                  USHORT usColumn, LPBYTE pAttr, HVIO hvio)
{
   register USHORT i,j;
   BYTE Cell[2];
   PCH pLVB;
   PVIOHOOK pVioHook;

   /*
    * Check for out of bounds values.
    */
   if ( usRow >= usMaxRow )
      usRow = usMaxRow-1;
   if ( usColumn >= usMaxCol )
      usColumn = usMaxCol-1;

   WriteString(pch, cb, usRow, usColumn, *pAttr);
   
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
    * Move data from user buffer to screen.
    */
   for ( j=0, i=0; i<cb; j+=2, i++ ) {
      if ( fCGA ) {
         Cell[0] = pch[i];
         Cell[1] = *pAttr;
         (void) _CGA_WRITE( pLVB+j, Cell, 2 );
      } else {
         pLVB[j]   = pch[i];
         pLVB[j+1] = (char) *pAttr;
      }
   }
#endif
   
   // call hook routines
   for ( pVioHook=pVioRootHook; pVioHook != NULL; pVioHook=pVioHook->pNext ) {
      (pVioHook->pProcedure)( usRow, usColumn, pch, cb );
   }

   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Write Teletype Mode
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioWrtTTY( PCH pch, USHORT cb, HVIO hvio )
{
   USHORT row;
   USHORT col;
//   union REGS regs;
//   struct SREGS sregs;
   PVIOHOOK pVioHook;

   if ( cb == 0 )
       return( CLIENT_STATUS_SUCCESS );

   // get current cursor position
   (void) VioGetCurPos( &row, &col, 0 );

   /*
    *  Output characters using DOS
    */
   WriteString(pch, cb, -1, -1, 0);
       
#if 0
   regs.h.ah = 0x40;
   regs.x.bx = 2;
   regs.x.cx = cb;
   regs.x.dx = FP_OFF( pch );
   sregs.ds  = FP_SEG( pch );
   intdosx( &regs, &regs, &sregs );
#endif
    
   // call hook routines
   for ( pVioHook=pVioRootHook; pVioHook != NULL; pVioHook=pVioHook->pNext ) {
      (pVioHook->pProcedure)( row, col, pch, cb );
   }

   return( CLIENT_STATUS_SUCCESS );
}
