/******************************Module*Header*******************************\
* Module Name: twcommon.h
*
* This file contains ThinWire stuff shared by both the host and the client
*
* Created: 25-Apr-1994
* Author: Marc Bloomfield
*
* Copyright (c) 1993-4 Citrix Systems, Inc.
*
*  $Log$
*  
*     Rev 1.47   21 Apr 1997 16:57:46   TOMA
*  update
*  
*     Rev 1.46   01 Apr 1997 22:06:24   BradA
*  Add aan unpacked TWTOPOSITION structure that can be used in calculating
*  new text positions without having overflow errors.
*  
*     Rev 1.45   30 Jan 1997 13:00:42   kurtp
*  update
*  
*     Rev 1.44   28 Jan 1997 13:40:18   kurtp
*  update
*  
*     Rev 1.43   04 Dec 1996 18:48:44   kurtp
*  update
*  
*     Rev 1.42   25 Nov 1996 17:47:08   kurtp
*  update
*  
*     Rev 1.41   20 Nov 1996 17:57:56   kurtp
*  update
*  
*     Rev 1.40   20 Nov 1996 16:49:38   kurtp
*  update
*  
*     Rev 1.39   20 Nov 1996 16:23:22   kurtp
*  update
*  
*     Rev 1.38   06 Nov 1996 09:43:06   kurtp
*  source sync
*  
*     Rev 1.37   06 May 1996 17:02:04   jeffm
*  allow include recursion
*  
*     Rev 1.36   09 Nov 1995 11:35:20   kurtp
*  update
*  
*     Rev 1.35   02 Nov 1995 12:53:04   kurtp
*  update
*  
*     Rev 1.34   31 Oct 1995 11:52:24   kurtp
*  update
*  
*     Rev 1.33   04 Oct 1995 16:53:48   kurtp
*  update
*  
\**************************************************************************/

#ifndef __TWCOMMON_H__
#define __TWCOMMON_H__

#include <citrix\twh2inc.h>

/*=============================================================================
==   ThinWire client-host structures 
=============================================================================*/

/*
 * ThinWire capabilities 
 */
typedef struct _SCREENRES {
    USHORT HRes;
    USHORT VRes;
} SCREENRES, * PSCREENRES;

typedef struct _THINWIRECAPS {
    BYTE Version;
    BYTE bPad;

    USHORT cbSmallCache;        // must be divisible by 32 (32K max)
    ULONG  cbLargeCache;        // must be divisible by 2K (8M max)

    ULONG flGraphicsCaps;
#define GCAPS_COMPLEX_CURVES      0x00000001L
#define GCAPS_COMPLEX_CURVES_FILL 0x00000002L 
#define GCAPS_PTRS_ANIMATED       0x00000004L 
#define GCAPS_PTRS_GT_32_BY_32    0x00000008L 
#define GCAPS_BRUSH_GT_8_BY_8     0x00000010L 
#define GCAPS_SS_BMP_FILE         0x00000020L 
#define GCAPS_BMPS_PRECACHED      0x00000040L 
#define GCAPS_ROP4_BITBLT         0x00000080L 
#define GCAPS_RES_VARIABLE        0x00000100L // var HRes/VRes up to fResCaps
#define GCAPS_SSB_1BYTE_PP        0x00000200L // save screen 1 byte per pixel

    USHORT  fColorCaps;
#define CCAPS_4_BIT               0x0001    //       16 colors
#define CCAPS_8_BIT               0x0002    //      256 colors
#define CCAPS_16_BIT              0x0004    //    65535 colors
#define CCAPS_24_BIT              0x0008    // 16777215 colors
    USHORT  usPad;
    USHORT ResCapsOff;
    USHORT ResCapsCnt;
    SCREENRES ResCaps;
} THINWIRECAPS, * PTHINWIRECAPS;



#define ExtractMode( pMode, hRes, vRes )                                  \
 {                                                                        \
    PSCREENRES pRes = (PSCREENRES)((LPBYTE)(pMode) + (pMode)->ResCapsOff); \
    hRes = pRes->HRes;  vRes = pRes->VRes;                                \
 }

/* 
 *  Structure sent to host by client dll  (vdtw30.dll)
 */
typedef struct _VDTW_C2H {

    /* version 1 */
    VD_C2H Header;
    THINWIRECAPS ThinWirePref;
    THINWIRECAPS ThinWireCaps;

    /* version 2 */
    SCREENRES ResCaps[9];       // overflow from THINWIRECAPS 
    ULONG CacheTiny;
    ULONG CacheLowMem;
    ULONG CacheXms;
    ULONG CacheDASD;

    /* version 3 */
    ULONG DimCacheSize;
    ULONG DimBitmapMin;
    ULONG DimSignatureLevel;
    ULONG DimFilesysOverhead;

} VDTW_C2H, * PVDTW_C2H;


// General stuff...
////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------
-- DEFINES
--------------------------------------------------------------------------*/

//whenever we have similar data variables for 2K, 512 bytes, 128 bytes
//and 32 bytes, instead of having four seperate variables, we will use
//an array where an index of
// 0 - for 2K chunks
// 1 - for 512 byte chunks
// 2 - for 128 byte chunks
// 3 - for 32 byte chunks
// 4 - for 32 by 32 mono ptrs
//if the array limit is less than 5 then we don't want to support the
//remaining types with this data type

//NOTE: CANNOT CHANGE THIS ORDER BECAUSE OF GLOBAL CONSTANT DATA ARRAYS

typedef enum _CHUNK_TYPE
{
   _2K,
   _512B,
   _128B,
   _32B,
   monoptr,
   numberofchunktypes
} CHUNK_TYPE, *PCHUNK_TYPE;


typedef enum _COLOR_CAPS 
{
    Color_Cap_16,
    Color_Cap_256,
    Color_Cap_64K,
    Color_Cap_16M,
    Color_Cap_Max,
} COLOR_CAPS, * PCOLOR_CAPS;


typedef struct _TWRECTI       /* rcl */
{
    SHORT  left;
    SHORT  top;
    SHORT  right;
    SHORT  bottom;
} TWRECTI, *PTWRECTI;

// DrvStrokePath stuff... HOST: stroke.c   CLIENT: twstroke.c
////////////////////////////////////////////////////////////////////////////

// Flip and round flags:

#define FL_H_ROUND_DOWN         0x00000080L     // .... .... 1... ....
#define FL_V_ROUND_DOWN         0x00008000L     // 1... .... .... ....

#define FL_FLIP_D               0x00000005L     // .... .... .... .1.1
#define FL_FLIP_V               0x00000008L     // .... .... .... 1...
#define FL_FLIP_SLOPE_ONE       0x00000010L     // .... .... ...1 ....
#define FL_FLIP_HALF            0x00000002L     // .... .... .... ..1.
#define FL_FLIP_H               0x00000200L     // .... ..1. .... ....

#define FL_ROUND_MASK           0x0000001CL     // .... .... ...1 11..
#define FL_ROUND_SHIFT          2

#define FL_RECTLCLIP_MASK       0x0000000CL     // .... .... .... 11..
#define FL_RECTLCLIP_SHIFT      2

#define FL_STRIP_MASK           0x00000003L     // .... .... .... ..11
#define FL_STRIP_SHIFT          0

#define FL_SIMPLE_CLIP          0x00000020      // .... .... ..1. ....
#define FL_COMPLEX_CLIP         0x00000040      // .... .... .1.. ....
#define FL_CLIP                (FL_SIMPLE_CLIP | FL_COMPLEX_CLIP)

#define FL_ARBITRARYSTYLED      0x00000400L     // .... .1.. .... ....
#define FL_MASKSTYLED           0x00000800L     // .... 1... .... ....
#define FL_STYLED              (FL_ARBITRARYSTYLED | FL_MASKSTYLED)
#define FL_ALTERNATESTYLED      0x00001000L     // ...1 .... .... ....

#define FL_STYLE_MASK           0x00000C00L
#define FL_STYLE_SHIFT          10

// Simpler flag bits in high byte:

#define FL_DONT_DO_HALF_FLIP    0x00002000L     // ..1. .... .... ....
#define FL_PHYSICAL_DEVICE      0x00004000L     // .1.. .... .... ....


#define STRIP_MAX           100
#define STYLE_MAX_COUNT      16
#define STYLE_DENSITY         3
#define STYLE_MAX_VALUE     0x3fffL


typedef LONG STYLEPOS;

struct _STRIP;
struct _LINESTATE;

typedef VOID (far *PFNSTRIP)(struct _STRIP*, struct _LINESTATE*, LONG*);

typedef struct _LINESTATE {

    STYLEPOS        spTotal;        // Sum of style array
    STYLEPOS        spTotal2;       // Twice sum of style array
    STYLEPOS        spNext;         // Style state at start of next line
    STYLEPOS        spComplex;      // Style state at start of complex clip line

    STYLEPOS*       aspRtoL;        // Style array in right-to-left order
    STYLEPOS*       aspLtoR;        // Style array in left-to-right order

    ULONG           xyDensity;      // Density of style
    ULONG           cStyle;         // Size of style array

    ULONG           ulStyleMaskLtoR;// Original style mask, left-to-right order
    ULONG           ulStyleMaskRtoL;// Original style mask, right-to-left order

    ULONG           ulStartMask;    // Determines if first element in style
                                    // array is for a gap or a dash

// Used for 2 pass ROPs and/or device-format bitmaps:

    PFNSTRIP*       apfnStrip;      // Actual strip table if doing 2-pass ROP
                                    // or a DFB

    ULONG           iColor;         // Color for 2nd pass of 2-pass ROP
                                    // or color index for DFB pen

    ULONG           ulVgaMode;      // VGA mode for 2nd pass of 2-pass ROP

    ULONG           ulDrawModeIndex;// Set to mix - 1
    ULONG           ulBitmapROP;    // ROP info for DFB strip drawer
    LONG            lNextPlane;     // Offset to next plane, same scan line

} LINESTATE;	                /* ls */


/*****************************************************************************\
 * Format for STROKEPATH packet:
 *
 * TWSPHEADER     Flags;     (required field)
 * TWSPPEN        Pen;       (present IFF Flags.fSamePen (b6 of fl) is FALSE)
 * TWSPSTYLEMASK  StyleMask; (present IFF Flags.style (b2-b4) = TW_LINE_MASK)
 * TWSPSTYLESTATE StyleState;(present IFF Flags.style (b2-b4) != TW_LINE_SOLID)
 * TWSPSTYLE      Style;     (present IFF Flags.style (b2-b4) = TW_LINE_OTHER)
 * TWSPBOUNDS     Bounds;    (present IFF Flags.clipping (b0-b1) = TW_CLIP_RECT)
 * TWSPSTRIP      Strip[n];  (n = # of enumerated strips within the path object)
 *
 * Assumptions:
 * - No geometric lines (cosmetic only)
 * - Foreground mix (Raster operation) is the same as the background mix
 * - Brushes are solid colors only
 * - Max elements in style array = 16
 * - Value of style array element is in range 1-3FFF
 * - Max points in strip = 100 ( for TRIVIAL and RECT clipping )
 * - Max points in strip =  20 ( for COMPLEX clipping )
 *
\*****************************************************************************/

/*****************************************************************************\
 * TWSPHEADER    Flags;     (required field)
\*****************************************************************************/
typedef struct _TWSPHEADER {
   UCHAR clipping  : 2;          // b1 b0
#define   TW_CLIP_TRIVIAL      0 //  0  0
#define   TW_CLIP_RECT         1 //  0  1
#define   TW_CLIP_COMPLEX      2 //  1  0
#define   TW_CLIP_reserved     3 //  1  1

   UCHAR style     : 3;          // b4 b3 b2
#define   TW_LINE_SOLID        0 //  0  0  0
#define   TW_LINE_ALTERNATE    1 //  0  0  1
#define   TW_LINE_DASH         2 //  0  1  0
#define   TW_LINE_DOT          3 //  0  1  1
#define   TW_LINE_DASHDOT      4 //  1  0  0
#define   TW_LINE_DASHDOTDOT   5 //  1  0  1
#define   TW_LINE_MASK         6 //  1  1  0 (8 bit mask specified in StyleMask)
#define   TW_LINE_OTHER        7 //  1  1  1 (Custom style specified in Style)

   UCHAR fStartGap : 1;       // b5 TRUE/FALSE
   UCHAR fSamePen  : 1;       // b6 TRUE/FALSE (Pen field used if FALSE)
   UCHAR res       : 1;       // b7
} TWSPHEADER, *PTWSPHEADER;

/*****************************************************************************\
 * TWSPPEN       Pen;       (present IFF Flags.fSamePen (b6 of fl) is FALSE)
\*****************************************************************************/
typedef struct _TWSPPEN {
   UCHAR rop2  : 4; // b3 b2 b1 b0
                    //  0  0  0  0   = 0 = R2_WHITE       (10)  1
                    //  0  0  0  1   = 1 = R2_BLACK       (01)  0
                    //  0  0  1  0   = 2 = R2_NOTMERGEPEN (02) DPon
                    //  0  0  1  1   = 3 = R2_MASKNOTPEN  (03) DPna
                    //  0  1  0  0   = 4 = R2_NOTCOPYPEN  (04) PN
                    //  0  1  0  1   = 5 = R2_MASKPENNOT  (05) PDna
                    //  0  1  1  0   = 6 = R2_NOT         (06) Dn
                    //  0  1  1  1   = 7 = R2_XORPEN      (07) DPx
                    //  1  0  0  0   = 8 = R2_NOTMASKPEN  (08) DPan
                    //  1  0  0  1   = 9 = R2_MASKPEN     (09) DPa
                    //  1  0  1  0   = A = R2_NOTXORPEN   (0A) DPxn
                    //  1  0  1  1   = B = R2_NOP         (0B) D
                    //  1  1  0  0   = C = R2_MERGENOTPEN (0C) DPno
                    //  1  1  0  1   = D = R2_COPYPEN     (0D) P
                    //  1  1  1  0   = E = R2_MERGEPENNOT (0E) PDno
                    //  1  1  1  1   = F = R2_MERGEPEN    (0F) DPo
   UCHAR color : 4; // b3 b2 b1 b0
                    //  I  R  G  B
                    //  0  0  0  0   = 0 = BLACK
                    //  0  0  0  1   = 1 = BLUE
                    //  0  0  1  0   = 2 = GREEN
                    //  0  0  1  1   = 3 = CYAN
                    //  0  1  0  0   = 4 = RED
                    //  0  1  0  1   = 5 = MAGENTA
                    //  0  1  1  0   = 6 = BROWN
                    //  0  1  1  1   = 7 = WHITE
                    //  1  0  0  0   = 8 = GRAY
                    //  1  0  0  1   = 9 = LIGHT BLUE
                    //  1  0  1  0   = A = LIGHT GREEN
                    //  1  0  1  1   = B = LIGHT CYAN
                    //  1  1  0  0   = C = LIGHT RED
                    //  1  1  0  1   = D = LIGHT MAGENTA
                    //  1  1  1  0   = E = YELLOW
                    //  1  1  1  1   = F = WHITE (high intesity)
} TWSPPEN, *PTWSPPEN;


/*****************************************************************************\
 * TWSPPEN256    Pen;       (present IFF Flags.fSamePen (b6 of fl) is FALSE)
 *
 * This is the 256 color version of TWSPPEN struct.
 *
\*****************************************************************************/
typedef struct _TWSPPEN256 {
   UCHAR rop2  : 4; // b3 b2 b1 b0
                    //  0  0  0  0   = 0 = R2_WHITE       (10)  1
                    //  0  0  0  1   = 1 = R2_BLACK       (01)  0
                    //  0  0  1  0   = 2 = R2_NOTMERGEPEN (02) DPon
                    //  0  0  1  1   = 3 = R2_MASKNOTPEN  (03) DPna
                    //  0  1  0  0   = 4 = R2_NOTCOPYPEN  (04) PN
                    //  0  1  0  1   = 5 = R2_MASKPENNOT  (05) PDna
                    //  0  1  1  0   = 6 = R2_NOT         (06) Dn
                    //  0  1  1  1   = 7 = R2_XORPEN      (07) DPx
                    //  1  0  0  0   = 8 = R2_NOTMASKPEN  (08) DPan
                    //  1  0  0  1   = 9 = R2_MASKPEN     (09) DPa
                    //  1  0  1  0   = A = R2_NOTXORPEN   (0A) DPxn
                    //  1  0  1  1   = B = R2_NOP         (0B) D
                    //  1  1  0  0   = C = R2_MERGENOTPEN (0C) DPno
                    //  1  1  0  1   = D = R2_COPYPEN     (0D) P
                    //  1  1  1  0   = E = R2_MERGEPENNOT (0E) PDno
                    //  1  1  1  1   = F = R2_MERGEPEN    (0F) DPo
   UCHAR res   : 4; // b3 b2 b1 b0
   UCHAR color;     // b7 b6 b5 b4 b3 b2 b1 b0
} TWSPPEN256, *PTWSPPEN256;

/*****************************************************************************\
 * TWSPSTYLEMASK StyleMask; (present IFF Flags.style (b2-b4) = TW_LINE_MASK)
\*****************************************************************************/
typedef UCHAR TWSPSTYLEMASK, *PTWSPSTYLEMASK;

/*****************************************************************************\
 * TWSPSTYLESTATE StyleState; (present IFF Flags.style (b2-b4) != TW_LINE_SOLID)
\*****************************************************************************/
typedef UCHAR TWSPSTYLESTATE, *PTWSPSTYLESTATE;

/*****************************************************************************\
 * TWSPSTYLE     Style;     (present IFF Flags.style (b2-b4) = TW_LINE_OTHER)
\*****************************************************************************/
typedef struct _TWSPSTYLE {
   UCHAR   cData;
   USHORT  *pData;
} TWSPSTYLE, *PTWSPSTYLE;

/*****************************************************************************\
 * TWSPBOUNDS    Bounds;    (present IFF Flags.clipping (b0-b1) = TW_CLIP_RECT)
\*****************************************************************************/
typedef struct _TWSPBOUNDS {
   SHORT left;
   SHORT top;
   SHORT right;
   SHORT bottom;
} TWSPBOUNDS, *PTWSPBOUNDS;


/*****************************************************************************\
 * TWSPSUBPATH  SubPath; (Header for new subpath)
\*****************************************************************************/
typedef struct _TWSPSUBPATH {
   UCHAR  fLast      : 1;  // b0 TRUE if last sub path
   UCHAR  fSamePoint : 1;  // b1 TRUE if first point is same as previous point
   UCHAR  fBezier    : 1;  // b2   TRUE/FALSE True if bezier
   UCHAR  fClosed    : 1;  // b3   TRUE/FALSE True if closed figure
   UCHAR  fNewStart  : 1;  // b4   TRUE/FALSE True if new figure start
   UCHAR  fType      : 3;  // b7 b6 b5
#define TW_SP_NORMAL   0   //  0  0  0  no special attribute
#define TW_SP_HORZ     1   //  0  0  1  horizontal line (2 pts) 
#define TW_SP_VERT     2   //  0  1  0  vertical line (2 pts) 
#define TW_SP_DIAG     3   //  0  1  1  diagonal line (2 pts)
#define TW_SP_ELLIPSE  4   //  1  0  0  ellipse
                           //  1  0  1  reserved
                           //  1  1  0  reserved
#define TW_SP_ENDDATA  7   //  1  1  1  end data
} TWSPSUBPATH, *PTWSPSUBPATH;

                         // b1 b0   b1 = fNoFrac   b0 = fSigned
#define TW_PT_FIX      0 //  0  0   TWPOINTFIX     structure
#define TW_PT_FIXSIGN  1 //  0  1   TWPOINTFIXSIGN structure 
#define TW_PT_UNSIGN   2 //  1  0   TWPOINT        structure 
#define TW_PT_SIGN     3 //  1  1   TWPOINTSIGN    structure 
/*****************************************************************************\
 * TWPOINTFIX - For points with fractions & with positive coordinates
 *            Coordinate range: (0,0) to (2047 15/16, 1023 15/16)
\*****************************************************************************/
#define SIZE_OF_TWPOINTFIX sizeof(TWPOINTFIX)
typedef struct _TWPOINTFIX {
   ULONG fSigned  :  1; // b0   True if x and y are signed values (must be FALSE)
   ULONG fNoFracs :  1; // b1   TRUE/FALSE True if all points are integers
   ULONG fLast    :  1; // b2   TRUE/FALSE True if last point in subpath
   ULONG  x       : 15; // b3-b17
   ULONG  y       : 14; // b18-b31
} TWPOINTFIX, *PTWPOINTFIX;

/*****************************************************************************\
 * TWPOINTFIXSIGN - For points which don't fit TWPOINTFIX, TWPOINT,
 *                                          or TWPOINTSIGN
 *            Coordinate range: (-16384,-8192) to (16383 15/16,8191 15/16)
\*****************************************************************************/
#pragma pack(1)
#define SIZE_OF_TWPOINTFIXSIGN sizeof(TWPOINTFIXSIGN)
typedef struct _TWPOINTFIXSIGN {
   UCHAR fSigned  :  1; // b0   True if x and y are signed values (must be TRUE) 
   UCHAR fNoFracs :  1; // b1   TRUE/FALSE True if all points are integers
   UCHAR fLast    :  1; // b2   TRUE/FALSE True if last point in subpath
   UCHAR res      :  5; // b3-b7

   LONG  x;
   LONG  y;
} TWPOINTFIXSIGN, *PTWPOINTFIXSIGN;
#pragma pack()


/*****************************************************************************\
 * TWPOINT - For points without fractions & with positive coordinates
 *            Coordinate range: (0,0) to (2047,1023)
\*****************************************************************************/
#define SIZE_OF_TWPOINT 3
typedef struct _TWPOINT     {
   ULONG fSigned  :  1; // b0   True if x and y are signed values (must be FALSE) 
   ULONG fNoFracs :  1; // b1   TRUE/FALSE True if all points are integers
   ULONG fLast    :  1; // b2   TRUE/FALSE True if last point in subpath
   ULONG x        : 11; // b3-b13
   ULONG y        : 10; // b14-b23
   ULONG nu       :  8; // b24-b31 not used
} TWPOINT, *PTWPOINT;

/*****************************************************************************\
 * TWPOINTSIGN - For points without fractions & with negative coordinates
 *            Coordinate range: (-16384,-8192) to (16383,8191)
\*****************************************************************************/
#define SIZE_OF_TWPOINTSIGN sizeof(TWPOINTSIGN)
typedef struct _TWPOINTSIGN     {
   LONG fSigned  :  1; // b0   True if x and y are signed values (must be TRUE) 
   LONG fNoFracs :  1; // b1   TRUE/FALSE True if all points are integers
   LONG fLast    :  1; // b2   TRUE/FALSE True if last point in subpath
   LONG x        : 15; // b3-b17
   LONG y        : 14; // b18-b31
} TWPOINTSIGN, *PTWPOINTSIGN;

/*****************************************************************************\
 * TWPOINTSPECIAL
\*****************************************************************************/
typedef struct _TWPOINTSPECIAL     {
   SHORT fSamePoint :  1; // b1   TRUE/FALSE True if 1st pt same as previous
   SHORT fType      :  2; // b1 b0
//      TW_SP_NORMAL   0  //  0  0  diagonal line
//      TW_SP_HORZ     1  //  0  1  horizontal line
//      TW_SP_VERT     2  //  1  0  vertical line
   SHORT value      : 13; // b3-12
} TWPOINTSPECIAL, *PTWPOINTSPECIAL;

/*****************************************************************************\
 * TWRUN
\*****************************************************************************/
typedef struct _TWRUN {
   LONG fLast  : 1;      // b0 TRUE/FALSE True if last run entry
   LONG res    : 1;      // b1 reserved, must be 0
   LONG iStart : 15;     // b2-b16
   LONG iStop  : 15;     // b17-b31
} TWRUN;

typedef struct _ELLIPSEDATA
{
   SHORT    xLeft;
   SHORT    yTop;
   SHORT    xRight;
   SHORT    yBottom;
   SHORT    xBez1;
   SHORT    yBez2;
} ELLIPSEDATA, *PELLIPSEDATA;


// DrvTextOut stuff... HOST: textout.c   CLIENT: twtext.c
////////////////////////////////////////////////////////////////////////////

#define MAX_2K_CHUNKS_PER_GLYPH 32
#define TWTO_DEF_FG_COLOR_16  0x0   // Black
#define TWTO_DEF_BG_COLOR_16  0xF   // White
#define TWTO_DEF_DX_X         275   // First dx on bootup
#define TWTO_DEF_DX_Y          16   // First dy on bootup

// 256 color defs
#define TWTO_DEF_FG_COLOR_256 0x00  // Black
#define TWTO_DEF_BG_COLOR_256 0xFF  // White


/*****************************************************************************\
 * TWTOFLAGS       Flags;      
\*****************************************************************************/
// Use same bit for two mutually exclusive indicators
// (unions don't seem to work with bit fields)
#define bSameCharInc bDefPlacement
                            //                                         bytes to
                            // b7 b6                                    follow
#define TWTO_POS_FULL     0 //  0  0 - Full three byte position           3
#define TWTO_POS_SAME     1 //  0  1 - Same position as last string       0
#define TWTO_POS_SAMEY1DX 2 //  1  0 - Same Y, 1 byte dx from last string 1
#define TWTO_POS_SAMEXDY  3 //  1  1 - Same X, Y uses same delta as last  0
typedef struct _TWTOFLAGS {
   UCHAR bSameColor        : 1, // TRUE/FALSE  True if same FG/BG color as last
         bOpaqueBackground : 1, // TRUE/FALSE  True if opaque background
         bMonospaced       : 1, // TRUE/FALSE  True if monospaced       
         bDefPlacement     : 1, // True if pos is calc'd using cached widths
         bCalcWidth        : 1, // True if spacing same as cx                                    
         bRclDelta         : 1, // True 1B delta sent instead of full rcl                        
         fFirstPos         : 2; // Flags describing position of first glyph
} TWTOFLAGS, *PTWTOFLAGS;

/*****************************************************************************\
 * TWTOCOLOR       Color;      
\*****************************************************************************/
typedef struct _TWTOCOLOR {
   UCHAR FG : 4; // b3 b2 b1 b0 
                 //  I  R  G  B
                 //  0  0  0  0   = 0 = BLACK                                                    
                 //  0  0  0  1   = 1 = BLUE                                                     
                 //  0  0  1  0   = 2 = GREEN                                                    
                 //  0  0  1  1   = 3 = CYAN                                                     
                 //  0  1  0  0   = 4 = RED                                                      
                 //  0  1  0  1   = 5 = MAGENTA                                                  
                 //  0  1  1  0   = 6 = BROWN                                                    
                 //  0  1  1  1   = 7 = WHITE                                                    
                 //  1  0  0  0   = 8 = GRAY                                                     
                 //  1  0  0  1   = 9 = LIGHT BLUE                                               
                 //  1  0  1  0   = A = LIGHT GREEN                                              
                 //  1  0  1  1   = B = LIGHT CYAN                                               
                 //  1  1  0  0   = C = LIGHT RED                                                
                 //  1  1  0  1   = D = LIGHT MAGENTA                                            
                 //  1  1  1  0   = E = YELLOW                                                   
                 //  1  1  1  1   = F = WHITE (high intesity)                                    
   UCHAR BG : 4; // b3 b2 b1 b0 
                 //  I  R  G  B
                 //  0  0  0  0   = 0 = BLACK                                                    
                 //  0  0  0  1   = 1 = BLUE                                                     
                 //  0  0  1  0   = 2 = GREEN                                                    
                 //  0  0  1  1   = 3 = CYAN                                                     
                 //  0  1  0  0   = 4 = RED                                                      
                 //  0  1  0  1   = 5 = MAGENTA                                                  
                 //  0  1  1  0   = 6 = BROWN                                                    
                 //  0  1  1  1   = 7 = WHITE                                                    
                 //  1  0  0  0   = 8 = GRAY                                                     
                 //  1  0  0  1   = 9 = LIGHT BLUE                                               
                 //  1  0  1  0   = A = LIGHT GREEN                                              
                 //  1  0  1  1   = B = LIGHT CYAN                                               
                 //  1  1  0  0   = C = LIGHT RED                                                
                 //  1  1  0  1   = D = LIGHT MAGENTA                                            
                 //  1  1  1  0   = E = YELLOW                                                   
                 //  1  1  1  1   = F = WHITE (high intesity)                                    
} TWTOCOLOR, *PTWTOCOLOR;


/*****************************************************************************\
 * TWTOCOLOR256       Color;      
 *
 * This is the 256 color version of TWTOCOLOR struct.
 *
\*****************************************************************************/
typedef struct _TWTOCOLOR256 {
   UCHAR FG;     // b7 b6 b5 b4 b3 b2 b1 b0 
   UCHAR BG;     // b7 b6 b5 b4 b3 b2 b1 b0 
} TWTOCOLOR256, *PTWTOCOLOR256;


/*****************************************************************************\
 * TWTOGLYPHHEADER GlyphHeader;
\*****************************************************************************/
#define SIZE_OF_GLYPHHEADER       3
#define SIZE_OF_GLYPHHEADERMEDIUM 2
#define SIZE_OF_HCACHE_EXTRA 2 
#define HCACHE_VALID_BITS 0x0FFF
typedef struct _TWTOGLYPHHEADER {
   ULONG  bShortFormat    : 1,  // TRUE/FALSE True if GLYPHHEADERSHORT structure (must be first)
          fMediumFormat   : 2,  // see TWTO_MF_xxx defines (below)
          bTotallyClipped : 1,  // TRUE/FALSE True if glyph is totally clipped (must be in 1st byte)
          bLastGlyph      : 1,  // TRUE/FALSE True if last, False if another follows (must be in 1st byte) 
          bhCacheCache    : 1,  // TRUE/FALSE True if hCacheCache follows
          bGetFromCache   : 1,  // TRUE/FALSE True if we should get from cache
          bPutInCache     : 1,  // TRUE/FALSE True if we should put in cache

          bWidthIncluded  : 1,  // TRUE/FALSE True if dxNext in GlyphDimension or follows
          hCache          : 12, // value of the client cache handle (low 8 bits hCache must be in low word header)
          ChunkType       : 2,  // _2k, _512B, _128B, _32B
          res             : 1,  // set to 1 to avoid ESC char conflict

          notused         : 8;
} TWTOGLYPHHEADER, *PTWTOGLYPHHEADER;

/*****************************************************************************\
 * TWTOGH GH;  // Local copy of GlyphHeader w/o bit fields - for performance
\*****************************************************************************/
typedef struct _TWTOGH {
   UCHAR      bLastGlyph;      // TRUE/FALSE True if last, False if another follows (must be in 1st byte) 
   UCHAR      bGetFromCache;   // TRUE/FALSE True if we should get from cache
   UCHAR      bPutInCache;     // TRUE/FALSE True if we should put in cache
   UCHAR      bWidthIncluded;  // TRUE/FALSE True if dxNext in GlyphDimension or follows
   USHORT     hCache;          // value of the client cache handle (low 8 bits hCache must be in low word header)
   CHUNK_TYPE ChunkType;       // _2k, _512B, _128B, _32B
} TWTOGH, *PTWTOGH;

/*****************************************************************************\
 * TWTOGLYPHHEADERMEDIUM GlyphHeaderMedium;
\*****************************************************************************/
                                // b2 b1
#define TWTO_MF_NORMAL      0   //  0  0 - normal 3 byte format
#define TWTO_MF_TRUNCATE    1   //  0  1 - 2 byte format hCache is really hCacheCache
#define TWTO_MF_MEDIUM      2   //  1  0 - 2 byte format - TWTOGLYPHHEADERMEDIUM structure
#define TWTO_MF_SPACES      3   //  1  1 - 1 byte format - TWTOGLYPHHEADERSPACES structure
typedef struct _TWTOGLYPHHEADERMEDIUM {
   USHORT bShortFormat    : 1,  // TRUE/FALSE True if GLYPHHEADERSHORT structure (must be first)
          fMediumFormat   : 2,  // see TWTO_MF_xxx defines (above)
          hCache          : 12, // value of the client cache handle (low 8 bits hCache must be in low word header)
          bTiny           : 1;  // TRUE: _32B ChunkType, FALSE: _128B
} TWTOGLYPHHEADERMEDIUM, *PTWTOGLYPHHEADERMEDIUM;

/*****************************************************************************\
 * TWTOGLYPHHEADERSPACES GlyphHeaderSpaces;
\*****************************************************************************/
typedef struct _TWTOGLYPHHEADERSPACES {
   UCHAR  bShortFormat    : 1,  // TRUE/FALSE True if GLYPHHEADERSHORT structure (must be first)
          fMediumFormat   : 2,  // see TWTO_MF_xxx defines (above)
          count           : 5;  // number of contiguous spaces
} TWTOGLYPHHEADERSPACES, *PTWTOGLYPHHEADERSPACES;

/*****************************************************************************\
 * TWTOGLYPHHEADERSHORT GlyphHeaderShort;
\*****************************************************************************/
typedef struct _TWTOGLYPHHEADERSHORT {
   UCHAR  bShortFormat    : 1; // TRUE/FALSE True if GLYPHHEADERSHORT structure (must be first) 
   UCHAR  hCacheCache     : 7; // value of the client cache handle
} TWTOGLYPHHEADERSHORT, *PTWTOGLYPHHEADERSHORT;

/*****************************************************************************\
 * TWTOGLYPHDIMENSION  GlyphDimension;
\*****************************************************************************/
#define SIZE_OF_GLYPHDIMENSION 6
#define SIZE_OF_GLYPHDIMENSION_WITH_WIDTH 7
typedef struct _TWTOGLYPHDIMENSION {
   ULONG cx              : 11;
   ULONG cy              : 10;
   LONG  dyThis          : 11;  // Amount to be added to yBase = yOffset + iStart


   // If glyphs g1 and g2 have x positions of x1 and x2, respectively,
   // x2 = x1 + dxNext1 + dxThis2
   LONG  bColumnRemoved :  1; 
   LONG  dxThis         : 12;  // This char adds this to cur x pos
   ULONG dxNext         : 11;  // Next char adds this to cur x pos (opt - last)
   LONG  notused        :  8; 
} TWTOGLYPHDIMENSION, *PTWTOGLYPHDIMENSION;

/*****************************************************************************\
 * TWTOGD  GD; // Local copy of GlyphDimension w/o bit fields - for performance 
\*****************************************************************************/
typedef struct _TWTOGD {
   USHORT cx;
   USHORT cy;
   SHORT  dyThis;  // Amount to be added to yBase = yOffset + iStart
   SHORT  dxThis;  // This char adds this to cur x pos
   USHORT dxNext;  // Next char adds this to cur x pos (opt - last)
} TWTOGD, *PTWTOGD;

/*****************************************************************************\
 * TWTOPOSITION Position;
\*****************************************************************************/
#define SIZE_OF_POSITION 3
typedef struct _TWTOPOSITION {
   LONG bShortFormat : 1,
        x            : 12,
        y            : 11,
        notused      : 8;
} TWTOPOSITION, *PTWTOPOSITION;

typedef struct _TWUNPACKTOPOSITION {
    short bShortFormat;
    short x;
    short y;
    short notused;
} TWUNPACKTOPOSITION, *PTWUNPACKTOPOSITION;

/*****************************************************************************\
 * TWTOPOSITIONSHORT PositionShort;
\*****************************************************************************/
typedef struct _TWTOPOSITIONSHORT {
   CHAR bShortFormat : 1,
        delta        : 7;
} TWTOPOSITIONSHORT, *PTWTOPOSITIONSHORT;


/*****************************************************************************\
 * TWTOPT1RCL      pt1Rcl;    ( 3 bytes )
\*****************************************************************************/
typedef struct _TWTOPT1RCL {
                            //                                         Bytes to
   ULONG fType   : 3;       // b2 b1 b0                                  follow
#define TW_PT1RCL_SAME    0 //  0  0  0 -   same delta X,   same delta Y   0
#define TW_PT1RCL_SAMEYX1 1 //  0  0  1 -     1B delta X,   same delta Y   1
#define TW_PT1RCL_11X5Y   2 //  0  1  0 - 11-bit delta X,  5-bit delta Y   2
#define TW_PT1RCL_11X10Y  3 //  0  1  1 - 11-bit delta X, 10-bit delta Y   3
// The following are used for 3-bit types
#define TW_PT1RCL_SAMEYX2 4 //  1  0  0 -     2B delta X,   same delta Y   2
#define TW_PT1RCL_10X6Y   5 //  1  0  1 - 10-bit delta X,  6-bit delta Y   2
#define TW_PT1RCL_9X7Y    6 //  1  1  0 -  9-bit delta X,  7-bit delta Y   2
#define TW_PT1RCL_8X8Y    7 //  1  1  1 -  8-bit delta X,  8-bit delta Y   2
   ULONG xLeft   : 11; // x - coordinate
   ULONG yTop    : 10; // y - coordinate
   ULONG notused : 8;
} TWTOPT1RCL, *PTWTOPT1RCL;

/*****************************************************************************\
 * TWTOPT1RCLE      pt1RclE;    ( 3 bytes )
\*****************************************************************************/
typedef struct _TWTOPT1RCLE {
   ULONG bMore   : 1;  // TRUE/FALSE  True if not last rectangle
                            //                                      Bytes to
   ULONG fType   : 2;       // b1 b0                                  follow
     // TW_PT1RCL_SAME          0  0 -   same delta X,   same delta Y   0
     // TW_PT1RCL_SAMEYX1       0  1 -     1B delta X,   same delta Y   1
     // TW_PT1RCL_11X5Y         1  0 - 11-bit delta X,  5-bit delta Y   2
     // TW_PT1RCL_11X10Y        1  1 - 11-bit delta X, 10-bit delta Y   3
   ULONG xLeft   : 11; // x - coordinate
   ULONG yTop    : 10; // y - coordinate
   ULONG notused : 8;
} TWTOPT1RCLE, *PTWTOPT1RCLE;

/*****************************************************************************\
 * TWPT11X5Y pt2Rcl;    ( 2 bytes )
\*****************************************************************************/
typedef struct _TWPT11X5Y {
   USHORT dx : 11; // x - coordinate delta
   USHORT dy :  5; // y - coordinate delta
} TWPT11X5Y, *PTWPT11X5Y;

/*****************************************************************************\
 * TWPT10X6Y pt2Rcl;    ( 2 bytes )
\*****************************************************************************/
typedef struct _TWPT10X6Y {
   USHORT dx : 10; // x - coordinate delta
   USHORT dy :  6; // y - coordinate delta
} TWPT10X6Y, *PTWPT10X6Y;

/*****************************************************************************\
 * TWPT9X7Y pt2Rcl;    ( 2 bytes )
\*****************************************************************************/
typedef struct _TWPT9X7Y {
   USHORT dx : 9; // x - coordinate delta
   USHORT dy : 7; // y - coordinate delta
} TWPT9X7Y, *PTWPT9X7Y;

/*****************************************************************************\
 * TWPT8X8Y pt2Rcl;    ( 2 bytes )
\*****************************************************************************/
typedef struct _TWPT8X8Y {
   USHORT dx : 8; // x - coordinate delta
   USHORT dy : 8; // y - coordinate delta
} TWPT8X8Y, *PTWPT8X8Y;

// Used by stroke
#define TW_PT1RCL_3X5Y    0 //  0  0  0 -  3-bit delta X,  5-bit delta Y   1
#define TW_PT1RCL_5X3Y    1 //  0  0  1 -  5-bit delta X,  3-bit delta Y   1
#define TW_PT1RCL_4X4Y    4 //  1  0  0 -  4-bit delta X,  4-bit delta Y   1
#define TW_PT1RCL_6X10Y   6 //  1  1  0 -  6-bit delta X, 10-bit delta Y   2
/*****************************************************************************\
 * TWPT6X10Y pt2Rcl;    ( 2 bytes )
\*****************************************************************************/
typedef struct _TWPT6X10Y {
   USHORT dx : 6; // x - coordinate delta
   USHORT dy : 10; // y - coordinate delta
} TWPT6X10Y, *PTWPT6X10Y;

/*****************************************************************************\
 * TWPT4X4Y pt2Rcl;    ( 1 byte )
\*****************************************************************************/
typedef struct _TWPT4X4Y {
   USHORT dx : 4; // x - coordinate delta
   USHORT dy : 4; // y - coordinate delta
} TWPT4X4Y, *PTWPT4X4Y;

/*****************************************************************************\
 * TWPT3X5Y pt2Rcl;    ( 1 byte )
\*****************************************************************************/
typedef struct _TWPT3X5Y {
   USHORT dx : 3; // x - coordinate delta
   USHORT dy : 5; // y - coordinate delta
} TWPT3X5Y, *PTWPT3X5Y;

/*****************************************************************************\
 * TWPT5X3Y pt2Rcl;    ( 1 byte )
\*****************************************************************************/
typedef struct _TWPT5X3Y {
   USHORT dx : 5; // x - coordinate delta
   USHORT dy : 3; // y - coordinate delta
} TWPT5X3Y, *PTWPT5X3Y;

/*****************************************************************************\
 * TWPT11X10Y pt2Rcl;    ( 3 bytes )
\*****************************************************************************/
typedef struct _TWPT11X10Y {
   ULONG dx      : 11; // x - coordinate delta
   ULONG dy      : 10; // y - coordinate delta
   ULONG res     :  3;
   ULONG notused :  8;
} TWPT11X10Y, *PTWPT11X10Y;

// Determine size of pt2Rcl
#define GETNEXTPTSIZE( pt1Rcl, pSize )          \
switch ( pt1Rcl.fType ) {                       \
   case TW_PT1RCL_SAME:    *(pSize) = 0; break; \
   case TW_PT1RCL_SAMEYX1: *(pSize) = 1; break; \
   case TW_PT1RCL_11X10Y:  *(pSize) = 3; break; \
   default:                *(pSize) = 2; break; \
}

#define GETNEXTPTSIZE2( pt1Rcl, pSize )         \
switch ( pt1Rcl.fType ) {                       \
   case TW_PT1RCL_3X5Y:                         \
   case TW_PT1RCL_4X4Y:                         \
   case TW_PT1RCL_5X3Y:    *(pSize) = 1; break; \
   case TW_PT1RCL_11X10Y:  *(pSize) = 3; break; \
   default:                *(pSize) = 2; break; \
}

// The following is for complex clipping rectangles for textout

                                    //                                   Bytes 
                                    // b1 b0                             Total
#define TWTO_RCLCLIP_4L3T4R3B     0 //  0  0 - dL:4  dT:3  dR:4  dB:3      2     
                                    // 2 ctl +  4 +  3 +  4 +  3 = 16 bits
                                    // signed deltas:  4 bit =    -8 to 7
                                    //                 3 bit =    -4 to 3 
#define TWTO_RCLCLIP_6L5T6R5B     1 //  0  1 - dL:6  dT:5  dR:6  dB:5      3     
                                    // 2 ctl +  6 +  5 +  6 +  5 = 24 bits
                                    // signed deltas:  6 bit =   -32 to 31 
                                    //                 5 bit =   -16 to 15 
#ifdef OPTIMIZE_CLIPPED_ENDS
#define TWTO_RCLCLIP_8L7T8R7B     2 //  1  0 - dL:8  dT:7  dR:8  dB:7      4     
                                    // 2 ctl +  8 +  7 +  8 +  7 = 32 bits
                                    // signed deltas:  8 bit =  -128 to 127
                                    //                 7 bit =   -64 to 63 
#else
#define TWTO_RCLCLIP_11L0T11R0B   2 //  1  0 - dL:11  dT:0  dR:11 dB:0     3     
                                    // 2 ctl +  11 + 0 +  11 +  0 = 24 bits
                                    // unsigned deltas:  11 bits = 0 to 2047
#endif
#define TWTO_RCLCLIP_12L11T12R11B 3 //  1  1 - dL:12 dT:11 dR:12 dB:11     6     
                                    // 2 ctl + 12 + 11 + 12 + 11 = 48 bits
                                    // signed deltas: 12 bit = -2048 to 2047 
                                    //                11 bit = -1024 to 1023 





/*****************************************************************************\
 * TW4L3T4R3B ccRcl;    ( 2 bytes )
\*****************************************************************************/
typedef struct _TW4L3T4R3B {
   SHORT fType   : 2,
         dL      : 4,
         dT      : 3,
         dR      : 4,
         dB      : 3;
} TW4L3T4R3B, *PTW4L3T4R3B;

/*****************************************************************************\
 * TW6L5T6R5B ccRcl;    ( 3 bytes )
\*****************************************************************************/
#define SIZE_OF_TW6L5T6R5B 3
typedef struct _TW6L5T6R5B {
   LONG  fType   : 2,
         dL      : 6,
         dT      : 5,
         dR      : 6,
         dB      : 5,
         notused : 8;
} TW6L5T6R5B, *PTW6L5T6R5B;

#ifdef OPTIMIZE_CLIPPED_ENDS
/*****************************************************************************\
 * TW8L7T8R7B ccRcl;    ( 4 bytes )
\*****************************************************************************/
typedef struct _TW8L7T8R7B {
   LONG  fType   : 2,
         dL      : 8,
         dT      : 7,
         dR      : 8,
         dB      : 7;
} TW8L7T8R7B, *PTW8L7T8R7B;
#else
/*****************************************************************************\
 * TW11L0T11R0B ccRcl;    ( 3 bytes )
\*****************************************************************************/
#define SIZE_OF_TW11L0T11R0B 3
typedef struct _TW11L0T11R0B {
  ULONG  fType   : 2,
         dL      : 11,
         dR      : 11;
} TW11L0T11R0B, *PTW11L0T11R0B;
#endif

/*****************************************************************************\
 * TW12L11T12R11B ccRcl;    ( 6 bytes )
\*****************************************************************************/
#pragma pack(1)
typedef struct _TW12L11T12R11B {
   LONG  fType   : 2,
         dL      : 12,
         dT      : 11,
         dRLow   : 7;

   SHORT dRHigh  : 5,
         dB      : 11;
} TW12L11T12R11B, *PTW12L11T12R11B;
#pragma pack()

// We will allow up to 255 complex clipping rectangles
// This is the largest number of uncompressed rcls which can fit in our
// temporary buffer, vpenr (static_buffer_2), which is LARGE_CACHE_CHUNK_SIZE
// 255 * sizeof(RECTI) + sizeof(enr.c) = 2041 (< 2048)
#define TWTO_RCLCLIP_LIMIT 255

//Trace Mask Defines
//These are for host compatibility - otherwise they are obsolete
#define CTXDBG_ERROR        TT_TW_ERROR 
#define CTXDBG_BLTTRICK     TT_TW_BLTTRICK 
#define CTXDBG_ENTRYEXIT    TT_TW_ENTRY_EXIT
#define CTXDBG_TEXT         TT_TW_TEXT
#define CTXDBG_XLATEOBJ     TT_TW_XLATEOBJ  
#define CTXDBG_BRUSH        TT_TW_BRUSH 
#define CTXDBG_DITHER       TT_TW_DITHER 
#define CTXDBG_STROKE       TT_TW_STROKE
#define CTXDBG_FILL         TT_TW_FILL 
#define CTXDBG_PAINT        TT_TW_PAINT 
#define CTXDBG_BLT          TT_TW_BLT
#define CTXDBG_COPY         TT_TW_COPY 
#define CTXDBG_PTRSHAPE     TT_TW_PTRSHAPE 
#define CTXDBG_PTRMOVE      TT_TW_PTRMOVE
#define CTXDBG_RLESTATS     TT_TW_RLESTATS 
#define CTXDBG_CLIPCMPLX    TT_TW_CLIPCMPLX 
#define CTXDBG_PROTOCOLDATA TT_TW_PROTOCOLDATA
#define CTXDBG_SSB          TT_TW_SSB
#define CTXDBG_CACHE        TT_TW_CACHE
#define CTXDBG_LOWCACHE     TT_TW_LOWCACHE 
#define CTXDBG_CONNECT      TT_TW_CONNECT 
#define CTXDBG_PALETTE      TT_TW_PALETTE
#define CTXDBG_DIM          TT_TW_DIM


/*****************************************************************************\
 * TWPALETTEHEADER  PaletteHeader
\*****************************************************************************/
                                    // b15 b14
#define TWPAL_NOT_CACHED        0   //  0   0 - Not cached
#define TWPAL_TO_CACHE          1   //  0   1 - Store to cache
#define TWPAL_FROM_CACHE        2   //  1   0 - Retreive from cache

typedef struct _TWPALETTEHEADER {
   USHORT iStart       : 8,     // b0-b7   starting index of paletteu
          res1         : 6,     // b8-b13  reserved
          fType        : 2;     // b14-b15 cache type
} TWPALETTEHEADER, *PTWPALETTEHEADER;

                                    // b1 b0
#define TWPAL_CHUNK_128B        0   //  0  0 - 128 byte cache object
#define TWPAL_CHUNK_512B        1   //  0  1 - 512 byte cache object 
#define TWPAL_CHUNK_2K          2   //  1  0 - 2K  byte cache object

typedef struct _TWPALETTEHEADERCACHE {
   USHORT ChunkType    : 2,     // b0-b1  0-128B, 1-512B chunk, 2-2K chunk
          res2         : 2,     // b2-b3  reserved
          hCache       : 12;    // b4-b15 value of the client cache handle 
} TWPALETTEHEADERCACHE, *PTWPALETTEHEADERCACHE;


/*****************************************************************************\
 * Format for PACKET_COMMAND_CACHE
 *
 *
\*****************************************************************************/

#define PACKET_COMMAND_CACHE_ERROR        1
#define PACKET_COMMAND_CACHE_RESIZE       2
#define PACKET_COMMAND_CACHE_DELETE       3
#define PACKET_COMMAND_CACHE_STREAM       4
#define PACKET_COMMAND_CACHE_DISABLE      5
#define PACKET_COMMAND_CACHE_ENABLE       6


#define CACHE_FILE_HANDLE_SIZE 8
typedef UCHAR CACHE_FILE_HANDLE[ CACHE_FILE_HANDLE_SIZE ];

#pragma pack(1)
typedef struct _ICA_CACHE_ERROR {
    USHORT ByteCount;
    UCHAR Command;
    CACHE_FILE_HANDLE FileHandle;
} ICA_CACHE_ERROR, *PICA_CACHE_ERROR;
#pragma pack()


#pragma pack(1)
typedef struct _ICA_CACHE_RESIZE {
    USHORT ByteCount;
    UCHAR Command;
    ULONG Size;
} ICA_CACHE_RESIZE, *PICA_CACHE_RESIZE;
#pragma pack()


#pragma pack(1)
typedef struct _ICA_CACHE_DELETE {
    USHORT ByteCount;
    UCHAR Command;
    CACHE_FILE_HANDLE FileHandle;
} ICA_CACHE_DELETE, *PICA_CACHE_DELETE;
#pragma pack()


#pragma pack(1)
typedef struct _CACHE_FILE_CONTEXT {
    CACHE_FILE_HANDLE Filehandle;
    ULONG Size;
#define CACHE_FILE_READONLY 0x1
    UCHAR Flags;
    UCHAR SignatureLevel;
} CACHE_FILE_CONTEXT, *PCACHE_FILE_CONTEXT;
#pragma pack()


#pragma pack(1)
typedef struct _ICA_CACHE_STREAM {  
    USHORT ByteCount;
    UCHAR  Command;
    UCHAR  Count;
    CACHE_FILE_CONTEXT FileList[0];
} ICA_CACHE_STREAM, *PICA_CACHE_STREAM;
#pragma pack()


#pragma pack(1)
typedef struct _ICA_CACHE_DISABLE {
    USHORT ByteCount;
    UCHAR Command;
} ICA_CACHE_DISABLE, *PICA_CACHE_DISABLE;
#pragma pack()


#pragma pack(1)
typedef struct _ICA_CACHE_ENABLE {
    USHORT ByteCount;
    UCHAR Command;
} ICA_CACHE_ENABLE, *PICA_CACHE_ENABLE;
#pragma pack()


#endif //__TWCOMMON_H__
