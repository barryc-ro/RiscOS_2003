
/*****************************************************************************
*
*   WFCURSOR.C
*
*   Thin Wire Windows - client cursor protocol cracker
*
*   Copyright (c) Citrix Systems Inc. 1995-1996
*
*   Author: Kurt Perry (kurtp) 15-May-1995
*
*   $Log$
*  
*     Rev 1.6   15 Apr 1997 18:16:54   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   09 Feb 1996 11:27:52   kurtp
*  update
*  
*     Rev 1.4   03 Jan 1996 13:34:00   kurtp
*  update
*  
****************************************************************************/

#include "wfglobal.h"


/*=============================================================================
==  Local Functions
=============================================================================*/

BOOL    PointerSetShape( HWND, HDC );
HCURSOR CreateMonoPointer( HWND, LPBYTE, LPBYTE, int, int );
HCURSOR CreateColorPointer( HWND, HDC, LPBYTE, LPBYTE, UINT, int, int );
VOID    DestroyPointer( HWND );
VOID    InvertBitmap( LPBYTE, LPBYTE, UINT );


/*=============================================================================
==  External Functions Defined
=============================================================================*/


/*=============================================================================
==  Macros
=============================================================================*/

#define GF_XLO_NOT_TRIVIAL          0x01
#define GF_COLOR_8BPP               0x02
#define GF_RESERVED                 0x04
#define GF_MASK_CACHE_HANDLE_MASK   0x07
#define GF_SAVE_MASK_AND_HOTSPOT    0x08
#define GF_RESTORE_MASK             0x10
#define GF_USE_HOTSPOT              0x20
#define GF_COLOR_POINTER            0x40
#define GF_HIDE_POINTER             0x80

#define CF_BITMAP_CACHE_HANDLE_MASK 0x0F
#define CF_SAVE_BITMAP              0x40
#define CF_RESTORE_BITMAP           0x80

#define HOTSPOT_MASK_X              0x1F
#define HOTSPOT_MASK_Y              0x1F

#define BITMAP_4BPP                    4
#define BITMAP_8BPP                    8

#define SIZE_AND_MASK                128        //  32x32 mono
#define SIZE_XOR_MASK                128        //  32x32 mono
#define SIZE_BITMAP_4BPP             512        //  32x32 color
#define SIZE_BITMAP_8BPP            1024        //  32x32 color

#define MAX_BITMAP_DATA (SIZE_AND_MASK + SIZE_BITMAP_8BPP)  //  worst case

#define MONO_BYTES_PER_SCANLINE        4
#define COLOR_BYTES_PER_SCANLINE_4BPP 16
#define COLOR_BYTES_PER_SCANLINE_8BPP 32

#define BYTES_MONOPTR                256
#define BYTES_128                    128
#define BYTES_512                    512
#define COLOR_BYTES_PER_MONO_BYTE_4BPP (COLOR_BYTES_PER_SCANLINE_4BPP/MONO_BYTES_PER_SCANLINE)
#define COLOR_BYTES_PER_MONO_BYTE_8BPP (COLOR_BYTES_PER_SCANLINE_8BPP/MONO_BYTES_PER_SCANLINE)


/*=============================================================================
==  Global Vars
=============================================================================*/

extern COLOR_CAPS vColor;
extern LOGPALETTE * vpLogPalette;


/*=============================================================================
==  Local Vars (static)
=============================================================================*/

/*
 *  vhCursor    - normal cursor/pointer
 *  vhCursorNot - inverse cursor/pointer (for click-ticks)
 *  vhCursorVis - currently visible cursor/pointer
 *  vfCursor    - boolean for cursor/icon (win32 only)
 */
HCURSOR vhCursor    = NULL;
HCURSOR vhCursorNot = NULL;
HCURSOR vhCursorVis = NULL;

#ifdef WIN32
BOOL    vfCursor    = TRUE;
#endif

int     vxHotSpot = 0;
int     vyHotSpot = 0;
int     vaxHotSpot[8];
int     vayHotSpot[8];

int     vcxCursor = 32;
int     vcyCursor = 32;


/****************************************************************************
 *
 *  TWCmdPointerSetShape (POINTER_SETSHAPE service routine)
 *
 *  PARAMETERS:
 *     none
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
 ****************************************************************************/

void 
TWCmdPointerSetShape( HWND hWnd, HDC hDC )
{

    TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_PTRSHAPE, "TWCmdPointerSetShape: entered" ));

    /*
     *  Call worker routine
     */
    if ( !PointerSetShape( hWnd, hDC ) ) {

        ASSERT( 0, 0 );
    }
 
    TWCmdReturn( TRUE ); // return to NewNTCommand or ResumeNTCommand
}


/****************************************************************************
 *
 *  PointerSetShape (POINTER_SETSHAPE worker routine)
 *
 *  PARAMETERS:
 *     none
 *
 *  RETURN:
 *
 ****************************************************************************/

BOOL
PointerSetShape( HWND hWnd, HDC hDC )
{
    int     xHotSpot;
    int     yHotSpot;
    POINT   pt;
    UINT    hAndMask;
    UINT    hBitmap;
    UINT    cbAndMask;
    UINT    cbBitmap;
    UINT    cbScanLine;
    BYTE    iTemp0;
    BYTE    iTemp1;
    BYTE    fGeneral;
    HCURSOR hCursor;
    LPBYTE  lpAndMask = NULL;
    LPBYTE  lpXorMask = NULL;
    LPBYTE  lpBitmap  = NULL;
    LPBYTE  lpBuffer  = (LPBYTE) lpstatic_buffer;
    INT     i;
    LPBYTE  llpBitmap;
    UINT    cbRead;
    CHUNK_TYPE ChunkType;
    UINT    bppBitmap;
    UINT    cbCache;

    /*
     *  Retrieve the General Flag byte
     */
    GetNextTWCmdBytes( &fGeneral, 1 );
    TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: command byte - %02x", fGeneral ));

    /*
     *  Hide pointer?
     */
    if ( (fGeneral & GF_HIDE_POINTER) ) {

        /*
         *  Call destroy to remove pointer and free resources
         */
        DestroyPointer( hWnd );

        return( TRUE );
    }

    /*
     *  Do some up front sanity checking
     */
#ifdef DEBUG
    if ( (fGeneral & GF_SAVE_MASK_AND_HOTSPOT) &&
         (fGeneral & GF_RESTORE_MASK) ) {

        ASSERT( 0, 0 );
        return( FALSE );
    }
#endif

    /*
     *  Get current hot spot from packet or use old one
     */
    if ( (fGeneral & GF_USE_HOTSPOT) ) {

        /*
         *  Retreive X hotspot
         */
        GetNextTWCmdBytes( &xHotSpot, 1 );
        xHotSpot &= HOTSPOT_MASK_X;

        /*
         *  Retreive Y hotspot
         */
        GetNextTWCmdBytes( &yHotSpot, 1 );
        yHotSpot &= HOTSPOT_MASK_Y;

        TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: set new hotspot - xHotSpot=%u, yHotSpot=%u", xHotSpot, yHotSpot ));
    }
    else if ( (fGeneral & GF_COLOR_POINTER) ) {

        xHotSpot = vxHotSpot;
        yHotSpot = vyHotSpot;

        TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: restore color hotspot - xHotSpot=%u, yHotSpot=%u", xHotSpot, yHotSpot ));
    }
    else {

        hAndMask = (UINT) (fGeneral & GF_MASK_CACHE_HANDLE_MASK);
        xHotSpot = vaxHotSpot[hAndMask];
        yHotSpot = vayHotSpot[hAndMask];
        TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: restore mono (%u) hotspot - xHotSpot=%u, yHotSpot=%u", hAndMask, xHotSpot, yHotSpot ));
    }

    /*
     *  Get cache area if we are moving AND mask to/from cache
     */
    if ( (fGeneral & (GF_RESTORE_MASK | GF_SAVE_MASK_AND_HOTSPOT)) ) {

        /*
         *  Get color AND mask area in cache
         */
        if ( (fGeneral & GF_COLOR_POINTER) ) {
    
            /*
             *  Construct 12 bit color cache handle 
             */
            GetNextTWCmdBytes( &iTemp0, 1 );
            GetNextTWCmdBytes( &iTemp1, 1 );
            hAndMask = (UINT) ((iTemp0 & 0x0f) << 8) | (iTemp1 & 0xff);
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: cache handle - hAndMask=%08lx", hAndMask ));

            /*
             *  Get cache read or write pointer
             */
            if ( (fGeneral & GF_RESTORE_MASK) ) {

                lpAndMask = lpTWCacheRead( hAndMask, _128B, &cbAndMask, 0 );
                ASSERT( lpAndMask != NULL, 0 );
            }
            else if ( (fGeneral & GF_SAVE_MASK_AND_HOTSPOT) ) {

                lpAndMask = lpTWCacheWrite( hAndMask, _128B, BYTES_128, 0 );
                ASSERT( lpAndMask != NULL, 0 );
            }
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: cache pointer - lpAndMask=%08lx", lpAndMask ));
        }

        /*
         *  Get mono AND and XOR area in cache
         */
        else {
            
            /*
             *  Construct 3 bit mono cache handle
             */
            hAndMask = (UINT) (fGeneral & GF_MASK_CACHE_HANDLE_MASK);
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: cache handle - hAndMask=%08lx", hAndMask ));

            /*
             *  Get cache read or write pointer
             */
            if ( (fGeneral & GF_RESTORE_MASK) ) {

                lpAndMask = lpTWCacheRead( hAndMask, monoptr, &cbAndMask, 0 );
                ASSERT( lpAndMask != NULL, 0 );
            }
            else if ( (fGeneral & GF_SAVE_MASK_AND_HOTSPOT) ) {

                lpAndMask = lpTWCacheWrite( hAndMask, monoptr, BYTES_MONOPTR, 0 );
                ASSERT( lpAndMask != NULL, 0 );
            }
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: cache pointer - lpAndMask=%08lx", lpAndMask ));

            lpXorMask = lpAndMask + SIZE_AND_MASK;
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: cache pointer - lpXorMask=%08lx", lpAndMask ));
        }
    }

    /*
     *  Read mask from data stream?
     */
    if ( !(fGeneral & GF_RESTORE_MASK) ) {

        /*
         *  Get some local storage if not caching
         */
        if ( lpAndMask == NULL ) {

            /*
             *  Get some local storage
             */
            lpAndMask = lpBuffer;
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: local pointer - lpAndMask=%08lx", lpAndMask ));

            /*
             *  Move local buffer pointer
             */
            lpBuffer += SIZE_AND_MASK;

            /*
             *  For mono pointers get pointer to XOR area
             */
            if ( !(fGeneral & GF_COLOR_POINTER) ) {
    
                /*
                 *  Create XOR pointer just in case
                 */
                lpXorMask = lpBuffer;
                TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: local pointer - lpXorMask=%08lx", lpXorMask ));
    
                /*
                 *  Move local buffer pointer
                 */
                lpBuffer += SIZE_AND_MASK;
            }
        }

        /*
         *  Retreive AND mask always, both color and mono use it
         */
        GetNextTWCmdBytes( lpAndMask, SIZE_AND_MASK );

        /*
         *  If color ...
         */
        if ( (fGeneral & GF_COLOR_POINTER) ) {

            /*
             *  Close the color write and open a read handle
             */
            if ( (fGeneral & GF_SAVE_MASK_AND_HOTSPOT) ) {
    
                finishedTWCacheWrite( BYTES_128 );
                lpAndMask = lpTWCacheRead( hAndMask, _128B, &cbAndMask, 0 );
                ASSERT( lpAndMask != NULL, 0 );
            }
        }

        /*
         *  ... else if mono
         */
        else {

            /*
             *  Retreive XOR mask 
             */
            GetNextTWCmdBytes( lpXorMask, SIZE_XOR_MASK );

            /*
             *  Close the mono write and open a read handle
             */
            if ( (fGeneral & GF_SAVE_MASK_AND_HOTSPOT) ) {

                finishedTWCacheWrite( BYTES_MONOPTR );
                lpAndMask = lpTWCacheRead( hAndMask, monoptr, &cbAndMask, 0 );
                ASSERT( lpAndMask != NULL, 0 );
            }
        }
    }    

    /*
     *  Color?
     */
    if ( (fGeneral & GF_COLOR_POINTER) ) {

       BITMAP_HEADER  bm_header;  //  only used for 256 color bitmaps

        /*
         *  Setup BPP info
         */
        if ( vColor == Color_Cap_256 ) {
            if ( (fGeneral & GF_COLOR_8BPP) ) {
                cbScanLine = COLOR_BYTES_PER_SCANLINE_8BPP;
                cbBitmap   = SIZE_BITMAP_8BPP;
                ChunkType  = _2K;
                bppBitmap  = BITMAP_8BPP;
                cbCache    = cbBitmap + sizeof(BITMAP_HEADER);
            }
            else {
                cbScanLine = COLOR_BYTES_PER_SCANLINE_4BPP;
                cbBitmap   = SIZE_BITMAP_4BPP;
                ChunkType  = _512B;
                bppBitmap  = BITMAP_4BPP;
                cbCache    = cbBitmap;
            }
        }
        else {
            cbScanLine = COLOR_BYTES_PER_SCANLINE_4BPP;
            cbBitmap   = SIZE_BITMAP_4BPP;
            ChunkType  = _512B;
            bppBitmap  = BITMAP_4BPP;
            cbCache    = cbBitmap;
        }

        /*
         *  Get color specific command byte
         */
        GetNextTWCmdBytes( &iTemp0, 1 );
        TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: color specific command byte - %02x", iTemp0 ));

        /* 
         *  If storing or restoring bitmap get cache area for bitmap
         */
        if ( (iTemp0 & (CF_RESTORE_BITMAP | CF_SAVE_BITMAP)) ) {

            /*
             *  Construct 12 bit color cache handle 
             */
            GetNextTWCmdBytes( &iTemp1, 1 );
            hBitmap = (int) ((iTemp0 & 0x0f) << 8) | (iTemp1 & 0xff);
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: cache handle - hBitmap=%08lx", hBitmap ));

            /*
             *  Get cache read or write pointer
             */
            if ( (iTemp0 & CF_RESTORE_BITMAP) ) {

                lpBitmap = lpTWCacheRead( hBitmap, ChunkType, &cbRead, 0 );
                ASSERT(cbRead == cbCache, cbRead);
                ASSERT(lpBitmap != NULL, 0);

                /*
                 *  Extract and validate bitmap header on 256 bitmaps
                 */
                if ( (vColor == Color_Cap_256) && (ChunkType == _2K) ) {
                    memcpy((LPVOID) &bm_header, lpBitmap, sizeof(bm_header));
                    lpBitmap += sizeof(bm_header);
                    ASSERT(bm_header.flags.color      == 2,  bm_header.flags.color);
                    ASSERT(bm_header.flags.chained    == 0,  bm_header.flags.chained);   
                    ASSERT(bm_header.total_scanlines  == 32, bm_header.total_scanlines) 
                    ASSERT(bm_header.cache_bytes_wide == 32, bm_header.cache_bytes_wide);
                    ASSERT(bm_header.pixel_width      == 32, bm_header.pixel_width);
                    ASSERT(bm_header.pixel_offset     == 0,  bm_header.pixel_offset);    
                    ASSERT(bm_header.flags.size       == 0,  bm_header.flags.size);
                }
            }
            else if ( (iTemp0 & CF_SAVE_BITMAP) ) {

                lpBitmap = lpTWCacheWrite( hBitmap, ChunkType, cbBitmap, (ChunkType == _2K ? hBitmap : 0) );
                ASSERT( lpBitmap != NULL, 0 );

                /*
                 *  Setup bitmap header and write
                 */
                if ( (vColor == Color_Cap_256) && (ChunkType == _2K) ) {
                    bm_header.flags.color      = 2;
                    bm_header.flags.chained    = 0;
                    bm_header.total_scanlines  = 32;
                    bm_header.cache_bytes_wide = 32;
                    bm_header.pixel_width      = 32;
                    bm_header.pixel_offset     = 0;
                    bm_header.flags.size       = 0;
                    memcpy(lpBitmap, (LPVOID) &bm_header, sizeof(bm_header));
                    lpBitmap += sizeof(bm_header);
               }
            }
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: cache pointer - lpBitmap=%08lx", lpBitmap ));
        }

        /*
         *  Read mask from data stream?
         */
        if ( !(iTemp0 & CF_RESTORE_BITMAP) ) {

            /*
             *  Get some local storage if not caching
             */
            if ( lpBitmap == NULL ) {

                /*
                 *  Get local storage
                 */
                lpBitmap = lpBuffer;
                TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: local pointer - lpBitmap=%08lx", lpBitmap ));
    
                /*
                 *  Move local buffer pointer
                 */
                lpBuffer += cbBitmap;
            }

            /*
             *  Retreive bitmap
             */
            GetNextTWCmdBytes( lpBitmap, cbBitmap );

            /*
             *  Reverse bitmap, DIBs are backwards
             *  - have to update lpBuffer, we use more space later
             */
            llpBitmap = lpBuffer;
            lpBuffer += cbBitmap;
            for ( i = 0; i < vcyCursor; i++ ) {
                memcpy( (llpBitmap + i * cbScanLine), 
                        (lpBitmap + ((vcyCursor - i - 1) * cbScanLine)), 
                        cbScanLine );
            }
            memcpy( lpBitmap, llpBitmap, cbBitmap );
        
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: reversed color bitmap %08lx", (ULONG) llpBitmap ));

            /*
             *  Close the color write and open a read handle
             */
            if ( (iTemp0 & CF_SAVE_BITMAP) ) {
    
                finishedTWCacheWrite( cbCache );
                lpBitmap = lpTWCacheRead( hBitmap, ChunkType, &cbRead, 0 );
                ASSERT( cbRead == cbCache, cbRead );
                ASSERT( lpBitmap != NULL, 0 );

                /*
                 *  Extract and validate bitmap header on 256 bitmaps
                 */
                if ( (vColor == Color_Cap_256) && (ChunkType == _2K) ) {
                    memcpy((LPVOID) &bm_header, lpBitmap, sizeof(bm_header));
                    lpBitmap += sizeof(bm_header);
                    ASSERT(bm_header.flags.color      == 2,  bm_header.flags.color);
                    ASSERT(bm_header.flags.chained    == 0,  bm_header.flags.chained);   
                    ASSERT(bm_header.total_scanlines  == 32, bm_header.total_scanlines) 
                    ASSERT(bm_header.cache_bytes_wide == 32, bm_header.cache_bytes_wide);
                    ASSERT(bm_header.pixel_width      == 32, bm_header.pixel_width);
                    ASSERT(bm_header.pixel_offset     == 0,  bm_header.pixel_offset);    
                    ASSERT(bm_header.flags.size       == 0,  bm_header.flags.size);
                }
            }
        }

        /*
         *  Remove current pointer and destroy it
         */
        DestroyPointer( hWnd );
    
        /*
         *  Create color pointer
         *
         *  Note: CreateColorPointer does both image and ~image for WIN16
         */
        hCursor = CreateColorPointer( hWnd, hDC, lpAndMask, lpBitmap, 
                                      bppBitmap, xHotSpot, yHotSpot );

#ifdef WIN32
        /*
         *  Create inverse color pointer, then restore bitmap
         *
         */
        InvertBitmap( lpAndMask, lpBitmap, cbBitmap );
        vhCursorNot = CreateColorPointer( hWnd, hDC, lpAndMask, lpBitmap, 
                                          bppBitmap, xHotSpot, yHotSpot );
        InvertBitmap( lpAndMask, lpBitmap, cbBitmap );
#endif

        /*
         *  Allow selection now
         */
        vhCursorVis = vhCursor = hCursor;
    }

    /*
     *  Mono
     */
    else {

        /*
         *  Remove current pointer and destroy it
         */
        DestroyPointer( hWnd );
    
        /*
         *  Create mono pointer
         */
        hCursor = CreateMonoPointer( hWnd, lpAndMask, lpXorMask, xHotSpot, yHotSpot );

        /*
         *  Create inverse mono pointer, then restore bitmap
         */
        InvertBitmap( lpAndMask, lpXorMask, BYTES_128 );
        vhCursorNot = CreateMonoPointer( hWnd, lpAndMask, lpXorMask, xHotSpot, yHotSpot );
        InvertBitmap( lpAndMask, lpXorMask, BYTES_128 );

        /*
         *  Allow selection now
         */
        vhCursorVis = vhCursor = hCursor;
    }

    /*
     *  Save hotspot? Always color and sometime mono!
     */
    if ( (fGeneral & GF_COLOR_POINTER) ) {
        vxHotSpot = xHotSpot;
        vyHotSpot = yHotSpot;
        TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: save color hotspot - xHotSpot=%u, yHotSpot=%u", xHotSpot, yHotSpot ));
    }
    else if ( (fGeneral & GF_SAVE_MASK_AND_HOTSPOT) ) {
        hAndMask = (UINT) (fGeneral & GF_MASK_CACHE_HANDLE_MASK);
        vaxHotSpot[hAndMask] = xHotSpot;
        vayHotSpot[hAndMask] = yHotSpot;
        TRACE(( TC_TW, TT_TW_PTRSHAPE, "PointerSetShape: save mono (%u) hotspot - xHotSpot=%u, yHotSpot=%u", hAndMask, xHotSpot, yHotSpot ));
    }

    /*
     *  Find cursor
     */
    GetCursorPos( &pt );

    /*
     *  Only display cursor if we have focus and mouse is within our window
     */
    if ( (!IsIconic(hWnd)) && 
         (hWnd == GetFocus()) && 
         (hWnd == WindowFromPoint(pt)) ) {
        SetCursor( vhCursorVis );
    }

    return( TRUE );
}


/****************************************************************************
 *
 *  CreateMonoPointer
 *
 *  PARAMETERS:
 *     none
 *
 *  RETURN:
 *
 ****************************************************************************/

HCURSOR
CreateMonoPointer( HWND   hWnd,
                   LPBYTE lpAndMask, 
                   LPBYTE lpXorMask, 
                   int    xHotSpot, 
                   int    yHotSpot )
{
    HCURSOR   hCursor = NULL;
    HINSTANCE hInst = NULL;

    TRACE(( TC_TW, TT_TW_PTRSHAPE, "CreateMonoPointer: lpAndMask %08lx, lpXorMask %08lx", (ULONG) lpAndMask, (ULONG) lpXorMask ));

    /*
     *  Get instance handle for create
     */
#ifdef WIN16
    hInst = (HINSTANCE) GetWindowWord( hWnd, GWW_HINSTANCE );
#endif

    /*
     *  Create new cursor
     */
    hCursor = CreateCursor( hInst, 
                            xHotSpot, 
                            yHotSpot, 
                            vcxCursor, 
                            vcyCursor, 
                            lpAndMask, 
                            lpXorMask );

    ASSERT( hCursor != NULL, 0 );


#ifdef WIN32
    /*
     *  Cursor is a cursor!
     */
    vfCursor = TRUE;
#endif

    /*
     *  Return cursor handle
     */
    return( hCursor );
}


/****************************************************************************
 *
 *  CreateColorPointer
 *
 *  This is the WIN 32 version of this routine.
 *
 *  PARAMETERS:
 *     none
 *
 *  RETURN:
 *
 ****************************************************************************/

HCURSOR
CreateColorPointer( HWND   hWnd,
                    HDC    hDC, 
                    LPBYTE lpAndMask, 
                    LPBYTE lpBitmap, 
                    UINT   bppBitmap,
                    int    xHotSpot, 
                    int    yHotSpot )
{
    HCURSOR hCursor   = NULL;
    LPBYTE  lpBuffer  = (LPBYTE) lpstatic_buffer + MAX_BITMAP_DATA;

#ifdef WIN16

    int    i;
    int    j;
    BYTE   b0;
    BYTE   b1;
    LPBYTE p;
    DWORD  dwMono;
    LPBYTE llpBitmap;
    UINT   cbScanLine = (bppBitmap == BITMAP_4BPP)    ?
                        COLOR_BYTES_PER_SCANLINE_4BPP :
                        COLOR_BYTES_PER_SCANLINE_8BPP;

    /*
     *  Reverse bitmap, DIBs are backwards
     *  - have to update lpBuffer, we use more space later
     */
    llpBitmap = lpBuffer;
    lpBuffer += (vcyCursor * cbScanLine);
    for ( i = 0; i < vcyCursor; i++ ) {
        memcpy( (llpBitmap + i * cbScanLine), 
                (lpBitmap + ((vcyCursor - i - 1) * cbScanLine)), 
                cbScanLine );
    }

    TRACE(( TC_TW, TT_TW_PTRSHAPE, "CreateColorPointer: reversed color bitmap %08lx", (ULONG) llpBitmap ));

    /*
     *  Convert the Color DIB into a Mono XOR map
     */
    if ( (vColor == Color_Cap_16) || (bppBitmap == BITMAP_4BPP) ) {

        for ( i=0, p=lpBuffer ; i<vcxCursor; i++ ) {
    
            for ( j=0, dwMono=0; j<COLOR_BYTES_PER_SCANLINE_4BPP; j++ ) {
    
                b0 = *(llpBitmap + (i * COLOR_BYTES_PER_SCANLINE_4BPP) + j);
                b1 = (b0 & 0x0f);
                b0 = (b0 & 0xf0);
    
                dwMono = dwMono << 1;
                if ( (b0 & 0x80) && (b0 != 0x90) && (b0 != 0xc0) )
                    ++dwMono;
    
                dwMono = dwMono << 1;
                if ( (b1 & 0x08) && (b1 != 0x09) && (b1 != 0x0c) )
                    ++dwMono;
            }
    
            for ( j=0; j<MONO_BYTES_PER_SCANLINE; j++ ) {
    
                *(p++) = (BYTE) ((dwMono & 0xff000000) >> 24);
                dwMono = dwMono << 8;
            }
        }
    }
    else {
        for ( i=0, p=lpBuffer ; i<vcxCursor; i++ ) {
    
            for ( j=0, dwMono=0; j<COLOR_BYTES_PER_SCANLINE_8BPP; j++ ) {
    
                b0 = *(llpBitmap + (i * COLOR_BYTES_PER_SCANLINE_8BPP) + j);
    
                dwMono = dwMono << 1;
                if ( (((2 * vpLogPalette->palPalEntry[b0].peRed) + 
                       (1 * vpLogPalette->palPalEntry[b0].peBlue) +
                       (5 * vpLogPalette->palPalEntry[b0].peGreen)) / 8) > 128 ) {
                    ++dwMono;
                }
            }
    
            for ( j=0; j<MONO_BYTES_PER_SCANLINE; j++ ) {
    
                *(p++) = (BYTE) ((dwMono & 0xff000000) >> 24);
                dwMono = dwMono << 8;
            }
        }
    }

    /*
     *  Make a mono pointer image
     */
    hCursor = CreateMonoPointer( hWnd, lpAndMask, lpBuffer, xHotSpot, yHotSpot );

    /*
     *  Invert pointer here for reverse image, more efficient for WIN16 
     *  because there is no need to restore bitmap (transient).
     */
    InvertBitmap( lpAndMask, lpBuffer, BYTES_128 );

    /*
     *  Make a inverse mono pointer
     */
    vhCursorNot = CreateMonoPointer( hWnd, lpAndMask, lpBuffer, xHotSpot, yHotSpot );

#else   // WIN32

    int         cbBMI;
    WORD        i;
    WORD *      pWord;
    BITMAP      bm;
    ICONINFO    ici;
    HBITMAP     hbmMask;
    HBITMAP     hbmColor;
    PBITMAPINFO pbmi;

    TRACE(( TC_TW, TT_TW_PTRSHAPE, "CreateColorPointer: lpAndMask %08lx, lpBitmap %08lx", (ULONG) lpAndMask, (ULONG) lpBitmap ));

    /*
     *  Create AND mask
     */
    bm.bmType       = 0;
    bm.bmWidth      = vcxCursor;
    bm.bmHeight     = vcyCursor;
    bm.bmWidthBytes = MONO_BYTES_PER_SCANLINE;
    bm.bmPlanes     = 1;
    bm.bmBitsPixel  = 1;
    bm.bmBits       = lpAndMask;
    hbmMask         = CreateBitmapIndirect( &bm );

    /*
     *  Create DIB different depending on color depth
     */
    if ( bppBitmap == BITMAP_4BPP ) {
    
        /*
         *  Get local memory for bmi
         */
        cbBMI = sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * 16);
        pbmi = (PBITMAPINFO) lpBuffer;
        lpBuffer += cbBMI;
        memset( pbmi, 0, cbBMI );
     
        /*
         *  Create DI Bitmap
         */
        pbmi->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
        pbmi->bmiHeader.biWidth       = vcxCursor;
        pbmi->bmiHeader.biHeight      = vcyCursor;
        pbmi->bmiHeader.biPlanes      = 1;
        pbmi->bmiHeader.biBitCount    = bppBitmap;
        pbmi->bmiHeader.biCompression = BI_RGB;
        memcpy( &(pbmi->bmiColors[0]), &(bitmapinfo_4BPP_RGBQUAD.bmiColors[0]), sizeof(RGBQUAD) * 16 );
        hbmColor = CreateDIBitmap( hDC, 
                                   (BITMAPINFOHEADER FAR *) pbmi, 
                                   CBM_INIT, 
                                   lpBitmap,
                                   pbmi,
                                   DIB_RGB_COLORS );

    }
    else if ( bppBitmap == BITMAP_8BPP ) {

        /*
         *  Get local memory for bmi
         */
        cbBMI = sizeof(BITMAPINFOHEADER) + (sizeof(WORD) * 256);
        pbmi = (PBITMAPINFO) lpBuffer;
        lpBuffer += cbBMI;
        memset( pbmi, 0, cbBMI );
     
        /*
         *  Initialize header
         */
        pbmi->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
        pbmi->bmiHeader.biWidth       = vcxCursor;
        pbmi->bmiHeader.biHeight      = vcyCursor;
        pbmi->bmiHeader.biPlanes      = 1;
        pbmi->bmiHeader.biBitCount    = bppBitmap;
        pbmi->bmiHeader.biCompression = BI_RGB;

        /*
         *  Create trivial xlate table
         */
        for( pWord=(WORD*)pbmi->bmiColors, i=0; i<256; i++ ) {
            *(pWord++) = i;
        }

        /*
         *  Create DI Bitmap
         */
        hbmColor = CreateDIBitmap( hDC, 
                                   (BITMAPINFOHEADER FAR *) pbmi, 
                                   CBM_INIT, 
                                   lpBitmap,
                                   pbmi,
                                   DIB_PAL_COLORS );

        ASSERT( hbmColor != NULL, 0 );
    }
#ifdef DEBUG
    else {
        ASSERT( FALSE, 0 );
    }
#endif

    /*
     *  Fill in ICONINFO struct
     */
    ici.fIcon    = FALSE;   //  really a cursor
    ici.xHotspot = xHotSpot;
    ici.yHotspot = yHotSpot;
    ici.hbmMask  = hbmMask;
    ici.hbmColor = hbmColor;

    /*
     *  Create new icon, er make that cursor
     */
    hCursor = (HCURSOR) CreateIconIndirect( &ici );

    ASSERT( hCursor != NULL, 0 );

    /*
     *  Delete bitmaps
     */
    DeleteObject( hbmColor );
    DeleteObject( hbmMask );

    /*
     *  Cursor is a DIB!
     */
    vfCursor = FALSE;

#endif

    /*
     *  Return cursor handle
     */
    return( hCursor );
}


/****************************************************************************
 *
 *  DistroyOldPointer
 *
 *  PARAMETERS:
 *
 *  RETURN:
 *
 ****************************************************************************/

VOID
DestroyPointer( HWND hWnd )
{
    POINT   pt;
    HCURSOR hCursor;
    HCURSOR hCursorNot;
    HCURSOR hCursorVis;

    TRACE(( TC_TW, TT_TW_PTRSHAPE, "DestroyPointer: hide cursor" ));

    /*
     *  Remove any race condition with messgae loop
     */
    hCursor     = vhCursor;
    hCursorNot  = vhCursorNot;
    hCursorVis  = vhCursorVis;
    vhCursor    = NULL;
    vhCursorNot = NULL;
    vhCursorVis = NULL;

    /*
     *  Do we need to remove pointer
     */
    if ( (!IsIconic(hWnd)) && 
         (hWnd == GetFocus()) && 
         (hWnd == WindowFromPoint(pt)) ) {
        SetCursor( NULL );
    }

    /*
     *  Destroy old pointer
     */
    if ( hCursor != NULL ) {

#ifdef WIN32
        /*
         *  Check if we have in cursor or an icon
         */
        if  ( vfCursor ) {
#endif
            DestroyCursor( hCursor );
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "DestroyPointer: destroy cursor - vhCursor=%08lx", (ULONG) hCursor ));
#ifdef WIN32
        }
        else {
            DestroyIcon( (HICON) hCursor );
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "DestroyPointer: destroy icon - vhCursor=%08lx", (ULONG) hCursor ));
        }
#endif
    }

    /*
     *  Destroy old inverse pointer
     */
    if ( hCursorNot != NULL ) {

#ifdef WIN32
        /*
         *  Check if we have in cursor or an icon
         */
        if  ( vfCursor ) {
#endif
            DestroyCursor( hCursorNot );
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "DestroyPointer: destroy cursor - vhCursorNot=%08lx", (ULONG) hCursorNot ));
#ifdef WIN32
        }
        else {

            DestroyIcon( (HICON) hCursorNot );
            TRACE(( TC_TW, TT_TW_PTRSHAPE, "DestroyPointer: destroy icon - vhCursorNot=%08lx", (ULONG) hCursorNot ));
        }
#endif
    }
}


/****************************************************************************
 *
 *  InvertBitmap
 *
 *  PARAMETERS:
 *
 *  RETURN:
 *
 ****************************************************************************/

VOID    
InvertBitmap( LPBYTE pAndMask, LPBYTE pBitmap, UINT cbBitmap )
{
    BYTE c;
    BYTE AndMask;
    UINT i;
    UINT j;
    UINT index;
    LPBYTE  lpBuffer  = (LPBYTE) lpstatic_buffer + MAX_BITMAP_DATA;

    /*
     *  Handle mono and color differently
     */
    if ( cbBitmap == BYTES_128 ) {
    
        for ( i=0; i<cbBitmap; i++ ) {
    
            *(pBitmap+i) = *(pBitmap+i) ^ ~(*(pAndMask+i));
        }
    }
    else if ( cbBitmap == BYTES_512 ) {

        /*
         *  Reverse AND bitmap because DIBs are backwards
         */
        for ( i = 0; i < (UINT) vcyCursor; i++ ) {
            memcpy( (lpBuffer + i * MONO_BYTES_PER_SCANLINE), 
                    (pAndMask + ((vcyCursor - i - 1) * MONO_BYTES_PER_SCANLINE)), 
                    MONO_BYTES_PER_SCANLINE );
        }

        for ( i=0; i<BYTES_128; i++ ) {

            c = *(lpBuffer + i);

            for ( j=0; j<COLOR_BYTES_PER_MONO_BYTE_4BPP; j++ ) {

                AndMask  = (c & 0x80) ? 0xF0 : 0x00;
                c = c << 1;
                AndMask |= (c & 0x80) ? 0x0F : 0x00;
                c = c << 1;

                index = (i * COLOR_BYTES_PER_MONO_BYTE_4BPP) + j;
                *(pBitmap+index) = *(pBitmap+index) ^ ~AndMask;
            }
        }
    }
    else {

        /*
         *  Reverse AND bitmap because DIBs are backwards
         */
        for ( i = 0; i < (UINT) vcyCursor; i++ ) {
            memcpy( (lpBuffer + i * MONO_BYTES_PER_SCANLINE), 
                    (pAndMask + ((vcyCursor - i - 1) * MONO_BYTES_PER_SCANLINE)), 
                    MONO_BYTES_PER_SCANLINE );
        }

        for ( i=0; i<BYTES_128; i++ ) {

            c = *(lpBuffer + i);

            for ( j=0; j<COLOR_BYTES_PER_MONO_BYTE_8BPP; j++ ) {

                AndMask  = (c & 0x80) ? 0xF0 : 0x00;
                c = c << 1;
                AndMask |= (c & 0x80) ? 0x0F : 0x00;
                c = c << 1;

                index = (i * COLOR_BYTES_PER_MONO_BYTE_8BPP) + j;
                *(pBitmap+index) = *(pBitmap+index) ^ ~AndMask;
            }
        }
    }
}
