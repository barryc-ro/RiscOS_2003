
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
*   mode.c,v
*   Revision 1.1  1998/01/12 11:36:22  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
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

#include "../../../inc/mouapi.h"
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
       1,     0,     80,   25,   640,   400,   8,     8,
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
       1,     0,     80,   25,   640,   400,   8,     8
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
static PWDTEXTMODE G_TextModes;

static int lookup_index(int index)
{
    int i;
    for (i = 0; i < NUM_TEXT_MODES; i++)
    {
	if (G_TextModes[i].Index == index)
	    return i;
    }
    return -1;
}

PWDTEXTMODE GetTextMode(int index)
{
    return &G_TextModes[lookup_index(index)];
}

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

    VioUnsetMode( pIca->hVio );
    
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
#if defined(DOS) || defined(RISCOS)
    BOOL enableMouse;
#endif

    pIca = (PWDICA) pWd->pPrivate;

#if defined(DOS) || defined(RISCOS)
    enableMouse = pIca->fMouseVisible;
#endif

    pIca->TextIndex = (USHORT) *pInputBuffer;

    TRACE(( TC_WD, TT_API1, "SET_VIDEO_MODE: index=%u", pIca->TextIndex ));

    _SetTextMode( pWd, pIca->TextIndex );

#if defined(DOS) || defined(RISCOS)
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

    pIca = (PWDICA) pWd->pPrivate;

    pIca->ShiftState = (USHORT) (*pInputBuffer & 0x70);

    TRACE(( TC_WD, TT_API1, "SET_LED: pIca->ShiftState=0x%02x", pIca->ShiftState ));

    //
    // for DOS, we let the server control the state of the keyboard
    //
    rc = KbdSetLeds( (int)pIca->ShiftState );

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
    int flags, mode;

    _swix(OS_CheckModeValid, _IN(0) | _OUT(0) | _FLAGS, G_VGAVideoModes[0], &mode, &flags);

    TRACE(( TC_WD, TT_API1, "CheckMode: in %d out %d flags %x", G_VGAVideoModes[0], mode, flags));

    if ((flags & _C) == 0)
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
    int i;
    VIOMODEINFO ModeInfo;
#ifdef DOS
    VIOINTENSITY VideoState;
    struct  SREGS       sregs;
    union   REGS        regs;
#endif

    pIca = (PWDICA) pWd->pPrivate;

    pIca->TextIndex = TextIndex;

    /*
     *  Hide the mouse pointer in DOS
     */
#ifdef DOS
    (void) MouseShowPointer( (pIca->fMouseVisible = FALSE) );
#endif

    i = lookup_index(TextIndex);

    TRACE(( TC_WD, TT_API2, "_SetTextMode: Text Index %u array index %d mode %d", TextIndex, i, G_VideoModes[i] ));
    
    if (i != -1)
    {
	ModeInfo.fmt_ID = G_VideoModes[i];
	ModeInfo.col = G_TextModes[i].Columns;
	ModeInfo.row = G_TextModes[i].Rows;
	(void) VioSetMode( &ModeInfo, pIca->hVio );
    }

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
#if defined(DOS) || defined(RISCOS)
    // (void) MouseSetScreenDimensions(0,0);
    {
	PWDTEXTMODE info = GetTextMode(pIca->TextIndex);
	(void) MouseSetScreenDimensions(info->ResolutionX * 2, info->ResolutionY * 2);
    }
#endif
}

