
/*******************************************************************************
*
*   WINONLY.C
*
*   Thin Wire Windows - windows only code
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*   Revision 1.2  1998/01/27 18:39:44  smiddle
*   Lots more work on Thinwire, resulting in being able to (just) see the
*   log on screen on the test server.
*
*   Version 0.03. Tagged as 'WinStation-0_03'
*
*   Revision 1.1  1998/01/19 19:13:11  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.25   14 Aug 1997 20:34:34   kurtp
*  update
*  
*     Rev 1.24   14 Aug 1997 15:55:10   kurtp
*  fix full screen, again
*  
*     Rev 1.23   05 Aug 1997 18:33:08   kurtp
*  fix WF1.7 trap as reported by terryt
*  
*     Rev 1.22   04 Aug 1997 19:19:20   kurtp
*  update
*  
*     Rev 1.20   14 Jul 1997 18:21:28   kurtp
*  Add LVB to transparent text ops
*  
*     Rev 1.19   15 Apr 1997 18:17:14   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.19   21 Mar 1997 16:09:44   bradp
*  update
*  
*     Rev 1.18   06 Mar 1997 15:07:36   kurtp
*  update
*  
*     Rev 1.17   18 Jul 1996 13:27:44   marcb
*  update
*  
*     Rev 1.16   17 Jul 1996 19:36:48   jeffm
*  Take the union of the desktop with the browser window.
*  
*     Rev 1.15   28 Jun 1996 13:08:12   marcb
*  update
*  
*     Rev 1.14   25 Jun 1996 17:27:24   marcb
*  update
*  
*     Rev 1.13   30 May 1996 16:55:30   jeffm
*  update
*  
*     Rev 1.12   08 May 1996 14:56:46   jeffm
*  update
*  
*     Rev 1.11   20 Jan 1996 14:29:06   kurtp
*  update
*  
*     Rev 1.10   03 Jan 1996 13:34:44   kurtp
*  update
*
*******************************************************************************/

/*
 *  Get the standard C includes
 */
#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../../../inc/client.h"
#include "twtype.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../../../inc/wengapip.h"

#include "twwin.h"
#include "twdata.h"

#include "swis.h"

ULONG TinyCacheSize  = (ULONG)((USHORT)32*(USHORT)1024);
ULONG LargeCacheSize;
int cbTWPackLen = 0;
LPBYTE pTWPackBuf = NULL;
int vSVGAmode = FALSE;
VDTWCACHE vVdTWCache;
extern HDC  vhdc;
extern HWND vhWnd;
extern COLOR_CAPS vColor;
PVD vpVd = NULL;

/*=============================================================================
==  External Functions Defined
=============================================================================*/

extern BOOL Init16Color( VOID );
extern BOOL Reset16Color( VOID );
extern BOOL Destroy16Color( VOID );
extern BOOL Init256Color( VOID );
extern BOOL Reset256Color( VOID );
extern BOOL Destroy256Color( VOID );


/*=============================================================================
==  Functions Defined
=============================================================================*/



/*=============================================================================
==  Local Data
=============================================================================*/

//HHOOK     vhHook = NULL;
//STATIC HINSTANCE ghInstance; //EMBEDFLAG
VOID      SetMouseHook( HWND );
extern    USHORT    bClickticks;

#ifdef  WIN16
#define   MOUCALLBACK   CALLBACK _loadds
TIMERPROC lpfnTimerProc = NULL;
#else
#define   MOUCALLBACK   CALLBACK
#endif
LRESULT   MOUCALLBACK MouseHookProc( INT nCode, WPARAM wParam, LPARAM lParam );

#define   TIMER_CLICK_TICK  101
void      MOUCALLBACK TimerProc( HWND hWnd, UINT msg, UINT idTimer, DWORD dwTime );

extern HCURSOR vhCursor;
extern HCURSOR vhCursorNot;
extern HCURSOR vhCursorVis;
extern BOOL    fWindowsSynced;

extern ULONG   vcxLVB;
extern ULONG   vcxBytes;
extern ULONG   vcyLVB;
extern ULONG   vcbLVB;

static HGLOBAL h_LVB = NULL;
extern LPBYTE  vpLVB;


/****************************************************************************\
 *  InitThinwire
 *
 *  This routine is called to initialize a thinwire connection
 *
 *  PARAMETERS:
 *     regColorCaps (input)
 *        requested color capablities
 *     uWidth (input)
 *        resolution width in pixels
 *     uHeight (input)
 *        resolution height in pixels
 *
 *  RETURN:
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

    /*
     *  Not initizlized by default
     */
COLOR_CAPS curColorCaps = Color_Cap_Max;

void far InitThinwire( COLOR_CAPS reqColorCaps, USHORT uWidth, USHORT uHeight )
{
       INT cx, cy;
//     WINDOWPLACEMENT wndpl;

#if 0
/*
     *  Save window size for wengine
     */
    SetWindowLong( vhWnd, GWL_WINDOWWIDTH, (LONG) uWidth );
    SetWindowLong( vhWnd, GWL_WINDOWHEIGHT, (LONG) uHeight );


/*
     *  Create LVB if required
     */
    if ( vhdc && ( (GetDeviceCaps( vhdc, BITSPIXEL ) == 16) ||
                  !(GetDeviceCaps( vhdc, RASTERCAPS ) & RC_DEVBITS)) ) {

        /*
         *  Get client work area size
         */
        vcxLVB = (ULONG) uWidth;
        vcyLVB = (ULONG) uHeight;
    
        /*
         *  Dword align width of LVB
         */
        if (vcxLVB % 32 ) {
            vcxLVB += (32L - (vcxLVB % 32L));
        }
    
        /*
         *  Generate byte sizes (extra dword for DWORD aligned bitmaps)
         */
        vcxBytes = vcxLVB / 8L;
        vcbLVB   = vcxBytes * vcyLVB + sizeof(DWORD);

        /*
         *  Free any previous LVB
         */
        if ( vpLVB ) {
            GlobalUnlock(h_LVB);
            GlobalFree(h_LVB);
            vpLVB = NULL;
        }

        /*
         *  Allocate LVB memory
         */
        if ( (h_LVB = GlobalAlloc(GMEM_MOVEABLE, (DWORD)vcbLVB)) ) {
            if ( !(vpLVB = (LPBYTE) GlobalLock(h_LVB)) ) {
                GlobalFree(h_LVB);
            }
        }
    }
#endif    
    /*
     *  Add in border size
     */
    cx = uWidth;
    cy = uHeight;
#if 0
    cx += (INT) GetWindowLong( vhWnd, GWL_CXBORDER );
    cy += (INT) GetWindowLong( vhWnd, GWL_CYBORDER );

    if ( GetWindowLong( vhWnd, GWL_TITLEPRESENT ) ) {
        cy += (INT) GetWindowLong( vhWnd, GWL_CYTITLE );
    }
#endif
    TRACE(( TC_TW, TT_L1,
            "InitThinwire: uWidth(%d) uHeight(%d) cx(%d) cy(%d)",
            uWidth, uHeight, cx, cy ));

#if 1
    SetMode(reqColorCaps, uWidth, uHeight);
#else
    if ( IsIconic( vhWnd ) ) {
        if ( GetWindowPlacement( vhWnd, &wndpl ) ) {
            wndpl.rcNormalPosition.right  = wndpl.rcNormalPosition.left + cx;
            wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top  + cy;
            wndpl.showCmd = SW_SHOWMINNOACTIVE;
            SetWindowPlacement( vhWnd, &wndpl );
        }
    }
    else {
        SetWindowPos( vhWnd, NULL, 0, 0, cx, cy,
                      SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
    }
#endif
    /*
     *  Do we need to reset or initialize color caps?
     */
    if ( curColorCaps != Color_Cap_Max ) {

        /*
         *  Check 16 or 256 color
         */
        if ( reqColorCaps == Color_Cap_16 ) {

           TRACE((TC_TW,TT_L1,
                  "InitThinwire: initializing 16 color capabilities" ));
            /*
             *  Just need a reset, or destroy/init
             */
            if ( curColorCaps == reqColorCaps ) {
                Reset16Color();
            }
            else {
                Destroy256Color();
                Init16Color();
            }
        }
        else if ( reqColorCaps == Color_Cap_256 ) {

           TRACE((TC_TW,TT_L1,
                  "InitThinwire: initializing 256 color capabilities" ));
            /*
             *  Just need a reset, or destroy/init
             */
            if ( curColorCaps == reqColorCaps ) {
                Reset256Color();
            }
            else {
                Destroy16Color();
                Init256Color();
            }
        }
#ifdef DEBUG
        else {
            TRACE((TC_TW,TT_L1,
                   "InitThinwire: Invalid curColorCaps %u", curColorCaps));
            ASSERT( curColorCaps < Color_Cap_64K, curColorCaps );
        }
#endif
    }
    else {

        TRACE((TC_TW,TT_L1,
               "InitThinwire: setting default color capabilities" ));

        switch ( reqColorCaps ) {

        case Color_Cap_16 :
            Init16Color();
            break;

        case Color_Cap_256 :
            Init256Color();
            break;

        default :
            TRACE((TC_TW,TT_L1,
                   "InitThinwire: Invalid curColorCaps %u", curColorCaps));
            ASSERT( curColorCaps < Color_Cap_64K, curColorCaps );
            break;
        }
    }

    /*
     *  Save current color caps
     */
    curColorCaps = reqColorCaps;
}

/****************************************************************************\
 *  TWInitWindow
 *
 *  This routine is called to initialize the display window
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *
 *  RETURN:
 *     0 for success, otherwise error code
 *
\****************************************************************************/
int TWInitWindow( PVD pVd, HWND hWnd )
{
    int rc = CLIENT_STATUS_SUCCESS;

    TRACE(( TC_TW, TT_TW_CONNECT, "TWInitWindow: entered (%08lX)", (ULONG)hWnd ));

    /*
     *  This routine should never be called more than once
     */
    ASSERT( vhdc == NULL, 0 );

    /*
     *  Set mouse hook
     */
//  SetMouseHook( hWnd );

    vpVd = pVd;

    if ( !(vhdc = GetDC(vhWnd = hWnd)) ) {
        rc = CLIENT_ERROR_VD_ERROR;
        TRACE(( TC_TW, TT_ERROR, "TWInitWindow: GetDC failed" ));
        ASSERT( 0, 0 );
    }
    return( rc );
}

#if 0
/****************************************************************************\
 *  TWPaint
 *
 *  This routine is called to update the display window
 *
 *  PARAMETERS:
 *     pVd (input)
 *        pointer to PVD
 *     hWnd (input)
 *        window handle
 *
 *  RETURN:
 *     0 for success, otherwise error code
 *
\****************************************************************************/
int TWPaint( PVD pVd, HWND hWnd )
{
    int rc = CLIENT_STATUS_SUCCESS;
    WDSETINFORMATION wdsi;
    RECT rcl;
    PWDREDRAW pRedraw;
    PWDRCL    prcl;
    USHORT crcl;

    if ( !GetUpdateRect( hWnd, &rcl, FALSE ) ) {
        goto done;
    }

    crcl = 1;
    wdsi.WdInformationLength = 6*crcl + sizeof( WDREDRAW ) - sizeof( WDRCL );

      // round up to next dword boundary
    if ( !(pRedraw = (PWDREDRAW)malloc( wdsi.WdInformationLength + sizeof(DWORD) )) ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        ASSERT( 0, rc );
        goto done;
    }
    memset( pRedraw, 0, wdsi.WdInformationLength );

    pRedraw->cb = wdsi.WdInformationLength - sizeof( pRedraw->cb );
    prcl        = pRedraw->rcl;
    prcl->x = rcl.left;
    prcl->y = rcl.top;
    (LPBYTE)prcl+=3;
    prcl->x = rcl.right;
    prcl->y = rcl.bottom;

    TRACE(( TC_TW, TT_TW_CONNECT, "TWPaint: update rcl l:(%u) t:(%u) r:(%u) b:(%u) Redraw data:",
                                  rcl.left, rcl.top, rcl.right, rcl.bottom ));
    TRACEBUF(( TC_TW, TT_TW_CONNECT, (char far *)pRedraw,
                                     (ULONG)wdsi.WdInformationLength ));

    wdsi.WdInformationClass  = WdRedraw; // Redraw the invalid region
    wdsi.pWdInformation      = pRedraw;
    rc = WdCall( pVd, WD__SETINFORMATION, &wdsi);

    TRACE(( TC_TW, TT_TW_CONNECT, "TWPaint: rc(%d) hWnd(%08lX)",
                                  rc, (ULONG)hWnd ));
done:
    return( rc );
}
#endif

/****************************************************************************\
 *  TWDestroyWindow
 *
 *  This routine is called to terminate the display window
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *
 *  RETURN:
 *     0 for success, otherwise error code
 *
\****************************************************************************/
int TWDestroyWindow( HWND hWnd )
{
    int rc = CLIENT_STATUS_SUCCESS;

    TRACE(( TC_TW, TT_TW_CONNECT, "TWDestroyWindow: entered (%08lX)", (ULONG)hWnd ));

#if 0
    /*
     *  Free any previous LVB
     */
    if ( vpLVB ) {
        GlobalUnlock(h_LVB);
        GlobalFree(h_LVB);
        vpLVB = NULL;
    }
#endif
    
    if ( vhdc ) {

        //deal with cleanup
        if ( vColor == Color_Cap_256 ) {
           Destroy256Color();
        }
        else {
           Destroy16Color();
        }

        ReleaseDC( hWnd, vhdc );
    }
    vhdc  = NULL;
    vhWnd = NULL;

#if 0
    /*
     *  If a hook was set then free it
     */
    if ( vhHook != NULL ) {

        UnhookWindowsHookEx( vhHook );
        vhHook = NULL;
    }
#endif
    
#ifdef WIN16
    /*
     *  If timer proc instance exists then destroy it
     */
    if ( lpfnTimerProc != NULL ) {
        FreeProcInstance( lpfnTimerProc );
        lpfnTimerProc = NULL;
    }
#endif
    return( rc );
}


/*******************************************************************************
 *
 * Initialize object caches
 *
 *   Create cache modules - low mem, XMS and disk
 *
 * ENTRY:
 *    ulLowMem (input)
 *       maximum DOS low memory allowed
 *
 *    ulXMS (input)
 *       maximum DOS XMS allowed
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS
 *    TinyCacheSize  - amount of DOS low memory allocated for Tiny Cache
 *    LargeCacheSize - combined amount of memory/disk allocated for Large Cache
 *
 ******************************************************************************/
int TWInitializeObjectCaches( ULONG ulLowMem, ULONG ulXMS )
{
    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * Free object caches
 *
 *   Deallocate Tiny Cache, Large Cache and information block
 *
 * ENTRY:
 *    None
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS
 *
 ******************************************************************************/
int TWFreeObjectCaches( VOID )
{
    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * TWReadCacheParameters
 *
 *   Get caching parameters from protocol.ini
 *
 * ENTRY:
 *    pInitSection (input)
 *       pointer to ini file section
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS
 *
 ******************************************************************************/
USHORT TWReadCacheParameters( PVOID pIniSection )
{
    int freemem, limit;
    
    /*
     *  Get large cache size (in 1KB chunks)
     */
    LargeCacheSize = (ULONG) bGetPrivateProfileLong( pIniSection,
                                                     INI_LARGECACHE, 
                                                     DEF_LARGECACHE );

    /*
     *  Never greater than 8MB (8KB)
     */
    LargeCacheSize = (LargeCacheSize > 8192) ? 8192 : LargeCacheSize;

    
    /*
     *  Adjust KB to MB
     */
    LargeCacheSize <<= 10;

    // ensure there is some memory left (0.5Mb) for sprites etc.
    _swix(Wimp_SlotSize, _INR(0,1) | _OUT(2), -1, -1, &freemem);
    limit = (freemem - 512*1024) &~ 1023;
    if (LargeCacheSize > limit)
	LargeCacheSize = limit;
    
    return( CLIENT_STATUS_SUCCESS );
}

/****************************************************************************\
 *  TWMoveCursor
 *
 *  Move the mouse cursor
 *
 *  PARAMETERS:
 *     uX (input)
 *        x coordinate
 *     uY (input)
 *        y coordinate
 *
 *  RETURN:
 *     CLIENT_STATUS_SUCCESS
 *
\****************************************************************************/

USHORT far
TWMoveCursor(USHORT uX, USHORT uY)
{
#if 0
    POINT pt;

    /*
     *  Get current cursor position
     */
    GetCursorPos( &pt );

    /*
     *  Only position cursor if we have focus and mouse is within our window
     */
    if ( (vhWnd == GetFocus()) && (vhWnd == WindowFromPoint(pt)) ) {

        /*
         *  Setup up client point and map to screen
         */
        pt.x = uX;
        pt.y = uY;
        ClientToScreen( vhWnd, &pt );

        TRACE(( TC_TW, TT_TW_PTRMOVE,
                "TWMoveCursor: client( %d, %d ) screen( %d, %d )",
                uX, uY, pt.x, pt.y ));

        /*
         *  Move it
         */
        SetCursorPos( pt.x, pt.y );
    }
#endif
    return( CLIENT_STATUS_SUCCESS );
}

#if 0
/****************************************************************************\
 *  ctx_detect_supervga
 *
 *  detect if hardware is capable of supporting svga video mode
 *
 *  PARAMETERS:
 *     void
 *
 *  RETURN:
 *     void
 *
\****************************************************************************/
void far pascal ctx_detect_supervga( void )
{
}
#endif

/****************************************************************************\
 *  TWCmdInitializeThinwireClient (INITIALIZE_THINWIRE_CLIENT service routine)
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
void TWCmdInitializeThinwireClient( HWND hWnd, HDC hdc )
{
   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "TWCmdInitializeThinwireClient: entered" ));

   ASSERT( 0, 0 );

   TWCmdReturn( TRUE ); // return to NewNTCommand or ResumeNTCommand
}


#ifdef LATER
//
// USAGE EXAMPLE
//
{
    RECT   rect;
    INT    cRect;
    LPRECT lpRect;

    /*
     *  Enumerate overlapped visible windows
     */
    if ( wfnEnumRects( hVio, hDC, &lpRect, &cRect ) == CLIENT_STATUS_SUCCESS ) {

        INT i;

        /*
         *  If there are any
         */
        for ( i = 0; i < cRect; i++ ) {

            rect = *(lpRect + i);

            TRACE(( TC_TW, TT_L1, "Scroll: collision, top %u, left %u, bottom %u, right %u",
                                    rect.top, rect.left, rect.bottom, rect.right ));
        }

        /*
         *  Free rect array
         */
        if ( lpRect ) {
            wfnFreeRects( lpRect );
        }
    }
}
#endif

/*******************************************************************************
 *
 *  Function: wfnEnumRects
 *
 *  Purpose: Return array of clipping rectangles in client coordinates
 *
 *  Entry:
 *      hWnd (input)
 *          window handle
 *      hDC (input)
 *  Exit:
 *      *pcRect  = number of rectangles returned (up to MAX_RECTS)
 *      *lppRect = array of clipping rectangles
 *
 ******************************************************************************/


#if 1

int wfnEnumRects( HWND hWnd, HDC hDC, LPRECT FAR * lppRect, INT * pcRect, LPRECT pClipRect )
{
    ASSERT((pClipRect != NULL), (int)pClipRect);

    *pcRect = 1;
    *lppRect = malloc(sizeof(RECT));
    *(*lppRect) = *pClipRect ;

    return 1;
}

#else
//  MAX_RECTS must be at least 4 for off screen client areas
#define MAX_RECTS   64

INT
wfnEnumRects( HWND hWnd, HDC hDC, LPRECT FAR * lppRect, INT * pcRect, LPRECT pClipRect )
{
    INT  rc = CLIENT_STATUS_SUCCESS;
    INT  cRects = MAX_RECTS;
    RECT rect;
    RECT clientRect;
    RECT screenRect;
    HWND hWndZ;
//  PWFEINSTANCE pInstanceData = (PWFEINSTANCE)GetWindowLong( hWnd, GWL_INSTANCEDATA );
    HWND hWndScreen;
    LONG Style;


    /*
     *  None to start
     */
    *pcRect = 0;

#if 0
    /*
     *  Do not bother if we are iconic
     */
    if ( IsIconic( hWnd ) ) {
        *lppRect = NULL;
        return( rc );
    }
#endif
    
    /*
     *  Allocate rectangle array
     */
    if ( !(*lppRect = (LPRECT) malloc( cRects * sizeof(RECT) )) ) {
#ifdef LATER
        rc = (INT) GetLastError();
#else
        rc = CLIENT_ERROR_NO_MEMORY;
#endif
        ASSERT( *lppRect != NULL, 0 );
        TRACE(( TC_TW, TT_ERROR, "wfnEnumRects: malloc failed, rc = %d", rc ));
        goto done;
    }

#if 0
    /*
     *  If no clipping rect is provided use the entire client area
     */
    if ( pClipRect == NULL ) {

        /*
         *  Use entire client area
         */
        pClipRect = &clientRect;

        /*
         *  Get client area
         */
        GetClientRect( hWnd, pClipRect );

        TRACE(( TC_TW, TT_L4, "wfnEnumRects: pClipRect client - top %u, left %u, bottom %u, right %u",
                                pClipRect->top, pClipRect->left, pClipRect->bottom, pClipRect->right ));
    }
#endif

    /*
     *  Map client to screen points
     */
    MapWindowPoints( hWnd, HWND_DESKTOP, (LPPOINT) pClipRect, 2 );

    TRACE(( TC_TW, TT_L4, "wfnEnumRects: pClipRect screen - top %u, left %u, bottom %u, right %u",
                            pClipRect->top, pClipRect->left, pClipRect->bottom, pClipRect->right ));

    /*
     * Get the screen window handle 
     * (If embedded, the screen is the browser window)
     */
    if ( pInstanceData && pInstanceData->hWndPlugin ) {

        HWND hWndParent;
        RECT ParentRect;

        //  Get parent window size
        hWndParent = GetParent( pInstanceData->hWndPlugin );
        GetClientRect( hWndParent, &ParentRect );
        MapWindowPoints( hWndParent, HWND_DESKTOP, (LPPOINT) &ParentRect, 2 );

        /*
         * If the "screen" has any scroll bars,
         * adjust the screen boundaries inside them
         */
        Style = GetWindowLong( hWndParent, GWL_STYLE );
        
        if ( Style & WS_HSCROLL ) {
            ParentRect.bottom -= GetSystemMetrics( SM_CYHSCROLL );
        }
        if ( Style & WS_VSCROLL ) {
            ParentRect.right -= GetSystemMetrics( SM_CXVSCROLL );
        }
        
#ifdef WIN32
        /*
         * If the "screen" has a 3d edge,
         * adjust the screen boundaries inside them
         */
        if ( GetWindowLong( hWndParent, GWL_EXSTYLE ) & WS_EX_CLIENTEDGE ) {
            ParentRect.right -= GetSystemMetrics( SM_CXEDGE );
            ParentRect.bottom -= GetSystemMetrics( SM_CYEDGE );
        }
#endif

        /*
         *  Get screen window size
         */
        hWndScreen = GetDesktopWindow();
#ifdef WIN32
        if ( !SystemParametersInfo( SPI_GETWORKAREA, sizeof(RECT), &screenRect, 0)) 
#endif
        GetWindowRect( hWndScreen, &screenRect );

        screenRect.left   = MAX(screenRect.left, ParentRect.left);
        screenRect.top    = MAX(screenRect.top , ParentRect.top );
        screenRect.bottom = MIN(screenRect.bottom , ParentRect.bottom );
        screenRect.right  = MIN(screenRect.right  , ParentRect.right );

//      {
//        char szBuf[128];
//
//        wsprintf(szBuf, "left %u top %u right %u bottom %u",
//                 screenRect.left, 
//                 screenRect.top,
//                 screenRect.right,
//                 screenRect.bottom);
//        MessageBox( NULL, szBuf, "Screen limit", MB_OK);
//         
//      }

    } else {
        /*
         *  Get screen window size
         */
        hWndScreen = GetDesktopWindow();
        GetWindowRect( hWndScreen, &screenRect );
    }


    /*
     *  Client area off screen-left
     */
    if ( pClipRect->left < screenRect.left ) {

        /*
         *  Start with client area
         */
        *(*lppRect + *pcRect) = *pClipRect;

        /*
         *  Restrict to off screen points
         */
        (*lppRect + *pcRect)->right = screenRect.left;

        /*
         *  Map back to client co-ords
         */
        MapWindowPoints( HWND_DESKTOP, hWnd, (LPPOINT) (*lppRect + *pcRect), 2 );

        TRACE(( TC_TW, TT_L1, "wfnEnumRects: off screen left  - top %u, left %u, bottom %u, right %u",
                                (*lppRect + *pcRect)->top,
                                (*lppRect + *pcRect)->left,
                                (*lppRect + *pcRect)->bottom,
                                (*lppRect + *pcRect)->right ));

        /*
         *  Next
         */
        ++(*pcRect);
    }

    /*
     *  Client area off screen-right
     */
    if ( pClipRect->right > screenRect.right ) {

        /*
         *  Start with client area
         */
        *(*lppRect + *pcRect) = *pClipRect;

        /*
         *  Restrict to off screen points
         */
        (*lppRect + *pcRect)->left = screenRect.right;

        /*
         *  Map back to client co-ords
         */
        MapWindowPoints( HWND_DESKTOP, hWnd, (LPPOINT) (*lppRect + *pcRect), 2 );

        TRACE(( TC_TW, TT_L1, "wfnEnumRects: off screen right  - top %u, left %u, bottom %u, right %u",
                                (*lppRect + *pcRect)->top,
                                (*lppRect + *pcRect)->left,
                                (*lppRect + *pcRect)->bottom,
                                (*lppRect + *pcRect)->right ));
        /*
         *  Next
         */
        ++(*pcRect);
    }

    /*
     *  Client area off screen-top
     */
    if ( pClipRect->top < screenRect.top ) {

        /*
         *  Start with client area
         */
        *(*lppRect + *pcRect) = *pClipRect;

        /*
         *  Restrict to off screen points
         */
        (*lppRect + *pcRect)->bottom = screenRect.top;

        /*
         *  Map back to client co-ords
         */
        MapWindowPoints( HWND_DESKTOP, hWnd, (LPPOINT) (*lppRect + *pcRect), 2 );

        TRACE(( TC_TW, TT_L1, "wfnEnumRects: off screen top  - top %u, left %u, bottom %u, right %u",
                                (*lppRect + *pcRect)->top,
                                (*lppRect + *pcRect)->left,
                                (*lppRect + *pcRect)->bottom,
                                (*lppRect + *pcRect)->right ));

        /*
         *  Next
         */
        ++(*pcRect);
    }

    /*
     *  Client area off screen-bottom
     */
    if ( pClipRect->bottom > screenRect.bottom ) {

        /*
         *  Start with client area
         */
        *(*lppRect + *pcRect) = *pClipRect;

        /*
         *  Restrict to off screen points
         */
        (*lppRect + *pcRect)->top = screenRect.bottom;

        /*
         *  Map back to client co-ords
         */
        MapWindowPoints( HWND_DESKTOP, hWnd, (LPPOINT) (*lppRect + *pcRect), 2 );

        TRACE(( TC_TW, TT_L1, "wfnEnumRects: off screen bottom  - top %u, left %u, bottom %u, right %u",
                                (*lppRect + *pcRect)->top,
                                (*lppRect + *pcRect)->left,
                                (*lppRect + *pcRect)->bottom,
                                (*lppRect + *pcRect)->right ));

        /*
         *  Next
         */
        ++(*pcRect);
    }


    /*
     *  Enumerate overlapping visible windows
     */
    hWndZ = hWnd;

    /*
     *  Find First/Next in z-order
     */
    while ( hWndZ = GetNextWindow( hWndZ, GW_HWNDPREV ) ) {

        /*
         *  Skip hidden windows
         */
        if ( !IsWindowVisible( hWndZ ) )
            continue;

        /*
         *  Get windows rect
         */
        GetWindowRect( hWndZ, &rect );

        TRACE(( TC_TW, TT_L4, "wfnEnumRects: rect screen - top %u, left %u, bottom %u, right %u",
                                rect.top, rect.left, rect.bottom, rect.right ));

        /*
         *  Skip disjoint windows
         */
        if ( !IntersectRect( (*lppRect + *pcRect), &rect, pClipRect ) )
            continue;

        /*
         *  Map screen to client points
         */
        MapWindowPoints( HWND_DESKTOP, hWnd, (LPPOINT) (*lppRect + *pcRect), 2 );

        TRACE(( TC_TW, TT_L1, "wfnEnumRects: intersection - top %u, left %u, bottom %u, right %u",
                                (*lppRect + *pcRect)->top,
                                (*lppRect + *pcRect)->left,
                                (*lppRect + *pcRect)->bottom,
                                (*lppRect + *pcRect)->right ));

        /*
         *  Need to grow array?
         */
        if ( ++(*pcRect) > cRects ) {

            /*
             *  Add more room
             */
            cRects += MAX_RECTS;

            /*
             *  Do the realloc
             */
            if ( !(*lppRect = (LPRECT) realloc( *lppRect, cRects * sizeof(RECT) )) ) {
#ifdef LATER
               rc = (INT) GetLastError();
#else
               rc = CLIENT_ERROR_NO_MEMORY;
#endif
               ASSERT( *lppRect != NULL, 0 );
               TRACE(( TC_TW, TT_ERROR, "wfnEnumRects: realloc failed, rc = %d", rc ));
               goto done;
            }

        }
    }

done:

    /*
     *  Realloc rect array to fit, if empty then dump it
     */
    if ( *pcRect ) {

        /*
         *  Do the realloc
         */
        if ( !(*lppRect = (LPRECT) realloc( *lppRect, *pcRect * sizeof(RECT) )) ) {
#ifdef LATER
           rc = (INT) GetLastError();
#else
           rc = CLIENT_ERROR_NO_MEMORY;
#endif
           ASSERT( *lppRect != NULL, 0 );
           TRACE(( TC_TW, TT_ERROR, "wfnEnumRects: realloc failed, rc = %d", rc ));
        }
    }
    else {

        /*
         *  No elements, free array now
         */
        wfnFreeRects( *lppRect );
        *lppRect = NULL;
    }

    return( rc );
}
#endif

/*******************************************************************************
 *
 *  Function: wfnFreeRects
 *
 *  Purpose:
 *
 *  Entry:
 *
 *  Exit:
 *
 ******************************************************************************/

VOID
wfnFreeRects( LPRECT lpRect )
{

    TRACE(( TC_TW, TT_L4, "wfnFreeRects: lpRect %08x", lpRect ));

    /*
     *  Free rect structure
     */
    if ( lpRect ) {
        free( lpRect );
    }
}

/****************************************************************************
 *
 *  SetMouseHook
 *
 *  PARAMETERS:
 *     none
 *
 *  RETURN:
 *
 ****************************************************************************/

VOID
SetMouseHook( HWND hWnd )
{

//    /*
//     *  Set hook
//     */
//    vhHook = SetWindowsHookEx( WH_MOUSE,
//                               (HOOKPROC) MouseHookProc,
//                               ghInstance,
//#ifdef WIN16
////  BUGBUG: make this work!!!  (HTASK) GetWindowTask( hWnd ) );
//                               (HTASK) 0 );
//#else
//                               (DWORD) GetCurrentThreadId() );
//#endif
//
//    TRACE(( TC_TW, TT_TW_PTRSHAPE, "SetMouseHook: vhHook=%08lx", (ULONG) vhHook ));
//
//#ifdef WIN16
//    /*
//     *  Create timer hook instance
//     */
//    lpfnTimerProc = (TIMERPROC) MakeProcInstance( (FARPROC) TimerProc, ghInstance );
//    ASSERT( lpfnTimerProc != NULL, 0 );
//#endif

    /*
     *  Setup default mouse pointer
     */
//  vhCursor = vhCursorNot = vhCursorVis = LoadCursor( NULL, IDC_ARROW );
}

#if 0
/****************************************************************************
 *
 *  TimerProc
 *
 *  PARAMETERS:
 *     none
 *
 *  RETURN:
 *
 ****************************************************************************/

void MOUCALLBACK
TimerProc( HWND hWnd, UINT msg, UINT idTimer, DWORD dwTime )
{

    TRACE(( TC_TW, TT_TW_PTRSHAPE, "TimerProc: idTimer %u", idTimer ));

    /*
     *  Restore the original cursor image
     */
    SetCursor( (vhCursorVis = vhCursor) );

    /*
     *  Kill the timer
     */
  KillTimer( hWnd, idTimer );
}
#endif

#if 0
/****************************************************************************
 *
 *  MouseHookProc
 *
 *  PARAMETERS:
 *     none
 *
 *  RETURN:
 *
 ****************************************************************************/

LRESULT MOUCALLBACK
MouseHookProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    MOUSEHOOKSTRUCT FAR * pMHS = (MOUSEHOOKSTRUCT FAR *) lParam;

    /*
     *  Only act on pulled messages, while in thinwire and not iconic
     */
    if ( (nCode == HC_ACTION) && !IsIconic(vhWnd) && (fWindowsSynced == TRUE) ) {

        switch ( wParam ) {

            case WM_MOUSEMOVE :

                /*
                 *  Only change cursor for our window
                 */
                if ( vhWnd == pMHS->hwnd ) {
                    SetCursor( vhCursorVis );
                }
                break;

            case WM_LBUTTONDOWN :
            case WM_MBUTTONDOWN :
            case WM_RBUTTONDOWN :

                /*
                 *  Only change cursor for our window
                 */
                if ( vhWnd == pMHS->hwnd ) {

                    /*
                     *  Set click tick timer
                     */
                    if ( bClickticks &&
                         SetTimer( vhWnd,
                                   TIMER_CLICK_TICK,
                                   bClickticks * 50,    // tick * 1/20 of a sec
                                   (TIMERPROC) TimerProc ) ) {
                        vhCursorVis = vhCursorNot;
                    }
                    SetCursor( vhCursorVis );
                }
                break;

            case WM_LBUTTONUP :
            case WM_MBUTTONUP :
            case WM_RBUTTONUP :

                /*
                 *  Only change cursor for our window
                 */
                if ( vhWnd == pMHS->hwnd ) {
                    SetCursor( vhCursorVis );
                }
                break;

            default :

                break;
        }
    }

    return( CallNextHookEx( vhHook, nCode, wParam, lParam ) );
}
#endif

#if 1
int wfnRepaintRects(LPRECT lpRect, INT cRect, BOOL fScreenToScreen)
{
    return 1;
}
#else
/********************************************************************************
 *
 * wfnRepaintRects
 *
 * causes a host repaint of cRect number of rectangles pointed to by lpRect
 *
 *
 ********************************************************************************/

#define MINIMUM_REPAINT_DELAY   5000    //  wait at least 5 seconds ...

int wfnRepaintRects(LPRECT lpRect, INT cRect, BOOL fScreenToScreen)
{
        INT              i;
        int              rc = CLIENT_STATUS_SUCCESS;
        WDSETINFORMATION wdsi;
        PWDREDRAW        pRedraw;
        PWDRCL           prcl;
static  DWORD            lastTickCount = 0;
        DWORD            currTickCount = GetTickCount();

   /*
    *   Only do this if there are rect to process ...
    */
   if (cRect) {

       /*
        *  Have we waited long enough?
        */
       if ( (fScreenToScreen == FALSE ) ||
            ((currTickCount - lastTickCount) > MINIMUM_REPAINT_DELAY) ) {

            //if no rectangles then don't do anything
            wdsi.WdInformationLength = 6*cRect + sizeof(WDREDRAW) - sizeof(WDRCL);

            // round up to next dword boundary
            if ( !(pRedraw = (PWDREDRAW)malloc( wdsi.WdInformationLength + sizeof(DWORD) )) ) {
                rc = CLIENT_ERROR_NO_MEMORY;
                ASSERT( 0, rc );
                goto done;
            }
            memset( pRedraw, 0, wdsi.WdInformationLength );

            pRedraw->cb = wdsi.WdInformationLength - sizeof( pRedraw->cb );
            prcl        = pRedraw->rcl;

            for (i=0; i<cRect ; i++ ) {
               prcl->x = (lpRect + i)->left;
               prcl->y = (lpRect + i)->top;
               (LPBYTE) prcl += 3;
               prcl->x = (lpRect + i)->right;
               prcl->y = (lpRect + i)->bottom;
               (LPBYTE) prcl += 3;
            }
            wdsi.WdInformationClass  = WdRedraw; // Redraw the invalid region
            wdsi.pWdInformation      = pRedraw;
            rc = WdCall(vpVd, WD__SETINFORMATION, &wdsi);
        }
        else {

            /*
             *  For now just invalidate the entire client region and repaint
             */
            InvalidateRect( vhWnd, NULL, FALSE );
            TWPaint( vpVd, vhWnd );
        }

        /*
         *  Update last tick count
         */
        lastTickCount = currTickCount;
    }

done:

    return( rc );
}
#endif
