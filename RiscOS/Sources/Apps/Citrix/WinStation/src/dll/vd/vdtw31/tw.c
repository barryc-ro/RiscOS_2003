//#define DEEPDEBUG 1

/*******************************************************************************
*
*   TW.C
*
*   Thin Wire Windows - initialization code
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*  
*     Rev 1.14   28 Apr 1997 14:57:28   kurtp
*  I fixed a bug in this file, update, duh!
*  
*     Rev 1.13   15 Apr 1997 18:15:54   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   21 Mar 1997 16:09:24   bradp
*  update
*  
*     Rev 1.12   13 Jan 1997 16:52:04   kurtp
*  Persistent Cache
*  
*     Rev 1.11   08 May 1996 14:51:32   jeffm
*  update
*  
*     Rev 1.10   03 Jan 1996 13:32:46   kurtp
*  update
*
*******************************************************************************/

#include "windows.h"

/*
 *  Get the standard C includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>

#include "../../../inc/client.h"
#include "twtype.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/logapi.h"

#include "../../../inc/clib.h"

#include "twwin.h"
#include "twdata.h"


/*=============================================================================
==   External Functions and Data
=============================================================================*/
//(see twwin.h)

extern int TWInitializeObjectCaches( ULONG, ULONG );
extern int TWFreeObjectCaches(void);
extern void far pascal ctx_detect_supervga(void);

#ifdef DOS

extern int far InitThinwire( USHORT, USHORT );
extern void far MouseHandler(PMOUSERAWDATA pData);
extern void far TimerHandler(void);
extern int TWCallHookMouse( PVD pVd, PVOID pMouseHook );
extern int TWCallUnHookMouse( PVD pVd, PVOID pMouseHook );
extern int TWCallResetMouse( PVD pVd );
extern int TWCallHookTimer( PVD pVd, PVOID pTimerHook );
extern int TWCallUnHookTimer( PVD pVd, PVOID pTimerHook );

#else

extern BOOL TWDIMCacheInit(PVD, BOOL);
extern void far    InitThinwire( COLOR_CAPS, USHORT, USHORT );
extern VOID       TWUseSystemPalette( HWND, HDC );
extern COLOR_CAPS vColor;
extern HDC        vhdc;
extern HWND       vhWnd;
extern PVD        vpVd;

ULONG  vTimeLastDim;
BOOL   vfDimContinue = FALSE;
BOOL   vPersistentCache;
ULONG  vPersistentSigLevel;

#endif

/*=============================================================================
==   Functions Defined
=============================================================================*/

USHORT TWAllocCache( PVD pVd, ULONG, ULONG );
USHORT TWDeallocCache( PVD pVd );
USHORT TWDetermineSVGA( PVD pVd );
USHORT TWWindowsStart( PVD pVd, PTHINWIRECAPS pThinWireMode );
USHORT TWWindowsStop( PVD pVd );


#ifdef DEBUGREMOVE
USHORT MakeCODEVIEWWork(USHORT Input);
extern ULONG gulTimerCount;
extern ULONG gulMouseCount;
#endif

/*=============================================================================
 ==   Data
 ============================================================================*/

// this is set after the INIT_WINDOWS command has been received
// it is cleared when we loose focus.  all data is thrown away until
// another INIT_WINDOWS is received
BOOL  fWindowsSynced = FALSE;

// 0 or the current command
BYTE bCmdInProg=0;



/**************************************************************************
*
*  TWAllocCache
*
*     Allocate and initialize cache.  Done at VD Open time.
*
*  ENTRY:
*     pVd (input)
*        pointer to virtual driver data structure
*     ulXms (input)
*        xms memory available cache
*     ulLowMem (input)
*        low memory available for cache
*
*  EXIT:
*     CLIENT_STATUS_SUCCESS
*
*  Note: might want to pass some size parms here
*
***************************************************************************/
USHORT TWAllocCache( PVD pVd, ULONG ulXms, ULONG ulLowMem  )
{
   int rc;

   TRACE(( TC_VD, TT_API2,
     "TWAllocCache: xms %lu, low %lu", ulXms, ulLowMem ));
   rc = TWInitializeObjectCaches(ulLowMem, ulXms);
   ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

   return(rc);
}

/**************************************************************************
*
*  TWDeallocCache
*
*     DeAllocate cache.  Done at VD Close time.
*
*  ENTRY:
*     none
*
*  EXIT:
*     CLIENT_STATUS_SUCCESS
*
*  Note: might want to pass some size parms here
*
***************************************************************************/
USHORT TWDeallocCache( PVD pVd )
{
   int rc;

   TRACE(( TC_VD, TT_API2, "TWDeallocCache: " ));
   rc = TWFreeObjectCaches();
   ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
   return(rc);
}

/**************************************************************************
*
*  TWDetermineSVGA
*
*     Figure out type of SVGA.  Done at VD Open time.
*
*  ENTRY:
*     none
*
*  EXIT:
*     CLIENT_STATUS_SUCCESS
*
*  Note: might want to pass some VESA register overrides here.
*
***************************************************************************/
USHORT TWDetermineSVGA( PVD pVd )
{
   TRACE(( TC_VD, TT_API2, "TWDetermineSVGA: " ));
// ctx_detect_supervga();
   return(CLIENT_STATUS_SUCCESS);
}

/**************************************************************************
*
*  TWWindowsStart
*
*     We have focus.  Start up the thinwire things.
*
*  ENTRY:
*
*  EXIT:
*     NO_ERROR - successful
*
***************************************************************************/
USHORT TWWindowsStart( PVD pVd, PTHINWIRECAPS pThinWireMode )
{
   USHORT rc = CLIENT_STATUS_SUCCESS;
   USHORT uWidth, uHeight;

   ExtractMode( pThinWireMode, uWidth, uHeight );

   TRACE(( TC_VD, TT_API2, "TWWindowsStart: w=%u, h=%u", uWidth, uHeight ));
   TRACE(( TC_TW, TT_TW_CONNECT, "TWWindowsStart: w=%u, h=%u", uWidth, uHeight ));

   // init thinwire code, and set graphics modes


   //_asm int 3
   if ( uWidth && uHeight ) {
#ifdef DOS
      (void) MouseSetScreenDimensions( uWidth, uHeight );
      InitThinwire( uWidth, uHeight );
#else
      InitThinwire( vColor, uWidth, uHeight );
#endif
   }

#ifdef DOS
   // reset the mouse to make sure it is turned off
   rc = TWCallResetMouse(pVd);

   // hook the mouse and timer if there was a mouse
   if(rc==CLIENT_STATUS_SUCCESS) {
      rc = TWCallHookMouse( pVd, (PVOID)MouseHandler );
      ASSERT((rc == 0), rc);
      rc = TWCallHookTimer( pVd, (PVOID)TimerHandler );
      ASSERT((rc == 0), rc);
   }
#endif

   // reset the command state
   bCmdInProg = 0;

   fWindowsSynced = TRUE;

   return( CLIENT_STATUS_SUCCESS );
}

/****************************************************************************\
 *  TWCmdInit (TWCMD_INIT service routine)
 *
 *  This routine is called to service the DrvConnect thinwire display
 *  driver function.
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *     hdc (input)
 *        device context
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/
void TWCmdInit( HWND hWnd, HDC hdc )
{
    USHORT         rc = 0;
    USHORT         hRes, vRes;
#ifndef DOS
    USHORT         Mask;
    COLOR_CAPS     Color;
#endif

    TRACE(( TC_TW, TT_TW_PACKET+TT_TW_CONNECT, "TWCmdInit: Entered" ));

    GetNextTWCmdBytes( &vThinWireMode, sizeof(THINWIRECAPS) );

    TRACE(( TC_TW, TT_TW_PACKET+TT_TW_PALETTE,
            "TWCmdInit: Received - Res(%02d,%02d) fColor(%04X) flGraphCaps(%0lX)",
            vThinWireMode.ResCaps.HRes, vThinWireMode.ResCaps.VRes,
            vThinWireMode.fColorCaps,
            vThinWireMode.flGraphicsCaps ));

#ifndef DOS
    /*
     *  Set global color mode
     */
    for ( Color = vColor = Color_Cap_16, Mask=1; 
          Color < Color_Cap_Max; 
          Color++, Mask <<= 1 ) {

        if ( (Mask & vThinWireMode.fColorCaps) ) {
            vColor = Color;
            break;
        }
    }

    /*
     *  If persistent cache enabled, then go get signature level
     */
    if ( (vThinWireMode.flGraphicsCaps & GCAPS_BMPS_PRECACHED) ) {
        vPersistentCache = TRUE;
        GetNextTWCmdBytes( &vPersistentSigLevel, sizeof(ULONG) );
        vfDimContinue = TWDIMCacheInit(vpVd, FALSE);
        vTimeLastDim = Getmsec();
    }
    else {
        vPersistentCache = FALSE;
    }
#endif

    /*
     *  Get resolution
     */
    ExtractMode( &vThinWireMode, hRes, vRes );

    TRACE(( TC_TW, TT_TW_PACKET+TT_TW_CONNECT,
            "TWCmdInit: Mode set to hRes(%d) vRes(%d)",
            hRes, vRes ));
#ifdef DOS
    (void) MouseSetScreenDimensions( hRes, vRes );
    InitThinwire( hRes, vRes );
#else
    InitThinwire( vColor, hRes, vRes );
#endif

    TRACE(( TC_TW, TT_TW_PACKET+TT_TW_CONNECT, "TWCmdInit: Exiting" ));
    TWCmdReturn( !rc ); // return to NewNTCommand or ResumeNTCommand
}

/**************************************************************************
*
*  TWWindowsStop
*
*     We lost focus.  Stop doing things like mouse cursor draw.
*
*  ENTRY:
*
*  EXIT:
*     CLIENT_STATUS_SUCCESS - successful
*
***************************************************************************/
USHORT TWWindowsStop( PVD pVd )
{

   TRACE(( TC_VD, TT_API2, "TWWindowsStop: " ));
   TRACE(( TC_TW, TT_TW_CONNECT, "TWWindowsStop: " ));
//   TRACE(( TC_VD, TT_API2, "TWWindowsStop: mou=%lu tim=%lu", gulMouseCount, gulTimerCount ));
   // init thinwire code, and set graphics modes
   // errors shouldnt matter here
#ifdef DOS
   TWCallUnHookTimer( pVd, (PVOID)TimerHandler );
   TWCallUnHookMouse( pVd, (PVOID)MouseHandler );
#endif

   /*
    *  Not non-gui and change mouse pointer for text or other ...
    */
   fWindowsSynced = FALSE;
#if !defined( DOS ) && !defined(RISCOS)
   TWUseSystemPalette( vhWnd, vhdc );
   SetCursor( LoadCursor( NULL, IDC_ARROW ) );
#endif

   return( CLIENT_STATUS_SUCCESS );
}

#ifdef REMOVE
#ifdef DEBUG
/**************************************************************************
*
*  Make CODEVIEW work!
*
*
*     Best i can tell after much pain there is something wrong
*     with the linker, CVPACK, or the libraries we are linking with.
*     This causes llinkd.exe to crap out in the runtime initialization
*     code prior to getting to _main.  This didnt always happen after
*     each build; it seemed like it depended on how many lines were
*     in a source module (for example, it wouldn't work, then i would
*     add a few lines and it would work).
*
*     In one instance i found that it was crapping out because the high word of
*     fpMath in \c700\source\startup\dos\crt0dat.asm was garbage and
*     the low word was 0.  This caused the startup code to jump into the weeds.
*
*              (excerpt from crt0dat.asm)
*                       dw      0               ; force segment to be at least 0's
*                 labelD        <PUBLIC,_fpinit>        ; public for signal
*                 fpmath        dd      1 dup (?)       ; linking trick for fp
*                 fpdata        dd      1 dup (?)
*                 fpsignal dd   1 dup (?)       ; fp signal message
*
*
*     This function forces us to need a floating point function which
*     i think forces  us to fill in fpmath.  This seems to work, or at
*     least so far - knock knock.
*
***************************************************************************/

USHORT MakeCODEVIEWWork(USHORT Input)
{
   float flfl;

   flfl = Input;
   flfl = flfl/111;
   return((USHORT)flfl);

}
#endif
#endif //REMOVE

