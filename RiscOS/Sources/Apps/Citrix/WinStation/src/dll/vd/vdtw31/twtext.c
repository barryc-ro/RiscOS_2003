
/******************************Module*Header*******************************\
*
*   TWTEXT.C
*
*   Thin Wire Windows - Text output processing
*
*   Copyright (c) Citrix Systems Inc 1994-1996
*
*   Author: Marc Bloomfield (marcb) 15-Apr-1994
*
*   $Log$
*   Revision 1.3  1998/01/30 19:10:45  smiddle
*   Fixed clipping (as long as its simple), and palettes (mostly) and text.
*   Fixed a few more dodgy alignmenet structures and made some progress
*   towards getting the save/restore screen code working.
*
*   Version 0.04. Tagged as 'WinStation-0_04'
*
*   Revision 1.2  1998/01/27 18:38:59  smiddle
*   Lots more work on Thinwire, resulting in being able to (just) see the
*   log on screen on the test server.
*
*   Version 0.03. Tagged as 'WinStation-0_03'
*
*   Revision 1.1  1998/01/19 19:12:54  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.19   08 Aug 1997 18:16:54   kurtp
*  clipped text optimization
*  
*     Rev 1.18   08 Aug 1997 17:55:52   kurtp
*  clipped text optimization
*  
*     Rev 1.17   08 Aug 1997 16:18:54   kurtp
*  fix non-clipping glyph bug
*  
*     Rev 1.16   07 Aug 1997 22:17:58   kurtp
*  fix glyph bug from dawn of time
*  
*     Rev 1.15   04 Aug 1997 19:14:36   kurtp
*  update
*  
*     Rev 1.12   25 Jul 1997 14:18:50   kurtp
*  optimize transparent textout
*  
*     Rev 1.11   15 Jul 1997 14:40:52   kurtp
*  update
*  
*     Rev 1.10   15 Jul 1997 14:21:04   kurtp
*  update
*  
*     Rev 1.9   14 Jul 1997 18:21:22   kurtp
*  Add LVB to transparent text ops
*  
*     Rev 1.8   08 Jul 1997 17:46:36   kurtp
*  update
*  
*     Rev 1.7   08 Jul 1997 17:40:06   kurtp
*  optimize Opaque TextOut on slow VGA devices
*  
*     Rev 1.6   15 Apr 1997 18:16:28   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   21 Mar 1997 16:09:34   bradp
*  update
*  
*     Rev 1.5   08 May 1996 14:53:44   jeffm
*  update
*  
*     Rev 1.4   03 Jan 1996 13:33:28   kurtp
*  update
*  
\**************************************************************************/

#include <string.h>

#include "wfglobal.h"
#include "twtype.h"
#include "twtext.h"

#include "../../../inc/clib.h"

extern HDC vhdc;


//===========================================================================
//
// NOTE: Changes made to this module may also need to be changed in
//       textout.c in the CITRIX host (thin00.dll).
//===========================================================================


/*=============================================================================
==   Internal Functions and Data
=============================================================================*/

/*
 * Use global variables instead of function parameters to improve speed
 * by reducing CPU usage on the client - it may not be pretty, but it's faster
 */
static TWTOFLAGS    vFlags;
static USHORT       vLastCharInc = 8;
static UCHAR        vfFirstGlyph;
static RECTI        vrclBackground;
static SCHAR        vfrclClip;

static BOOL    vbComplex;  // TRUE if current operation is complex clipping

static TWTOGLYPHDIMENSION vGlyphDimension;

static TWTOGH vGH;         // For performance reasons, this structure is used 
                           // as the working copy of the GlyphHeader - although 
                           // the GlyphHeader bit fields work nicely when 
                           // sending stuff across the wire, it causes the 
                           // client to burn cpu when acessing repeatedly

static TWTOGD vGD;         // For performance reasons, this structure is used
                           // as the working copy of the GlyphDimension

static LPBYTE vpGlyphData;
static USHORT vcbReceived;

#ifdef LATER
static USHORT vhCacheExtra[MAX_2K_CHUNKS_PER_GLYPH];
#endif
#define TWCLR_CURRENT (ULONG)0xFFFFFFFF

static TWTOHCACHECACHE vhCacheCache[128];
static USHORT vdxLast = TWTO_DEF_DX_X;
static USHORT vdyLast = TWTO_DEF_DX_Y; 
static TWTOCOLOR    vLastColor16  = { TWTO_DEF_FG_COLOR_16,
                                      TWTO_DEF_BG_COLOR_16 };
static TWTOCOLOR256 vLastColor256 = { TWTO_DEF_FG_COLOR_256,
                                      TWTO_DEF_BG_COLOR_256 };

void GetNextGlyphData( HDC hDC, BOOL bSetGD );


void   MaskGlyphsToLVB( HDC, LPBYTE, INT, INT, INT, INT );
void   MaskRclExtraToLVB( HDC, INT, INT, INT, INT );
void   MaskLVBToScreen( HDC, BOOL );


/*=============================================================================
==   External Functions and Data
=============================================================================*/

extern int cbTWPackLen;
extern LPBYTE pTWPackBuf;


typedef struct _RECT_ENUM {
   UCHAR c;
   RECTI arcl[TWTO_RCLCLIP_LIMIT];
} RECT_ENUM, FAR * PRECT_ENUM;

#define MAX_BYTES  (LARGE_CACHE_CHUNK_SIZE - SIZE_OF_GLYPHDIMENSION_WITH_WIDTH)   

#ifdef DOS

#define venr static_buffer_2
extern  RECT_ENUM  venr;
PRECT_ENUM vpEnr = (PRECT_ENUM) &venr;

#define vpStart static_buffer_1
#define vpData (vpStart+SIZE_OF_GLYPHDIMENSION_WITH_WIDTH)
extern UCHAR vpStart[MAX_BYTES + SIZE_OF_GLYPHDIMENSION_WITH_WIDTH];
extern int vcbScan;

extern void far ExcludeCursor( RECTI *prclBoundary );
extern void far UnexcludeCursor( void );

#else

/*
 *  For the following we will use lpstatic_buffer space.
 *  Pointers vpEnr and vpStart will both point to the same 
 *  location in lpstatic_buffer, because we read in the 
 *  clipping rects first then select them into the DC.
 *  After that the space is reused for the glyph data.
 */
PRECT_ENUM  vpEnr;
LPBYTE      vpStart;
LPBYTE      vpData;
LPBYTE      vpBitmap;

/* 
 *  Max bitmap can we build before we have to Blt?
 */
#define MAX_BITMAP  (STATIC_BUFFER_SIZE - LARGE_CACHE_CHUNK_SIZE)


/*
 *  The following are used for buffering Glyphs to reduce the number 
 *  of memory to screen BitBlts, performance stuff don't you know!
 */
INT         vLeftGlyphBuf;
INT         vTopGlyphBuf;   
INT         vRightGlyphBuf; 
INT         vBottomGlyphBuf;
BOOL        vfGlyphBuf;
USHORT      vcxGlyphBuf;    
USHORT      vcyGlyphBuf;    
ULONG       vcbGlyphBuf;
HRGN        hrgnDest;

extern      HPALETTE vhPalette;

/*
 *  LVB globals follow
 */

/*
 *  LVB Flush delay
 */
#define UPDATE_DELAY 150

/*
 *  LVB size characteristics
 */
ULONG   vcxLVB;
ULONG   vcxBytes;
ULONG   vcyLVB;
ULONG   vcbLVB;

LPBYTE  vpLVB = NULL;
BOOL    vfLVB = FALSE;

RECT     vRectLVB;
COLORREF vTextColor;
ULONG    vcsNextUpdate = 0;

UCHAR    vMaskBits[9] = { 0x00, 0x01, 0x03, 0x07, 0x0f,
                                0x1f, 0x3f, 0x7f, 0xff };

#endif


/****************************************************************************\
 *
 *  CacheWrite
 *
 *  This routine writes data into a cache slot.  This routine is called when the
 *  glyph is initially put into the cache.  This routine may be called again
 *  to update the glyph widht (dxNext in GlyphDimension) if this value was not
 *  known at the time when the glyph was put into the cache.
 *
 *  PARAMETERS:
 *     USHORT              uscbData        - total size of cache object
 *
 *  RETURN: 
 *     none
 *
\****************************************************************************/

__inline void 
CacheWrite( USHORT uscbData )
{


    LPBYTE lpCacheArea;

    /*
     *  Get pointer to cache area
     */
    lpCacheArea = lpTWCacheWrite( vGH.hCache, vGH.ChunkType, uscbData, vGH.hCache );
    TRACE(( TC_TW, TT_TW_TEXT, "lpTWCacheWrite: hCache %u, ChunkType %u, Bytes %u, lpCacheArea %08x", vGH.hCache, vGH.ChunkType, uscbData, lpCacheArea ));
    ASSERT( lpCacheArea != NULL, 0 );

    /*
     *  Copy data and finish write
     */ 
    if ( lpCacheArea ) {

        //  copy header
        memcpy( lpCacheArea, 
                &vGlyphDimension, 
                SIZE_OF_GLYPHDIMENSION_WITH_WIDTH );
        
        //  copy data, only if different area
        if ( (lpCacheArea + SIZE_OF_GLYPHDIMENSION_WITH_WIDTH) != vpGlyphData ) 
            memcpy((lpCacheArea + SIZE_OF_GLYPHDIMENSION_WITH_WIDTH), 
                    vpGlyphData, 
                   (uscbData   - SIZE_OF_GLYPHDIMENSION_WITH_WIDTH) );

        //  complete write
        finishedTWCacheWrite( uscbData );
    }

}


/****************************************************************************\
 *
 *  GlyphBlt
 *
 *  This routine will output a single glyph to the screen.  The glyph will be
 *  clipped as governed by the specified clipping rectangle.
 *
 *  Note:  This function uses VGA write mode 3, where the color is put in the
 *         set/reset graphics register, 0xFF is in the map mask register and
 *         00 in the color don't care register.  The pels get illuminated on
 *         the screen by ANDing the screen memory with the bit map mask.
 *
 *  PARAMETERS:
 *     none  - all global for performance reasons
 *
 *  RETURN: 
 *     UCHAR       bMore  
 *                  TRUE      - Another glyph follows
 *                  FALSE     - This is the last glyph in the string
 *                                                   
\****************************************************************************/

__inline UCHAR 
GlyphBlt( HDC hDC )
{
    typedef struct {
       UCHAR   low;
       UCHAR   high;
    } BYTES;

static TWTOPOSITION Position;
static TWTOPOSITION vLastStringPos;
static UCHAR     bFirstDisplayedGlyph;
static SHORT     sdeltaY;
static SHORT     xPos = 0, xLast = 0, xBase = 0;
static SHORT     yPos = 0, yBase = 0;

    USHORT       cbWide;
    USHORT       cbTotal;
    SHORT        y;
    USHORT       i, j, k = 0;
    USHORT       cbLeft;
    TWTOGLYPHHEADER GlyphHeader;
    BYTE far    *pCacheEntry;
    USHORT       cbData;
    UCHAR        hCacheCache;
    UCHAR        bCalcPosition;
    UCHAR        fMediumFormat;
    RECTI       *prclClip;
    UCHAR        rotate;
    UCHAR        invrotate;
    UCHAR        fyClip;
    USHORT       LClipBits, RClipBits;
    SHORT        Lx, Rx;
    register union {
                USHORT Word;
                BYTES  Byte;
             }   Data;
    BYTE         bData;



    USHORT      cx;
    USHORT      CX;
    USHORT      cy;
    LPBYTE      p;
    COLORREF    oldBkColor;
    HBITMAP     hbmpOld;
    HBITMAP     hbmpGlyph;

#ifdef LATER
    USHORT       cExtra2KChunks;
#endif

    TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Entry\n" ));

    // Initialize state of first displayed glyph
    // Note: The position is always sent over with the first glyph that is
    //       sent which is not totally clipped
    if ( vfFirstGlyph ) {
       bFirstDisplayedGlyph = TRUE;
    }

    // Initialize glyph data area to our global static buffer
    vpGlyphData = vpData;
    vcbReceived = 0;

    // Set the bCalcPosition flag
    bCalcPosition = !bFirstDisplayedGlyph &&
                    (vFlags.bMonospaced || vFlags.bDefPlacement);

    // Get client cache handle and glyph flags
    GetNextTWCmdBytes( &GlyphHeader, 1 );
    if ( GlyphHeader.bShortFormat ) {
       // Construct GlyphHeader from short format
       hCacheCache =
                 ((PTWTOGLYPHHEADERSHORT)&GlyphHeader)->hCacheCache;

       vGH.bLastGlyph      = FALSE;
       vGH.bGetFromCache   = TRUE;
       vGH.bPutInCache     = FALSE;
       vGH.bWidthIncluded  = FALSE;
    } else {
       fMediumFormat = (UCHAR)GlyphHeader.fMediumFormat;

       // If the glyph is totally clipped, abort
       if ( (fMediumFormat == TWTO_MF_SPACES )      ||
            ( (fMediumFormat != TWTO_MF_MEDIUM ) &&
               GlyphHeader.bTotallyClipped         ) ) {
          DTRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Entire glyph is outside clipped region" ));
          if ( fMediumFormat == TWTO_MF_SPACES ) {
             vGH.bLastGlyph = FALSE;
             xPos += (vLastCharInc *
                      (USHORT)((PTWTOGLYPHHEADERSPACES)&GlyphHeader)->count);
          } else {
             vGH.bLastGlyph = (UCHAR)GlyphHeader.bLastGlyph;
          }
          goto done;
       }

       if ( fMediumFormat ) {
          // Get rest of medium format
          GlyphHeader.hCache = 0; // clear out all 12 bits first
          GetNextTWCmdBytes( ((char *)&GlyphHeader) + 1,
                             SIZE_OF_GLYPHHEADERMEDIUM - 1 );
          switch( fMediumFormat ) {
             case TWTO_MF_TRUNCATE:
                hCacheCache = (UCHAR)GlyphHeader.hCache; // hCache is really hCacheCache
                vGH.bLastGlyph     = (UCHAR)GlyphHeader.bLastGlyph    ;
                vGH.bGetFromCache  = (UCHAR)GlyphHeader.bGetFromCache ;
                vGH.bPutInCache    = (UCHAR)GlyphHeader.bPutInCache   ;
                vGH.bWidthIncluded = (UCHAR)GlyphHeader.bWidthIncluded;
                break;
             case TWTO_MF_MEDIUM:
                {
                   UCHAR  bTiny  = (UCHAR)((PTWTOGLYPHHEADERMEDIUM)&GlyphHeader)->bTiny;
                   USHORT hCache = ((PTWTOGLYPHHEADERMEDIUM)&GlyphHeader)->hCache;

                   vGH.bLastGlyph     = FALSE; 
                   vGH.bGetFromCache  = TRUE; 
                   vGH.bPutInCache    = FALSE; 
                   vGH.bWidthIncluded = FALSE; 
                   vGH.hCache         = hCache;
                   vGH.ChunkType      = bTiny ? _32B : _128B;
                }
                break;
          }
       } else {
          // Get rest of normal format
          GetNextTWCmdBytes( ((char *)&GlyphHeader) + 1,
                             SIZE_OF_GLYPHHEADER - 1 );

          vGH.bLastGlyph     = (UCHAR)GlyphHeader.bLastGlyph    ;
          vGH.bGetFromCache  = (UCHAR)GlyphHeader.bGetFromCache ;
          vGH.bPutInCache    = (UCHAR)GlyphHeader.bPutInCache   ;
          vGH.bWidthIncluded = (UCHAR)GlyphHeader.bWidthIncluded;
          vGH.hCache         = (USHORT)GlyphHeader.hCache;
          vGH.ChunkType      = (CHUNK_TYPE)GlyphHeader.ChunkType; 
         
          // Add the cache handle to the cache handle cache 
          if ( GlyphHeader.bhCacheCache ) {
             GetNextTWCmdBytes( &hCacheCache, 1 );
             TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received hCacheCache(%02X)",
                     hCacheCache ));                              
         
             vhCacheCache[hCacheCache].hCache    = (USHORT)vGH.hCache;
             vhCacheCache[hCacheCache].ChunkType = (USHORT)vGH.ChunkType; 
          }
       }

    }

    // Get the hCache and ChunkType from the hCacheCache table
    if ( GlyphHeader.bShortFormat ||
         (fMediumFormat == TWTO_MF_TRUNCATE) ) {
       TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Building GlyphHeader from hCacheCache(%02X) table",
               hCacheCache ));                              
       vGH.ChunkType = vhCacheCache[hCacheCache].ChunkType; 
       vGH.hCache    = vhCacheCache[hCacheCache].hCache;
    }

#ifdef DEBUG
    if ( GlyphHeader.bShortFormat ) {
       TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received Short format GlyphHeader:" ));
       DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)&GlyphHeader, 
                                      (ULONG)sizeof_TWTOGLYPHHEADERSHORT ));
    } else {
       switch ( fMediumFormat ) {
          case TWTO_MF_TRUNCATE:
             TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received Truncate format GlyphHeader:" )); 
             DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)&GlyphHeader, 
                                           (ULONG)SIZE_OF_GLYPHHEADERMEDIUM ));
             break;

          case TWTO_MF_MEDIUM:
             TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received Medium format GlyphHeader:" )); 
             TRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)&GlyphHeader, 
                                     (ULONG)SIZE_OF_GLYPHHEADERMEDIUM ));
             break;

          case TWTO_MF_NORMAL:
             TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received Normal format GlyphHeader:" )); 
             DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)&GlyphHeader, 
                                     (ULONG)SIZE_OF_GLYPHHEADER ));
             break;

          default:
             TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Invalid fMediumFormat(%d)",
                     (int)GlyphHeader.fMediumFormat ));
             break;
    
       }
    } 
    TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received - hCache(%04X) fPutInCache(%d) fGetFromCache(%d)",
                                   (int)GlyphHeader.hCache,
                                   (int)GlyphHeader.bPutInCache,
                                   (int)GlyphHeader.bGetFromCache ));
    TRACE(( TC_TW, TT_TW_TEXT, "fTotallyClipped(%d) fLastGlyph(%d) ChunkType(%d) fCalcPosition(%d)",
                                   (int)GlyphHeader.bTotallyClipped,
                                   (int)GlyphHeader.bLastGlyph,
                                   (int)GlyphHeader.ChunkType,
                                   (int)bCalcPosition ));

    if ( !fMediumFormat && GlyphHeader.bhCacheCache ) {
       TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received hCacheCache(%02X)",
               hCacheCache ));                              
    }
#endif

    TRACE(( TC_TW, CTXDBG_TEXT,
       "DrvTextOut: sdelta(%d) vpos(%d,%d)",
       (int)sdeltaY, (int)vLastStringPos.x, (int)vLastStringPos.y  ));

    // Get raw position
    if ( !bCalcPosition ) {
       {
          SHORT xPrevious = (SHORT)Position.x;

          switch ( vFlags.fFirstPos ) {
             case TWTO_POS_FULL:
                GetNextTWCmdBytes( &Position, 1 );
                if ( Position.bShortFormat ) {
                   {
                      int delta = (int)((PTWTOPOSITIONSHORT)&Position)->delta;
      
                      Position.x = xPrevious + vGD.cx + delta;
                      TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received - Short Position (%d, %d) delta(%d)",
                          (int)Position.x, (int)Position.y, delta  ));
                   }
                } else {
                   GetNextTWCmdBytes( ((char *)&Position) + 1, SIZE_OF_POSITION - 1);
                   TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received - Normal Position (%d, %d)",
                       (int)Position.x, (int)Position.y  ));
                   if ( bFirstDisplayedGlyph ) {
                      sdeltaY = (SHORT)Position.y - (SHORT)vLastStringPos.y;
                   }
                }
                if ( bFirstDisplayedGlyph ) {
                   memcpy( &vLastStringPos, &Position, SIZE_OF_POSITION );
                }
                break;

             case TWTO_POS_SAMEY1DX:
                {
                   SCHAR deltaPos;

                   memcpy( &Position, &vLastStringPos, SIZE_OF_POSITION );
                   GetNextTWCmdBytes( &deltaPos, sizeof(deltaPos) );
                   TRACE(( TC_TW, CTXDBG_TEXT,
                      "DrvTextOut: Received %d bytes deltPos - %d: (%d,%d)", 
                      (int)sizeof(deltaPos), (int)deltaPos,
                      (int)Position.x, (int)Position.y  ));
   
                   Position.x += deltaPos;
                }
                memcpy( &vLastStringPos, &Position, SIZE_OF_POSITION );
                break;

             case TWTO_POS_SAME:
                memcpy( &Position, &vLastStringPos, SIZE_OF_POSITION );
                TRACE(( TC_TW, CTXDBG_TEXT,
                   "DrvTextOut: Using same position as last string: (%d,%d)",
                   (int)Position.x, (int)Position.y  ));
                break;

             case TWTO_POS_SAMEXDY:
                memcpy( &Position, &vLastStringPos, SIZE_OF_POSITION );
                TRACE(( TC_TW, CTXDBG_TEXT,
                   "DrvTextOut: Using same position as last string with dy(%d): (%d,%d)",
                   (int)sdeltaY,
                   (int)Position.x, (int)Position.y  ));
                Position.y += (LONG)sdeltaY;
                memcpy( &vLastStringPos, &Position, SIZE_OF_POSITION );
                TRACE(( TC_TW, CTXDBG_TEXT,
                   "DrvTextOut: Current position dy(%d): (%d,%d) vpos(%d,%d)",
                   (int)sdeltaY,
                   (int)Position.x,       (int)Position.y,
                   (int)vLastStringPos.x, (int)vLastStringPos.y  ));
                break;

          }
       }
    }

    if ( bFirstDisplayedGlyph ) {
       bFirstDisplayedGlyph = FALSE;
       vFlags.fFirstPos = TWTO_POS_FULL;
    }

    // For _32B objects, use the cache entry buffer instead of the normal
    // global static buffer
    if ( (USHORT)vGH.bGetFromCache || (USHORT)vGH.bPutInCache ) {
       if ( (CHUNK_TYPE)vGH.ChunkType == _32B ) {
          // <= 32 byte object are cached in low memory using a 
          // directly addressable address space
          pCacheEntry = lp32ByteObject(vGH.hCache);

          DTRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: pCacheEntry(%08lX)", pCacheEntry ));
          vpGlyphData = pCacheEntry+SIZE_OF_GLYPHDIMENSION_WITH_WIDTH;
       }
    }


    if ( vGH.bGetFromCache ) {
       if ( (CHUNK_TYPE)vGH.ChunkType == _32B ) {
         memcpy( &vGlyphDimension, pCacheEntry, SIZE_OF_GLYPHDIMENSION_WITH_WIDTH );
         vGD.cx             = (USHORT)vGlyphDimension.cx;
         vGD.cy             = (USHORT)vGlyphDimension.cy;
         vGD.dyThis         = (SHORT)vGlyphDimension.dyThis;
         vGD.dxThis         = (SHORT)vGlyphDimension.dxThis;
       } else {
          // Read in first block - GlyphDimension will be retrieved from cache
          GetNextGlyphData( hDC, TRUE );
       }

       DTRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received %u of %u bytes (%u left):", cbReceived, cbTotal, cbLeft ));                              
       DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)vpGlyphData, (ULONG)vcbReceived ));
       
       TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Retrieved GlyphDimension from cache(%d X %d), data:",
                 (USHORT)vGlyphDimension.cx, (USHORT)vGlyphDimension.cy ));
       TRACEBUF(( TC_TW, TT_TW_TEXT, &vGlyphDimension, (ULONG)SIZE_OF_GLYPHDIMENSION_WITH_WIDTH ));

       // If glyph width was not stored when the glyph was initially cached,
       // get the width now and update the cache
       if ( vGH.bWidthIncluded ) {
          GetNextTWCmdBytes( &vGD.dxNext, sizeof(vGD.dxNext) );
          vGlyphDimension.dxNext = vGD.dxNext;
          TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received dxNext(%d)",
                    (int)vGlyphDimension.dxNext ));

          // Now update the cache 
          if ( (CHUNK_TYPE)vGH.ChunkType == _32B ) {
             ((PTWTOGLYPHDIMENSION)pCacheEntry)->dxNext = vGD.dxNext;
          } else {
             ASSERT( vcbReceived, vcbReceived );
             CacheWrite( (USHORT) (vcbReceived + SIZE_OF_GLYPHDIMENSION_WITH_WIDTH) );
          }
       } else {
          vGD.dxNext = (USHORT)vGlyphDimension.dxNext;
       }

       // Figure out how many bytes wide the glyph is and how many total bytes
       // there are in the glyph
       cbWide  = (((vGD.cx - 1) / 8) + 1); 
       cbTotal = cbWide * vGD.cy; 
       cbLeft  = cbTotal - vcbReceived;
    } else {
       // Get GlyphDimension
       GetNextTWCmdBytes( &vGlyphDimension,
                          vGH.bWidthIncluded ?
                             SIZE_OF_GLYPHDIMENSION_WITH_WIDTH : 
                             SIZE_OF_GLYPHDIMENSION );
       vGD.cx             = (USHORT)vGlyphDimension.cx;
       vGD.cy             = (USHORT)vGlyphDimension.cy;
       vGD.dyThis         = (SHORT)vGlyphDimension.dyThis;
       vGD.dxThis         = (SHORT)vGlyphDimension.dxThis;

       if ( vFlags.bCalcWidth ) {
          vGD.dxNext = vGD.cx; 
          if ( vGlyphDimension.bColumnRemoved ) {
              vGD.dxNext++;
          }
          vGlyphDimension.dxNext = vGD.dxNext; 
       } else {
          vGD.dxNext = (USHORT)vGlyphDimension.dxNext;
       }
       TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received - GlyphDimension(%d X %d), data:",
                 (USHORT)vGlyphDimension.cx, (USHORT)vGlyphDimension.cy ));
       TRACEBUF(( TC_TW, TT_TW_TEXT, &vGlyphDimension,
                                  (ULONG)SIZE_OF_GLYPHDIMENSION_WITH_WIDTH ));

       // Figure out how many bytes wide the glyph is and how many total bytes
       // there are in the glyph
       cbWide  = (((vGD.cx - 1) / 8) + 1); 
       cbTotal = cbWide * vGD.cy; 
       cbLeft  = cbTotal;

       // For 32B objects cached in low memory, we want to cache the 
       // GlyphDimensions here and now.
       if ( vGH.ChunkType == _32B ) {
          if ( vGH.bPutInCache ) {
              // Cache the GlyphDimension now
              memcpy( pCacheEntry, &vGlyphDimension, SIZE_OF_GLYPHDIMENSION_WITH_WIDTH );
          }
       }
    }

    // Determine the working position of the glyph
    if ( bCalcPosition ) {
       if ( vFlags.bMonospaced ) {
          // Monospaced - add the standard increment to the last position
          xPos += vLastCharInc;
       } else {
          // Proportional - calculate the new position
          xBase += xLast;
       }
       yPos = yBase;
    } else {
       xPos = (SHORT)Position.x;   
       yPos = (SHORT)Position.y;   
       yBase = yPos;
       xBase = xPos;
    }

    if ( !vFlags.bMonospaced ) {
       TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: xLast(%d) xBase(%d) dxThis(%d) dxNext(%d)",
                 (int)xLast, (int)xBase,
                 (int)vGlyphDimension.dxThis, (int)vGlyphDimension.dxNext ));
       xLast = (SHORT)vGD.dxNext;
       xPos = xBase + (SHORT)vGD.dxThis;
    }

    // Adjust glyph position based on blank scanlines removed from the top,
    // and the current glyph origin
    yPos += (SHORT)vGD.dyThis;

    y = yPos;

    TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: cx(%u) cy(%u) xLeft(%d) yTop(%d) yBase(%d) dyThis(%d)",
                         (USHORT)vGlyphDimension.cx, (USHORT)vGlyphDimension.cy,
                         Position.x, Position.y, yBase,
                         (int)vGlyphDimension.dyThis ));
    TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: x(%d) y(%d) cbWide(%u) cbTotal(%u)",
                         xPos,     yPos,     cbWide,    cbTotal ));



    // Get the other cache handle(s) (if any)
    if ( (USHORT)vGH.bGetFromCache || (USHORT)vGH.bPutInCache ) {
      
       // Figure out the chunk type for caching
       cbData         = cbTotal + SIZE_OF_GLYPHDIMENSION_WITH_WIDTH;

#ifdef LATER
       cExtra2KChunks = 0;
       if ( (CHUNK_TYPE)vGH.ChunkType == _2K ) {
          cExtra2KChunks = ((USHORT)cbData-1)/2048;
      
          // Now get the chained handles
          if ( cExtra2KChunks && vGH.bPutInCache ) {
             {
                USHORT i;
         
                for ( i = 0; i < cExtra2KChunks; i++ ) {
                   GetNextTWCmdBytes( &vhCacheExtra[i], SIZE_OF_HCACHE_EXTRA );
                   vhCacheExtra[i] &= HCACHE_VALID_BITS;
                   DTRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: hCache[%d] (%04X)",
                           i, vhCacheExtra[i] ));
                }
             }
          }
       }
#endif
    }

    DTRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: pData(%08lX) vpData(%08lX) vpStart(%08lX)",
               (char far *)vpGlyphData, (char far *)vpData, (char far *)vpStart ));

    /*
     *  Buffered glyph?
     */
    if ( vfGlyphBuf ) {

        LPBYTE pScreenStart;
        LPBYTE pScreen = vpBitmap;
        SHORT  xPosBuf = xPos - vLeftGlyphBuf;
        SHORT  yPosBuf = yPos - vTopGlyphBuf;
        INT    cbScan  = (INT) vcxGlyphBuf / 8;
        UCHAR  antimatter;

        TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: xPos %d, yPos %d, xPosBuf %d, yPosBuf %d, cbScan %u, pScreen %08x, k %u",
                    xPos, yPos, xPosBuf, yPosBuf, cbScan, pScreen, k ));

        /*
         *  Figure out how much the data needs to be rotated in order
         *  to write to the closest memory byte boundary
         */
        if ( xPosBuf < 0 ) {
            rotate = (UCHAR)( (8 - (-xPosBuf % 8)) % 8 );
            pScreen += (((xPosBuf+1)/8) + (yPosBuf * cbScan) - 1);
        } else {
            rotate = (UCHAR)(xPosBuf % 8);
            pScreen += ((xPosBuf/8) + (yPosBuf * cbScan));
        }
        invrotate = (UCHAR)8 - rotate;
        antimatter = vMaskBits[invrotate];
        TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Rotate %d bits", rotate ));

        /*
         *  Make background rect clipping for this case
         */
        prclClip = &vrclBackground;

        /*
         * Blt the glyph to the memory buffer
         */
        for ( i = vGD.cy; i; y++, i-- ) { 

            /*
             *  Get starting point of this scan line
             */
            pScreenStart = pScreen;
       
            /*
             *  Need to read more data?
             */
            if ( !vcbReceived ) {
               // Get Bmp data
               vcbReceived = MIN( cbLeft, MAX_BYTES );  // limit by buffer size
               vcbReceived -= (vcbReceived % cbWide);    // scan-line align
               cbLeft -= vcbReceived;
               GetNextGlyphData( hDC, FALSE );
               k = 0;
    
               TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Received %u of %u bytes (%u left):", vcbReceived, cbTotal, cbLeft ));                              
               DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)vpGlyphData, (ULONG)vcbReceived ));
            }
        
            /*
             *  Special clipping case
             */
            if ( vfrclClip || vbComplex ) {

                /*
                 * Only draw the glyph row if it is inside the clipped region
                 */
                fyClip =  (UCHAR)( (y >= (SHORT)prclClip->bottom) ||
                                   (y <  (SHORT)prclClip->top) );
        
                /*
                 *  Scan line is totally clipped
                 */
                if ( fyClip ) {
                    k += cbWide;
                }

                // Output row of glyph, taking into consideration rotating bits
                // relative to a screen byte boundary
                else {
    
                    for ( Data.Word = 0xffff, j = cbWide, Lx=xPos, Rx=xPos+8-1; j;
                          j--, Rx+=8,Lx+=8 ) { // For each byte wide...
        
                        Data.Word <<= rotate;
             
                        TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: j(%u) k(%u) p = (%02X) cbWide(%u)", j, k, ~vpGlyphData[k], cbWide ));
            
                        bData = vpGlyphData[k]; // Get current byte
            
                        // Adjust the data for left and right clipping
                        if ( Lx < (SHORT)prclClip->left ) {
                            LClipBits     = (USHORT)prclClip->left - (USHORT)Lx;
                            if ( LClipBits < 8 ) {
                                TRACE(( TC_TW, TT_TW_TEXT, "ClipLeft: Stripping left %d bits from p[%d](%02X)",
                                                             LClipBits, k, vpGlyphData[k]));
                                bData |= ~vMaskBits[8-LClipBits]; 
                            } else {
                                TRACE(( TC_TW, TT_TW_TEXT, "ClipLeft: Not blting p[%d](%02X)",
                                                             k, vpGlyphData[k]));
                                bData = 0xff;
                            } 
                        } 
            
                        // May need to clip right instead of or in addition to clip left
                        if ( Rx >= (SHORT)prclClip->right ) {
            
                            RClipBits = (USHORT)Rx - (USHORT)prclClip->right + 1;
                            if ( RClipBits < 8 ) {
                                TRACE(( TC_TW, TT_TW_TEXT, "ClipRight: Stripping right %d bits from p[%d](%02X)",
                                                             RClipBits, k, vpGlyphData[k]));
                                bData |= vMaskBits[RClipBits]; 
                            } else {
                                TRACE(( TC_TW, TT_TW_TEXT, "ClipRight: Not blting p[%d](%02X)",
                                                             k, vpGlyphData[k]));
                                bData = 0xff;
                            } 
                        }
        
                        Data.Byte.low = bData; 
                        k++;
                    
                        Data.Word <<= invrotate;
                        Data.Word |= antimatter;
    
                        if ( (Lx < 0) || (Data.Byte.high == 0xff) ) {
                            pScreen++;
                        } else {
                            *pScreen++ &= Data.Byte.high;
                        }
                    } 
        
                    if ( (Lx < 0) || (Data.Byte.low == 0xff) ) {
                        pScreen++;
                    } else {
                        *pScreen++ &= Data.Byte.low;
                    }
                }

            }

            /*
             *  Optimized non clipping case
             */
            else
            {

                /*
                 * This routine is rewritten in asm, below, to improve performance
                 */
                
                for ( Data.Word = 0xffff, j = cbWide; j; j-- ) { // For each byte wide...
     
                    Data.Word <<= rotate;
                    Data.Byte.low = vpGlyphData[k++];
                 
                    Data.Word <<= invrotate;
                    *pScreen++ &= Data.Byte.high;

                    Data.Word |= antimatter;
                } 
                *pScreen++ &= Data.Byte.low;
            }

            vcbReceived -= cbWide;     // Indicate scan line processed
            pScreen = (pScreenStart + cbScan);
        }
    }

    /*
     *  Unbuffered glyph
     */
    else {
    
        /*
         *  Need to flush out transparent glyphs now
         */
        MaskLVBToScreen( hDC, TRUE );

        /*
         *  Glyph may come in in more than one chunk
         */
        do { 
        
            /*
             *  Get data from stream or cache if not already read
             */
            if ( !vcbReceived ) {
                vcbReceived = MIN( cbLeft, MAX_BYTES );  // limit by buffer size
                vcbReceived -= (vcbReceived % cbWide);    // scan-line align
                TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: %u of %u", vcbReceived, cbLeft ));
                cbLeft -= vcbReceived;
                GetNextGlyphData( hDC, FALSE );
                cy = vcbReceived / cbWide;
            }
            else {
                cbLeft = 0;
                cy = vGD.cy;
            }
            
            /*
             *  Create padded bitmap
             */
            cx = (((vGD.cx - 1) / 16) + 1) * 2;  //  bytes for target
            CX =  ((vGD.cx - 1) /  8) + 1;       //  bytes for source (over estimate)
            for ( i=0; i<cy; i++ ) {
        
                /*
                 *  Opti!
                 */
                p = (vpBitmap + (i * cx));
        
                /*
                 *  Pad to WORD boundary
                 */
                memset( p, 0, cx );
                memcpy( p, (vpGlyphData + (i * CX)), CX );
        
            }
        
            /*
             *  Create bitmap
             */
            hbmpGlyph = CreateBitmap( (cx * 8), cy, 1, 1, vpBitmap );
        
            /*
             *  Select into compatible DC
             */
            hbmpOld = SelectObject( compatDC, hbmpGlyph );
        
            /*
             *  Blt to screen
             */
            if ( vFlags.bOpaqueBackground ) {
        
                TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Opaque" ));
        
                BitBlt( hDC, xPos, yPos, vGD.cx, cy, compatDC, 0, 0, SRCCOPY  );
            }
            else {
        
                TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Transparent" ));
            
                BitBlt( hDC, xPos, yPos, vGD.cx, cy, compatDC, 0, 0, SRCAND );
                oldBkColor = SetBkColor( hDC, ((vColor == Color_Cap_256) ? 
                                                PALETTEINDEX(0) : 
                                                RGB( 0, 0, 0 )) );
                BitBlt( hDC, xPos, yPos, vGD.cx, cy, compatDC, 0, 0, SRCPAINT );
                SetBkColor( hDC, oldBkColor );
            }
        
            TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: BitBlt - Bitmap(%d X %d) at (%d,%d)",
                                       (int)vGD.cx, (int)cy, (int)xPos, (int)yPos ));
        
            /*
             *  Delete bitmap
             */
            SelectObject( compatDC, hbmpOld );
            DeleteObject( hbmpGlyph );
    
            /*
             *  Update yPos to reflect partial bitmap xfer
             */
            yPos += cy;
    
            /*
             *  Again!
             */
            vcbReceived = 0;
    
        }  while ( cbLeft );
    }

done:

   return( (UCHAR)!vGH.bLastGlyph );
}


/****************************************************************************\
 *
 *  FillBackground
 *
 *  This routine will fill a rect with the specified solid color.  It is called
 *  for the opaque text background and also for the extra rects used for 
 *  underline and strikeout.
 *
 *  Note:  This function uses VGA write mode 3, where the color is put in the
 *         set/reset graphics register, 0xFF is in the map mask register and
 *         00 in the color don't care register.  The pels get illuminated on
 *         the screen by ANDing the screen memory with the bit map mask.
 *
 *  PARAMETERS:
 *     ULONG  Color - color to fill it with
 *
 *  RETURN: 
 *     none
 *
\****************************************************************************/

__inline void 
FillBackground( HDC hDC, ULONG Color )
{
    /*
     *  Fill rectangle with color requested
     */
    if ( vColor == Color_Cap_256 ) {
        COLORREF crBrush = PALETTEINDEX((Color == TWCLR_CURRENT) ? vLastColor256.FG : Color);
        HBRUSH   hBrush;

        hBrush = CreateSolidBrush(crBrush);
        FillRect( hDC, (RECT FAR *) &vrclBackground, hBrush );
        DeleteObject( hBrush );
    }
    else {
        FillRect( hDC, (RECT FAR *) &vrclBackground, 
                  hbrsolid[ (Color == TWCLR_CURRENT) ? vLastColor16.FG : Color ] );
    }
}


#ifndef DOS
/****************************************************************************\
 *
 *  SelectVisibleRegionIntoDC
 *
 *  This routine will create a region from all the clipping rectangles
 *  and then selected it into the global DC.
 *
 *  PARAMETERS: none
 *
 *  RETURN:     none
 *
\****************************************************************************/

__inline void 
SelectVisibleRegionIntoDC( HDC hDC )
{
    int  cRcl;
    HRGN hrgnSrc1 = NULL;
    HRGN hrgnSrc2 = NULL;

    /*
     *  Empty by default 
     */
    hrgnDest = NULL;

    /*
     *  Complex clipping region?
     */
    if ( vbComplex ) {

        /*
         *  Setup the rect count
         */
        cRcl = vpEnr->c - 1;

        /*
         *  Combine all rects into one region
         */
        do {
         
            /*
             *  Create latest region
             */
            hrgnSrc2 = CreateRectRgnIndirect( (RECT FAR *)  &(vpEnr->arcl[cRcl]) );

            /*
             *  Combine if necessary
             */
            if ( (hrgnSrc1 != NULL) && (hrgnSrc2 != NULL) ) {

                /*
                 *  Create an empty region to combine the other two into
                 */
                hrgnDest = CreateRectRgn( 0, 0, 0, 0 );

                /*
                 *  Combine the regions
                 */
                CombineRgn( hrgnDest, hrgnSrc1, hrgnSrc2, RGN_OR );

                /*
                 *  Free the parts
                 */
                DeleteObject( hrgnSrc1 );
                DeleteObject( hrgnSrc2 );

                /*
                 *  Make combined region src1
                 */
                hrgnSrc1 = hrgnDest;

            }
            else {

                /*
                 *  Slide second region to first
                 */
                hrgnSrc1 = hrgnSrc2;
            }

        } while ( cRcl-- );

        /* 
         *  Assign dest region
         */
        hrgnDest = hrgnSrc1;

    }
    else {

        /*
         *  Create the simple region
         */
        hrgnDest = CreateRectRgnIndirect( (RECT FAR *) &vrclBackground );

    }

    /*
     *  Do we have a region to select?
     */
    if ( hrgnDest != NULL ) {

        /*
         *  Select the clipping region
         */
        SelectClipRgn( hDC, hrgnDest );
    }
}


/****************************************************************************\
 *
 *  CreateGlyphBuffer
 *
 *  This routine will try and use the pBitmap area for a GlyphBlt
 *  buffer to reduce the number of BitBlts by building up a series
 *  of Glyphs into a buffer before Blting them to the screen.
 *
 *  PARAMETERS: none
 *
 *  RETURN:     none
 *
\****************************************************************************/

__inline void 
CreateGlyphBuffer()
{
    INT Size;

    /*
     *  Create BYTE aligned background rect for faster BitBlts
     */
    vLeftGlyphBuf   = ((vrclBackground.left / 8)) * 8;
    vTopGlyphBuf    = vrclBackground.top;
    vRightGlyphBuf  = ((vrclBackground.right / 8) + 1) * 8; 
    vBottomGlyphBuf = vrclBackground.bottom;

    /*
     *  Make it WORD aligned for CreateBitmap
     */
    if ( (Size = (vRightGlyphBuf - vLeftGlyphBuf + 1) % 16) ) {
        vRightGlyphBuf += (16 - Size);
    }

    TRACE(( TC_TW, TT_TW_TEXT, "CreateGlyphBuffer: rect left %u, top %u, right %u, bottom %u",
                                vLeftGlyphBuf,  
                                vTopGlyphBuf,   
                                vRightGlyphBuf, 
                                vBottomGlyphBuf ));

    /*
     *  Set up globals
     */
    vfGlyphBuf  = FALSE;
    vcxGlyphBuf = (USHORT) (vRightGlyphBuf - vLeftGlyphBuf + 1);
    vcyGlyphBuf = (USHORT) (vBottomGlyphBuf - vTopGlyphBuf + 1);
    vcbGlyphBuf = (USHORT) (((ULONG) vcxGlyphBuf * (ULONG) vcyGlyphBuf) / (ULONG) 8L);

    /*
     *  Do we have room in vpBitmap area, then set it up
     */
    if ( vcbGlyphBuf < MAX_BITMAP ) {
        vfGlyphBuf = TRUE;
        memset( vpBitmap, 0xFF, (INT) vcbGlyphBuf );
        TRACE(( TC_TW, TT_TW_TEXT, "CreateGlyphBuffer: success" ));
    }
    else {
        TRACE(( TC_TW, TT_TW_TEXT, "CreateGlyphBuffer: faliure" ));
    }

    TRACE(( TC_TW, TT_TW_TEXT, "CreateGlyphBuffer: vcbGlyphBuf %u, vfGlyphBuf %04x", vcbGlyphBuf, vfGlyphBuf ));
}
#endif


/****************************************************************************\
 *  w_TWCmdTextOut
 *
 *  This is the worker routine for all variations of the DrvTextOut
 *  services.
 *
 *  PARAMETERS:
 *     USHORT Options
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void 
w_TWCmdTextOut( HWND hWnd, HDC hDC, USHORT Options )
{
    USHORT       rc = 0;
    UCHAR        bMore;
    TWTOPT1RCL   pt1Rcl;
    ULONG        pt2Rcl;
    ULONG *      ppt2Rcl = &pt2Rcl;
    USHORT       uscbData;
    SCHAR        deltaRcl;

    LPBYTE p = (LPBYTE) lpstatic_buffer;

    /*
     *  vpEnr and vpStart point to same spot because we use
     *  vpEnr first, and then reuse space for glpyh data
     */
    vpEnr    = (PRECT_ENUM) p;
    vpStart  = p;
    vpData   = (p + SIZE_OF_GLYPHDIMENSION_WITH_WIDTH);
    vpBitmap = (p + LARGE_CACHE_CHUNK_SIZE);

    vfFirstGlyph = TRUE;

    DTRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_TEXT, "TWCmdTextOut: Entered" ));

    vbComplex = ( Options & TWTO_CMPLXCLIP );

    // Get Flags
    GetNextTWCmdBytes( &vFlags, sizeof_TWTOFLAGS );
    TRACE(( TC_TW, TT_TW_TEXT,
            "TWCmdTextOut: Received - Flags(%02X)",
                                      *((UCHAR *)&vFlags) ));
    
    // Get Color
    if ( !vFlags.bSameColor ) {
       if ( vColor == Color_Cap_256 ) {
          GetNextTWCmdBytes( &vLastColor256, sizeof_TWTOCOLOR256 );
          TRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: Received - Color(%04X)",
                                     *((ULONG *)&vLastColor256) ));
       }
       else
       {
          GetNextTWCmdBytes( &vLastColor16, sizeof_TWTOCOLOR );
          TRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: Received - Color(%02X)",
                                     *((UCHAR *)&vLastColor16) ));
       }
    }
#ifdef DEBUG
    else {
       if ( vColor == Color_Cap_256 ) {
           DTRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: Using same color(%04X)",
                                       *((ULONG *)&vLastColor256) ));
       }
       else 
       {
           DTRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: Using same color(%02X)",
                                       *((UCHAR *)&vLastColor16) ));
       }
    }
#endif

    // Get CharInc
    if ( vFlags.bMonospaced && !vFlags.bSameCharInc ) {
       GetNextTWCmdBytes( &vLastCharInc, sizeof( vLastCharInc ) );
       TRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: Received - CharInc(%d)", vLastCharInc ));
    }
#ifdef DEBUG
    else if ( vFlags.bMonospaced ) {
       DTRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: Using same usCharInc(%04X)",
                                         vLastCharInc ));
    }
#endif

    // Get background rectangle (potentially already clipped) 

    if ( vFlags.bRclDelta ) {
       GetNextTWCmdBytes( &deltaRcl, sizeof(deltaRcl) );
       vrclBackground.right += (SHORT)deltaRcl;

       TRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: rclDelta(%d) - rclBackground l:%d t:%d r:%d b:%d",
                  (int)deltaRcl,
                  vrclBackground.left, vrclBackground.top,
                  vrclBackground.right, vrclBackground.bottom ));
    } else {
       // Get pt1Rcl
       DTRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: Receiving compressed rclBackground:" ));
       GetNextTWCmdBytes( &pt1Rcl, 3 );
       DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)&pt1Rcl, 3UL ));
      
       // Get size of pt2Rcl
       GETNEXTPTSIZE( pt1Rcl, &uscbData );
       GetNextTWCmdBytes( ppt2Rcl, (int)uscbData );
       DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)ppt2Rcl, (ULONG)uscbData ));
      
       // Put it all together
       GETTEXTPTDATA( pt1Rcl, vrclBackground, ppt2Rcl, vdxLast, vdyLast );

       vrclBackground.right++;        // Change LRH back to 1-based 1
       vrclBackground.bottom++;                   
       TRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOut: Decompressed - rclBackground l:%d t:%d r:%d b:%d",
                  vrclBackground.left, vrclBackground.top,
                  vrclBackground.right, vrclBackground.bottom ));
    }

    if ( vbComplex ) {
       GetNextTWCmdBytes( vpEnr, sizeof(vpEnr->c) );
       TRACE(( TC_TW, TT_TW_TEXT,
       "TWCmdTextOut: ComplexClip getting %d rcls", vpEnr->c ));

       // Now for the tricky part - receive the compressed rectangles and
       // decompress them into the static buffer
       {
          UCHAR i, bSideBySide;
          UCHAR const c = vpEnr->c;
          SHORT dL, dT, dR, dB;
          TW12L11T12R11B ccRcl; // Allocate buffer of largest size

          for ( i = 0; i < c; i++ ) {
             // We know we need at least sizeof(TW4L3T4R3B) bytes, so lets get them now
             GetNextTWCmdBytes( &ccRcl, sizeof_TW4L3T4R3B );
             TRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)&ccRcl,
                       (ULONG)sizeof_TW4L3T4R3B ));

             bSideBySide = FALSE;
             switch ( ccRcl.fType & 3 ) {  // compiler wierdness, again
                case TWTO_RCLCLIP_4L3T4R3B:
                   dL = ((PTW4L3T4R3B)&ccRcl)->dL;
                   dT = ((PTW4L3T4R3B)&ccRcl)->dT;
                   dR = ((PTW4L3T4R3B)&ccRcl)->dR;
                   dB = ((PTW4L3T4R3B)&ccRcl)->dB;
                   TRACE(( TC_TW, TT_TW_TEXT,
                   "TWCmdTextOut: ComplexClip rcl[%d] 2 BYTES dL:4 dT:3 dR:4 dB:3", i ));
                   break;
                case TWTO_RCLCLIP_6L5T6R5B:
                   // Now get the third (last) byte
                   GetNextTWCmdBytes( (char *)&ccRcl + sizeof_TW4L3T4R3B,
                                      SIZE_OF_TW6L5T6R5B - sizeof_TW4L3T4R3B );
                   TRACEBUF(( TC_TW, TT_TW_TEXT,
                              (char far *)&ccRcl + sizeof_TW4L3T4R3B,
                              (ULONG)SIZE_OF_TW6L5T6R5B - sizeof_TW4L3T4R3B ));
                   dL = (SHORT)((PTW6L5T6R5B)&ccRcl)->dL;
                   dT = (SHORT)((PTW6L5T6R5B)&ccRcl)->dT;
                   dR = (SHORT)((PTW6L5T6R5B)&ccRcl)->dR;
                   dB = (SHORT)((PTW6L5T6R5B)&ccRcl)->dB;
                   TRACE(( TC_TW, TT_TW_TEXT,
                   "TWCmdTextOut: ComplexClip rcl[%d] 3 BYTES dL:6 dT:5 dR:6 dB:5", i ));
                   break;
#ifdef OPTIMIZE_CLIPPED_ENDS
                case TWTO_RCLCLIP_8L7T8R7B:
                   // Now get the last two bytes
                   GetNextTWCmdBytes( (char *)&ccRcl + sizeof_TW4L3T4R3B,
                                      sizeof_TW8L7T8R7B - sizeof_TW4L3T4R3B );
                   TRACEBUF(( TC_TW, TT_TW_TEXT, 
                              (char far *)&ccRcl + sizeof_TW4L3T4R3B,
                              (ULONG)sizeof_TW8L7T8R7B - sizeof_TW4L3T4R3B ));
                   dL = (SHORT)((PTW8L7T8R7B)&ccRcl)->dL;
                   dT = (SHORT)((PTW8L7T8R7B)&ccRcl)->dT;
                   dR = (SHORT)((PTW8L7T8R7B)&ccRcl)->dR;
                   dB = (SHORT)((PTW8L7T8R7B)&ccRcl)->dB;
                   TRACE(( TC_TW, TT_TW_TEXT,
                   "TWCmdTextOut: ComplexClip rcl[%d] 4 BYTES dL:8 dT:7 dR:8 dB:7", i ));
                   break;
#else
                case TWTO_RCLCLIP_11L0T11R0B:
                   // Now get the last byte
                   GetNextTWCmdBytes( (char *)&ccRcl + sizeof_TW4L3T4R3B,
                                      SIZE_OF_TW11L0T11R0B - sizeof_TW4L3T4R3B );
                   TRACEBUF(( TC_TW, TT_TW_TEXT, 
                              (char far *)&ccRcl + sizeof_TW4L3T4R3B,
                              (ULONG)SIZE_OF_TW11L0T11R0B - sizeof_TW4L3T4R3B ));
                   dL = (SHORT)((PTW11L0T11R0B)&ccRcl)->dL;
                   dT = 0;
                   dR = (SHORT)((PTW11L0T11R0B)&ccRcl)->dR;
                   dB = 0;
                   TRACE(( TC_TW, TT_TW_TEXT,
                   "TWCmdTextOut: ComplexClip rcl[%d] 3 BYTES dL:11 dT:0 dR:11 dB:0", i ));
                   bSideBySide = TRUE;
                   break;
#endif
                case TWTO_RCLCLIP_12L11T12R11B:
                   {
                      USHORT high;

                      // Now get the last four bytes
                      GetNextTWCmdBytes( (char *)&ccRcl + sizeof_TW4L3T4R3B,
                                     sizeof_TW12L11T12R11B - sizeof_TW4L3T4R3B );
                      TRACEBUF(( TC_TW, TT_TW_TEXT, 
                                 (char far *)&ccRcl + sizeof_TW4L3T4R3B,
                             (ULONG)sizeof_TW12L11T12R11B - sizeof_TW4L3T4R3B ));
                      dL = (SHORT)ccRcl.dL;
                      dT = (SHORT)ccRcl.dT;
                      high = (USHORT)ccRcl.dRHigh;
                      dR = (SHORT)( ((USHORT)ccRcl.dRLow & 0x7F) | ( high << 7 ));
                      dB = ccRcl.dB;
                      TRACE(( TC_TW, TT_TW_TEXT,
                      "TWCmdTextOut: ComplexClip rcl[%d] 6 BYTES dL:12 dT:11 dR:12 dB:11", i ));
                   }
                   break;
             }

             if ( i == 0 ) {
                vpEnr->arcl[i].left   = dL;
                vpEnr->arcl[i].top    = dT;
                vpEnr->arcl[i].right  = dR;
                vpEnr->arcl[i].bottom = dB;
             } else {
                if ( bSideBySide ) {
                   vpEnr->arcl[i].left   = vpEnr->arcl[i-1].right + dL;
                   vpEnr->arcl[i].right  = vpEnr->arcl[i].left + dR;
                } else {
                   vpEnr->arcl[i].left   = vpEnr->arcl[i-1].left   + dL;
                   vpEnr->arcl[i].right  = vpEnr->arcl[i-1].right  + dR;
                }
                vpEnr->arcl[i].top    = vpEnr->arcl[i-1].top    + dT;
                vpEnr->arcl[i].bottom = vpEnr->arcl[i-1].bottom + dB;
             }
              TRACE(( TC_TW, TT_TW_TEXT,
              "TWCmdTextOut: ComplexClip dL(%d) dT(%d) dR(%d) dB(%d)",
                         dL, dT, dR, dB ));
              TRACE(( TC_TW, TT_TW_TEXT,
              "TWCmdTextOut: ComplexClip l(%d) t(%d) r(%d) b(%d)",
                         vpEnr->arcl[i].left, vpEnr->arcl[i].top,
                         vpEnr->arcl[i].right, vpEnr->arcl[i].bottom ));
          }
       }
    }

    /*
     *  Create compatible DC if not exist
     */
    if ( compatDC == NULL ) {
       compatDC = CreateCompatibleDC( hDC );
    }
    ASSERT( compatDC != NULL,0 );

    /*
     *  Simple clipping
     */
    vfrclClip = (SCHAR)(Options & TWTO_RCLCLIP);

    /*
     *  Create the region mask out of clipping rects
     */
    SelectVisibleRegionIntoDC( hDC );

    /*
     *  Create buffer for GlyphBlt function
     */
    CreateGlyphBuffer();

    /*
     *  16 or 256 color case
     */
    if ( vColor == Color_Cap_256 ) {
    
        /*
         *  Set the opaque or transparent FG/BG colors
         */
        if ( vFlags.bOpaqueBackground ) {
    
            TRACE(( TC_TW, TT_TW_TEXT, "w_TWCmdTextOut: Opaque" ));
            
            /*
             *  Fill clipping rect for non-buffered cases
             */
            if ( !vfGlyphBuf ) {
                FillBackground( hDC, vLastColor256.BG );
            }
    
            /*
             *  Set FG/BG colors if necessary and save state
             */
            if ( (dcstate.bkcolor  != PALETTEINDEX(vLastColor256.BG)) ||
                 (dcstate.txtcolor != PALETTEINDEX(vLastColor256.FG)) ) {
                                                          
                dcstate.txtcolor = PALETTEINDEX(vLastColor256.FG);
                SetTextColor( hDC, dcstate.txtcolor );
                dcstate.bkcolor  = PALETTEINDEX(vLastColor256.BG);
                SetBkColor( hDC, dcstate.bkcolor );
            }
        }
        else {
    
            TRACE(( TC_TW, TT_TW_TEXT, "w_TWCmdTextOut: Transparent" ));
    
            /*
             *  Set FG/BG colors for mask trick
             */
            if ( (dcstate.bkcolor  != PALETTEINDEX(255)) ||
                 (dcstate.txtcolor != PALETTEINDEX(vLastColor256.FG)) ) {
                             
                dcstate.bkcolor  = PALETTEINDEX(255);
                dcstate.txtcolor = PALETTEINDEX(vLastColor256.FG);
    
                SetBkColor( hDC, dcstate.bkcolor );
                SetTextColor( hDC, dcstate.txtcolor );
            }
        }
    }
    else {      //  16 color
    
        /*
         *  Set the opaque or transparent FG/BG colors
         */
        if ( vFlags.bOpaqueBackground ) {
    
            TRACE(( TC_TW, TT_TW_TEXT, "w_TWCmdTextOut: Opaque" ));
            
            /*
             *  Fill clipping rect for non-buffered cases
             */
            if ( !vfGlyphBuf ) {
                FillBackground( hDC, vLastColor16.BG );
            }
    
            /*
             *  Set FG/BG colors if necessary and save state
             */
            if ( (dcstate.bkcolor  != twsolidcolor[ vLastColor16.BG ]) ||
                 (dcstate.txtcolor != twsolidcolor[ vLastColor16.FG ]) ) {
                                                          
                dcstate.txtcolor = twsolidcolor[ vLastColor16.FG ];
                SetTextColor( hDC, dcstate.txtcolor );
                dcstate.bkcolor  = twsolidcolor[ vLastColor16.BG ];
                SetBkColor( hDC, dcstate.bkcolor );
            }
        }
        else {
    
            TRACE(( TC_TW, TT_TW_TEXT, "w_TWCmdTextOut: Transparent" ));
    
            /*
             *  Set FG/BG colors for mask trick
             */
            if ( (dcstate.bkcolor != RGB( 255, 255, 255 )) ||
                 (dcstate.txtcolor != twsolidcolor[ vLastColor16.FG ]) ) {
                             
                dcstate.bkcolor = RGB( 255, 255, 255 );
                dcstate.txtcolor = twsolidcolor[ vLastColor16.FG ];
    
                SetBkColor( hDC, dcstate.bkcolor );
                SetTextColor( hDC, dcstate.txtcolor );
            }
        }
    }

    /*
     *  Process all of the glyphs in the string
     */
    do {
    
        /*
         *  Output the glyph to the screen
         */
        bMore = GlyphBlt( hDC );
    
        /*
         *  No longer first glyph
         */
        if ( vfFirstGlyph ) {
            vfFirstGlyph = FALSE;
        }

    } while ( bMore );

    /*
     *  If we have buffer glyph data blast it out
     */
    if ( vfGlyphBuf ) {

        HBITMAP  hbmpOld;
        HBITMAP  hbmpGlyph;
        COLORREF oldBkColor;

        /*
         *  Need to flush out transparent glyphs now
         */
        if ( vFlags.bOpaqueBackground ) {
            MaskLVBToScreen( hDC, TRUE );
        }

        /*
         *  Blt to screen
         */
        if ( vFlags.bOpaqueBackground ) {
    
            TRACE(( TC_TW, TT_TW_TEXT, "GlyphBlt: Buffered Opaque left %u, top %u, width %u, height %u",
                                        vLeftGlyphBuf, vTopGlyphBuf, vcxGlyphBuf, vcyGlyphBuf ));
    
            /*
             *  Create bitmap and select it
             */
            hbmpGlyph = CreateBitmap( (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, 1, 1, vpBitmap );
            hbmpOld = SelectObject( compatDC, hbmpGlyph );
        
            BitBlt( hDC, vLeftGlyphBuf, vTopGlyphBuf, (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, compatDC, 0, 0, SRCCOPY  );
    
            /*
             *  Delete bitmap
             */
            SelectObject( compatDC, hbmpOld );
            DeleteObject( hbmpGlyph );
        }
        else {

            /*
             *  If there is no LVB then let the video driver handle this
             */
            if ( vpLVB == NULL ) { 

                /*
                 *  Create bitmap and select it
                 */
                hbmpGlyph = CreateBitmap( (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, 1, 1, vpBitmap );
                hbmpOld = SelectObject( compatDC, hbmpGlyph );
            
#ifdef WIN32
                if ( !MaskBlt( hDC, vLeftGlyphBuf, vTopGlyphBuf,   
                     (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, 
                     compatDC, 0, 0, 
                     hbmpGlyph, 0, 0, 
                     MAKEROP4(0x00AA0029, SRCCOPY))) 
#endif
                {
                    BitBlt( hDC, vLeftGlyphBuf, vTopGlyphBuf, (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, compatDC, 0, 0, SRCAND );
                    oldBkColor = SetBkColor( hDC, ((vColor == Color_Cap_256) ? 
                                                    PALETTEINDEX(0) : 
                                                    RGB( 0, 0, 0 )) );
                    BitBlt( hDC, vLeftGlyphBuf, vTopGlyphBuf, (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, compatDC, 0, 0, SRCPAINT );
                    SetBkColor( hDC, oldBkColor );
                }
        
                /*
                 *  Delete bitmap
                 */
                SelectObject( compatDC, hbmpOld );
                DeleteObject( hbmpGlyph );
            }
            else 
            {
    
                /*
                 *  Do not use LVB on complex clipping cases
                 */
                if ( vbComplex ) 
                {
                    /*
                     *  Create bitmap and select it
                     */
                    hbmpGlyph = CreateBitmap( (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, 1, 1, vpBitmap );
                    hbmpOld = SelectObject( compatDC, hbmpGlyph );
                
                    BitBlt( hDC, vLeftGlyphBuf, vTopGlyphBuf, (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, compatDC, 0, 0, SRCAND );
                    oldBkColor = SetBkColor( hDC, ((vColor == Color_Cap_256) ? 
                                                    PALETTEINDEX(0) : 
                                                    RGB( 0, 0, 0 )) );
                    BitBlt( hDC, vLeftGlyphBuf, vTopGlyphBuf, (INT) vcxGlyphBuf, (INT) vcyGlyphBuf, compatDC, 0, 0, SRCPAINT );
                    SetBkColor( hDC, oldBkColor );

                    /*
                     *  Delete bitmap
                     */
                    SelectObject( compatDC, hbmpOld );
                    DeleteObject( hbmpGlyph );
                }
                else {

                    MaskGlyphsToLVB( hDC, vpBitmap, vcxGlyphBuf,   vcyGlyphBuf,
                                                    vLeftGlyphBuf, vTopGlyphBuf );
                }
            }
        }
    }

    /*
     *  Restore previous clipping region
     */
    if ( hrgnDest != NULL ) { 

        /*
         *  Restore empty clipping region
         */
        SelectClipRgn( hDC, NULL );

        /*
         *  Destroy the clipping region
         */
        DeleteObject( hrgnDest );
        hrgnDest = NULL;
    }

    DTRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_TEXT, "TWCmdTextOut: Exiting" ));
    TWCmdReturn( !rc ); // return to NewNTCommand or ResumeNTCommand
}


/****************************************************************************\
 *
 *  GetNextGlyphData
 *
 *  This routine retrieves the glyph data, either from the thinwire packet data
 *  or from the glyph cache.  All of the glyph data that fits in the the
 *  receiving buffer will be retrieved.  If the glyph is to large, it will be
 *  truncated on a scan line boundary.
 *
 *  PARAMETERS:
 *     BOOL bSetGD  TRUE - Set vGD with appropriate values
 *                  FALSE - Don't modify vGD
 *  RETURN: 
 *     none
 *
\****************************************************************************/

void
GetNextGlyphData( HDC hDC, BOOL bSetGD )
{


    USHORT i;
    LPBYTE lpCacheArea;

    struct {
        USHORT uscb;
        USHORT ulcbHigh;
    } Data;
    USHORT BeginBlock = 0, EndBlock = 0;
   
    if (vGH.bGetFromCache ) {
      
        // This object is being retrieved from the cache
        // Note that for _32B objects, we do nothing - pData was already set
        // up in GlyphBlt to point to the cache entry
        if ( vGH.ChunkType != _32B ) {

            // Always read the whole chunk - size will be adjusted
            // by TWCacheRead to actual value written
            switch ( vGH.ChunkType ) {
                case _128B: Data.uscb = 128; break;
                case _512B: Data.uscb = 512; break;
                default:    Data.uscb = 2048; break;
            }                                  
            Data.ulcbHigh = 0;

            // Cache entry is in the XMS/DASD cache
            DTRACE(( TC_TW, TT_TW_TEXT,
                    "GetNextGlyphData: Calling TWCacheRead for %u bytes h(%04X) typ(%d)",
                     Data.uscb,  vGH.hCache, (USHORT)vGH.ChunkType ));

            /*
             *  Get cache pointer
             */
            TRACE(( TC_TW, TT_TW_TEXT, "lpTWCacheRead: hCache %u, ChunkType %u", vGH.hCache, vGH.ChunkType ));
            lpCacheArea = lpTWCacheRead( vGH.hCache, 
                                         vGH.ChunkType,
                                         (LPUINT) &Data.uscb,
                                         BeginBlock );
            TRACE(( TC_TW, TT_TW_TEXT, "lpTWCacheRead: hCache %u, ChunkType %u, Bytes %u, lpCacheArea %08x", 
                    vGH.hCache, vGH.ChunkType, Data.uscb, lpCacheArea ));
            ASSERT( lpCacheArea != NULL, 0 );

            /*
             *  Copy data
             */ 
            if ( lpCacheArea ) {
                vpGlyphData = (lpCacheArea + SIZE_OF_GLYPHDIMENSION_WITH_WIDTH);
                memcpy( &vGlyphDimension, lpCacheArea, SIZE_OF_GLYPHDIMENSION_WITH_WIDTH );
            }
        }

        if ( bSetGD ) {
            vGD.cx     = (USHORT)vGlyphDimension.cx;
            vGD.cy     = (USHORT)vGlyphDimension.cy;
            vGD.dyThis = (SHORT)vGlyphDimension.dyThis;
            vGD.dxThis = (SHORT)vGlyphDimension.dxThis;
        }
  
        // Figure out how many bytes were retrieved
        switch ( vGH.ChunkType ) {
            case _32B:
            case _128B:
                // For these types, TWCacheRead does not remember the size
                vcbReceived = (((vGD.cx - 1) / 8) + 1)  
                                * vGD.cy;
                DTRACE(( TC_TW, TT_TW_TEXT,
                         "GetNextGlyphData: cx(%u) cy(%u) cb(%u) cbl(%lu)",
                         vGD.cx, vGD.cy, vcbReceived, (ULONG)vcbReceived ));
                break;
  
               // For these types, TWCacheRead remembers the size
            case _512B:
                vcbReceived = Data.uscb - SIZE_OF_GLYPHDIMENSION_WITH_WIDTH;
            case _2K:
                vcbReceived = Data.uscb;
                // First block contains GlyphDimension
                if ( BeginBlock == 0 ) { 
                    vcbReceived -= SIZE_OF_GLYPHDIMENSION_WITH_WIDTH;
                }
                break;
        } 
        DTRACE(( TC_TW, TT_TW_TEXT,
                 "GetNextGlyphData: TWCacheRead - vpStart(%08lX) pData(%08lX) cbData(%ld) ID(%08lX)",
                 (char far *)vpStart, (char far *)vpGlyphData, (ULONG)vcbReceived, ulHandle));
        DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)vpGlyphData, (ULONG)vcbReceived ));
    } 
    else {

        DTRACE(( TC_TW, TT_TW_TEXT, "GetNextGlyphData: Getting NextTWCmdBytes" ));

        // This object is not being retrieved from the cache
        GetNextTWCmdBytes( vpGlyphData, vcbReceived );
        TRACE(( TC_TW, TT_TW_TEXT, "GetNextGlyphData: NextTWCmdBytes (%d)", vcbReceived ));

        DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)vpGlyphData, (ULONG)vcbReceived ));

        /* 
         *  Invert bits for BitBlt, 0s are foreground and 1s are background
         */
        for ( i=0; i<vcbReceived; i++ ) {
            *(vpGlyphData+i) = ~(*(vpGlyphData+i));
        }

        Data.uscb = vcbReceived+SIZE_OF_GLYPHDIMENSION_WITH_WIDTH;

        // If we're placing the entry in the cache, do it now
        // Note that _32B objects were already cached directly via the
        // GetNextTWCmdBytes call (the GlyphDimension was added up in GlyphBlt)
        if ( vGH.bPutInCache && ( vGH.ChunkType != _32B ) ) {
            ASSERT( vcbReceived, vcbReceived );
            CacheWrite( Data.uscb );
        } 
    } 
}


/****************************************************************************\
 *  TWCmdTextOutNoClip (TWCMD_TEXTOUT_NOCLIP service routine)
 *
 *  This routine is called to service the no-clipping variations of the
 *  DrvTextOut thinwire display driver function.
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *     hDC (input)
 *        device context
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
TWCmdTextOutNoClip( HWND hWnd, HDC hDC )
{

    TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_TEXT, "TWCmdTextOutNoClip: entered" ));

    w_TWCmdTextOut( hWnd, hDC, TWTO_NOCLIP );
}


/****************************************************************************\
 *  TWCmdTextOutRclClip (TWCMD_TEXTOUT_RCLCLIP service routine)
 *
 *  This routine is called to service the rectangle (simple) clipping
 *  variations of the DrvTextOut thinwire display driver function.
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *     hDC (input)
 *        device context
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
TWCmdTextOutRclClip( HWND hWnd, HDC hDC )
{

    TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_TEXT, "TWCmdTextOutRclClip: entered" ));

    w_TWCmdTextOut( hWnd, hDC, TWTO_RCLCLIP );
}


/****************************************************************************\
 *  TWCmdTextOutCmplxClip (TWCMD_TEXTOUT_CMPLXCLIP service routine)
 *
 *  This routine is called to service the complex clipping variations of the
 *  DrvTextOut thinwire display driver function.
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *     hDC (input)
 *        device context
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
TWCmdTextOutCmplxClip( HWND hWnd, HDC hDC )
{

    TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_TEXT, "TWCmdTextOutCmplxClip: entered" ));

    w_TWCmdTextOut( hWnd, hDC, TWTO_CMPLXCLIP );
}


/****************************************************************************\
 *  TWCmdTextOutRclExtra (TWCMD_TEXTOUT_RCLEXTRA service routine)
 *
 *  This routine is called to draw the extra rectangles (underlines, 
 *  strikeouts, etc.) sent by the DrvTextOut thinwire display driver function.
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *     hDC (input)
 *        device context
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
TWCmdTextOutRclExtra( HWND hWnd, HDC hDC )
{
    LPBYTE p = (LPBYTE) lpstatic_buffer;
    UCHAR bMore;
    TWTOPT1RCLE  pt1RclE;
    ULONG   pt2Rcl;
    ULONG * ppt2Rcl = &pt2Rcl;
    USHORT  uscbData;

    TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_TEXT, "TWCmdTextOutRclExtra: entered" ));

    /*
     *  vpEnr and vpStart point to same spot because we use
     *  vpEnr first, and then reuse space for glpyh data
     */
    vpEnr    = (PRECT_ENUM) p;
    vpStart  = p;
    vpData   = (p + SIZE_OF_GLYPHDIMENSION_WITH_WIDTH);
    vpBitmap = (p + LARGE_CACHE_CHUNK_SIZE);

    /*
     *  Create the region mask out of clipping rects for complex only
     */
    if ( vbComplex ) {
        if ( vpLVB ) {
            MaskLVBToScreen( vhdc, TRUE );
        }
        SelectVisibleRegionIntoDC( hDC );
    }

    // Get extra rectangle(s) (potentially already clipped) 
    do {
        // Get pt1Rcl
        DTRACE(( TC_TW, TT_TW_TEXT, "TWCmdTextOutRclExtra: Receiving compressed rclExtra:" ));
        GetNextTWCmdBytes( &pt1RclE, 3 );
        DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)&pt1RclE, 3UL ));
      
        // Get size of pt2Rcl
        GETNEXTPTSIZE( pt1RclE, &uscbData );
        GetNextTWCmdBytes( ppt2Rcl, (int)uscbData );
        DTRACEBUF(( TC_TW, TT_TW_TEXT, (char far *)ppt2Rcl, (ULONG)uscbData ));
      
        // Put it all together
        GETTEXTPTDATA( pt1RclE, vrclBackground, ppt2Rcl, vdxLast, vdyLast );
  
        vrclBackground.right++;        // Change LRH back to 1-based 1
        vrclBackground.bottom++;                   
  
        bMore = (UCHAR)pt1RclE.bMore;
  
        TRACE(( TC_TW, TT_TW_TEXT,
        "TWCmdTextOutRclExtra: Received - rclExtra: l:%d t:%d r:%d b:%d",
                   vrclBackground.left,  vrclBackground.top,
                   vrclBackground.right, vrclBackground.bottom ));
  
        // If entire string is outside of the clipping region, ignore
        // (remember, rcl is bottom-right exclusive)
        if ( ( vrclBackground.left < vrclBackground.right  ) &&
             ( vrclBackground.top  < vrclBackground.bottom ) ) {
           if ( vpLVB && vfLVB && !vbComplex ) {
               MaskRclExtraToLVB( hDC,
                                  vrclBackground.left,  vrclBackground.top,
                                  vrclBackground.right, vrclBackground.bottom );
           }
           else {
               FillBackground( hDC, TWCLR_CURRENT );
           }
        }
#ifdef DEBUG
        else {
            TRACE((TC_TW, TT_TW_TEXT,
                  "TWCmdTextOutRclExtra: Entire extra rectangle is outside clipping" ));
        }
#endif
    } while ( bMore );

    /*
     *  Restore previous clipping region
     */
    if ( hrgnDest != NULL ) { 

        /*
         *  Restore empty clipping region
         */
        SelectClipRgn( hDC, NULL );

        /*
         *  Destroy the clipping region
         */
        DeleteObject( hrgnDest );
        hrgnDest = NULL;
    }

    TWCmdReturn( TRUE ); // return to NewNTCommand or ResumeNTCommand
}


#ifndef DOS


/****************************************************************************\
 *  MaskRclExtraToLVB
 *
 *  This routine is called to mask the current extra rect to the LVB.
 *
 *  PARAMETERS:
 *     LeftGlyph (input)
 *        upper right corner
 *     TopGlyph (input)
 *        upper left corner
 *     RightGlyph (input)
 *        lower right corner
 *     Bottom Glyph (input)
 *        lower right corner
 *
\****************************************************************************/

void
MaskRclExtraToLVB( HDC hDC, 
                 INT LeftGlyph,  INT TopGlyph,
                 INT RightGlyph, INT BottomGlyph )
{
    ULONG  iLVB;
    ULONG  cx, cy;
    ULONG  cxBytes;
    LPBYTE p = (LPBYTE) lpstatic_buffer + LARGE_CACHE_CHUNK_SIZE;
    LPBYTE pbm;
    
    if ( TopGlyph < vRectLVB.top )
        vRectLVB.top = TopGlyph;
    if ( BottomGlyph > vRectLVB.bottom )
        vRectLVB.bottom = BottomGlyph;
    if ( LeftGlyph < vRectLVB.left )
        vRectLVB.left = LeftGlyph;
    if ( RightGlyph > vRectLVB.right )
        vRectLVB.right = RightGlyph;

    /*
     *  Create the mask size
     */
    cxBytes = ((RightGlyph + (8 - ((RightGlyph - 1) % 8))) - (LeftGlyph & 0xfff8)) >> 3;
    memset( p, 0x00, (INT) cxBytes );

    /*
     *  Fixup the ends
     */
    if ( (LeftGlyph % 8) ) {
        *(p) |= (0xFF << (8 - (LeftGlyph % 8)));
    }
    if ( (RightGlyph % 8) ) {
        *(p+cxBytes-1) |= (0xFF >> (RightGlyph % 8));
    }

    /*
     *  Band the glyphs into the LVB
     */
    iLVB = ((ULONG)TopGlyph * vcxBytes) + (ULONG)(LeftGlyph >> 3);
    for ( cy=TopGlyph;  cy<(ULONG)BottomGlyph; cy++ ) {
        pbm = p;
        for ( cx=0; cx<cxBytes; cx++ ) {
            *(vpLVB + iLVB++) &= *(pbm++);
        }
        iLVB += (vcxBytes - cxBytes);
    }
}


/****************************************************************************\
 *  MaskGlyphsToLVB
 *
 *  This routine is called to mask the current transparent glyphs  
 *  to the LVB.
 *
 *  PARAMETERS:
 *     pBitmap (input)
 *        current array of glyphs
 *     cxGlyph (input)
 *        width of glyph buf
 *     cyGlyph (input)
 *        height of glyph buf
 *     LeftGlyph (input)
 *        upper right corner
 *     TopGlyph (input)
 *        upper left corner
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
MaskGlyphsToLVB( HDC hDC, 
                 LPBYTE pBitmap, INT cxGlyph,   INT cyGlyph,
                                 INT LeftGlyph, INT TopGlyph )
{
    ULONG iLVB;
    ULONG cx, cy;
    ULONG cxBytes = (ULONG) (cxGlyph >> 3);
    LPBYTE pbm = pBitmap;
    INT   BottomGlyph = TopGlyph + cyGlyph;
    INT   RightGlyph  = LeftGlyph + cxGlyph;
    
    /*
     *  Color change?
     */
    if ( (vfLVB == TRUE) && (vTextColor != dcstate.txtcolor) ) 
    {
        MaskLVBToScreen( hDC, TRUE );
    }

    /*
     *  Boundary check
     */
    if ( (TopGlyph >= (INT)vcyLVB) || (LeftGlyph >= (INT)vcxLVB) ) {
        return;
    }

    /*
     *  More bounds checking
     */
    if ( BottomGlyph >= (INT) vcyLVB ) {
        BottomGlyph = (INT) vcyLVB;
        cyGlyph = BottomGlyph - TopGlyph;
    }
    if ( RightGlyph >= (INT) vcxLVB ) {
        RightGlyph = (INT) vcxLVB;
        cxGlyph = RightGlyph - LeftGlyph;
    }

    /*
     *  New buffer
     */ 
    if ( !vfLVB ) {
        memset( vpLVB, 0xFF, (INT) vcbLVB );
        vRectLVB.top    = TopGlyph;
        vRectLVB.bottom = BottomGlyph;
        vRectLVB.left   = LeftGlyph;
        vRectLVB.right  = RightGlyph;
        vfLVB = TRUE;
        vTextColor = dcstate.txtcolor;
    }
    else {
    
        if ( TopGlyph < vRectLVB.top )
            vRectLVB.top = TopGlyph;
        if ( BottomGlyph > vRectLVB.bottom )
            vRectLVB.bottom = BottomGlyph;
        if ( LeftGlyph < vRectLVB.left )
            vRectLVB.left = LeftGlyph;
        if ( RightGlyph > vRectLVB.right )
            vRectLVB.right = RightGlyph;
    }

    /*
     *  Band the glyphs into the LVB
     */
    iLVB = ((ULONG)TopGlyph * vcxBytes) + (ULONG)(LeftGlyph >> 3);
    for ( cy=0;  cy<(ULONG)cyGlyph; cy++ ) {
        for ( cx=0; cx<cxBytes; cx++ ) {
            *(vpLVB + iLVB++) &= *(pbm++);
        }
        iLVB += (vcxBytes - cxBytes);
    }
}


/****************************************************************************\
 *  MaskLVBToScreen
 *
 *  This routine is called to mask the current LVB to the screen.
 *  to the LVB.
 *
 *  PARAMETERS:
 *     cxGlyph (input)
 *        width of glyph buf
 *     cyGlyph (input)
 *        height of glyph buf
 *     LeftGlyph (input)
 *        upper right corner
 *     TopGlyph (input)
 *        upper left corner
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
MaskLVBToScreen( HDC hDC, BOOL fForceFlush )
{
    ULONG    isLVB;
    ULONG    idLVB;
    ULONG    cy;
    ULONG    cxBytes;
    ULONG    TopGlyph;
    ULONG    LeftGlyph;
    ULONG    cxGlyph;
    ULONG    cxBitmap;
    ULONG    cyBitmap;
    ULONG    cyGlyph;
    HBITMAP  hbmpOld;
    HBITMAP  hbmpGlyph;
    HDC      hdcMem;
    HBITMAP  hbmpMem;
    HBITMAP  hbmpMemOld;
    HPALETTE hPaletteOld = NULL;
    BOOL     fDone = FALSE;

    /*
     *  If no data skip
     */
    if ( !vfLVB ) {
        return;
    }

    /*
     *  Need to flush?
     */
    if ( fForceFlush == TRUE ) {
        goto FlushLVB;
    }
    else if ( vcsNextUpdate < (ULONG)Getmsec() ) {
        goto FlushLVB;
    }
    return;

FlushLVB :

    /*
     *  Get the bounds and reset
     */
    vfLVB = FALSE;
    TopGlyph  = vRectLVB.top;
    LeftGlyph = vRectLVB.left;
    cyGlyph   = cyBitmap = vRectLVB.bottom - TopGlyph;
    cxGlyph   = cxBitmap = vRectLVB.right  - LeftGlyph;

    /*
     *  Dword align
     */
    if ( cxBitmap % 32 ) {
        cxBitmap += (32 - (cxBitmap % 32));
    }

    /*
     *  Bytes in scan line
     */
    cxBytes = cxBitmap >> 3;

    /*
     *  Setup source and destination indexes
     */
    isLVB = (TopGlyph * vcxBytes) + (LeftGlyph >> 3);
    idLVB = 0;

    /*
     *  Move the glyph buffer to the origin of the LVB
     */
    for ( cy=0;  cy<cyGlyph; cy++ ) {
        
        /*
         *  Move source bytes to origin
         */
        memcpy( (vpLVB + idLVB), (vpLVB + isLVB), (INT) cxBytes );

        /*
         *  Increment source and destination
         */
        isLVB += vcxBytes;
        idLVB += cxBytes;
    }

    /*
     *  Remove any clip regions
     */
    SelectClipRgn( hDC, NULL );

    /*
     *  Create bitmap
     */
    hbmpGlyph = CreateBitmap( (INT) cxBitmap, (INT) cyBitmap, 1, 1, vpLVB );

    /*
     *  Select into compatible DC
     */
    hbmpOld = SelectObject( compatDC, hbmpGlyph );

    hdcMem = CreateCompatibleDC( hDC );
    hbmpMem = CreateCompatibleBitmap( hDC, (INT) cxBitmap, (INT) cyBitmap );
    hbmpMemOld = SelectObject( hdcMem, hbmpMem );

    if ( (vColor == Color_Cap_256) && (vhPalette != NULL) ) {
        hPaletteOld = SelectPalette( hdcMem, vhPalette, TRUE  );
        RealizePalette( hdcMem );
    }

    //  SCREEN to MEM
    BitBlt( hdcMem, 0, 0, (INT) cxGlyph, (INT) cyGlyph, 
            hDC, (INT) LeftGlyph, (INT) TopGlyph, SRCCOPY  );

    //  SCRAND
    BitBlt( hdcMem, 0, 0, (INT) cxGlyph, (INT) cyGlyph, 
            compatDC, 0, 0, SRCAND );

    SetTextColor( hdcMem, vTextColor );
    SetBkColor( hdcMem, ((vColor == Color_Cap_256) ? 
                        PALETTEINDEX(0) : 
                        RGB( 0, 0, 0 )) );

    //  SCRPAINT
    BitBlt( hdcMem, 0, 0, (INT) cxGlyph, (INT) cyGlyph, 
            compatDC, 0, 0, SRCPAINT );

    //  MEM to SCREEN
    BitBlt( hDC, (INT) LeftGlyph, (INT) TopGlyph, (INT) cxGlyph, (INT) cyGlyph, 
            hdcMem, 0, 0, SRCCOPY  );

    if ( (vColor == Color_Cap_256) && (vhPalette != NULL) ) {
        SelectObject( hdcMem, hPaletteOld );
    }
    SelectObject( hdcMem, hbmpMemOld );
    DeleteObject( hbmpMem );
    DeleteDC( hdcMem );

    SelectObject( compatDC, hbmpOld );
    DeleteObject( hbmpGlyph );

    /*
     *  Restore previous clipping region
     */
    if ( hrgnDest != NULL ) { 
        SelectClipRgn( hDC, hrgnDest );
    }

    /*
     *  Schedule next update
     */
    vcsNextUpdate = Getmsec() + UPDATE_DELAY;
}
#endif
