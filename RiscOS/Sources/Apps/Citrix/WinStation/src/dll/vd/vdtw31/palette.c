
/*******************************************************************************
*
*   PALETTE.C
*
*   Thin Wire Windows - Palette Code
*
*   Copyright (c) Citrix Systems Inc. 1995-1996
*
*   Author: Kurt Perry (kurtp) 14-Sep-1995
*
*   $Log$
*   Revision 1.2  1998/01/28 18:14:47  smiddle
*   Safety checkin
*
*   Revision 1.1  1998/01/19 19:12:43  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.47   06 May 1997 15:44:32   kurtp
*  Fix MSACCESS 2.0/Win95 bug
*  
*     Rev 1.46   15 Apr 1997 18:15:40   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.47   09 Apr 1997 16:22:04   kurtp
*  update
*  
*     Rev 1.46   21 Mar 1997 16:09:22   bradp
*  update
*  
*     Rev 1.45   06 Dec 1996 12:00:22   kurtp
*  update
*  
*     Rev 1.44   20 Jun 1996 13:50:42   jeffm
*  Don't use UpdateColors for plugin
*  
*     Rev 1.43   11 Jun 1996 19:55:16   jeffm
*  update
*  
*     Rev 1.41   30 May 1996 16:56:24   jeffm
*  update
*  
*     Rev 1.40   20 May 1996 19:08:56   jeffm
*  update
*  
*     Rev 1.37   12 Feb 1996 17:15:30   kurtp
*  update
*  
*     Rev 1.36   08 Feb 1996 12:17:28   kurtp
*  update
*  
*     Rev 1.35   15 Jan 1996 15:42:34   kurtp
*  update
*  
*     Rev 1.34   12 Jan 1996 12:16:14   kurtp
*  update
*  
*     Rev 1.33   03 Jan 1996 13:32:36   kurtp
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
#include "../../../inc/pdapi.h"
#include "../../../inc/wengapip.h"

#include "twwin.h"
#include "twdata.h"
#include "wfcache.h"

/*
 *  Currently only valid for windows client's
 */
#ifndef DOS

/*=============================================================================
==   Typedefs and Structures
=============================================================================*/

#ifdef  WIN16
#define   PALCALLBACK   CALLBACK _loadds
TIMERPROC lpfnPaletteTimerProc = NULL;
#else
#define   PALCALLBACK   CALLBACK 
#endif
#define   PALETTE_TIMER_ID          102
#define   PALETTE_TIMER_TIMEOUT     2500

void      PALCALLBACK PaletteTimerProc( HWND hWnd, UINT msg, UINT idTimer, DWORD dwTime );


/*=============================================================================
==   Functions Defined
=============================================================================*/

BOOL    bUseStaticEntries( HWND, HDC );
BOOL    SetStaticColorUse( HWND, HDC, BOOL );
USHORT  TWRealizePalette( HWND, HDC, UINT *, USHORT );
VOID    UpdateSystemColors( HWND, HDC, BOOL );
BOOL    CALLBACK EnumWindowsProc( HWND, LPARAM );
BOOL    IsPluginWindow(HWND);

/*=============================================================================
==   Defines and typedefs
=============================================================================*/

/*
 *  Logical palette is 256 entries big, just the same as the
 *  system palette.  Entries 0 and 255 are reserved for colors
 *  BLACK (RGB 0x00,0x00,0x00) and WHITE (RGB 0xFF,0xFF,0xFF),
 *  therefore entry 0 and 255 are not accessable.  The high 
 *  static values start at entry 246.
 */

#define LOGICAL_PALETTE_SIZE                256
#define LOGICAL_PALETTE_MAX                 255
#define LOGICAL_PALETTE_STATIC              246


/*
 *  The palette index byte is the flags since 0 and 255 are reserved
 *  then we use 1-254 as valid entries and 0 and 255 as special case
 *  entries.
 */

#define TW_PALETTE_INIT                     0x00
#define TW_PALETTE_RESERVED                 0xFF

#define MAX_UPDATE_COLORS                   3

/*
 *  Cache control word stuff
 */
#define BYTES_PER_ENTRY                     3


#define VALIDATE_SYSTEM_LOGICAL_STATIC      0
#define VALIDATE_SYSTEM_LOGICAL_ALL         1
#define VALIDATE_SYSTEM_DEFAULT_STATIC      2


/*=============================================================================
 ==   Data
 ============================================================================*/

extern HDC       compatDC;
//extern HINSTANCE ghInstance;

HPALETTE     vhPalette             = NULL;
HGLOBAL      vhLogPalette          = NULL;
LOGPALETTE * vpLogPalette          = NULL;
BOOL         vfPaletteDevice       = TRUE;
DWORD        vdwPaletteTime        = 0L;
BOOL         vfUsingDefaultPalette = TRUE;
BOOL         vfWeirdSystemPalette  = FALSE;
BOOL         vfPaletteAvailable    = FALSE;


//  Note: this table was taken from the VGA256 driver
//
//  Global Table defining the 20 Window Default Colors.  For 256 color
//  palettes the first 10 must be put at the beginning of the palette
//  and the last 10 at the end of the palette.

const PALETTEENTRY BASEPALETTE[20] =
{
    { 0,   0,   0,   0 },       // 0
    { 0x80,0,   0,   0 },       // 1
    { 0,   0x80,0,   0 },       // 2
    { 0x80,0x80,0,   0 },       // 3
    { 0,   0,   0x80,0 },       // 4
    { 0x80,0,   0x80,0 },       // 5
    { 0,   0x80,0x80,0 },       // 6
    { 0xC0,0xC0,0xC0,0 },       // 7
    { 192, 220, 192, 0 },       // 8
    { 166, 202, 240, 0 },       // 9
    { 255, 251, 240, 0 },       // 10
    { 160, 160, 164, 0 },       // 11
    { 0x80,0x80,0x80,0 },       // 12
    { 0xFF,0,   0   ,0 },       // 13
    { 0,   0xFF,0   ,0 },       // 14
    { 0xFF,0xFF,0   ,0 },       // 15
    { 0   ,0,   0xFF,0 },       // 16
    { 0xFF,0,   0xFF,0 },       // 17
    { 0,   0xFF,0xFF,0 },       // 18
    { 0xFF,0xFF,0xFF,0 },       // 19
};


/*
 *  These structs are used to map the system colors
 *  when the client is using the static colors in the
 *  system palette.  It also stores the current colors
 *  before changing so it can restore the system colors.
 */ 
#define rgbBlack    RGB(0,0,0)
#define rgbWhite    RGB(255,255,255)

#if 0
#define NumSysColors (sizeof(SysPalIndex)/sizeof(SysPalIndex[0]))
static int SysPalIndex[] = {
    COLOR_ACTIVEBORDER,   
    COLOR_ACTIVECAPTION,  
    COLOR_APPWORKSPACE,   
    COLOR_BACKGROUND,     
    COLOR_BTNFACE,        
    COLOR_BTNSHADOW,      
    COLOR_BTNTEXT,        
    COLOR_CAPTIONTEXT,    
    COLOR_GRAYTEXT,       
    COLOR_HIGHLIGHT,      
    COLOR_HIGHLIGHTTEXT,  
    COLOR_INACTIVEBORDER, 
    COLOR_INACTIVECAPTION,
    COLOR_MENU,           
    COLOR_MENUTEXT,       
    COLOR_SCROLLBAR,      
    COLOR_WINDOW,         
    COLOR_WINDOWFRAME,    
    COLOR_WINDOWTEXT,     
};
static COLORREF OldColors[NumSysColors];

#endif

static COLORREF MonoColors[] = {
    rgbBlack,
    rgbWhite,
    rgbWhite,
    rgbWhite,
    rgbWhite,
    rgbBlack,
    rgbBlack,
    rgbBlack,
    rgbBlack,
    rgbBlack,
    rgbWhite,
    rgbWhite,
    rgbWhite,
    rgbWhite,
    rgbBlack,
    rgbWhite,
    rgbWhite,
    rgbBlack,
    rgbBlack,
};


#ifdef DEBUG
char * pszIdRealize[] = {
    "TWREALIZEPALETTE_INIT_FG",
    "TWREALIZEPALETTE_INIT_BG",
    "TWREALIZEPALETTE_FG",    
    "TWREALIZEPALETTE_BG",    
    "TWREALIZEPALETTE_FOCUS", 
    "TWREALIZEPALETTE_SET_FG",
    "TWREALIZEPALETTE_SET_BG",
};
#endif

/****************************************************************************
 ****
 ****   Internal routines follow
 ****
 ****************************************************************************/

PALETTEENTRY apalSystem[LOGICAL_PALETTE_SIZE];

BOOL
ValidateSystemPalette( HWND hwnd, HDC hdc, USHORT uMode ) 
{
    INT i;

    TRACE((TC_TW,TT_TW_PALETTE, "ValidateSystemPalette: enter, uMode = %u", uMode));

    /*
     *  Do not bother on non-palette devices or if we ar not active
     */
    if ( (vfPaletteDevice == TRUE) && 
         (vpLogPalette != NULL ) /*&&
         (hwnd == GetActiveWindow()) */) {

        TRACE((TC_TW,TT_TW_PALETTE, "ValidateSystemPalette: validating" ));

        /*
         *  Validate palette size
         */
//      ASSERT( GetDeviceCaps( hdc, SIZEPALETTE ) == LOGICAL_PALETTE_SIZE, LOGICAL_PALETTE_SIZE );

        /*
         *  Read system palette
         */
        GetSystemPaletteEntries( hdc, 0, LOGICAL_PALETTE_SIZE, apalSystem );

        /*
         *  Validate each entry
         */
        if ( uMode == VALIDATE_SYSTEM_LOGICAL_STATIC ) {

            /*
             *  Static values
             */
            for ( i = 0; i < 10; i++ ) {
        
                // First 10
                if ( (vpLogPalette->palPalEntry[i].peRed   != apalSystem[i].peRed)   ||
                     (vpLogPalette->palPalEntry[i].peGreen != apalSystem[i].peGreen) ||
                     (vpLogPalette->palPalEntry[i].peBlue  != apalSystem[i].peBlue) ) {

                    TRACE((TC_TW,TT_TW_PALETTE, "ValidateSystemPalette: exit FALSE" ));
                    return FALSE;
                }
        
                // Last 10
                if ( (vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peRed   != apalSystem[i+LOGICAL_PALETTE_STATIC].peRed)   ||
                     (vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peGreen != apalSystem[i+LOGICAL_PALETTE_STATIC].peGreen) ||
                     (vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peBlue  != apalSystem[i+LOGICAL_PALETTE_STATIC].peBlue) ) {
        
                    TRACE((TC_TW,TT_TW_PALETTE, "ValidateSystemPalette: exit FALSE" ));
                    return FALSE;
                }
            }
        }
        else if ( uMode == VALIDATE_SYSTEM_LOGICAL_ALL ) {

            for ( i=0; i<LOGICAL_PALETTE_SIZE; i++ ) {
    
                // all
                if ( (vpLogPalette->palPalEntry[i].peRed   != apalSystem[i].peRed)   ||
                     (vpLogPalette->palPalEntry[i].peGreen != apalSystem[i].peGreen) ||
                     (vpLogPalette->palPalEntry[i].peBlue  != apalSystem[i].peBlue) ) {
                    
                    TRACE((TC_TW,TT_TW_PALETTE, "ValidateSystemPalette: exit FALSE" ));
                    return FALSE;
                }
            }
        }
        else if ( uMode == VALIDATE_SYSTEM_DEFAULT_STATIC ) {

            /*
             *  Static values
             */
            for ( i = 0; i < 10; i++ ) {
        
                // First 10
                if ( (BASEPALETTE[i].peRed   != apalSystem[i].peRed)   ||
                     (BASEPALETTE[i].peGreen != apalSystem[i].peGreen) ||
                     (BASEPALETTE[i].peBlue  != apalSystem[i].peBlue) ) {

                    TRACE((TC_TW,TT_TW_PALETTE, "ValidateSystemPalette: entry %u, FALSE", i ));
                    return FALSE;
                }
        
                // Last 10
                if ( (BASEPALETTE[i+10].peRed   != apalSystem[i+LOGICAL_PALETTE_STATIC].peRed)   ||
                     (BASEPALETTE[i+10].peGreen != apalSystem[i+LOGICAL_PALETTE_STATIC].peGreen) ||
                     (BASEPALETTE[i+10].peBlue  != apalSystem[i+LOGICAL_PALETTE_STATIC].peBlue) ) {
        
                    TRACE((TC_TW,TT_TW_PALETTE, "ValidateSystemPalette: entry %u, FALSE", i+10 ));
                    return FALSE;
                }
            }
        }
    }

    TRACE((TC_TW,TT_TW_PALETTE, "ValidateSystemPalette: exit TRUE" ));
    return TRUE;
}


#if 0
/****************************************************************************
 *
 *  PaletteTimerProc
 *
 *  PARAMETERS:
 *     none
 *
 *  RETURN:
 *
 ****************************************************************************/

void PALCALLBACK
PaletteTimerProc( HWND hwnd, UINT msg, UINT idTimer, DWORD dwTime ) 
{

    TRACE(( TC_TW, TT_TW_PALETTE, "PaletteTimerProc: idTimer %u", idTimer ));

    /*
     *  Kill the timer
     */
    KillTimer( hwnd, idTimer );

    /*
     *  Force repaint of screen
     */
    InvalidateRect( hwnd, NULL, FALSE );
}
#endif

#if 0
/****************************************************************************
 *
 *  EnumWindowsProc
 *
 *  This function is called for all top level windows.
 *
 *  PARAMETERS:
 *
 ****************************************************************************/

BOOL CALLBACK
EnumWindowsProc( HWND hwnd, LPARAM lParam )
{

    /*
     *  Notify this window
     */
    SendMessage( hwnd, WM_SYSCOLORCHANGE, (WPARAM)0, (LPARAM)0 );

    /* 
     *  Keep enumerating
     */
    return( 1 );
}
#endif

#if 0
/****************************************************************************
 *
 *  UpdateSystemColors
 *
 *  This is routine determines if the static entries of the system
 *  palette have change and then sets or resets the system colors
 *  then notifies all top level windows.
 *
 *  PARAMETERS:
 *
 ****************************************************************************/

VOID 
UpdateSystemColors( HWND hwnd, HDC hdc, BOOL fSysColors )
{
       INT  i;
       BOOL fNotify = FALSE;
static BOOL f_I_ChangedSysColors = FALSE;

    TRACE((TC_TW,TT_TW_PALETTE, "VDTW: UpdateSystemColors - enter"));

    /*
     *  Only needed for palette devices
     */
    if ( vfPaletteDevice ) {
    
        /* 
         *  Have the static colors been set?
         */
        if ( (fSysColors == TRUE) && (f_I_ChangedSysColors == FALSE)) { 
    
            TRACE((TC_TW,TT_TW_PALETTE, "VDTW: UpdateSystemColors - use static colors"));
    
            /* 
             *  Make a note that it was me who changed the system colors
             */
            f_I_ChangedSysColors = TRUE;

            /*
             *  Save old colors
             */
            for ( i=0; i<NumSysColors; i++ ) {
                OldColors[i] = GetSysColor( SysPalIndex[i] );
            }
    
            /*
             *  Set system colors
             */
            SetSysColors( NumSysColors, SysPalIndex, MonoColors );
            /*
             *  Notify everyone else
             */
            fNotify = TRUE;
        }
        else if ((fSysColors == FALSE) && (f_I_ChangedSysColors == TRUE)) {
    
            TRACE((TC_TW,TT_TW_PALETTE, "VDTW: UpdateSystemColors - free static colors"));
    
            /*
             *  Reset note 
             */
            f_I_ChangedSysColors = FALSE;
    
            /*
             *  Set system colors
             */
            SetSysColors( NumSysColors, SysPalIndex, OldColors );
            /*
             *  Notify everyone else
             */
            fNotify = TRUE;
        }
    
        /*
         *  Do I need to notify all top level windows?
         */
        if ( fNotify ) {
            EnumWindows( (WNDENUMPROC)EnumWindowsProc, (LPARAM)0 );
            TRACE((TC_TW,TT_TW_PALETTE, "VDTW: UpdateSystemColors - notify others"));
        }
    }
#ifdef DEBUG
    else {
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: UpdateSystemColors - skipped, non-palette device"));
    }
#endif
}
#endif


/****************************************************************************
 *
 *  bUseStaticEntries
 *
 *  This is routine determines if the static entries of the system
 *  palette should be used or not
 *
 *  PARAMETERS:
 *
 ****************************************************************************/

BOOL
bUseStaticEntries( HWND hwnd, HDC hdc )
{
    USHORT i;
    BOOL   fUse = FALSE;

    /*
     *  Only needed for palette devices
     */
    if ( vfPaletteDevice ) {

        /*
         *  Use static entries if any static entries are not the defaults
         */
        for ( i = 0; i < 10; i++ ) {
    
            // First 10
            if ( (vpLogPalette->palPalEntry[i].peRed   != BASEPALETTE[i].peRed)   ||
                 (vpLogPalette->palPalEntry[i].peGreen != BASEPALETTE[i].peGreen) ||
                 (vpLogPalette->palPalEntry[i].peBlue  != BASEPALETTE[i].peBlue) ) {
    
                fUse = TRUE;
                break;
            }
    
            // Last 10
            if ( (vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peRed   != BASEPALETTE[i+10].peRed)   ||
                 (vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peGreen != BASEPALETTE[i+10].peGreen) ||
                 (vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peBlue  != BASEPALETTE[i+10].peBlue) ) {
    
                fUse = TRUE;
                break;
            }
        }
    
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: bUseStaticEntries - %s", (fUse ? "TRUE" : "FALSE")));
    }
    else {

        fUse = TRUE;
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: bUseStaticEntries - always use for non-palette devices"));
    }

    return( fUse );
}


/****************************************************************************
 *
 *  SetStaticColorUse
 *
 *  This is routine sets or resets the static color usage
 *
 *  PARAMETERS:
 *
 ****************************************************************************/

BOOL   
SetStaticColorUse( HWND hwnd, HDC hdc, BOOL bUseStaticColors )
{
    BOOL fChanged = FALSE;

    /*
     *  Only needed for palette devices
     */
    if ( vfPaletteDevice ) {
    
        /*
         *  Use static entries
         */
        if ( bUseStaticColors == TRUE ) {
    
            /*
             *  Are we not already using the static entries?
             */
            if( GetSystemPaletteUse( hdc ) == SYSPAL_STATIC ) {
                SetSystemPaletteUse( hdc, SYSPAL_NOSTATIC );
                UnrealizeObject( (HGDIOBJ)vhPalette );
                fChanged = TRUE;
                TRACE((TC_TW,TT_TW_PALETTE, "VDTW: SetStaticColorUse - SetSystemPaletteUse(SYSPAL_NOSTATIC)" ));
            }
        }
    
        /*
         *  Reset static entry use
         */
        else { 
    
            /*
             *  Are we currently using the static entries?
             */
            if( GetSystemPaletteUse( hdc ) == SYSPAL_NOSTATIC ) {
                SetSystemPaletteUse( hdc, SYSPAL_STATIC );
                UnrealizeObject( (HGDIOBJ)vhPalette );
                fChanged = TRUE;
                TRACE((TC_TW,TT_TW_PALETTE, "VDTW: SetStaticColorUse - SetSystemPaletteUse(SYSPAL_STATIC)" ));
            }
        }
    }
#ifdef DEBUG
    else {
        fChanged = FALSE;
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: SetStaticColorUse - skipped for non-palette device" ));
    }
#endif
    return( fChanged );
}


/****************************************************************************
 *
 *  w_TWCmdPaletteSet
 *
 *  This is the worker routine 
 *
 *  PARAMETERS:
 *     USHORT Options
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
 ****************************************************************************/

void 
w_TWCmdPaletteSet( HWND hwnd, 
                   HDC hdc, 
                   PTWPALETTEHEADER pTWPalHeader,
                   PTWPALETTEHEADERCACHE pTWPalHeaderCache )
{
    BYTE   cEntry;
    WORD   wControl;
    USHORT i;
    USHORT iStop;
    UINT   cColors;
    UINT   hCache;
    USHORT uscb;
    USHORT rc = 0;
    USHORT fRealizePalette;
    CHUNK_TYPE ChunkType;
    LPBYTE lpCacheArea;

    /*
     *  Do some up front cache work
     */
    if ( (pTWPalHeader->fType == TWPAL_TO_CACHE) ||
         (pTWPalHeader->fType == TWPAL_FROM_CACHE) ) {

        /*
         *  Get cache handle
         */
        hCache = pTWPalHeaderCache->hCache;

        /*
         *  Calculate chunk type
         */
        switch ( pTWPalHeaderCache->ChunkType ) {
            case TWPAL_CHUNK_128B : ChunkType = _128B; break;
            case TWPAL_CHUNK_512B : ChunkType = _512B; break;
            case TWPAL_CHUNK_2K :   ChunkType = _2K;   break;
            default : ASSERT( FALSE, pTWPalHeaderCache->ChunkType ); break;
        }
    }

    /*
     *  Retreive from cache
     */
    if ( pTWPalHeader->fType == TWPAL_FROM_CACHE ) {

        /*
         *  Get pointer to cache data
         */
        lpCacheArea = lpTWCacheRead( hCache, 
                                     ChunkType,
                                     (LPUINT) &uscb,
                                     0 );

        TRACE((TC_TW,TT_TW_PALETTE, 
               "w_TWCmdPaletteSet: lpTWCacheRead - hCache %u, ChunkType %u, Bytes %u, lpCacheArea %08x", 
                hCache, ChunkType, uscb, lpCacheArea ));
        ASSERT( lpCacheArea != NULL, 0 );

        /*
         *  Only copy if it was found
         */
        if ( lpCacheArea != NULL ) {

            /*
             *  Get control word
             */
            wControl = *((WORD *) lpCacheArea);
            lpCacheArea += sizeof(WORD);
            ASSERT( (wControl >> 8) == (BYTES_PER_ENTRY - 1), wControl ); 

            /*
             *  Extract entry count from control word
             */
            cEntry = (BYTE) (wControl & 0x00FF) + 1;
    
            /*
             *  Calculate end 
             */
            iStop = pTWPalHeader->iStart + (USHORT) cEntry;
        
            ASSERT( iStop < LOGICAL_PALETTE_SIZE, iStop );
        
            TRACE((TC_TW,TT_TW_PALETTE, "VDTW: w_TWCmdPaletteSet - retrieve cache entries %u up to %u", pTWPalHeader->iStart, iStop));
        
            /*
             *  Read into logical palette
             */
            for ( i = pTWPalHeader->iStart; ((i < iStop) && (i < LOGICAL_PALETTE_MAX)); i++ ) {

                /*
                 *  Copy data from cache to logical palette
                 */
                memcpy( &vpLogPalette->palPalEntry[i], lpCacheArea, 3 );
                lpCacheArea += 3;
            }
        }
    }
    else {
    
        /*
         *  Get count, and make 1 based (comes across zero based one)
         */
        GetNextTWCmdBytes( &cEntry, 1 );
        ++cEntry;
    
        /*
         *  Calculate end 
         */
        iStop = pTWPalHeader->iStart + (USHORT) cEntry;
    
        ASSERT( iStop < LOGICAL_PALETTE_SIZE, iStop );
    
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: w_TWCmdPaletteSet - update entries %u up to %u", pTWPalHeader->iStart, iStop));
    
        /*
         *  Read into logical palettes
         */
        for ( i = pTWPalHeader->iStart; ((i < iStop) && (i < LOGICAL_PALETTE_MAX)); i++ ) {
    
            /*
             *  Get the RGB values
             */
            GetNextTWCmdBytes( &vpLogPalette->palPalEntry[i], 3 );
    
            /*
             *  Fix magic school bus, animates palette with the color
             *  white(255,255,255) in index other than 255.  This cause
             *  SetBkColor all kinds of greif when doing a ROP with white
             *  even using PALETTEINDEX the ROP colapses to the first 
             *  white it finds.  Boo hiss ...
             *
             *  Now it happens on MSACCESS 2.0 with grey (128,128,128) ...
             *
             *  Oh yeah, this only happens on palette devices ... wkp
             */
            if ( vfPaletteDevice == TRUE ) {
                 if ( (vpLogPalette->palPalEntry[i].peRed   == 0xff) &&
                      (vpLogPalette->palPalEntry[i].peGreen == 0xff) && 
                      (vpLogPalette->palPalEntry[i].peBlue  == 0xff) ) {

                     vpLogPalette->palPalEntry[i].peBlue = 0xfe;
                     TRACE((TC_TW,TT_TW_PROTOCOLDATA, 
                            "w_TWCmdPaletteSet - oh yeah, give me a white not in index 255" ));
                 }
                 else if ( (i > 9) && 
                           (i < LOGICAL_PALETTE_STATIC) &&
                           (vpLogPalette->palPalEntry[i].peRed   == 0x80) &&
                           (vpLogPalette->palPalEntry[i].peGreen == 0x80) && 
                           (vpLogPalette->palPalEntry[i].peBlue  == 0x80) ) {
                     vpLogPalette->palPalEntry[i].peBlue = 0x7f;
                     TRACE((TC_TW,TT_TW_PROTOCOLDATA, 
                            "w_TWCmdPaletteSet - oh yeah, give me a gray not in index 248" ));
                 }
            }

            TRACE((TC_TW,TT_TW_PROTOCOLDATA, 
                   "VDTW: w_TWCmdPaletteSet - Pal[%03u] = RGB(%u,%u,%u), RGB(%02x,%02x,%02x)", 
                   i,
                   vpLogPalette->palPalEntry[i].peRed,
                   vpLogPalette->palPalEntry[i].peGreen,
                   vpLogPalette->palPalEntry[i].peBlue,
                   vpLogPalette->palPalEntry[i].peRed,
                   vpLogPalette->palPalEntry[i].peGreen,
                   vpLogPalette->palPalEntry[i].peBlue ));
        }

        /*
         *  Store to cache
         */
        if ( pTWPalHeader->fType == TWPAL_TO_CACHE ) {

            /*
             *  Get write pointer to cache object
             */
            lpCacheArea = lpTWCacheWrite( hCache,
                                          ChunkType, 
                                          ((USHORT) cEntry * BYTES_PER_ENTRY) + sizeof(WORD), 
                                          (ChunkType == _2K ? hCache : 0) );
            ASSERT( lpCacheArea != NULL, 0 );

            /*
             *  Only write if valid
             */
            if ( lpCacheArea != NULL ) {
    
                /*
                 *  Build control word
                 */
                wControl = ((BYTES_PER_ENTRY - 1) << 8) | (cEntry & 0x00FF) - 1;
    
                /*
                 *  Store control word
                 */
                *((WORD *) lpCacheArea) = wControl;
                lpCacheArea += sizeof(WORD);

                /*
                 *  Calculate end 
                 */
                iStop = pTWPalHeader->iStart + (USHORT) cEntry;
            
                ASSERT( iStop < LOGICAL_PALETTE_SIZE, iStop );
            
                TRACE((TC_TW,TT_TW_PALETTE, "VDTW: w_TWCmdPaletteSet - update cache entries %u up to %u", pTWPalHeader->iStart, iStop));
            
                /*
                 *  Store from logical palette
                 */
                for ( i = pTWPalHeader->iStart; ((i < iStop) && (i < LOGICAL_PALETTE_MAX)); i++ ) {
    
                    /*
                     *  Copy data from cache to logical palette
                     */
                    memcpy( lpCacheArea, &vpLogPalette->palPalEntry[i], 3 );
                    lpCacheArea += 3;
                }
    
                /*
                 *  Complete cache write
                 */
                finishedTWCacheWrite( ((USHORT) cEntry * 3) + sizeof(WORD) );
            }
        }
    }

    /*
     *  Realize flag
     */
    if ( vfPaletteDevice ) {
        fRealizePalette = (/*hwnd == GetActiveWindow() */ TRUE ? TWREALIZEPALETTE_SET_FG :
                                                       TWREALIZEPALETTE_SET_BG);
    }
    else {
        fRealizePalette = TWREALIZEPALETTE_SET_FG;
    }

    /*
     *  Realize the new palette
     */
    TWRealizePalette( hwnd, hdc, &cColors, fRealizePalette );

    TWCmdReturn( !rc ); // return to NewNTCommand or ResumeNTCommand
}


/****************************************************************************
 ****
 ****   External routines follow
 ****
 ****************************************************************************/


/****************************************************************************
 *
 *  TWCreateDefaultPalette
 *
 *  This is routine allocates and initializes the default logical palettes
 *
 *  PARAMETERS:
 *
 ****************************************************************************/

VOID 
TWCreateDefaultPalette( HWND hwnd, HDC hdc )
{
    UINT  i;
    BYTE  jRed;
    BYTE  jGre;
    BYTE  jBlu;
    UINT  cColors;
    USHORT fRealizePalette;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWCreateDefaultPalette - enter" ));

    /*
     *  Allocate and initialize logical palette if needed
     */
    if ( vpLogPalette == NULL ) {

        /*
         *  Allocate logical palette with static entries
         */
        vhLogPalette = GlobalAlloc( GMEM_MOVEABLE, 
                                sizeof(LOGPALETTE) +
                                sizeof(PALETTEENTRY) * 
                                LOGICAL_PALETTE_SIZE );

        TRACE((TC_TW,TT_TW_PALETTE,
               "VDTW: TWCreateDefaultPalette - GlobalAlloc vhLogPalette %lx", (ULONG)vhLogPalette ));

        /*
         *  Get pointer
         */
        vpLogPalette = (LOGPALETTE *) GlobalLock( vhLogPalette );

        TRACE((TC_TW,TT_TW_PALETTE,
               "VDTW: TWCreateDefaultPalette - GlobalLock vpLogPalette %lx", (ULONG)vpLogPalette ));

        /*
         *  Initialize
         */
        vpLogPalette->palNumEntries = LOGICAL_PALETTE_SIZE;
        vpLogPalette->palVersion    = 0x300;

    }
    ASSERT( vpLogPalette != NULL, 0 );

    /*
     *  Assign starting RGB values
     */
    jRed = jGre = jBlu = 0;

    /*
     *  Fill in logical palettes
     */
    for ( i = 0; i < LOGICAL_PALETTE_SIZE; i++ ) {

        /*
         *  Assign logical palette with Static values
         */
        vpLogPalette->palPalEntry[i].peRed   = jRed;
        vpLogPalette->palPalEntry[i].peGreen = jGre;
        vpLogPalette->palPalEntry[i].peBlue  = jBlu;
        vpLogPalette->palPalEntry[i].peFlags = PC_NOCOLLAPSE;

        /*
         *  Generate next RGB Value
         */
        if (!((jRed += 8) & 0x3F))
            if (!((jGre += 8) & 0x3F))
                jBlu += 16;

    }

    /*
     *  Fill in Windows Reserved Colors from the WIN 3.0 DDK
     *  The Window Manager reserved the first and last 10 colors for
     *  painting windows borders and for non-palette managed applications.
     */
    for (i = 0; i < 10; i++)
    {

        // First 10
        vpLogPalette->palPalEntry[i].peRed   = BASEPALETTE[i].peRed;
        vpLogPalette->palPalEntry[i].peGreen = BASEPALETTE[i].peGreen;
        vpLogPalette->palPalEntry[i].peBlue  = BASEPALETTE[i].peBlue;
        vpLogPalette->palPalEntry[i].peFlags = BASEPALETTE[i].peFlags;

        // Last 10
        vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peRed   = BASEPALETTE[i+10].peRed;
        vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peGreen = BASEPALETTE[i+10].peGreen;
        vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peBlue  = BASEPALETTE[i+10].peBlue;
        vpLogPalette->palPalEntry[i+LOGICAL_PALETTE_STATIC].peFlags = PC_NOCOLLAPSE;
    }

    /*
     *  Is this a palette managed device?
     */
    if (/* (GetDeviceCaps( hdc, RASTERCAPS ) & RC_PALETTE)*/ TRUE ) {
        vfPaletteDevice = TRUE;
        fRealizePalette = (/*hwnd == GetActiveWindow()*/ TRUE ? TWREALIZEPALETTE_INIT_FG :
                                                       TWREALIZEPALETTE_INIT_BG);
    }
    else {
        vfPaletteDevice = FALSE;
        fRealizePalette = TWREALIZEPALETTE_INIT_FG;
        SetSystemPaletteUse( hdc, SYSPAL_NOSTATIC );
    }

    /*
     *  Logical palette is now initialized
     */
    vfPaletteAvailable = TRUE;

    /*
     *  Update system palette with our logical palette
     */
    TWRealizePalette( hwnd, hdc, &cColors, fRealizePalette );

#ifdef WIN16
    /*
     *  Create timer hook instance
     */
    lpfnPaletteTimerProc = (TIMERPROC) MakeProcInstance( (FARPROC) PaletteTimerProc, ghInstance );
    ASSERT( lpfnPaletteTimerProc != NULL, 0 );
#endif

    /*
     *  Note that we are currently using the default palette
     */
    vfUsingDefaultPalette = TRUE;

    /*
     *  Is the default system palette weird?
     */
#ifdef WIN32
    if ( (vfPaletteDevice == TRUE) &&
         (ValidateSystemPalette( hwnd, hdc, VALIDATE_SYSTEM_DEFAULT_STATIC ) == FALSE) ) {
        vfWeirdSystemPalette = TRUE;
    }
    else
#endif
    {
        vfWeirdSystemPalette = FALSE;
    }

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWCreateDefaultPalette - exit" ));
}


/****************************************************************************
 *
 *  TWUseSystemPalette
 *
 *  This is routine resets the system default palette
 *
 *  PARAMETERS:
 *
 ****************************************************************************/

VOID
TWUseSystemPalette( HWND hwnd, HDC hdc )
{
    BOOL fSysColorsChanged;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWUseSystemPalette - enter" ));

    /*
     *  Static colors in use by us?
     */
    fSysColorsChanged = SetStaticColorUse( hwnd, hdc, FALSE );

    /*
     *  Restore system default palette
     */
    SelectPalette( hdc, (HPALETTE) GetStockObject( DEFAULT_PALETTE), IsPluginWindow(hwnd) );
    RealizePalette( hdc );

    /*
     *  Restore system default palette to compatible dc
     */
    if ( compatDC ) {
        SelectPalette( compatDC, (HPALETTE) GetStockObject( DEFAULT_PALETTE), IsPluginWindow(hwnd) );
        RealizePalette( compatDC );
    }

#ifndef RISCOS
    /*
     *  Reset system colors, etc.
     */
    if ( fSysColorsChanged ) {
        UpdateSystemColors( hwnd, hdc, FALSE );
    }
#endif
    
    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWUseSystemPalette - exit" ));
}


/****************************************************************************
 *
 *  TWDeleteDefaultPalette
 *
 *  This is routine frees the default logical palettes
 *
 *  PARAMETERS:
 *
 ****************************************************************************/

VOID
TWDeleteDefaultPalette( HWND hwnd, HDC hdc )
{
    BOOL    fSysColorsChanged;
    HGLOBAL hretcode;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWDeleteDefaultPalette - enter" ));

    /*
     *  Static colors in use by us?
     */
    fSysColorsChanged = SetStaticColorUse( hwnd, hdc, FALSE );

    /*
     *  Restore default palette
     */
    SelectPalette( hdc, (HPALETTE) GetStockObject( DEFAULT_PALETTE), IsPluginWindow(hwnd) );
#ifndef RISCOS
    RealizePalette( hdc );
#endif

    /*
     *  Restore system default palette to compatible dc
     */
    if ( compatDC ) {
        SelectPalette( compatDC, (HPALETTE) GetStockObject( DEFAULT_PALETTE), IsPluginWindow(hwnd) );
        RealizePalette( compatDC );
    }

    /*
     *  Free any existing created palette
     */
    if ( vhPalette != NULL ) {
        TRACE((TC_TW,TT_TW_PALETTE,
               "VDTW: TWDeleteDefaultPalette - DeleteObject vhPalette %lx", (ULONG)vhPalette ));
        DeleteObject( (HGDIOBJ)vhPalette );
        vhPalette = NULL;
    }

    /*
     *  Free static palette if allocated
     */
    if ( vhLogPalette ) {
       
        if ( vpLogPalette ) {
            TRACE((TC_TW,TT_TW_PALETTE,
                   "VDTW: TWDeleteDefaultPalette - GlobalUnlock vhLogPalette %lx", (ULONG)vhLogPalette ));
            GlobalUnlock( vhLogPalette );
            vpLogPalette = NULL;
        }

        TRACE((TC_TW,TT_TW_PALETTE,
               "VDTW: TWDeleteDefaultPalette - GlobalFree vhLogPalette %lx", (ULONG)vhLogPalette ));
        hretcode = GlobalFree( (HGLOBAL) vhLogPalette );
        ASSERT(!hretcode, 0);
        vhLogPalette = NULL;
    }

#ifndef RISCOS
    /*
     *  Reset system colors, etc.
     */
    UpdateSystemColors( hwnd, hdc, FALSE );

    /*
     *  Kill palette timer
     */
    KillTimer( hwnd, PALETTE_TIMER_ID );
#endif
    
#ifdef WIN16
    /*
     *  If timer proc instance exists then destroy it
     */
    if ( lpfnPaletteTimerProc != NULL ) {
        FreeProcInstance( lpfnPaletteTimerProc );
        lpfnPaletteTimerProc = NULL;
    }
#endif

    /*
     *  Logical Palette is no longer available
     */
    vfPaletteAvailable = FALSE;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWDeleteDefaultPalette - exit" ));
}


/****************************************************************************
 *
 *  TWRealizePalette
 *
 *  Realizes logical palette into system palette, taking into account 
 *  static colors and foreground and background considerations.
 *
 *  PARAMETERS:
 *
 ****************************************************************************/

USHORT
TWRealizePalette( HWND hwnd, HDC hdc, UINT * pcColors, USHORT idRealizePalette )
{
    UINT i;
    BYTE peFlags = 0;
    BOOL fSysColorStatic;
    BOOL fSysColorsChanged = FALSE;
    BOOL fDontChangeStatic = FALSE;
static DWORD cUpdateColors = 0;
static BOOL  fSystemColorsUsed = FALSE;
static BOOL  fWeTookStaticColors = FALSE;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, 
           "VDTW: TWRealizePalette - %s", pszIdRealize[idRealizePalette] ));

    /*     
     *  Don't bother when palette not initialized
     */
    if ( !vfPaletteAvailable ) {
        *pcColors = 0;
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Do we need to use any of the static entries?
     */
    if ( (vfWeirdSystemPalette == TRUE) &&
         (idRealizePalette != TWREALIZEPALETTE_FOCUS) &&
         (idRealizePalette != TWREALIZEPALETTE_BG) &&
         (idRealizePalette != TWREALIZEPALETTE_SET_BG) ) {

        /*
         *  Note for set
         */
        fDontChangeStatic   = TRUE;
        fWeTookStaticColors = TRUE;

        /*
         *  Set our use of static colors
         */
        (void) SetStaticColorUse( hwnd, hdc, TRUE );

        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - using static entries"));
    }
    else if ( ((idRealizePalette == TWREALIZEPALETTE_BG) || 
               (idRealizePalette == TWREALIZEPALETTE_FOCUS) ) && 
              (fWeTookStaticColors == TRUE) ) {

        /*
         *  Note for reset
         */
        fWeTookStaticColors = FALSE;

        /*
         *  Reset our use of static colors
         */
        (void) SetStaticColorUse( hwnd, hdc, FALSE );

        /*
         *  Make this a background realization
         */
        idRealizePalette= TWREALIZEPALETTE_SET_BG;

        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - not using static colors"));
    }

    /*
     *  Possibility of static color changes
     */
    if ( (idRealizePalette == TWREALIZEPALETTE_FG)       || 
         (idRealizePalette == TWREALIZEPALETTE_FOCUS)    ||
         (idRealizePalette == TWREALIZEPALETTE_SET_FG)   ||
         (idRealizePalette == TWREALIZEPALETTE_SET_BG)   ||
         (idRealizePalette == TWREALIZEPALETTE_INIT_FG) ) {

        /*
         *  Only use static color while in foreground
         */
        if ( fDontChangeStatic || fWeTookStaticColors ) {

            /*
             *  Bad shit happening
             */
            peFlags = PC_NOCOLLAPSE;
            TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - using static entries because of fucked up system palette!"));
        }
        else if ( (idRealizePalette != TWREALIZEPALETTE_FOCUS) &&
                  (idRealizePalette != TWREALIZEPALETTE_SET_BG) &&
                  (bUseStaticEntries( hwnd, hdc ) == TRUE) ) {

            /*
             *  Set the dc mode to using static entries in system palette
             */
            fSysColorStatic = TRUE;
            fSysColorsChanged = SetStaticColorUse( hwnd, hdc, fSysColorStatic );
    
            /*
             *  Set no collapse bit for low static entries
             */
            peFlags = PC_NOCOLLAPSE;
            TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - using static entries"));
        }
        else if ( (vfWeirdSystemPalette == FALSE) ||
                 ((vfWeirdSystemPalette == TRUE) && 
                  (fWeTookStaticColors == TRUE)) ) {
    
            /*
             *  Set the dc mode to restore static entries in system palette
             */
            fSysColorStatic = FALSE;
            fSysColorsChanged = SetStaticColorUse( hwnd, hdc, fSysColorStatic );
    
            /*
             *  Reset no collapse bit for low static entries
             */
            TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - NOT using static entries"));
        }
    }

    /*
     *  Logical palette change messages 
     */
    if ( (idRealizePalette == TWREALIZEPALETTE_SET_FG)  ||
         (idRealizePalette == TWREALIZEPALETTE_SET_BG)  ||
         (idRealizePalette == TWREALIZEPALETTE_INIT_FG) ||
         (idRealizePalette == TWREALIZEPALETTE_INIT_BG) ) {

        HPALETTE hNewPalette;
        HPALETTE hOldPalette = vhPalette;

        /*
         *  Apply current flag to low static entries, entry 0 is never changed.
         */
        for ( i = 1; i < 10; i++ ) {
            vpLogPalette->palPalEntry[i].peFlags = peFlags;
        }
    
        /*
         *  Create logical palette (local)
         */
        hNewPalette = CreatePalette( vpLogPalette );
        TRACE((TC_TW,TT_TW_PALETTE,
               "VDTW: TWRealizePalette - CreatePalette hNewPalette %lx", (ULONG)hNewPalette ));

        /*
         *  Select palette into our dc 
         */
        SelectPalette( hdc, (vhPalette = hNewPalette), IsPluginWindow(hwnd) );
        *pcColors = RealizePalette( hdc );
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - Select New Palette"));
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - %u colors realized", *pcColors ));
    
        /*
         *  Select palette to compatible dc
         */
        if ( compatDC ) {
            SelectPalette( compatDC, vhPalette, IsPluginWindow(hwnd) );
            RealizePalette( compatDC );
        }
    
        /*
         *  Free any existing palette (global)
         */
        if ( hOldPalette != NULL ) {
            TRACE((TC_TW,TT_TW_PALETTE,
                   "VDTW: TWRealizePalette - DeleteObject hOldPalette %lx", (ULONG)hOldPalette ));
            DeleteObject( (HGDIOBJ)hOldPalette );
        }

        /*
         *  Note that we are no longer using the default palette
         */
        vfUsingDefaultPalette = FALSE;


        /*
         *  Validate system palette
         */
//      ASSERT( ValidateSystemPalette( hwnd, hdc, VALIDATE_SYSTEM_DEFAULT_ALL ), 0 );
    }
    else {

        /*
         *  Realize palette
         */
        SelectPalette( hdc, vhPalette, IsPluginWindow(hwnd) );
        *pcColors = RealizePalette( hdc );

        /*
         *  Select palette to compatible dc
         */
        if ( compatDC ) {
            SelectPalette( compatDC, vhPalette, IsPluginWindow(hwnd) );
            RealizePalette( compatDC );
        }
    
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - Select Old Palette"));
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - %u colors realized", *pcColors ));
    }

    /*
     *  Map logical palette into system palette
     */

    /*
     *  Did any logical palette entries get remapped to the system palette?
     */
    if ( (*pcColors != 0) || 
         ((idRealizePalette == TWREALIZEPALETTE_FG) && (cUpdateColors != 0) ||
         (vfPaletteDevice == FALSE) ) ) {

        /*
         *  Foreground, background, etc.
         */
        if ( (idRealizePalette == TWREALIZEPALETTE_INIT_FG) ) {
            /* do nothing */;
        }
        else if ( (idRealizePalette == TWREALIZEPALETTE_INIT_BG) ) {
            /* do nothing */;
        }
        else if ( idRealizePalette == TWREALIZEPALETTE_FG ) {
            if ( (vfPaletteDevice == TRUE) && 
                 ((vfUsingDefaultPalette == FALSE) || 
                  (fSystemColorsUsed == TRUE)) ) {
//                InvalidateRect( hwnd, NULL, FALSE );
                cUpdateColors = 0;
                fSystemColorsUsed = FALSE;
            }
        }
        else if ( idRealizePalette == TWREALIZEPALETTE_BG ) {
            if ( (vfPaletteDevice == TRUE) && 
                 (vfUsingDefaultPalette == FALSE) ) {
                if ( !IsPluginWindow(hwnd) && (++cUpdateColors < MAX_UPDATE_COLORS) ) {
//                    UpdateColors( hdc );
                }
                else {
//                    InvalidateRect( hwnd, NULL, FALSE );
                    cUpdateColors = 0;
                }
            }
            else if( (vfPaletteDevice == TRUE) && 
                     (GetSystemPaletteUse( hdc ) == SYSPAL_NOSTATIC) ) {
                fSystemColorsUsed = TRUE;
            }
        }
        else if ( (idRealizePalette == TWREALIZEPALETTE_SET_FG) ) {
            if ( !vfPaletteDevice ) {

                DWORD dwCurrentTime = 0; // GetCurrentTime();

#ifndef RISCOS
                /*
                 *  Decide wether to invalidate now or later
                 */
                if ( dwCurrentTime < (vdwPaletteTime + PALETTE_TIMER_TIMEOUT) ) {
    
                    if ( SetTimer( hwnd, 
                                   PALETTE_TIMER_ID, 
                                   PALETTE_TIMER_TIMEOUT,
#ifdef WIN16
                              (TIMERPROC) lpfnPaletteTimerProc ) ==
#else
                              (TIMERPROC) PaletteTimerProc ) ==
#endif
                         0 ) {

                        TRACE((TC_TW,TT_TW_PALETTE, 
                               "VDTW: TWRealizePalette - SetTimer failed, InvalidateRect"));

                        InvalidateRect( hwnd, NULL, FALSE );
                    }
                    vdwPaletteTime = dwCurrentTime;
                }
                else
#endif
		{
                    vdwPaletteTime = dwCurrentTime;
//                    InvalidateRect( hwnd, NULL, FALSE );
                    TRACE((TC_TW,TT_TW_PALETTE, 
                           "VDTW: TWRealizePalette - Timeout window expired, InvalidateRect"));
                }
            }
        }
        else if ( (idRealizePalette == TWREALIZEPALETTE_SET_BG) ) {
            /* do nothing */;
        }
        else if ( idRealizePalette == TWREALIZEPALETTE_FOCUS ) {
            if ( (vfPaletteDevice == TRUE) && (vfUsingDefaultPalette == FALSE) ) {
//                UpdateColors( hdc );
            }
        }
    }

#ifndef RISCOS
    /*
     *  Update the system colors
     */
    if ( fSysColorsChanged == TRUE ) {
        UpdateSystemColors( hwnd, hdc, fSysColorStatic );
    }
#endif

    return( CLIENT_STATUS_SUCCESS );
}
#endif


/****************************************************************************
 *
 *  TWCmdPalette (TWCMD_PALETTE service routine)
 *
 *  This routine is called to service the DrvSetPalette thinwire display
 *  driver function and for palette initialization.
 *
 *  PARAMETERS:
 *     hwnd (input)
 *        window handle
 *     hdc (input)
 *        device context
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
 ****************************************************************************/

void
TWCmdPalette( HWND hwnd, HDC hdc )
{
    USHORT rc = 0;
#ifdef DOS
    ASSERT( FALSE, 0 );
#else
    TWPALETTEHEADER TWPalHeader;
    TWPALETTEHEADERCACHE TWPalHeaderCache;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWCmdPalette: enter" ));

    /*
     *  Get header and length
     */
    GetNextTWCmdBytes( &TWPalHeader, sizeof_TWPALETTEHEADER );

    TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWCmdPalette - TWPalHeader.fType %x", TWPalHeader.fType));

    /*
     *  Command subcode
     */
    switch ( TWPalHeader.fType ) {

        case TWPAL_TO_CACHE :

            /*
             *  Read cache header
             */
            GetNextTWCmdBytes( &TWPalHeaderCache, sizeof_TWPALETTEHEADERCACHE );
            break;

        case TWPAL_FROM_CACHE :

            /*
             *  Read cache header
             */
            GetNextTWCmdBytes( &TWPalHeaderCache, sizeof_TWPALETTEHEADERCACHE );
            break;

        case TWPAL_NOT_CACHED :

            /*
             *  Got all the data we need
             */
            break;

        default :

            ASSERT( !(TWPalHeader.fType > TWPAL_NOT_CACHED), TWPalHeader.fType );
            goto done;
    }

    /*
     *  Pass command on to worker
     */
    w_TWCmdPaletteSet( hwnd, hdc, &TWPalHeader, &TWPalHeaderCache );

done:

#endif

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWCmdPalette: exit" ));
    TWCmdReturn( !rc ); // return to NewNTCommand or ResumeNTCommand
}

/****************************************************************************
 *
 * IsPluginWindow
 *
 * Determines whether we are running in a plugin window or not
 *
 *  PARAMETERS:
 *     hwnd (input)
 *        window handle
 *
 *  RETURN: 
 *     TRUE  - plugin window present
 *     FALSE - standalone window
 ****************************************************************************/
BOOL    IsPluginWindow(HWND hWnd)
{
#if defined( DOS ) || defined(RISCOS)
   return(FALSE);
#else
   PWFEINSTANCE pInstanceData = (PWFEINSTANCE)GetWindowLong( hWnd, GWL_INSTANCEDATA );

   if(pInstanceData == NULL)
      return(FALSE);

   if(pInstanceData->hWndPlugin)
      return(TRUE);
   else
      return(FALSE);
#endif

}

