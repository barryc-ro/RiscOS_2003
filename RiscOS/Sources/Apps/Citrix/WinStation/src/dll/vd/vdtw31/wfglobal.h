
/*******************************************************************************
*
*   WFGLOBAL.H
*
*   Thin Wire Windows - Global Data Header
*
*   Copyright (c) Citrix Systems Inc. 1995-1996
* 
*   Author: Jeff Krantz (jeffk)
*
*   $Log$
*  
*     Rev 1.14   15 Apr 1997 18:17:00   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   08 May 1996 14:50:00   jeffm
*  update
*  
*     Rev 1.12   03 Jan 1996 14:02:50   kurtp
*  update
*  
*******************************************************************************/

#ifndef __WFGLOBAL_H__
#define __WFGLOBAL_H__

#include "wfthin.h"

extern UINT DIB_ColorMode;

extern COLORREF  twsolidcolor[16];

extern BITMAPINFO_4BPP_RGBQUAD bitmapinfo_4BPP_RGBQUAD;
extern BITMAPINFO_4BPP_PALETTE bitmapinfo_4BPP_PALETTE;

//jk256
extern BITMAPINFO_4BPP_PALETTE bitmapinfo_4BPP_PALETTE_default256;
extern BITMAPINFO_4BPP_PALETTE bitmapinfo_4BPP_PALETTE_last256;

//jk256
extern BITMAPINFO_8BPP_PALETTE bitmapinfo_8BPP_PALETTE;
extern BITMAPINFO_8BPP_PALETTE bitmapinfo_8BPP_PALETTE_MAP;

//jk256
extern BYTE convert_default16to256[16];

#if 0    //not using these right now for 2 color bitmaps
extern BITMAPINFO_1BPP_RGBQUAD bitmapinfo_1BPP_RGBQUAD;
extern BITMAPINFO_1BPP_PALETTE bitmapinfo_1BPP_PALETTE;
#endif

extern BITMAPINFO_4BPP_RGBQUAD bitmapinfo_SSB;

extern WORD stupid_rop_info[256];

extern BYTE Rop3ToRop2[256];

extern HBRUSH hbrsolid[16];
extern HBRUSH hbrsolid256[21];
extern ULONG  lasthbrsolid256index;

extern HPEN   hpensolid[16];
extern HPEN   hpensolid256[21];

extern DCSTATE dcstate;

extern BRUSHOBJOBJ brushobjobj[115];

extern BRUSHDIB BrushDIB[MAXBRUSHREALIZED + 1];

/*
 *  This buffer size must be at least 3*2K + 4 for DIB caching!
 *  See WFGLOBAL.C for more information, do not change this
 *  without contacting JeffK for KurtP.  The value was bumped
 *  up to 8K to make text i/o much faster with upto 48 pt glyphs.
 */
#define STATIC_BUFFER_SIZE  8192

extern LPDWORD lpstatic_buffer;

extern int number_nonsolidbrushes_realized;

extern HDC compatDC;

extern COLOR_CAPS vColor;
#endif //__WFGLOBAL_H__
