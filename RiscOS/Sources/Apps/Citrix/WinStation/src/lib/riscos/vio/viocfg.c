
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
#include "vdu.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*  Get CLIB includes */
#include "../../../inc/client.h"
#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/vioapi.h"

#include "swis.h"

#include "vio.h"

/*=============================================================================
 ==   Local Vars
 ============================================================================*/

/*=============================================================================
 ==   Global Variables
 ============================================================================*/

unsigned int usMaxRow;
unsigned int usMaxCol;
int fMONO;
char *VioScreen = NULL;

static int ModeNumber;

/*=============================================================================
 ==   Local Functions
 ============================================================================*/

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
    TRACE((TC_VIO, TT_API1, "VioSetCp: page %d\n", idCodePage));
    
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
    // clear all elements but len
    memset( &pvioModeInfo->fbType, 0, pvioModeInfo->cb - 2 );
    
    pvioModeInfo->fmt_ID = ModeNumber;
    
    pvioModeInfo->col    = usMaxCol;
    pvioModeInfo->row    = usMaxRow;
    pvioModeInfo->color  = COLORS_16;

    pvioModeInfo->hres   = vduval(vduvar_XLimit) + 1;
    pvioModeInfo->vres   = vduval(vduvar_YLimit) + 1;
    
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
    PVIOINTENSITY pvi = (PVIOINTENSITY) pState;
    PVIOOVERSCAN  pos = (PVIOOVERSCAN)  pState;

    switch ( pvi->type ) {
    case 0x0000:   // palstate
	break;
    case 0x0001:   // overscan
	pos->color = 0;
	break;
    case 0x0002:   // intensity
	pvi->fs = 0;
	break;
    default:
	rc = 1;
	break;
   }

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
    char s[10];

    TRACE(( TC_WD, TT_API1, "VioSetMode: mode %d", pvioModeInfo->fmt_ID ));
    
    FadeScreen(0);

    // change mode
    _swix(OS_WriteI + 22, 0);
    _swix(OS_WriteI + pvioModeInfo->fmt_ID, 0);

    // setup palette
    _swix(OS_WriteN, _INR(0,1), Palette16, sizeof(Palette16));

    // enable scroll protect
    memset(s, 0, sizeof(s));
    s[0] = 23;
    s[1] = 0xFE;		// AND
    s[2] = 1;			// XOR
    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));
    
    // store mode parameters for routines
    ModeNumber = pvioModeInfo->fmt_ID;
    usMaxRow = pvioModeInfo->row;
    usMaxCol = pvioModeInfo->col;
    fMONO = FALSE;

    // redefine characters
    setup_char_defs();

    if (VioScreen)
	free(VioScreen);
    VioScreen = malloc(usMaxRow * usMaxCol * 2);
    
    return( rc );
}

void VioUnsetMode(HVIO hvio)
{
    free(VioScreen);
    VioScreen = NULL;

    reset_char_defs();
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

    PVIOINTENSITY pvi = (PVIOINTENSITY) pState;

    switch ( pvi->type ) {
    case 0x0000:   // palstate
        break;
    case 0x0001:   // overscan
        break;
    case 0x0002:   // intensity
        break;
    case 0x0003:   // color reg
        break;
    case 0x0005:   // underline
        /* BUGBUG -- add support for underline */
        break;
    default:
        rc = 1;
    }
    return( rc );
}

