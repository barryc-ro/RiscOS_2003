
/*******************************************************************************
*
*   TWSTROKE.C
*
*   Thin Wire Windows - Stoke (Line) Code
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*  
*     Rev 1.11   15 Apr 1997 18:16:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.11   21 Mar 1997 16:09:32   bradp
*  update
*  
*     Rev 1.10   08 May 1996 14:51:04   jeffm
*  update
*  
*     Rev 1.9   03 Jan 1996 13:33:20   kurtp
*  update
*  
*******************************************************************************/

#ifdef DOS

#include "windows.h"

#include <stdio.h>
//#include <setjmp.h>
//#include "vmsetjmp.h"

#include "../../../inc/client.h"
#include "../../../inc/logapi.h"
#include "twtype.h"
#include "citrix/ica.h2"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"
#include "twstroke.h"
#include "twwin.h"
#include "twdata.h"
#include "twbez.h"

#else

#include <stdlib.h>

#include "wfglobal.h"
#include "twtype.h"
#include "twstroke.h"
#include "twbez.h"

#endif


//===========================================================================
//
// NOTE: Changes made to this module may also need to be changed in
//       stroke.c in the CITRIX host (thin00.dll).
//===========================================================================


/*=============================================================================
==   External Functions and Data
=============================================================================*/

extern int cbTWPackLen;
extern LPBYTE pTWPackBuf;


// VGA ulVgaMode constants:

#define DR_SET   0x00
#define DR_AND   0x08
#define DR_OR    0x10
#define DR_XOR   0x18

// Bit flag set if two passes needed:

#define DR_2PASS 0x80

// Table to convert ROP to usable information:

static struct {
    ULONG ulColorAnd;
    ULONG ulColorXor;
    ULONG ulVgaMode;
} arop[] = {
    {0x00, 0xff, DR_SET},          //  1   R2_WHITE
    {0x00, 0x00, DR_SET},          //  0   R2_BLACK
    {0xff, 0xff, DR_AND|DR_2PASS}, // DPon R2_NOTMERGEPEN   Dest invert + DPna
    {0xff, 0xff, DR_AND},          // DPna R2_MASKNOTPEN
    {0xff, 0xff, DR_SET},          // PN   R2_NOTCOPYPEN
    {0xff, 0x00, DR_AND|DR_2PASS}, // PDna R2_MASKPENNOT    Dest invert + DPa
    {0x00, 0xff, DR_XOR},          // Dn   R2_NOT           Invert dest without pen
    {0xff, 0x00, DR_XOR},          // DPx  R2_XORPEN
    {0xff, 0xff, DR_OR|DR_2PASS},  // DPan R2_NOTMASKPEN    Dest invert + DPno
    {0xff, 0x00, DR_AND},          // DPa  R2_MASKPEN
    {0xff, 0xff, DR_XOR},          // DPxn R2_NOTXORPEN     DPxn == DPnx
    {0x00, 0x00, DR_OR},           // D    R2_NOP           Silliness!
    {0xff, 0xff, DR_OR},           // DPno R2_MERGENOTPEN
    {0xff, 0x00, DR_SET},          // P    R2_COPYPEN
    {0xff, 0x00, DR_OR|DR_2PASS},  // PDno R2_MERGEPENNOT   Dest invert + DPo
    {0xff, 0x00, DR_OR},           // DPo  R2_MERGEPEN
};

#ifndef DOS

INT arop2[] = {
    R2_WHITE,
    R2_BLACK,
    R2_NOTMERGEPEN,
    R2_MASKNOTPEN,
    R2_NOTCOPYPEN,
    R2_MASKPENNOT, 
    R2_NOT,        
    R2_XORPEN,
    R2_NOTMASKPEN, 
    R2_MASKPEN,
    R2_NOTXORPEN,  
    R2_NOP,        
    R2_MERGENOTPEN,
    R2_COPYPEN,
    R2_MERGEPENNOT,
    R2_MERGEPEN,
};

#endif

#ifdef DOS

PFNSTRIP gapfnStripSolidSet[] = {

// Special strip drawers for solid lines with SET style ROPs:
    vStripSolidHorizontalSet,
    vStripSolidVertical,
    vStripSolidDiagonalHorizontal,
    vStripSolidDiagonalVertical

};

PFNSTRIP gapfnCatchTwoPass[] = {
    vCatchTwoPass,
    vCatchTwoPass,
    vCatchTwoPass,
    vCatchTwoPass
};

PFNSTRIP gapfnStrip[] = {

// The order of these first 3 sets of strip drawers is determined by
// the FL_STYLE_MASK bits of the line flags:


    vStripSolidHorizontal,
    vStripSolidVertical,
    vStripSolidDiagonalHorizontal,
    vStripSolidDiagonalVertical,

    vStripStyledHorizontal,
    vStripStyledVertical,
    NULL,    // Diagonal goes here
    NULL,    // Diagonal goes here

    vStripMaskedHorizontal,
    vStripMaskedVertical,
    NULL,    // Diagonal goes here
    NULL,    // Diagonal goes here
};

#else

PFNSTRIP2 gapfnStrip[] = {

// The order of these first 3 sets of strip drawers is determined by
// the FL_STYLE_MASK bits of the line flags:


    vrlSolidHorizontal,
    vrlSolidVertical,
    vrlSolidDiagonalHorizontal,
    vrlSolidDiagonalVertical,

    vStripStyledHorizontal,
    vStripStyledVertical,
    vStripStyledVertical,       // Diagonal goes here
    vStripStyledVertical,       // Diagonal goes here

    vStripStyledHorizontal,
    vStripStyledVertical,
    vStripStyledVertical,       // Diagonal goes here
    vStripStyledVertical,       // Diagonal goes here

    vStripStyledHorizontal,
    vStripStyledVertical,
    vStripStyledVertical,       // Diagonal goes here
    vStripStyledVertical,       // Diagonal goes here

};

#endif


/*
 *  DOS vs. WINxx
 */
#ifdef DOS

BOOL FindBoundary( TWPOINTFIXI ptfxFirst, TWPOINTFIXI *pptfxBuf, TWRUNI *pRUN,
                   ULONG cptfx, RECTI *prclBoundary );

extern int vcbScan;

#define MAX_POINTS (LARGE_CACHE_CHUNK_SIZE / 4) // size of static_buffer_1
#define vtwptfx static_buffer_1
extern TWPOINTFIXI vtwptfx[MAX_POINTS];
#define vpStyleData ((USHORT *)(&vtwptfx[MAX_POINTS]))

extern void far ExcludeCursor( RECTI *prclBoundary );
extern void far UnexcludeCursor( void );

#else

#define MAX_POINTS  (LARGE_CACHE_CHUNK_SIZE / 4) // size of static_buffer_1
TWPOINTFIXI * vtwptfx;
USHORT      * vpStyleData;

// Style array for alternate style (alternates one pixel on, one pixel off):

STYLEPOS gaspAlternateStyle[] = { 1 };

void EllipseToBeziers( PELLIPSEDATA pptfx, TWPOINTFIXI *pptfxB );

BOOL fIsDriver32Bit = FALSE;
BOOL fVersionCheck  = FALSE;

#endif


#define FREE( p ) { if ( p ) free( p ); }

#define GETNEXTRUN( p ) \
   GetNextTWCmdBytes( &runTemp, sizeof(runTemp) ); \
   (p)->x = (SHORT)runTemp.iStart & (SHORT)0x7fff;                \
   (p)->y = (SHORT)runTemp.iStop  & (SHORT)0x7fff;

#define GETNEXTPOINT( p )                                               \
   GetNextTWCmdBytes( &ptfxTemp, SIZE_OF_TWPOINT );                     \
   switch ( *((UCHAR *)&ptfxTemp) & 0x3 ) {                             \
      case TW_PT_UNSIGN:                                                \
         (p)->x = (SHORT)((TWPOINT *)&ptfxTemp)->x << 4;                \
         (p)->y = (SHORT)((TWPOINT *)&ptfxTemp)->y << 4;                \
         break;                                                         \
      case TW_PT_FIX:                                                   \
         GetNextTWCmdBytes( ((UCHAR *)&ptfxTemp) + SIZE_OF_TWPOINT,     \
                            SIZE_OF_TWPOINTFIX - SIZE_OF_TWPOINT );     \
         (p)->x = (SHORT)((TWPOINTFIX *)&ptfxTemp)->x & (SHORT)0x7fff;  \
         (p)->y = (SHORT)((TWPOINTFIX *)&ptfxTemp)->y & (SHORT)0x3fff;  \
         break;                                                         \
      case TW_PT_SIGN:                                                  \
         GetNextTWCmdBytes( ((UCHAR *)&ptfxTemp) + SIZE_OF_TWPOINT,     \
                            SIZE_OF_TWPOINTSIGN - SIZE_OF_TWPOINT );    \
         (p)->x = (SHORT)((TWPOINTSIGN *)&ptfxTemp)->x << 4;            \
         (p)->y = (SHORT)((TWPOINTSIGN *)&ptfxTemp)->y << 4;            \
         break;                                                         \
      case TW_PT_FIXSIGN:                                               \
         GetNextTWCmdBytes( ((UCHAR *)&ptfxTemp) + SIZE_OF_TWPOINT,     \
                            SIZE_OF_TWPOINTFIXSIGN - SIZE_OF_TWPOINT ); \
         (p)->x = (SHORT)*(PLONG)&((TWPOINTFIXSIGN *)&ptfxTemp)[1];     \ // SJM: this may just work!!!
         (p)->y = (SHORT)*(PLONG)&((TWPOINTFIXSIGN *)&ptfxTemp)[5];     \
         break;                                                         \
   }


#define GOTLASTPOINT() ptfxTemp.fLast
#define GOTLASTRUN() runTemp.fLast

void w_TWCmdStrokePath( HWND hWnd, HDC hdc, BOOL fStraight );


/****************************************************************************\
 *
 *  TWCmdStrokePath (TWCMD_STROKEPATH service routine)
 *
 *  This routine is called to service the DrvStrokePath thinwire display
 *  driver function.
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *     hdc (input)
 *        device context
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
TWCmdStrokePath( HWND hWnd, HDC hdc )
{

    TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: normal" ));

    w_TWCmdStrokePath( hWnd, hdc, FALSE );
}


/****************************************************************************\
 *
 *  TWCmdStrokePathStraight (TWCMD_STROKEPATHSTRAIGHT service routine)
 *
 *  This routine is called to service the DrvStrokePath thinwire display
 *  driver function for straight lines.
 *
 *  PARAMETERS:
 *     hWnd (input)
 *        window handle
 *     hdc (input)
 *        device context
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
TWCmdStrokePathStraight(  HWND hWnd, HDC hdc  )
{

   TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: straight lines" ));

   w_TWCmdStrokePath( hWnd, hdc, TRUE );
}


/****************************************************************************\
 *
 *  w_TWCmdStrokePath
 *
 *  This is the worker routine for all variations of the DrvStrokePath
 *  services.
 *
 *  PARAMETERS:
 *     BOOL fStraight - TRUE if straight line consisting of
 *                      - same Pen as previous call
 *                      - 2 points, all integer values
 *                      - No clipping, simple or complex
 *                      - solid line
 *
 *  RETURN: ( via TWCmdReturn() - a longjmp in disguise )
 *     TRUE  - success
 *     FALSE - error occurred
 *
\****************************************************************************/

void
w_TWCmdStrokePath( HWND hWnd, HDC hDC, BOOL fStraight )
{
    USHORT         rc = 0;
    TWSPHEADER     Flags;
static TWSPPEN     Pen16;
static TWPOINTFIXI vLastPoint = { 0, 0 };
    TWSPSTYLEMASK  StyleMask = 0;
    TWSPSTYLESTATE StyleState;
    TWSPSTYLE      Style;
    TWSPBOUNDS     Bounds;
    TWSPSUBPATH    SubPath;
    FLONG          fl        = FL_PHYSICAL_DEVICE;
    RECTL*         prclClip  = (RECTL*) NULL;
    TWRUNI*        prun      = (TWRUNI*) NULL;
    DEVSURF        dsurf;
    STYLEPOS       aspLtoR[STYLE_MAX_COUNT];
    STYLEPOS       aspRtoL[STYLE_MAX_COUNT];
    ULONG          iStripIndex;
    ULONG          cptfx;
    TWPOINTFIXI    ptfxFirst;
    TWPOINTFIXI *  pptfxSecond;
    TWPOINTFIXI    ptfxB;
    TWPOINTFIXI *  pptfxBuf;
    TWRUNI *       pRUN;
    TWPOINTFIXI    ptfxStartFigure;
    TWPOINTFIXI    ptfxEndFigure;
    BOOL           fAbort = FALSE;
    TWPOINTFIXSIGN ptfxTemp;
    TWRUN          runTemp;
    TWPOINT  *     pptTemp;
    TWTOPT1RCL     pt1Rcl;
    ULONG          pt2Rcl;
    ULONG *        ppt2Rcl = &pt2Rcl;
    ULONG          ulcbData;
    BOOL           fTooManyPoints;
#ifdef DOS
    ULONG          ulVgaMode;
    ULONG          iColor;
    PFNSTRIP*      apfn;
    RECTL          arclClip[4]; // For rectangular clipping
    LINESTATE      ls;
    RECTI          rclBoundary;
#else
static TWSPPEN256  Pen256;
       PFNSTRIP2*  apfn;
       LINESTATE2  ls;
       HRGN        hrgnDest = NULL;
       LPBYTE      p = (LPBYTE) lpstatic_buffer;

    /*
     *  Setup receive buffer
     */
    vtwptfx     = (TWPOINTFIXI *) p;
    vpStyleData = (USHORT *) (p + MAX_POINTS * sizeof(TWPOINTFIXI));
#endif


    TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Entered" ));

    if ( fStraight ) {
       Flags.clipping  = TW_CLIP_TRIVIAL;
       Flags.style     = TW_LINE_SOLID;
       Flags.fStartGap = FALSE;
       Flags.fSamePen  = TRUE;
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Straight - Flags(%02X)",
                                      *((UCHAR *)&Flags) ));
    } else {
       // Get Flags
       GetNextTWCmdBytes( &Flags, sizeof_TWSPHEADER );
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - Flags(%02X)",
                                      *((UCHAR *)&Flags) ));
    }


    // If Pen changed, overwrite our static copy, otherwise use existing Pen
    if ( !Flags.fSamePen ) {
       // Get Pen
#ifndef DOS
       if ( vColor == Color_Cap_256 ) 
       {
          GetNextTWCmdBytes( &Pen256, sizeof_TWSPPEN256 );
          TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - Pen256.color %u, Pen256.rop2 %u",
                  Pen256.color, Pen256.rop2));
       }
       else 
#endif
       {
          GetNextTWCmdBytes( &Pen16, sizeof_TWSPPEN );
          TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - Pen16.color %u, Pen16.rop2 %u",
                  Pen16.color, Pen16.rop2));
       }
    }
#ifdef DEBUG
    else {
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Using previous Pen" ));
    }
#endif

    if ( Flags.style == TW_LINE_MASK ) {
       // Get StyleMask
       GetNextTWCmdBytes( &StyleMask, sizeof_TWSPSTYLEMASK );
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - StyleMask(%02X)",
                                      *((UCHAR *)&StyleMask) ));
    }

    if ( (Flags.style != TW_LINE_SOLID) && (Flags.clipping != TW_CLIP_COMPLEX) ) {
       // Get StyleState
       GetNextTWCmdBytes( &StyleState, sizeof_TWSPSTYLESTATE );
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - StyleState(%02X)",
                                      (USHORT)StyleState ));
    }

    if ( Flags.style == TW_LINE_OTHER ) {
       // Get Style
       GetNextTWCmdBytes( &Style.cData, sizeof(Style.cData) );
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - Style cData(%d)",
                                      (USHORT)Style.cData ));
       Style.pData = vpStyleData;

       GetNextTWCmdBytes( Style.pData, Style.cData*sizeof(USHORT) );
       TRACEBUF(( TC_TW, TT_TW_STROKE,
             (char far *)Style.pData, (ULONG)(Style.cData*sizeof(USHORT)) ));

    }

    if ( Flags.clipping == TW_CLIP_RECT ) {
       // Get Bounds
       // Get pt1Rcl
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Receiving compressed Bounds:" ));
       GetNextTWCmdBytes( &pt1Rcl, 3 );
       TRACEBUF(( TC_TW, TT_TW_STROKE, (char far *)&pt1Rcl, 3UL ));
      
       // Get size of pt2Rcl
       GETNEXTPTSIZE2( pt1Rcl, &ulcbData );
       GetNextTWCmdBytes( ppt2Rcl, (int)ulcbData );
       TRACEBUF(( TC_TW, TT_TW_STROKE, (char far *)ppt2Rcl, ulcbData ));
      
       // Put it all together
       GETSTROKEPTDATA( pt1Rcl, Bounds, ppt2Rcl );

       Bounds.right++;                   // Change LRH from 0-based 1
       Bounds.bottom++;                   
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - Bounds left(%d) right(%d) top(%d) bottom(%d)",
          Bounds.left, Bounds.right, Bounds.top, Bounds.bottom ));
    }


    //
    // Crack the protocol
    //
    switch ( Flags.style ) {
  
        case TW_LINE_ALTERNATE:
            fl |= FL_ALTERNATESTYLED | FL_MASKSTYLED;
            break;

        case TW_LINE_DOT:
   	    StyleMask = 0xAA;        // 10101010 DOT			
            break;
   
        case TW_LINE_DASH:
            StyleMask = 0xCC;        // 11001100 DASH	
            break;
   
        case TW_LINE_DASHDOT:
            StyleMask = 0xE4;        // 11100100 DASHDOT			
            break;
   
        case TW_LINE_DASHDOTDOT:
            StyleMask = 0xEA;        // 11101010 DASHDOTDOT
            break;
   
        case TW_LINE_MASK:
            break;                   // other 8-bit mask
   
        case TW_LINE_OTHER:

            // Handle Arbitrary Styles
            // -----------------------
            //
            // Because arbitrary styles are new to Win32, many apps won't
            // know about them and will use the default styles (which will
            // be handled by the "Masked" optimization case above), and so
            // this code path won't get exercised too often.  (See GDI's
            // ExtCreatePen API.)
            //
            // But you still have to handle them, and do them right!

            {
                USHORT    * pstyle;
                STYLEPOS*   pspDown;
                STYLEPOS*   pspUp;
     
     
                if ( !fAbort ) {
                    pstyle = &Style.pData[Style.cData];
      
                    ls.spTotal   = 0;
                    while (pstyle-- > Style.pData)
                    {
                        ls.spTotal += (LONG)*pstyle;
                    }
      
                    fl        |= FL_ARBITRARYSTYLED;
                    ls.cStyle  = (ULONG)Style.cData;
                    ls.aspRtoL = aspRtoL;
                    ls.aspLtoR = aspLtoR;
      
                    if ( Flags.fStartGap ) {
#ifdef DOS
                        ls.ulStartMask = (ULONG) ~0L;
#else
                        ls.ulStartMask = ~0;
#endif
                    } else {
                        ls.ulStartMask = 0;
                    }
      
                    pstyle  = Style.pData;
                    pspDown = &ls.aspRtoL[ls.cStyle - 1];
                    pspUp   = &ls.aspLtoR[0];
      
                    // We always draw strips left-to-right, but styles have to be laid
                    // down in the direction of the original line.  This means that in
                    // the strip code we have to traverse the style array in the
                    // opposite direction
      
                    while (pspDown >= &ls.aspRtoL[0])
                    {
      
                        *pspDown = (LONG)*pstyle * STYLE_DENSITY;
                        *pspUp   = *pspDown;
      
                        pspUp++;
                        pspDown--;
                        pstyle++;
                    }
                }
            }
            break;
    }

#ifdef DOS

   if ( StyleMask )
   {
      int n;

      fl |= FL_MASKSTYLED;

      ls.spTotal  = 8;

      ls.ulStyleMaskRtoL = StyleMask;

      // Construct the left to right mask
      for ( ls.ulStyleMaskLtoR = 0, n = 0; n < 8; n++ ) {
         if ( StyleMask & (1 << n) ) {
            ls.ulStyleMaskLtoR |= 1 << (7-n);
         }
      }

      // Replicate byte and initialize to style position zero:

          ls.ulStyleMaskLtoR |= (ls.ulStyleMaskLtoR << 8);
          ls.ulStyleMaskLtoR |= (ls.ulStyleMaskLtoR << 16);

          ls.ulStyleMaskRtoL |= (ls.ulStyleMaskRtoL << 8);
          ls.ulStyleMaskRtoL |= (ls.ulStyleMaskRtoL << 16);

      // Check if we should start with a gap or start with a dash:
      if ( Flags.fStartGap ) {
         ls.ulStyleMaskLtoR = ~ls.ulStyleMaskLtoR;
         ls.ulStyleMaskRtoL = ~ls.ulStyleMaskRtoL;
      }
   }

   if ( fl & FL_STYLED ) {
      if ( fl & FL_ALTERNATESTYLED ) {
         ls.spTotal   = 1;
         ls.spTotal2  = 2;
         ls.xyDensity = 1;
         ls.spNext = (LONG)StyleState & 1;
      } else {
         ls.spTotal  *= STYLE_DENSITY;
         ls.spTotal2  = 2 * ls.spTotal;
         ls.xyDensity = STYLE_DENSITY;

     // Compute starting style position (this is guaranteed not to overflow):
#ifdef OLD
         ls.spNext = HIWORD(StyleState.l) * STYLE_DENSITY +
                     LOWORD(StyleState.l);
#else
         ls.spNext = (LONG)StyleState;
#endif
      }
   }

   if ( Flags.clipping == TW_CLIP_RECT) {
       fl |= FL_SIMPLE_CLIP;

       arclClip[0].top    =  (LONG)Bounds.top;
       arclClip[0].left   =  (LONG)Bounds.left;
       arclClip[0].bottom =  (LONG)Bounds.bottom;
       arclClip[0].right  =  (LONG)Bounds.right;

   // FL_FLIP_D:

       arclClip[1].top    =  (LONG)Bounds.left;
       arclClip[1].left   =  (LONG)Bounds.top;
       arclClip[1].bottom =  (LONG)Bounds.right;
       arclClip[1].right  =  (LONG)Bounds.bottom;

   // FL_FLIP_V:

       arclClip[2].top    = -(LONG)Bounds.bottom + 1;
       arclClip[2].left   =  (LONG)Bounds.left;
       arclClip[2].bottom = -(LONG)Bounds.top + 1;
       arclClip[2].right  =  (LONG)Bounds.right;

   // FL_FLIP_V | FL_FLIP_D:

       arclClip[3].top    =  (LONG)Bounds.left;
       arclClip[3].left   = -(LONG)Bounds.bottom + 1;
       arclClip[3].bottom =  (LONG)Bounds.right;
       arclClip[3].right  = -(LONG)Bounds.top + 1;

       prclClip = arclClip;
#ifdef DEBUG
       {
           int i;

           for ( i = 0; i <= 3; i++ ) {
             TRACE(( TC_TW, TT_TW_STROKE,
        "TWCmdStrokePath: arclClip[%d] left(%ld) right(%ld) top(%ld) bottom(%ld)",
             i, arclClip[i].left, arclClip[i].right,
                arclClip[i].top, arclClip[i].bottom ));
           }
       }
#endif

   } else if ( Flags.clipping == TW_CLIP_COMPLEX ) {
        fl |= FL_COMPLEX_CLIP;
   }

#else
 
    /*
     *  Generate the LS for bLines
     */
    if ( fl & FL_STYLED ) {
        if ( fl & FL_ALTERNATESTYLED ) {
            ls.cStyle      = 1;
            ls.spTotal     = 1;
            ls.spTotal2    = 2;
            ls.spRemaining = 1;
            ls.aspRtoL     = &gaspAlternateStyle[0];
            ls.aspLtoR     = &gaspAlternateStyle[0];
            ls.spNext      = (ULONG) StyleState & 1;
            ls.xyDensity   = 1;
            fl            |= FL_ARBITRARYSTYLED;
            ls.ulStartMask = 0L;
        }
        else {
            ls.spTotal  *= STYLE_DENSITY;
            ls.spTotal2  = 2 * ls.spTotal;
            ls.xyDensity = STYLE_DENSITY;
            ls.spNext = (LONG)StyleState;
        }
    }
    else if ( StyleMask ) {
 
        USHORT         i;
        TWSPSTYLEMASK  hBit;
        TWSPSTYLEMASK  lBit;
        TWSPSTYLEMASK  TempMask;

        /*
         *  Styled line
         */
        fl |= FL_STYLED;

        /*
         *  Setup 
         */
        ls.spTotal     = 8 * STYLE_DENSITY;
        ls.spTotal2    = 8 * 2 * STYLE_DENSITY;
        ls.xyDensity   = STYLE_DENSITY;
        ls.spNext      = StyleState;
        ls.aspRtoL     = aspRtoL;
        ls.aspLtoR     = aspLtoR;

        /*
         *  Construct the left to right mask
         */
        for ( TempMask = 0, i = 0; i < 8; i++ ) {
           if ( StyleMask & (1 << i) ) {
              TempMask |= 1 << (7-i);
           }
        }
  
        /*
         *  Start on a gap?
         */
        if ( Flags.fStartGap ) {
            ls.ulStartMask = ~0;
        } else {
            ls.ulStartMask = 0;
        }

        /*
         *  Initialize style array stuff
         */
        ls.cStyle          = 0;
        aspLtoR[ls.cStyle] = STYLE_DENSITY;

        /*
         *  Start with high bit
         */
        hBit = (TempMask & 0x80);

        /*
         *  Count bits in each dash or gap
         */
        for ( i=1; i<8; i++ ) {

            lBit = (TempMask & 0x40) << 1;

            if ( hBit == lBit ) 
                aspLtoR[ls.cStyle]  += STYLE_DENSITY;
            else 
                aspLtoR[++ls.cStyle] = STYLE_DENSITY;

            hBit = lBit;
            TempMask <<= 1;
        }

        /*
         *  Make equal to count not just index
         */
        ++ls.cStyle;

        /*
         *  Generate Right to Left array
         */
        for ( i=0; i<ls.cStyle; i++ ) {
            aspRtoL[i] = aspLtoR[ls.cStyle - i - 1];
        }

#ifdef DEBUG
        if ( vColor == Color_Cap_256 ) {
            TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Styled Pen256, color %u, rop2 %u, Count %u, Style %04X, fStartGap %u", Pen256.color, Pen256.rop2, ls.cStyle, StyleMask, Flags.fStartGap ));
        }
        else {
            TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Styled Pen16, color %u, rop2 %u, Count %u, Style %04X, fStartGap %u", Pen16.color, Pen16.rop2, ls.cStyle, StyleMask, Flags.fStartGap ));
        }
#endif
    }

    /*
     *  Color 16 or 256?
     */
    if ( vColor == Color_Cap_256 ) {

        /*
         *  Select solid pen if not same as currently selected into DC
         */
        if ( dcstate.pencolor != Pen256.color ) {

            int iPen = Pen256.color;

            /*
             *  Save true pen index
             */
            dcstate.pencolor = iPen;

            /*
             *  Calculate cache index
             */
            if ( iPen > 9 ) {

                if( iPen < 246 ) {

                    HPEN hPen = CreatePen( PS_SOLID, 1, PALETTEINDEX(iPen) );

                    /*
                     *  Remove old pen
                     */
                    iPen = 20;
                    if ( hpensolid256[iPen] != NULL ) {
                        SelectObject( hDC, (HPEN) GetStockObject(BLACK_PEN) );
                        DeleteObject( hpensolid256[iPen] );
                    }

                    hpensolid256[iPen] = hPen;
                    TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: CreatePen %u (index %u)", Pen256.color, iPen )); 
                }
                else {

                    iPen -= 236;
                }
            }

            TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: SelectPen %u (index %u)", Pen256.color, iPen )); 
            SelectObject( hDC, hpensolid256[iPen]  );
        }

        /*
         *  Set rop2 if not same as currently selected into DC
         */
        if ( dcstate.rop2 != Pen256.rop2 ) {
            dcstate.rop2 = Pen256.rop2;
            SetROP2( hDC, arop2[ Pen256.rop2 ]  );
        }

        TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Pen256.color %u, Pen256.rop2 %u", Pen256.color, Pen256.rop2));
    }
    else {

        /*
         *  Select solid pen if not same as currently selected into DC
         */
        if ( dcstate.pencolor != Pen16.color ) {
            SelectObject( hDC, hpensolid[ dcstate.pencolor = Pen16.color]  );
        }
    
        /*
         *  Set rop2 if not same as currently selected into DC
         */
        if ( dcstate.rop2 != Pen16.rop2 ) {
            dcstate.rop2 = Pen16.rop2;
            SetROP2( hDC, arop2[ Pen16.rop2 ]  );
        }

        TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Pen16.color %u, Pen16.rop2 %u", Pen16.color, Pen16.rop2));
    }

    /*
     *  Create clip region for simple clipping
     */
    if ( Flags.clipping == TW_CLIP_RECT) {

        fl |= FL_SIMPLE_CLIP;

        /*
         *  Create the simple region
         */
        if ( (hrgnDest = CreateRectRgn( Bounds.left, 
                                        Bounds.top, 
                                        Bounds.right, 
                                        Bounds.bottom )) != NULL ) {

            TRACE(( TC_TW, TT_TW_STROKE,
            "TWCmdStrokePath: ClipRgn - left(%d) right(%d) top(%d) bottom(%d)",
                Bounds.left, Bounds.right, Bounds.top, Bounds.bottom ));
    
            /*
             *  Select the clipping region
             */
            SelectClipRgn( hDC, hrgnDest );
        }

    } 
    else if ( Flags.clipping == TW_CLIP_COMPLEX ) {
        fl |= FL_COMPLEX_CLIP;
    }

#endif

   TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: fl(%08lX)", fl ));

   // Compute the pointer to the correct strip drawing table.  We
   // have a special table for solid lines done with VGA mode == SET:

   iStripIndex = 4 * ((fl & FL_STYLE_MASK) >> FL_STYLE_SHIFT);
   apfn = &gapfnStrip[iStripIndex];

#ifdef DOS

   ulVgaMode = arop[Pen16.rop2].ulVgaMode;

   if (ulVgaMode == DR_SET && iStripIndex == 0)
       apfn = &gapfnStripSolidSet[0];

   // Compute the correct color, based on the ROP we're doing:

   iColor = (Pen16.color & arop[Pen16.rop2].ulColorAnd)
          ^ (arop[Pen16.rop2].ulColorXor);

   if (!(ulVgaMode & DR_2PASS)) {
       TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Calling vSetStrips iColor(%08lX) ulVgaMode(%08lx)",
                 iColor, ulVgaMode ));
       vSetStrips(iColor, ulVgaMode);
   } else
   {
   // If the ROP requires 2 passes, we sneakily change our strip
   // table pointer to point to only our own routine, and it
   // handles calling the appropriate strip routines twice:

       ls.ulVgaMode = ulVgaMode ^ DR_2PASS;
       ls.iColor    = iColor;
       ls.apfnStrip = apfn;
       apfn         = gapfnCatchTwoPass;
   }

   TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: apfn(%08lX)", apfn ));

   dsurf.lNextScan = vcbScan;
// dsurf.lNextScan              = 80; // 640pels/row / 8bits/byte * 1bit/pel = 80 bytes/row
// dsurf.lNextScan              =100; // 800pels/row / 8bits/byte * 1bit/pel =100 bytes/row
// dsurf.pvBitmapStart          = (char *)0xA0000000; // PVB Start

#endif

    do {
       if ( fStraight ) {
          SubPath.fLast   = TRUE;
          SubPath.fBezier = FALSE;
          SubPath.fClosed = FALSE;
          SubPath.fType   = TW_SP_NORMAL; // Assume normal for now
          TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Straight - SubPath fBez(%d) fClose(%d)",
                     SubPath.fBezier, SubPath.fClosed ));
       } else {
          // Get sub path header
          GetNextTWCmdBytes( &SubPath, sizeof_TWSPSUBPATH );
          TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - SubPath(%02X) fLast(%d) fSame(%d) fBez(%d) fClose(%d) fType(%1X)",
                     *((USHORT *)(&SubPath)) &0xFF, SubPath.fLast, SubPath.fSamePoint,
                     SubPath.fBezier, SubPath.fClosed, SubPath.fType ));
       }

       if ( SubPath.fType == TW_SP_ENDDATA ) {
           TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Empty subpath - finished" ));
           break;
       }

       if ( (fl & FL_STYLED) && (fl & FL_COMPLEX_CLIP) ) {
          // Get complex StyleState
          GetNextTWCmdBytes( &StyleState, sizeof(StyleState) );
          TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - Complex StyleState(%02X)",
                                         (USHORT)StyleState ));
#ifdef OLD
          ls.spComplex = HIWORD(StyleState.l) * ls.xyDensity
                       + LOWORD(StyleState.l);
#else
          ls.spComplex =  (LONG)StyleState;
#endif
       }

       // The second point will either be the last of a complex clip sub path
       // or will be the first entry in our point buffer
       if ( Flags.clipping == TW_CLIP_COMPLEX ) {
           pptfxSecond = &ptfxB;

       } else if ( SubPath.fBezier ) {
           // For beziers, the bFlatten routine wants all the points in
           // one contiguous buffer
           pptfxSecond = &vtwptfx[1];

       } else {
           pptfxSecond = vtwptfx;
       }

       if ( fStraight ) {
       // Special case horizontal/vertical lines
          {
             TWPOINTSPECIAL * pptSpecial = (TWPOINTSPECIAL *)&ptfxTemp;

             pptTemp = (TWPOINT *)&ptfxTemp;

             GetNextTWCmdBytes( pptSpecial, 2 ); // We know it's at least 2 bytes

             SubPath.fSamePoint = (UCHAR)pptSpecial->fSamePoint;
             SubPath.fType      = (UCHAR)pptSpecial->fType & (UCHAR)0x03;
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Straight - fType(%1X) special(%2X)",
                                     SubPath.fType, *((USHORT *)pptSpecial) ));

             if ( SubPath.fSamePoint ) {
                ptfxFirst = vLastPoint;
                TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Use last point as first x(%04X) y(%04X)",
                                              ptfxFirst.x, ptfxFirst.y ));
             } else {
                GetNextTWCmdBytes( ((UCHAR *)&ptfxTemp)+2, 1 );  // Get 3rd byte

                TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: ppt(%06lX) ppt (%02X,%02X)",
                          *((ULONG *)pptTemp) & 0xFFFFFF, pptTemp->x, pptTemp->y ));
                ptfxFirst.x = ((SHORT)pptTemp->x & (SHORT)0x7ff) << 4;
                ptfxFirst.y = ((SHORT)pptTemp->y & (SHORT)0x3ff) << 4;
                TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - pointfixFirst x(%04X) y(%04X)",
                                              ptfxFirst.x, ptfxFirst.y ));

                GetNextTWCmdBytes( pptSpecial, 2 ); // Get 2nd point
             }

             if ( SubPath.fType == TW_SP_HORZ ) {
                 TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Straight - horizontal (%04X)",
                                         (SHORT)pptSpecial->value << 4 ));
                 pptfxSecond->x = (SHORT)pptSpecial->value << 4;
                 pptfxSecond->y = ptfxFirst.y;
             } else if ( SubPath.fType == TW_SP_VERT ) {
                 TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Straight - vertical (%04X)",
                                         (SHORT)pptSpecial->value << 4 ));
                 pptfxSecond->x = ptfxFirst.x;
                 pptfxSecond->y = (SHORT)pptSpecial->value << 4;
             } else {
                 TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Straight - diagonal" ));
                GetNextTWCmdBytes( ((UCHAR *)&ptfxTemp)+2, 1 );  // Get 3rd byte
                pptfxSecond->x = ((SHORT)pptTemp->x & (SHORT)0x7ff) << 4;
                pptfxSecond->y = ((SHORT)pptTemp->y & (SHORT)0x3ff) << 4;
             }
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - ptfxSecond (%04X,%04X)",
                                           pptfxSecond->x, pptfxSecond->y ));
          }
       } else {

          //
          // Get first point - may be same as previous point in which case
          //                   it will not be sent
          //
          if ( SubPath.fSamePoint ) {
             ptfxFirst = vLastPoint;
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Use last point as first x(%04X) y(%04X)",
                                           ptfxFirst.x, ptfxFirst.y ));
          } else {
             // First point was sent - it may be three or four bytes
             GETNEXTPOINT( &ptfxFirst );
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - pointfixFirst x(%04X) y(%04X)",
                                           ptfxFirst.x, ptfxFirst.y ));
          }

          //
          // Get second point - may be just an X or a Y if sub path is a
          //                    horizontal or vertical line
          //
          if ( SubPath.fType == TW_SP_HORZ ) {
             GetNextTWCmdBytes( &(pptfxSecond->x), sizeof(pptfxSecond->x) );
             pptfxSecond->y = ptfxFirst.y;
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Horizontal line ptfxSecond = (%04X,%04X)",
                                           pptfxSecond->x, pptfxSecond->y ));
          } else if ( SubPath.fType == TW_SP_VERT ) {
             GetNextTWCmdBytes( &(pptfxSecond->y), sizeof(pptfxSecond->y) );
             pptfxSecond->x = ptfxFirst.x;
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Vertical line ptfxSecond = (%04X,%04X)",
                                           pptfxSecond->x, pptfxSecond->y ));
          } else {
             // Second point was sent - it may be three or four bytes
             GETNEXTPOINT( pptfxSecond );
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - ptfxSecond (%04X,%04X)",
                                           pptfxSecond->x, pptfxSecond->y ));
          }
       }

GetMorePoints:

       /*
        *  Check for point overflow
        */
       fTooManyPoints = FALSE;

       // Get the rest of the points if clipping (then points are a run),
       // or if there are more than the two points we already got
       if ( Flags.clipping == TW_CLIP_COMPLEX ) {
           for ( cptfx = 0, runTemp.fLast = FALSE; !GOTLASTRUN(); cptfx++ ) {
              GETNEXTRUN( &vtwptfx[cptfx] );
              TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - Run[%ld] (%04X,%04X)",
                   cptfx, vtwptfx[cptfx].x, vtwptfx[cptfx].y ));

              /*
               *  Gnarly big'ole line
               */
              if ( (cptfx > (MAX_POINTS-2)) && !GOTLASTPOINT() ) {
                 fTooManyPoints = TRUE;
                 break;
              }
           }
       } else {
           cptfx = 1;
           if ( (SubPath.fType != TW_SP_HORZ) &&
                (SubPath.fType != TW_SP_VERT) &&
                (SubPath.fType != TW_SP_DIAG) ) {
              if ( SubPath.fBezier ) {
                 // 0 -> first point
                 // 1 -> second point
                 // 2 -> rest of points
                 cptfx = 2;
              }
              for ( ; !GOTLASTPOINT(); cptfx++ ) {
                 GETNEXTPOINT( &vtwptfx[cptfx] );
                 TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Received - pointfix[%ld] (%04X,%04X) fLast(%d)",
                      cptfx, vtwptfx[cptfx].x, vtwptfx[cptfx].y, GOTLASTPOINT() ));

                 /*
                  *  Gnarly big'ole line
                  */
                 if ( (cptfx > (MAX_POINTS-2)) && !GOTLASTPOINT() ) {
                    fTooManyPoints = TRUE;
                    break;
                 }
              }
           }
       }

       if ( !fAbort ) {
          if ( Flags.clipping == TW_CLIP_COMPLEX ) {
            pptfxBuf = &ptfxB;
            ptfxEndFigure = ptfxB;
            pRUN = (TWRUNI *)vtwptfx;
          } else {
            pptfxBuf = vtwptfx;
            pRUN = (TWRUNI *)NULL;

            if ( SubPath.fBezier ) {

                *pptfxBuf = ptfxFirst;

#ifdef WIN32
                /*
                 *  Did we do the version check call yet?
                 */
                if ( !fVersionCheck ) {

                    OSVERSIONINFO osvi;

                    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                    GetVersionEx( &osvi );

                    fIsDriver32Bit = (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) ? TRUE : FALSE;
                    fVersionCheck  = TRUE;
                }

                /* 
                 *  If this is just a solid ellipse then, hell we're
                 *  under windows let's just call the ellipst function.
                 *
                 *  Note: Only works for NT for now.
                 */
                if ( fIsDriver32Bit ) {

                    if ( (Flags.style == TW_LINE_SOLID) && (SubPath.fType == TW_SP_ELLIPSE) ) {
    
                        TWPOINTFIXI * pptfxNext;
                        INT           xLeft;
                        INT           yTop;
                        INT           xRight;
                        INT           yBottom;
    
                        /*
                         *  Generiate the Bezier points from the Ellipse points,
                         *  Point to buffer just after the 3 points
                         */
                        pptfxNext = &((pptfxBuf)[3]);
                        EllipseToBeziers( (PELLIPSEDATA)(pptfxBuf), pptfxNext );
                        
                        /*
                         *  Convert from 28.4 to 28.0
                         */
                        xLeft   = (INT) (pptfxNext[5].x >> 4);
                        yTop    = (INT) (pptfxNext[2].y >> 4);
                        xRight  = (INT) (pptfxNext[0].x >> 4) + 1;
                        yBottom = (INT) (pptfxNext[8].y >> 4) + 1;
    
                        /*
                         *  Use windows to draw the Ellipse
                         */
                        {
                            HBRUSH hBrush;
    
                            hBrush = SelectObject( hDC, GetStockObject( NULL_BRUSH ) );
                            Ellipse( hDC, xLeft, yTop, xRight, yBottom );
                            SelectObject( hDC, hBrush );
                        }
    
                        continue;
                    }
                }
#endif
                bFlatten( &cptfx, &pptfxBuf, SubPath.fType == TW_SP_ELLIPSE );
                ptfxFirst = *pptfxBuf;
                pptfxBuf  = &pptfxBuf[1];
                cptfx--;
            }
            ptfxEndFigure = pptfxBuf[cptfx-1];
          }
          vLastPoint = ptfxEndFigure;

#ifdef DOS
          FindBoundary( ptfxFirst, pptfxBuf, pRUN, cptfx, &rclBoundary );
          ExcludeCursor( &rclBoundary );
#endif

          if ( SubPath.fNewStart ) {
             ptfxStartFigure = ptfxFirst;
          }


          TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: bLines cptfx(%ld) 1(%04X,%04X) 2(%04X,%04X)",
                    cptfx, ptfxFirst.x, ptfxFirst.y,
                    pptfxBuf->x, pptfxBuf->y ));
#ifdef DEBUG

          if ( Flags.clipping == TW_CLIP_COMPLEX ) {
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: fl(%08lX) pRUN[0] (%04X,%04X)",
                       fl, pRUN->iStart, pRUN->iStop  ));
          }
#endif
          if (!bLines(&dsurf,
#ifndef DOS
                      hDC,
#endif
                      &ptfxFirst,
                      pptfxBuf,
                      pRUN,
                      cptfx,
                      &ls,
                      prclClip,
                      apfn,
                      fl)) {
             rc = FALSE;
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Error in bLines" ));
             fAbort = TRUE;
          }


          if ( SubPath.fClosed && 
               ( (ptfxEndFigure.x != ptfxStartFigure.x) ||
                 (ptfxEndFigure.y != ptfxStartFigure.y) ) ) {
             TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: bLines close 1(%04X,%04X) 2(%04X,%04X)",
                       ptfxEndFigure.x, ptfxEndFigure.y,
                       ptfxStartFigure.x, ptfxStartFigure.y ));
             if (!bLines(&dsurf,
#ifndef DOS
                         hDC,
#endif
                         &ptfxEndFigure,
                         &ptfxStartFigure,
                         (TWRUNI*)NULL,
                         1,
                         &ls,
                         prclClip,
                         apfn,
                         fl)) {
                rc = FALSE;
                TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Error in bLines" ));
                fAbort = TRUE;
             }
          }

#ifdef DOS
          UnexcludeCursor();
#endif

       }

       /*
        *  Continue big'ole line
        */
       if ( fTooManyPoints ) {

           /*
            *  Copy last two points to top
            */
           if ( Flags.clipping == TW_CLIP_COMPLEX ) {
               ptfxFirst = vtwptfx[(MAX_POINTS-3)];
               ptfxB     = vtwptfx[(MAX_POINTS-2)];
           }
           else {
              ptfxFirst  = vtwptfx[(MAX_POINTS-2)];
              vtwptfx[0] = vtwptfx[(MAX_POINTS-2)];
           }
           goto GetMorePoints;
       }
    } while ( !SubPath.fLast );


#ifdef DOS
    TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: ulVgaMode(%08lX)", ulVgaMode ));
    vClearStrips(  ulVgaMode );
#else

    /*
     *  Destroy the clipping region
     */
    if ( hrgnDest != NULL ) {
        SelectClipRgn( hDC, NULL );
        DeleteObject( hrgnDest );
    }

#endif


    TRACE(( TC_TW, TT_TW_STROKE, "TWCmdStrokePath: Exiting" ));
    TWCmdReturn( !rc ); // return to NewNTCommand or ResumeNTCommand
}


#ifdef DOS

/******************************Public*Routine******************************\
*
* VOID vCatchTwoPass(pstrip, pls, plStripEnd)
*
* Handles ROPs that cannot be done in a single pass using the VGA
* hardware.  In order not to have a check in our main drawing loop for
* two-pass ROPs, we change the strip function table so that this function
* intercepts the call to draw the strips.
*
* This routine then figures out the appropriate actual strip drawer, and
* makes two calls to it: first to invert the destination, then to do the
* rest of the ROP.
*
\**************************************************************************/

VOID far 
vCatchTwoPass(STRIP* pstrip, LINESTATE* pls, LONG* plStripEnd)
{
    BYTE*     pjScreen    = pstrip->pjScreen;
    BYTE      jBitMask    = pstrip->jBitMask;
    BYTE      jStyleMask  = pstrip->jStyleMask;
    STYLEPOS* psp         = pstrip->psp;
    STYLEPOS  spRemaining = pstrip->spRemaining;

// Figure out the actual strip routine we're supposed to call:

    PFNSTRIP pfn = pls->apfnStrip[(pstrip->flFlips & FL_STRIP_MASK) >>
                                  FL_STRIP_SHIFT];

// On the first pass, we invert the destination:
    vSetStrips(0xff, DR_XOR);

    (*pfn)(pstrip, pls, plStripEnd);

// We reset our strip variables for the second pass and handle the rest
// of the ROP:

    pstrip->pjScreen    = pjScreen;
    pstrip->jBitMask    = jBitMask;
    pstrip->jStyleMask  = jStyleMask;
    pstrip->psp         = psp;
    pstrip->spRemaining = spRemaining;

    vSetStrips(pls->iColor, pls->ulVgaMode);

    (*pfn)(pstrip, pls, plStripEnd);
}

#endif


BOOL 
FindBoundary( TWPOINTFIXI ptfxFirst, TWPOINTFIXI *pptfxBuf, TWRUNI *pRUN,
              ULONG cptfx, RECTI *prclBoundary )
{
    BOOL fSuccess = TRUE;
    int xLow, xHigh, yLow, yHigh;
    int xNew, yNew;
    ULONG i;

    if ( pRUN ) {
        // For complex clipping, there are only two points,
        // ptfxFirst and pptfxBuf[0]
        cptfx = 1;
    }

    xLow = xHigh = ptfxFirst.x >> 4;  // Lose the fractional part
    yLow = yHigh = ptfxFirst.y >> 4;  // Lose the fractional part
    xHigh++; yHigh++;                 // add one in case we dropped a fraction

    TRACE(( TC_TW, TT_TW_STROKE, "FindBoundary: start with (%d, %d)-(%d, %d) ",
              xLow, yLow, xHigh, yHigh ));

    for ( i = 0; i < cptfx; i++ ) {
        if ( (xNew = pptfxBuf[i].x >> 4) < xLow ) {
            xLow = xNew;
        } else {
            xNew++;
            if ( xNew > xHigh ) {
                xHigh = xNew;
            }
        }
        if ( (yNew = pptfxBuf[i].y >> 4) < yLow ) {
            yLow = yNew;
        } else {
            yNew++;
            if ( yNew > yHigh ) {
                yHigh = yNew;
            }
        }

        TRACE(( TC_TW, TT_TW_STROKE, "FindBoundary: Point (%d, %d) Bbounds (%d, %d)-(%d, %d) ",
                  xNew, yNew, xLow, yLow, xHigh, yHigh ));
    }

    prclBoundary->left   = xLow;
    prclBoundary->top    = yLow;
    prclBoundary->right  = xHigh;
    prclBoundary->bottom = yHigh;

    return( fSuccess );
}
