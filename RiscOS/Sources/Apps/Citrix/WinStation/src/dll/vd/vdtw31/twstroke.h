
/******************************Module*Header*******************************\
*
*   TWSTROKE.H
*
*   Thin Wire Windows - Line drawing constants and structures.
*
*   NOTE: This file mirrors TWSTROKES.INC.  Changes here must be reflected in
*   the .inc file!
*
*   Copyright (c) Citrix Systems Inc. 1993-1996
*
*   Author: Marc Bloomfield (marcb) 21-Dec-1993
*
*   $Log$
*  
*     Rev 1.3   15 Apr 1997 18:16:26   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   08 May 1996 14:48:48   jeffm
*  update
*  
*     Rev 1.1   03 Jan 1996 13:35:16   kurtp
*  update
*  
\**************************************************************************/

#ifndef __TWSTROKE_H__
#define __TWSTROKE_H__


#ifdef DOS

#define LOWORD(l)           ((WORD)(DWORD)(l))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))
#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))

#endif

typedef struct _STRIP {

// Updated by strip drawers:

   BYTE far *      pjScreen;       // Points to the first pixel of the line
   BYTE            jBitMask;       // Bit making for pixel in byte
   BYTE            jFiller1[3];    //   jBitMask sometimes treated as a ULONG
   BYTE            jStyleMask;     // Are we working on a gap in the style?
   BYTE            jFiller2[3];    //   jStyleMask sometimes treated as a ULONG

   STYLEPOS far*   psp;            // Pointer to current style entry
   STYLEPOS        spRemaining;    // To go in current style

// Not modified by strip drawers:

   LONG            lNextScan;      // Signed increment to next scan
   ULONG           plStripEnd;
   LONG            flFlips;        // Indicates if line goes up or down
   STYLEPOS far*   pspStart;       // Pointer to start of style array
   STYLEPOS far*   pspEnd;         // Pointer to end of style array
   ULONG           ulBitmapROP;                   
   ULONG           xyDensity;      // Density of style

//  extra stuff for WINxx
#ifndef DOS
    LONG   cStrips;               // # of strips in array
    POINTL ptlStart;             // first point
#endif

// We leave room for a couple of extra dwords at the end of the strips
// array that can be used by the strip drawers:

   LONG            alStrips[STRIP_MAX + 2]; // Array of strips
} STRIP;


typedef ULONG           IDENT;
//typedef ULONG           DWORD;
typedef ULONG FLONG;


//typedef void FAR                *LPVOID;
typedef HANDLE                  HDEV;
typedef HANDLE                  HSURF;


// The XYPAIR structure is used to allow ATOMIC read/write of the cursor
// position.  NEVER, NEVER, NEVER get or set the cursor position without
// doing this.  There is no semaphore protection of this data structure,
// nor will there ever be any. [donalds]

typedef struct  _XYPAIR
{
    USHORT  x;
    USHORT  y;
} XYPAIR;


typedef struct  tagDEVINFO
{
    FLONG       flGraphicsCaps;
    LOGFONTW     lfDefaultFont;
    LOGFONTW     lfAnsiVarFont;
    LOGFONTW     lfAnsiFixFont;
    ULONG       cFonts;
    ULONG       iDitherFormat;
    USHORT      cxDither;
    USHORT      cyDither;
    HPALETTE    hpalDefault;
} DEVINFO, *PDEVINFO;

#define HS_DDI_MAX 19

typedef LONG  LDECI4;

typedef struct _CIECHROMA
{
    LDECI4   x;
    LDECI4   y;
    LDECI4   Y;
}CIECHROMA;

typedef struct _COLORINFO
{
    CIECHROMA  Red;
    CIECHROMA  Green;
    CIECHROMA  Blue;
    CIECHROMA  Cyan;
    CIECHROMA  Magenta;
    CIECHROMA  Yellow;
    CIECHROMA  AlignmentWhite;

    LDECI4  RedGamma;
    LDECI4  GreenGamma;
    LDECI4  BlueGamma;

    LDECI4  MagentaInCyanDye;
    LDECI4  YellowInCyanDye;
    LDECI4  CyanInMagentaDye;
    LDECI4  YellowInMagentaDye;
    LDECI4  CyanInYellowDye;
    LDECI4  MagentaInYellowDye;
}COLORINFO, *PCOLORINFO;


typedef struct _twSIZE
{
    LONG        cx;
    LONG        cy;
} twSIZE, *PtwSIZE, *LPtwSIZE;

typedef twSIZE               twSIZEL;
typedef twSIZE               *PtwSIZEL;

typedef struct _GDIINFO
{
    ULONG ulVersion;
    ULONG ulTechnology;
    ULONG ulHorzSize;
    ULONG ulVertSize;
    ULONG ulHorzRes;
    ULONG ulVertRes;
    ULONG cBitsPixel;
    ULONG cPlanes;
    ULONG ulNumColors;
    ULONG flRaster;
    ULONG ulLogPixelsX;
    ULONG ulLogPixelsY;
    ULONG flTextCaps;

    ULONG ulDACRed;
    ULONG ulDACGreen;
    ULONG ulDACBlue;

    ULONG ulAspectX;
    ULONG ulAspectY;
    ULONG ulAspectXY;

    LONG xStyleStep;
    LONG yStyleStep;
    LONG denStyleStep;

    POINTL ptlPhysOffset;
    twSIZEL  szlPhysSize;

    ULONG ulNumPalReg;

// These fields are for halftone initialization.

    COLORINFO ciDevice;
    ULONG     ulDevicePelsDPI;
    ULONG     ulPrimaryOrder;
    ULONG     ulHTPatternSize;
    ULONG     ulHTOutputFormat;
    ULONG     flHTFlags;

} GDIINFO, *PGDIINFO;

//
// VIDEO_IOCTL_QUERY_POINTER_ATTR - Returns all attributes of the pointer.
//
// VIDEO_IOCTL_SET_POINTER_ATTR - Is used to set the attributes of the
//                                pointer.
//
// Information used by this function is passed using the following structure:
//

typedef struct _VIDEO_POINTER_ATTRIBUTES {
    ULONG Flags;
    ULONG Width;
    ULONG Height;
    ULONG WidthInBytes;
    ULONG Enable;
    SHORT Column;
    SHORT Row;
    UCHAR Pixels[1];
} VIDEO_POINTER_ATTRIBUTES, *PVIDEO_POINTER_ATTRIBUTES;

//
// VIDEO_IOCTL_QUERY_POINTER_CAPABILITIES - Returns capabilities of miniport
//                                          hardware cursor
//

typedef struct _VIDEO_POINTER_CAPABILITIES {
    ULONG Flags;
    ULONG MaxWidth;
    ULONG MaxHeight;
    ULONG HWPtrBitmapStart;
    ULONG HWPtrBitmapEnd;
} VIDEO_POINTER_CAPABILITIES, *PVIDEO_POINTER_CAPABILITIES;

#define HBITMAP  HANDLE

typedef struct  _PDEV
{
    FLONG   fl;                         // Driver flags (DRIVER_xxx)
    IDENT   ident;                      // Identifier
    HANDLE  hDriver;                    // Handle to the miniport
    HDEV    hdevEng;                    // Engine's handle to PDEV
    HSURF   hsurfEng;                   // Engine's handle to surface
    PVOID   pdsurf;                     // Associated surface
    twSIZEL   sizlSurf;                   // Displayed size of the surface
    DWORD   ulIs386;                    // 1 if 386, 0 if 486 or higher
    PBYTE   pjScreen;                   // Pointer to the frame buffer base
    XYPAIR  xyCursor;                   // Where the cursor should be
    POINTL  ptlExtent;                  // Cursor extent
    ULONG   cExtent;                    // Effective cursor extent.
    XYPAIR  xyHotSpot;                  // Cursor hot spot
    FLONG   flCursor;                   // Cursor status
    DEVINFO devinfo;                    // Device info
    HBITMAP ahbmPat[HS_DDI_MAX];        // Engine handles to standard patterns
    GDIINFO GdiInfo;                    // Device capabilities
    ULONG   ulModeNum;                  // Mode index for current VGA mode
    PVIDEO_POINTER_ATTRIBUTES pPointerAttributes; // HW Pointer Attributes
    ULONG   XorMaskStartOffset;         // Start offset of hardware pointer
                                        //  XOR mask relative to AND mask for
                                        //  passing to HW pointer
    DWORD   cjPointerAttributes;        // Size of buffer allocated
    DWORD   flPreallocSSBBufferInUse;   // True if preallocated saved screen
                                        //  bits buffer is in use
    PUCHAR  pjPreallocSSBBuffer;        // Pointer to preallocated saved screen
                                        //  bits buffer, if there is one
    ULONG   ulPreallocSSBSize;          // Size of preallocated saved screen
                                        //  bits buffer
    VIDEO_POINTER_CAPABILITIES PointerCapabilities; // HW pointer abilities
    PUCHAR  pucDIB4ToVGAConvBuffer;     // Pointer to DIB4->VGA conversion
                                        //  table buffer
    PUCHAR  pucDIB4ToVGAConvTables;     // Pointer to DIB4->VGA conversion
                                        //  table start in buffer (must be on a
                                        //  256-byte boundary)
} PDEV, *PPDEV;

//typedef unsigned int size_t;
typedef size_t SIZE_T;

//
// Returned in output buffer.
//

typedef struct _VIDEO_BANK_SELECT {
    ULONG Length;
    ULONG Size;
    ULONG BankingFlags;
    ULONG BankingType;
    ULONG PlanarHCBankingType;
    ULONG BitmapWidthInBytes;
    ULONG BitmapSize;
    ULONG Granularity;
    ULONG PlanarHCGranularity;
    ULONG CodeOffset;
    ULONG PlanarHCBankCodeOffset;
    ULONG PlanarHCEnableCodeOffset;
    ULONG PlanarHCDisableCodeOffset;
} VIDEO_BANK_SELECT, *PVIDEO_BANK_SELECT;

//
// Stored in the BankType and PlanarHCBankintType fields
//

typedef enum _VIDEO_BANK_TYPE {
    VideoNotBanked = 0,
    VideoBanked1RW,
    VideoBanked1R1W,
    VideoBanked2RW,
    NumVideoBankTypes
} VIDEO_BANK_TYPE, *PVIDEO_BANK_TYPE;


/**************************************************************************\
*
* Bank clipping info
*
\**************************************************************************/

typedef struct {
    RECTL rclBankBounds;    // describes pixels addressable in this bank
    ULONG ulBankOffset;     // offset of bank start from bitmap start, if
                            // the bitmap were linearly addressable
} BANK_INFO, *PBANK_INFO;

/**************************************************************************\
*
* Specifies desired justification for requestion scan line within bank window
*
\**************************************************************************/

typedef enum {
    JustifyTop = 0,
    JustifyBottom
} BANK_JUST;

typedef struct _DEVSURF {
    SIZE_T      lNextScan;              // Offset from scan  "n" to "n+1"
    RECTL       rcl1WindowClip;         // Single-window banking clip rect
    PVOID       pvBitmapStart;          // Single-window bitmap start pointer
                                        //  (adjusted as necessary to make
                                        //  window map in at proper offset)
} DEVSURF, * PDEVSURF;

typedef struct _RUN {
   ULONG       iStart;
   ULONG       iStop;
} RUN;




typedef LONG FIX;

typedef struct  _POINTFIX
{
    FIX   x;
    FIX   y;
} POINTFIX, *PPOINTFIX;


/*****************************************************************************\
 * TWPOINTFIXI
\*****************************************************************************/
typedef struct _TWPOINTFIXI {
   SHORT x;
   SHORT y;
} TWPOINTFIXI, *PTWPOINTFIXI;


typedef SHORT TWFIX;


/*****************************************************************************\
 * TWRUNI
\*****************************************************************************/
typedef struct _TWRUNI {
   SHORT iStart;
   SHORT iStop;
} TWRUNI;

// External calls

VOID far vSetStrips(ULONG, ULONG);
VOID far vClearStrips(ULONG);

// For two-pass ROPs:

VOID far vCatchTwoPass(STRIP*, LINESTATE*, LONG*);


// Prototypes to go to the screen:

#ifdef DOS

VOID far vStripSolidHorizontal(STRIP*, LINESTATE*, LONG*);
VOID far vStripSolidHorizontalSet(STRIP*, LINESTATE*, LONG*);
VOID far vStripSolidVertical(STRIP*, LINESTATE*, LONG*);
VOID far vStripSolidDiagonalHorizontal(STRIP*, LINESTATE*, LONG*);
VOID far vStripSolidDiagonalVertical(STRIP*, LINESTATE*, LONG*);

VOID far vStripStyledHorizontal(STRIP*, LINESTATE*, LONG*);
VOID far vStripStyledVertical(STRIP*, LINESTATE*, LONG*);

VOID far vStripMaskedHorizontal(STRIP*, LINESTATE*, LONG*);
VOID far vStripMaskedVertical(STRIP*, LINESTATE*, LONG*);

BOOL far bLines(DEVSURF*, TWPOINTFIXI*, TWPOINTFIXI*, TWRUNI*, ULONG,
            LINESTATE*, RECTL*, PFNSTRIP*, FLONG);
#else

struct _LINESTATE2;

typedef struct _LINESTATE2 {
    STYLEPOS*       pspStart;       // Pointer to start of style array
    STYLEPOS*       pspEnd;         // Pointer to end of style array
    STYLEPOS*       psp;            // Pointer to current style entry

    STYLEPOS        spRemaining;    // To go in current style
    STYLEPOS        spTotal;        // Sum of style array
    STYLEPOS        spTotal2;       // Twice sum of style array
    STYLEPOS        spNext;         // Style state at start of next line
    STYLEPOS        spComplex;      // Style state at start of complex clip line

    STYLEPOS*       aspRtoL;        // Style array in right-to-left order
    STYLEPOS*       aspLtoR;        // Style array in left-to-right order

    ULONG           ulStyleMask;    // Are we working on a gap in the style?
                                    // 0xff if yes, 0x0 if not
    ULONG           xyDensity;      // Density of style
    ULONG           cStyle;         // Size of style array

    ULONG           ulStyleMaskLtoR;// Original style mask, left-to-right order
    ULONG           ulStyleMaskRtoL;// Original style mask, right-to-left order

    BOOL            ulStartMask;    // Determines if first element in style
                                    // array is for a gap or a dash

} LINESTATE2;	                /* ls */

typedef VOID (far *PFNSTRIP2)(HDC, struct _STRIP*, struct _LINESTATE2*, LONG*);

VOID vssSolidHorizontal( HDC, STRIP*, LINESTATE2*, LONG* );
VOID vrlSolidHorizontal( HDC, STRIP*, LINESTATE2*, LONG * );
VOID vssSolidVertical(   HDC, STRIP*, LINESTATE2*, LONG * );
VOID vrlSolidVertical(   HDC, STRIP*, LINESTATE2*, LONG * );
VOID vssSolidDiagonalHorizontal( HDC, STRIP*, LINESTATE2*, LONG * );
VOID vrlSolidDiagonalHorizontal( HDC, STRIP*, LINESTATE2*, LONG * );
VOID vssSolidDiagonalVertical(   HDC, STRIP*, LINESTATE2*, LONG * );
VOID vrlSolidDiagonalVertical(   HDC, STRIP*, LINESTATE2*, LONG * );
VOID vStripStyledHorizontal(     HDC, STRIP*, LINESTATE2*, LONG * );
VOID vStripStyledVertical(       HDC, STRIP*, LINESTATE2*, LONG * );

BOOL far bLines(DEVSURF*, HDC, TWPOINTFIXI*, TWPOINTFIXI*, TWRUNI*, ULONG,
            LINESTATE2*, RECTL*, PFNSTRIP2*, FLONG);
#endif

// Decompress pt1Rcl & pt2Rcl
#define GETSTROKEPTDATA( pt1Rcl, rcl, pBuf )      \
{                                           \
USHORT dx, dy;                              \
switch ( pt1Rcl.fType ) {                   \
   case TW_PT1RCL_3X5Y:                     \
      dx = (USHORT)((PTWPT3X5Y)pBuf)->dx;   \
      dy = (USHORT)((PTWPT3X5Y)pBuf)->dy;   \
      break;                                \
   case TW_PT1RCL_4X4Y:                     \
      dx = (USHORT)((PTWPT4X4Y)pBuf)->dx;   \
      dy = (USHORT)((PTWPT4X4Y)pBuf)->dy;   \
      break;                                \
   case TW_PT1RCL_5X3Y:                     \
      dx = (USHORT)((PTWPT5X3Y)pBuf)->dx;   \
      dy = (USHORT)((PTWPT5X3Y)pBuf)->dy;   \
      break;                                \
   case TW_PT1RCL_6X10Y:                    \
      dx = (USHORT)((PTWPT6X10Y)pBuf)->dx;  \
      dy = (USHORT)((PTWPT6X10Y)pBuf)->dy;  \
      break;                                \
   case TW_PT1RCL_8X8Y:                     \
      dx = (USHORT)((PTWPT8X8Y)pBuf)->dx;   \
      dy = (USHORT)((PTWPT8X8Y)pBuf)->dy;   \
      break;                                \
   case TW_PT1RCL_10X6Y:                    \
      dx = (USHORT)((PTWPT10X6Y)pBuf)->dx;  \
      dy = (USHORT)((PTWPT10X6Y)pBuf)->dy;  \
      break;                                \
   case TW_PT1RCL_11X5Y:                    \
      dx = (USHORT)((PTWPT11X5Y)pBuf)->dx;  \
      dy = (USHORT)((PTWPT11X5Y)pBuf)->dy;  \
      break;                                \
   case TW_PT1RCL_11X10Y:                   \
      dx = (USHORT)((PTWPT11X10Y)pBuf)->dx; \
      dy = (USHORT)((PTWPT11X10Y)pBuf)->dy; \
      break;                                \
}                                           \
rcl.left   = (USHORT)pt1Rcl.xLeft;          \
rcl.top    = (USHORT)pt1Rcl.yTop;           \
rcl.right  = rcl.left + dx;                 \
rcl.bottom = rcl.top  + dy;                 \
}

/*
 *  Stuff from lines.h
 */

// Miscellaneous DDA defines:

#define LROUND(x, flRoundDown) (((x) + F/2 - ((flRoundDown) > 0)) >> 4)
#define F                     16
#define FLOG2                 4
#define LFLOOR(x)             ((x) >> 4)
#define FXFRAC(x)             ((x) & (F - 1))

#endif //__TWSTROKE_H__
