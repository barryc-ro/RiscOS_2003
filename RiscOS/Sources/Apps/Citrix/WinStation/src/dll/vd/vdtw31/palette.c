
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
*  
*     Rev 1.49   Jan 19 1998 16:07:26   briang
*  Include twi_en.h
*
*     Rev 1.48   Jan 14 1998 17:00:38   briang
*  TWI Integration
*
*     Rev 1.48   08 Oct 1997 13:32:00   AnatoliyP
*  TWI integration started
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
#include "mem.h"

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

#include "twi_en.h"

#ifdef TWI_INTERFACE_ENABLED

#include "apdata1.h"    // TWI common data, ref only

#endif  //TWI_INTERFACE_ENABLED


/*
 *  Currently only valid for windows client's
 */

/*=============================================================================
==   Typedefs and Structures
=============================================================================*/


/*=============================================================================
==   Functions Defined
=============================================================================*/

BOOL    bUseStaticEntries( HWND, HDC );
BOOL    SetStaticColorUse( HWND, HDC, BOOL );
USHORT  TWRealizePalette( HWND, HDC, UINT *, USHORT );
VOID    UpdateSystemColors( HWND, HDC, BOOL );


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

/*=============================================================================
 ==   Data
 ============================================================================*/

extern HDC       compatDC;
//extern HINSTANCE ghInstance;

HPALETTE     vhPalette             = NULL;
HGLOBAL      vhLogPalette          = NULL;
LOGPALETTE * vpLogPalette          = NULL;
BOOL         vfPaletteDevice       = TRUE;
BOOL         vfUsingDefaultPalette = TRUE;
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

static void 
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
    fRealizePalette = TWREALIZEPALETTE_SET_FG;

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
    }

    fRealizePalette = TWREALIZEPALETTE_INIT_FG;
    
    /*
     *  Logical palette is now initialized
     */
    vfPaletteAvailable = TRUE;

    /*
     *  Update system palette with our logical palette
     */
    TWRealizePalette( hwnd, hdc, &cColors, fRealizePalette );

    /*
     *  Note that we are currently using the default palette
     */
    vfUsingDefaultPalette = TRUE;

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
    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWUseSystemPalette - enter" ));
    /*
     *  Restore system default palette
     */
    SelectPalette( hdc, (HPALETTE) GetStockObject( DEFAULT_PALETTE), TRUE );
    RealizePalette( hdc );

    /*
     *  Restore system default palette to compatible dc
     */
    if ( compatDC ) {
        SelectPalette( compatDC, (HPALETTE) GetStockObject( DEFAULT_PALETTE), TRUE );
        RealizePalette( compatDC );
    }

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
    HGLOBAL hretcode;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWDeleteDefaultPalette - enter" ));

    /*
     *  Restore default palette
     */
    SelectPalette( hdc, (HPALETTE) GetStockObject( DEFAULT_PALETTE), TRUE );
#ifndef RISCOS
    RealizePalette( hdc );
#endif

    /*
     *  Restore system default palette to compatible dc
     */
    if ( compatDC ) {
        SelectPalette( compatDC, (HPALETTE) GetStockObject( DEFAULT_PALETTE), TRUE );
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
        SelectPalette( hdc, (vhPalette = hNewPalette), TRUE );
        *pcColors = RealizePalette( hdc );
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - Select New Palette"));
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - %u colors realized", *pcColors ));

        /*
         *  Select palette to compatible dc
         */
        if ( compatDC ) {
            SelectPalette( compatDC, vhPalette, TRUE );
            RealizePalette( compatDC );
        }


#ifdef TWI_INTERFACE_ENABLED

        if ( ShadowDC ) {
            SelectPalette( ShadowDC, vhPalette, IsPluginWindow(hwnd) );
            RealizePalette( ShadowDC );
        }

        if ( ShadowDC2 ) {
            SelectPalette( ShadowDC2, vhPalette, IsPluginWindow(hwnd) );
            RealizePalette( ShadowDC2 );
        }

#endif  //TWI_INTERFACE_ENABLED


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
    }
    else {

        /*
         *  Realize palette
         */
        SelectPalette( hdc, vhPalette, TRUE );
        *pcColors = RealizePalette( hdc );

        /*
         *  Select palette to compatible dc
         */
        if ( compatDC ) {
            SelectPalette( compatDC, vhPalette, TRUE );
            RealizePalette( compatDC );
        }
    
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - Select Old Palette"));
        TRACE((TC_TW,TT_TW_PALETTE, "VDTW: TWRealizePalette - %u colors realized", *pcColors ));
    }

    return( CLIENT_STATUS_SUCCESS );
}


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

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_PALETTE, "VDTW: TWCmdPalette: exit" ));
    TWCmdReturn( !rc ); // return to NewNTCommand or ResumeNTCommand
}

