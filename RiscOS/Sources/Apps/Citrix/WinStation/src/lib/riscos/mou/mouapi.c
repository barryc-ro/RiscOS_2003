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
* Revision 1.4  1998/02/06 17:11:36  smiddle
* First alpha release. Fixed problems with palettes, brushes. Added mouse
* pointers and sorted out running and quitting problems.
*
* Version 0.06. Tagged as 'WinStation-0_06'
*
* Revision 1.3  1998/01/30 19:11:02  smiddle
* Fixed clipping (as long as its simple), and palettes (mostly) and text.
* Fixed a few more dodgy alignmenet structures and made some progress
* towards getting the save/restore screen code working.
*
* Version 0.04. Tagged as 'WinStation-0_04'
*
* Revision 1.2  1998/01/27 18:40:26  smiddle
* Lots more work on Thinwire, resulting in being able to (just) see the
* log on screen on the test server.
*
* Version 0.03. Tagged as 'WinStation-0_03'
*
* Revision 1.1  1998/01/19 19:13:36  smiddle
* Added loads of new files (the thinwire, modem, script and ne drivers).
* Discovered I was working around the non-ansi bitfield packing in totally
* the wrong way. When fixed suddenly the screen starts doing things. Time to
* check in.
*
* Version 0.02. Tagged as 'WinStation-0_02'
*
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

#define ADJUP(n) ((n)=((n)<(gInfo.uQueueSize-1))?(n)+1:0)
#define ADJDOWN(n) ((n)=(n)?(n)-1:gInfo.uQueueSize-1)

/*******************************************************************************/

static int LastB = 0, LastT = 0, LastX = -1, LastY = -1;

int MouseReadAvail( PUSHORT puCountAvail )
{
    *puCountAvail = 1;
    
    LastT = 0;

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
	pMouData->X = (           x/2  * 0x10000) / gWidth  + 1;
	pMouData->Y = ((gHeight - y/2) * 0x10000) / gHeight + 1;
	pMouData->cMouState = x != LastX || y != LastY ? MOU_STATUS_MOVED : 0;

	if (diff & 1)		// right
	    pMouData->cMouState |= b & 1 ? MOU_STATUS_B2DOWN : MOU_STATUS_B2UP;
	
	if (diff & 2)		// mid
	    pMouData->cMouState |= b & 2 ? MOU_STATUS_B3DOWN : MOU_STATUS_B3UP;
	
	if (diff & 4)		// left
	    pMouData->cMouState |= b & 4 ? MOU_STATUS_B1DOWN : MOU_STATUS_B1UP;

	LastB = b;
	LastT = t;
	LastX = x;
	LastY = y;

	TRACE(( TC_MOU, TT_API2, "MouseRead: cooked st=0x%x X=%04x Y=%04x (OS pos %d,%d scrn %d,%d)",
		pMouData->cMouState, pMouData->X, pMouData->Y,
		x/2, y/2, gWidth, gHeight));
	
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

#if 0
   // remove hooks
   DeallocHook(&gpMouseReadRootHook, NULL);
   DeallocHook(&gpMouseIntRootHook, NULL);
#endif
   
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

#if 0
/*******************************************************************************
 *
 *  MouseRead
 *
 *    Get mouse data - coordinates are returned in normalized form.
 *
 * ENTRY:
 *    pMouse - pointer to mouse data
 *    puCount - number of mouse data structures to return
 *              returns number returned
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MouseRead( PMOUSEDATA pMouse, PUSHORT puCount )
{
   USHORT ucnt;
   PMOUSEDATA pmdwk;
   PFNHOOK phook;

   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   if(guMouseQueueOverflow) {
      guMouseQueueOverflow = 0;
      TRACE(( TC_MOU, TT_API2, "MouseRead: Queue Overflow" ));
//      ASSERT( FALSE, 0 );
   }

   guReadingMouseQueue = 1;
   for(ucnt=*puCount, pmdwk=pMouse; ucnt>0; ucnt--, pmdwk++) {

      if(guMouseNextQueueEntry!=guMouseFirstQueueEntry) {

         *pmdwk = *(gpMouseQueue+guMouseFirstQueueEntry);

         /*
          * All mouse coordinates are absolute
          * This changes coordinates to a normalized range
          * of (0,0) to (0xFFFF,0xFFFF)
          */
         TRACE(( TC_MOU, TT_API2, "MouseRead: raw X=%u Y=%u",pmdwk->X,pmdwk->Y ));
         pmdwk->X = (USHORT)((((ULONG)pmdwk->X * 0x10000) / gWidth ) + 1);
         pmdwk->Y = (USHORT)((((ULONG)pmdwk->Y * 0x10000) / gHeight) + 1);
         TRACE(( TC_MOU, TT_API2, "MouseRead: cooked st=0x%x X=%04x Y=%04x",pmdwk->cMouState,pmdwk->X,pmdwk->Y ));
         LogPrintf( LOG_CLASS, LOG_KEYBOARD, "MOUSE: st=0x%x X=%04x Y=%04x",
                    pmdwk->cMouState,pmdwk->X,pmdwk->Y );

         // call all hooks
         for (phook=gpMouseReadRootHook; phook != NULL; phook=phook->pNext )
             ((PLIBPROCEDURE)phook->pProcedure)(gpMouseQueue[guMouseFirstQueueEntry],sizeof(PMOUSEDATA));

         ADJUP(guMouseFirstQueueEntry);
      }
      else
         break;
   }
   guReadingMouseQueue = 0;
   *puCount -= ucnt;
   if(!*puCount)
      return(CLIENT_STATUS_NO_DATA);
   else {
      TRACE(( TC_MOU, TT_API1, "MouseRead: count=%u",*puCount ));
      return(CLIENT_STATUS_SUCCESS);
   }
}
#endif

#if 0
/*******************************************************************************
 *
 *  MouseReadAbs
 *
 *    Get mouse data - coordinates are returned in absolute form.
 *
 * ENTRY:
 *    pMouse - pointer to mouse data
 *    puCount - number of mouse data structures to return
 *              returns number returned
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MouseReadAbs( PMOUSEDATA pMouse, PUSHORT puCount )
{
   USHORT ucnt;
   PMOUSEDATA pmdwk;
   PFNHOOK phook;

   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   if(guMouseQueueOverflow) {
      guMouseQueueOverflow = 0;
      TRACE(( TC_MOU, TT_API2, "MouseRead: Queue Overflow" ));
//      ASSERT( FALSE, 0 );
   }

   guReadingMouseQueue = 1;
   for(ucnt=*puCount, pmdwk=pMouse; ucnt>0; ucnt--, pmdwk++) {

      if(guMouseNextQueueEntry!=guMouseFirstQueueEntry) {

         *pmdwk = *(gpMouseQueue+guMouseFirstQueueEntry);

         TRACE(( TC_MOU, TT_API2, "MouseReadAbs: st=0x%x X=%u Y=%u",pmdwk->cMouState,pmdwk->X,pmdwk->Y ));

         // call all hooks
         for (phook=gpMouseReadRootHook; phook != NULL; phook=phook->pNext )
             ((PLIBPROCEDURE)phook->pProcedure)(gpMouseQueue[guMouseFirstQueueEntry],sizeof(PMOUSEDATA));

         ADJUP(guMouseFirstQueueEntry);
      }
      else
         break;
   }
   guReadingMouseQueue = 0;
   *puCount -= ucnt;
   if(!*puCount)
      return(CLIENT_STATUS_NO_DATA);
   else {
      TRACE(( TC_MOU, TT_API1, "MouseReadAbs: count=%u",*puCount ));
      return(CLIENT_STATUS_SUCCESS);
   }
}
#endif

#if 0
/*******************************************************************************
 *
 *  MouseReadAvail
 *
 *    Return mouse queue status data.
 *
 * ENTRY:
 *    pMouse - pointer to mouse data
 *    nByteCount - size of mouse data buffer
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 ******************************************************************************/
int far  MouseReadAvail( PUSHORT puCountAvail )
{
   MOUSEDATA data;
    
   if(gfNoMouse)
      return(CLIENT_STATUS_NO_DATA);

#if 1
   while (mouGetEvent(&data) == CLIENT_STATUS_SUCCESS)
   {
       MousePush(&data);
   }
#endif

   if(guMouseNextQueueEntry>=guMouseFirstQueueEntry)
      *puCountAvail = guMouseNextQueueEntry-guMouseFirstQueueEntry;
   else
      *puCountAvail = gInfo.uQueueSize+guMouseNextQueueEntry-guMouseFirstQueueEntry;

   //TRACE(( TC_MOU, TT_API1, "MouseReadAvail: count=%u tc=%lu mc=%lu pc=%lu", *puCountAvail,gdebugtimcount,gdebugmoucount,gdebugpushcount ));
   if(*puCountAvail)
   {
/*  We want to wait 100ms (2 timer ticks) before we send the mouse button down command,
    this is to fix scrolling error.  gfWaitEnabled is set whenever a mouse button down is queued.
    gfWaitEnabled is cleared whenever the gMouButtonTimer expires or a mouse button up is queued. */ 
        
        if( gfWaitEnabled )
        {
            return(CLIENT_STATUS_NO_DATA);
        }
       return(CLIENT_STATUS_SUCCESS);
   }
   else
      return(CLIENT_STATUS_NO_DATA);

}
#endif

#if 0
/*******************************************************************************
 *
 *  MousePush
 *
 *    Place mouse click on FIFO.
 *
 * ENTRY:
 *    pMouseData - mouse data to push
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_NO_MOUSE
 *
 * Notes:
 *    This queues all button clicks, but will only queue a single
 *    movement-only event.  The sample (or MouseRead) rate will determine
 *    the granularity of mouse moves received.
 *
 *    This function occurs normally at mouse interrupt time.  I dont
 *    think there is a conflict with this and the MouseRead since read is
 *    reading from the front of the queue and this is adding to the end.
 *    The only thing that is looked at by both is
 *    guMouseFirstQueueEntry
 *
 *
 ******************************************************************************/
int far MousePush( PMOUSEDATA pMouseData )
{
   unsigned int unexttobe;
   int rc;

   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

//   TRACE(( TC_MOU, TT_API2, "MousePush: st=0x%x X=%u Y=%u",pMouseData->cMouState,pMouseData->X,pMouseData->Y ));

   ++gdebugpushcount;

   // if the last thing in the queue is movement only, then
   // overwrite it
   if(guMouseNextQueueEntry!=guMouseFirstQueueEntry) {
      if((gcLastQueueState == MOU_STATUS_MOVED) &&
         (!guReadingMouseQueue))
         ADJDOWN(guMouseNextQueueEntry);
   }

   // determine if there is room in the queue
   unexttobe = guMouseNextQueueEntry;
   ADJUP(unexttobe);


   if(unexttobe != guMouseFirstQueueEntry) {

      // yes queue it and set next next
      gcLastQueueState=pMouseData->cMouState;
      if ( pMouseData->cMouState & ( MOU_STATUS_B1DOWN | MOU_STATUS_B2DOWN | MOU_STATUS_B3DOWN ) ) {
         if ( guMouseDoubleClickTimerCount &&
         (pMouseData->X >= guMouseDoubleClickLeft) && (pMouseData->X < guMouseDoubleClickRight) &&
         (pMouseData->Y >= guMouseDoubleClickTop)  && (pMouseData->Y < guMouseDoubleClickBottom)
              ) {
            pMouseData->cMouState |= MOU_STATUS_DBLCLK;
            guMouseDoubleClickTimerCount = 0;
         }
         else {
            guMouseDoubleClickTimerCount = gInfo.uDoubleClickTimerGran;
           
            if ( pMouseData->X >= gInfo.uDoubleClickWidth )
                guMouseDoubleClickLeft = pMouseData->X - gInfo.uDoubleClickWidth/2;
            else
                guMouseDoubleClickLeft = 0;

            guMouseDoubleClickRight = pMouseData->X + gInfo.uDoubleClickWidth/2;

            if ( pMouseData->Y >= gInfo.uDoubleClickHeight )
                guMouseDoubleClickTop = pMouseData->Y - gInfo.uDoubleClickHeight/2;
            else
                guMouseDoubleClickTop = 0;

            guMouseDoubleClickBottom = pMouseData->Y + gInfo.uDoubleClickHeight/2;
         }

         /* don't want to send mouse button up right away, start the timer and tell it to wait */
         gfWaitEnabled  = TRUE;
         gfSetMouButtonTimer = TRUE;            
      }
           /* if we get a mouse button up, we better not wait any longer */
      else if( pMouseData->cMouState & (MOU_STATUS_B1UP | MOU_STATUS_B2UP | MOU_STATUS_B3UP) )
           gfWaitEnabled = FALSE;

      *(gpMouseQueue+guMouseNextQueueEntry)=*pMouseData;
      guMouseNextQueueEntry=unexttobe;
      rc=CLIENT_STATUS_SUCCESS;
      goto ExitExit;
   }

   guMouseQueueOverflow = 1;
   rc=CLIENT_ERROR_QUEUE_FULL;

ExitExit:

   return(rc);

}
#endif

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

   X = X * 2;
   Y = (gHeight - 1 - Y) * 2;
   
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
    if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   TRACE(( TC_MOU, TT_API1, "MouseSetRanges: hmin=%u hmax=%u vmin=%u vmax=%u",
                                       uHoriMin,uHoriMax,uVertMin,uVertMax ));

   //  set horizontal
   //  set vertical

   s[0] = 1;
   s[1] = uHoriMin;
   s[2] = uHoriMin >> 8;
   s[3] = uVertMin;
   s[4] = uVertMin >> 8;
   s[5] = uHoriMax;
   s[6] = uHoriMax >> 8;
   s[7] = uVertMax;
   s[8] = uVertMax >> 8;
// _swix(OS_Word, _INR(0,1), 21, s);
  
   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  MouseSetScreenDimensions
 *
 *    Set the size of the screen, for normalizing mouse data.
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
       gWidth = 640;
       gHeight = 480;
   }
   else {
      gWidth = uWidth;
      gHeight = uHeight;
   }

   TRACE(( TC_MOU, TT_API1, "MouseSetScreenDimensions: in: w=%u h=%u, set: w=%u h=%u",uWidth, uHeight,gWidth, gHeight ));

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

#if 0
/*******************************************************************************
 *
 *  MouseAddHook
 *
 *    Hook into the mouse read and/or mouse interrupt.
 *
 * ENTRY:
 *    HookClass - type of mouse hook
 *    pProcedure - pointer to hook procedure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/
int far  MouseAddHook( MOUSEHOOKCLASS Type, PVOID fnProc )
{
    USHORT rc;

#if 1
    rc = CLIENT_STATUS_SUCCESS;
#else
   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   if(Type==MouseHookRead)
      rc = AllocHook(&gpMouseReadRootHook, fnProc);
   else
      rc = AllocHook(&gpMouseIntRootHook, fnProc);
#endif
   TRACE(( TC_MOU, TT_API1, "MouseAddHook: Type=%u Hook=%lx rc=%u",Type,fnProc,rc ));

   return(rc);
}

/*******************************************************************************
 *
 *  MouseRemoveHook
 *
 *    Unhook the mouse read and/or mouse interrupt.
 *
 * ENTRY:
 *    HookClass - type of mouse hook
 *    pProcedure - pointer to hook procedure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/
int far  MouseRemoveHook( MOUSEHOOKCLASS Type, PVOID fnProc )
{
   USHORT rc;

#if 1
    rc = CLIENT_STATUS_SUCCESS;
#else
   if(gfNoMouse)
      return(CLIENT_ERROR_NO_MOUSE);

   if(Type==MouseHookRead)
      rc = DeallocHook(&gpMouseReadRootHook, fnProc);
   else
      rc = DeallocHook(&gpMouseIntRootHook, fnProc);
#endif
   TRACE(( TC_MOU, TT_API1, "MouseRemoveHook: Type=%u Hook=%lx rc=%u",Type,fnProc,rc ));

   return(rc);
}
#endif
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
   MouseSetRanges(0, gWidth-1, 0, gHeight-1);

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
#if 0
   PMOUSEDATA pnewq;
   PMOUSEDATA poldq;
   USHORT ucnt;
   USHORT usize;
   PMOUSEDATA pmdwk;

   // add one to wants to account for dead entry
   usize = uEntries+1;

   // allocate a queue
   pnewq = malloc(usize*sizeof(MOUSEDATA));
   if(!pnewq) {
      rc = CLIENT_ERROR_NO_MEMORY;
      goto ExitExit;
   }

   memset(pnewq, 0, usize*sizeof(MOUSEDATA));

   // if there already was a queue, then move data to new one and can old one
   if(gpMouseQueue == NULL) {
      // first queue
      // change globals
      gInfo.uQueueSize=usize;
      guMouseFirstQueueEntry=0;
      guMouseNextQueueEntry=0;
      gpMouseQueue=pnewq;
   }
   else {

      // copy all entries from old queue, with no interrupts please

      for(ucnt=usize, pmdwk=pnewq; ucnt>0; ucnt--, pmdwk++) {

         if(guMouseNextQueueEntry!=guMouseFirstQueueEntry) {

            *pmdwk = *(gpMouseQueue+guMouseFirstQueueEntry);
            ADJUP(guMouseFirstQueueEntry);
         }
         else
            break;
      }
      // change globals
      gInfo.uQueueSize=usize;
      guMouseFirstQueueEntry=0;
      guMouseNextQueueEntry=usize-ucnt;

      poldq=gpMouseQueue;
      gpMouseQueue=pnewq;

      //let go of old queue
      free(poldq);

   }
ExitExit:
   TRACE(( TC_MOU, TT_API2, "mouAllocQueue: rc=%u", rc ));
#endif
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

#if 0
/*******************************************************************************
 *
 *  mouIntHandler
 *
 *    Interrupt handler for special processing.
 *
 * ENTRY:
 *    PMOUSEDATA - pointer to mouse data
 *
 * EXIT:
 *    none
 *
 *
 ******************************************************************************/
void  mouIntHandler( PMOUSERAWDATA pMouseRawData )
{
   MOUSEDATA md;
   char cbut;
   PFNHOOK phook;

   ++gdebugmoucount;

   md.cMouState = 0;

   // If ButtonSwapping has been selected, swap the left and right
   // buttons in the RAW data. Note, that on a 3-button mouse, the middle
   // button is BUTTON3.

   if ( gfMousePreferencesLoaded && gMousePreferences.fSwapButtons ) {
       SHORT Btn1Msk, Btn2Msk, NewBtn1, NewBtn2;

       Btn2Msk = pMouseRawData->ButtonState & MOU_RAW_BUTTON2 ? 0xFFFF : 0;
       Btn1Msk = pMouseRawData->ButtonState & MOU_RAW_BUTTON1 ? 0xFFFF : 0;
       NewBtn1 = Btn2Msk & MOU_RAW_BUTTON1;
       NewBtn2 = Btn1Msk & MOU_RAW_BUTTON2;
       pMouseRawData->ButtonState = ( pMouseRawData->ButtonState &
                                      ~(MOU_RAW_BUTTON1|MOU_RAW_BUTTON2) ) |
                                    (NewBtn1|NewBtn2);
   }

   // check for button state changes and/or movement, and
   // queue these

   // look at button 1
   cbut=(char)pMouseRawData->ButtonState & MOU_RAW_BUTTON1;
   if(gcLastButton1!=cbut) {
      if(cbut)
         md.cMouState|=MOU_STATUS_B1DOWN;
      else
         md.cMouState|=MOU_STATUS_B1UP;
      gcLastButton1=cbut;
   }

   // now at button 2
   cbut=(char)pMouseRawData->ButtonState & MOU_RAW_BUTTON2;
   if(gcLastButton2!=cbut) {
      if(cbut)
         md.cMouState|=MOU_STATUS_B2DOWN;
      else
         md.cMouState|=MOU_STATUS_B2UP;
      gcLastButton2=cbut;
   }

   // now at button 3
   cbut=(char)pMouseRawData->ButtonState & MOU_RAW_BUTTON3;
   if(gcLastButton3!=cbut) {
      if(cbut)
         md.cMouState|=MOU_STATUS_B3DOWN;
      else
         md.cMouState|=MOU_STATUS_B3UP;
      gcLastButton3=cbut;
   }

   // set movement if any
   if((pMouseRawData->X != guLastX) || (pMouseRawData->Y != guLastY)) {
      md.cMouState|=MOU_STATUS_MOVED;
      guLastX = pMouseRawData->X;
      guLastY = pMouseRawData->Y;
   }

   // if any state changes, queue state and current position
   if(md.cMouState) {
      md.X = pMouseRawData->X;
      md.Y = pMouseRawData->Y;
      // if move only, defer move queuing until timer tick
      if((md.cMouState == MOU_STATUS_MOVED) && gInfo.uTimerGran)
         gMouseMoveData = md;
      else {
         // since button click has x and y data, throw away timer move data
         gMouseMoveData.cMouState = 0;
         MousePush(&md);
      }
   }

   //
   // call all hooks
   //
   for (phook=gpMouseIntRootHook; phook != NULL; phook=phook->pNext )
       ((PLIBPROCEDURE)phook->pProcedure)(pMouseRawData);

   return;
}
/*******************************************************************************
 *
 *  mouTimerHook
 *
 *    Called on each timer tick.
 *
 * ENTRY:
 *
 * EXIT:
 *
 * NOTE:
 *    should only be hooked if TimerGran is 1 or more; a 0 value
 *    implies that no time delays are applied
 ******************************************************************************/
void  far mouTimerHook( void )
{
   ++gdebugtimcount;
   if ( gInfo.uDoubleClickTimerGran ) {
      if ( guMouseDoubleClickTimerCount != 0 )
         guMouseDoubleClickTimerCount--;
   }

   if ( gInfo.uTimerGran ) {
      if(--guMouseTimerCount == 0) {
         if(gMouseMoveData.cMouState) {
            MousePush(&gMouseMoveData);
            gMouseMoveData.cMouState = 0;
         }
         guMouseTimerCount=gInfo.uTimerGran;
      }
   }
}
#endif

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


/*******************************************************************************
 *
 *  mouButtonTimerHook
 *
 *    Called on each timer tick.
 *
 * ENTRY:
 *
 * EXIT:
 *
 * NOTE:   This is the timer used for the delay in sending a mouse button down message
 *          2 timer ticks = 110ms.  When timer expires tell it not to wait any longer.
 *
 ******************************************************************************/

#if 0
void  far mouButtonTimerHook( void )
{
    if( gfSetMouButtonTimer ){
        guMouButtonTimer    = 2;
        gfSetMouButtonTimer = FALSE;
    }
    else if( guMouButtonTimer <= 0 )
        gfWaitEnabled = FALSE;
    else
        --guMouButtonTimer;    
}
#endif


