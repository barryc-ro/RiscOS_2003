
/*****************************************************************************
*
*   WFSSB256.C
*
*   Thin Wire Windows - client save screen bitmap code (256 color)
*
*   Copyright (c) Citrix Systems Inc. 1995-1996
*
*   Author: Kurt Perry (kurtp) 8-Nov-1995
*
*   $Log$
*  
*     Rev 1.7   04 Aug 1997 19:19:50   kurtp
*  update
*  
*     Rev 1.6   15 Apr 1997 18:17:08   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   03 Jan 1996 13:34:28   kurtp
*  update
*  
****************************************************************************/

#include "wfglobal.h"
#include "../../../inc/wdapi.h"


/*=============================================================================
==  Local Vars
=============================================================================*/

typedef struct  _BITMAPINFO_8BPP_SSB
{
   BITMAPINFOHEADER  bmiHeader;
   WORD              bmiColors[256*3];
} BITMAPINFO_8BPP_SSB, near * PBITMAPINFO_8BPP_SSB,
                       far * LPBITMAPINFO_8BPP_SSB;

BITMAPINFO_8BPP_SSB bitmapinfo_8BPP_SSB = {
   {sizeof(BITMAPINFOHEADER),
    0,                           //dynamic biWidth   - width
    0,                           //dynamic biHeight  - height and orientation
    1,                           //fixed   biPlanes
    8,                           //fixed   biBitCount - 4 for 256 colors
    BI_RGB,                      //fixed   biCompression - no compression
    0,                           //fixed   biSizeImage - 0 for no compression
    0,                           //?fixed? biXPelsPerMeter - hopefully unused
    0,                           //?fixed? biYPelsPerMeter - hopefully unused
    0,                           //fixed   biClrUsed  - all colors used
    0                            //fixed   biClrImportant - all colors imp.
   },
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
   21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
   61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
   81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
   101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
   111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
   121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
   131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
   141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
   151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
   161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
   171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
   181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
   191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
   201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
   211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
   221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
   231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
   241, 242, 243, 244, 245, 246, 247, 248, 249, 250,
   251, 252, 253, 254, 255 }
};


/****************************************************************************\
 *  w_TWCmdSSBSaveBitmap8BPP (SSB_SAVE_BITMAP service routine)
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
w_TWCmdSSBSaveBitmap8BPP( HWND hWnd, HDC device )
{
    HBITMAP hbmcompat, hbmold;
 
    SSB_HEADER  ssb_header;
 
    WORD     word1;
 
    UINT  object_handle, chain_handle1, chain_handle2;
 
    int   current_y;        //start at ULH_y and work down (increasing y)
    int   total_scanlines_left;   //in the screen area to save. must be int, not uint
 
    UINT  scanlines_current_view;    //number of scanlines in current compatible bitmap
    UINT  current_block_number = 0;  //current block number processing, 0 based
    UINT  bytes_current_block = 0;   //number of bytes put in current cache block
    UINT  bytes_leftover = 0;        //number of bytes leftover from processing of previous
                                     //block that need to put at beginning of next block
                                     //because scanlines cross block boundaries
 
    LPBYTE lpcache, lpbegincache;
 
    RECT  bounds;
    LPRECT lpoverlaprect;
    INT   coverlaprect;
    PWDRCL prcl;
 
    BOOL  jretcode;
 
    int   iretcode;
 
    TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_SSB, "TWCmdSSBSaveBitmap8BPP: entered" ));
 
    ASSERT(sizeof(ssb_header) == SSB_HEADER_SIZE, 0);
 
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
    ssb_header.byte_width = (ssb_header.pixel_width + 4) & ~0x03;
 
    TRACE((TC_TW,TT_TW_SSB,"TW:   byte_width=%u, object_handle=%u",
                          (UINT) ssb_header.byte_width, object_handle));
 
    //  get the cache size needed
    GetNextTWCmdBytes((LPBYTE)&ssb_header.total_blocks, 2);
    ssb_header.total_blocks++;
 
    //  calculate scanlines in each of the cache block types
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
    hbmcompat = CreateCompatibleBitmap(device, (int) ssb_header.byte_width,
                                               (int) (ssb_header.scanlines_in_blockn + 1) );
    hbmold = SelectObject(compatDC, hbmcompat);
 
    current_y = ssb_header.ULH_y;
    total_scanlines_left = ssb_header.total_scanlines;
 
    lpcache = lpTWCacheWrite(object_handle, _2K, 0, object_handle);      //assume chained
    lpbegincache = lpcache;
 
    TRACE((TC_TW,TT_TW_SSB,"TW:      first cache block pointer=%lx",lpcache));
 
    bitmapinfo_8BPP_SSB.bmiHeader.biWidth = (LONG) ssb_header.byte_width;
 
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
 
          ((LPBYTE) prcl) = lpcache + sizeof(ssb_header);
 
          //do the first rectangle
          prcl->x = lpoverlaprect->left;
          prcl->y = lpoverlaprect->top;
          ((LPBYTE) prcl) += 3;
          prcl->x = lpoverlaprect->right;
          prcl->y = lpoverlaprect->bottom;
 
          TRACE((TC_TW,TT_TW_SSB,"rect 1: left=%u, top=%u, right=%u, bottom=%u",
                (UINT) lpoverlaprect->left, (UINT) lpoverlaprect->top,
                (UINT) lpoverlaprect->right, (UINT) lpoverlaprect->bottom));
 
          if (coverlaprect == 2) {
             //do the second rectanle
             ((LPBYTE) prcl) += 3;
             prcl->x = (lpoverlaprect+1)->left;
             prcl->y = (lpoverlaprect+1)->top;
             ((LPBYTE) prcl) += 3;
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
     *  Now process the cache blocks, 
     *  may be more than number of scanlines when shadowing
     */
//  while (total_scanlines_left > 0) {
    while (ssb_header.total_blocks > current_block_number) {
 
        /*
         *  May not need all the cache blocks when shadowing
         *  a TRUE/HIGH color desktop running 256 color client
         */
        if ( total_scanlines_left > 0 ) {
    
            //need to determine how many scanlines doing for this block
            //also if block 0 then need to stick the header in the cache
            if (current_block_number == 0) {
                scanlines_current_view = ssb_header.scanlines_in_block0;
                memcpy(lpcache , &ssb_header, sizeof(ssb_header));
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
     
            //process whole scanlines directly into the cache in DIB format
            //there will always be at least 1 whole scanline because the widest
            //scanline can be 1280 pixels, which is 1280 bytes
            //so we don't need to handle the case of 0 whole scanlines

            //o.k. process scanlines_current_view
            //copy the bits into the compatible bitmap
            jretcode = BitBlt(compatDC,                              //destination hdc
                              0,                                     //dest x
                              0,                                     //dest y
                              (int) ssb_header.byte_width,           //pixel width
                              (int) scanlines_current_view,          //pixel height
                              device,                                //source hdc
                              (int) ssb_header.ULH_x,                //source x
                              (int) current_y,                       //source y
                              SRCCOPY);
            ASSERT(jretcode, 0);
      
            //put it into DIB format
            //negative height dibs dont work for windows 3.1
            bitmapinfo_8BPP_SSB.bmiHeader.biHeight = (LONG) scanlines_current_view;
      
            //can't do GetDIBits when bitmap selected into the memory DC
            SelectObject(compatDC, hbmold);
      
            iretcode = GetDIBits(compatDC,
                                 hbmcompat,                      //bitmap handle
                                 0,                              //first scanline
                                 (int) scanlines_current_view,   //number of scanlines
                                 lpcache,                        //destination of dibits
                                 (LPBITMAPINFO) &bitmapinfo_8BPP_SSB, //bitmapinfo address
                                 DIB_PAL_COLORS);                //dont have to change to palette when
                                                                 //we optimize rest to palette
            ASSERT((UINT) iretcode == scanlines_current_view, 0);
     
            hbmold = SelectObject(compatDC, hbmcompat);
      
            current_y += scanlines_current_view;
            total_scanlines_left -= scanlines_current_view;
            ASSERT(total_scanlines_left >= 0, 0);
        }
        else {
            scanlines_current_view = 0;
            bytes_current_block = 0;
        }
  
        //we are either ready to process the next cache block or we are done
        //so we need to close off the cache object
  
//      if (total_scanlines_left > 0) {
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
        }
        else {
            TRACE((TC_TW,TT_TW_SSB,"TW: SSBSave END finishing cache write with size=%u",
                              bytes_current_block+(scanlines_current_view *
                              ssb_header.byte_width) ));
            finishedTWCacheWrite(bytes_current_block + (scanlines_current_view *
                                                        ssb_header.byte_width) );
        }
    }     //of while there are scanlines left
 
    SelectObject(compatDC, hbmold);
    DeleteObject(hbmcompat);
 
    TWCmdReturn( TRUE ); // return to NewNTCommand or ResumeNTCommand
}


/****************************************************************************\
 *  w_TWCmdSSBRestoreBitmap8BPP (SSB_RESTORE_BITMAP service routine)
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
w_TWCmdSSBRestoreBitmap8BPP( HWND hWnd, HDC device )
{
    WORD  word1;
 
    SSB_HEADER  ssb_header;
 
    UINT object_handle;
    UINT current_block_number = 0;
    UINT blocksize;
    UINT scanlines_current_view;
 
    UINT  rULH_x,rULH_y;      //has ULH using for the restore
                            //need to keep track of previous ULH seperately so we know
                            //what the pixeloffset value is
 
    INT   total_scanlines_left;      //must be int
    int   current_y;
 
    LPBYTE         lpcache;
    LPBITMAPINFO   lpbitmapinfo;
 
    RECT   bounds, currentcheck, currentintersect;
    LPRECT lpoverlaprect;
    INT    coverlaprect;
    PWDRCL prcl;
    BOOL   jRepaint, jSubset;
    INT    i,j;
    BOOL jretcode;
    HBITMAP  hbmold, hbmcurrent;

 
    TRACE(( TC_TW, TT_TW_ENTRY_EXIT+TT_TW_SSB, "TWCmdSSBRestoreBitmap8BPP: entered" ));
 
    GetNextTWCmdBytes((LPBYTE) &word1, 2);
 
    object_handle = ((UINT) word1) >> 8;
    object_handle |= (UINT) ((word1 & 0x000f) << 8);
 
    lpcache = lpTWCacheRead(object_handle, _2K, (LPUINT) &blocksize, 0);
 
    if (blocksize == 0) {
       blocksize = 2048;
    }
 
    TRACE((TC_TW,TT_TW_SSB,"TW: object_handle=%u, cache pointer=%lx",
                         object_handle, lpcache));
 
    memcpy(&ssb_header, lpcache, sizeof(ssb_header));
 
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
  
        bounds.left   = rULH_x;
        bounds.top    = rULH_y;
        bounds.right  = bounds.left + ssb_header.pixel_width;
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
               ((LPBYTE) prcl) = lpcache - 32 + sizeof(ssb_header);
                i=0;     //rectangle number in ssb_header
                while ((i < (INT) ssb_header.count_ontop) && !jRepaint) {
                    currentcheck.left = (INT) prcl->x;
                    currentcheck.top  = (INT) prcl->y;
                    ((LPBYTE) prcl) += 3;
                    currentcheck.right = (INT) prcl->x;
                    currentcheck.bottom = (INT) prcl->y;
                    ((LPBYTE) prcl) += 3;
     
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
            else {
                jRepaint = TRUE;   //if no overlapping rectangles then repaint
            }
  
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
 
    lpbitmapinfo = (LPBITMAPINFO) &bitmapinfo_8BPP_SSB;

    lpbitmapinfo->bmiHeader.biWidth = (LONG) ssb_header.byte_width;
 
    //if we get here then the whole scanlines in cache blocks
    //optimization case is the one we use
   
    //at top of loop lpcache, current_block_number, current_y, total_scanlines_left
    //blocksize, lpbitmapinfo, width
   
    TRACE((TC_TW,TT_TW_SSB,"TW: optimized SSB restore case"));
   
    while (total_scanlines_left > 0) {
   
        if (current_block_number == 0) {
            scanlines_current_view = ssb_header.scanlines_in_block0;
        }
        else {
            scanlines_current_view = ssb_header.scanlines_in_blockn;
        }
    
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
        //change implementation to use SetDIBitsToDevice because much faster
        SetDIBitsToDevice( device, 
                          (int) rULH_x, 
                          (int) current_y,
                          ssb_header.pixel_width,
                          scanlines_current_view,
                          (int) 0,
                          (int) 0,                //ySrcULH
                          0,
                          scanlines_current_view,
                          lpcache,
                          lpbitmapinfo,
                          DIB_PAL_COLORS);
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
                    DIB_PAL_COLORS);

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

        TRACE((TC_TW,TT_TW_SSB,"TW:   SetDIB(dx %u, dy %u, bw %u, sl %u, sx %u)",
                          (int) rULH_x, 
                          (int) current_y,
                          ssb_header.pixel_width,
                          scanlines_current_view,
                          (int) ssb_header.ULH_x));

        total_scanlines_left -= scanlines_current_view;

        if (total_scanlines_left > 0) {
            current_y += scanlines_current_view;
            current_block_number++;
            lpcache = lpTWCacheRead(object_handle, _2K, (LPUINT) &blocksize,
                                                       current_block_number);
        }
    }

    TWCmdReturn( TRUE ); // return to NewNTCommand or ResumeNTCommand
}
