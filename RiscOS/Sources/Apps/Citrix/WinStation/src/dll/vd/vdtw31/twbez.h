
/*******************************************************************************
*
*   TWBEZ.H
*
*   Thin Wire Windows - Bezier Header
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*  
*     Rev 1.3   15 Apr 1997 18:15:58   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   08 May 1996 14:45:06   jeffm
*  update
*  
*     Rev 1.1   03 Jan 1996 13:34:56   kurtp
*  update
*  
*******************************************************************************/

#ifndef __TWBEZ_H__
#define __TWBEZ_H__

typedef struct _BEZIERCONTROLS {        // from gre\pathflat.cxx  
    POINTFIX ptfx[4];
} BEZIERCONTROLS;

typedef struct _HFD {
   LONG  e0;
   LONG  e1;
   LONG  e2;
   LONG  e3;
} HFD;

typedef struct _RECTFX
{
   LONG     xLeft;
   LONG     yTop;
   LONG     xRight;
   LONG     yBottom;
} RECTFX, *PRECTFX;


#define LTOFX(x) ((x)<<4)

#define TEST_MAGNITUDE_INITIAL    (6 * 0x00002aa0L)
#define TEST_MAGNITUDE_NORMAL     (TEST_MAGNITUDE_INITIAL << 3)

VOID bFlatten( ULONG *pcptfx, TWPOINTFIXI * *ppptfxBuf, BOOL fEllipse );

#endif //__TWBEZ_H__
