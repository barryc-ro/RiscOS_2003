
/*******************************************************************************
*
*   WFMISC.C
*
*   Thin Wire Windows - Misc Code
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Jeff Krantz (jeffk)
*
*   $Log$
*   Revision 1.1  1998/01/19 19:13:06  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.15   15 Apr 1997 18:17:04   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.14   08 May 1996 14:54:36   jeffm
*  update
*  
*     Rev 1.13   30 Jan 1996 18:12:56   kurtp
*  update
*  
*     Rev 1.12   27 Jan 1996 11:10:48   kurtp
*  update
*  
*     Rev 1.11   03 Jan 1996 13:34:16   kurtp
*  update
*  
*******************************************************************************/

#include "wfglobal.h"
#include "twtype.h"
#include "../../../inc/clib.h"
#include <stdlib.h>

extern ULONG   LargeCacheSize;   //jkscaffold bugbug

static int timesinit = 0;      //should only init once if dont destroy

static HGLOBAL  hstatic_buffer;      //holds handle for static buffer

extern HDC vhdc;
extern HWND vhWnd;
extern COLOR_CAPS vColor;


//  alloc and clean up routines
VOID TWCreateDefaultPalette( HWND, HDC );
VOID TWDeleteDefaultPalette( HWND, HDC );
BOOL Destroy16Color( VOID );
BOOL Destroy256Color( VOID );

//  palette routines
extern USHORT TWRealizePalette( HWND, HDC, UINT *, USHORT );

//jkbetter - if we do a create and destroy device context state of the windows
//client some of the resources may make more sense to destroy when the dc goes
//away and recreate them when the dc is recreated
//assuming this happens infrequently like when the focus is given and taken
//
//ThinInitOnce called once when app started to initialize
//and create all thinwire persistant resources
//returns FALSE if error
//
//chunks_2K is the number of 2K chunks desired in the large cache

BOOL ThinInitOnce(UINT chunks_2K)
{

   TRACE((TC_TW,TT_TW_CONNECT, "ThinInitOnce: entry"));

   ASSERT(!timesinit,0);
   timesinit++;

   compatDC = NULL;

   chunks_2K = (UINT) (LargeCacheSize >> 11);      //jkscaffold bugbug

   //jk256 - THINPAL MUST BE DEFINED TO BUILD THE 256 color driver correctly

   //if we decide to get smart on how we initialize the colorref arrays
   //then we may need to dynamically set DIB_ColorMode
#ifdef THINPAL
   DIB_ColorMode = DIB_PAL_COLORS;
#else
   DIB_ColorMode = DIB_RGB_COLORS;     //this should never happen!!!!!!!
#endif


   //initialize dcstate.  Note that the brush handle is initialized to NULL
   //because the host is responsible for understanding when there is
   //an already realized brush and when there is not...
   //we init the rest to the dc init default
   //note that the host tracks when a new brush needs to be realized into the
   //device context so we don't need to initialize the dc with a brush
   //on a connect but we do if we recreate the dc during a connected session
   //
   //
   //NOTE: if we were to dynamically remove and recreate the DC constantly
   //       then doing all of this state bookkeeping may not make sense
   //
   //jk256 -
   dcstate.hbr = NULL;           //maintained by create_brush
   dcstate.bkmode = OPAQUE;      //only meaningful for text and lines
                                 //assume not meaningful for any source
                                 //or no source bitblt.  All 2 color brushes
                                 //will be realized as 16 color brushes
                                 //so bkmode wont matter
   dcstate.bkcolor = twsolidcolor[15];   //color only needs to match our array
   dcstate.txtcolor = twsolidcolor[0];   //color meaningful for text, lines,
                                       //and bitblt of 2 color bitmaps
   dcstate.rop2 = R2_COPYPEN;    //rop2 meaningful for almost all operation
   dcstate.xBrushOrg = 0;        //meaningful for brushes only
   dcstate.yBrushOrg = 0;        //   "


   //allocate static buffer

   hstatic_buffer = GlobalAlloc(GMEM_MOVEABLE, STATIC_BUFFER_SIZE);
   ASSERT(hstatic_buffer, 0);
   lpstatic_buffer = (LPDWORD) GlobalLock(hstatic_buffer);
   ASSERT(lpstatic_buffer, 0);
   //ASSERT(chunks_2K == 0, 0);       //jkstage
   return(TWCache_Init(chunks_2K));
}





//BOOL ThinDestroyOnce called to delete all thinwire resource
//when the app is going away
//!!!!!!THE DEVICE CONTEXT MUST ALREADY BE DELETED!!!!!!!!!!

BOOL ThinDestroyOnce()
{
   BOOL bretcode;
   HGLOBAL hretcode;

   TRACE((TC_TW,TT_TW_CONNECT, "ThinDestroyOnce: entry"));
   ASSERT( timesinit == 1, 0); //to destroy there must be an init

   timesinit--;

   //deal with cleanup
   if ( vColor == Color_Cap_256 ) {
      Destroy256Color();
   }
   else {
      Destroy16Color();
   }


   //get rid of static buffer
   GlobalUnlock(hstatic_buffer);
   hretcode = GlobalFree(hstatic_buffer);
   ASSERT(!hretcode, 0);

   //destroy the compatible DC if it was created
   if (compatDC != NULL) {
      bretcode = DeleteDC(compatDC);
      ASSERT(bretcode,0);
   }

   return(TWCache_Destroy());
}

//see protocol document for description of 3BYTES
void  Input3Bytes(LPWORD lpword1, LPWORD lpword2)
{
   BYTE  byte1[3];
   WORD  word1,word2;
   GetNextTWCmdBytes((LPBYTE) &(byte1[0]),3);

   word1 = (WORD) byte1[0];
   word2 = (WORD) byte1[1];

   word1 |= (WORD) ( ((WORD) (byte1[2] & 0xf0)) << 4);
   word2 |= (WORD) ( ((WORD) (byte1[2] & 0x0f)) << 8);

   *lpword1 = word1;
   *lpword2 = word2;

   return;
}

//see protocol document for description of 3BYTESLONG
void  Input3BytesLong(LPDWORD lpdword1)
{
   BYTE  byte1[3];
   DWORD dword1;

   GetNextTWCmdBytes((LPBYTE) &(byte1[0]),3);

   dword1 = (DWORD) byte1[2];
   dword1 |= (DWORD) (((DWORD) byte1[0]) << 8);
   dword1 |= (DWORD) (((DWORD) byte1[1]) << 16);

   *lpdword1 = dword1;
   return;
}

#if 0

/********************************************************************************
 *
 *  WinInitOnce
 *  THIS ROUTINE IS A PLACEHOLDER AND IS NEVER CALLED RIGHT NOW
 *
 *  Initialize the windows support for 16 and 256 color, once only.
 *
 ********************************************************************************/

VOID
WinInitOnce()
{
    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "WinInitOnce: entry"));
}


/********************************************************************************
 *
 *  WinDestroyOnce
 *  THIS ROUTINE IS A PLACEHOLDER AND IS NEVER CALLED RIGHT NOW
 *
 *  Destory the windows support for 16 and 256 color, once only.
 *
 ********************************************************************************/

VOID
WinDestroyOnce()
{
    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "WinDestroyOnce: entry"));
}

#endif


/********************************************************************************
 *
 *  Init16Color
 *
 *  Initialize the windows support for 16 color
 *
 ********************************************************************************/

BOOL
Init16Color()
{
    int i;
    BOOL jreturn = TRUE;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Init16Color: entry"));

    ASSERT( vhdc != NULL, 0 );

    //only destroyed once
    if (compatDC == NULL) {
        compatDC = CreateCompatibleDC(vhdc);
        if (compatDC == NULL) {
            ASSERT(0,0);
            jreturn = FALSE;
        }
    }

    /*
     *  Allocate 16 color solid pens
     */
    for ( i=0; i<16; i++ ) {
        hpensolid[i] = CreatePen( PS_SOLID, 1, twsolidcolor[i] );
        ASSERT(hpensolid[i] != NULL, 0);
        if (hpensolid[i] == NULL) {
            ASSERT(0,0);
            jreturn = FALSE;
        }
    }

    /*
     *  Start with no pen selected
     */
    dcstate.pencolor = -1;

    //brush init
    for (i=0; i<16 ; i++ ) {
        hbrsolid[i] = CreateSolidBrush(twsolidcolor[i]);
        if (hbrsolid[i] == NULL) {
            ASSERT(0,0);
            jreturn = FALSE;
        }
    }

    //initialize the brushobjobj to a valid state
    //we need to set all hbr to NULL so destroy wont go crazy
    number_nonsolidbrushes_realized = 0;
    for (i=0; i<115 ; i++ ) {
        brushobjobj[i].hbr = NULL;
    }
    brushobjobj[113].next = 113;
    brushobjobj[113].previous = 112;    //no previous
    brushobjobj[112].previous = 112;
    brushobjobj[112].next = 113;        //no next


    //initialize BrushDIB to a valid state
    //we preallocate and lock everything for performance reasons
    //last entry for the no caching case
    //initialize bitmapinfo for a 8 by 8 brush

#ifdef THINPAL
    bitmapinfo_4BPP_PALETTE.bmiHeader.biWidth = 8;
    bitmapinfo_4BPP_PALETTE.bmiHeader.biHeight = 8;      //NOTE: win16 does not support top down dib
#else
    bitmapinfo_4BPP_RGBQUAD.bmiHeader.biWidth = 8;
    bitmapinfo_4BPP_RGBQUAD.bmiHeader.biHeight = 8;      //NOTE: win16 does not support top down dib
#endif


    for (i=0; i < (MAXBRUSHREALIZED + 1) ; i++ ) {
#ifdef THINPAL
        BrushDIB[i].dib_handle = GlobalAlloc(GMEM_MOVEABLE,
              sizeof(bitmapinfo_4BPP_PALETTE) + 32);
#else
        BrushDIB[i].dib_handle = GlobalAlloc(GMEM_MOVEABLE,
              sizeof(bitmapinfo_4BPP_RGBQUAD) + 32);
#endif
        ASSERT(BrushDIB[i].dib_handle, 0);
        BrushDIB[i].dib_address = GlobalLock(BrushDIB[i].dib_handle);
        ASSERT(BrushDIB[i].dib_address,0);
        if (BrushDIB[i].dib_address == NULL) {
            ASSERT(0,0);
            jreturn = FALSE;
            break;
        }
        //now initialize BITMAPINFO
#ifdef THINPAL
        memcpy(BrushDIB[i].dib_address,&bitmapinfo_4BPP_PALETTE,
               sizeof(bitmapinfo_4BPP_PALETTE));
#else
        memcpy(BrushDIB[i].dib_address,&bitmapinfo_4BPP_RGBQUAD,
               sizeof(bitmapinfo_4BPP_RGBQUAD));
#endif
    }

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Init16Color: exit"));
    return(jreturn);
}


/********************************************************************************
 *
 *  Reset16Color
 *
 *  Reset the windows support for 16 color
 *
 ********************************************************************************/

BOOL
Reset16Color()
{
    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Reset16Color: entry"));

    ASSERT( vhdc != NULL, 0 );
    return(TRUE);
}


/********************************************************************************
 *
 *  Destroy16Color
 *
 *  Destory the windows support for 16 color
 *
 ********************************************************************************/

BOOL
Destroy16Color()
{
    int i;
    BOOL bretcode;
    HGLOBAL hretcode;
    BOOL jretcode = TRUE;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Destroy16Color: entry"));

    /*
     *  Select stock pen into dc
     */
    if ( vhdc != NULL ) {
        SelectObject( vhdc, (HPEN) GetStockObject(BLACK_PEN) );
    }

    /*
     *  Free up pens objects
     */
    for ( i=0; i<16 ; i++ ) {
        if ( (hpensolid[i] != NULL) &&
             (DeleteObject(hpensolid[i]) == FALSE) ) {
            ASSERT(0,0);
            jretcode = FALSE;
        }
        hpensolid[i] = NULL;
    }

    //select stock object brush so can delete all brushes
    if ( vhdc != NULL ) {
        SelectObject( vhdc, (HBRUSH) GetStockObject(WHITE_BRUSH) );
    }

    dcstate.hbr = NULL;
    for (i=0; i<16 ; i++ ) {
       if (hbrsolid[i] != NULL) {
          bretcode = DeleteObject(hbrsolid[i]);
          ASSERT(bretcode,0);
          hbrsolid[i] = NULL;
          if (!bretcode) {
              jretcode = FALSE;
          }
       }
    }
    for (i=0; i<112 ; i++) {
       if (brushobjobj[i].hbr != NULL) {
          bretcode = DeleteObject(brushobjobj[i].hbr);
          ASSERT(bretcode, 0);
          brushobjobj[i].hbr = NULL;
          if (!bretcode) {
             jretcode = FALSE;
          }
       }
    }
    //deal with special 114 index
    if (brushobjobj[114].hbr != NULL) {
       bretcode = DeleteObject(brushobjobj[114].hbr);
       ASSERT(bretcode, 0);
       brushobjobj[114].hbr = NULL;
       if (!bretcode) {
          jretcode =  FALSE;
       }
    }

#ifdef for_hardcore_debug
    ASSERT(brushobjobj[112].previous == 112, 0);
    ASSERT(brushobjobj[113].next == 113, 0);
#endif
    brushobjobj[112].next = 113;
    brushobjobj[112].previous = 112;    //no previous
    brushobjobj[113].previous = 112;
    brushobjobj[113].next = 113;        //no next

    //deal with the BrushDIB area
    for (i=0 ; i < MAXBRUSHREALIZED + 1 ; i++ ) {
       if ( BrushDIB[i].dib_handle != NULL ) {
          GlobalUnlock(BrushDIB[i].dib_handle);
          hretcode = GlobalFree(BrushDIB[i].dib_handle);
          ASSERT(!hretcode, 0);
          BrushDIB[i].dib_handle = NULL;
          if (hretcode != NULL) {
             jretcode = FALSE;
          }
       }
    }

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Destory16Color: exit"));
    return(jretcode);
}


/********************************************************************************
 *
 *  Init256Color
 *
 *  Initialize the windows support for 256 color
 *
 ********************************************************************************/

BOOL
Init256Color()
{
    int i;
    BOOL jretcode = TRUE;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Init256Color: entry"));

    ASSERT( vhdc != NULL, 0 );

    //only destroyed once
    if (compatDC == NULL) {
        compatDC = CreateCompatibleDC(vhdc);
        if (compatDC == NULL) {
            jretcode = FALSE;
        }
    }

    /*
     *  Create a default palette ...
     */
    TWCreateDefaultPalette( vhWnd, vhdc );

    /*
     *  Allocate 20 palette index pens
     */
    for ( i=0; i<10; i++ ) {

        hpensolid256[i] = CreatePen( PS_SOLID, 1, PALETTEINDEX(i) );
        ASSERT(hpensolid256[i] != NULL, 0);

        hpensolid256[i+10] = CreatePen( PS_SOLID, 1, PALETTEINDEX(i+246) );
        ASSERT(hpensolid256[i+10] != NULL, 0);
        if ((hpensolid256[i] == NULL) || (hpensolid256[i+10] == NULL) ) {
            ASSERT(0,0);
            jretcode = FALSE;
        }
    }

    /*
     *  Empty cache entry
     */
    hpensolid256[20] = NULL;

    /*
     *  Start with no pen selected
     */
    dcstate.pencolor = -1;

    //allocate 20 solid color brushes indexes
    for ( i=0; i<10 ; i++ ) {

        hbrsolid256[i] = CreateSolidBrush( PALETTEINDEX(i) );
        ASSERT(hbrsolid256[i] != NULL,0);

        hbrsolid256[i+10] = CreateSolidBrush( PALETTEINDEX(i+246) );
        ASSERT(hbrsolid256[i+10] != NULL,0);

        if ((hbrsolid256[i] == NULL) || (hbrsolid256[i+10] == NULL) ) {
            ASSERT(0,0);
            jretcode = FALSE;
        }
    }
    //show no dynamically realized solid color brush for other indexes
    hbrsolid256[20] = NULL;
    lasthbrsolid256index = 0;

    //reset dcstate
    //host makes sure not to use a brush until sent accross
    dcstate.hbr = NULL;

    //initialize the brushobjobj to a valid state
    //we need to set all hbr to NULL so destroy wont go crazy
    number_nonsolidbrushes_realized = 0;
    for (i=0; i<115 ; i++ ) {
        brushobjobj[i].hbr = NULL;
    }
    brushobjobj[113].next = 113;
    brushobjobj[113].previous = 112;    //no previous
    brushobjobj[112].previous = 112;
    brushobjobj[112].next = 113;        //no next


    //initialize BrushDIB to a valid state
    //we preallocate and lock everything for performance reasons
    //last entry for the no caching case
    //initialize bitmapinfo for a 8 by 8 brush

    bitmapinfo_8BPP_PALETTE.bmiHeader.biWidth = 8;
    bitmapinfo_8BPP_PALETTE.bmiHeader.biHeight = 8;      //NOTE: win16 does not support top down dib


    for (i=0; i < (MAXBRUSHREALIZED + 1) ; i++ ) {
        BrushDIB[i].dib_handle = GlobalAlloc(GMEM_MOVEABLE,
              sizeof(bitmapinfo_8BPP_PALETTE) + 64);
        ASSERT(BrushDIB[i].dib_handle, 0);
        BrushDIB[i].dib_address = GlobalLock(BrushDIB[i].dib_handle);
        ASSERT(BrushDIB[i].dib_address,0);
        if (BrushDIB[i].dib_address == NULL) {
            ASSERT(0,0);
            jretcode = FALSE;
            break;
        }
        //now initialize BITMAPINFO
        memcpy(BrushDIB[i].dib_address,&bitmapinfo_8BPP_PALETTE,
               sizeof(bitmapinfo_8BPP_PALETTE));
    }

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Init256Color: exit"));
    return(jretcode);
}


/********************************************************************************
 *
 *  Reset256Color
 *
 *  Reset the windows support for 256 color
 *
 ********************************************************************************/

BOOL
Reset256Color()
{
    UINT cColors;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Reset256Color: entry"));

    /*
     *  Realize our current palette
     */
    if ( (vhWnd != NULL) && (vhdc != NULL) ) {
        TWRealizePalette( vhWnd, vhdc, &cColors, TWREALIZEPALETTE_FG );
    }

    return(TRUE);
}


/********************************************************************************
 *
 *  Destroy256Color
 *
 *  Destory the windows support for 256 color
 *
 ********************************************************************************/

BOOL
Destroy256Color()
{
    int i;
    BOOL jretcode = TRUE;
    BOOL bretcode;
    HGLOBAL hretcode;

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Destroy256Color: entry"));

    /*
     *  Delete the default palette ...
     */
    if ( vhdc != NULL ) {
        TWDeleteDefaultPalette( vhWnd, vhdc );
    }

    /*
     *  Select stock pen into dc
     */
    if ( vhdc != NULL ) {
        SelectObject( vhdc, (HPEN) GetStockObject(BLACK_PEN) );
    }

    /*
     *  Remove 21 palette index pens
     */
    for ( i=0; i<21; i++ ) {
        if ( (hpensolid256[i] != NULL) &&
             (DeleteObject(hpensolid256[i]) == FALSE) ) {
            ASSERT(0,0);
            jretcode = FALSE;
        }
        hpensolid256[i] = NULL;
    }

    //select stock object brush so can delete all brushes
    if ( vhdc != NULL ) {
        SelectObject( vhdc, (HBRUSH) GetStockObject(WHITE_BRUSH) );
    }

    dcstate.hbr = NULL;

    //remove 21 solid color brushes that are realize
    for ( i=0; i<21; i++ ) {
        if ( (hbrsolid256[i] != NULL) &&
             (DeleteObject(hbrsolid256[i]) == FALSE) ) {
            ASSERT(0,0);
            jretcode = FALSE;
        }
        hbrsolid256[i] = NULL;
    }
    lasthbrsolid256index = 0;

    for (i=0; i<112 ; i++) {
       if (brushobjobj[i].hbr != NULL) {
          bretcode = DeleteObject(brushobjobj[i].hbr);
          ASSERT(bretcode, 0);
          brushobjobj[i].hbr = NULL;
          if (!bretcode) {
             jretcode = FALSE;
          }
       }
    }
    //deal with special 114 index
    if (brushobjobj[114].hbr != NULL) {
       bretcode = DeleteObject(brushobjobj[114].hbr);
       ASSERT(bretcode, 0);
       brushobjobj[114].hbr = NULL;
       if (!bretcode) {
          jretcode =  FALSE;
       }
    }

#ifdef for_hardcore_debug
    ASSERT(brushobjobj[112].previous == 112, 0);
    ASSERT(brushobjobj[113].next == 113, 0);
#endif
    brushobjobj[112].next = 113;
    brushobjobj[112].previous = 112;    //no previous
    brushobjobj[113].previous = 112;
    brushobjobj[113].next = 113;        //no next

    //deal with the BrushDIB area
    for (i=0 ; i < MAXBRUSHREALIZED + 1 ; i++ ) {
       if ( BrushDIB[i].dib_handle != NULL ) {
          GlobalUnlock(BrushDIB[i].dib_handle);
          hretcode = GlobalFree(BrushDIB[i].dib_handle);
          ASSERT(!hretcode, 0);
          BrushDIB[i].dib_handle = NULL;
          if (hretcode != NULL) {
             jretcode = FALSE;
          }
       }
    }

    TRACE((TC_TW,TT_TW_ENTRY_EXIT+TT_TW_CONNECT, "Destroy256Color: exit"));
    return(jretcode);
}
