
/*******************************************************************************
*
*   TWBEZ.C
*
*   Thin Wire Windows - Bezier Code
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*  
*     Rev 1.3   15 Apr 1997 18:15:56   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.3   21 Mar 1997 16:09:26   bradp
*  update
*  
*     Rev 1.2   30 May 1996 16:56:58   jeffm
*  update
*  
*     Rev 1.1   03 Jan 1996 13:32:50   kurtp
*  update
*  
*******************************************************************************/

#ifdef DOS

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include "vmsetjmp.h"

#include "../../../inc/client.h"
#include "../../../inc/logapi.h"
#include "twtype.h"
#include <citrix\ica.h>
#include <citrix\ica-c2h.h>
#include <citrix\twcommon.h>
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

BOOL bBezierInit( TWPOINTFIXI* aptfxBez, RECTFX* prcfxClip);
BOOL bBezierNext(TWPOINTFIXI* pptfx, ULONG *pcptfx );
#ifdef LATER
BOOL bIntersect(RECTFX* prcfx1, RECTFX* prcfx2);
#endif

#ifdef DOS

#define MAX_FLAT_POINTS (LARGE_CACHE_CHUNK_SIZE / 4) // size of static_buffer_2
#define vtwptfx static_buffer_2
extern TWPOINTFIXI vtwptfx[MAX_FLAT_POINTS];
TWPOINTFIXI * vptwptfx = &vtwptfx[0];

#else

TWPOINTFIXI * vptwptfx = NULL;

#endif

// These macros aren't as legible as desired, but they are fast!
#define fxValue( p ) ((p.e0 + (1L << 12)) >> 13)
#define vHFDTakeStep(p) {a=p.e2;p.e0+=p.e1;p.e1+=a;p.e2+=a-p.e3;p.e3=a;}
#define vHFDHalveStepSize(p) {p.e2=(p.e2+p.e3)>>3;p.e1=(p.e1-p.e2)>>1;p.e3>>=2;}
#define vHFDDoubleStepSize(p) {p.e1+=p.e1+p.e2;p.e3<<=2;p.e2=(p.e2<<3)-p.e3;}
#define vHFDLazyHalveStepSize(p,s) {p.e2=(p.e2+p.e3)>>1;p.e1=(p.e1-(p.e2>>s))>>1;}
#define vHFDInit(p,p1,p2,p3,p4) {p.e0=(p1)<<10;p.e1=(p4-p1)<<10;p.e2=(3*(p2-p3-p3+p4))<<11;p.e3=(3*(p1-p2-p2+p3))<<11;}
#define vHFDSteadyState(p,s) {a=s-3;p.e0<<=3;p.e1<<=3;if(a<0){a=-a;p.e2<<=a;p.e3<<=a;}else{p.e2>>=a;p.e3>>=a;}}

void EllipseToBeziers( PELLIPSEDATA pptfx, TWPOINTFIXI *pptfxB );

VOID bFlatten( ULONG *pcptfx, TWPOINTFIXI * *ppptfxBuf, BOOL fEllipse )
{
    TWPOINTFIXI * pptfxNext;
    TWPOINTFIXI * pptfxLast;
    ULONG        cptfx = 0L;
#ifndef DOS
    LPBYTE p = (LPBYTE) lpstatic_buffer;

    vptwptfx = (TWPOINTFIXI *) (p + LARGE_CACHE_CHUNK_SIZE * 2);
#endif

   if ( fEllipse ) {
      pptfxNext = &((*ppptfxBuf)[3]); // Point to buffer just after the 3 points
      EllipseToBeziers( (PELLIPSEDATA)(*ppptfxBuf), pptfxNext );
      *ppptfxBuf = pptfxNext; // Reset the pointer for the bezier flattener
      *pcptfx = 13;
   }

   {
       ULONG i;

       for ( i = 0; i < *pcptfx; i++ ) {
          TRACE(( TC_TW, TT_TW_STROKE, "bFlatten:    %03ld (%04X, %04X)",
                     i, ((*ppptfxBuf)[i]).x, ((*ppptfxBuf)[i]).y ));
       }
   }


   for ( pptfxNext = *ppptfxBuf, pptfxLast = &((*ppptfxBuf)[*pcptfx-1]);
         pptfxNext < pptfxLast; 
         pptfxNext = &pptfxNext[3] ) {
      if ( !bBezierInit( pptfxNext, (RECTFX*)NULL )      ) {
         break;
      }
      *(vptwptfx + cptfx++) = pptfxNext[0]; // start with first point
      TRACE(( TC_TW, TT_TW_STROKE, "bFlatten: Flattened point %03ld (%04X, %04X)",
              cptfx, pptfxNext[0].x, pptfxNext[0].y ));

      bBezierNext( (vptwptfx + cptfx), &cptfx );
   }
   TRACE(( TC_TW, TT_TW_STROKE, "bFlatten: Returning %ld points", cptfx ));

   *pcptfx    = cptfx;
   *ppptfxBuf = vptwptfx;
}


static LONG vcSteps;
static HFD vxHFD;
static HFD vyHFD;
static RECTFX vrcfxBound;

BOOL bBezierInit(       // from gre\pathflat.cxx
TWPOINTFIXI* aptfxBez,     // Pointer to 4 control points
RECTFX* prcfxClip)      // Bound box of visible region (optional)
{
    POINTFIX aptfx[4];
    LONG cShift = 0;    // Keeps track of 'lazy' shifts
    register FIX fxOr;
    register FIX fxOffset;
    LONG a, b, c;

    vcSteps = 1;         // Number of steps to do before reach end of curve

    // convert our shorts to longs
    aptfx[0].x = (LONG)aptfxBez[0].x;
    aptfx[0].y = (LONG)aptfxBez[0].y;
    aptfx[1].x = (LONG)aptfxBez[1].x;
    aptfx[1].y = (LONG)aptfxBez[1].y;
    aptfx[2].x = (LONG)aptfxBez[2].x;
    aptfx[2].y = (LONG)aptfxBez[2].y;
    aptfx[3].x = (LONG)aptfxBez[3].x;
    aptfx[3].y = (LONG)aptfxBez[3].y;

    if (aptfx[0].x >= aptfx[1].x)
        if (aptfx[2].x >= aptfx[3].x)
        {
            vrcfxBound.xLeft  = MIN(aptfx[1].x, aptfx[3].x);
            vrcfxBound.xRight = MAX(aptfx[0].x, aptfx[2].x);
        }
        else
        {
            vrcfxBound.xLeft  = MIN(aptfx[1].x, aptfx[2].x);
            vrcfxBound.xRight = MAX(aptfx[0].x, aptfx[3].x);
        }
    else
        if (aptfx[2].x <= aptfx[3].x)
        {
            vrcfxBound.xLeft  = MIN(aptfx[0].x, aptfx[2].x);
            vrcfxBound.xRight = MAX(aptfx[1].x, aptfx[3].x);
        }
        else
        {
            vrcfxBound.xLeft  = MIN(aptfx[0].x, aptfx[3].x);
            vrcfxBound.xRight = MAX(aptfx[1].x, aptfx[2].x);
        }

    if (aptfx[0].y >= aptfx[1].y)
        if (aptfx[2].y >= aptfx[3].y)
        {
            vrcfxBound.yTop    = MIN(aptfx[1].y, aptfx[3].y);
            vrcfxBound.yBottom = MAX(aptfx[0].y, aptfx[2].y);
        }
        else
        {
            vrcfxBound.yTop    = MIN(aptfx[1].y, aptfx[2].y);
            vrcfxBound.yBottom = MAX(aptfx[0].y, aptfx[3].y);
        }
    else
        if (aptfx[2].y <= aptfx[3].y)
        {
            vrcfxBound.yTop    = MIN(aptfx[0].y, aptfx[2].y);
            vrcfxBound.yBottom = MAX(aptfx[1].y, aptfx[3].y);
        }
        else
        {
            vrcfxBound.yTop    = MIN(aptfx[0].y, aptfx[3].y);
            vrcfxBound.yBottom = MAX(aptfx[1].y, aptfx[2].y);
        }

    fxOffset = vrcfxBound.xLeft;
    fxOr  = (aptfx[0].x -= fxOffset);
    fxOr |= (aptfx[1].x -= fxOffset);
    fxOr |= (aptfx[2].x -= fxOffset);
    fxOr |= (aptfx[3].x -= fxOffset);

    fxOffset = vrcfxBound.yTop;
    fxOr |= (aptfx[0].y -= fxOffset);
    fxOr |= (aptfx[1].y -= fxOffset);
    fxOr |= (aptfx[2].y -= fxOffset);
    fxOr |= (aptfx[3].y -= fxOffset);

#ifdef LATER
    // This 32 bit cracker can only handle points in a 10 bit space:

    if ((fxOr & 0xffffc000) != 0)
        return(FALSE);
#endif

    vHFDInit( vxHFD, aptfx[0].x, aptfx[1].x, aptfx[2].x, aptfx[3].x);
    vHFDInit( vyHFD, aptfx[0].y, aptfx[1].y, aptfx[2].y, aptfx[3].y);

    if (prcfxClip == (RECTFX*) NULL
#ifdef LATER
        || bIntersect(&vrcfxBound, prcfxClip)
#endif
        )
    {
        while (TRUE)
        {
            register LONG lTestMagnitude = TEST_MAGNITUDE_INITIAL << cShift;

//          lError( &vxHFD )
            a = ABS(vxHFD.e2);
            b = ABS(vxHFD.e3);
            c = MAX( a, b );

//          lError( &vyHFD )
            a = ABS(vyHFD.e2);
            b = ABS(vyHFD.e3);
            a = MAX( a, b );

            if ( c <= lTestMagnitude && a <= lTestMagnitude)
                break;

            cShift += 2;
            vHFDLazyHalveStepSize( vxHFD, cShift);
            vHFDLazyHalveStepSize( vyHFD, cShift);
            vcSteps <<= 1;
        }
    }

    vHFDSteadyState( vxHFD, cShift);
    vHFDSteadyState( vyHFD, cShift);

// Note that this handles the case where the initial error for
// the Bezier is already less than TEST_MAGNITUDE_NORMAL:

    vHFDTakeStep( vxHFD );
    vHFDTakeStep( vyHFD );
    vcSteps--;

    return(TRUE);
}

BOOL bBezierNext(TWPOINTFIXI* pptfx, ULONG * pcptfx )   // from gre\pathflat.cxx
{
   LONG a, b, c;
   int i;


   for ( i = 0; vcSteps >= 0; i++, vcSteps-- ) {

// Return current point:

      pptfx[i].x = (SHORT)(fxValue(vxHFD) + vrcfxBound.xLeft);
      pptfx[i].y = (SHORT)(fxValue(vyHFD) + vrcfxBound.yTop);

      TRACE(( TC_TW, TT_TW_STROKE, "bBezierNext: Flattened point %03ld (%04X, %04X)",
                (LONG)i + 1L + *pcptfx, pptfx[i].x, pptfx[i].y ));

// If vcSteps == 0, that was the end point in the curve!

      if ( vcSteps == 0 ) break;

// Okay, we have to step:

//    lError( &vxHFD )
      a = ABS(vxHFD.e2);
      b = ABS(vxHFD.e3);
      c = MAX( a, b );

//    lError( &vyHFD )
      a = ABS(vyHFD.e2);
      b = ABS(vyHFD.e3);
      a = MAX( a, b );

      if ( MAX( c, a ) > TEST_MAGNITUDE_NORMAL )
      {
          vHFDHalveStepSize( vxHFD );
          vHFDHalveStepSize( vyHFD );
          vcSteps <<= 1;
      }


      while ( TRUE )
      {
//        lParentError(&vxHFD)
          a = ABS(vxHFD.e3 << 2);
          b = ABS((vxHFD.e2 << 3) - (vxHFD.e3 << 2));
          c = MAX( a, b );

//        lParentError(&vyHFD)
          a = ABS(vyHFD.e3 << 2);
          b = ABS((vyHFD.e2 << 3) - (vyHFD.e3 << 2));
          a = MAX( a, b );

          if ( !(!(vcSteps & 1) &&
                 c <= TEST_MAGNITUDE_NORMAL &&
                 a <= TEST_MAGNITUDE_NORMAL ) ) break;

          vHFDDoubleStepSize( vxHFD );
          vHFDDoubleStepSize( vyHFD );
          vcSteps >>= 1;
      }

      vHFDTakeStep( vxHFD );
      vHFDTakeStep( vyHFD );

   }

    *pcptfx += i + 1;
    return(TRUE);
}

#ifdef LATER
BOOL bIntersect(RECTFX* prcfx1, RECTFX* prcfx2) // from gre\pathflat.cxx
{
    BOOL bRet = (prcfx1->yTop <= prcfx2->yBottom &&
                 prcfx1->yBottom >= prcfx2->yTop &&
                 prcfx1->xLeft <= prcfx2->xRight &&
                 prcfx1->xRight >= prcfx2->xLeft);
    return(bRet);
}
#endif


void EllipseToBeziers( PELLIPSEDATA pptfx, TWPOINTFIXI *pptfxB )
{
    SHORT Ax, By, Cx, Dy;

    TRACE(( TC_TW, TT_TW_STROKE, "EllipseToBeziers: l(%04X) t(%04X) r(%04X) b(%04X)",
              pptfx->xLeft, pptfx->yTop, pptfx->xRight, pptfx->yBottom ));
    TRACE(( TC_TW, TT_TW_STROKE, "EllipseToBeziers: xBez1(%04X) yBez2(%04X)",
              pptfx->xBez1, pptfx->yBez2 ));

    // Initialize some variables
    Ax = ( pptfx->xRight - pptfx->xLeft   ) >> 1;
    By = ( pptfx->yTop   - pptfx->yBottom ) >> 1;

    Cx = pptfx->xRight - pptfx->xBez1;
    Dy = pptfx->yTop   - pptfx->yBez2;

    TRACE(( TC_TW, TT_TW_STROKE, "EllipseToBeziers: Ax(%04X) By(%04X) Cx(%04X) Dy(%04X)",
              Ax, By, Cx, Dy ));

    // Let's go for it...
    pptfxB[0].x  = pptfx->xRight      ;
    pptfxB[0].y  = pptfx->yBottom + By;
    pptfxB[1].x  = pptfx->xRight      ;
    pptfxB[1].y  = pptfx->yBez2       ;
    pptfxB[2].x  = pptfx->xBez1       ;
    pptfxB[2].y  = pptfx->yTop        ;
    pptfxB[3].x  = pptfx->xRight  - Ax;
    pptfxB[3].y  = pptfx->yTop        ;
    pptfxB[4].x  = pptfx->xLeft   + Cx;
    pptfxB[4].y  = pptfx->yTop        ;
    pptfxB[5].x  = pptfx->xLeft       ;
    pptfxB[5].y  = pptfx->yTop    - Dy;
    pptfxB[6].x  = pptfx->xLeft       ;
    pptfxB[6].y  = pptfx->yTop    - By;
    pptfxB[7].x  = pptfx->xLeft       ;
    pptfxB[7].y  = pptfx->yBottom + Dy;
    pptfxB[8].x  = pptfx->xLeft   + Cx;
    pptfxB[8].y  = pptfx->yBottom     ;
    pptfxB[9].x  = pptfx->xLeft   + Ax;
    pptfxB[9].y  = pptfx->yBottom     ;
    pptfxB[10].x = pptfx->xRight  - Cx;
    pptfxB[10].y = pptfx->yBottom     ;
    pptfxB[11].x = pptfx->xRight      ;
    pptfxB[11].y = pptfx->yBottom + Dy;
    pptfxB[12].x = pptfx->xRight      ;
    pptfxB[12].y = pptfx->yBottom + By;

    {
        int i;

        for ( i = 0; i < 13; i++ ) {
           TRACE(( TC_TW, TT_TW_STROKE, "EllipseToBeziers: Point %03d (%04X,%04X)",
                     i, pptfxB[i].x, pptfxB[i].y ));
        }
    }

}
