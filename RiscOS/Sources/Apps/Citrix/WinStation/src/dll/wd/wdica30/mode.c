
/*************************************************************************
*
*   mode.c
*
*   ICA 3.0 WinStation Driver - set mode packets
*
*    PACKET_SET_GRAPHICS      n  set graphics mode
*    PACKET_SET_TEXT          n  set text mode
*    PACKET_SET_VIDEO_MODE    3  set video mode
*    PACKET_SET_LED           1  set keyboard LEDs
*
*   Copyright 1994, Citrix Systems Inc.
*
*   Author: Brad Pedersen (4/9/94)
*
*   $Log$
*  
*     Rev 1.39   30 Apr 1997 14:38:14   terryt
*  shift states again
*  
*     Rev 1.38   15 Apr 1997 18:17:56   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.38   21 Mar 1997 16:09:52   bradp
*  update
*  
*     Rev 1.37   21 Oct 1996 11:34:14   BradA
*  Re-enable the mouse if necessary during a video mode change.
*  
*
*     Rev 1.36   05 Jun 1996 12:04:44   jeffm
*  update
*
*     Rev 1.35   10 Apr 1996 14:46:34   billm
*  CPR 2349: Force text into high intensity mode
*
*     Rev 1.34   13 Oct 1995 16:59:38   richa
*  Added support for some non-standard video adapters.
*
*     Rev 1.33   28 Jul 1995 18:30:00   kurtp
*  update
*
*     Rev 1.32   18 Jul 1995 11:42:02   kurtp
*  update
*
*     Rev 1.31   17 Jul 1995 21:27:08   kurtp
*  update
*
*     Rev 1.30   17 Jul 1995 19:11:14   kurtp
*  update
*
*     Rev 1.29   11 Jul 1995 13:57:52   kurtp
*  update
*
*     Rev 1.28   21 Jun 1995 15:01:16   kurtp
*  update
*
*     Rev 1.27   10 May 1995 14:38:18   kurtp
*  update
*
*     Rev 1.26   09 May 1995 18:15:32   kurtp
*  update
*
*     Rev 1.24   08 May 1995 18:07:00   kurtp
*  update
*
*     Rev 1.23   04 May 1995 18:29:04   kurtp
*  update
*
*     Rev 1.22   03 May 1995 11:46:02   kurtp
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
#include "citrix/ica.h"
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"

#ifdef DOS
#include "../../../inc/dos.h"
#include "../../../inc/mouapi.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"

#include "swis.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

void IcaSetGraphics( PWD, LPBYTE, USHORT );
void IcaSetText( PWD, LPBYTE, USHORT );
void IcaSetVideoMode( PWD, LPBYTE, USHORT );
void IcaSetLed( PWD, LPBYTE, USHORT );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

int _GetValidTextModes( PWDTEXTMODE, USHORT, PUSHORT );
void _SetTextMode( PWD, USHORT );


/*=============================================================================
==   External Functions Used
=============================================================================*/

int VdCall( PWD, USHORT, USHORT, PVOID );
int KbdWrite( PWD, PUCHAR, USHORT );
int AppendICAPacket( PWD, USHORT, LPBYTE, USHORT );


/*=============================================================================
==   Static global data
=============================================================================*/

static USHORT G_VGAVideoModes[] =
{
    46				// 80 x 25
//  27				// 80 x 60
};

static WDTEXTMODE G_VGATextModes[] = {
 /*  index   flags  cols  rows   xres  yres  xfont  yfont */
//     0,     0,     40,   25,   640,   200,   8,    16,
       1,     0,     80,   25,   640,   200,   8,     8,
//     2,     0,     80,   43,   640,   344,   8,     8,
//     3,     0,     80,   50,   640,   400,   8,     8,

// Vesa/Other stuff
//     4,     0,     80,   60,   720,   480,   9,     8        // vesa
//     5,     0,    132,   25,   1188,  400,   9,    16,        // vesa
//     6,     0,    132,   43,   1188,  350,   9,     8,        // vesa
//     7,     0,    132,   50,   1188,  400,   9,     8,        // vesa
//     8,     0,    132,   60,   1188,  480,   9,     8,        // vesa
};

static USHORT G_CGAVideoModes[] =
{
    14				// 80 x 25
//  17				// 132 x 25
};

static WDTEXTMODE G_CGATextModes[] = {
 /*  index   flags  cols  rows   xres  yres  xfont  yfont */
//     0,     0,     40,   25,   640,   200,   8,    16,
       1,     0,     80,   25,   640,   200,   8,     8
//     2,     0,     80,   43,   640,   344,   8,     8,
//     3,     0,     80,   50,   640,   400,   8,     8,

// Vesa/Other stuff
//     4,     0,     80,   60,   720,   480,   9,     8,        // vesa
//     5,     0,    132,   25,   1188,  400,   9,    16         // vesa
//     6,     0,    132,   43,   1188,  350,   9,     8,        // vesa
//     7,     0,    132,   50,   1188,  400,   9,     8,        // vesa
//     8,     0,    132,   60,   1188,  480,   9,     8,        // vesa
};

#define NUM_TEXT_MODES ((sizeof(G_VGATextModes) / sizeof(G_VGATextModes[0])))

static USHORT *G_VideoModes;
       PWDTEXTMODE G_TextModes;

/*******************************************************************************
 *
 *  IcaSetGraphics  (PACKET_SET_GRAPHICS)
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
IcaSetGraphics( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    VDSETINFORMATION Info;
    PWDICA pIca;
    int rc;

    TRACE(( TC_WD, TT_API1, "SET_GRAPHICS" ));

    pIca = (PWDICA) pWd->pPrivate;

    pIca->fGraphics = TRUE;

    /*
     *  Without this delay the video hardware either hangs or gets in
     *  a bad mode when reconnecting after a disconnect.
     */
#ifdef DOS
    Delay( 100L );
#endif

    /*
     *  Free VIO-Win resources
     */
#if !defined( DOS ) && !defined( RISCOS )
    (void) VioDestroyWindow( pIca->hVio );
#endif

    /*
     *  Give thinwire focus of the screen
     *  -- enables graphics mode
     */
    Info.VdInformationClass = VdSetFocus;
    rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  IcaSetText  (PACKET_SET_TEXT)
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
IcaSetText( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    VDSETINFORMATION    Info;
    PWDICA              pIca;
    int                 rc;

    TRACE(( TC_WD, TT_API1, "SET_TEXT" ));

    pIca = (PWDICA) pWd->pPrivate;

    /*
     *  Kill ThinWire focus
     */
    if ( pIca->fGraphics ) {
        Info.VdInformationClass = VdKillFocus;
        rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
        ASSERT( rc == CLIENT_STATUS_SUCCESS || rc == CLIENT_ERROR_VD_NOT_FOUND, rc );
    }

    pIca->fGraphics = FALSE;

    /*
     *  Enable text mode
     */
    _SetTextMode( pWd, pIca->TextIndex );
}


/*******************************************************************************
 *
 *  IcaSetVideoMode  (PACKET_SET_VIDEO_MODE)
 *
 *    P1 = index into G_TextModes
 *    P2 = codepage (low byte)
 *    P3 = codepage (high byte)
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
IcaSetVideoMode( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
#ifdef DOS
    BOOL enableMouse;
#endif

    pIca = (PWDICA) pWd->pPrivate;

#ifdef DOS
    enableMouse = pIca->fMouseVisible;
#endif

    pIca->TextIndex = (USHORT) *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "SET_VIDEO_MODE: index=%u", pIca->TextIndex ));

    _SetTextMode( pWd, pIca->TextIndex );

#ifdef DOS
    /*
     *  Re-enable the mouse if it was previously enabled.
     *  _SetTextMode will disable the mouse.
     */
    if ( enableMouse )
    {
       (void) MouseShowPointer( (pIca->fMouseVisible = TRUE) );
    }
#endif
}


/*******************************************************************************
 *
 *  IcaSetLed  (PACKET_SET_LED)
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
IcaSetLed( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    int rc = 0;
    PWDICA pIca;
#ifdef notdef
    UCHAR  szKeyBuf[6];
    USHORT iKey = 0;
#endif
#ifndef DOS
    BYTE   pbKeyState[256];
#endif
#ifndef DOS
    BYTE   ShiftState;
#endif

#ifdef OLD_CODE
#ifndef DOS
    static BOOL fIsNT;
    static BOOL fVersionCheck = FALSE;
#endif
#endif //OLD_CODE

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ShiftState = (USHORT) (*pInputBuffer & 0x70);

//  debug code
//  {
//     char szBuf[100];
//
//     wsprintf(szBuf,"Shiftstate %x",pIca->ShiftState);
//     MessageBox(NULL, szBuf, "Shiftstate", MB_OK);
//  }

    TRACE(( TC_WD, TT_API1, "SET_LED: pIca->ShiftState=0x%02x", pIca->ShiftState ));


#if 0
#ifdef DOS
    //
    // for DOS, we let the server control the state of the keyboard
    //
    rc = KbdSetLeds( (int)pIca->ShiftState );
#else
    /*
     * Just send the set led to the host.
     * Old hosts will ignore the packet.
     * WF 2.0 hosts will respect the packet.
     */
    GetKeyboardState( (LPBYTE) pbKeyState );

    ShiftState = 0;

    if ( pbKeyState[VK_CAPITAL] & 0x01 ) {
        ShiftState |= 0x40;
    }
    if ( pbKeyState[VK_NUMLOCK] & 0x01 ) {
        ShiftState |= 0x20;
    }
    if ( pbKeyState[VK_SCROLL] & 0x01 ) {
        ShiftState |= 0x10;
    }

    rc = AppendICAPacket( pWd, PACKET_SET_LED, (LPBYTE)&ShiftState, 1 );

#ifdef notdef
    // jdm 05/15/96
    // we have had a major change of heart based on feedback from microsoft
    // and are now allowing the client to control what the state of num lock,
    // caps lock, and scroll lock.  Any differences causes the client to
    // send keystrokes to the server to change its state to match the
    // client.  We never call SetKeyboardState in this technique.
    //
    // also note that we are careful to only change one lock at a time.
    // if you do not follow this rule, you get into a feedback loop that
    // goes on forever between the client and server.  In a way, we
    // have defined an improptu protocol for the state of the keyboard.


    //  get current key states
    GetKeyboardState( (LPBYTE) pbKeyState );

    //  the bit fields of pIca->ShiftState are same as the 40:17 bios data area
    if ( ( (pIca->ShiftState & 0x40) && !(pbKeyState[VK_CAPITAL] & 0x01)) ||
         (!(pIca->ShiftState & 0x40) &&  (pbKeyState[VK_CAPITAL] & 0x01)) ) {

        // Caps Lock key down, key up
        szKeyBuf[iKey++] = 0x3a;
        szKeyBuf[iKey++] = 0xba;
    }

    else if ( ( (pIca->ShiftState & 0x20) && !(pbKeyState[VK_NUMLOCK] & 0x01)) ||
         (!(pIca->ShiftState & 0x20) &&  (pbKeyState[VK_NUMLOCK] & 0x01)) ) {

        // Num Lock key down, key up
        szKeyBuf[iKey++] = 0x45;
        szKeyBuf[iKey++] = 0xc5;
    }

    else if ( ( (pIca->ShiftState & 0x10) && !(pbKeyState[VK_SCROLL] & 0x01)) ||
         (!(pIca->ShiftState & 0x10) &&  (pbKeyState[VK_SCROLL] & 0x01)) ) {

        // Scroll Lock key down, key up
        szKeyBuf[iKey++] = 0x46;
        szKeyBuf[iKey++] = 0xc6;
    }

    if(iKey)
       KbdWrite( pWd, szKeyBuf, iKey);
#endif
#endif
#endif
    
#ifdef OLD_CODE
#ifdef DOS
    //
    // for DOS, we let the server control the state of the keyboard
    //
    rc = KbdSetLeds( (int)pIca->ShiftState );
#elif WIN16

    //  get current key states
    GetKeyboardState( (LPBYTE) pbKeyState );

    //  the bit fields of pIca->ShiftState are same as the 40:17 bios data area
    if ( (pIca->ShiftState & 0x40) )
        pbKeyState[VK_CAPITAL] |= 0x01;
    else
        pbKeyState[VK_CAPITAL] &= ~(0x01);

    if ( (pIca->ShiftState & 0x20) )
        pbKeyState[VK_NUMLOCK] |= 0x01;
    else
        pbKeyState[VK_NUMLOCK] &= ~(0x01);

    if ( (pIca->ShiftState & 0x10) )
        pbKeyState[VK_SCROLL] |= 0x01;
    else
        pbKeyState[VK_SCROLL] &= ~(0x01);

    //  set new key states
    SetKeyboardState( (LPBYTE) pbKeyState );
#elif WIN32

    /*
     *  Did we do the version check call yet?
     */
    if ( !fVersionCheck ) {

        OSVERSIONINFO osvi;

        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx( &osvi );

        fIsNT = (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) ? TRUE : FALSE;
        fVersionCheck  = TRUE;
    }

    //  get current key states
    GetKeyboardState( (LPBYTE) pbKeyState );

    //  if NT do this disgusting hack ... otherwise do it correct
    if ( fIsNT ) {

        //  the bit fields of pIca->ShiftState are same as the 40:17 bios data area
        if ( ( (pIca->ShiftState & 0x40) && !(pbKeyState[VK_CAPITAL] & 0x01)) ||
             (!(pIca->ShiftState & 0x40) &&  (pbKeyState[VK_CAPITAL] & 0x01)) ) {
            keybd_event( VK_CAPITAL, 0xBA, 0, 0 );
            keybd_event( VK_CAPITAL, 0xBA, KEYEVENTF_KEYUP, 0 );
        }

        if ( ( (pIca->ShiftState & 0x20) && !(pbKeyState[VK_NUMLOCK] & 0x01)) ||
             (!(pIca->ShiftState & 0x20) &&  (pbKeyState[VK_NUMLOCK] & 0x01)) ) {
            keybd_event( VK_NUMLOCK, 0xC5,  KEYEVENTF_EXTENDEDKEY | 0, 0 );
            keybd_event( VK_NUMLOCK, 0xC5,  KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        }

        if ( ( (pIca->ShiftState & 0x10) && !(pbKeyState[VK_SCROLL] & 0x01)) ||
             (!(pIca->ShiftState & 0x10) &&  (pbKeyState[VK_SCROLL] & 0x01)) ) {
            keybd_event( VK_SCROLL, 0xC6, KEYEVENTF_EXTENDEDKEY | 0, 0 );
            keybd_event( VK_SCROLL, 0xC6, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );
        }
    }
    else {

        //  the bit fields of pIca->ShiftState are same as the 40:17 bios data area
        if ( (pIca->ShiftState & 0x40) )
            pbKeyState[VK_CAPITAL] |= 0x01;
        else
            pbKeyState[VK_CAPITAL] &= ~(0x01);

        if ( (pIca->ShiftState & 0x20) )
            pbKeyState[VK_NUMLOCK] |= 0x01;
        else
            pbKeyState[VK_NUMLOCK] &= ~(0x01);

        if ( (pIca->ShiftState & 0x10) )
            pbKeyState[VK_SCROLL] |= 0x01;
        else
            pbKeyState[VK_SCROLL] &= ~(0x01);

        //  set new key states
        SetKeyboardState( (LPBYTE) pbKeyState );
    }

#endif
#endif //OLD_CODE

    ASSERT( rc == 0, rc );
}


/*******************************************************************************
 *
 *  _GetValidTextModes
 *
 * ENTRY:
 *    pTextModes (output)
 *       pointer buffer to return array of valid text modes
 *    Length (input)
 *       byte count of buffer
 *    pBytesAvail (output)
 *       address to return the number of bytes actually returned
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - success
 *
 ******************************************************************************/

int
_GetValidTextModes( PWDTEXTMODE pTextModes,
                    USHORT Length,
                    PUSHORT pBytesAvail )
{
    USHORT i;
    USHORT ByteCount = NUM_TEXT_MODES * sizeof(WDTEXTMODE);
    PWDTEXTMODE modes;
    int flags;

    _swix(OS_CheckModeValid, _IN(0) | _FLAGS, G_VGAVideoModes[0], &flags);
    if (flags & _C)
    {
	G_VideoModes = G_VGAVideoModes;
	G_TextModes = G_VGATextModes;
    }
    else
    {
	G_VideoModes = G_CGAVideoModes;
	G_TextModes = G_CGATextModes;
    }
    
    /*
     *  Return number of bytes needed
     */
    *pBytesAvail = ByteCount;

    TRACE(( TC_WD, TT_API1, "_GetValidTextModes: bc %u (%u)",
            ByteCount, ByteCount / sizeof(WDTEXTMODE) ));

    /*
     *  Make sure buffer is big enough
     */
    if ( Length < ByteCount )
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );

    /*
     *  Return standard vga modes
     */
    for ( i=0; i < NUM_TEXT_MODES; i++ ) {
        TRACE(( TC_WD, TT_API1, "Mode %u: %u X %u, res %u X %u, font %u X %u",
                i,
                G_TextModes[i].Columns,
                G_TextModes[i].Rows,
                G_TextModes[i].ResolutionX,
                G_TextModes[i].ResolutionY,
                G_TextModes[i].FontSizeX,
                G_TextModes[i].FontSizeY
                ));
        *pTextModes++ = G_TextModes[i];
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  _SetTextMode
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    TextIndex (input
 *       index into text mode table (G_TextModes)
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
_SetTextMode( PWD pWd, USHORT TextIndex )
{
    PWDICA pIca;
#ifdef DOS
    VIOMODEINFO ModeInfo;
    VIOINTENSITY VideoState;
    struct  SREGS       sregs;
    union   REGS        regs;
#endif

    pIca = (PWDICA) pWd->pPrivate;

    pIca->TextIndex = TextIndex;

    TRACE(( TC_WD, TT_API2, "_SetTextMode: Text Index %u", TextIndex ));

    /*
     *  Hide the mouse pointer in DOS
     */
#ifdef DOS
    (void) MouseShowPointer( (pIca->fMouseVisible = FALSE) );
#endif

    _swix(OS_WriteI + 22, 0);
    _swix(OS_WriteI + G_VideoModes[TextIndex], 0);
    
    /*
     *  Initialize VIO-Win mode, after freeing previous instance
     */
#if 0
    
    (void) VioDestroyWindow( pIca->hVio );

    (void) VioInitWindow( pIca->hVio,
                          G_TextModes[pIca->TextIndex].Rows,
                          G_TextModes[pIca->TextIndex].Columns,
                          FALSE );
#endif

    /*
     *  Set screen dimensions in mouse library
     *  Mouse drivers less than 9.0 mess up the resolution after a
     *  mode switch.
     *
     *  Example:
     *     Start a DOS session.
     *     ALT-Enter
     *     mode 80,25
     *     slick
     *     - the mouse coordinates are not right
     *
     *  This may also help VESA mode problems.
     *
     *  Note that since the host has the G_TextModes table,
     *  it has to match anyway.
     */
#ifdef DOS
    // (void) MouseSetScreenDimensions(0,0);
    (void) MouseSetScreenDimensions(G_TextModes[pIca->TextIndex].ResolutionX,
                                    G_TextModes[pIca->TextIndex].ResolutionY);
#endif
}

