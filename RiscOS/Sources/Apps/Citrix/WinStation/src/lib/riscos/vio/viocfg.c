
/*************************************************************************
*
*  VIOCFG.C
*
*  Video configuration routines.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry (3/28/1994)
*
*  viocfg.c,v
*  Revision 1.1  1998/01/12 11:37:36  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.14   15 Apr 1997 18:51:28   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   13 Oct 1995 16:25:36   richa
*  American Airline enhansements.
*
*     Rev 1.12   09 May 1995 12:12:58   kurtp
*  update
*
*     Rev 1.11   03 May 1995 11:15:40   butchd
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

#include "swis.h"

//#include "graph.h"


/*=============================================================================
 ==   Local Vars
 ============================================================================*/

#if 0
short pixels[] = { 200, 350, 400, 480 };

// vesa modes ROW, COL, MODE
short vesamodes[][3] = {
    { 60,  80, 0x108 },
    { 25, 132, 0x109 },
    { 43, 132, 0x10A },
    { 50, 132, 0x10B },
    { 60, 132, 0x10C },
};
#define MAX_VESA_MODES (sizeof(vesamodes)/sizeof(vesamodes[0]))
#endif

/*=============================================================================
 ==   Global Variables
 ============================================================================*/

unsigned int usMaxRow;
unsigned int usMaxCol;
int fMONO;

/*=============================================================================
 ==   Local Functions
 ============================================================================*/

//static int  _setvideomoderowsandcols( short, short, short );
//static void _getvideoconfig( struct videoconfig * );

/*****************************************************************************
*
*  FUNCTION: Set Code Page
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioSetCp( USHORT usReserved, USHORT idCodePage, HVIO hvio )
{
   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Get Video Config
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioGetConfig (USHORT usConfigId, PVIOCONFIGINFO pvioin, HVIO hvio)
{
#if 0
    struct videoconfig gr_info;

   (void) _getvideoconfig( &gr_info );
   pvioin->cbMemory = (ULONG) gr_info.memory * 1024L;

   switch ( gr_info.adapter ) {
   case _MDPA:
   case _HGC:
      pvioin->adapter = DISPLAY_MONOCHROME;
      break;
   case _CGA:
      pvioin->adapter = DISPLAY_CGA;
      break;
   case _EGA:
      pvioin->adapter = DISPLAY_EGA;
      break;
   case _VGA:
      pvioin->adapter = DISPLAY_VGA;
      break;
   }

   switch ( gr_info.monitor ) {
   case _MONO:
      pvioin->display = MONITOR_MONOCHROME;
      break;
   case _COLOR:
      pvioin->display = MONITOR_COLOR;
      break;
   case _ENHCOLOR:
      pvioin->display = MONITOR_ENHANCED;
      break;
   case _ANALOG:
      pvioin->display = MONITOR_8503;
      break;
   default:
      pvioin->display = MONITOR_851X_COLOR;
      break;
   }
#endif
   return( CLIENT_STATUS_SUCCESS );
}


/*****************************************************************************
*
*  FUNCTION: Get Video Mode
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioGetMode (PVIOMODEINFO pvioModeInfo, HVIO hvio)
{
#if 0
    struct videoconfig gr_info;
   int colors;

   (void) _getvideoconfig( &gr_info );

   // clear all elements but len
   memset( &pvioModeInfo->fbType, 0, pvioModeInfo->cb - 2 );

   // set up a "type" field using the mode value
   pvioModeInfo->fbType = (UCHAR) gr_info.mode;

   switch (gr_info.mode) {
   case 0:
   case 2:
      pvioModeInfo->fbType = 5;
      break;
   case 1:
   case 3:
      pvioModeInfo->fbType = 1;
      break;
   case 5:
      pvioModeInfo->fbType = 7;
      break;
   case 0x0F:
      pvioModeInfo->fbType = 2;
      break;
   case 4:
   case 6:
   case 0x0D:
   case 0x0E:
   case 0x10:
   case 0x11:
   case 0x12:
   case 0x13:
      pvioModeInfo->fbType = 3;
      break;
   case 7:
   default:
      pvioModeInfo->fbType = 0;
      break;
   }

   pvioModeInfo->col    = gr_info.numtextcols;
   pvioModeInfo->row    = gr_info.numtextrows;
   pvioModeInfo->hres   = gr_info.numxpixels;
   pvioModeInfo->vres   = gr_info.numypixels;

   if ( (colors = gr_info.numcolors) == 2 ) {
      pvioModeInfo->color = COLORS_2;
   }
   else if ( colors == 4 ) {
      pvioModeInfo->color = COLORS_4;
   }
   else {
      pvioModeInfo->color = COLORS_16;
   }
#endif
   return( CLIENT_STATUS_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION: Get Video State
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioGetState (PVOID pState, HVIO hvio)
{
   USHORT        rc = CLIENT_STATUS_SUCCESS;
#if 0
    PVIOINTENSITY pvi = (PVIOINTENSITY) pState;
   PVIOOVERSCAN  pos = (PVIOOVERSCAN)  pState;
   union REGS    regs;

   switch ( pvi->type ) {
   case 0x0000:   // palstate
      break;
   case 0x0001:   // overscan
      regs.x.ax = 0x1008;
      int86(0x10, &regs, &regs );
      pos->color = (USHORT) regs.h.bh;
      break;
   case 0x0002:   // intensity
      pvi->fs = 0;
      break;
   default:
      rc = 1;
   }
#endif
   return( rc );
}

/*****************************************************************************
*
*  FUNCTION: Set Video Mode
*
*  ENTRY:
*
****************************************************************************/

static char Palette16[] =
{
    19, 0, 16, 0x00, 0x00, 0x00,
    19, 1, 16, 0x00, 0x00, 0x88,
    19, 2, 16, 0x00, 0x88, 0x00,
    19, 3, 16, 0x00, 0x88, 0x88,
    19, 4, 16, 0x88, 0x00, 0x00,
    19, 5, 16, 0x88, 0x00, 0x88,
    19, 6, 16, 0x88, 0x88, 0x00,
    19, 7, 16, 0x88, 0x88, 0x88,
    19, 8,  16, 0x00, 0x00, 0x00,
    19, 9,  16, 0x00, 0x00, 0xFF,
    19, 10, 16, 0x00, 0xFF, 0x00,
    19, 11, 16, 0x00, 0xFF, 0xFF,
    19, 12, 16, 0xFF, 0x00, 0x00,
    19, 13, 16, 0xFF, 0x00, 0xFF,
    19, 14, 16, 0xFF, 0xFF, 0x00,
    19, 15, 16, 0xFF, 0xFF, 0xFF
};

int WFCAPI
VioSetMode (PVIOMODEINFO pvioModeInfo, HVIO hvio)
{
    int rc = CLIENT_STATUS_SUCCESS;

    // change mode
    _swix(OS_WriteI + 22, 0);
    _swix(OS_WriteI + pvioModeInfo->fmt_ID, 0);

    // setup palette
    _swix(OS_WriteN, _INR(0,1), Palette16, sizeof(Palette16));

    // store mode parameters for routines
    usMaxRow = pvioModeInfo->row;
    usMaxCol = pvioModeInfo->col;
    fMONO = FALSE;

#if 0
    union REGS regs;
    short mode;

    if ( pvioModeInfo->cb == -1 ) {
        usMaxRow = pvioModeInfo->row;
        usMaxCol = pvioModeInfo->col;
        return(rc);
    }

    // pick up color mode for text
    switch ((unsigned short)(pvioModeInfo->color)) {
    case 0:
    case COLORS_2:
    case COLORS_4:
        mode = _TEXTMONO;
        break;
    case COLORS_16:
        if(pvioModeInfo->fbType & VGMT_DISABLEBURST)
            mode = (pvioModeInfo->col == 40) ? _TEXTBW40 : _TEXTBW80;
        else
            mode = (pvioModeInfo->col == 40) ? _TEXTC40 : _TEXTC80;
        break;
    }

    // if len at least 8 then set rows and columns
    if(pvioModeInfo->cb >= 8) {
        usMaxRow = pvioModeInfo->row;
        usMaxCol = pvioModeInfo->col;
        rc = _setvideomoderowsandcols(mode, usMaxRow, usMaxCol );
    }
    else {
/*      _setvideomode(mode);        */
        regs.h.ah = 0;
        regs.h.al = (BYTE) mode;
        int86( 0x10, &regs, &regs );
   }
#endif
   return( rc );
}

/*****************************************************************************
*
*  FUNCTION: Set Video State
*
*  ENTRY:
*
****************************************************************************/

int WFCAPI
VioSetState (PVOID pState, HVIO hvio)
{
    USHORT        rc = CLIENT_STATUS_SUCCESS;
#if 0
    PVIOPALSTATE  pps = (PVIOPALSTATE)  pState;
    PVIOOVERSCAN  pos = (PVIOOVERSCAN)  pState;
    PVIOINTENSITY pvi = (PVIOINTENSITY) pState;
    PVIOCOLORREG  pcr = (PVIOCOLORREG)  pState;
    union REGS    regs;

    switch ( pvi->type ) {
    case 0x0000:   // palstate
        regs.x.ax = 0x1000;
        regs.x.bx = (pps->acolor[0] << 8) | (pps->iFirst & 0x0f);
        int86(0x10, &regs, &regs );
        break;
    case 0x0001:   // overscan
        regs.x.ax = 0x1001;
        regs.h.bh = (char) pos->color;
        int86(0x10, &regs, &regs );
        break;
    case 0x0002:   // intensity
        regs.x.ax = 0x1003;
        regs.x.bx = pvi->fs ? 0 : 1;
        int86(0x10, &regs, &regs );
        break;
    case 0x0003:   // color reg
        regs.x.ax = 0x1010;
        regs.x.bx = pcr->firstcolorreg;
        regs.h.ch = pcr->colorregaddr[2];   // green
        regs.h.cl = pcr->colorregaddr[1];   // blue
        regs.h.dh = pcr->colorregaddr[0];   // reg
        int86(0x10, &regs, &regs );
        break;
    case 0x0005:   // underline
        /* BUGBUG -- add support for underline */
        break;
    default:
        rc = 1;
    }
#endif
    return( rc );
}

#if 0

/*****************************************************************************
*
*  FUNCTION: _setvideomoderowsandcols
*
*  ENTRY:
*
****************************************************************************/

static int
_setvideomoderowsandcols( short mode, short rows, short cols )
{
    union REGS regs;
    int rc = CLIENT_STATUS_SUCCESS;
    char Buffer[256];
    char * pBuf = Buffer;
    struct SREGS sregs;
    int i;
    int vesamode;

    // isa modes
    if ( (cols == 40 || cols == 80) && (rows == 25 || rows == 43 || rows == 50) ) {

        // page 0
        regs.x.ax = 0x500;
        int86( 0x10, &regs, &regs );

        // get ega info
        regs.x.ax = 0x1200;
        regs.h.bl = 0x10;
        regs.x.cx = 0;
        int86( 0x10, &regs, &regs );

        // ega emulation present ?
        if ( regs.x.cx ) {

            // scan lines 350 or 400
            regs.x.ax = (rows == 43) ? 0x1201 : 0x1202;
            regs.h.bl = 0x30;
            int86( 0x10, &regs, &regs );

            // color or mono
            regs.x.ax = mode;
            int86( 0x10, &regs, &regs );

            // mono?
            fMONO = (mode == _TEXTMONO) ? TRUE : FALSE;

            // load fonts?
            if ( rows == 43 || rows == 50 ) {
                regs.x.ax = 0x1112;
                regs.h.bl = 0;
                int86( 0x10, &regs, &regs );
            }

            // clear screen
            regs.x.ax = 0x600;
            regs.h.bh = 0x07;           // white on black
            regs.x.cx = 0;
            regs.x.dx = ((rows-1) << 8) + 79;
            int86( 0x10, &regs, &regs );
        }
        else {
            rc = CLIENT_ERROR_INVALID_MODE;
        }
    }

    // vesa modes
    else {

        /*
         *  Query VESA information
         */
        regs.x.ax = 0x4f00;
        regs.x.di = FP_OFF(pBuf);
        sregs.es = FP_SEG(pBuf);
        int86x( 0x10, &regs, &regs, &sregs );

        /*
         *  Look up vesa mode
         */
        vesamode = -1;
        for ( i=0; i<MAX_VESA_MODES; i++ ) {
            if ( rows == vesamodes[i][0] && cols == vesamodes[i][1] ) {
                vesamode = vesamodes[i][2];
                break;
            }
        }

        //  Validate vesa and mode
        if ( regs.x.ax != 0x004f || mode == -1 ) {
            rc = CLIENT_ERROR_INVALID_MODE;
        }
        else {

            /*
             *  Set VESA mode
             */
            regs.x.ax = 0x4f02;
            regs.x.bx = vesamode;
            int86( 0x10, &regs, &regs );
        }
    }

    return( rc );
}

/*****************************************************************************
*
*  FUNCTION: _getvideoconfig
*
*  ENTRY:
*
****************************************************************************/

static void
_getvideoconfig( struct videoconfig * vcfg )
{
    union REGS regs;
    struct SREGS sregs;
    BYTE  VgaVideoInfo[64];
    LPBYTE pVVI = VgaVideoInfo;

    //  vga bios present
    regs.h.ah = 0x1b;
    regs.x.bx = 0x0000;
    regs.x.di = FP_OFF( pVVI );
    sregs.es  = FP_SEG( pVVI );
    int86x( 0x10, &regs, &regs, &sregs );

    //  is vga present ?
    if ( regs.h.al == 0x1b ) {

        //  stash video config junk
        vcfg->numxpixels    = (short) 0;
        vcfg->numypixels    = pixels[ *(pVVI+0x2a)];
        vcfg->numtextcols   = (short) *(pVVI+0x05);
        vcfg->numtextrows   = (short) *(pVVI+0x22);
        vcfg->numcolors     = (short) *(pVVI+0x27);
        vcfg->bitsperpixel  = (short) 0;
        vcfg->numvideopages = (short) *(pVVI+0x29);
        vcfg->mode          = (short) ((BYTE) *(pVVI+0x04));
        vcfg->memory        = (short) (( (BYTE) *(pVVI+0x04) + 1) * 64);

        //  display combo
        regs.x.ax = 0x1a00;
        regs.x.bx = 0x0000;
        int86( 0x10, &regs, &regs );

        switch ( regs.h.bl ) {
            case 0x01:
                vcfg->adapter = _MDPA;
                vcfg->monitor = _MONO;
                break;
            case 0x02:
                vcfg->adapter = _CGA;
                vcfg->monitor = _COLOR;
                break;
            case 0x05:
                vcfg->adapter = _EGA;
                vcfg->monitor = _MONO;
                break;
            case 0x04:
            case 0x06:
                vcfg->adapter = _EGA;
                vcfg->monitor = _COLOR;
                break;
            case 0x07:
                vcfg->adapter = _VGA;
                vcfg->monitor = _ANALOGMONO;
                break;
            case 0x08:
                vcfg->adapter = _VGA;
                vcfg->monitor = _ANALOG;
                break;
            case 0x0a:
                vcfg->adapter = _MCGA;
                vcfg->monitor = _COLOR;
                break;
            case 0x0b:
                vcfg->adapter = _MCGA;
                vcfg->monitor = _ANALOGMONO;
                break;
            case 0x0c:
                vcfg->adapter = _MCGA;
                vcfg->monitor = _ANALOG;
                break;
            default :
                vcfg->adapter = 0;
                vcfg->monitor = 0;
                break;
        }
    }
    else {

        //  oh well, use int 10 to figure out what we got
        regs.x.ax = 0x0f00;
        int86( 0x10, &regs, &regs );

        //  set default junk
        vcfg->numypixels  = 320;
        vcfg->numtextcols = 80;
        vcfg->numtextrows = 25;
        vcfg->adapter     = _CGA;
        vcfg->monitor     = _COLOR;

        //  what mode really
        switch ( regs.h.al ) {
            case 0x07 :
                vcfg->adapter = _MDPA;
                vcfg->monitor = _MONO;
                break;
            case 0x00 :
            case 0x01 :
            case 0x04 :
            case 0x05 :
                vcfg->numtextcols = 40;
                break;
            case 0x06 :
                vcfg->numypixels = 640;
                break;
        }
    }
}
#endif
