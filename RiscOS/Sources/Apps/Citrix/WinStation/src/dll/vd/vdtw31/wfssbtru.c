
/*****************************************************************************
*
*   WFSSBTRU.C
*
*   Thin Wire Windows - client save screen bitmap code (HIGH/TRUE color)
*
*   Copyright (c) Citrix Systems Inc. 1995-1996
*
*   Author: Kurt Perry (kurtp) 10-Nov-1995
*
*   $Log$
*   Revision 1.1  1998/01/19 19:13:09  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.6   04 Aug 1997 19:19:54   kurtp
*  update
*  
*     Rev 1.5   15 Apr 1997 18:17:10   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.4   30 May 1996 16:56:00   jeffm
*  update
*  
*     Rev 1.3   03 Jan 1996 13:34:34   kurtp
*  update
*  
****************************************************************************/

#include <string.h>

#include "wfglobal.h"
#include "../../../inc/wdapi.h"


/*=============================================================================
==  Local Vars
=============================================================================*/

#ifdef notdef
typedef struct  _BITMAPINFO_24BPP_SSB
{
   BITMAPINFOHEADER  bmiHeader;
   WORD              bmiColors[1];
} BITMAPINFO_8BPP_SSB, near * PBITMAPINFO_24BPP_SSB,
                       far * LPBITMAPINFO_24BPP_SSB;

BITMAPINFO_8BPP_SSB bitmapinfo_24BPP_SSB = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
   24,                           //fixed   biBitCount - 24 for True
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {0, 0}
};
#else
BITMAPINFO bitmapinfo_24BPP_SSB = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
   24,                           //fixed   biBitCount - 24 for True
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
};
#endif


/****************************************************************************\
 *  TWCmdSSBSaveBitmap24BPP (SSB_SAVE_BITMAP service routine)
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
w_TWCmdSSBSaveBitmap24BPP( HWND hWnd, HDC device )
{
    HBITMAP hbmcompat, hbmold;
 
    SSB_HEADER  ssb_header;
 
    WORD     word1;
 
    UINT  object_handle, chain_handle1, chain_handle2;
 
    int   current_y;        //start at ULH_y and work down (increasing y)
    int   total_scanlines_left;   //in the screen area to save. must be int, not uint
 
    UINT  scanlines_current_view;    //number of scanlines in current compatible bitmap
    UINT  wholescanlines_current_view;  // like " but only # whole scanlines that can fit
    UINT  current_block_number = 0;  //current block number processing, 0 based
    UINT  bytes_current_block = 0;   //number of bytes put in current cache block
    UINT  bytes_leftover = 0;        //number of bytes leftover from processing of previous
                                     //block that need to put at beginning of next block
                                     //because scanlines cross block boundaries
    LPBYTE lpcache, lptemp, lpbegincache;
    LPBYTE lptempbuffer = (LPBYTE) lpstatic_buffer;
 
    RECT  bounds;
    LPRECT lpoverlaprect;
    INT   coverlaprect;
    PWDRCL prcl;
 
    BOOL  jretcode;
 
    int   iretcode;

    TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_SSB, "TWCmdSSBSaveBitmap24BPP: entered" ));
 
//  ASSERT(sizeof(ssb_header) == SSB_HEADER_SIZE, 0); not true for RISC OS
 
    GetNextTWCmdBytes((LPBYTE) &word1, 2);    //high order byte bits 7-0 of client
                                              //object cache handle
                                              //low order byte - bits 3-0 bits 11-8 of object handle
                                              //5,4 bits 9,8 of 0 based 1 height
    object_handle = (UINT) (word1 >> 8);
    object_handle |= (UINT) ((word1 & 0x000f) << 8);   //object handle setup
 
    ssb_header.total_scanlines = ((word1 & 0x0030) << 4);    //partial
 
    GetNextTWCmdBytes((LPBYTE) &word1, 2);    //high order byte bits 7-0 of ulh_x
                                              //low order byte
                                              //    bits 2,1,0 - 10,9,8 of ulh_x
                                              //    bits 4,3 -   9,8 of ulh_y
                                              //    7,6,5    -   10,9,8 of 0 based 1 width
    ssb_header.ULH_x = word1 >> 8;
    ssb_header.ULH_x |= (word1 & 0x0007) << 8;     //complete
 
    ssb_header.ULH_y = (word1 & 0x0018) << 5;      //partial
 
    ssb_header.pixel_width = (word1 & 0x00e0) << 3;  //partial
 
    GetNextTWCmdBytes((LPBYTE) &ssb_header.ULH_y, 1);
    GetNextTWCmdBytes((LPBYTE) &ssb_header.total_scanlines, 1);
    GetNextTWCmdBytes((LPBYTE) &ssb_header.pixel_width, 1);
 
    ssb_header.total_scanlines++;
    ssb_header.pixel_width++;
 
    TRACE((TC_TW,TT_TW_SSB,"TW: ULH_x=%u, ULH_y=%u, pixel_width=%u, total_scanlines=%u",
                         (UINT) ssb_header.ULH_x, (UINT) ssb_header.ULH_y,
                         (UINT) ssb_header.pixel_width, (UINT) ssb_header.total_scanlines));
 
     //  calculate the scanline byte width
     ssb_header.byte_width = ((ssb_header.pixel_width + 4) & ~0x03) * 3;
 
    TRACE((TC_TW,TT_TW_SSB,"TW:   byte_width=%u, object_handle=%u",
                          (UINT) ssb_header.byte_width, object_handle));
 
    //  get the cache size needed
    GetNextTWCmdBytes((LPBYTE)&ssb_header.total_blocks, 2);
    ssb_header.total_blocks++;
 
    //now we need to figure out whether we can use optimization case 1 where
    //we don't cross scanline boundaries in a cache block
 
    ssb_header.scanlines_in_block0 = (2048 - 32) / ssb_header.byte_width;
    ssb_header.scanlines_in_blockn = 2048 / ssb_header.byte_width;
 
    TRACE((TC_TW,TT_TW_SSB,"TW:   total_blocks=%u, scanlines block 0=%u, n=%u",
             (UINT) ssb_header.total_blocks, (UINT) ssb_header.scanlines_in_block0, (UINT) ssb_header.scanlines_in_blockn));
 
    //do some common processing
 
    if (compatDC == NULL) {
        compatDC = CreateCompatibleDC(device);
    }
 
    //biggest compatible bitmap that might need is scanlines_in_blockn + 1
    //note: bitmap HDC parm must be related to the screen and NOT a memory DC
    hbmcompat = CreateCompatibleBitmap(device, (int) (ssb_header.byte_width / 3),
                                               (int) (ssb_header.scanlines_in_blockn + 1) );
    hbmold = SelectObject(compatDC, hbmcompat);
 
    current_y = ssb_header.ULH_y;
    total_scanlines_left = ssb_header.total_scanlines;
 
    lpcache = lpTWCacheWrite(object_handle, _2K, 0, object_handle);      //assume chained
    lpbegincache = lpcache;
 
    TRACE((TC_TW,TT_TW_SSB,"TW:      first cache block pointer=%lx",lpcache));
 
    bitmapinfo_24BPP_SSB.bmiHeader.biWidth = (LONG) (ssb_header.byte_width / 3);
 
    //this is logic that gets the list of rectangles from our local desktop
    //that are on top of the savescreenbitmap area and saves the information in the
    //cache area
 
    bounds.left = ssb_header.ULH_x;
    bounds.top = ssb_header.ULH_y;
    bounds.right = bounds.left + ssb_header.pixel_width;
    bounds.bottom = bounds.top + ssb_header.total_scanlines;
 
 
    wfnEnumRects(hWnd, device, (LPRECT FAR *) &lpoverlaprect, (LPINT) &coverlaprect,
                   (LPRECT) &bounds);
 
    ssb_header.count_ontop = coverlaprect;
 
    if (coverlaprect > 0) {
        TRACE((TC_TW,TT_TW_SSB,"TW: %u overlapped rectangles on SSB",coverlaprect));
        if ((coverlaprect == 1) || (coverlaprect == 2)) {
            //put the rectangle information in the cache
   
	    prcl = (PWDRCL)(lpcache + SSB_HEADER_SIZE);
   
            //do the first rectangle
            prcl->x = lpoverlaprect->left;
            prcl->y = lpoverlaprect->top;
	    prcl = (PWDRCL)((LPBYTE)prcl + 3);
            prcl->x = lpoverlaprect->right;
            prcl->y = lpoverlaprect->bottom;
   
            TRACE((TC_TW,TT_TW_SSB,"rect 1: left=%u, top=%u, right=%u, bottom=%u",
                  (UINT) lpoverlaprect->left, (UINT) lpoverlaprect->top,
                  (UINT) lpoverlaprect->right, (UINT) lpoverlaprect->bottom));
   
            if (coverlaprect == 2) {
                //do the second rectanle
		prcl = (PWDRCL)((LPBYTE)prcl + 3);
                prcl->x = (lpoverlaprect+1)->left;
                prcl->y = (lpoverlaprect+1)->top;
		prcl = (PWDRCL)((LPBYTE)prcl + 3);
                prcl->x = (lpoverlaprect+1)->right;
                prcl->y = (lpoverlaprect+1)->bottom;
                TRACE((TC_TW,TT_TW_SSB,"rect 2: left=%u, top=%u, right=%u, bottom=%u",
                   (UINT) (lpoverlaprect+1)->left, (UINT) (lpoverlaprect+1)->top,
                   (UINT) (lpoverlaprect+1)->right, (UINT) (lpoverlaprect+1)->bottom));
            }
        }
        wfnFreeRects(lpoverlaprect);
    }
 
    /*
     *  Special case where scanline spans more than one cache block
     */
    if ( (ssb_header.scanlines_in_block0 == 0) &&
         (ssb_header.scanlines_in_blockn == 0) ) {

        UINT  byte_index;

        TRACE((TC_TW,TT_TW_SSB,"TW: very long scanlines, spanning multiple cache blocks"));

        /*
         *  Always read one scanline at a time
         */
        scanlines_current_view = 1;
        memcpy(lpbegincache , &ssb_header, SSB_HEADER_SIZE);
        bytes_current_block = 32;

        /*
         *  Do all scan lines
         */
//      while ( ssb_header.total_blocks > current_block_number ) {
        while (total_scanlines_left > 0) {
     
            /*
             *  Get the screen data into compat bitmap
             */
            jretcode = BitBlt(compatDC,                              //destination hdc
                              0,                                     //dest x
                              0,                                     //dest y
                              (int) (ssb_header.byte_width / 3),     //pixel width
                              (int) scanlines_current_view,          //pixel height
                              device,                                //source hdc
                              (int) ssb_header.ULH_x,                //source x
                              (int) current_y,                       //source y
                              SRCCOPY);
            ASSERT(jretcode, 0);
      
            /*
             *  Put in DIB format negative height dibs don't work for win 3.1
             */
            bitmapinfo_24BPP_SSB.bmiHeader.biHeight = (LONG) scanlines_current_view;
      
            //can't do GetDIBits when bitmap selected into the memory DC
            SelectObject(compatDC, hbmold);
      
            /*
             *  Get the bits
             */
            iretcode = GetDIBits(compatDC,
                                 hbmcompat,                      //bitmap handle
                                 0,                              //first scanline
                                 (int) scanlines_current_view,   //number of scanlines
                                 lptempbuffer,                   //destination of dibits
                                 (LPBITMAPINFO) &bitmapinfo_24BPP_SSB, //bitmapinfo address
                                 DIB_PAL_COLORS);                //dont have to change to palette when
                                                                 //we optimize rest to palette
            ASSERT((UINT) iretcode == scanlines_current_view, 0);
     
            hbmold = SelectObject(compatDC, hbmcompat);

            TRACE((TC_TW,TT_TW_SSB,"TW:   new scanline, current_block_number=%u, current_y=%u",
                           current_block_number, current_y));
     
            current_y += scanlines_current_view;
            total_scanlines_left -= scanlines_current_view;
            bytes_leftover = ssb_header.byte_width;
            byte_index = 0;
  
            /*
             *  Process bytes in scanline, will cross cache block boundary
             */
            while ( bytes_leftover ) {
    
                /*
                 *  More bytes than cache space, or enough cache space
                 */
                if ( bytes_leftover >= (2048 - bytes_current_block) ) {

                    memcpy( lpbegincache + bytes_current_block,
                            lptempbuffer + byte_index,
                            (2048 - bytes_current_block));

                    TRACE((TC_TW,TT_TW_SSB,"TW:   bytes_current_block=%u, bytes_leftover=%u",
                                      bytes_current_block, bytes_leftover));

                    bytes_leftover -= (2048 - bytes_current_block);
                    byte_index     += (2048 - bytes_current_block);
                }
                else {

                    memcpy( lpbegincache + bytes_current_block,
                            lptempbuffer + byte_index,
                            bytes_leftover );

                    TRACE((TC_TW,TT_TW_SSB,"TW:   bytes_current_block=%u, bytes_leftover=%u",
                                      bytes_current_block, bytes_leftover));

                    bytes_current_block += bytes_leftover;
                    byte_index          += bytes_leftover;
                    break;
                }

                //we are either ready to process the next cache block or we are done
                //so we need to close off the cache object
                current_block_number++;
                if (ssb_header.total_blocks > current_block_number) {
    
                    //current_block_number is 0 based and ssb_header.total_blocks is 1 based
                    ASSERT(ssb_header.total_blocks > current_block_number, 0);
          
                    //we need to figure out the next chain handle
                    //if current_block_number is even then chain_handle2 already has the chain handle
                    //if current_block_number is odd then we either have to get the next 1 or 2
                    //chain handles depending on how many blocks are left to get
          
                    if (!(current_block_number & 0x0001)) {
                        //its even
                        lpcache = lpTWCacheWrite(object_handle, _2K, 0,  chain_handle2);
                        TRACE((TC_TW,TT_TW_SSB,"TW:   even, next chain handle=%u", chain_handle2));
                    }
                    else if (ssb_header.total_blocks == (current_block_number + 1)) {
                        //this is the last block and we need to get from datastream
                        GetNextTWCmdBytes((LPBYTE) &word1, 2);
                        chain_handle1 = word1 >> 8;
                        chain_handle1 |= (word1 & 0x000f) << 8;
                        lpcache = lpTWCacheWrite(object_handle, _2K, 0, chain_handle1);
                        TRACE((TC_TW,TT_TW_SSB,"TW:   last, next chain handle=%u", chain_handle1));
                    }
                    else {
                        //there are at least 2 blocks left that we need to get from the datastream
                        GetNextTWCmdBytes((LPBYTE) &word1, 2);
                        chain_handle1 = word1 >> 8;
                        chain_handle1 |= (word1 & 0x000f) << 8;
          
                        chain_handle2 = (word1 & 0x00f0) << 4;
                        GetNextTWCmdBytes((LPBYTE) &chain_handle2, 1);
          
                        lpcache = lpTWCacheWrite(object_handle, _2K, 0, chain_handle1);
                        TRACE((TC_TW,TT_TW_SSB,"TW:   odd, next chain handle=%u", chain_handle1));
                    }

                    TRACE((TC_TW,TT_TW_SSB,"TW:   cache address=%lx", lpcache));
                    lpbegincache = lpcache;
                    bytes_current_block = 0;
                }
            }
        }

        TRACE((TC_TW,TT_TW_SSB,"TW: SSBSave END finishing cache write with size=%u",
                          bytes_current_block));
        finishedTWCacheWrite(bytes_current_block);
    }

    /*
     *  if can't do the optimization then must 0 out scanlines_in_block0
     *  if only 1 block then can do the optimization by definition
     */
    else if ( (ssb_header.total_blocks > 1) && (ssb_header.total_blocks <
                     ( (ssb_header.total_scanlines - ssb_header.scanlines_in_block0) /
                        ssb_header.scanlines_in_blockn + 2 ) ) ) {
        //if we get to here we cannot use optimization 1 so we must pack the data in
        //breaking up the scanlines between cache blocks
  
        //the way we do this is by GetDIBits as many whole scanlines that we can
        //directly into the cache then do a GetDIBits of a single scanline into a temporary
        //buffer and then copy as many bytes as can into the current cache block
        //NOTE: because we had trouble getting the final scanline we GetDIBits of everything
        //we will need into a temporary buffer and then do the appropriate copies
  
        ssb_header.scanlines_in_block0 = 0;    //mark as can't do optimization 1
  
        TRACE((TC_TW,TT_TW_SSB,"TW: cannot do special optimization so some scanlines are broken up"));
  
        //at top of loop bytes_current_block has possible value from bottom of loop
        //should be 0 for first block
        while (total_scanlines_left > 0) {
   
            //do special block 0 processing
            if (current_block_number == 0) {
                ASSERT(bytes_current_block == 0, 0);
                memcpy(lpcache , &ssb_header, SSB_HEADER_SIZE);
                lpcache += 32;    //where to start putting data in the cache
                bytes_current_block = 32;
            }
            //else leave bytes_current_block alone
   
            TRACE((TC_TW,TT_TW_SSB,"TW:   top of while loop, total_scanlines_left=%u",
                  (UINT) total_scanlines_left));
   
            TRACE((TC_TW,TT_TW_SSB,"TW:      current_block_number=%u, current_y=%u, bytes_current_block=%u",
                   current_block_number, current_y, bytes_current_block));
   
            // already done::copy bytes left over from previous scanline (in temporary buffer)
   
            //figure out how many whole scanlines will fit into this block
            wholescanlines_current_view = (2048 - bytes_current_block) / ssb_header.byte_width;
   
            bytes_leftover = 2048 - bytes_current_block - (wholescanlines_current_view *
                                                            ssb_header.byte_width);
            ASSERT(bytes_leftover <= ssb_header.byte_width,0);
   
            //if this is the last block then we may have to reduce wholescanlines
            //and there is no partial line
            if (wholescanlines_current_view >= (UINT) total_scanlines_left) {
                TRACE((TC_TW,TT_TW_SSB,"TW:      detected processing last block"));
                wholescanlines_current_view = total_scanlines_left;
                scanlines_current_view = wholescanlines_current_view;
                bytes_current_block += (wholescanlines_current_view * ssb_header.byte_width);
            }
            //else this is not the last block
            //scanlines_current_view will include the partial scanline that we need to create
            else if (bytes_leftover > 0) {
                scanlines_current_view = wholescanlines_current_view + 1;
                bytes_current_block = 2048 - bytes_leftover;    //# bytes used in current block
            }
            else {
                scanlines_current_view = wholescanlines_current_view;  //no partial scanline this block
                bytes_current_block = 2048;
            }
   
            TRACE((TC_TW,TT_TW_SSB,"TW:      wholescanlines_current_view=%u, scanlines_current_view=%u",
                                 wholescanlines_current_view, scanlines_current_view));
   
            TRACE((TC_TW,TT_TW_SSB,"TW:      bytes_current_block=%u, bytes_leftover=%u",
                                 bytes_current_block, bytes_leftover));
   
            //create compatible bitmap for scanlines_current_view
            jretcode = BitBlt(compatDC,                              //destination hdc
                              0,                                     //dest x
                              0,                                     //dest y
                              (int) (ssb_header.byte_width / 3),     //pixel width
                              (int) scanlines_current_view,          //pixel height
                              device,                                //source hdc
                              (int) (ssb_header.ULH_x),              //source x
                              (int) current_y,                       //source y
                              SRCCOPY);
            ASSERT(jretcode, 0);
   
            //
            //note: we have discovered the following problems with GetDIBits
            //at least on NT - in order to get it to work the size of the dib
            //must be equal to the number of scanlines copied and we need to start
            //from scanline 0
            //therefore we GetDIBits for all the scanlines we will need into a temporary
            //buffer and then we do the appropriate copies unless we have the unlikely
            //occurrence that for this iteration the block ends on a scanline boundary
            //
            //in order to get windows 3.1 to work we cannot use negative height dibs either
   
   
            //can't do GetDIBits when bitmap selected into the memory DC
            SelectObject(compatDC, hbmold);
   
            if (wholescanlines_current_view != scanlines_current_view) {

                //need to do this compare incase this is the last block
                //or in case there is an exact match at the end of the block
                //for this iteration
    
                bitmapinfo_24BPP_SSB.bmiHeader.biHeight = (LONG) scanlines_current_view;
    
                iretcode = GetDIBits(compatDC,
                                  hbmcompat,  //bitmap handle
                                  0,          //first scanline
                                  (int) scanlines_current_view,    //number of scanlines
                                  lptempbuffer,    //destination of dibits
                                  (LPBITMAPINFO) &bitmapinfo_24BPP_SSB,  //bitmapinfo address
                                  DIB_RGB_COLORS);     //dont have to change to palette when
                                                       //we optimize rest to palette
                ASSERT((UINT) iretcode == scanlines_current_view, 0);
    
                memcpy(lpcache,lptempbuffer + ssb_header.byte_width,
                         wholescanlines_current_view * ssb_header.byte_width);
    
    
                //the last scanline of the compatible bitmap needs to get converted
                //and partially moved over
    
                memcpy(lpbegincache+bytes_current_block,
                       lptempbuffer,
                       bytes_leftover);
    
                lptemp = lptempbuffer + bytes_leftover;
                //TRACE((TC_TW,TT_TW_SSB,"overhanging scanline of data:"));
                //TRACEBUF((TC_TW,TT_TW_SSB,lptempbuffer,ssb_header.byte_width));
    
                //figure out how many bytes are leftover in the temp buffer that need to be moved
                //into the next block
                bytes_leftover = ssb_header.byte_width - bytes_leftover;
            }
            else {
                //we can go directly into the cache for this block because
                //everything ends at the right boundary conditions
                bitmapinfo_24BPP_SSB.bmiHeader.biHeight = (LONG) wholescanlines_current_view;
    
                iretcode = GetDIBits(compatDC,
                                  hbmcompat,  //bitmap handle
                                  0,          //first scanline
                                  (int) wholescanlines_current_view,    //number of scanlines
                                  lpcache,    //destination of dibits
                                  (LPBITMAPINFO) &bitmapinfo_24BPP_SSB,  //bitmapinfo address
                                  DIB_RGB_COLORS);     //dont have to change to palette when
                                                       //we optimize rest to palette
                ASSERT((UINT) iretcode == wholescanlines_current_view, 0);
    
                bytes_leftover = 0;
                TRACE((TC_TW,TT_TW_SSB,"TW: wholescanlines_current_view == scanlines_current_view case"));
            }
   
            hbmold = SelectObject(compatDC, hbmcompat);
   
   
            TRACE((TC_TW,TT_TW_SSB,"TW:      number bytes left to copy into next block=%u",
                                    bytes_leftover));
   
            current_y += scanlines_current_view;
            total_scanlines_left -= scanlines_current_view;
            ASSERT(total_scanlines_left >= 0, 0);
   
   
            //we are either ready to process the next cache block or we are done
            //so we need to close off the cache object
   
            //jkfix - can have bytes_leftover when processed all the scanlines
            //need to change matching else to if
            //if (total_scanlines_left > 0) {
            if ((total_scanlines_left > 0) || bytes_leftover) {

                current_block_number++;
                //current_block_number is 0 based and ssb_header.total_blocks is 1 based
                ASSERT(ssb_header.total_blocks > current_block_number, 0);
    
                //we need to figure out the next chain handle
                //if current_block_number is even then chain_handle2 already has the chain handle
                //if current_block_number is odd then we either have to get the next 1 or 2
                //chain handles depending on how many blocks are left to get
    
                if (!(current_block_number & 0x0001)) {
                    //its even
                    lpcache = lpTWCacheWrite(object_handle, _2K, 0,  chain_handle2);
                    TRACE((TC_TW,TT_TW_SSB,"TW:   next chain handle=%u", chain_handle2));
                }
                else if (ssb_header.total_blocks == (current_block_number + 1)) {
                    //this is the last block and we need to get from datastream
                    GetNextTWCmdBytes((LPBYTE) &word1, 2);
                    chain_handle1 = word1 >> 8;
                    chain_handle1 |= (word1 & 0x000f) << 8;
                    lpcache = lpTWCacheWrite(object_handle, _2K, 0, chain_handle1);
                    TRACE((TC_TW,TT_TW_SSB,"TW:   next chain handle=%u", chain_handle1));
                }
                else {
                    //there are at least 2 blocks left that we need to get from the datastream
                    GetNextTWCmdBytes((LPBYTE) &word1, 2);
                    chain_handle1 = word1 >> 8;
                    chain_handle1 |= (word1 & 0x000f) << 8;
    
                    chain_handle2 = (word1 & 0x00f0) << 4;
                    GetNextTWCmdBytes((LPBYTE) &chain_handle2, 1);
    
                    lpcache = lpTWCacheWrite(object_handle, _2K, 0, chain_handle1);
                    TRACE((TC_TW,TT_TW_SSB,"TW:   next chain handle=%u", chain_handle1));
                }
    
                TRACE((TC_TW,TT_TW_SSB,"TW:, cache address=%lx", lpcache));
                lpbegincache = lpcache;
    
                if (bytes_leftover > 0) {
                    //move bytes_leftover into new cache block
                    memcpy(lpcache, lptemp, bytes_leftover);
                    bytes_current_block = bytes_leftover;
                    lpcache += bytes_leftover;          //jkfix
                }
                else {
                    bytes_current_block = 0;
                }
            }
            //jkfix
            //else {
            if (total_scanlines_left == 0) {
                TRACE((TC_TW,TT_TW_SSB,"TW:  SSBSave END finishing cache write with size=%u",
                       bytes_current_block));
                finishedTWCacheWrite(bytes_current_block);
            }
   
        }     //of while scanlines left to do
    }        //of cant use optimization 1
    else {

        //if we get here then everything fits in 1 block so can GetDIBits directly into cache
        //OR multiple blocks but can still move data directly into the cache because
        //scanlines can be kept within a cache block
  
        TRACE((TC_TW,TT_TW_SSB,"TW:   doing special optimization"));
  
        while (total_scanlines_left > 0) {
  
            //need to determine how many scanlines doing for this block
            //also if block 0 then need to stick the header in the cache
            if (current_block_number == 0) {
                scanlines_current_view = ssb_header.scanlines_in_block0;
                memcpy(lpcache , &ssb_header, SSB_HEADER_SIZE);
                lpcache += 32;    //where to start putting data in the cache
                bytes_current_block = 32;
            }
            else {
                scanlines_current_view = ssb_header.scanlines_in_blockn;
                bytes_current_block = 0;
            }

            //bytes_current_block setup so can add amount of data put in and get correct
            //size whether or not its block 0
            //size only computed if its the last block
   
            if (scanlines_current_view > (UINT) total_scanlines_left) {
                scanlines_current_view = total_scanlines_left;
            }
   
            TRACE((TC_TW,TT_TW_SSB,"TW:   top of while loop, total_scanlines_left=%u",
                   (UINT) total_scanlines_left));
   
            TRACE((TC_TW,TT_TW_SSB,"TW:   current_block_number=%u, current_y=%u",
                   current_block_number, current_y));
   
            TRACE((TC_TW,TT_TW_SSB,"TW:   bytes_current_block=%u, scanlines_current_view=%u",
                   bytes_current_block, scanlines_current_view));
   
            //o.k. process scanlines_current_view
            //copy the bits into the compatible bitmap
            jretcode = BitBlt(compatDC,                              //destination hdc
                              0,                                     //dest x
                              0,                                     //dest y
                              (int) (ssb_header.byte_width / 3),     //pixel width
                              (int) scanlines_current_view,          //pixel height
                              device,                                //source hdc
                              (int) (ssb_header.ULH_x),              //source x
                              (int) current_y,                       //source y
                              SRCCOPY);
            ASSERT(jretcode, 0);
   
            //put it into DIB format
            //negative height dibs dont work for windows 3.1
            bitmapinfo_24BPP_SSB.bmiHeader.biHeight = (LONG) scanlines_current_view;
   
            //can't do GetDIBits when bitmap selected into the memory DC
            SelectObject(compatDC, hbmold);
   
            iretcode = GetDIBits(compatDC,
                                 hbmcompat,  //bitmap handle
                                 0,          //first scanline
                                 (int) scanlines_current_view,    //number of scanlines
                                 lpcache,    //destination of dibits
                                 (LPBITMAPINFO) &bitmapinfo_24BPP_SSB,  //bitmapinfo address
                                 DIB_RGB_COLORS);     //dont have to change to palette when
                                                      //we optimize rest to palette
            ASSERT((UINT) iretcode == scanlines_current_view, 0);
   
            hbmold = SelectObject(compatDC, hbmcompat);
   
            current_y += scanlines_current_view;
            total_scanlines_left -= scanlines_current_view;
            ASSERT(total_scanlines_left >= 0, 0);
   
   
            //we are either ready to process the next cache block or we are done
            //so we need to close off the cache object
   
            if (total_scanlines_left > 0) {
                current_block_number++;
                //current_block_number is 0 based and ssb_header.total_blocks is 1 based
                ASSERT(ssb_header.total_blocks > current_block_number, 0);
    
                //we need to figure out the next chain handle
                //if current_block_number is even then chain_handle2 already has the chain handle
                //if current_block_number is odd then we either have to get the next 1 or 2
                //chain handles depending on how many blocks are left to get
    
                if (!(current_block_number & 0x0001)) {
                    //its even
                    lpcache = lpTWCacheWrite(object_handle, _2K, 0,  chain_handle2);
                    TRACE((TC_TW,TT_TW_SSB,"TW:   next chain handle=%u", chain_handle2));
                }
                else if (ssb_header.total_blocks == (current_block_number + 1)) {
                    //this is the last block and we need to get from datastream
                    GetNextTWCmdBytes((LPBYTE) &word1, 2);
                    chain_handle1 = word1 >> 8;
                    chain_handle1 |= (word1 & 0x000f) << 8;
                    lpcache = lpTWCacheWrite(object_handle, _2K, 0, chain_handle1);
                    TRACE((TC_TW,TT_TW_SSB,"TW:   next chain handle=%u", chain_handle1));
                }
                else {
                    //there are at least 2 blocks left that we need to get from the datastream
                    GetNextTWCmdBytes((LPBYTE) &word1, 2);
                    chain_handle1 = word1 >> 8;
                    chain_handle1 |= (word1 & 0x000f) << 8;
     
                    chain_handle2 = (word1 & 0x00f0) << 4;
                    GetNextTWCmdBytes((LPBYTE) &chain_handle2, 1);
    
                    lpcache = lpTWCacheWrite(object_handle, _2K, 0, chain_handle1);
                    TRACE((TC_TW,TT_TW_SSB,"TW:   next chain handle=%u", chain_handle1));
                }
            }
            else {
                TRACE((TC_TW,TT_TW_SSB,"TW: SSBSave END finishing cache write with size=%u",
                       bytes_current_block+(scanlines_current_view *
                       ssb_header.byte_width) ));
                finishedTWCacheWrite( bytes_current_block + 
                                     (scanlines_current_view *
                                      ssb_header.byte_width) );
            }
        }     //of while there are scanlines left
    }       //of can use optimization 1
 
    SelectObject(compatDC, hbmold);
    DeleteObject(hbmcompat);
 
    TWCmdReturn( TRUE ); // return to NewNTCommand or ResumeNTCommand
}


/****************************************************************************\
 *  TWCmdSSBRestoreBitmap24BPP (SSB_RESTORE_BITMAP service routine)
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
w_TWCmdSSBRestoreBitmap24BPP( HWND hWnd, HDC device )
{

   WORD  word1;

   SSB_HEADER  ssb_header;

   UINT object_handle;
   UINT current_block_number = 0;
   UINT blocksize, remainingsize;
   UINT scanlines_current_view;
   UINT wholescanlines_current_view;

   UINT  rULH_x,rULH_y;      //has ULH using for the restore
                           //need to keep track of previous ULH seperately so we know
                           //what the pixeloffset value is

   INT   total_scanlines_left;      //must be int
   int   current_y;

   LPBYTE lpcache, lpleftover;
   LPBYTE lpbegincache;
   LPBYTE lptempbuffer = (LPBYTE) lpstatic_buffer;
   LPBITMAPINFO   lpbitmapinfo;

   RECT  bounds, currentcheck, currentintersect;
   LPRECT lpoverlaprect;
   INT   coverlaprect;
   PWDRCL prcl;
   BOOL  jRepaint, jSubset;
   INT   i,j;
   BOOL jretcode;
   HBITMAP  hbmold, hbmcurrent;

   TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_SSB, "TWCmdSSBRestoreBitmap24BPP: entered" ));

   GetNextTWCmdBytes((LPBYTE) &word1, 2);

   object_handle = ((UINT) word1) >> 8;
   object_handle |= (UINT) ((word1 & 0x000f) << 8);

   lpcache = lpTWCacheRead(object_handle, _2K, (LPUINT) &blocksize, 0);
   lpbegincache = lpcache;

   if (blocksize == 0) {
      blocksize = 2048;
   }

   TRACE((TC_TW,TT_TW_SSB,"TW: object_handle=%u, cache pointer=%lx",
                        object_handle, lpcache));

   memcpy(&ssb_header, lpcache, SSB_HEADER_SIZE);

   lpcache += 32;
   blocksize -= 32;

   if (word1 & 0x0080) {
      TRACE((TC_TW,TT_TW_SSB,"TW:   CHANGE ULH COORDINATES FROM CACHED ULH"));
      GetNextTWCmdBytes((LPBYTE) &word1, 2);
      rULH_x = word1 >> 8;
      rULH_x |= (word1 & 0x0007) << 8;

      rULH_y = (word1 & 0x0018) << 5;
      GetNextTWCmdBytes((LPBYTE) &rULH_y, 1);
   }
   else {
      TRACE((TC_TW,TT_TW_SSB,"TW:   USE CACHED ULH COORDINATES"));
      rULH_x = ssb_header.ULH_x;
      rULH_y = ssb_header.ULH_y;
   }

   //see if need to do a repaint of the ssb rectangle
   //if need to do a repaint of the ssb rectangle then don't need to bother
   //retrieving the data out of the cache
   if (ssb_header.count_ontop > 0) {
      //need to worry about on top rectangles
      TRACE((TC_TW,TT_TW_SSB,"TW: %u previous on top rectangles for restore",
                             (UINT) ssb_header.count_ontop));
      jRepaint = FALSE;

      bounds.left = rULH_x;
      bounds.top = rULH_y;
      bounds.right = bounds.left + ssb_header.pixel_width;
      bounds.bottom = bounds.top + ssb_header.total_scanlines;

      if ((ssb_header.count_ontop > 2) || (rULH_x != ssb_header.ULH_x)
                                       || (rULH_y != ssb_header.ULH_y)) {
         jRepaint = TRUE;
      }
      else {
         //1 or 2 previous rectangles, check against current on top rectangles


         wfnEnumRects(hWnd, device, (LPRECT FAR *) &lpoverlaprect, (LPINT) &coverlaprect,
                        (LPRECT) &bounds);
         if (coverlaprect) {
	    prcl = (PWDRCL)(lpcache - 32 + SSB_HEADER_SIZE);
            i=0;     //rectangle number in ssb_header
            while ((i < (INT) ssb_header.count_ontop) && !jRepaint) {
               currentcheck.left = (INT) prcl->x;
               currentcheck.top  = (INT) prcl->y;
	       prcl = (PWDRCL)((LPBYTE)prcl + 3);
               currentcheck.right = (INT) prcl->x;
               currentcheck.bottom = (INT) prcl->y;
	       prcl = (PWDRCL)((LPBYTE)prcl + 3);

               TRACE((TC_TW,TT_TW_SSB,"currentcheck(%u) left=%u, top=%u, right=%u, bottom=%u",
                        i,(UINT) currentcheck.left, (UINT) currentcheck.top,
                        (UINT) currentcheck.right, (UINT) currentcheck.bottom));
               i++;

               jSubset = FALSE;
               j=0;
               while ((j < coverlaprect) && !jSubset) {
                  if (IntersectRect(&currentintersect,lpoverlaprect+j,&currentcheck)) {
                     TRACE((TC_TW,TT_TW_SSB,"intersect9%u) left=%u, top=%u, right=%u, bottom=%u",
                         j, (UINT) currentintersect.left, (UINT) currentintersect.top,
                         (UINT) currentintersect.right, (UINT) currentintersect.bottom));

                     if ((currentintersect.left == currentcheck.left) &&
                         (currentintersect.top  == currentcheck.top ) &&
                         (currentintersect.right == currentcheck.right) &&
                         (currentintersect.bottom == currentcheck.bottom)  ) {
                        jSubset = TRUE;
                     }
                  }
                  j++;
               }
               if (!jSubset) {
                  jRepaint = TRUE;
               }
            }
         }
         else jRepaint = TRUE;   //if no overlapping rectangles then repaint

         wfnFreeRects(lpoverlaprect);
      }

      if (jRepaint) {
         wfnRepaintRects((LPRECT) &bounds, 1, FALSE);
         TWCmdReturn( TRUE ); // return to NewNTCommand or ResumeNTCommand
      }
   }


   TRACE((TC_TW,TT_TW_SSB,"TW:   ULH_x=%u, ULH_y=%u, total_scanlines=%u, pixel_width=%u",
         (UINT) ssb_header.ULH_x, (UINT) ssb_header.ULH_y,
         (UINT) ssb_header.total_scanlines, (UINT) ssb_header.pixel_width));
   TRACE((TC_TW,TT_TW_SSB,"TW:   byte_width=%u, total_blocks=%u, scanlines block 0=%u, n=%u",
         (UINT) ssb_header.byte_width, (UINT) ssb_header.total_blocks,
         (UINT) ssb_header.scanlines_in_block0, (INT) ssb_header.scanlines_in_blockn));
   TRACE((TC_TW,TT_TW_SSB,"TW:   rULH_x=%u, rULH_y=%u",rULH_x,rULH_y));

   total_scanlines_left = ssb_header.total_scanlines;
   current_y = rULH_y;

   //the reason for this pathetically weird code is that on windows 95
   //the dark and light gray colors are reversed unless we reuse the color
   //table that was used when we got the bits on the save
   lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_24BPP_SSB;

   lpbitmapinfo->bmiHeader.biWidth = (LONG) (ssb_header.byte_width / 3);

   if ( (ssb_header.scanlines_in_block0 == 0) && 
        (ssb_header.scanlines_in_blockn == 0) ) {

        UINT byte_index;
        UINT bytes_current_block = 32;

        TRACE((TC_TW,TT_TW_SSB,"TW: very long scanlines, spanning multiple cache blocks"));

        /*
         *  Always read one scanline at a time
         */
        scanlines_current_view = 1;
        TRACE((TC_TW,TT_TW_SSB,"TW:   first cache address=%lx", lpbegincache));
        TRACE((TC_TW,TT_TW_SSB,"TW:   first scanline, current_block_number=%u, current_y=%u",
                       current_block_number, current_y));

        /*
         *  Do all scan lines
         */
//      while ( ssb_header.total_blocks > current_block_number ) {
        while (total_scanlines_left > 0) {
     
            /*
             *  How many bytes in scanline
             */
            remainingsize = ssb_header.byte_width;
            byte_index    = 0;
  
            /*
             *  Process bytes in scanline, will cross cache block boundary
             */
            while ( remainingsize ) {

                UINT buffer_remain = (2048 - bytes_current_block);

                TRACE((TC_TW,TT_TW_SSB,"TW:  buffer_remain=%u", buffer_remain));
    
                /*
                 *  More bytes than cache space, or enough cache space
                 */
                if ( remainingsize >= buffer_remain ) {

                    memcpy( lptempbuffer + byte_index,
                            lpbegincache + bytes_current_block,
                            buffer_remain );

                    TRACE((TC_TW,TT_TW_SSB,"TW:  1. bytes_current_block=%u, remainingsize=%u",
                                      bytes_current_block, remainingsize));

                    remainingsize -= buffer_remain;
                    byte_index    += buffer_remain;
                }
                else {

                    memcpy( lptempbuffer + byte_index,
                            lpbegincache + bytes_current_block,
                            remainingsize );

                    TRACE((TC_TW,TT_TW_SSB,"TW:   2. bytes_current_block=%u, remainingsize=%u",
                                      bytes_current_block, remainingsize));

                    bytes_current_block += remainingsize;
                    break;
                }

                current_block_number++;
                if (ssb_header.total_blocks > current_block_number) {
                    bytes_current_block = 0;
                    blocksize = 2048;
                    lpcache = lpTWCacheRead(object_handle, _2K, (LPUINT) &blocksize,
                                                               current_block_number);
                    lpbegincache = lpcache;
                    TRACE((TC_TW,TT_TW_SSB,"TW:   cache address=%lx", lpcache));
                }
            }

            //cannot use negative height dibs for windows 3.1 support
            lpbitmapinfo->bmiHeader.biHeight =  (LONG) scanlines_current_view;
   
#ifdef setdibits_ssb
            SetDIBitsToDevice(device, 
                              (int) rULH_x, 
                              (int) current_y,
                              ssb_header.pixel_width,
                              scanlines_current_view,
                              (int) 0,
                              (int) 0,
                              0,
                              scanlines_current_view,
                              lptempbuffer,
                              lpbitmapinfo,
                              DIB_RGB_COLORS);
#else
        /*
         *  wkp: 7/25/97
         *
         *  This code is faster than SetDIBits on slow video hardware
         *  by a factor of ~ 2:1, has no effect on fast video.
         */

        hbmcurrent = CreateDIBitmap(
                    device,
                    (BITMAPINFOHEADER FAR *) lpbitmapinfo,
                    CBM_INIT,
                    lptempbuffer,
                    lpbitmapinfo,
                    DIB_RGB_COLORS);

        hbmold = SelectObject(compatDC,hbmcurrent);

        jretcode = BitBlt(
                 device,
                 (int) rULH_x, (int) current_y,
                 ssb_header.pixel_width,
                 scanlines_current_view,
                 compatDC,
                 (int) 0,
                 (int) 0,                      //ySrcULH
                 SRCCOPY);

        ASSERT(jretcode,0);

        SelectObject(compatDC,hbmold);

        jretcode = DeleteObject(hbmcurrent);

        ASSERT(jretcode,0);
#endif
   
            TRACE((TC_TW,TT_TW_SSB,"TW:   new scanline, current_block_number=%u, current_y=%u",
                           current_block_number, current_y));

            current_y += scanlines_current_view;
            total_scanlines_left -= scanlines_current_view;
        }
   }
   else if (ssb_header.scanlines_in_block0) {
      //if we get here then the whole scanlines in cache blocks
      //optimization case is the one we use

      //at top of loop lpcache, current_block_number, current_y, total_scanlines_left
      //blocksize, lpbitmapinfo, width

      TRACE((TC_TW,TT_TW_SSB,"TW: optimized SSB restore case"));

      while (total_scanlines_left > 0) {

         if (current_block_number == 0) {
            scanlines_current_view = ssb_header.scanlines_in_block0;
         }
         else scanlines_current_view = ssb_header.scanlines_in_blockn;

         if (scanlines_current_view > (UINT) total_scanlines_left) {
            scanlines_current_view = total_scanlines_left;
         }

         TRACE((TC_TW,TT_TW_SSB,"TW: scanlines_current_view=%u, total_scanlines_left=%u",
                                 scanlines_current_view, total_scanlines_left));
         TRACE((TC_TW,TT_TW_SSB,"TW: current_block_number=%u, current_y=%u",
                              current_block_number, current_y));

         //cannot use negative height dibs for windows 3.1 support
         lpbitmapinfo->bmiHeader.biHeight =  (LONG) scanlines_current_view;

#ifdef setdibits_ssb
         SetDIBitsToDevice(device, 
                           (int) rULH_x, 
                           (int) current_y,
                           ssb_header.pixel_width,
                           scanlines_current_view,
                           (int) 0,
                           (int) 0,                      //ySrcULH
                           0,
                           scanlines_current_view,
                           lpcache,
                           lpbitmapinfo,
                           DIB_RGB_COLORS);
#else
        /*
         *  wkp: 7/25/97
         *
         *  This code is faster than SetDIBits on slow video hardware
         *  by a factor of ~ 2:1, has no effect on fast video.
         */

        hbmcurrent = CreateDIBitmap(
                    device,
                    (BITMAPINFOHEADER FAR *) lpbitmapinfo,
                    CBM_INIT,
                    lpcache,
                    lpbitmapinfo,
                    DIB_RGB_COLORS);

        hbmold = SelectObject(compatDC,hbmcurrent);

        jretcode = BitBlt(
                 device,
                 (int) rULH_x, (int) current_y,
                 ssb_header.pixel_width,
                 scanlines_current_view,
                 compatDC,
                 (int) 0,
                 (int) 0,                      //ySrcULH
                 SRCCOPY);

        ASSERT(jretcode,0);

        SelectObject(compatDC,hbmold);

        jretcode = DeleteObject(hbmcurrent);

        ASSERT(jretcode,0);
#endif

         total_scanlines_left -= scanlines_current_view;

         if (total_scanlines_left > 0) {
            current_y += scanlines_current_view;
            current_block_number++;
            lpcache = lpTWCacheRead(object_handle, _2K, (LPUINT) &blocksize,
                                                       current_block_number);
         }

      }

   }
   else {
      //the last scanline in the block crosses a cache block boundary
      //at top of loop lpcache, current_block_number, current_y, total_scanlines_left
      //blocksize, lpbitmapinfo, width

      TRACE((TC_TW,TT_TW_SSB,"TW: complicated SSB restore case"));

      while(total_scanlines_left > 0) {

         wholescanlines_current_view = blocksize / ssb_header.byte_width;

         blocksize -= wholescanlines_current_view * ssb_header.byte_width;

         lpleftover = lpcache + (wholescanlines_current_view * ssb_header.byte_width);


         TRACE((TC_TW,TT_TW_SSB,"TW: wholescanlines_current_view=%u, total_scanlines_left=%u",
                                 wholescanlines_current_view, total_scanlines_left));
         TRACE((TC_TW,TT_TW_SSB,"TW: current_block_number=%u, current_y=%u",
                              current_block_number, current_y));
         TRACE((TC_TW,TT_TW_SSB,"TW: bytes left in current block=%u",blocksize));

         TRACE((TC_TW,TT_TW_SSB,"TW: lpcache=%lx, lpleftover=%lx",lpcache,lpleftover));

         //cannot use negative height dibs for windows 3.1
         lpbitmapinfo->bmiHeader.biHeight = (LONG) wholescanlines_current_view;

#ifdef setdibits_ssb
         SetDIBitsToDevice(device, 
                           (int) rULH_x, 
                           (int) current_y,
                           ssb_header.pixel_width,
                           wholescanlines_current_view,
                           (int) 0,
                           (int) 0,                      //ySrcULH
                           0,
                           wholescanlines_current_view,
                           lpcache,
                           lpbitmapinfo,
                           DIB_RGB_COLORS);
#else
        /*
         *  wkp: 7/25/97
         *
         *  This code is faster than SetDIBits on slow video hardware
         *  by a factor of ~ 2:1, has no effect on fast video.
         */

        hbmcurrent = CreateDIBitmap(
                    device,
                    (BITMAPINFOHEADER FAR *) lpbitmapinfo,
                    CBM_INIT,
                    lpcache,
                    lpbitmapinfo,
                    DIB_RGB_COLORS);

        hbmold = SelectObject(compatDC,hbmcurrent);

        jretcode = BitBlt(
                 device,
                 (int) rULH_x, (int) current_y,
                 ssb_header.pixel_width,
                 wholescanlines_current_view,
                 compatDC,
                 (int) 0,
                 (int) 0,                      //ySrcULH
                 SRCCOPY);

        ASSERT(jretcode,0);

        SelectObject(compatDC,hbmold);

        jretcode = DeleteObject(hbmcurrent);

        ASSERT(jretcode,0);
#endif

         total_scanlines_left -= wholescanlines_current_view;

         if (total_scanlines_left > 0) {
            current_y += wholescanlines_current_view;
            current_block_number++;

            if (blocksize == 0) {
               //nothing left over to process from previous cache block
               lpcache = lpTWCacheRead(object_handle, _2K, (LPUINT) &blocksize,
                                                       current_block_number);
               if (blocksize == 0) {
                  blocksize = 2048;
               }
            }
            else {
               //we have blocksize amount of data to process from previous block
               //pointed by by lpleftover.
               //we then have to get the next block and fill in the scan line

               remainingsize = ssb_header.byte_width - blocksize;
               memcpy(lptempbuffer, lpleftover, blocksize);

               lpleftover = lptempbuffer + blocksize;

               lpcache = lpTWCacheRead(object_handle, _2K, (LPUINT) &blocksize,
                                                     current_block_number);
               if (blocksize == 0) {
                  blocksize = 2048;
               }

               memcpy(lpleftover, lpcache, remainingsize);
               blocksize -= remainingsize;
               lpcache += remainingsize;

               //TRACE((TC_TW,TT_TW_SSB,"overhanging scanline of data"));
               //TRACEBUF((TC_TW,TT_TW_SSB,lptempbuffer,ssb_header.byte_width));


               //cannot use negative height dibs for windows 3.1
               lpbitmapinfo->bmiHeader.biHeight = (LONG) 1;

#ifdef setdibits_ssb
               SetDIBitsToDevice(device, 
                                 (int) rULH_x, 
                                 (int) current_y,
                                 ssb_header.pixel_width,
                                 1,
                                 (int) 0,
                                 (int) 0,                      //ySrcULH
                                 0,
                                 1,
                                 lptempbuffer,
                                 lpbitmapinfo,
                                 DIB_RGB_COLORS);
#else
               /*
                *  wkp: 7/25/97
                *
                *  This code is faster than SetDIBits on slow video hardware
                *  by a factor of ~ 2:1, has no effect on fast video.
                */
       
               hbmcurrent = CreateDIBitmap(
                           device,
                           (BITMAPINFOHEADER FAR *) lpbitmapinfo,
                           CBM_INIT,
                           lptempbuffer,
                           lpbitmapinfo,
                           DIB_RGB_COLORS);
       
               hbmold = SelectObject(compatDC,hbmcurrent);
       
               jretcode = BitBlt(
                        device,
                        (int) rULH_x, (int) current_y,
                        ssb_header.pixel_width,
                        1,
                        compatDC,
                        (int) 0,
                        (int) 0,                      //ySrcULH
                        SRCCOPY);
       
               ASSERT(jretcode,0);
       
               SelectObject(compatDC,hbmold);
       
               jretcode = DeleteObject(hbmcurrent);
       
               ASSERT(jretcode,0);
#endif
      
               total_scanlines_left -= 1;

               if (total_scanlines_left > 0) {
                  current_y += 1;
               }
               else ASSERT(blocksize == 0,0);
            }

         }
         else ASSERT(blocksize == 0,0);

      }  //while scanlines>0
   }     // complex case else group
   TWCmdReturn( TRUE ); // return to NewNTCommand or ResumeNTCommand
}

