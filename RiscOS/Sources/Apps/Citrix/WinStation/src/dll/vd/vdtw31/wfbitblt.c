
/*******************************************************************************
*
*   WFBITBLT.C
*
*   Thin Wire Windows - BitBlt Code
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Jeff Krantz (jeffk)
*
*   $Log$
*   Revision 1.2  1998/01/27 18:39:11  smiddle
*   Lots more work on Thinwire, resulting in being able to (just) see the
*   log on screen on the test server.
*
*   Version 0.03. Tagged as 'WinStation-0_03'
*
*   Revision 1.1  1998/01/19 19:12:58  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.15   04 Aug 1997 19:18:22   kurtp
*  update
*  
*     Rev 1.14   15 Apr 1997 18:16:48   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.14   24 Mar 1997 11:23:02   kurtp
*  update
*  
*     Rev 1.13   13 Jan 1997 16:52:10   kurtp
*  Persistent Cache
*  
*     Rev 1.12   08 May 1996 14:54:14   jeffm
*  update
*  
*     Rev 1.11   27 Jan 1996 11:35:12   kurtp
*  update
*  
*     Rev 1.10   03 Jan 1996 13:33:40   kurtp
*  update
*  
*******************************************************************************/

#include <string.h>

#include "wfglobal.h"

#include "../../../inc/clib.h"

//complex clipping enumeraton support
#define  RECTFULL    0
#define  NEXTTINY    1
#define  NEXTMEDIUM1 2
#define  NEXTMEDIUM2 3
#define  NEXTHUGE    4
#define  SAMEMEDIUM  5
#define  SAMEHUGE    6
#define  RECTLAST    7

//dsDELTA update support
#define  DELTASAME   0
#define  DELTATINYX  1
#define  DELTATINYY  2
#define  DELTAMEDX   3
#define  DELTAMEDY   4
#define  DELTALRGEQ  5
#define  DELTALARGE  6
#define  DELTAHUGE   7


static RECT lastclip, lastscanclip;    //holds state for cmplxCLPenum

static DSDELTA nosrcblt_dsdelta, scrtoscrblt_dsdelta;

//has state of last fb color. used for monochrome bitmaps only
//jk256 - in 256 color mode last_fbcolor has the foreground color
//       and last_backColor has the background color
static BYTE last_fbColor;
static BYTE last_backColor;


//RleDecompress is passed the amount of rle data that needs to be gotten
//and decompressed (rlecount).
//It points to the input data to be decompressed by input parm lpinput.
//It puts the decompressed data in a buffer pointed to by input parm
//lpoutput. lpoutput is guaranteed to be big enough
//returns the amount of data as a result of the decompression
//returns 0 if error

#define  JKC__ESC        0x82
#define  JKC__TYPEGEN    0
#define  JKC__TYPE0      0x40
#define  JKC__TYPEF      0x80
#define  JKC__TYPEESC    0xFF
#define  JKWESC__TYPEESC 0xFF82


extern UINT UnpackPixels( LPBYTE, UINT, LPBYTE, WORD, BYTE, UINT );


//  more lvb junk
void   MaskLVBToScreen( HDC, BOOL );
extern  RECT  vRectLVB;
extern  ULONG vcxLVB;
extern  ULONG vcyLVB;
extern  BOOL  vfLVB;

//
//  DIM support stuff
//
extern PVD vpVd;

LPBYTE lpTWDIMCacheRead( ULONG sigH, ULONG sigL, LPUINT pbytecount, UINT section );
VOID   finishedTWDIMCacheRead( VOID );
LPBYTE lpTWDIMCacheWrite( ULONG sigH, ULONG sigL, UINT section );
BOOL   finishedTWDIMCacheWrite( UINT );
BOOL   fTWDIMCacheRemove( ULONG sigH, ULONG sigL );
int    twDIMCacheWrite( PVD pvd, CACHE_FILE_HANDLE fh  );
int    twDIMCacheError( PVD pvd, CACHE_FILE_HANDLE fh  );


//NOTE: This routine has been ported from assembler with the logic intact

UINT   RleDecompress(UINT rlecount, LPBYTE lpinput, LPBYTE lpoutput)
{
   INT   datacount = 0;  //has amount of expanded data

   INPUT_WORD  in;

   BOOL  notfinished;
   BYTE  type;
   WORD  count;
   BYTE  datarepeat;

   TRACE((TC_TW,TT_TW_RLESTATS,"TW: rle input count=%u, ",rlecount));

   while (rlecount >= 2) {
      //when in this loop there is an assumption that there are at least
      //2 more bytes that need to be parsed

      ASSERT(((INT) rlecount) >= 0,0);
      notfinished = TRUE;     //need because of common processing

      *((LPWORD) &in) = read_word(lpinput);
      lpinput += 2;
      rlecount -= 2;

      if (in.lowbyte != JKC__ESC) {
         //first byte is not the esc char

         if (in.hibyte != JKC__ESC) {
            //neither bytes are the esc char
            write_word(lpoutput, *((LPWORD) &in));
            lpoutput += 2;
            datacount += 2;
            notfinished = FALSE;    //finished, go to common end point
         }
         else {
            //the second byte was the esc char
            //so save the first byte and arrange things so
            //equivalent situation for common esc processing
            *(lpoutput++) = (BYTE) in.lowbyte;
            datacount++;

            in.hibyte = *(lpinput++);
            rlecount--;
            //notfinished is still TRUE
         }
      }

      //an else plus sometimes need to come through here even if
      //did previous if
      if (notfinished == TRUE) {
         //hibyte has the TYPE byte
         //first check to see if its the 2 byte sequence that generates
         //one byte of the rle ESC char
         if (in.hibyte == JKC__TYPEESC) {
            *(lpoutput++) = JKC__ESC;
            datacount++;
         }
         else {
            //continue rle esc processing
            type = ((BYTE) in.hibyte) & 0xc0;

            if (type != 0) {
               //this is one of the two special repeat byte cases
               if (type == JKC__TYPE0) {
                  datarepeat = 0;         //lowbyte will have data byte
               }
               else datarepeat = 0xff;

               //now do count processing for special repeat case

               if (((BYTE) in.hibyte) & 0x20 ) {
                  //its the big count case
                  in.hibyte &= 0x1f;
                  in.lowbyte = *(lpinput++);
                  rlecount--;
                  count = (WORD) *((LPWORD) &in);
               }
               else count = (WORD) (((BYTE) in.hibyte) & 0x1f) + 2;    //small count case
            }
            else {
               //its the general esc case
               if (((BYTE) in.hibyte) & 0x20 ) {
                  //its the big count case
                  in.hibyte &= 0x1f;
                  in.lowbyte = *(lpinput++);
                  datarepeat = *(lpinput++);
                  rlecount -= 2;
                  count = (WORD) *((LPWORD) &in);
               }
               else {
                  //its the small count case
                  count = (WORD) (((BYTE) in.hibyte) & 0x1f) + 2;
                  datarepeat = *(lpinput++);
                  rlecount--;
               }
            }

            //now move the character into the output buffer

            datacount += count;

            //jkbetter - use c run time when we understand
            //the memory model considerations?
            //while (count > 0) {
            //   *(lpoutput++) = datarepeat;
            //   count--;
            //}
            memset(lpoutput,datarepeat,count);
            lpoutput += count;

         }
      }

   }

   //when fall through there are either no or 1 input byte left to process
   if (rlecount == 1) {
      *lpoutput = *lpinput;
      datacount++;
   }

   TRACE((TC_TW,TT_TW_RLESTATS,"TW: output count=%u, ",datacount));

   return(datacount);

}


///////////////////////////////////////////////////////////////////////
//
// Does the same thing as RleDecompress and MakeInvertedDibFormat combined
// This avoids having to decompress the data into a temporary buffer and then
// move it again.  We need to know how many scanlines we expect the section to be
// before we start doing the decompress because the first scanline is placed at the
// higher address which is computed from the expected number of scanlines.
// This forces the caller to believe that the host will always send the highest number
// of scanlines that it thinks it knows how to send in a section.


UINT  RleInvertedDecompress(UINT rlecount, LPBYTE lpinput, LPBYTE lpoutput,
                           UINT input_scanline_bytes, UINT output_scanline_bytes,
                           UINT count_scanlines)
{
   INT   datacount = 0;  //has amount of expanded data

   INPUT_WORD  in;

   BOOL  notfinished;
   BYTE  type;
   WORD  count;
   BYTE  datarepeat;
   UINT  delta;

   LPBYTE lpcurrentoutputline, lpcurrentoutput;
   INT   bytes_left;

   TRACE((TC_TW,TT_TW_RLESTATS,"TW: rle input count=%u, ",rlecount));

   lpcurrentoutputline = lpoutput + ((count_scanlines - 1) * output_scanline_bytes);
   lpcurrentoutput = lpcurrentoutputline;
   bytes_left = input_scanline_bytes;       //when hits 0 need to reset to this value
                                             //after moving everything up one scanline
   delta = output_scanline_bytes - input_scanline_bytes;

   while (rlecount >= 2) {
      //when in this loop there is an assumption that there are at least
      //2 more bytes that need to be parsed

      ASSERT(((INT) rlecount) >= 0,0);
      notfinished = TRUE;     //need because of common processing

      *((LPWORD) &in) = read_word(lpinput);
      lpinput += 2;

      rlecount -= 2;

      if (in.lowbyte != JKC__ESC) {
         //first byte is not the esc char

         if (in.hibyte != JKC__ESC) {
            //neither bytes are the esc char
            if (bytes_left >= 2) {
	       write_word(lpcurrentoutput, *((LPWORD) &in));
               lpcurrentoutput += 2;

               datacount += 2;
               bytes_left -= 2;
            }
            else {
               //bytes left must be 1 and we have 2 bytes to output
               *lpcurrentoutput = (BYTE) in.lowbyte;
               lpcurrentoutputline -= output_scanline_bytes;
               lpcurrentoutput = lpcurrentoutputline;
               bytes_left = input_scanline_bytes - 1;
               *(lpcurrentoutput++) = (BYTE) in.hibyte;
               datacount += (2 + delta);
            }
            notfinished = FALSE;    //finished, go to common end point
         }
         else {
            //the second byte was the esc char
            //so save the first byte and arrange things so
            //equivalent situation for common esc processing
            *(lpcurrentoutput++) = (BYTE) in.lowbyte;
            bytes_left--;
            datacount++;

            in.hibyte = *(lpinput++);
            rlecount--;
            //notfinished is still TRUE
         }
         if (bytes_left == 0) {
            //need to reset all the output pointers to the next output scanline
            lpcurrentoutputline -= output_scanline_bytes;
            lpcurrentoutput = lpcurrentoutputline;
            bytes_left = input_scanline_bytes;
            datacount += delta;
         }
      }

      //an else plus sometimes need to come through here even if
      //did previous if
      if (notfinished == TRUE) {
         //hibyte has the TYPE byte
         //first check to see if its the 2 byte sequence that generates
         //one byte of the rle ESC char
         if (in.hibyte == JKC__TYPEESC) {
            *(lpcurrentoutput++) = JKC__ESC;
            datacount++;
            bytes_left--;
         }
         else {
            //continue rle esc processing
            type = ((BYTE) in.hibyte) & 0xc0;

            if (type != 0) {
               //this is one of the two special repeat byte cases
               if (type == JKC__TYPE0) {
                  datarepeat = 0;         //lowbyte will have data byte
               }
               else datarepeat = 0xff;

               //now do count processing for special repeat case

               if (((BYTE) in.hibyte) & 0x20 ) {
                  //its the big count case
                  in.hibyte &= 0x1f;
                  in.lowbyte = *(lpinput++);
                  rlecount--;
                  count = (WORD) *((LPWORD) &in);
               }
               else count = (WORD) (((BYTE) in.hibyte) & 0x1f) + 2;    //small count case
            }
            else {
               //its the general esc case
               if (((BYTE) in.hibyte) & 0x20 ) {
                  //its the big count case
                  in.hibyte &= 0x1f;
                  in.lowbyte = *(lpinput++);
                  datarepeat = *(lpinput++);
                  rlecount -= 2;
                  count = (WORD) *((LPWORD) &in);
               }
               else {
                  //its the small count case
                  count = (WORD) (((BYTE) in.hibyte) & 0x1f) + 2;
                  datarepeat = *(lpinput++);
                  rlecount--;
               }
            }

            //now move the character into the output buffer

            datacount += count;

            ////jkbetter - use c run time when we understand
            ////the memory model considerations?
            ////while (count > 0) {
            ////   *(lpoutput++) = datarepeat;
            ////   count--;
            ////}
            //memset(lpoutput,datarepeat,count);
            //lpoutput += count;
            while (count > 0) {
               if (bytes_left >= (INT) count) {
                  memset(lpcurrentoutput,datarepeat,count);
                  lpcurrentoutput += count;
                  bytes_left -= count;
                  count = 0;
               }
               else {
                  //need to duplicate more than current scanline allows
                  memset(lpcurrentoutput,datarepeat,bytes_left);
                  count -= bytes_left;
                  lpcurrentoutputline -= output_scanline_bytes;
                  lpcurrentoutput = lpcurrentoutputline;
                  bytes_left = input_scanline_bytes;
                  datacount += delta;
               }

            }

         }
      }
      if (bytes_left == 0) {
         //need to reset all the output pointers to the next output scanline
         lpcurrentoutputline -= output_scanline_bytes;
         lpcurrentoutput = lpcurrentoutputline;
         bytes_left = input_scanline_bytes;
         datacount += delta;
      }

   }

   //when fall through there are either no or 1 input byte left to process
   if (rlecount == 1) {
      *lpcurrentoutput = *lpinput;
      datacount += 1 + delta;
      ASSERT(bytes_left == 1,0);
   }
   else
   {
       ASSERT(bytes_left == (INT) input_scanline_bytes,0);
       TRACE((TC_TW,TT_TW_RLESTATS,"TW: bytes_left %d input_scanline_bytes %d rlecount %d",bytes_left, input_scanline_bytes, rlecount));
   }

   TRACE((TC_TW,TT_TW_RLESTATS,"TW: output count including dib_bytes_wide expansion=%u, ",datacount));

   return(datacount);

}

//////////////////////////////////////////////////////////////////
//
// PaletteMap supports the palette map protocol
// It eats all the protocol relating to the palette map
// Should not be called if the palette is trivial unless the first
// byte read in will be a 1, in which case the trivial palette will be identified
//
// The word palette area of the global bitmapinfo structure is setup
//    with the correct palette to use
//
// output:  *amountPaletteMapCache - 0 if trivial or if nothing to cache
//                                   # - number of palette entries that need to cache
//                                     after the bitmap operation is complete
//          *handlePaletteMapCache - the _512B cache handle to use when caching the palette
//
// returns 0 if should use the identity (trivial palette) because the first
//          byte read is 1
//         1 if we have a palette map situation
// after doing the bitmap operation, amountPaletteMapCache should be used to determine
// whether PaletteMapCache should be called.

WORD  PaletteMap(LPWORD lpamountPaletteMapCache, LPWORD lphandlePaletteMapCache)
{
   BYTE  byte1;
   LPBYTE lpcache;
   WORD  handle;
   WORD  count;      //0 based 1 value
   UINT  cached;     //0 - not cached or to be cached
                     //1 - to be cached but not currently cached
                     //2 - currently cached
   UINT  cache_bytecount;
   UINT  i;


   GetNextTWCmdBytes((LPBYTE) &byte1,1);

   if (byte1 == 1) {
      *lpamountPaletteMapCache = 0;     //don't need to cache anything
      return(0);
   }

   cached = byte1 >> 6;

   if (cached) {
      //setup the handle
      handle = (byte1 & 0x0f) << 8;
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      handle |= (WORD) byte1;

      //if already cached then get out of the cache and return
      if (cached == 2) {
         lpcache = lpTWCacheRead((UINT) handle, _512B,
                                 (LPUINT) &cache_bytecount, 0);
         count = read_word(lpcache);
         ASSERT(!(count & 0xff00),0);  //1 byte per entry and reserved fields
         count++;    //1 base the count
         lpcache += 2;
         for (i=0; i < count ; i++) {
            bitmapinfo_8BPP_PALETTE_MAP.bmiColors[i] = (WORD) (*(lpcache + i));
         }
         *lpamountPaletteMapCache = 0;     //don't need to cache anything
         return(1);
      }
   }

   //if fall through here then not already cached
   //need to get the number of entries

   GetNextTWCmdBytes((LPBYTE) &byte1,1);
   count = ((WORD) byte1) + 1;   //add 1 for 1 based count

   for (i=0; i < count ; i++) {
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      bitmapinfo_8BPP_PALETTE_MAP.bmiColors[i] = (WORD) byte1;
   }
   if (cached) {
      //must be 1 which means cache later on
      *lpamountPaletteMapCache = count;     //cached count, 1 based value
      *lphandlePaletteMapCache = handle;
   }
   else *lpamountPaletteMapCache = 0;     //don't need to cache anything
   return(1);
}

//////////////////////////////////////////////////////////////////////
//
// PaletteMapCache is used to cache the palette that was received previously
//
void  PaletteMapCache(WORD amountPaletteMapCache, WORD handlePaletteMapCache)
{
   UINT  i;
   LPBYTE lpcache;
   BOOL  jretcode;

   lpcache = lpTWCacheWrite((UINT) handlePaletteMapCache, _512B,
                  0, (UINT) handlePaletteMapCache);   //do size later!

   write_word(lpcache, amountPaletteMapCache - 1);
   lpcache += 2;
   for (i=0; i < amountPaletteMapCache ; i++) {
      *(lpcache + i) = (BYTE) bitmapinfo_8BPP_PALETTE_MAP.bmiColors[i];
   }

   jretcode = finishedTWCacheWrite(amountPaletteMapCache + 2);
   ASSERT(jretcode,0);
}


//////////////////////////////////////////////////////////////
//cmplxclpenum enumerates the complex clipping protocol subpacket one
//rectangle at a time.  Each time it gets called it returns another
//enumerated rectangle to the caller in returnclipulh

//The subroutine keeps internal state accross calls in lastclip and
//lastscanclip.
//they do not need to be initialized on the first call

//The expected enumeration case is in case on entry and the next
//enumeration case is returned in case on return.

//the first time the routine is called case should be set to RECTFULL

//this routine cannot fail, but it can go off into the weeds

void cmplxCLPenum(LPINT lpcase, LPRECT_ULH lpreturnclpulh)
{
   INPUT_WORD  inw1;
   WORD        word1,word2;
   BYTE        byte1;

   TRACE((TC_TW,TT_TW_CLIPCMPLX,"TW: clipcmplx case %u, ",(UINT) *lpcase));

   switch(*lpcase)

   {
   case RECTFULL:
      GetNextTWCmdBytes((LPWORD) &inw1,2);
      *lpcase = ((BYTE) inw1.hibyte) >> 5;

      lpreturnclpulh->left = lastclip.left = lastscanclip.left =
                           (((WORD) *((LPWORD) &inw1) ) >> 2) & 0x07ff;
      *((LPWORD) &inw1) = (((WORD) *((LPWORD) &inw1) ) << 8) & 0x0300;
      GetNextTWCmdBytes((LPBYTE) &inw1,1);
      lpreturnclpulh->top = lastclip.top = lastscanclip.top =
                                                      *((LPWORD) &inw1);

      Input3Bytes((LPWORD) &word1, (LPWORD) &word2);

      word1++;       //width
      word2++;       //height

      lpreturnclpulh->width = word1;
      lpreturnclpulh->height = word2;

      lastclip.bottom = lastscanclip.bottom = lastclip.top + word2;

      lastclip.right = lastscanclip.right = lastclip.left + word1;

      TRACE((TC_TW,TT_TW_CLIPCMPLX,"TW: left=%u, top=%u, width=%u, height=%u\n",
            lpreturnclpulh->left, lpreturnclpulh->top,
            lpreturnclpulh->width, lpreturnclpulh->height));

      return;

   case NEXTTINY:
      //the height is 1 and the top = previous bottom

      lpreturnclpulh->top = lastclip.top = lastscanclip.top =
                                           lastscanclip.bottom;
      lpreturnclpulh->height = 1;
      lastscanclip.bottom++;
      lastclip.bottom = lastscanclip.bottom;

      //7,6,5 next case
      //4,3,2 signed x1 delta
      //1,0 signed x2 delta

      GetNextTWCmdBytes((LPBYTE) &byte1,1);

      *lpcase = byte1 >> 5;

      word1 = (byte1 >> 2) & 0x07;     //signed x1 delta

      if (word1 & 0x0004) {
         //its a negative number - take 2's complement and add 1 to get positive
         //then subtract from ...
         //we do it this way instead of assembler code way in order
         //to avoid understanding the exact length of the variable
         word1 = (word1 ^ 0x0007) + 1;
         lastscanclip.left -= word1;
      }
      else lastscanclip.left += word1;

      lpreturnclpulh->left = lastclip.left = lastscanclip.left;

      word1 = byte1 & 0x03;            //signed x2 delta

      if (word1 & 0x0002) {
         //its a negative number
         word1 = (word1 ^ 0x0003) + 1;
         lastscanclip.right -= word1;
      }
      else lastscanclip.right += word1;
      lastclip.right = lastscanclip.right;
      lpreturnclpulh->width = lastclip.right - lastclip.left;

      TRACE((TC_TW,TT_TW_CLIPCMPLX,"TW: left=%u, top=%u, width=%u, height=%u\n",
            lpreturnclpulh->left, lpreturnclpulh->top,
            lpreturnclpulh->width, lpreturnclpulh->height));

      return;

   case NEXTMEDIUM1:
      //when get word
      //15,14,13 are the next case flags
      //12,11,10 are height 0 base 1
      //9,8,7,6,5 are signed delta x1
      //4,3,2,1,0 are signed delta x2
      //top is previous bottom

      lpreturnclpulh->top = lastclip.top = lastscanclip.top =
                                           lastscanclip.bottom;

      GetNextTWCmdBytes((LPWORD) &word1,2);

      *lpcase = word1 >> 13;

      //do the height
      lpreturnclpulh->height = ((word1 >> 10) & 7) + 1;
      lastclip.bottom = lastscanclip.bottom = lastclip.top +
                                              lpreturnclpulh->height;

      //do delta x1
      word2 = (word1 >> 5) & 0x001f;
      if (word2 & 0x0010) {
         //its a negative number
         word2 = (word2 ^ 0x001f) + 1;
         lastscanclip.left -= word2;
      }
      else lastscanclip.left += word2;
      lpreturnclpulh->left = lastclip.left = lastscanclip.left;

      //do delta x2
      word1 &= 0x001f;
      if (word1 & 0x0010) {
         //its a negative number
         word1 = (word1 ^ 0x001f) + 1;
         lastscanclip.right -= word1;
      }
      else lastscanclip.right += word1;
      lastclip.right = lastscanclip.right;
      lpreturnclpulh->width = lastclip.right - lastclip.left;

      TRACE((TC_TW,TT_TW_CLIPCMPLX,"TW: left=%u, top=%u, width=%u, height=%u\n",
            lpreturnclpulh->left, lpreturnclpulh->top,
            lpreturnclpulh->width, lpreturnclpulh->height));

      return;

   case NEXTMEDIUM2:
      //when get word
      //15,14,13 are the next case flags
      //12 - 6 are height 0 based 1
      //5,4,3 are signed delta x1
      //2,1,0 are signed delta x2
      //top is previous bottom

      lpreturnclpulh->top = lastclip.top = lastscanclip.top =
                                           lastscanclip.bottom;

      GetNextTWCmdBytes((LPWORD) &word1,2);

      *lpcase = word1 >> 13;

      //do the height
      lpreturnclpulh->height = ((word1 >> 6) & 0x007f) + 1;
      lastclip.bottom = lastscanclip.bottom = lastclip.top + lpreturnclpulh->height;

      //do delta x1
      word2 = (word1 >> 3) & 0x0007;
      if (word2 & 0x0004) {
         //its a negative number
         word2 = (word2 ^ 0x0007) + 1;
         lastscanclip.left -= word2;
      }
      else lastscanclip.left += word2;
      lpreturnclpulh->left = lastclip.left = lastscanclip.left;

      //do delta x2
      word1 &= 0x0007;
      if (word1 & 0x0004) {
         //its a negative number
         word1 = (word1 ^ 0x0007) + 1;
         lastscanclip.right -= word1;
      }
      else lastscanclip.right += word1;
      lastclip.right = lastscanclip.right;
      lpreturnclpulh->width = lastclip.right - lastclip.left;

      TRACE((TC_TW,TT_TW_CLIPCMPLX,"TW: left=%u, top=%u, width=%u, height=%u\n",
            lpreturnclpulh->left, lpreturnclpulh->top,
            lpreturnclpulh->width, lpreturnclpulh->height));

      return;

   case NEXTHUGE:
      //when get word
      //15,14,13  next case flags
      //12 - 4 are height 0 based 1
      //3,2,1,0 + next byte 7,6 is signed delta x1
      //next byte 5,4,3,2,1,0 are signed delta x2
      //top is previous bottom

      lpreturnclpulh->top = lastclip.top = lastscanclip.top =
                                           lastscanclip.bottom;

      GetNextTWCmdBytes((LPWORD) &word1,2);

      *lpcase = word1 >> 13;

      //do the height
      lpreturnclpulh->height = ((word1 >> 4) & 0x01ff) + 1;
      lastclip.bottom = lastscanclip.bottom = lastclip.top +
                                              lpreturnclpulh->height;

      //do delta x1
      GetNextTWCmdBytes((LPBYTE) &byte1,1);

      word2 = byte1;
      word2 = (word2 >> 6) & 3;
      word1 = ((word1 << 2) | word2) & 0x003f;
      if (word1 & 0x0020) {
         //its a negative number
         word1 = (word1 ^ 0x003f) + 1;
         lastscanclip.left -= word1;
      }
      else lastscanclip.left += word1;
      lpreturnclpulh->left = lastclip.left = lastscanclip.left;

      //do delta x2
      word1 = byte1 & 0x3f;
      if (word1 & 0x0020) {
         //its a negative number
         word1 = (word1 ^ 0x003f) + 1;
         lastscanclip.right -= word1;
      }
      else lastscanclip.right += word1;
      lastclip.right = lastscanclip.right;
      lpreturnclpulh->width = lastclip.right - lastclip.left;

      TRACE((TC_TW,TT_TW_CLIPCMPLX,"TW: left=%u, top=%u, width=%u, height=%u\n",
            lpreturnclpulh->left, lpreturnclpulh->top,
            lpreturnclpulh->width, lpreturnclpulh->height));

      return;

   case SAMEMEDIUM:
      //do not change lastscanclip
      //when get word
      //15,14,13 are the next case flags
      //12,11,10,9,8,7 are the 0 based 1 positive delta for new left
      //                                  from the previous right
      //6,5,4,3,2,1,0 are the 0 base 1 value for the width

      //top and bottom are the same as lastclip
      lpreturnclpulh->top = lastclip.top;
      lpreturnclpulh->height = lastclip.bottom - lastclip.top;

      GetNextTWCmdBytes((LPWORD) &word1,2);

      *lpcase = word1 >> 13;

      word2 = word1;

      //do position delta for new left
      word2 = ((word2 >> 7) & 0x003f) + 1;
      lpreturnclpulh->left = lastclip.left = lastclip.right + word2;

      //do width
      word1 = (word1 & 0x007f) + 1;
      lpreturnclpulh->width = word1;
      lastclip.right = lastclip.left + word1;

      TRACE((TC_TW,TT_TW_CLIPCMPLX,"TW: left=%u, top=%u, width=%u, height=%u\n",
            lpreturnclpulh->left, lpreturnclpulh->top,
            lpreturnclpulh->width, lpreturnclpulh->height));

      return;

   case SAMEHUGE:
      //do not change lastscanclip
      //when get word
      //12,11,10,9,8,7,6,5,4,3 are the 0 based 1 positive delta for new left
      //                                  from the previous right
      //2,1,0 plus next byte are the 0 base 1 value for the width

      //top and bottom are the same as lastclip
      lpreturnclpulh->top = lastclip.top;
      lpreturnclpulh->height = lastclip.bottom - lastclip.top;

      GetNextTWCmdBytes((LPWORD) &word1,2);

      *lpcase = word1 >> 13;

      word2 = word1;

      //do position delta for new left
      word2 = ((word2 >> 3) & 0x03ff) + 1;
      lpreturnclpulh->left = lastclip.left = lastclip.right + word2;

      //do width
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      word1 = (((word1 & 0x0007) << 8) + (WORD) byte1) + 1;
      lpreturnclpulh->width = word1;
      lastclip.right = lastclip.left + word1;

      TRACE((TC_TW,TT_TW_CLIPCMPLX,"TW: left=%u, top=%u, width=%u, height=%u\n",
            lpreturnclpulh->left, lpreturnclpulh->top,
            lpreturnclpulh->width, lpreturnclpulh->height));

      return;

   default:
      ASSERT(0,0);

   }
   ASSERT(0,0);

}


//cmplxRgn uses cmplxCLPenum to build up a region which is creates
//and returns the handle of.  It is the responsibility of the caller
//to delete the returned region object.
//
//        RECT_ULH and case.

HRGN  cmplxRgn()
{
   HRGN  hrgnSum;
   HRGN  hrgnTemp;

   RECT_ULH next_clip;
   int   icase = RECTFULL;
   int   retcode;
   BOOL  bretcode;

   cmplxCLPenum((LPINT) &icase, (LPRECT_ULH) &next_clip);

   hrgnSum = CreateRectRgn(next_clip.left, next_clip.top,
                           next_clip.left + next_clip.width,
                           next_clip.top + next_clip.height);
   ASSERT(hrgnSum != NULL,0);

   while (icase != RECTLAST) {
      cmplxCLPenum((LPINT) &icase, (LPRECT_ULH) &next_clip);

      hrgnTemp = CreateRectRgn(next_clip.left, next_clip.top,
                              next_clip.left + next_clip.width,
                              next_clip.top + next_clip.height);

      ASSERT(hrgnTemp != NULL,0);

      retcode = CombineRgn(hrgnSum,hrgnSum,hrgnTemp,RGN_OR);

//    ASSERT((retcode != ERROR) && (retcode != NULLREGION),0);

      bretcode = DeleteObject(hrgnTemp);

      ASSERT(bretcode,0);

   }

   return(hrgnSum);

}


//deltaenum is the sister routine to the assember routine noclpdeltaenum
//It is passed the dsDELTA case in case
//updates *lpdelta based on datastream it reads. necessary datastream is eaten

void  DeltaEnum(UINT deltacase,LPDSDELTA lpdelta)
{
   UINT uint1;
   WORD word1,word2;
   BYTE byte1;

   switch (deltacase) {
   case DELTASAME:
      return;

   case DELTATINYX:
      lpdelta->deltax_last = 1;
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      lpdelta->deltay_last = ((UINT) byte1) + 1;
      return;

   case DELTATINYY:
      lpdelta->deltay_last = 1;
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      lpdelta->deltax_last = ((UINT) byte1) + 1;
      return;

   case DELTAMEDX:
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      uint1 = (UINT) byte1;
      lpdelta->deltax_last = (uint1 >> 6) + 1;
      lpdelta->deltay_last = (uint1 & 0x3f) + 1;
      return;

   case DELTAMEDY:
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      uint1 = (UINT) byte1;
      lpdelta->deltay_last = (uint1 >> 6) + 1;
      lpdelta->deltax_last = (uint1 & 0x3f) + 1;
      return;

   case DELTALRGEQ:
      GetNextTWCmdBytes((LPWORD) &word1,2);
      uint1 = (UINT) word1;
      lpdelta->deltax_last = (uint1 & 0x00ff) + 1;
      lpdelta->deltay_last = (uint1 >> 8) + 1;
      return;

   case DELTALARGE:
      GetNextTWCmdBytes((LPWORD) &word1,2);
      uint1 = (UINT) word1;
      lpdelta->deltax_last = (uint1 & 0x01ff) + 1;
      lpdelta->deltay_last = (uint1 >> 9) + 1;
      return;

   case DELTAHUGE:
      Input3Bytes((LPWORD) &word1, (LPWORD) &word2);
      lpdelta->deltax_last = word1 + 1;
      lpdelta->deltay_last = word2 + 1;
      return;

   default:
      ASSERT(0,0);

   }

   ASSERT(0,0);

}



//this routine handles the bitblt_nosrc_rop3_noclip packet
//corresponds to the blt_nosrc_rop3_noclip assembler routine
//
//input parm is the device context to use
//device context has the last realized brush
//returns TRUE is no errors
//
//sucks up whatever protocol it needs
/****************************************************************************\
 *  TWCmdBitBltNoSrcROP3NoClip (BITBLT_NOSRC_ROP3_NOCLIP service routine)
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
void TWCmdBitBltNoSrcROP3NoClip( HWND hWnd, HDC device )
{

   DWORD rop3;
   DWORD dword1;
   BYTE byte1, byte2;
   UINT deltacase;
   int  xleft, yleft;
   BOOL jretcode;

   GetNextTWCmdBytes((LPBYTE) &byte1,1);
   byte2 = byte1;

   rop3 = ((DWORD) byte1) << 16;

   rop3 = rop3 | ((DWORD) stupid_rop_info[byte1]);

   TRACE((TC_TW,TT_TW_PAINT+TT_TW_ENTRY_EXIT,"TW: TWCmdBitBltNoSrcROP3NoClip: entered, (UINT) rop3=%u\n", (UINT) byte1));

   byte1 &= 0x0f;
   byte2 = (byte2 & 0xf0) >> 4;

   if (byte1 != byte2) {
      if (!create_brush(hWnd,device)) {
         TRACE((TC_TW,TT_TW_PAINT+TT_TW_ENTRY_EXIT,"TW: TWCmdBitBltNoSrcROP3NoClip: FALSE EXIT \n"));
         TWCmdReturn(FALSE);
      }
   }

   Input3BytesLong((LPDWORD) &dword1);

   deltacase = (UINT) (dword1 >> 21);

   xleft = (UINT) ((dword1 >> 10) & 0x000007ff);
   yleft = (UINT) (dword1 & 0x000003ff);

   TRACE((TC_TW,TT_TW_PAINT,"TW: deltacase=%u\n", deltacase));

   DeltaEnum(deltacase, (LPDSDELTA) &nosrcblt_dsdelta);

   TRACE((TC_TW,TT_TW_PAINT,"TW:       xleft=%u, yleft=%u, width=%u, height=%u\n",
            xleft, yleft, nosrcblt_dsdelta.deltax_last,
            nosrcblt_dsdelta.deltay_last));

   jretcode = PatBlt(device, xleft, yleft, nosrcblt_dsdelta.deltax_last,
                                       nosrcblt_dsdelta.deltay_last,rop3);

   TRACE((TC_TW,TT_TW_PAINT+TT_TW_ENTRY_EXIT,"TW: TWCmdBitBltNoSrcROP3NoClip: EXIT jretcode=%u\n", (UINT) jretcode));

   TWCmdReturn(jretcode);
}


//this routine handles the bitblt_nosrc_rop3_cmplxclip packet
//corresponds to the blt_nosrc_rop3_cmplsclip routine
//
//input parm is the device context to use
//device context has the last realized brush
//returns TRUE if no errors
//
//sucks up whatever protocol it needs

//jkbetter - there is a chance that for Windows 3.1
//    calling patblt for every rectangle would be more efficient then
//    building up the region and then calling PaintRgn once.
//    Should also probably verify that this is fastest method for windows 95
//
//jktest - make sure to test disjoint regions to make sure
//          the fill mode is working as expected
/****************************************************************************\
 *  TWCmdBitBltNoSrcROP3CmplxClip (BITBLT_NOSRC_ROP3_CMPLXCLIP service routine)
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
void TWCmdBitBltNoSrcROP3CmplxClip( HWND hWnd, HDC device )
{

   //the if 0 is the code that uses a complex region
   //the else code is the new code to -----
   //test to check out performance of doing each rectangle one at a time instead
   //of doing the region thing
   //we found that on windows 3.1 the complex fill tests sped up
   //by 2.5 to over 10 times faster!!!!!!!!!!!
#if 0
   int   rop2,retcode;
   BOOL  bretcode2;
   HRGN  region;
#else
   DWORD rop3;
   RECT_ULH next_clip;
   int   icase = RECTFULL;
#endif
   BYTE byte1, byte2;
   BOOL  bretcode1;

   GetNextTWCmdBytes((LPBYTE) &byte1,1);
   byte2 = byte1;

#if 0
   rop2 = Rop3ToRop2[byte1];
   TRACE((TC_TW,TT_TW_PAINT+TT_TW_ENTRY_EXIT,"TW: TWCmdBitBltNoSrcROP3CmplxClip: , rop2=%u\n", (UINT) rop2));
#else
   rop3 = ((DWORD) byte1) << 16;
   rop3 = rop3 | ((DWORD) stupid_rop_info[byte1]);

   TRACE((TC_TW,TT_TW_PAINT+TT_TW_ENTRY_EXIT,"TW: TWCmdBitBltNoSrcROP3CmplxClip: , rop3=%u\n", (UINT) rop3));
#endif


   byte1 &= 0x0f;
   byte2 = (byte2 & 0xf0) >> 4;

   if (byte1 != byte2) {
      if (!create_brush(hWnd,device)) {
         TRACE((TC_TW,TT_TW_PAINT+TT_TW_ENTRY_EXIT,"TW: TWCmdBitBltNoSrcROP3CmplxClip: FALSE EXIT \n"));
         TWCmdReturn(FALSE);
      }
   }

#if 0
   if (dcstate.rop2 != rop2) {
      dcstate.rop2 = rop2;
      retcode = SetROP2(device, rop2);

      ASSERT(retcode,0);
   }

   region = cmplxRgn();

   bretcode1 = PaintRgn(device, region);

   ASSERT(bretcode1,0);

   bretcode2 = DeleteObject(region);

   ASSERT(bretcode2,0);

   TWCmdReturn(bretcode1);
#else
   while (icase != RECTLAST) {

      cmplxCLPenum((LPINT) &icase, (LPRECT_ULH) &next_clip);

      TRACE((TC_TW,TT_TW_PAINT,"TW:       xleft=%u, yleft=%u, width=%u, height=%u\n",
            next_clip.left, next_clip.top, next_clip.width, next_clip.height));

      bretcode1 = PatBlt(device, next_clip.left, next_clip.top, next_clip.width,
                                       next_clip.height,rop3);
      ASSERT(bretcode1,0);
   }
   TRACE((TC_TW,TT_TW_PAINT+TT_TW_ENTRY_EXIT,"TW: TWCmdBitBltNoSrcROP3CmplxClip: EXIT \n"));
   TWCmdReturn(TRUE);
#endif

}

//this routine handles the bitblt_scrtoscr_rop3 packet
//corresponds to the blt_scrtoscr_rop3 routine
//
//input parm is the device context to use
//device context has the last realized brush
//returns TRUE if no errors
//
//sucks up whatever protocol it needs
//
//jkbetter - there is a chance that for Windows 3.1
//    calling bitblt for every rectangle would be more efficient then
//    building up the region and then calling Bitblt once.
//    Should also probably verify that this is fastest method for windows 95
//
/****************************************************************************\
 *  TWCmdBitBltScrToScrROP3 (BITBLT_SCRTOSCR_ROP3 service routine)
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
void TWCmdBitBltScrToScrROP3( HWND hWnd, HDC device )
{
   DWORD rop3, dword1;
   WORD  word1, word2;
   HRGN  region;
   BYTE  byte1, byte2;
   RECT  bounds, srcbounds;
   BOOL  bRetcode1,bRetcode2;
   UINT  deltacase;
   LPRECT lpoverlaprect;
   INT   coverlaprect;

   int   xDest, yDest, xSrc, ySrc;

   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_COPY, "TWCmdBitBltScrToScrROP3: entered" ));

   Input3Bytes((LPWORD) &word1, (LPWORD) &word2);

   xSrc = word1 & 0x07ff;
   ySrc = word2 & 0x07ff;

   if (word2 & 0x0800) {
      //its the optimized case of 0cc rop, no rop and no brush in datastream
      rop3 = 0x00cc0020;
      TRACE((TC_TW,TT_TW_COPY+TT_TW_ENTRY_EXIT,"TW: bitblt_scrtoscr_rop3, rop3=COPY\n"));
   }
   else {
      //its the general case with rop3 and possibly a brush
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      byte2 = byte1;

      TRACE((TC_TW,TT_TW_COPY,"TW: bitblt_scrtoscr_rop3, rop3=%u, ", (UINT) byte1));

      rop3 = ((DWORD) byte1) << 16;

      rop3 = rop3 | ((DWORD) stupid_rop_info[byte1]);

      byte1 &= 0x0f;
      byte2 = (byte2 & 0xf0) >> 4;

      if (byte1 != byte2) {
         if (!create_brush(hWnd,device)) {
            TRACE((TC_TW,TT_TW_COPY+TT_TW_ENTRY_EXIT,"TW: bitblt_scrtoscr_rop3 FALSE EXIT \n"));
            TWCmdReturn(FALSE);
         }
      }
   }

   TRACE((TC_TW,TT_TW_COPY,"TW:        xSrc=%u, ySrc=%u, ", xSrc, ySrc));

   //ready to deal with clipping or no clipping case
   if (word1 & 0x0800) {
      //complex clipping case

      Input3Bytes((LPWORD) &word1, (LPWORD) &word2);

      TRACE((TC_TW,TT_TW_COPY,"TW: xDest=%u, yDest=%u, CLIPPING\n",
                           (UINT) word1, (UINT) word2));

      region = cmplxRgn();

      GetRgnBox(region, (LPRECT) &bounds);

      SelectClipRgn(device,region);

      //bounds is the destination clipping rectangle
      //compute srcbounds which is the source clipping rectangle

      srcbounds.left = bounds.left + xSrc - (int) word1;
      srcbounds.top  = bounds.top  + ySrc - (int) word2;
      srcbounds.right = srcbounds.left + (bounds.right - bounds.left);
      srcbounds.bottom = srcbounds.top + (bounds.bottom - bounds.top);


      //jkfix - the complex clipping region is the destination
      //the source ulh can be computed from the following
      // 1) add original source coordinate to complex clip coordinate
      // 2) subtract original destination coordinate
      //the following code is wrong:
      //bRetcode1 = BitBlt(device, (int) word1, (int) word2,
      //               bounds.right - bounds.left, bounds.bottom - bounds.top,
      //               device, xSrc, ySrc, rop3);
      //the following code is correct
      bRetcode1 = BitBlt(device, (int) bounds.left, (int) bounds.top,
                     bounds.right - bounds.left, bounds.bottom - bounds.top,
                     device, (int) srcbounds.left,
                     (int) srcbounds.top, rop3);

      ASSERT(bRetcode1,0);

      SelectClipRgn(device,NULL);      //restore state of DC

      bRetcode2 = DeleteObject(region);

      ASSERT(bRetcode2,0);

      //fall through to common code where srcbounds is used to see if there are
      //any intersecting on top rectangles

   }

   else {   //its the no clipping case
      Input3BytesLong((LPDWORD) &dword1);

      deltacase = (UINT) (dword1 >> 21);

      xDest = (UINT) ((dword1 >> 10) & 0x000007ff);
      yDest = (UINT) (dword1 & 0x000003ff);

      TRACE((TC_TW,TT_TW_COPY,"TW: xDest=%u, yDest=%u, deltacase=%u\n",xDest,yDest,deltacase));

      DeltaEnum(deltacase, (LPDSDELTA) &scrtoscrblt_dsdelta);

      TRACE((TC_TW,TT_TW_COPY,"TW: width=%u, height=%u, NO CLIPPING\n",
                  (UINT) scrtoscrblt_dsdelta.deltax_last,
                  (UINT) scrtoscrblt_dsdelta.deltay_last));

      bRetcode1 = BitBlt(device, xDest, yDest, scrtoscrblt_dsdelta.deltax_last,
                    scrtoscrblt_dsdelta.deltay_last, device, xSrc, ySrc, rop3);

      srcbounds.left = xSrc;
      srcbounds.top  = ySrc;
      srcbounds.right = xSrc + scrtoscrblt_dsdelta.deltax_last;
      srcbounds.bottom = ySrc + scrtoscrblt_dsdelta.deltay_last;

   }

//we fall through to here with bRetcode1 having the return code for the operation
   //srcbounds has the source rectangle which is used to determine whether or not there
   //are any overlapping on top rectangles.  If there are then we need to request repaints.

   wfnEnumRects(hWnd, device, (LPRECT FAR *) &lpoverlaprect, (LPINT) &coverlaprect,
                  (LPRECT) &srcbounds);

   if (coverlaprect) {

      INT i;
      INT xDelta = xDest - xSrc;
      INT yDelta = yDest - ySrc;

      TRACE((TC_TW,TT_TW_COPY,"TW: special case of %u overlapped rectangles\n",
                           (UINT) coverlaprect));

      //adjust for blt
      for ( i=0; i<coverlaprect; i++ ) {
          (lpoverlaprect + i)->left   += xDelta;
          (lpoverlaprect + i)->top    += yDelta;
          (lpoverlaprect + i)->right  += xDelta;
          (lpoverlaprect + i)->bottom += yDelta;
      }

      wfnRepaintRects(lpoverlaprect,coverlaprect,TRUE);
      wfnFreeRects(lpoverlaprect);
   }
   
   TRACE((TC_TW,TT_TW_COPY+TT_TW_ENTRY_EXIT,"TW: bitblt_scrtoscr_rop3 EXIT bRetcode1=%u\n", (UINT) bRetcode1));

   TWCmdReturn(bRetcode1);

}

//MakeDibFormat coverts a dib format that does not have scanlines with length
//multiples of 4 to scanlines that do.  The current bytes per line and the
//desired bytes per line are input as parms

//it returns the amount of data output

//NOTE: IT IS ASSUMED THAT LPOUTPUT points to an address on a 4 byte boundary

UINT  MakeDibFormat(LPBYTE lpinput, LPBYTE lpoutput,
                    UINT input_scanline_bytes, UINT output_scanline_bytes,
                    UINT count_scanlines)
{
   UINT  count_zeros, count_output;

   count_output = 0;

   TRACE((TC_TW,TT_TW_BLT,"TW: MakeDibFormat input_scanline_bytes=%u, output_scanline_bytes=%u, ",
                           input_scanline_bytes, output_scanline_bytes));
   ASSERT(input_scanline_bytes < output_scanline_bytes,0);
   ASSERT((output_scanline_bytes - input_scanline_bytes) <= 3,0);

   count_zeros = output_scanline_bytes - input_scanline_bytes;

   count_output = count_scanlines * output_scanline_bytes;

   TRACE((TC_TW,TT_TW_BLT,"TW: count_output=%u\n", count_output));

   while (count_scanlines > 0) {
      memcpy(lpoutput, lpinput, input_scanline_bytes);
      lpinput += input_scanline_bytes;
      lpoutput = lpoutput + input_scanline_bytes + count_zeros;

      count_scanlines--;
   }

   return(count_output);
}

//MakeInvertedDibFormat works exactly like MakeDibFormat except the last scanline
//is put in the buffer first

UINT MakeInvertedDibFormat(LPBYTE lpinput, LPBYTE lpoutput,
                    UINT input_scanline_bytes, UINT output_scanline_bytes,
                    UINT count_scanlines)
{
   UINT  count_output;

   LPBYTE  lpcurrent;

   count_output = 0;

   TRACE((TC_TW,TT_TW_BLT,"TW: MakeInvertedDibFormat input_scanline_bytes=%u, output_scanline_bytes=%u, ",
                           input_scanline_bytes, output_scanline_bytes));

   count_output = count_scanlines * output_scanline_bytes;

   TRACE((TC_TW,TT_TW_BLT,"TW: count_output=%u\n", count_output));

   lpcurrent = lpoutput + ((count_scanlines-1) * output_scanline_bytes);

   while (count_scanlines > 0) {
      memcpy(lpcurrent, lpinput, input_scanline_bytes);
      lpinput += input_scanline_bytes;
      lpcurrent -= output_scanline_bytes;

      count_scanlines--;
   }

   return(count_output);
}



//GENERAL NOTE FOR ALL SOURCE BITBLT OPERATIONS
//HOST WILL ONLY GENERATE THE SIMPLECLIP AND CMPLXCLIP PACKETS
//IF THE BITMAP CAN BE CACHED
//SO WE CAN ASSUME THAT IF BITMAP NOT CACHEABLE THEN ITS THE NOCLIP CASE

//this routine is the common routine that supports all the bitblt_source
//packets.  Because we do not need to move the data to/from the cache
//we do not need to optimize for the one 2K slot size case.
//
//it sucks up all of the packet datastream including the complex clipping
//info in the complex clipping case
//
//for the noclip case we need to handle the possiblity that the bitmap
//may not be cached.  If the bitmap is cached then we we do not display
//the bitmap until the entire bitmap has come in and it is cached.
//that gives us the snappy performance

//for the simpleclip case the bitmap must be cached.  We do not display
//the bitmap until it is entirely cached for snappy performance reasons.

//for the complex clipping case the bitmap must be cached.  We do not
//display the bitmap until it is entirely cached for snappy performance
//reasons.  We also handle the complex clipping one rectangle at a time
//because that way we cut out all uneccessary data access

//The only time we should expect fbcolor information is for monochrome bitmaps

//input parm is the device context to use
//device context has the last realized brush
//returns TRUE if no errors
//
//sucks up whatever protocol it needs

BOOL  BltSrcCommon(HWND hWnd, HDC device, LPDSBITMAP_PARMS lpparms)
{
   int   xDestULH, yDestULH;
   UINT  i;
   WORD  word1, word2;
   DWORD rop3;
   BYTE  byte1, byte2;
   BOOL  jtocache;      //if TRUE then want to cache the incoming data
   BOOL  jcached;       //if TRUE then bitmap already cached
   BITMAP_HEADER  bm_header;  //used even for 256 color case
   UINT  bm_cache_special; // ONLY APPLIES IF 16 color bitmap 0 - normal or 2 color
                           // 1 - _512B and no control info cached
                           // 2 - _2K and no control info cached
   UINT  received_bytes_wide; //actual bytes per scanline received
                           //for 16 color different than bm_header.cache_bytes_wide
                           //for 2 color, the same as cache_bytes wide
   UINT  dib_bytes_wide;   //for 16 color: width of scanline needed for dib representation
                           //              same as cache_bytes_wide for 16 color
                           //for 2 color: width of scanline needed for
                           //             WORD boundary scanlines. not always
                           //             same as cache bytes wide
                           //jk256 - for 256 color same as 16 color

   WORD  cache_handle;     //if caching, the client object cache handle
   WORD  chain_handle;     //chain handle of current section

   DWORD total_cache_bytes; //number of bytes needed for caching
                            //including control information

   WORD  section_rle_bytecount;

   UINT  section_bytecount;      //count of rle expanded data

   UINT  section_count;       //which chain section in. 0 if first

   LPBYTE lpsection_bytes;         //pointer to rle expanded data

   LPBYTE lpsection_dib_bytes;     //pointer to dib format data

   UINT  section_scanlines_left;

   UINT  total_section_scanlines;  //for clipping case still need to keep track
                                  //of the total number of scanlines in the section

   UINT  start_scanline;

   INT   total_scanlines_left;      //must be INT not UINT

   WORD  size_array_1[4] = {0,3,2,1};

   LPBITMAPINFO   lpbitmapinfo;

   CHUNK_TYPE  parm_chunk_type;

   int   retcode;

   BOOL  jretcode;

   LPBYTE lpcache;

   UINT  cache_bytecount;

   BOOL  jcache_bmheader;     //TRUE is cache section has the 8 byte
                              //bitmap header

   UINT  first_section_scanlines;   //# scanlines in first section
   UINT  nth_section_scanlines;     //# scanlines in nth section
   UINT  last_section_scanlines;    //# scanlines in last section

   SRCBLT_CASE working_case;        //the case the code assumes

   RECT_ULH dest_rect,src_rect;              //for complex clipping
                     //because width & height the same we don't set the
                     //width and height of src_rect. use dest_rect
   INT      cmplx_case;             //  "

   UINT  finishedsize;     //size of last cache write for when calling finished cache

   HBITMAP  hbmold, hbmcurrent;

   WORD     use16_256palette;    //case of what 16 color palette to use in 256 color mode
                                 //0 - default
                                 //1 - newly transmitted palette and remember in last
                                 //2 - used last nondefault palette
   WORD     use256_256palette;   //case of what 256 color palette to use in 256 color mode
                                 //0 - use the trivial palette (identity)
                                 //1 - use the palette indicated by palette_map
                                 //    datastream
   WORD    amountPaletteMapCache;  //number of palette entries in palette object to cache
                                    //if 0 then no object to cache
   WORD    handlePaletteMapCache;  //the object handle to cache the palette data into

   UINT    temp_scanlines_left;
   UINT    num_bytes_to_unpack;
   WORD    section_flags;
   BYTE    bm_color;

   //   wkp: new stuff
   DWORD   signatureLO;
   DWORD   signatureHI;
   BOOL    jdim;

   working_case = lpparms -> packet_case;    //assume same as parms say

#ifdef DEBUG
   ASSERT(sizeof(bm_header) == 8,0);     //the protocol and host assume this
#endif

   //leave in to not destabilize 16 color mode, but should never be NULL
   ASSERT(compatDC != NULL,0);
   if (compatDC == NULL) {
      compatDC = CreateCompatibleDC(device);
   }

   Input3Bytes((LPWORD) &word1, (LPWORD) &word2);

   xDestULH = (UINT) word1;
   yDestULH = (UINT) (word2 & 0x03ff);

   //jk256 - different for 256 color mode
   if (word2 & 0x0800) {
      GetNextTWCmdBytes((LPBYTE) &last_fbColor,1);
      if (vColor == Color_Cap_256) {
         GetNextTWCmdBytes((LPBYTE) &last_backColor,1);
      }
   }
   if (!(word2 & 0x0400)) {
      //its the optimized case of 0cc rop, no rop and no brush in datastream
      rop3 = 0x00cc0020;
   }
   else {
      //jkstage256 - brushes
      //its the general case with rop3 and possibly a brush
      GetNextTWCmdBytes((LPBYTE) &byte1,1);
      byte2 = byte1;

      TRACE((TC_TW,TT_TW_ERROR,"special debug rop3 byte1=%08X\n",(DWORD) byte1));

      rop3 = ((DWORD) byte1) << 16;
      TRACE((TC_TW,TT_TW_ERROR,"special debug rop3 stage1 rop3=%08X\n",rop3));
      TRACE((TC_TW,TT_TW_ERROR,"special debug rop3 stupid_rop_info=%04X\n",stupid_rop_info[byte1]));

      rop3 = rop3 | ((DWORD) (stupid_rop_info[byte1]));

      TRACE((TC_TW,TT_TW_ERROR,"special debug rop3 stage2 rop3=%08X\n",rop3));

      byte1 &= 0x0f;
      byte2 = (byte2 & 0xf0) >> 4;

      if (byte1 != byte2) {
         if (!create_brush(hWnd,device)) {
            TRACE((TC_TW,TT_TW_BLT,"BltSrcCommon returning FALSE because of bad brush\n"));
            return(FALSE);
         }
      }

   }

   //rop3,xDestULH,yDestULH,last_fbColor,and brush in device are setup

   TRACE((TC_TW,TT_TW_BLT,"rop3=%08X\n",rop3));

more_data:

   //time to process dsBITMAP
   GetNextTWCmdBytes((LPBYTE) &byte1, 1);

   // DIM?
   if ( (byte1 & 0xc0) == 0xc0 ) {

      jdim     = TRUE;
      jtocache = !(byte1 & 0x30);
      jcached  = !!(byte1 & 0x10);

      //    validate signature is zero
      ASSERT( !(byte1 & 0x0f), byte1 );
     
      if ( jtocache ) {

          // no object handle so ignore hi-byte and stuff low byte
          GetNextTWCmdBytes((LPBYTE) &word1, 2);

          TRACE((TC_TW,TT_TW_BLT+TT_TW_DIM,"BltSrcCommon DIM add to cache (C-2)\n"));
      }
      else if ( jcached ) {

          GetNextTWCmdBytes((LPDWORD) &signatureLO, 4);
          GetNextTWCmdBytes((LPDWORD) &signatureHI, 4);

          TRACE((TC_TW,TT_TW_BLT+TT_TW_DIM,"BltSrcCommon DIM display from cache sig %8x%8x(C-1)\n", signatureHI, signatureLO));
      }
      else if ( (byte1 & 0x20) ) {

          GetNextTWCmdBytes((LPDWORD) &signatureLO, 4);
          GetNextTWCmdBytes((LPDWORD) &signatureHI, 4);

          (void) fTWDIMCacheRemove( signatureHI, signatureLO );
          
          TRACE((TC_TW,TT_TW_BLT+TT_TW_DIM,"BltSrcCommon DIM remove from cache sig %8x%8x(C-0)\n", signatureHI, signatureLO));

          goto more_data;
      }
      else ASSERT(1,0);
   }
   else {

      GetNextTWCmdBytes((LPWORD) &byte2, 1);
      jdim     = FALSE;
      word1    = (byte1 | (byte2<<8));
      jtocache = !!(word1 & 0x0040);
      jcached  = !!(word1 & 0x0080);

      TRACE((TC_TW,TT_TW_BLT+TT_TW_DIM,"BltSrcCommon not a DIM object\n"));
   }

   amountPaletteMapCache = 0;     //assume dont have a new palette object to cache

   if (!jcached) {

      //jk256
      //if 16 color bitmap and not already cached then indicates kind of palette to use
      use16_256palette = word1 & 0x0003;     //only use if 16 color bitmap

      //jk256
      //if 256 color bitmap and not already cached then indicates whether
      //to use palette_map datastream or use the trivial palette
      use256_256palette = word1 & 0x0001;


      //the bitmap is not already cached
      //2 cases, either cache bitmap and display
      //or just display the bitmap (don't cache it)
      TRACE((TC_TW, TT_TW_BLT,"TW: bitblt, bitmap not already cached\n"));
      word2 = word1;

      //jk256 - 256 color case
      if ((word1 & 0x0030) == 0x0020) {
         //256 color bitmap
         ASSERT(vColor == Color_Cap_256,0);
         bitmap_header_flags(bm_header, color) = 2;
         bm_header.pixel_offset = 0;
         //word1 will hold width, word2 will hold height
         word2 &= 0x0700;
         word1 = (word1 & 0x3800) >> 3;
         GetNextTWCmdBytes((LPBYTE) &word1,1);
         GetNextTWCmdBytes((LPBYTE) &word2,1);
         bm_header.pixel_width = word1 + 1;
         bm_header.total_scanlines = word2 + 1;
         bm_cache_special = 0;      //assume 0 case

         received_bytes_wide = bm_header.pixel_width;
         //calculate number of bytes per logical scanline in cache
         //and needed for dib representation (all lines length multiple of 4
         bm_header.cache_bytes_wide = received_bytes_wide +
                                      size_array_1[received_bytes_wide & 3];
         dib_bytes_wide = bm_header.cache_bytes_wide;

         if (use256_256palette == 1) {
             if (PaletteMap((LPWORD) &amountPaletteMapCache,(LPWORD) &handlePaletteMapCache) == 0) {
                ASSERT(0,0);
             }
         }
      }
      else if ((word1 & 0x0030) == 0x0010) {
         //16 color bitmap
         bitmap_header_flags(bm_header, color) = 1;
         bm_header.pixel_offset = (BYTE) ((word1 & 0x8000) >> 15);
         //word1 will hold width, word2 will hold height
         word2 &= 0x0700;
         word1 = (word1 & 0x7800) >> 3;
         GetNextTWCmdBytes((LPBYTE) &word1,1);
         GetNextTWCmdBytes((LPBYTE) &word2,1);
         bm_header.pixel_width = word1 + 1;
         bm_header.total_scanlines = word2 + 1;
         bm_cache_special = 0;      //assume 0 case
         if (bm_header.pixel_offset == 0) {
            if ((bm_header.pixel_width == 32) && (bm_header.total_scanlines == 32)) {
               bm_cache_special = 1;
            }
            else if ((bm_header.pixel_width == 64) && (bm_header.total_scanlines == 64)) {
               bm_cache_special = 2;
            }
         }
         //get # pixels
         received_bytes_wide = bm_header.pixel_width + bm_header.pixel_offset;
         //calculate number of bytes needed to receive
         received_bytes_wide = (received_bytes_wide +
                                 (received_bytes_wide & 1))/2;
         //calculate number of bytes per logical scanline in cache
         //and needed for dib representation (all lines length multiple of 4
         bm_header.cache_bytes_wide = received_bytes_wide +
                                      size_array_1[received_bytes_wide & 3];
         dib_bytes_wide = bm_header.cache_bytes_wide;

         //jk256
         //if 256 color mode and use16_256palette says so then we need to get
         //a new palette for the last palette of 16 colors
         if ((vColor == Color_Cap_256) && (use16_256palette == 1)) {
            for (i=0; i<16 ; i++) {
               GetNextTWCmdBytes((LPBYTE) &bitmapinfo_4BPP_PALETTE_last256.bmiColors[i], 1);
            }
         }
      }
      else if (!(word1 & 0x0030)){
         //2 color bitmap
         bitmap_header_flags(bm_header, color) = 0;
         bm_header.pixel_offset = (BYTE) ((word1 & 0xe000) >> 13);
         //word1 will hold width, word2 will hold height
         word2 &= 0x0300;
         word1 = (word1 & 0x1c00) >> 2;
         GetNextTWCmdBytes((LPBYTE) &word1,1);
         GetNextTWCmdBytes((LPBYTE) &word2,1);
         bm_header.pixel_width = word1 + 1;
         bm_header.total_scanlines = word2 + 1;
         bm_cache_special = 0;
         //get # pixels
         received_bytes_wide = bm_header.pixel_width + bm_header.pixel_offset;
         //calculate number of bytes needed to receive
         received_bytes_wide = (received_bytes_wide / 8) +
                               ( ( (received_bytes_wide & 7) + 7) / 8);
         //for monochrome, cache scanline size same as received
         bm_header.cache_bytes_wide = received_bytes_wide;
         //for 2 color, all scanlines must be a multiple of 2
         dib_bytes_wide = received_bytes_wide + (received_bytes_wide & 1);
      }
      else ASSERT(1,0);    //dims not supported yet

      //now deal with size info
      //jkstage256 - chunk type and control info and chain calculation different for
      //    a 256 color bitmap.  for bitblt trick size  <= 2048 and chunk type dont matter

      if (bm_cache_special == 0) {
         //total_cache_bytes is setup to not reflect the control
         //information if not caching!!!!
         total_cache_bytes = (DWORD) (((DWORD) bm_header.cache_bytes_wide) *
                             ((DWORD) bm_header.total_scanlines));
         if (jtocache) {
            total_cache_bytes += 8;  //for control  information
         }
         if (total_cache_bytes > 2048) {
            bitmap_header_flags(bm_header, chained) = 1;
            bitmap_header_flags(bm_header, size) = 0;
            parm_chunk_type = _2K;
         }
         else {
            bitmap_header_flags(bm_header, chained) = 0;
            if (total_cache_bytes > 512) {
               bitmap_header_flags(bm_header, size) = 0;
               parm_chunk_type = _2K;
            }
            else {
               bitmap_header_flags(bm_header, size) = 1;
               parm_chunk_type = _512B;
            }
         }

      }
      else {
         //special case
         bitmap_header_flags(bm_header, chained) = 0;
         if (bm_cache_special == 1) {
            bitmap_header_flags(bm_header, size) = 1;
            parm_chunk_type = _512B;
         }
         else {
            bitmap_header_flags(bm_header, size) = 0;
            parm_chunk_type = _2K;
         }
      }

      //we are all setup
#ifdef DEBUG
      TRACE((TC_TW,TT_TW_BLT,"TW: rop3=%08x\n", rop3));

      if (bitmap_header_flags(bm_header, color) == 2) {
         TRACE((TC_TW,TT_TW_BLT,"TW: 256 color bitmap, "));
      }
      else if (bitmap_header_flags(bm_header, color) == 1) {
         TRACE((TC_TW,TT_TW_BLT,"TW: 16 color bitmap, "));
      }
      else TRACE((TC_TW, TT_TW_BLT, "TW: 2 color bitmap, "));

      if (bitmap_header_flags(bm_header, size)) {
         TRACE((TC_TW, TT_TW_BLT, "_512B size, "));
         ASSERT(bm_cache_special != 2,0);
         if (bm_cache_special == 1) {
            TRACE((TC_TW, TT_TW_BLT," special, \n"));
         }
      }
      else {
         ASSERT(bm_cache_special != 1,0);
         if (!bitmap_header_flags(bm_header, chained)) {
            TRACE((TC_TW, TT_TW_BLT, "_2K size, not chained, "));
            if (bm_cache_special == 2) {
               TRACE((TC_TW, TT_TW_BLT, "special, "));
            }
         }
         else {
            ASSERT(bm_cache_special == 0,0);
            TRACE((TC_TW, TT_TW_BLT, "_2K size, chained, "));
         }
      }
      TRACE((TC_TW, TT_TW_BLT, "TW: pixel_width=%u, pixel_offset=%u, total_scan_lines=%u\n",
            (UINT) bm_header.pixel_width, (UINT) bm_header.pixel_offset,
            (UINT) bm_header.total_scanlines));
      TRACE((TC_TW, TT_TW_BLT, "TW: received_bytes_wide=%u, cache_bytes_wide=%u, dib_bytes_wide=%u\n",
                  (UINT) received_bytes_wide, (UINT) bm_header.cache_bytes_wide,
                  (UINT) dib_bytes_wide));

#endif

      total_scanlines_left = bm_header.total_scanlines;

      section_count = 0;

      //in the case of not caching we will be displaying the bitmap
      //while we are receiving it, a section at a time.
      //even when caching, we will initialize these things so we
      //can avoid doing it later on when taking the data out of the cache
      //the taking it out of the cache case must initialize the same stuff

      //jk256
      if (bitmap_header_flags(bm_header, color) == 2) {
         if (use256_256palette == 0) {
            lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_8BPP_PALETTE;
         }
         else {
            lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_8BPP_PALETTE_MAP;
         }
         lpbitmapinfo->bmiHeader.biWidth = dib_bytes_wide;
      }
      else if (bitmap_header_flags(bm_header, color) == 1) {
         //16 color
         if (vColor == Color_Cap_256) {
            if (use16_256palette == 0) {
               lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_4BPP_PALETTE_default256;
            }
            else lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_4BPP_PALETTE_last256;
         }
#ifdef THINPAL
         else
         lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_4BPP_PALETTE;
#else
         else
         lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_4BPP_RGBQUAD;
#endif
         lpbitmapinfo->bmiHeader.biWidth = dib_bytes_wide * 2;
      }
      else {
         //2 color
         //jk256 - 2 bytes for foreground and background color
         if (vColor == Color_Cap_256) {
            if ((dcstate.bkcolor != PALETTEINDEX(last_backColor) ) ||
                (dcstate.txtcolor != PALETTEINDEX(last_fbColor) ) ) {
               SetTextColor(device, PALETTEINDEX(last_fbColor) );
               SetBkColor(device, PALETTEINDEX(last_backColor) );
               dcstate.bkcolor = PALETTEINDEX(last_backColor);
               dcstate.txtcolor = PALETTEINDEX(last_fbColor);
            }
         }
         else {
            //16 color mode
            if ((dcstate.bkcolor != twsolidcolor[last_fbColor & 0x0f]) ||
                (dcstate.txtcolor != twsolidcolor[last_fbColor >> 4]) ) {
               SetTextColor(device, twsolidcolor[last_fbColor >> 4]);
               SetBkColor(device, twsolidcolor[last_fbColor & 0x0f]);
               dcstate.bkcolor = twsolidcolor[last_fbColor & 0x0f];
               dcstate.txtcolor = twsolidcolor[last_fbColor >> 4];
            }
         }

         //not using the following code block because requested colors
         //are converted to shades of gray
#if 0
         //since 2 color, we must initialize the color info
         //jkbetter, code needs to change when go to palette mgmgt
         //jktest - make sure get 0 and 1 color values correct
         lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_1BPP_RGBQUAD;
         lpbitmapinfo->bmiHeader.biWidth = dib_bytes_wide * 8;
         i = ((UINT) last_fbColor) >> 4;    //foreground color
         lpbitmapinfo->bmiColors[0].rgbBlue = GetBValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[0].rgbGreen = GetGValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[0].rgbRed = GetRValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[0].rgbReserved = 0;


         i = (UINT) (last_fbColor & 0x0f) ;    //background color
         lpbitmapinfo->bmiColors[1].rgbBlue = GetBValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[1].rgbGreen = GetBValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[1].rgbRed = GetBValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[1].rgbReserved = 0;
#endif

      }

      //jk256
      //for a 256 color bitmap, it is possible for section 0 to have
      //no data if dib_bytes_wide >2040 bytes because we allow a scanline
      //to be up to 2048 bytes wide and scanlines do not cross cache slot
      //boundaries
      //in this case we still count the section with no data as a section

      while (total_scanlines_left > 0) {
         //this is the loop that gets all the communicated bitmap data
         //this is the top of the section loop
         if (jtocache) {
            if ( jdim ) {
               if (section_count == 0) {

                  GetNextTWCmdBytes((LPDWORD) &signatureLO, 4);
                  GetNextTWCmdBytes((LPDWORD) &signatureHI, 4);
        
                  TRACE((TC_TW,TT_TW_BLT+TT_TW_DIM,"BltSrcCommon DIM to cache sig %8x%8x(C-1)\n", signatureHI, signatureLO));

                  //jk256 - section 0 may have no data
                  if ((bitmap_header_flags(bm_header, color) == 2) && (dib_bytes_wide > 2040)) {
                     lpcache = lpTWDIMCacheWrite( signatureHI, signatureLO, section_count );
                     memcpy(lpcache, (LPVOID) &bm_header, sizeof(bm_header));
                     section_count++;
                  }
               }
            }
            else {
               if (section_count == 0) {
                  GetNextTWCmdBytes((LPWORD) &cache_handle, 2);
                  chain_handle = cache_handle;
   
                  //jk256 - section 0 may have no data
                  if ((bitmap_header_flags(bm_header, color) == 2) && (dib_bytes_wide > 2040)) {
                     lpcache = lpTWCacheWrite((UINT) cache_handle, parm_chunk_type,
                                    0, (UINT) chain_handle);   //do size later!
                     memcpy(lpcache, (LPVOID) &bm_header, sizeof(bm_header));
                     GetNextTWCmdBytes((LPWORD) &chain_handle, 2);
                     section_count++;
                  }
               }
               else GetNextTWCmdBytes((LPWORD) &chain_handle, 2);
   
               TRACE((TC_TW,TT_TW_BLT+TT_TW_CACHE,"TW: object handle=%u, chain handle=%u\n",
                                 (UINT) cache_handle, (UINT) chain_handle));
            }
         }

         //get the amount of data
         GetNextTWCmdBytes((LPWORD) &section_rle_bytecount, 2);
         section_flags = section_rle_bytecount & 0xF000;
         section_rle_bytecount &= 0x0FFF;

         TRACE((TC_TW,TT_TW_BLT,
                "TW: RLE info, section_flags %04x, section_byte_count %04x",
                section_flags, section_rle_bytecount));

         if (section_flags & 0x8000) {
            //its rle data

            TRACE((TC_TW, TT_TW_BLT,"TW: rle bitmap section=%u, rlesize=%u, ",
                     (UINT) section_count, (UINT) section_rle_bytecount));
            ASSERT(section_rle_bytecount <= 2048,0);

            //  packed pixel format
            if ( (vColor == Color_Cap_256) && (section_flags & 0x7000) ) {
                GetNextTWCmdBytes((LPBYTE) (lpstatic_buffer),
                                  (UINT) section_rle_bytecount);
            }
            else {
                GetNextTWCmdBytes((LPBYTE) (lpstatic_buffer+1022),
                                  (UINT) section_rle_bytecount);
            }

            if (!jtocache) {

               lpsection_bytes = (LPBYTE) lpstatic_buffer;

               // packed pixel format
               if ( (vColor == Color_Cap_256) && (section_flags & 0x7000) ) {

                  section_bytecount = RleDecompress(section_rle_bytecount,
                                       (LPBYTE) (lpstatic_buffer),
                                       (LPBYTE) (lpstatic_buffer+1022));

                  TRACE((TC_TW, TT_TW_BLT, "TW: NOT CACHING bytecount=%u\n",(UINT) section_bytecount));
                  ASSERT(section_bytecount > 0,0);
                  ASSERT(section_bytecount <= 2048,0);

                  TRACE((TC_TW,TT_TW_BLT,
                         "TW: unpacking pixels, flags %04x\n",
                         section_flags));

                  // calculate byte to unpack
                  temp_scanlines_left = 2048 / dib_bytes_wide;
                  if (temp_scanlines_left > (UINT) total_scanlines_left) {
                     temp_scanlines_left = total_scanlines_left;
                  }
                  num_bytes_to_unpack = temp_scanlines_left * 
                                        received_bytes_wide;

                  //  go do the unpack
                  bm_color = (BYTE) bitmap_header_flags(bm_header, color);
                  section_bytecount = UnpackPixels( (LPBYTE) (lpstatic_buffer+1022), 
                                                    (UINT)   section_bytecount,
                                                    (LPBYTE) lpstatic_buffer,
                                                    (WORD)   section_flags,
                                                    (BYTE)   bm_color,
                                                    (UINT)   num_bytes_to_unpack);
               }
               else {

                  section_bytecount = RleDecompress(section_rle_bytecount,
                                       (LPBYTE) (lpstatic_buffer+1022),
                                       (LPBYTE) lpstatic_buffer);
                  TRACE((TC_TW, TT_TW_BLT, "TW: NOT CACHING bytecount=%u\n",(UINT) section_bytecount));
                  ASSERT(section_bytecount > 0,0);
                  ASSERT(section_bytecount <= 2048,0);
               }
            }
            else {
               //we are caching

               //get a cache pointer

               if ( jdim ) {
                  lpcache = lpTWDIMCacheWrite( signatureHI, signatureLO, section_count );
               }
               else {
                  lpcache = lpTWCacheWrite((UINT) cache_handle, parm_chunk_type,
                                 0, (UINT) chain_handle);   //do size later!
               }

               //only put header in cache if section 0 and special is normal
               jcache_bmheader = ((!section_count) && (!bm_cache_special));

               if (jcache_bmheader) {
                  TRACE((TC_TW,TT_TW_BLT,"TW: putting bm_header into cache\n"));
                  //(BITMAP_HEADER) *((LPBITMAP_HEADER) lpcache) = bm_header;
                  memcpy(lpcache, (LPVOID) &bm_header, sizeof(bm_header));
                  lpcache += 8;
                  cache_bytecount = 8;    //show put header in the cache
               }
               else cache_bytecount = 0;

               //lpcache points to where the cache data should go
               //header is setup if needs to be
               //cache_bytecount is 0 or 8 depending on header

               //NOTE: still need to calculate section_scanlines_left

               //data goes directly into the cache if 2 color
               //16 color data must always be treated special

               if (!bitmap_header_flags(bm_header, color)) {
                  //move directly into cache and we are done with this section
                  TRACE((TC_TW,TT_TW_BLT,"TW: RleDecompress directly into cache at address=%lx\n",
                                       lpcache));
                  section_bytecount = RleDecompress(section_rle_bytecount,
                                    (LPBYTE) (lpstatic_buffer+1022),
                                    (LPBYTE) lpcache);

                  TRACE((TC_TW, TT_TW_BLT, "TW: bytecount=%u\n",(UINT) section_bytecount));
                  ASSERT(section_bytecount > 0,0);
                  ASSERT(section_bytecount <= 2048,0);

                  section_scanlines_left = section_bytecount / received_bytes_wide;
#ifdef DEBUG
                  ASSERT((section_bytecount % received_bytes_wide) == 0,0);
#endif
                  TRACE((TC_TW,TT_TW_BLT,"TW: CACHING total scanlines left=%u, scanlines in section=%u\n",
                               total_scanlines_left,section_scanlines_left));

                  finishedsize = cache_bytecount + section_bytecount;
               }
               //jk256 - NOTE:!!! code block same for 16 and 256 color bitmap
               else {
                  //its a 16 or 256 color bitmap not in the correct format

#if 1
                  //  packed pixel format
                  if ( (vColor == Color_Cap_256) && (section_flags & 0x7000) ) {
   
                     TRACE((TC_TW,TT_TW_BLT,"TW: CACHING, expanding rle data into temporary buffer\n",
                                          lpcache));
   
                     section_bytecount = RleDecompress(section_rle_bytecount,
                                       (LPBYTE) lpstatic_buffer,
                                       (LPBYTE) (lpstatic_buffer+1022));
   
                     TRACE((TC_TW,TT_TW_BLT,
                            "TW: unpacking pixels, flags %04x\n",
                            section_flags));
   
                     // calculate byte to unpack
                     temp_scanlines_left = (2048 - cache_bytecount) / dib_bytes_wide;
                     if (temp_scanlines_left > (UINT) total_scanlines_left) {
                        temp_scanlines_left = total_scanlines_left;
                     }
                     num_bytes_to_unpack = temp_scanlines_left * 
                                           received_bytes_wide;
   
                     //  go do the unpack
                     bm_color = (BYTE) bitmap_header_flags(bm_header, color);
                     section_bytecount = UnpackPixels( (LPBYTE) (lpstatic_buffer+1022), 
                                                       (UINT)   section_bytecount,
                                                       (LPBYTE) lpstatic_buffer,
                                                       (WORD)   section_flags,
                                                       (BYTE)   bm_color,
                                                       (UINT)   num_bytes_to_unpack);
   
                     TRACE((TC_TW, TT_TW_BLT, "TW: bytecount=%u\n",(UINT) section_bytecount));
                     ASSERT(section_bytecount > 0,0);
                     ASSERT(section_bytecount <= 2048,0);
   
                     section_scanlines_left = section_bytecount / received_bytes_wide;
                     TRACE((TC_TW,TT_TW_BLT,"TW: CACHING total scanlines left=%u, scanlines in section=%u\n",
                                  total_scanlines_left,section_scanlines_left));
   
                     finishedsize = cache_bytecount +
                                   MakeInvertedDibFormat((LPBYTE) lpstatic_buffer,
                                   (LPBYTE) lpcache,
                                   received_bytes_wide, dib_bytes_wide,
                                   section_scanlines_left);
                  }
                  else {
   
                     //changing code to use RleInvertedDecompress
                     //need to compute section_scanlines_left from section size and
                     //the size of a color scanline in the cache == dib_bytes_wide
   
                     section_scanlines_left = (2048 - cache_bytecount) / dib_bytes_wide;
                     if (section_scanlines_left > (UINT) total_scanlines_left) {
                        section_scanlines_left = total_scanlines_left;
                     }
                     section_bytecount = RleInvertedDecompress(section_rle_bytecount,
                                                (LPBYTE) (lpstatic_buffer+1022),
                                                (LPBYTE) lpcache,
                                                received_bytes_wide, dib_bytes_wide,
                                                section_scanlines_left);
   
                     TRACE((TC_TW, TT_TW_BLT, "TW: RleInverted Decompress bytecount=%u\n",(UINT) section_bytecount));
                     ASSERT(section_bytecount > 0,0);
                     ASSERT(section_bytecount <= (2048 - cache_bytecount),0);
                     TRACE((TC_TW,TT_TW_BLT,"TW: CACHING total scanlines left=%u, scanlines in section=%u\n",
                                  total_scanlines_left,section_scanlines_left));
   
                     finishedsize = cache_bytecount + section_bytecount;
                  }
#else
                  TRACE((TC_TW,TT_TW_BLT,"TW: CACHING, expanding rle data into temporary buffer\n",
                                       lpcache));

                  section_bytecount = RleDecompress(section_rle_bytecount,
                                    (LPBYTE) (lpstatic_buffer+1022),
                                    (LPBYTE) lpstatic_buffer);

                  TRACE((TC_TW, TT_TW_BLT, "TW: bytecount=%u\n",(UINT) section_bytecount));
                  ASSERT(section_bytecount > 0,0);
                  ASSERT(section_bytecount <= 2048,0);

                  section_scanlines_left = section_bytecount / received_bytes_wide;
                  TRACE((TC_TW,TT_TW_BLT,"TW: CACHING total scanlines left=%u, scanlines in section=%u\n",
                               total_scanlines_left,section_scanlines_left));


                  finishedsize = cache_bytecount +
                                MakeInvertedDibFormat((LPBYTE) lpstatic_buffer,
                                (LPBYTE) lpcache,
                                received_bytes_wide, dib_bytes_wide,
                                section_scanlines_left);
#endif
               }
            }

         }
         else {
            //its not rle data

            //if caching and ( (16 color and format of scanline already
            // correct) or 2 color)
            //then we want to move the bitmap data directly into the cache slot

            section_bytecount = section_rle_bytecount;
            TRACE((TC_TW,TT_TW_BLT,"TW: bitmap NOT rle section=%u, size=%u\n",
                        (UINT) section_count, (UINT) section_bytecount));
            ASSERT((section_bytecount > 0) && (section_bytecount <= 2048),0);


            if (!jtocache) {

               TRACE((TC_TW,TT_TW_BLT,"TW: NOT CACHING\n"));
               lpsection_bytes = (LPBYTE) lpstatic_buffer;

               // packed pixel format
               if ( (vColor == Color_Cap_256) && (section_flags & 0x7000) ) {

                  GetNextTWCmdBytes((LPBYTE) (lpstatic_buffer+1022), section_bytecount);

                  TRACE((TC_TW,TT_TW_BLT,
                         "TW: unpacking pixels, flags %04x\n",
                         section_flags));

                  // calculate byte to unpack
                  temp_scanlines_left = 2048 / dib_bytes_wide;
                  if (temp_scanlines_left > (UINT) total_scanlines_left) {
                     temp_scanlines_left = total_scanlines_left;
                  }
                  num_bytes_to_unpack = temp_scanlines_left * 
                                        received_bytes_wide;

                  //  go do the unpack
                  bm_color = (BYTE) bitmap_header_flags(bm_header, color);
                  section_bytecount = UnpackPixels( (LPBYTE) (lpstatic_buffer+1022), 
                                                    (UINT)   section_bytecount,
                                                    (LPBYTE) lpstatic_buffer,
                                                    (WORD)   section_flags,
                                                    (BYTE)   bm_color,
                                                    (UINT)   num_bytes_to_unpack);
               }
               else {

                  GetNextTWCmdBytes((LPBYTE) lpstatic_buffer, section_bytecount);
               }
            }
            else {
               //we are caching

               //get a cache pointer

               if ( jdim ) {
                  lpcache = lpTWDIMCacheWrite( signatureHI, signatureLO, section_count );
               }
               else {
                   lpcache = lpTWCacheWrite((UINT) cache_handle, parm_chunk_type,
                                  0, (UINT) chain_handle);   //do size later!
               }

               //only put header in cache if section 0 and special is normal
               jcache_bmheader = ((!section_count) && (!bm_cache_special));

               if (jcache_bmheader) {
                  TRACE((TC_TW,TT_TW_BLT,"TW: putting bm_header into cache\n"));
                  //(BITMAP_HEADER) *((LPBITMAP_HEADER) lpcache) = bm_header;
                  memcpy(lpcache, (LPVOID) &bm_header, sizeof(bm_header));
                  lpcache += 8;
                  cache_bytecount = 8;    //show put header in the cache
               }
               else cache_bytecount = 0;



               //lpcache points to where the cache data should go
               //header is setup if needs to be
               //cache_bytecount is 0 or 8 depending on header
               //section_scanlines_left is calculated

               //data goes directly into the cache if 2 color
               //16 color data must always be treated special

               if (!bitmap_header_flags(bm_header, color)) {

                  section_scanlines_left = section_bytecount / received_bytes_wide;
#ifdef DEBUG
                  ASSERT((section_bytecount % received_bytes_wide) == 0,0);
#endif
                  TRACE((TC_TW,TT_TW_BLT,"TW: CACHING total scanlines left=%u, scanlines in section=%u\n",
                                  total_scanlines_left,section_scanlines_left));
    
                  //move directly into cache and we are done with this section
                  TRACE((TC_TW,TT_TW_BLT,"TW: moving data directly into cache at address=%lx\n",
                                       lpcache));
                  GetNextTWCmdBytes((LPBYTE) lpcache, section_bytecount);

                  finishedsize = cache_bytecount + section_bytecount;

               }
               //jk256 - NOTE:!!! code block same for 16 and 256 color bitmap
               else {
                  //its a 16 color bitmap not in the correct format
                  //because almost every bitmap follows the rle path we will
                  //not write a special routine to bring the data in directly
                  //into the buffer in the correct final format
                  //but we will continue to go through this 2 step procedure

                  //NOTE: same code path for 256 color bitmap

                  // packed pixel format
                  if ( (vColor == Color_Cap_256) && (section_flags & 0x7000) ) {

                     GetNextTWCmdBytes((LPBYTE) (lpstatic_buffer+1022), section_bytecount);
                     TRACE((TC_TW,TT_TW_BLT,"TW: expanding data into dib format into cache at address=%lx\n",
                                          lpcache));

                     TRACE((TC_TW,TT_TW_BLT,
                            "TW: unpacking pixels, flags %04x\n",
                            section_flags));

                     // calculate byte to unpack
                     temp_scanlines_left = (2048 - cache_bytecount) / dib_bytes_wide;
                     if (temp_scanlines_left > (UINT) total_scanlines_left) {
                        temp_scanlines_left = total_scanlines_left;
                     }
                     num_bytes_to_unpack = temp_scanlines_left * 
                                           received_bytes_wide;
   
                     //  go do the unpack
                     bm_color = (BYTE) bitmap_header_flags(bm_header, color);
                     section_bytecount = UnpackPixels( (LPBYTE) (lpstatic_buffer+1022),
                                                       (UINT)   section_bytecount,
                                                       (LPBYTE) lpstatic_buffer,
                                                       (WORD)   section_flags,
                                                       (BYTE)   bm_color,
                                                       (UINT)   num_bytes_to_unpack);
                  }
                  else {

                     GetNextTWCmdBytes((LPBYTE) lpstatic_buffer, section_bytecount);
                     TRACE((TC_TW,TT_TW_BLT,"TW: expanding data into dib format into cache at address=%lx\n",
                                          lpcache));
                  }

                  section_scanlines_left = section_bytecount / received_bytes_wide;
#ifdef DEBUG
                  ASSERT((section_bytecount % received_bytes_wide) == 0,0);
#endif
                  TRACE((TC_TW,TT_TW_BLT,"TW: CACHING total scanlines left=%u, scanlines in section=%u\n",
                                  total_scanlines_left,section_scanlines_left));
    
                  finishedsize = cache_bytecount +
                                MakeInvertedDibFormat((LPBYTE) lpstatic_buffer,
                                (LPBYTE) lpcache,
                                received_bytes_wide, dib_bytes_wide,
                                section_scanlines_left);
               }
            }
         }

         //WE ONLY WANT TO PROCESS THIS CODE IF NOT CACHING BECAUSE THEN
         //WE NEED TO DISPLAY THE BITMAP

         //at this point
         // lpsection_bytes points at data
         // section_bytecount has the amount of data
         //
         // we are not caching SO WE NEED TO DISPLAY THE DATA
         //    a) data at lpstatic_buffer+0 is already in correct format
         //    b) data needs to be put in correct format starting at
         //       lpstatic_buffer+512
         //


         if (!jtocache) {
            //we are not caching so we need to possibly convert the data
            //and then display it


            //compute number of scanlines in this section

            section_scanlines_left = section_bytecount / received_bytes_wide;
#ifdef DEBUG
            ASSERT((section_bytecount % received_bytes_wide) == 0,0);
#endif
            TRACE((TC_TW,TT_TW_BLT,"TW: total scanlines left=%u, scanlines in section=%u\n",
                               total_scanlines_left,section_scanlines_left));


            //if not caching, then we want to display the section here
            //since not caching we can assume its a noclip scenario

            //use different bitmapinfo if 16 color or 2 color bitmap or 256
            //lpbitmapinfo and width in bitmapinfo already setup
            //only thing left is bitmap height which is independent
            //of the number of colors

            //NOTE: WIN16 CODE DOES NOT WORK WITH DIBs with negative heights
            //      so we use a negative height in StretchDIBits for the source height
            //       the really weird thing is the ySrc coordinate that we ended up having
            //to use in order for the thing to work.  got it to work by trial and error
            //NOTE:The same code worked for NT and Win3.1 but did not work on windows 95.
            //Because of this we needed to use ifdefs between win16 and win32

            //NOTE: same code now works on all 3 platforms and seperate win32 code
            //did not work on NT with noncopy rops even though it worked on win95
            //so we now have common code for all 3.  Left old code because this is
            //an area where the rules keep on changing

            //LAST NOTE: we are changing this code to use the optimized code path
            //of setdibits_bitblttodevice (the rop is always srccopy)

            //WARNING TO ANYBODY READING THIS CODE:
            // we assume that the rop is always srccopy because we know that
            // we are allocating a big enough cache for the windows client that
            // if we don't cache the bitmap we are using the bitblt trick
            //which is a copy


            //jk256 - code good for both 16 and 256 color bitmaps
            if (bitmap_header_flags(bm_header, color)) {

//#ifdef WIN32
#if 0

               lpbitmapinfo->bmiHeader.biHeight = -((LONG) section_scanlines_left);

               //noclip case is simple
               retcode = StretchDIBits(device, xDestULH, yDestULH,
                                       (int) bm_header.pixel_width,
                                       (int) section_scanlines_left,
                                       (int) bm_header.pixel_offset, //xSrcULH
                                       0,  //ySrcULH - see biHeight comment
                                       (int) bm_header.pixel_width,
                                       (int) section_scanlines_left,
                                       lpsection_dib_bytes,  //pointer to bitmap data
                                       lpbitmapinfo,   //pointer to bitmap description
                                       DIB_ColorMode,  // - code changes when use palettes
                                       rop3);

               TRACE((TC_TW,TT_TW_BLT,"StretchDIBits return code=%u\n",(UINT) retcode));

               ASSERT(retcode != GDI_ERROR,0);

#else

               //we have optimized but we are keeping a copy of the original code here
               //so if we go back to try things we already know what we have done
               lpbitmapinfo->bmiHeader.biHeight = (LONG) section_scanlines_left;

#if 1
               //winbench shows 3 to 10 times faster than StretchDIBits
               //still need to put data in the correct format

               MakeInvertedDibFormat((LPBYTE) lpsection_bytes,
                                     (LPBYTE) (lpstatic_buffer+512),
                                     received_bytes_wide, dib_bytes_wide,
                                     section_scanlines_left);

               retcode = SetDIBitsToDevice(device, xDestULH, yDestULH,
                                    (int) bm_header.pixel_width,  //width of area in dib that are using
                                    (int) section_scanlines_left,
                                    bm_header.pixel_offset, //starting x coord in dib that are using
                                    0,
                                    0,
                                    (UINT) section_scanlines_left,
                                    (LPBYTE) (lpstatic_buffer+512),
                                    lpbitmapinfo,
                                    DIB_ColorMode);

#else
               //put data in the correct format
               if (received_bytes_wide == dib_bytes_wide) {
                  //already in dib format
                  lpsection_dib_bytes = lpsection_bytes;
               }
               else {
                  //take data at lpsection_bytes and put in dib format
                  //starting at lpstatic_buffer+512

                  //note: for the no caching case we keep the data in
                  //rightside up format even for 16 color bitmaps
                  //
                  //for the caching case, 16 color bitmaps go in upside down

                  lpsection_dib_bytes = (LPBYTE) (lpstatic_buffer+512);

                  MakeDibFormat((LPBYTE) lpsection_bytes,
                              (LPBYTE) (lpstatic_buffer+512),
                              received_bytes_wide, dib_bytes_wide,
                              section_scanlines_left);
               }

               retcode = StretchDIBits(device, xDestULH, yDestULH,
                                       (int) bm_header.pixel_width,
                                       (int) section_scanlines_left,
                                       (int) bm_header.pixel_offset, //xSrcULH
                                       (int) section_scanlines_left + 1,  //ySrcULH - see biHeight comment
                                       (int) bm_header.pixel_width,
                                       - ((int) section_scanlines_left),
                                       lpsection_dib_bytes,  //pointer to bitmap data
                                       lpbitmapinfo,   //pointer to bitmap description
                                       DIB_ColorMode,  // code changes when use palettes
                                       rop3);
#endif

               TRACE((TC_TW,TT_TW_BLT,"no chache image display return code=%u\n",(UINT) retcode));

#endif
            }
            else {
               //its 2 color
               //get into correct format
               if (received_bytes_wide == dib_bytes_wide) {
                  //already in dib format
                  lpsection_dib_bytes = lpsection_bytes;
               }
               else {
                  //take data at lpsection_bytes and put in dib format
                  //starting at lpstatic_buffer+512

                  //note: for the no caching case we keep the data in
                  //rightside up format even for 16 color bitmaps
                  //
                  //for the caching case, 16 color bitmaps go in upside down

                  lpsection_dib_bytes = (LPBYTE) (lpstatic_buffer+512);

                  MakeDibFormat((LPBYTE) lpsection_bytes,
                              (LPBYTE) (lpstatic_buffer+512),
                              received_bytes_wide, dib_bytes_wide,
                              section_scanlines_left);
               }

               hbmcurrent = CreateBitmap(
                              dib_bytes_wide * 8,      //width
                              section_scanlines_left,    //height
                              1,
                              1,
                              lpsection_dib_bytes);

               hbmold = SelectObject(compatDC,hbmcurrent);

               jretcode = BitBlt(
                           device,
                           xDestULH,
                           yDestULH,
                           bm_header.pixel_width,
                           section_scanlines_left,
                           compatDC,
                           bm_header.pixel_offset,
                           0,
                           rop3);

               ASSERT(jretcode,0);

               SelectObject(compatDC,hbmold);

               jretcode = DeleteObject(hbmcurrent);

               ASSERT(jretcode,0);
            }

            yDestULH += section_scanlines_left; //only update in no cache case
                                                //because displaying here
         }

         total_scanlines_left -= section_scanlines_left;
         section_count++;


      }  //while scanlines left

      //we are done if not caching and not cached because displayed
      if (!jtocache) {
         TRACE((TC_TW,TT_TW_BLT,"BltSrcCommon returning because not caching\n"));
         if (amountPaletteMapCache > 0) {
            PaletteMapCache(amountPaletteMapCache, handlePaletteMapCache);
         }
         return(TRUE);
      }
      else {

         if ( jdim ) {
            jretcode = finishedTWDIMCacheWrite(finishedsize);
         }
         else {
            jretcode = finishedTWCacheWrite(finishedsize);
         }
         ASSERT(jretcode,0);
      }


   }
   else {
      //already cached
      //need to setup bm_header, cache_handle,
      //dib_bytes_wide
      //correct bitmapinfo structure with width and lpbitmapinfo
      //if 2 color bitmap then bitmapinfo has color info
      //bm_cache_special

      if ( jdim ) {

         parm_chunk_type = _2K;

         //not the special no header cached case
         bm_cache_special = 0;
         lpcache = lpTWDIMCacheRead( signatureHI, signatureLO, &cache_bytecount, 0 );

         //bm_header = (BITMAP_HEADER) *((LPBITMAP_HEADER) lpcache);
         if ( lpcache != NULL ) {
            memcpy((LPVOID) &bm_header, lpcache, sizeof(bm_header));
         }
         else {
            bitmap_header_flags(bm_header, color) = 
                      (BYTE)  (signatureLO & 0x00000007);
         }
         TRACE((TC_TW,TT_TW_BLT,"BltSrcCommon: set bm_header: cached DIM\n"));
      }
      else {
         cache_handle = (word1 >> 8) | ((word1 << 8) & 0x0f00);
   
         if (word1 & 0x0020) {
            parm_chunk_type = _512B;
         }
         else parm_chunk_type = _2K;
   
         if (word1 & 0x0010) {
            //the special no header cached case
            bitmap_header_flags(bm_header, color) = 1;
            bitmap_header_flags(bm_header, chained) = 0;
            if (parm_chunk_type == _2K) {
               bm_cache_special = 2;
               bm_header.total_scanlines = 64;
               bm_header.cache_bytes_wide = 32;
               bm_header.pixel_width = 64;
               bm_header.pixel_offset = 0;
               bitmap_header_flags(bm_header, size) = 0;
            }
            else {
               //must be _512B
               bm_cache_special = 1;
               bm_header.total_scanlines = 32;
               bm_header.cache_bytes_wide = 16;
               bm_header.pixel_width = 32;
               bm_header.pixel_offset = 0;
               bitmap_header_flags(bm_header, size) = 1;
            }
         }
         else {
            //not the special no header cached case
            bm_cache_special = 0;
            lpcache = lpTWCacheRead((UINT) cache_handle, parm_chunk_type,
                                    (LPUINT) &cache_bytecount, 0);
            //bm_header = (BITMAP_HEADER) *((LPBITMAP_HEADER) lpcache);
            memcpy((LPVOID) &bm_header, lpcache, sizeof(bm_header));
         }
	 TRACE((TC_TW,TT_TW_BLT,"BltSrcCommon: set bm_header: bm_cache_special %d\n", bm_cache_special));
	 TRACEBUF((TC_TW,TT_TW_BLT, &bm_header, sizeof(bm_header)));
      }

      //if its 256 color mode and its a 16 color bitmap then we need to
      //read an extra byte to determine which 16 color palette to use
      //if 256 color bitmap then need to read an extra byte to see
      //what 256 color palette to use

      if (vColor == Color_Cap_256) {
         if (bitmap_header_flags(bm_header, color) == 1) {
            GetNextTWCmdBytes((LPBYTE) &byte1, 1);
            use16_256palette = (WORD) (byte1 >> 6);
            if (use16_256palette == 1) {
               for (i=0; i<16 ; i++) {
                  GetNextTWCmdBytes((LPBYTE) &bitmapinfo_4BPP_PALETTE_last256.bmiColors[i], 1);
               }
            }
         }
         else if (bitmap_header_flags(bm_header, color) == 2) {
            use256_256palette = PaletteMap((LPWORD) &amountPaletteMapCache,(LPWORD) &handlePaletteMapCache);
         }
      }

      // BUGBUG: read failed
      if ( jdim ) {
         if ( lpcache == NULL) {
   
            // need to suck up more protocol
            if (working_case == cmplxclip) {
               cmplx_case = RECTFULL;
               while (cmplx_case != RECTLAST) {
                  cmplxCLPenum((LPINT) &cmplx_case, (LPRECT_ULH) &dest_rect);
               }
            }
   
            // need to cache palette
            if (amountPaletteMapCache > 0) {
               PaletteMapCache(amountPaletteMapCache, handlePaletteMapCache);
            }
   
            // notify host of error
            if ( vpVd ) {
                CACHE_FILE_HANDLE fh;

                *((PULONG)&fh) = signatureHI;
                *((PULONG)&fh[4]) = signatureLO;
                twDIMCacheError( vpVd, fh  );
            }
   
            return(TRUE);
         }
      }

      //now comput received_bytes_wide and dib_bytes_wide

      //if bm_header.cache_bytes_wide == dib_bytes_wide then scanline on 4 byte boundary

      //if 2 color bitmap then bm_header.cache_bytes_wide is set to whatever
      //transmitted scan line size was whether in dib format or not

      //if 16 color bitmap then cached and dib bytes wide the same

      //jk256
      if (bitmap_header_flags(bm_header, color) == 2) {
         //256 color bitmap
         ASSERT(vColor == Color_Cap_256,0);

         dib_bytes_wide = bm_header.cache_bytes_wide;    //cached in dib format

         if (use256_256palette == 0) {
            lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_8BPP_PALETTE;
         }
         else {
            lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_8BPP_PALETTE_MAP;
         }
         lpbitmapinfo->bmiHeader.biWidth = dib_bytes_wide;
      }
      else if (bitmap_header_flags(bm_header, color) == 1) {
         //16 color bitmap

         dib_bytes_wide = bm_header.cache_bytes_wide;    //cached in dib format

         if (vColor == Color_Cap_256) {
            if (use16_256palette == 0) {
               lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_4BPP_PALETTE_default256;
            }
            else lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_4BPP_PALETTE_last256;
         }

#ifdef THINPAL
         else
         lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_4BPP_PALETTE;
#else
         else
         lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_4BPP_RGBQUAD;
#endif
         lpbitmapinfo->bmiHeader.biWidth = dib_bytes_wide * 2;
      }
      else {
         //2 color bitmap
         //jk256 - 2 bytes for foreground and background color
         if (vColor == Color_Cap_256) {
            if ((dcstate.bkcolor != PALETTEINDEX(last_backColor) ) ||
                (dcstate.txtcolor != PALETTEINDEX(last_fbColor) ) ) {
               SetTextColor(device, PALETTEINDEX(last_fbColor) );
               SetBkColor(device, PALETTEINDEX(last_backColor) );
               dcstate.bkcolor = PALETTEINDEX(last_backColor);
               dcstate.txtcolor = PALETTEINDEX(last_fbColor);
            }
         }
         else {
            //16 color mode
            if ((dcstate.bkcolor != twsolidcolor[last_fbColor & 0x0f]) ||
                (dcstate.txtcolor != twsolidcolor[last_fbColor >> 4]) ) {
               SetTextColor(device, twsolidcolor[last_fbColor >> 4]);
               SetBkColor(device, twsolidcolor[last_fbColor & 0x0f]);
               dcstate.bkcolor = twsolidcolor[last_fbColor & 0x0f];
               dcstate.txtcolor = twsolidcolor[last_fbColor >> 4];
            }
         }

         dib_bytes_wide = bm_header.cache_bytes_wide +
                           (bm_header.cache_bytes_wide & 1);

         //not using the following code block because requested colors
         //are converted to shades of gray

#if 0
         //since 2 color, we must initialize the color info
         //jkbetter, code needs to change when go to palette mgmgt
         //jktest - make sure get 0 and 1 color values correct
         lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_1BPP_RGBQUAD;
         lpbitmapinfo->bmiHeader.biWidth = dib_bytes_wide * 8;
         i = ((UINT) last_fbColor) >> 4;    //foreground color
         lpbitmapinfo->bmiColors[0].rgbBlue = GetBValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[0].rgbGreen = GetGValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[0].rgbRed = GetRValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[0].rgbReserved = 0;


         i = (UINT) (last_fbColor & 0x0f) ;    //foreground color
         lpbitmapinfo->bmiColors[1].rgbBlue = GetBValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[1].rgbGreen = GetBValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[1].rgbRed = GetBValue(twsolidcolor[i]);
         lpbitmapinfo->bmiColors[1].rgbReserved = 0;
#endif

      }


   }

   //fall through to common code where display bitmap at cache_handle
   //bm_header, bm_cache_special, and dib_bytes_wide
   //bitmapinfo width and lpbitmapinfo (and color if 2 color)
   //already setup

   //we are all setup
#ifdef DEBUG
   TRACE((TC_TW,TT_TW_BLT,"TW: preparing to display bitmap out of the cache, object_handle=%u\n",
                        (UINT) cache_handle));
   if (bitmap_header_flags(bm_header, color) == 2) {
      TRACE((TC_TW,TT_TW_BLT,"TW: 16 color bitmap, "));
   }
   else if (bitmap_header_flags(bm_header, color) == 1) {
      TRACE((TC_TW,TT_TW_BLT,"TW: 16 color bitmap, "));
   }
   else TRACE((TC_TW, TT_TW_BLT, "TW: 2 color bitmap, "));

   if (bitmap_header_flags(bm_header, size)) {
      TRACE((TC_TW, TT_TW_BLT, "_512B size, "));
      ASSERT(bm_cache_special != 2,0);
      if (bm_cache_special == 1) {
         TRACE((TC_TW, TT_TW_BLT," special, \n"));
      }
   }
   else {
      ASSERT(bm_cache_special != 1,0);
      if (!bitmap_header_flags(bm_header, chained)) {
         TRACE((TC_TW, TT_TW_BLT, "_2K size, not chained, "));
         if (bm_cache_special == 2) {
            TRACE((TC_TW, TT_TW_BLT, "special, "));
         }
      }
      else {
         ASSERT(bm_cache_special == 0,0);
         TRACE((TC_TW, TT_TW_BLT, "_2K size, chained, "));
      }
   }
   TRACE((TC_TW, TT_TW_BLT, "TW: pixel_width=%u, pixel_offset=%u, total_scan_lines=%u\n",
         (UINT) bm_header.pixel_width, (UINT) bm_header.pixel_offset,
         (UINT) bm_header.total_scanlines));
   TRACE((TC_TW, TT_TW_BLT, "TW: cache_bytes_wide=%u, dib_bytes_wide=%u\n",
               (UINT) bm_header.cache_bytes_wide, (UINT) dib_bytes_wide));

#endif



   total_scanlines_left = bm_header.total_scanlines;

   //section_scanlines_left = # scanlines in current section; changes
   //first_section_scanlines = # scanlines in the first section
   //nth_section_scanlines = # scanlines in all sections except first and last
   //

   //jk256 - if cache_bytes_wide > 2040 bytes then it can only be a 256 color
   //bitmap and first_section_scanlines will be 0

   //if not chained then only 1 section
   if (!bitmap_header_flags(bm_header, chained)) {
      first_section_scanlines = total_scanlines_left;
      nth_section_scanlines = 0;    //set to zero if only 1 section
      last_section_scanlines = 0;   //  "
   }
   else {
      //its _2K and chained
      if (bm_cache_special == 0) {
         cache_bytecount = 8;    //control information
      }
      else cache_bytecount = 0;

      //number of scanlines in first section
      first_section_scanlines = (2048 - cache_bytecount) /
                                 bm_header.cache_bytes_wide;
      nth_section_scanlines = 2048 / bm_header.cache_bytes_wide;

      last_section_scanlines = (total_scanlines_left -
                                first_section_scanlines) %
                                nth_section_scanlines;
      if (last_section_scanlines == 0) {
         last_section_scanlines = nth_section_scanlines;
      }
   }


#ifdef DEBUG

   if (!bitmap_header_flags(bm_header, chained)) {
      TRACE((TC_TW,TT_TW_BLT,"TW: bitmap NOT chained. total_scanlines = %u\n",
                        total_scanlines_left));
   }
   else {
      //its chained
      TRACE((TC_TW,TT_TW_BLT,"TW: bitmap chained,total_scanlines=%u,first_section_scanlines=%u\n",
                              total_scanlines_left, first_section_scanlines));
      TRACE((TC_TW,TT_TW_BLT,"TW: nth_section_scanlines=%u, last_section_scanlines=%u\n",
                        nth_section_scanlines, last_section_scanlines));
      TRACE((TC_TW,TT_TW_BLT,"TW: predicted # sections=%u\n", 2 +
                        ( (total_scanlines_left - first_section_scanlines -
                         last_section_scanlines) / nth_section_scanlines) ));

      ASSERT((first_section_scanlines > 0) || (bitmap_header_flags(bm_header, color) == 2),0);
   }


#endif

   //if its the simpleclip case then we want to get the simple clipping
   //dimensions here.  If the source is just clipped horizontally
   //and the entire vertical is used then we want to tweak bm_header to
   //clip horizontally and then take the noclip case path

   //if noclip then set lpparms->xSrcULH to 0 so common code will work
   //do first because next group may change working_case!!!


   if (working_case == noclip) lpparms->xSrcULH = 0;

   else if (working_case == simpleclip) {
      //first see if still need to compute the ulh of the rectangle
      switch (lpparms->simpleclip_case) {
      case 4:
         //lower right hand corner of clipping rectangle and source bitmap
         //are the same

         lpparms->xSrcULH = bm_header.pixel_width -
                                      (WORD) lpparms->simpleclip_srcwidth;
         ASSERT(bm_header.pixel_width >= (WORD) lpparms->simpleclip_srcwidth, 0);

         lpparms->ySrcULH = bm_header.total_scanlines -
                                      (WORD) lpparms->simpleclip_srcheight;
         ASSERT(bm_header.total_scanlines >= (WORD) lpparms->simpleclip_srcheight, 0);
         break;

      case 5:
         //lower left hand corner of the clipping rectangle and source bitmap
         //are the same

         lpparms->xSrcULH = 0;

         lpparms->ySrcULH = bm_header.total_scanlines -
                                      (WORD) lpparms->simpleclip_srcheight;
         ASSERT(bm_header.total_scanlines >= (WORD) lpparms->simpleclip_srcheight, 0);
         break;

      case 6:
         //upper right hand corner of the clipping rectangle and source
         //bitmap are the same

         lpparms->xSrcULH = bm_header.pixel_width -
                                      (WORD) lpparms->simpleclip_srcwidth;
         ASSERT(bm_header.pixel_width >= (WORD) lpparms->simpleclip_srcwidth, 0);

         lpparms->ySrcULH = 0;
         break;

      default:
         break;

      }

      //ok, now see if can treat as a no clipping case by changing
      //pixel_width


      if (lpparms->simpleclip_srcheight == (int) bm_header.total_scanlines) {
#ifdef DEBUG
         ASSERT(lpparms->ySrcULH == 0,0);
#endif
         TRACE((TC_TW,TT_TW_BLT,"TW: handling simple clip with noclip code\n"));
         bm_header.pixel_width = lpparms->simpleclip_srcwidth;
         working_case = noclip;

      }
   }





//if its the no clipping case then start with section_count = 0
//and total_scanlines_left as the total number of scanlines

//since this may be a simple clipping case where only horizontal clipping
//is happenning, the operation adds lpparms->xSrcULH to bm_header.pixel_offset
//so in the case where there is no horizontal clipping lpparms->xSrcULH is
//set to 0.  bm_header.pixel_width is adjusted if simple clipping case
//handled as noclip


   if (working_case == noclip) {

      section_count = 0;

      //jk256 - take into account that the first section may have no
      //data so we want to start with the second section
      if (first_section_scanlines == 0) {
         section_count = 1;
      }

      while (total_scanlines_left > 0) {

         if ( jdim ) {
            lpcache = lpTWDIMCacheRead(signatureHI, signatureLO, 
                                       (LPUINT) &cache_bytecount, section_count);
            // BUGBUG: dim file read error!
            if ( lpcache == NULL ) {

                // notify host of error
                if ( vpVd ) {
                    CACHE_FILE_HANDLE fh;
    
                    *((PULONG)&fh) = signatureHI;
                    *((PULONG)&fh[4]) = signatureLO;
                    twDIMCacheError( vpVd, fh  );
                }
       
                return(TRUE);
            }
         }
         else {
            lpcache = lpTWCacheRead((UINT) cache_handle, parm_chunk_type,
                                    (LPUINT) &cache_bytecount, section_count);
         }

         //figure out the number of scanlines in this section
         if (!section_count) {
            //its the first section
            section_scanlines_left = first_section_scanlines;

            if (!bm_cache_special) {
               lpcache += 8;
               cache_bytecount -= 8;
            }

#ifdef DEBUG
            if (!bitmap_header_flags(bm_header, chained)) {
               ASSERT((cache_bytecount % bm_header.cache_bytes_wide) == 0,0);
            }
#endif

         }
         //it is a middle section
         else if (cache_bytecount == 0) section_scanlines_left = nth_section_scanlines;
         //if not first or middle it must be the last section
         else {
            section_scanlines_left = last_section_scanlines;
#ifdef DEBUG
            ASSERT((last_section_scanlines * bm_header.cache_bytes_wide) == cache_bytecount,cache_bytecount);
#endif
         }

         //this logic works for 256 color bitmaps also
         //where color is nonzero if 256 color or 16 color
         //if 16 color OR 2 color and scanline correct length then
         //display right out of the cache
         if (bitmap_header_flags(bm_header, color) ||
                                (bm_header.cache_bytes_wide == dib_bytes_wide)) {
            lpsection_dib_bytes = lpcache;
         }
         else {
            //its 2 color and does not have the correct format
            MakeDibFormat((LPBYTE) lpcache, (LPBYTE) lpstatic_buffer,
                           bm_header.cache_bytes_wide, dib_bytes_wide,
                           section_scanlines_left);
            lpsection_dib_bytes = (LPBYTE) lpstatic_buffer;
         }

         //lpbitmapinfo already points at the mostly setup bitmapinfo structure
         //different structure depending on number of colors
         //only thing left to setup is the height


         //NOTE: WIN16 CODE DOES NOT WORK WITH DIBs with negative heights
         //      so we use a negative height in StretchDIBits for the source height
         //       the really weird thing is the ySrc coordinate that we ended up having
         //to use in order for the thing to work.  got it to work by trial and error
         //NOTE:The same code worked for NT and Win3.1 but did not work on windows 95.
         //Because of this we needed to use ifdefs between win16 and win32

         //NOTE: same code now works on all 3 platforms and seperate win32 code
         //did not work on NT with noncopy rops even though it worked on win95
         //so we now have common code for all 3.  Left old code because this is
         //an area where the rules keep on changing


         //jk256 - note that this code should work for 16 color bitmaps
         // as well as 256 color bitmaps
         if (bitmap_header_flags(bm_header, color)) {

//#ifdef WIN32
#if 0

            lpbitmapinfo->bmiHeader.biHeight = - ((LONG) section_scanlines_left);

            //noclip case
            retcode = StretchDIBits(device, xDestULH, yDestULH,
                                    (int) bm_header.pixel_width,
                                    (int) section_scanlines_left,
                                    (int) (((int) bm_header.pixel_offset) +
                                           lpparms->xSrcULH),        //xSrcULH
                                    0,  //ySrcULH - see biHeight comment
                                    (int) bm_header.pixel_width,
                                    (int) section_scanlines_left,
                                    lpsection_dib_bytes,       //pointer to bitmap
                                    lpbitmapinfo,           //pointer to bitmap description
                                    DIB_ColorMode,       // code changes when use palettes
                                    rop3);

            TRACE((TC_TW,TT_TW_BLT,"StretchDIBits return code=%u\n",(UINT) retcode));

            ASSERT(retcode != GDI_ERROR,0);

#else

            lpbitmapinfo->bmiHeader.biHeight = (LONG) section_scanlines_left;

            //noclip case

#ifdef setdibits_bitblt
            if (rop3 == 0x00cc0020) {
               //if srccopy then can use faster operation
               //winbench shows 3 to 10 times faster than StretchDIBits
               retcode = SetDIBitsToDevice(device, xDestULH, yDestULH,
                                    (int) bm_header.pixel_width,  //width of area in dib that are using
                                    (int) section_scanlines_left,
                                    bm_header.pixel_offset + lpparms->xSrcULH, //starting x coord in dib that are using
                                    0,
                                    0,
                                    (UINT) section_scanlines_left,
                                    lpsection_dib_bytes,
                                    lpbitmapinfo,
                                    DIB_ColorMode);
               TRACE((TC_TW,TT_TW_BLT,"SetDIBitsToDevice return code=%u\n",(UINT) retcode));
            }
            else 
#endif
            {
               //note: the following code was found to be 10 - 20% faster
               //than the StretchDIBits code that it replaces
#if 1

               hbmcurrent = CreateDIBitmap(
                           device,
                           (BITMAPINFOHEADER FAR *) lpbitmapinfo,
                           CBM_INIT,
                           lpsection_dib_bytes,
                           lpbitmapinfo,
                           DIB_ColorMode);

               hbmold = SelectObject(compatDC,hbmcurrent);

               jretcode = BitBlt(
                        device,
                        xDestULH,
                        yDestULH,
                        bm_header.pixel_width,
                        section_scanlines_left,
                        compatDC,
                        bm_header.pixel_offset + lpparms->xSrcULH,
                        0,
                        rop3);

               ASSERT(jretcode,0);

               SelectObject(compatDC,hbmold);

               jretcode = DeleteObject(hbmcurrent);

               ASSERT(jretcode,0);


#else
               retcode = StretchDIBits(device, xDestULH, yDestULH,
                                    (int) bm_header.pixel_width,
                                    (int) section_scanlines_left,
                                    (int) (((int) bm_header.pixel_offset) +
                                           lpparms->xSrcULH),        //xSrcULH
                                    0,       //ySrcULH
                                    (int) bm_header.pixel_width,
                                    (int) section_scanlines_left,
                                    lpsection_dib_bytes,       //pointer to bitmap
                                    lpbitmapinfo,           //pointer to bitmap description
                                    DIB_ColorMode,       // code changes when use palettes
                                    rop3);
#endif
            }

            //TRACE((TC_TW,TT_TW_BLT,"StretchDIBits return code=%u\n",(UINT) retcode));
#endif
         }
         else {
            //its 2 color
            hbmcurrent = CreateBitmap(
                           dib_bytes_wide * 8,      //width
                           section_scanlines_left,    //height
                           1,
                           1,
                           lpsection_dib_bytes);

            hbmold = SelectObject(compatDC,hbmcurrent);

            jretcode = BitBlt(
                        device,
                        xDestULH,
                        yDestULH,
                        bm_header.pixel_width,
                        section_scanlines_left,
                        compatDC,
                        bm_header.pixel_offset + lpparms->xSrcULH,
                        0,
                        rop3);

            ASSERT(jretcode,0);

            SelectObject(compatDC,hbmold);

            jretcode = DeleteObject(hbmcurrent);

            ASSERT(jretcode,0);
         }


         yDestULH += section_scanlines_left;

         total_scanlines_left -= section_scanlines_left;

         ASSERT(total_scanlines_left >= 0,0);

         section_count++;

      }
      TRACE((TC_TW,TT_TW_BLT,"BltSrcCommon returning after displaying bitmap noclip or simpleclip subset\n"));

      if (amountPaletteMapCache > 0) {
         PaletteMapCache(amountPaletteMapCache, handlePaletteMapCache);
      }

      //   close DIM file
      if ( jdim ) {
         finishedTWDIMCacheRead();
      }

      return(TRUE);
   }

   //if we fall through here its not the noclip case
   //and not the simple clip case where there was only horizontal clipping

   //the complex clipping code uses dest_rect,src_rect to hold the current clipping
   //rectangle and xSrcULH,ySrcULH to hold the offsets needed to calculate
   //the source clipping rectangle using xDestULH,yDestULH as anchor.

   //we use the same drawing code to handle the simple clipping case
   //by messing with dest_rect,src_rect,xDestULH,yDestULH,xSrcULH,ySrcULH

   //note: we don't set src_rect width and height because same as dest_rect

   cmplx_case = RECTFULL;

   while (cmplx_case != RECTLAST) {
      if (working_case == cmplxclip) {
         cmplxCLPenum((LPINT) &cmplx_case, (LPRECT_ULH) &dest_rect);
         src_rect.left = dest_rect.left - xDestULH + lpparms->xSrcULH;
         src_rect.top  = dest_rect.top  - yDestULH + lpparms->ySrcULH;
         TRACE((TC_TW,TT_TW_BLT+TT_TW_CLIPCMPLX,
               "TW: complex clipping dest left=%u, top=%u, width=%u, height=%u\n",
               dest_rect.left, dest_rect.top, dest_rect.width, dest_rect.height));
         TRACE((TC_TW,TT_TW_BLT+TT_TW_CLIPCMPLX,
               "TW:                 src  left=%u, top=%u\n",
               src_rect.left, src_rect.top));
      }
      else {
         dest_rect.left = xDestULH;
         dest_rect.top  = yDestULH;
         dest_rect.width = lpparms->simpleclip_srcwidth;
         dest_rect.height = lpparms->simpleclip_srcheight;
         src_rect.left = lpparms->xSrcULH;
         src_rect.top  = lpparms->ySrcULH;
         cmplx_case = RECTLAST;     //so only do this once
         TRACE((TC_TW,TT_TW_BLT,"TW: simple clipping dest left=%u, top=%u, width=%u, height=%u\n",
                dest_rect.left, dest_rect.top, dest_rect.width, dest_rect.height));

         TRACE((TC_TW,TT_TW_BLT,"TW:                src  left=%u, top=%u\n",
                src_rect.left, src_rect.top));
      }

      //when we fall through to here we must do the operation
      //for dest_rect and src_rect left and top only

      //number scanlines left to do kept in dest_rect.height
      //section_count will have the current section processing
      //we keep on incrementing both tops

      //start_scanline has the starting scanline # for the current section

      //section_scanlines_left will have the number of scanlines
      //actually processing in this section

      //we initialize section_count to the first relevant section
      //the reason why we don't do this inside the while loop is so we
      //only have to do the section_count calculation once.

      //we know the next section, if there is one is the next one

      //jk256 - remember, first_section_scanlines can be 0
      //        if it a 256 color bitmap wider than 2040
      //code works as is with first_section_scanlines possibly being 0

      if ((UINT) src_rect.top < first_section_scanlines) {
         //its section 0
         section_count = 0;
         start_scanline = src_rect.top;
         section_scanlines_left = first_section_scanlines - start_scanline;
         if ((UINT) dest_rect.height < section_scanlines_left) {
            section_scanlines_left = dest_rect.height;
         }
      }
      else {
         //its not section 0
         //for 2 color: we dont need to get exact count of scanlines if the last section
         //            because the total operation will not exceed the size of the bitmap
         //for 16 color we do need to know the exact scanline count of each section
         //       because the data is stored upside down so when we clip vertically
         //       we may need to adjust some stuff that requires knowing the original
         //       scanline size of the section
         //256 color has same considerations as 16 color

#if 0 //jkfix
         section_count = ( ((src_rect.top + 1) - first_section_scanlines) /
                          nth_section_scanlines ) + 1;
         start_scanline = ((src_rect.top + 1) - first_section_scanlines) %
                          nth_section_scanlines;
#endif
         section_count =  ((src_rect.top - first_section_scanlines) /
                          nth_section_scanlines ) + 1;
         start_scanline = (src_rect.top  - first_section_scanlines) %
                          nth_section_scanlines;
         section_scanlines_left = nth_section_scanlines - start_scanline;

         if ((UINT) dest_rect.height < section_scanlines_left) {
            section_scanlines_left = dest_rect.height;
         }

      }
      while (dest_rect.height > 0) {

         TRACE((TC_TW,TT_TW_BLT,"TW: processing section=%u, start_line=%u, section_lines=%u\n",
                     section_count, start_scanline, section_scanlines_left));

         if ( jdim ) {
            lpcache = lpTWDIMCacheRead(signatureHI, signatureLO, 
                                       (LPUINT) &cache_bytecount, section_count);
            // BUGBUG: dim file read error!
            if ( lpcache == NULL ) {

                // notify host of error
                if ( vpVd ) {
                    CACHE_FILE_HANDLE fh;
    
                    *((PULONG)&fh) = signatureHI;
                    *((PULONG)&fh[4]) = signatureLO;
                    twDIMCacheError( vpVd, fh  );
                }
       
                break;
            }
         }
         else {
            lpcache = lpTWCacheRead((UINT) cache_handle, parm_chunk_type,
                                    (LPUINT) &cache_bytecount, section_count);
         }                         

         //adjust lpcache if first section and has header
         if ( (!section_count) && (!bm_cache_special) ) {
            lpcache += 8;
            cache_bytecount -= 8;
         }

         //if 2 color then adjust lpcache for start_scanline
         //if 16 color then need to figure out total_section_scanlines
         //256 color code the same as 16 color code
         if (!bitmap_header_flags(bm_header, color)) {
            lpcache += start_scanline * bm_header.cache_bytes_wide;
         }
         else {
            //jk256 - 256 color or ...
            //16 color so we need to know the total number of scanlines
            //in the section
            if (!section_count) {
               //its the first section
               total_section_scanlines = first_section_scanlines;
            }
            //it is a middle section
            else if (cache_bytecount == 0) total_section_scanlines = nth_section_scanlines;
            //if not first or middle it must be the last section
            else {
               total_section_scanlines = last_section_scanlines;
            }

         }

         //jk256 - 256 color or ...
         //if 16 color OR 2 color and scanline correct length then
         //display right out of the cache
         if (bitmap_header_flags(bm_header, color) ||
                                (bm_header.cache_bytes_wide == dib_bytes_wide)) {
            lpsection_dib_bytes = lpcache;
            TRACE((TC_TW,TT_TW_BLT,"TW: displaying out of cache\n"));
         }
         else {
            //its 2 color and does not have the correct format
            TRACE((TC_TW,TT_TW_BLT,"TW: converting format before displaying\n"));
            MakeDibFormat((LPBYTE) lpcache, (LPBYTE) lpstatic_buffer,
                           bm_header.cache_bytes_wide, dib_bytes_wide,
                           section_scanlines_left);
            lpsection_dib_bytes = (LPBYTE) lpstatic_buffer;
         }

         //lpbitmapinfo already points at the mostly setup bitmapinfo structure
         //different structure depending on number of colors
         //only thing left to setup is the height


         //NOTE: WIN16 CODE DOES NOT WORK WITH DIBs with negative heights
         //      so we use a negative height in StretchDIBits for the source height
         //       the really weird thing is the ySrc coordinate that we ended up having
         //to use in order for the thing to work.  got it to work by trial and error
         //NOTE:The same code worked for NT and Win3.1 but did not work on windows 95.
         //Because of this we needed to use ifdefs between win16 and win32

         //NOTE: same code now works on all 3 platforms and seperate win32 code
         //did not work on NT with noncopy rops even though it worked on win95
         //so we now have common code for all 3.  Left old code because this is
         //an area where the rules keep on changing

         //jk256 - 256 color and 16 color bitmap path the same

         if (bitmap_header_flags(bm_header, color)) {

            //readjust global bitmap factors to take into account
            //the starting and ending scanline of this section

#ifdef setdibits_bitblt
            if (rop3 == 0x00cc0020) {
               //if srccopy then can use faster operation
               //winbench shows 3 - 10 times faster than StretchDIBits
               lpbitmapinfo->bmiHeader.biHeight = (LONG) total_section_scanlines - start_scanline;
               lpsection_dib_bytes += (lpbitmapinfo->bmiHeader.biHeight - section_scanlines_left) *
                                    dib_bytes_wide;
               retcode = SetDIBitsToDevice(device, dest_rect.left, dest_rect.top,
                                    (int) dest_rect.width,
                                    (int) section_scanlines_left,
                                    bm_header.pixel_offset + src_rect.left,
                                    0,
                                    0,
                                    (UINT) section_scanlines_left,
                                    lpsection_dib_bytes,
                                    lpbitmapinfo,
                                    DIB_ColorMode);
               TRACE((TC_TW,TT_TW_BLT,"SetDIBitsToDevice return code=%u\n",(UINT) retcode));
            }
            else 
#endif
            {
            //note: the following code was found to be 10 - 20% faster
            //than the StretchDIBits code that it replaces
#if 1
               lpbitmapinfo->bmiHeader.biHeight = (LONG) total_section_scanlines - start_scanline;
               hbmcurrent = CreateDIBitmap(
                           device,
                           (BITMAPINFOHEADER FAR *) lpbitmapinfo,
                           CBM_INIT,
                           lpsection_dib_bytes,
                           lpbitmapinfo,
                           DIB_ColorMode);

               hbmold = SelectObject(compatDC,hbmcurrent);

               jretcode = BitBlt(
                        device,
                        dest_rect.left,
                        dest_rect.top,
                        dest_rect.width,
                        section_scanlines_left,
                        compatDC,
                        bm_header.pixel_offset + src_rect.left,
                        0,
                        rop3);

               ASSERT(jretcode,0);

               SelectObject(compatDC,hbmold);

               jretcode = DeleteObject(hbmcurrent);

               ASSERT(jretcode,0);


#else
            lpbitmapinfo->bmiHeader.biHeight = (LONG) total_section_scanlines - start_scanline;
            lpsection_dib_bytes += (lpbitmapinfo->bmiHeader.biHeight - section_scanlines_left) *
                                    dib_bytes_wide;
            retcode = StretchDIBits(device, dest_rect.left, dest_rect.top,
                                    (int) dest_rect.width,
                                    (int) section_scanlines_left,
                                    (int) (((int) bm_header.pixel_offset) +
                                           src_rect.left),           //xSrcULH
                                    0,   //ySrcULH
                                    (int) dest_rect.width,
                                    (int) section_scanlines_left,
                                    lpsection_dib_bytes,       //pointer to bitmap
                                    lpbitmapinfo,           //pointer to bitmap description
                                    DIB_ColorMode,       // code changes when use palettes
                                    rop3);
#endif
            }

            //TRACE((TC_TW,TT_TW_BLT,"StretchDIBits return code=%u\n",(UINT) retcode));

         }

         else {
            //its 2 color
            hbmcurrent = CreateBitmap(
                           dib_bytes_wide * 8,      //width
                           section_scanlines_left,    //height
                           1,
                           1,
                           lpsection_dib_bytes);

            hbmold = SelectObject(compatDC,hbmcurrent);

            jretcode = BitBlt(
                        device,
                        dest_rect.left,
                        dest_rect.top,
                        dest_rect.width,
                        section_scanlines_left,
                        compatDC,
                        bm_header.pixel_offset + src_rect.left,
                        0,
                        rop3);

            ASSERT(jretcode,0);

            SelectObject(compatDC,hbmold);

            jretcode = DeleteObject(hbmcurrent);

            ASSERT(jretcode,0);
         }

         //update the height left
         dest_rect.height -= section_scanlines_left;

         ASSERT(dest_rect.height >=0, 0);

         if (dest_rect.height > 0) {
            //update everything else since we got more to do
            dest_rect.top += section_scanlines_left;

            section_count++;  //go to next section
            start_scanline = 0;  //first scanline of section

            if (nth_section_scanlines < (UINT) dest_rect.height) {
               section_scanlines_left = nth_section_scanlines;
            }
            else {
               //last section
               section_scanlines_left = dest_rect.height;
            }
         }
      }     //height loop for this rectangle

   }        //clipping rectangle loop

   //   close DIM file
   if ( jdim ) {
      finishedTWDIMCacheRead();
   }

   TRACE((TC_TW,TT_TW_BLT,"BltSrcCommon returning after displaying bitmap noclip or simpleclip subset\n"));
   if (amountPaletteMapCache > 0) {
      PaletteMapCache(amountPaletteMapCache, handlePaletteMapCache);
   }

   return(TRUE);

}





//this routine handles the bitblt_source_rop3_noclip packet
//input parm is the device context to use
//device context has the last realized brush
//returns TRUE if no errors
//
//sucks up whatever protocol it needs
/****************************************************************************\
 *  TWCmdBitBltSourceROP3NoClip (BITBLT_SOURCE_ROP3_NOCLIP service routine)
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
void TWCmdBitBltSourceROP3NoClip( HWND hWnd, HDC device )
{
   DSBITMAP_PARMS    parms;
   BOOL  jretcode;

   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_BLT, "TWCmdBitBltSourceROP3NoClip: entered" ));

   parms.packet_case = noclip;

   jretcode = BltSrcCommon( hWnd, device, (LPDSBITMAP_PARMS) &parms);

   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_BLT, "TWCmdBitBltSourceROP3NoClip: EXIT jretcode=%u\n",(UINT) jretcode ));

   TWCmdReturn(jretcode);
}


//this routine handles the bitblt_source_rop3_simpleclip packet
//input parm is the device context to use
//device context has the last realized brush
//returns TRUE if no erros
//
//sucks up whatever protocol it needs
/****************************************************************************\
 *  TWCmdBitBltSourceROP3SimpleClip (BITBLT_SOURCE_ROP3_SIMPLECLIP service routine)
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
void TWCmdBitBltSourceROP3SimpleClip( HWND hWnd, HDC device )
{

   DSBITMAP_PARMS parms;
   BOOL     jretcode;

   WORD     word1,word2;

   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_BLT, "TWCmdBitBltSourceROP3SimpleClip: entered" ));

   Input3Bytes((LPWORD) &word1, (LPWORD) &word2);

   parms.packet_case = simpleclip;

   parms.simpleclip_case = ((word1 & 0x0800) >> 9) | (word2 >> 10);
   parms.simpleclip_srcwidth = (word1 & 0x07ff) + 1;
   parms.simpleclip_srcheight = (word2 & 0x03ff) + 1;

   ASSERT(parms.simpleclip_case < 7,0);

   switch (parms.simpleclip_case) {
   case 0:
      Input3Bytes((LPWORD) &word1, (LPWORD) &word2);
      parms.xSrcULH = word1;
      parms.ySrcULH = word2;
      break;

   case 1:
      GetNextTWCmdBytes((LPWORD) &word1, 2);
      parms.xSrcULH = word1;
      parms.ySrcULH = 0;
      break;

   case 2:
      GetNextTWCmdBytes((LPWORD) &word1, 2);
      parms.xSrcULH = 0;
      parms.ySrcULH = word1;
      break;

   case 3:
      parms.xSrcULH = 0;
      parms.ySrcULH = 0;
      break;

   default:
      break;
   }

   TRACE((TC_TW,TT_TW_BLT,"TW:   case=%u, xSrc=%u, ySrc=%u, width=%u, height=%u\n",
            parms.simpleclip_case, parms.xSrcULH, parms.ySrcULH,
            parms.simpleclip_srcwidth, parms.simpleclip_srcheight));

   jretcode = BltSrcCommon( hWnd, device, (LPDSBITMAP_PARMS) &parms);

   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_BLT, "TWCmdBitBltSourceROP3SimpleClip: EXIT, jretcode=%u\n", (UINT) jretcode ));

   TWCmdReturn(jretcode);
}


//this routine handles the bitblt_source_rop3_cmplxclip packet
//input parm is the device context to use
//device context has the last realized brush
//returns TRUE if no erros
//
//sucks up whatever protocol it needs. BltSrcCommon deals with the
//complex clipping protocol and sucks it up
/****************************************************************************\
 *  TWCmdBitBltSourceROP3CmplxClip (BITBLT_SOURCE_ROP3_CMPLXCLIP service routine)
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
void TWCmdBitBltSourceROP3CmplxClip( HWND hWnd, HDC device )
{

   DSBITMAP_PARMS parms;
   BOOL  jretcode;

   WORD     word1,word2;

   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_BLT, "TWCmdBitBltSourceROP3CmplxClip: entered" ));

   Input3Bytes((LPWORD) &word1, (LPWORD) &word2);

   parms.packet_case = cmplxclip;

   parms.xSrcULH = word1;
   parms.ySrcULH = word2;

   TRACE((TC_TW,TT_TW_BLT,"TW: xSrc=%u, ySrc=%u\n",parms.xSrcULH, parms.ySrcULH));

   jretcode = BltSrcCommon( hWnd, device, (LPDSBITMAP_PARMS) &parms);

   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_BLT, "TWCmdBitBltSourceROP3CmplxClip: EXIT, jretcode=%u\n", (UINT) jretcode ));

   TWCmdReturn(jretcode);
}
