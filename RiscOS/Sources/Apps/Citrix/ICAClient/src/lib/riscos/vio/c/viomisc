
/*************************************************************************
*
*  VIOMISC.C
*
*  Miscellaneous video routines.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry (3/29/1994)
*
*  viomisc.c,v
*  Revision 1.1  1998/01/12 11:37:38  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  Revision 1.1  1998/01/08 10:41:19  smiddle
*  Initial revision
*
*  
*     Rev 1.9   15 Apr 1997 18:51:36   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.8   19 Jul 1995 12:16:38   kurtp
*  update
*  
*     Rev 1.7   19 Jul 1995 12:08:40   kurtp
*  update
*  
*     Rev 1.6   03 May 1995 11:15:54   butchd
*  clib.h now standard
*
*************************************************************************/

#include "windows.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Get CLIB includes */
#include "../../../inc/client.h"
#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/logapi.h"

//  Get private includes
#include "vio.h"

#include "swis.h"

/*=============================================================================
 ==   Functions Used
 ============================================================================*/

/*=============================================================================
 ==   Local Variables
 ============================================================================*/

PVIOHOOK pVioRootHook = NULL;

/*=============================================================================
 ==   Global Variables
 ============================================================================*/

extern unsigned int usMaxRow;
extern unsigned int usMaxCol;


/*****************************************************************************
*
*  FUNCTION: Clear Screen
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioClearScreen ( VOID )
{
   BYTE bCell[2];

   // space white on black
   bCell[0] = 0x20;
   bCell[1] = 0x07;

   // clear screen
   (void) VioScrollUp( 0, 0, 0xffff, 0xffff, 0xffff, bCell, 0 );
   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Save Screen
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioSaveScreen (PVIOSAVESCREEN pSaveScreen)
{
   USHORT rc;

   // zap structure
   memset( pSaveScreen, 0, sizeof(VIOSAVESCREEN) );

   // save video monde
   pSaveScreen->ModeInfo.cb = 12;
   rc = VioGetMode( &pSaveScreen->ModeInfo, 0 );

   // save video state
   pSaveScreen->Overscan.cb = sizeof( VIOOVERSCAN );
   pSaveScreen->Overscan.type = 1;
   rc = VioGetState( &pSaveScreen->Overscan, 0 );

   // save cursor type
   rc = VioGetCurType( &pSaveScreen->CursorInfo, 0 );

   // save cursor position
   rc = VioGetCurPos( &pSaveScreen->Row, &pSaveScreen->Col, 0 );

   // malloc space for video text
   pSaveScreen->cbVideoBuf = pSaveScreen->ModeInfo.row * pSaveScreen->ModeInfo.col * 2;
   pSaveScreen->pchVideoBuf = (PCHAR) malloc( pSaveScreen->cbVideoBuf );

   // save video text
   if ( pSaveScreen->pchVideoBuf )
      rc = VioReadCellStr( pSaveScreen->pchVideoBuf, &pSaveScreen->cbVideoBuf, 0, 0, 0 );
   else
      rc = CLIENT_ERROR_NO_MEMORY;

   return( rc );
}

/*****************************************************************************
*
*  FUNCTION: Restore Screen
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioRestoreScreen (PVIOSAVESCREEN pSaveScreen)
{
   VIOMODEINFO ModeInfo;
   USHORT rc;

   // get current video mode
   ModeInfo.cb = pSaveScreen->ModeInfo.cb;
   rc = VioGetMode( &ModeInfo, 0 );

   // junk
   if ( memcmp( &ModeInfo, &pSaveScreen->ModeInfo, pSaveScreen->ModeInfo.cb ) ) {
      rc = VioSetMode( &pSaveScreen->ModeInfo, 0 );
   }
   usMaxRow = pSaveScreen->ModeInfo.row;
   usMaxCol = pSaveScreen->ModeInfo.col;

   // restore video state
   rc = VioSetState( &pSaveScreen->Overscan, 0 );

   // restore cursor type
   rc = VioSetCurType( &pSaveScreen->CursorInfo, 0 );

   // restore cursor pos
   rc = VioSetCurPos( pSaveScreen->Row, pSaveScreen->Col, 0 );

   // restore video text
   rc = VioWrtCellStr( pSaveScreen->pchVideoBuf, pSaveScreen->cbVideoBuf, 0, 0, 0 );

   // destroy screen
   rc = VioDestroyScreen( pSaveScreen );

   return( rc );
}

/*****************************************************************************
*
*  FUNCTION: Destroy Screen
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioDestroyScreen (PVIOSAVESCREEN pSaveScreen)
{
   USHORT rc = CLIENT_STATUS_SUCCESS;

   // free memory
   if ( pSaveScreen->pchVideoBuf )
      free( pSaveScreen->pchVideoBuf );
   else
      rc = CLIENT_ERROR_NO_MEMORY;

   // zap structure
   memset( pSaveScreen, 0, sizeof(VIOSAVESCREEN) );

   return( rc );
}

/*****************************************************************************
*
*  FUNCTION: Cause Speaker Noise
*
*  ENTRY:
*
****************************************************************************/

#define TIMER_FREQ   1193180L
#define TIMER_COUNT  0x42
#define TIMER_MODE   0x43
#define TIMER_OSC    0xb6
#define OUT_8255     0x61
#define SPKRON       3

int WFCAPI
VioBeep (USHORT usFrequency, USHORT usDuration)
{
   unsigned status, ratio, part_ratio;

   TRACE(( TC_LIB, TT_API1, "VioBeep: freq %u, dur %u", usFrequency, usDuration ));

   /* a 0, 0 means speaker off */
   if ( usFrequency == 0 && usDuration == 0 )
   {
       sound_off();

       return( CLIENT_STATUS_SUCCESS );
   }

   sound_beep(usFrequency, usDuration);
   
   return( CLIENT_STATUS_SUCCESS );
}


/*****************************************************************************
*
*  FUNCTION: Add Callout Hook
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioAddHook (PVOID pProcedure)
{
   PVIOHOOK pVioHook;

   // check to see if this proc is already hooked
   for ( pVioHook=pVioRootHook; pVioHook != NULL; pVioHook=pVioHook->pNext ) {

      // found?
      if ( (PVOID)pVioHook->pProcedure == pProcedure ) {
         goto done;
      }
   }

   // allocate a hook structure
   if ( (pVioHook = (PVIOHOOK) malloc( sizeof(VIOHOOK) )) != NULL ) {

       // initialize and link in at head
       pVioHook->pProcedure = (PLIBPROCEDURE)pProcedure;
       pVioHook->pNext = pVioRootHook;
       pVioRootHook = pVioHook;
   }
   else {

       return( CLIENT_ERROR_NO_MEMORY );
   }

   // ok
done:
   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Remove Callout Hook
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioRemoveHook (PVOID pProcedure)
{
   PVIOHOOK pVioHook;
   PVIOHOOK pPrevVioHook;

   // find old hook
   for ( pVioHook=pVioRootHook, pPrevVioHook = NULL;
         pVioHook != NULL;
         pVioHook=pVioHook->pNext ) {

      // found hooked proc?
      if ( (PVOID)pVioHook->pProcedure == pProcedure ) {

         // remove from list
         if ( pPrevVioHook == NULL ) {    // first
            pVioRootHook = pVioHook->pNext;
         }
         else {                           // other
            pPrevVioHook->pNext = pVioHook->pNext;
         }

         // free memory
         free( pVioHook );

         // out of here
         goto done;
      }

      // maintain prev ptr
      pPrevVioHook = pVioHook;
   }

   // not found
   return( CLIENT_STATUS_NO_DATA );

   // ok
done:
   return( CLIENT_STATUS_SUCCESS );
}
