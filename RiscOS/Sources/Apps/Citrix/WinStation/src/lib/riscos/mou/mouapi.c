/*************************************************************************
*
* mouapi.c
*
* MOUSE emulation routines for DOS
*
* Copyright 1994, Citrix Systems Inc.
*
* $Author$  Andy (3/15/94)
*
* $Log$
*  
*     Rev 1.24   02 Dec 1997 17:33:56   terryt
*  vesa driver
*  
*     Rev 1.23   Jun 27 1997 20:24:52   scottc
*  fixed scrolling to far problem (merge from 1.6)
*  
*     Rev 1.22   27 Jun 1997 15:54:14   terryt
*  double click support
*  
*     Rev 1.21   15 Apr 1997 18:50:36   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.20   26 Jun 1996 15:29:18   brucef
*  Added API to load and manage user preferences for
*  mouse behavior.
*  
*  
*     Rev 1.19   01 May 1996 16:01:34   kurtp
*  3 Button Mouse Support
*  
*     Rev 1.18   20 Jul 1995 15:25:02   bradp
*  update
*  
*     Rev 1.17   10 Jul 1995 10:38:00   bradp
*  update
*  
*     Rev 1.16   10 Jul 1995 09:22:32   bradp
*  update
*  
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
#include "../../../inc/clib.h"
#include "../../../inc/mouapi.h"
//#include "../../../inc/timapi.h"
#include "../../../inc/logapi.h"

#include "swis.h"

//*  Added enhanced mouse resolution for new vesa driver see EQU ENH_RES
#define ENH_RES 1

/*=============================================================================
==   Functions used
=============================================================================*/

/*=============================================================================
==   Local Functions defined
=============================================================================*/
int mouCheck( void );
int mouSetup( void );
int mouTakedown( void );
int mouSetHandler( void );
int mouAllocQueue( USHORT uEntries );
void mouClearHandler( void );
void  mouIntHandler( PMOUSERAWDATA pMouseRawData );
void  far mouTimerHook( void );
void mouLoadPref( void );
void  far mouButtonTimerHook( void );

/*=============================================================================
==   DATA
=============================================================================*/


ULONG gdebugtimcount=0l;
ULONG gdebugmoucount=0l;
ULONG gdebugpushcount=0l;


BOOL gfNoMouse=TRUE;

// queue is circular with a first/next index
// one entry is "thrown away" so that empty queue can be identified
// as first==next

PMOUSEDATA gpMouseQueue=NULL;
unsigned int guMouseQueueOverflow=0;
unsigned int guReadingMouseQueue=0;
unsigned int guMouseFirstQueueEntry=0;
unsigned int guMouseNextQueueEntry=0;
unsigned int guMouseTimerCount=0;
unsigned int guMouseDoubleClickTimerCount=0;
unsigned int guMouseDoubleClickTop = 0;
unsigned int guMouseDoubleClickLeft = 0;
unsigned int guMouseDoubleClickRight = 0;
unsigned int guMouseDoubleClickBottom = 0;
unsigned int guMouButtonTimer      = 0;        // used to delay mouse button messages
BOOL         gfSetMouButtonTimer   = FALSE;
BOOL         gfWaitEnabled         = FALSE;
MOUSEDATA gMouseMoveData={0,0,0};
BOOL gfHandlerSet=FALSE;
MOUINFORMATION gInfo={MOUSE_DEFAULT_TIMER,
                      MOUSE_DEFAULT_QSIZE,
                      MOUSE_DEFAULT_DBLCLK_TIMER,
                      MOUSE_DEFAULT_DBLCLK_WIDTH,
                      MOUSE_DEFAULT_DBLCLK_HEIGHT};
PFNHOOK gpMouseIntRootHook = NULL;
PFNHOOK gpMouseReadRootHook = NULL;
MOUPREFERENCES gMousePreferences;
BOOL gfMousePreferencesLoaded = FALSE;

// track state changes this way
// these are the last raw button states received/reported
char gcLastButton1=0;
char gcLastButton2=0;
char gcLastButton3=0;
USHORT guLastX=0;
USHORT guLastY=0;
char gcLastQueueState=0;

USHORT gWidth = 0;
USHORT gHeight = 0;

static USHORT gOriginX = 0;
static USHORT gOriginY = 0;

#define ADJUP(n) ((n)=((n)<(gInfo.uQueueSize-1))?(n)+1:0)
#define ADJDOWN(n) ((n)=(n)?(n)-1:gInfo.uQueueSize-1)

/*******************************************************************************/

static int LastB = 0, LastT = 0, LastX = -1, LastY = -1;

int MouseReadAvail( PUSHORT puCountAvail )
{
    *puCountAvail = 1;
    
    //LastT = 0;

    return CLIENT_STATUS_SUCCESS;
}

/*
 * If this ever does write out more than one structure bear in mind that they
 * are unaligned structures.
 */

int MouseRead(PMOUSEDATA pMouData, PUSHORT puCount)
{
    int x, y, b, t;
    int diff;

    _swix(OS_Mouse, _OUTR(0,3), &x, &y, &b, &t);

    diff = b ^ LastB;

    if (diff ||
	((x != LastX || y != LastY) && (t - LastT) > gInfo.uTimerGran))
    {
	pMouData->X = ((              x - gOriginX) * 0x10000) / gWidth + 1;
	pMouData->Y = ((gHeight - 1 - y + gOriginY) * 0x10000) / gHeight + 1;
	pMouData->cMouState = x != LastX || y != LastY ? MOU_STATUS_MOVED : 0;

	if (diff & 1)		// right
	    pMouData->cMouState |= b & 1 ? MOU_STATUS_B2DOWN : MOU_STATUS_B2UP;
	
	if (diff & 2)		// mid
	    pMouData->cMouState |= b & 2 ? MOU_STATUS_B3DOWN : MOU_STATUS_B3UP;
	
	if (diff & 4)		// left
	    pMouData->cMouState |= b & 4 ? MOU_STATUS_B1DOWN : MOU_STATUS_B1UP;

	TRACE(( TC_MOU, TT_API2, "MouseRead: cooked st=0x%x X=%04x Y=%04x (OS pos %d,%d scrn %d,%d) dT %d",
		pMouData->cMouState, pMouData->X, pMouData->Y,
		x, y, gWidth, gHeight, t - LastT));
	
	LastB = b;
	LastT = t;
	LastX = x;
	LastY = y;

	*puCount = 1;

	return CLIENT_STATUS_SUCCESS;;
    }

    return CLIENT_STATUS_NO_DATA;
}

/*******************************************************************************
 *
 *  MouseLoad
 *
 *    Initialize and return entrypoints.
 *
 * ENTRY:
 *    * pfnMouseProcedures
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/

int far  MouseLoad( PPLIBPROCEDURE pfnMouseProcedures )
{
   int rc;

   // reset and check for mouse
   rc = mouCheck();

   if(rc == CLIENT_STATUS_SUCCESS) {

      rc = mouSetup();
      ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
   }

   TRACE(( TC_MOU, TT_API1, "MouseLoad: rc=%u", rc ));

   return(rc);
}

/*******************************************************************************
 *
 *  MouseUnload
 *
 *    Say bye to mouse.
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    STATUS_NO_MOUSE
 *
 ******************************************************************************/

int far  MouseUnload( void )
{
   int rc=CLIENT_STATUS_SUCCESS;

   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);
   
   // undo the mouse driver things
   rc = mouTakedown();
   ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

   TRACE(( TC_MOU, TT_API1, "MouseUnload: rc=%u", rc ));

   return(rc);
}

/*******************************************************************************
 *
 *  MouseLoadPreferences
 *
 *    Load the Preference information into the local preference structure.
 *
 * ENTRY:
 *    * pPref
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int far  MouseLoadPreferences( PMOUPREFERENCES pPref )
{
   int rc = CLIENT_STATUS_SUCCESS;

   TRACE(( TC_MOU, TT_API1, "MouseLoadPreferences: Loading local copy" ));

   gMousePreferences = *pPref;
   gfMousePreferencesLoaded = TRUE;

   return(rc);
}



/*******************************************************************************
 *
 *  MousePosition
 *
 *    Position mouse cursor on screen
 *
 * ENTRY:
 *    X - x coordinate (0-0xffff)
 *    Y - y coordinate (0-0xffff)
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MousePosition( USHORT X, USHORT Y )
{
    char s[5];
    
   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   /*
    * This changes coordinates from a normalized range
    * of (0,0) to (0xFFFF,0xFFFF) to absolute screen coordinates.
    */
   X = (USHORT)(((ULONG)X * gWidth )/0x10000);
   Y = (USHORT)(((ULONG)Y * gHeight)/0x10000);

   Y = (gHeight - 1 - Y);

   X += gOriginX;
   Y += gOriginY;
   
   TRACE(( TC_MOU, TT_API1, "MousePosition: X=%u Y=%u",X,Y ));

   //  Make a position call to mouse driver
   s[0] = 3;
   s[1] = X;
   s[2] = X >> 8;
   s[3] = Y;
   s[4] = Y >> 8;
   _swix(OS_Word, _INR(0,1), 21, s);
   
   return(CLIENT_STATUS_SUCCESS);
}
/*******************************************************************************
 *
 *  MousePositionAbs
 *
 *    Position mouse cursor on screen, given absolute coordinates
 *
 * ENTRY:
 *    X - x coordinate
 *    Y - y coordinate
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MousePositionAbs( USHORT X, USHORT Y )
{
    char s[5];

    if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   TRACE(( TC_MOU, TT_API1, "MousePositionAbs: X=%u Y=%u",X,Y ));

   X = X * 2;
   Y = (gHeight - 1 - Y) * 2;
   
   X += gOriginX;
   Y += gOriginY;

   //  Make a position call to mouse driver
   //  Make a position call to mouse driver
   s[0] = 3;
   s[1] = X;
   s[2] = X >> 8;
   s[3] = Y;
   s[4] = Y >> 8;
   _swix(OS_Word, _INR(0,1), 21, s);

   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  MouseSetRanges
 *
 *    Set the screen ranges.
 *
 * ENTRY:
 *    dimensions
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MouseSetRanges( USHORT uHoriMin, USHORT uHoriMax,
                                USHORT uVertMin, USHORT uVertMax )
{
    char s[9];
    int hmin, hmax, vmin, vmax;

    if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   TRACE(( TC_MOU, TT_API1, "MouseSetRanges(in): hmin=%u hmax=%u vmin=%u vmax=%u",
                                       uHoriMin,uHoriMax,uVertMin,uVertMax ));

   hmin = uHoriMin*2 + gOriginX;
   hmax = uHoriMax*2 + gOriginX;
   vmin = (gHeight - 1) - uVertMax*2 + gOriginY;
   vmax = (gHeight - 1) - uVertMin*2 + gOriginY;
   
   TRACE(( TC_MOU, TT_API1, "MouseSetRanges(ro): hmin=%u hmax=%u vmin=%u vmax=%u",
                                       hmin,hmax,vmin,vmax ));

   //  set horizontal
   //  set vertical

   s[0] = 1;
   s[1] = hmin;
   s[2] = hmin >> 8;
   s[3] = vmin;
   s[4] = vmin >> 8;
   s[5] = hmax;
   s[6] = hmax >> 8;
   s[7] = vmax;
   s[8] = vmax >> 8;
   _swix(OS_Word, _INR(0,1), 21, s);
  
   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  MouseSetScreenDimensions
 *
 *    Set the size of the screen, for normalizing mouse data.
 *    SJM In OS units, not pixels
 *
 * ENTRY:
 *    dimensions
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/
int far  MouseSetScreenDimensions( USHORT uWidth, USHORT uHeight )
{
   // if called with 0,0 let mouse determine screen dimensions
   if ( uWidth == 0 && uHeight == 0 ) {

      // initialize return values to zero
      // (for unsupported mouse drivers, like IBM PS/2)

      // get virtual screen coords (mouse 6.1+)
       gWidth = 1280;
       gHeight = 960;
   }
   else {
      gWidth = uWidth;
      gHeight = uHeight;
   }

   TRACE(( TC_MOU, TT_API1, "MouseSetScreenDimensions: in: w=%u h=%u, set: w=%u h=%u",uWidth, uHeight,gWidth, gHeight ));

   return(CLIENT_STATUS_SUCCESS);
}

/*
 * In OS units
 */

int MouseSetScreenOrigin( USHORT x, USHORT y )
{
    gOriginX = x;
    gOriginY = y;

    return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  MouseShowPointer
 *
 *    Show or hide mouse pointer.
 *
 * ENTRY:
 *    fOn - flag TRUE to show pointer
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MouseShowPointer( BOOL fOn )
{
   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   TRACE(( TC_MOU, TT_API3, "MouseShowPointer: fOn=%u ",fOn ));

   //  Make a position call to mouse driver
    _swix(OS_Byte, _INR(0,1), 106, fOn ? 1 : 0); // pointer on

   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  MouseReset
 *
 *    Reset mouse software.  Call after graphics mode changes.
 *    Turns off cursor, sets up interrupt handling.
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MouseReset( void )
{
   int rc;

   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   guMouseDoubleClickTimerCount = 0;

   TRACE(( TC_MOU, TT_API1, "MouseReset: " ));
   // do the reset
   rc = mouCheck();

   if(rc == CLIENT_STATUS_SUCCESS)
      // set up the interrupt handler again
      mouSetHandler();


   return(rc);
}


/*******************************************************************************
 *
 *  MouseSetInformation
 *
 *    Set some mouse behaviors.
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MouseSetInformation( PMOUINFORMATION pMouInfo )
{
   int rc=CLIENT_STATUS_SUCCESS;

   TRACE(( TC_MOU, TT_API1, "MouseSetInformation: gran=%u qsize=%u",pMouInfo->uTimerGran, pMouInfo->uQueueSize));

   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   // if changing queue size, then realloc queue on the fly
   if((pMouInfo->uQueueSize) && ((pMouInfo->uQueueSize+1) != gInfo.uQueueSize)) {
      rc = mouAllocQueue(pMouInfo->uQueueSize);
   }

   // change timer tick count (count is 1 based; i.e. 1 means every time)
   if(pMouInfo->uTimerGran)
      gInfo.uTimerGran = pMouInfo->uTimerGran;

   // If 0, do not detect double clicks on the client
   gInfo.uDoubleClickTimerGran = pMouInfo->uDoubleClickTimerGran;

   if(pMouInfo->uDoubleClickWidth)
      gInfo.uDoubleClickWidth = pMouInfo->uDoubleClickWidth;

   if(pMouInfo->uDoubleClickHeight)
      gInfo.uDoubleClickHeight = pMouInfo->uDoubleClickHeight;

   return(rc);
}

/*******************************************************************************
 *
 *  mouCheck
 *
 *    Reset mouse and check existence.
 *
 * ENTRY:
 *    HookClass - type of mouse hook
 *    pProcedure - pointer to hook procedure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int mouCheck( void )
{
   int rc=CLIENT_ERROR_NO_MOUSE;

   // look for mouse vector
   rc = CLIENT_STATUS_SUCCESS;
   gfNoMouse = FALSE;
   gfHandlerSet=FALSE;

   // if first time get the current screen dimensions from mouse driver
   if ( gWidth == 0 && gHeight == 0 )
       MouseSetScreenDimensions( 0, 0 );

   // restore X & Y ranges - reset call for unknown reason changes
   // .. the Y range for SVGA - BUGBUG
   //MouseSetRanges(0, gWidth-1, 0, gHeight-1);

   mouLoadPref();

   TRACE(( TC_MOU, TT_API2, "mouCheck rc=%u", rc ));
   return(rc);
}

/*******************************************************************************
 *
 *  mouSetup
 *
 *    Set up mouse things on first open
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/
int mouSetup( void )
{
   int rc=CLIENT_STATUS_SUCCESS;

   // allocate a queue
   rc = mouAllocQueue(gInfo.uQueueSize);
   if(rc!=CLIENT_STATUS_SUCCESS)
      goto ExitExit;

   // set up timer hook
   guMouseTimerCount=gInfo.uTimerGran;
   if(gInfo.uTimerGran || gInfo.uDoubleClickTimerGran) {
//      rc = TimerAddHook((PVOID)mouTimerHook);
      ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
   }

//   rc = TimerAddHook((PVOID)mouButtonTimerHook);
   ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

   // set up handler (this may have already been done without an Open
   // if there is a hook user with no opens)
   mouSetHandler();

ExitExit:
   TRACE(( TC_MOU, TT_API2, "mouSetup: rc=%u", rc ));
   return(rc);

}
/*******************************************************************************
 *
 *  mouAllocQueue
 *
 *    Set up mouse things on first open
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/
int mouAllocQueue( USHORT uEntries )
{
    int rc=CLIENT_STATUS_SUCCESS;
   return(rc);

}

/*******************************************************************************
 *
 *  mouTakedown
 *
 *    Undo all mouse things after last close.
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    errors
 *
 ******************************************************************************/
int mouTakedown( void )
{
   int rc=CLIENT_STATUS_SUCCESS;

   // deallocate a queue
   if(gpMouseQueue)
      free(gpMouseQueue);

   // set up timer hook
   if(gInfo.uTimerGran || gInfo.uDoubleClickTimerGran) {
//      rc = TimerRemoveHook((PVOID)mouTimerHook);
      ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
   }

//   rc = TimerRemoveHook((PVOID)mouButtonTimerHook);
   ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

   // clear int handler (maybe)
   mouClearHandler();

   return(rc);

}

/*******************************************************************************
 *
 *  mouSetHandler
 *
 *    Add interrupt handler to mouse for special processing.
 *
 * ENTRY:
 *    HookClass - type of mouse hook
 *    pProcedure - pointer to hook procedure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/
int mouSetHandler( void )
{
   PVOID pwork;

   // do the set only if it hasnt already been done
   if(gfHandlerSet==FALSE) {
      gfHandlerSet=TRUE;

   }
   return( CLIENT_STATUS_SUCCESS );
}
/*******************************************************************************
 *
 *  mouClearHandler
 *
 *    Remove interrupt handler, but only if all potential users are gone.
 *
 * ENTRY:
 *
 * EXIT:
 *
 ******************************************************************************/
void mouClearHandler( void )
{

   //  Make a reset call to mouse driver
   gfHandlerSet=FALSE;

   return;
}

/*******************************************************************************
 *
 *  mouLoadPref
 *
 *    Sets the Preferences via an INT 33h call.
 *
 * ENTRY:
 *
 * EXIT:
 *
 * NOTE:
 *    Preferences were established via the MouseLoadPreferences API in this
 *    file.
 ******************************************************************************/
void mouLoadPref( void )
{
   if ( gfMousePreferencesLoaded &&
        ( (gMousePreferences.HorizSpeed != -1) && 
          (gMousePreferences.VertSpeed != -1)  &&
          (gMousePreferences.DblSpeedThreshold != -1) ) ) {

   } else {
      TRACE( (TC_MOU, TT_API2, "mouLoadPref: No Mouse Tracking Changes") );
   }
}


