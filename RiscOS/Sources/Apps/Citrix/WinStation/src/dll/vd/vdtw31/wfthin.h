
/*******************************************************************************
*
*   WFTHIN.H
*
*   Thin Wire Windows - Protocol Structures
*
*   Copyright (c) Citrix Systems Inc. 1995-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*  
*     Rev 1.7   15 Apr 1997 18:17:12   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.7   21 Mar 1997 16:09:42   bradp
*  update
*  
*     Rev 1.6   08 May 1996 14:41:24   jeffm
*  update
*  
*     Rev 1.5   03 Jan 1996 14:02:54   kurtp
*  update
*  
*******************************************************************************/

#ifndef __WFTHIN_H__
#define __WFTHIN_H__

#include "windows.h"
#include "../../../inc/client.h"
#include "../../../inc/logapi.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"
#include "wfcache.h"
#include "twdata.h"
#include "twwin.h"

//define THINPAL to build supporting bitmaps from the default logical palette
//and the solidcolor array from the default logical palette
//this is in lieu of absolute RGB colors
//
//jk256 - NOTE THINPAL MUST BE DEFINED FOR the 256 color driver
//          to build correctly.  code left in to show alternative
//          for 16 colors only to work
#define THINPAL


#define MAXBRUSHREALIZED   16

typedef  struct   _INPUT_WORD
{
   WORD  lowbyte/* :  8*/;
   WORD  hibyte /* : 8 */;
} INPUT_WORD, near * PINPUT_WORD, far * LPINPUT_WORD;

typedef  struct   _RECT_ULH
{
   int   left;
   int   top;
   int   width;
   int   height;
} RECT_ULH, near * PRECT_ULH, far * LPRECT_ULH;



typedef struct _DSDELTA
{
   UINT  deltax_last;
   UINT  deltay_last;
} DSDELTA, near * PDSDELTA, far * LPDSDELTA;

//used to hold the current or initial state of the DC
//for anything that might change in it
typedef struct _DCSTATE
{
   HBRUSH   hbr;        //currently selected brush handle. always solid or in objobj
   int      bkmode;     //background mode. OPAQUE or TRANSPARENT
   COLORREF bkcolor;    //background color
   COLORREF txtcolor;   //text color
   int      pencolor;   //pen color (index)
   int      rop2;
   int      xBrushOrg;
   int      yBrushOrg;
#ifdef WIN16
   POINT    screenULH;
   int      lastbrush;  //has objobj index if nonsolid color - used for case
                        //when client window has moved so need to rerotate
                        //the currently selected brush
                        //has negative number of (index+1) if solid
#endif
} DCSTATE, near * PDCSTATE, far * LPDCSTATE;

//used to state the 3 possible source bitblt cases
typedef  enum  _SRCBLT_CASE
{
   noclip,
   simpleclip,
   cmplxclip
} SRCBLT_CASE;

//used to pass parms from the 3 source bitblt packet crackers
//to a common routine to handle the dsBITMAP datastream
typedef  struct   _DSBITMAP_PARMS
{
   SRCBLT_CASE packet_case;
   UINT        simpleclip_case;     //need in case width & height
                                    //based off of bitmap info
   int         simpleclip_srcwidth;    //simpleclip case only
   int         simpleclip_srcheight;   // "
   int         xSrcULH;             //for simpleclip & cmplxclip
   int         ySrcULH;             //for simpleclip & cmplxclip
} DSBITMAP_PARMS, near * PDSBITMAP_PARMS, far * LPDSBITMAP_PARMS;


//used to hold all the brush objobj related information
//if the HBRUSH is not NULL then a brush has been realized with the GDI
//we only allow MAXBRUSHREALIZED non solid color brushes realized at a time
//when we check the objobj, if the HBRUSH is NULL we need to realize
//a brush, so we may have to delete a different realized brush
//to help us do that we have a next and previous pointer in the objobj array
//which has an array index to the previous and next lru element in the
//array where lru is done on when the brush was last used.
//113 is used as the end of chain anchor and 112 is used as the
//beginning of chain pointer.  There are structure indexes for 112 and 113
//but they are only used as the doubly linked list anchors and nothing else
//Index 114 is used to hold a realized brush handle if no caching.
//that way we know what brush handle to delete if we have another no caching
//case
//On chain when HBRUSH is not NULL
//HBRUSH is not NULL if a brush is realized and NULL if it isn't

//jk256 - changes for 256 colors
//       color is changed from a byte to a word
//          in 16 color mode only the low order byte is used as before
//          in 256 color mode if its a 2 color brush then the low
//             order byte is the F color and the high order byte is the B color
//             if not a 2 color brush then it is 0
//       cache handle - now the low order 12 bits has the object handle
//             in 256 color mode, the high order bit is 0 if 2 or 16 color
//                   and is 1 if its a 256 color brush.  this way we know
//                   how to access the cache
typedef struct _BRUSHOBJOBJ
{
   HBRUSH   hbr;     //realized brush representing this objobj entry or NULL
   BYTE     previous;
   BYTE     next;
   WORD     cache_handle;  //10 bit client cache object handle
                           //jk256 12 bit object handle and high order bit
                           //    is set to 1 if a 256 color brush
   WORD     color;      //color info - same as datastream if 2 color in cache
                        //             0 if 16 color in cache
                        //jk256 - see above for change to 256 color mode
   BYTE     rotation;      //same format as protocol
   BYTE     dib_index;   //has index into BrushDIB describing dib bitmap
} BRUSHOBJOBJ, near * PBRUSHOBJOBJ, far * LPBRUSHOBJOBJ;


//for realized brushes pointed to by brushobjobj.dibindex
//the last entry in the array is used for the no caching case
//until realize all brushes, number_nonsolidbrushes_realized is used
//as the next index.
//then use index used by entry taken off of the lru list
//handles and pointers obtained during initialization
typedef  struct _BRUSHDIB
{
   HGLOBAL  dib_handle;
   LPVOID   dib_address;
} BRUSHDIB, near * PBRUSHDIB, far * LPBRUSHDIB;


//if the 8 byte header for a bitmap that may or may not be cached
//if the bitmap is cached, then this header precedes the bitmap data
//which follows in the cache slot.  The 8 byte header is not used
//if its the special bitmap case of exactly 32 by 32 16 color or
//64 by 64 16 color

//NOTE: The host calculates the number of logical scanlines that can fit
//in a _2K or _2K-8 segment based on the logical scanline being stored as
//a ddb.  A logical scanline stored as a ddb can take up to 3 more bytes than
//a logical scanline stored as a dib.  In the windows client, we store
//bitmaps as dibs.  It says in the WIN32 spec that "the bits in the array
//are packed together, but each scan line must be padded with zeroes to end
//on a LONG data-type boundary.
//For 16 color bitmaps -We got really lucky here since we want to blt
//right out of the cache.  If we pad dib scanlines to end on a LONG boundary
//(4 byte boundary), then a dib scanline will take up as much space as a
//ddb scanline because a ddb scanline is in units of 8 pixels and so is a dib
//scanline if its in quanta of 4 bytes.  This makes our movement into the
//cache a little more complex because we must insert zeroes where required.
//!!!!BUT For 2 color bitmaps and savescreenbitmap we will probably need to
//build a temporary bitmap because we must pad all the scanlines to end on
//on LONG boundaries!!!!
//
//
typedef  struct _BITMAP_HEADER_FLAGS
{
   BYTE  color/* : 2*/;     //0-monochrome bitmap   1-16 color bitmap
                        //10-256 color bitmap
                        //11 reserved for future color stuff
   BYTE  size/*  : 1*/;     //0-_2K bitmap          1-_512B bitmap
   BYTE  chained/* : 1*/;   //0-not chained         1-chained
   BYTE  reserved/* : 4*/;  //not used right now
} BITMAP_HEADER_FLAGS;

//jk256 - move flags to first byte because in future may need to decide how much control
//       store based on the flags
//#pragma pack(1)
typedef struct _BITMAP_HEADER
{
   BITMAP_HEADER_FLAGS flags; //# colors, _512 or _2K, chained or not
   BYTE  pixel_offset;        //The pixel offset of the bitmap
   WORD  total_scanlines;     //total number of logical scanlines of bitmap
                              //this is the pixel height
   WORD  cache_bytes_wide;    //number of bytes in a cached dib scanline
                              //FOR 16 color bitmap:
                              //same as assembler ddb width because every
                              //scanline cached have a modula 4 length
                              //so we can stretchdibits right out of cache
                              //FOR 2 color bitmap
                              //same as assembler which is ddb width
   WORD  pixel_width;         //The width of the bitmap in pixels
} BITMAP_HEADER, near * PBITMAP_HEADER, far * LPBITMAP_HEADER;

//#pragma pack()


//the following are the various bitmapinfo typedefs for 16 color and
//2 color bitmaps.  See global.c for comments on how used

//jk256
typedef struct  _BITMAPINFO_8BPP_PALETTE
{
   BITMAPINFOHEADER  bmiHeader;
   WORD              bmiColors[256];
} BITMAPINFO_8BPP_PALETTE, near * PBITMAPINFO_8BPP_PALETTE,
                           far * LPBITMAPINFO_8BPP_PALETTE;


typedef struct  _BITMAPINFO_4BPP_RGBQUAD
{
   BITMAPINFOHEADER  bmiHeader;
   RGBQUAD           bmiColors[16];
} BITMAPINFO_4BPP_RGBQUAD, near * PBITMAPINFO_4BPP_RGBQUAD,
                           far * LPBITMAPINFO_4BPP_RGBQUAD;


typedef struct  _BITMAPINFO_4BPP_PALETTE
{
   BITMAPINFOHEADER  bmiHeader;
   WORD              bmiColors[16];
} BITMAPINFO_4BPP_PALETTE, near * PBITMAPINFO_4BPP_PALETTE,
                           far * LPBITMAPINFO_4BPP_PALETTE;


typedef struct  _BITMAPINFO_1BPP_RGBQUAD
{
   BITMAPINFOHEADER  bmiHeader;
   RGBQUAD           bmiColors[2];
} BITMAPINFO_1BPP_RGBQUAD, near * PBITMAPINFO_1BPP_RGBQUAD,
                           far * LPBITMAPINFO_1BPP_RGBQUAD;


typedef struct  _BITMAPINFO_1BPP_PALETTE
{
   BITMAPINFOHEADER  bmiHeader;
   WORD              bmiColors[2];
} BITMAPINFO_1BPP_PALETTE, near * PBITMAPINFO_1BPP_PALETTE,
                           far * LPBITMAPINFO_1BPP_PALETTE;


//the following in the control header for the savescreenbitmap save area
//the size of the header must be divisible by 4
//we use a size of 16 here but the protocol allows for 32 bytes.  This means that when we
//do the number of blocks calculation we assume a 32 byte control area
#define SSB_HEADER_SIZE 18
//#pragma pack(1)
typedef struct _SSB_HEADER
{
   WORD  ULH_x;      //original save from ulh_x
   WORD  ULH_y;      //original save from ulh_y
   WORD  total_scanlines;  //height of saved bitmap in pixels (scanlines are logical)
   WORD  pixel_width;      //the width of the relevant saved data in pixels
   WORD  byte_width;       //the number of bytes per scanline saved.
                           //left justified on 8 pixel boundary and # bytes divisible by 4
   WORD  total_blocks;     //total number of cache blocks that make up the object
   WORD  scanlines_in_block0; //value is 0 if cant use optimization number 1 so scanlines cross
                              //cache block boundaries.  else number of scanlines in block0
   WORD  scanlines_in_blockn; //if can use optimization number 1 its the number of whole
                              //scanlines that live in the rest of the cache blocks.
                              //There are probably fewer scanlines in the last block
   WORD  count_ontop;         //a count of the number of detected local windows that are on
                              //top of our savescreenbitmap area at savescreen time
                              //if the value is 0 then there were none
                              //if the value is 1 or 2 then the rectangles follow at 6
                              //bytes per rectangle
                              //if the count is >2 then no rectangle information follows
} SSB_HEADER, near * PSSB_HEADER, far * LPSSB_HEADER;



//#pragma pack()




////////////////////////////////////////////////////////////////////
// routines
////////////////////////////////////////////////////////////////////


//see protocol document for description of 3BYTES
void  Input3Bytes(LPWORD lpword1, LPWORD lpword2);

//see protocol document for description of 3byteslong
void  Input3BytesLong(LPDWORD lpdword1);

//create_brush reads protocol to eventually
//select a brush object into the DC that is passed in as a parm
//returns TRUE is no error
BOOL  create_brush(HWND hwnd, HDC device);

//requests a host repaint of cRect number of rectangles pointed to by lpRect
int  wfnRepaintRects(LPRECT lpRect, INT cRect, BOOL fScreenToScreen);

#endif //WFTHIN_H
