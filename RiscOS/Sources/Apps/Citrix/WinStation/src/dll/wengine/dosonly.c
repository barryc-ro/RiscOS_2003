/*****************************************************************************
*
*   DOSONLY.C
*
*   DOS only routines for client engine
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Marc Bloomfield (marcb) 13-Mar-1995
*
*   dosonly.c,v
*   Revision 1.1  1998/01/12 11:36:34  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.13   11 Jun 1997 10:29:44   terryt
*  client double click support
*  
*     Rev 1.12   15 Apr 1997 18:18:18   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.11   25 Sep 1995 15:28:12   bradp
*  update
*  
*  
****************************************************************************/

#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/client.h"
#include "../../inc/clib.h"
#include "../../inc/kbdapi.h"
#include "../../inc/mouapi.h"
#include "../../inc/wdapi.h"
#include "../../inc/pdapi.h"
#include "../../inc/vdapi.h"
#include "../../inc/biniapi.h"
#include "../../inc/wengapip.h"
#include "../../inc/logapi.h"

#include "swis.h"

/*=============================================================================
==   Local Functions
=============================================================================*/

/*=============================================================================
==   External Functions
=============================================================================*/

/*=============================================================================
==  External Vars
=============================================================================*/

extern USHORT G_MouseTimer;
extern USHORT G_MouseDoubleClickTimer;
extern USHORT G_MouseDoubleClickHeight;
extern USHORT G_MouseDoubleClickWidth;
extern USHORT G_KeyboardTimer;


/*=============================================================================
==   Local Vars
=============================================================================*/
static LONG vWindowLong = NULL;
static BYTE fMouse = FALSE;
static ULONG KeyboardTime = 0;


/*******************************************************************************
 *
 *  InputPoll
 *
 *    Called to see if there is any mouse/keyboard input and process it if
 *    there is.
 *
 *    NOTE: This routine can not wait!
 *
 * ENTRY:
 *    VOID
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/
INT InputPoll( VOID )
{
    int i;
    int rc;
    int Ch;
    int cCh;
    USHORT Count;
    int ShiftState;
    USHORT CountAvail;
#define MAX_CHARS 10
    UCHAR aCh[MAX_CHARS];
#define MAX_MOUDATA 25
    MOUSEDATA MouData[MAX_MOUDATA];
    KBDCLASS KbdMode;
    MOUINFORMATION MouInfo;
    ULONG CurrentTime;

//  TRACE(( TC_WENG, TT_L1, "InputPoll: (entered)" ));
    
    /*
     *  Check keyboard input buffer first
     */
    rc = KbdReadAvail( (int*)&CountAvail );
    if ( (rc == CLIENT_STATUS_SUCCESS) || KeyboardTime > 0 ) {

        CurrentTime = Getmsec();
        if ( KeyboardTime == 0 ) 
            KeyboardTime = CurrentTime + G_KeyboardTimer;

        if ( CurrentTime >= KeyboardTime ) {

            if ( CountAvail == 0 ) {
                KeyboardTime = 0;

            } else {

                KeyboardTime = CurrentTime + G_KeyboardTimer;
    
                (void) KbdGetMode( &KbdMode );
        
                /*
                 *  Read and buffer keystokes (up to MAX_CHARS)
                 */
                for ( i=0, cCh=0; i < CountAvail; i++ ) {
            
                    if ( KbdMode == Kbd_Ascii ) {
                        
                        if ( rc = KbdReadChar( &Ch, &ShiftState ) ) {
                            break;
                        }
        
                        aCh[cCh++] = Ch;
        
                        if ( cCh == MAX_CHARS ) {
                            cCh = 0;
                            if ( rc = wdCharCode( aCh, MAX_CHARS ) ) {
                                break;
                            }
                        }
                    }
                    else {
        
                        if ( rc = KbdReadScan( &Ch, &ShiftState ) ) {
                            break;
                        }
        
                        aCh[cCh++] = Ch;
        
                        if ( cCh == MAX_CHARS ) {
                            cCh = 0;
                            if ( rc = wdScanCode( aCh, MAX_CHARS ) ) {
                                break;
                            }
                        }
                    }
                }
    
                /*
                 *  Flush any remaining chars
                 */
                if ( cCh ) {
                    int rc2 = rc;
        
                    if ( KbdMode == Kbd_Ascii )
                        rc = wdCharCode( aCh, cCh );
                    else
                        rc = wdScanCode( aCh, cCh );
        
                    //  restore previous ret code (if one)
                    rc = (rc == CLIENT_STATUS_SUCCESS) ? rc2 : rc;
                }
            } 
        }
    }

    //  ignore these error codes
    if ( rc == CLIENT_STATUS_REBOOT  || 
         rc == CLIENT_STATUS_MENUKEY ||
         rc == CLIENT_STATUS_NO_DATA ) {
        rc = CLIENT_STATUS_SUCCESS;
    }

    //  check mouse one no errors
    if ( rc == CLIENT_STATUS_SUCCESS ) {
        /*
         *  BUGBUG: mouse reset needed?
         */
        if ( !fMouse ) {

            if ( MouseReset() == CLIENT_STATUS_SUCCESS ) {
                fMouse = TRUE;

                // 55 msec = one timer tick 1/18.2 seconds
                MouInfo.uTimerGran = (G_MouseTimer / 55) + 1;
                MouInfo.uQueueSize = 0;
                MouInfo.uDoubleClickTimerGran = (G_MouseDoubleClickTimer / 55) + 1;
                MouInfo.uDoubleClickHeight = G_MouseDoubleClickHeight;
                MouInfo.uDoubleClickWidth = G_MouseDoubleClickWidth;
                (void) MouseSetInformation( &MouInfo );
            }
        }
        /*
         *  Mouse data ready
         */
	if ( fMouse && (rc = MouseReadAvail( &CountAvail ) == CLIENT_STATUS_SUCCESS ) ) {
        
            /*
             *  Append mouse data to output buffer
             */
            while ( CountAvail > 0 ) {
        
                Count = min( CountAvail, MAX_MOUDATA );
                if ( rc = MouseRead( MouData, &Count ) )
                    break;
        
                if ( rc = wdMouseData( (LPVOID) MouData, Count ) )
                    break;
    
                CountAvail -= Count;
            }
        }
    }

    //  ignore these error codes
    if ( rc == CLIENT_STATUS_NO_DATA ) {
        rc = CLIENT_STATUS_SUCCESS;
    }

    return( rc );
}

/*******************************************************************************
 *
 *  Function: WinAlloc
 *
 *  Purpose: Allocate far memory
 *
 *  Entry:
 *     cb (input)
 *        size of memory in bytes
 *
 *  Exit:
 *     pointer to allocated memory or NULL if error
 *
 ******************************************************************************/
VOID FAR *WinAlloc( UINT cb )
{
    LPBYTE pMem = malloc( cb );

    if ( pMem ) {
        memset( pMem, 0, cb );
    }

    return( pMem );
}

/*******************************************************************************
 *
 *  Function: WinReAlloc
 *
 *  Purpose: ReAllocate far memory
 *
 *  Entry:
 *     pMem (input)
 *        memory block to free
 *     cb (input)
 *        size of memory in bytes
 *
 *  Exit:
 *     pointer to allocated memory or NULL if error
 *
 ******************************************************************************/
VOID FAR *WinReAlloc( VOID FAR *pMem, UINT cb )
{
    return( realloc( pMem, cb ) );
}

/*******************************************************************************
 *
 *  Function: WinFree
 *
 *  Purpose: Free memory allocated by WinAlloc
 *
 *  Entry:
 *     pMem (input)
 *        memory block to free
 *
 *  Exit:
 *     void
 *
 ******************************************************************************/
VOID WinFree( VOID FAR *pMem )
{
    free( pMem );
}

/*******************************************************************************
 *
 *  Function: VioWM_Create
 *
 *  Purpose: WM_CREATE processing for client window (MainWndProc)
 *           (full screen text only)
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     pheight (input)
 *        pointer to place to put text row height 
 *     pwidth (input)
 *        pointer to place to put character width 
 *
 *  Exit:
 *     0 to indicate WM_PAINT message was processed
 *
 ******************************************************************************/
LRESULT VioWM_Create( HWND hWnd, INT *pheight, INT *pwidth )
{
   LRESULT rc = 0;

   return( rc );
}

/*******************************************************************************
 *
 *  Function: VioWM_Paint
 *
 *  Purpose: WM_PAINT processing for client window (MainWndProc)
 *           (full screen text only)
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     height (input)
 *        text row height 
 *     width (input)
 *        character width 
 *
 *  Exit:
 *     0 to indicate WM_PAINT message was processed
 *
 ******************************************************************************/
LRESULT VioWM_Paint( HWND hWnd, INT height, INT width )
{
   LRESULT rc = 0;

   return( rc );
}

/*******************************************************************************
 *
 *  Function: VioDestroy
 *
 *  Purpose: Cleanup full screen text buffers
 *
 *  Entry:
 *     void
 *
 *  Exit:
 *     void
 *
 ******************************************************************************/
VOID VioDestroy( VOID )
{
}

/*******************************************************************************
 *
 *  Function: DefWindowProc
 *
 *  Purpose: Default window procedure for DOS
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     message (input)
 *        type of message 
 *     uParam (input)
 *        message-specific paramater
 *     lParam (input)
 *        message-specific paramater
 *
 *  Exit:
 *     message specific return code
 *
 ******************************************************************************/
LRESULT WINAPI DefWindowProc( HWND hWnd,
                              UINT message,
                              WPARAM uParam,
                              LPARAM lParam )
{
   LRESULT rc = 0;

   return( rc );
}

/*******************************************************************************
 *
 *  Function: GetWindowLong
 *
 *  Purpose: Get instance data pointer
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     nOffset (input)
 *        offset of value to receive
 *
 *  Exit:
 *     current value of the nOffset LONG
 *
 ******************************************************************************/
LONG WINAPI GetWindowLong( HWND hWnd, INT nOffset )
{
    ASSERT( !nOffset, nOffset ); // better be 0!
    return( vWindowLong );
}

/*******************************************************************************
 *
 *  Function: SetWindowLong
 *
 *  Purpose: Set instance data pointer
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     nOffset (input)
 *        offset of value to receive
 *     nVal (input)
 *        new value to set
 *
 *  Exit:
 *     previous value of the nOffset LONG
 *
 ******************************************************************************/
LONG WINAPI SetWindowLong( HWND hWnd, INT nOffset, LONG nVal )
{
    LONG Old = vWindowLong;

    ASSERT( !nOffset, nOffset ); // better be 0!

    vWindowLong = nVal;

    return( Old );
}

/*******************************************************************************
 *
 *  Function: GetClientRect
 *
 *  Purpose: Get client rectangle
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     lprcl (output)
 *        client rectangle
 *
 *  Exit:
 *     void
 *
 ******************************************************************************/
void WINAPI GetClientRect( HWND hWnd, RECT FAR* lprcl )
{
    lprcl->left   = 0;
    lprcl->top    = 0;
    lprcl->right  = DEF_DESIREDHRES;
    lprcl->bottom = DEF_DESIREDVRES;
}


/*******************************************************************************
 *
 *  Function: FlushScanCode
 *
 *  Purpose: send all queued scan codes down the pipe
 *
 *  Entry:
 *     nothing
 *
 *  Exit:
 *     nothing
 *
 ******************************************************************************/

VOID 
FlushScanCode()
{
}
