
/*******************************************************************************
*
*   PIXPACK.C
*
*   Thin Wire Windows - Pixel Unpack Routines
*
*   Copyright (c) Citrix Systems Inc. 1995-1996
*
*   Author: Kurt Perry (kurtp) 06-Dec-1995
*
*   $Log$
*  
*     Rev 1.6   15 Apr 1997 18:15:44   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   30 Jan 1996 11:20:06   kurtp
*  update
*  
*     Rev 1.4   03 Jan 1996 13:32:42   kurtp
*  update
*  
*******************************************************************************/

#include <string.h>
#include "wfglobal.h"


#define PACKED_PIXEL_UNPACKED   0x0000
#define PACKED_PIXEL_1BPP       0x1000
#define PACKED_PIXEL_2BPP       0x2000
#define PACKED_PIXEL_4BPP       0x3000
#define PACKED_PIXEL_FREQUENCY  0x4000


#define PACKED_PIXEL_ESC_1        0x0E
#define PACKED_PIXEL_ESC_N        0x0F


#define PACKED_PIXEL_ESC_NIBBLE      4
#define PACKED_PIXEL_ESC_BYTE        8


#define FREQUENCY_MASK            0xF0
#define FREQUENCY_SHIFT              4


//  count of bytes in 256 color clut, 1/2 for 16 color cluts
INT     acbColors[4] = { 2, 4, 16, 14 };
INT     acBits[4]    = { 1, 2,  4,  4 };
BYTE    ashift[4]    = { 7, 6,  4,  4 };
BYTE    amask[4]     = { 0x80, 0xC0, 0xF0, 0xF0 };

#ifdef DEBUG
char * pszFunction[] = {
    "PACKED_PIXEL_UNPACKED",
    "PACKED_PIXEL_1BPP",
    "PACKED_PIXEL_2BPP",
    "PACKED_PIXEL_4BPP",
    "PACKED_PIXEL_FREQUENCY",
};
#endif

/****************************************************************************
 *
 *  UnpackPixels
 *
 *  This is routine unpack pixels from a packed pixel format buffer.
 *  Try and say that three times fast ...
 *
 *  PARAMETERS: who cares, we're going public in a few days ...
 *
 ****************************************************************************/

UINT 
UnpackPixels( LPBYTE inbuf, 
              UINT   cbinbuf, 
              LPBYTE outbuf, 
              WORD   flags, 
              BYTE   color,
              UINT   cbtotalbytes )
{
    INT     i;
    INT     cBits;
    INT     cBitsPerUnit;
    INT     icbinbuf = (INT) cbinbuf;
    INT     repcount;
    ULONG   cNibble = 0;
    INT     cbColors;
    UINT    cboutbuf;
    BYTE    pixel_in;
    BYTE    pixel_out;
    BYTE    pixel_out2;
    BYTE    pixel_mask;
    BYTE    pixel_shift;
    BYTE    pixel_clut[16];
    INT     itype = ((flags & 0x7000) >> 12) - 1;

    ASSERT(vColor == Color_Cap_256,0);

    TRACE((TC_TW,TT_TW_BLT+TT_TW_ENTRY_EXIT,
           "UnpackPixels: %s", pszFunction[(flags&0x7000) >> 12]));
    TRACE((TC_TW,TT_TW_BLT+TT_TW_ENTRY_EXIT,
           "              inbuf %lx, cb %x (%u), outbuf %lx",
           inbuf, icbinbuf, icbinbuf, outbuf));
    TRACE((TC_TW,TT_TW_BLT+TT_TW_ENTRY_EXIT,
           "              flags %x, color %u, cbtotalbytes %x (%u)",
           flags, color, cbtotalbytes, cbtotalbytes));

    TRACEBUF((TC_TW,TT_TW_BLT,
          (char far *)inbuf, 0x10));

    //  initialize vars
    cBitsPerUnit = acBits[itype];
    cbColors     = acbColors[itype]; 
    pixel_mask   = amask[itype]; 
    pixel_shift  = ashift[itype]; 

    //  clear clut
    memset( pixel_clut, 0x00, 16 );

    //  Extrace the clut out of inbuf
    if ( color == 1 ) {
        
        //  half as much for 16 color
        cbColors >>= 1;

        //  get the clut
        for ( i=0; i<cbColors; i++ ) {

            pixel_clut[i<<1]     = (*(inbuf) & 0xf0) >> 4;
            pixel_clut[(i<<1)+1] = *(inbuf++) & 0x0f;
            --icbinbuf;
        }

        //  restore
        cbColors <<= 1;
    }
    else if ( color == 2 ) {

        //  get the clut
        for ( i=0; i<cbColors; i++ ) {

            pixel_clut[i] = *(inbuf++);
            --icbinbuf;
        }
    }

#ifdef DEBUG
    for ( i=0; i<cbColors; i++ ) {
        TRACE((TC_TW,TT_TW_BLT,
               "UnpackPixels: pixel_clut[%u] - %02x", i, pixel_clut[i]));
    }
#endif

    //  counters
    cboutbuf = 0;
    cBits    = 0;
 
    //  get first byte, don't worry there are at least 128 bytes...
    pixel_in = *(inbuf++);
    --icbinbuf;

    //  all but frequency packed
    if ( (flags & 0x7000) != PACKED_PIXEL_FREQUENCY ) {
    
        //  16 color or 256
        if ( color == 1 ) {

            //  process all bytes
            while ( icbinbuf >= 0 ) {
        
                //  fetch look up value
                pixel_out = pixel_clut[(pixel_in & pixel_mask) >> pixel_shift];
        
                //  store byte?
                if ( (cNibble++) % 2 ) {
                    *(outbuf++) = (pixel_out2 | (pixel_out & 0xf));
                    ++cboutbuf;
                }
                else {
                    pixel_out2 = (pixel_out << 4);
                }
        
                //  are we done?
                if ( cboutbuf >= cbtotalbytes ) {
                    break;
                }
                     
                //  how far have we gone?
                cBits += cBitsPerUnit;
        
                //  need more data?
                if ( (cBits + cBitsPerUnit) > 8 ) {
        
                    ASSERT((8 - cBits) == 0, cBits);
        
                    //  get next byte
                    pixel_in = *(inbuf++);
                    --icbinbuf;
        
                    //  start over
                    cBits = 0;
                }
                else {
        
                   //  shift packed pixel 
                   pixel_in <<= cBitsPerUnit;
                }
            }
        }
        else {

            //  process all bytes
            while ( icbinbuf >= 0 ) {
        
                //  store byte
                *(outbuf++) = pixel_clut[(pixel_in & pixel_mask) >> pixel_shift]; 
                ++cboutbuf;
        
                //  are we done?
                if ( cboutbuf >= cbtotalbytes ) {
                    break;
                }
                     
                //  how far have we gone?
                cBits += cBitsPerUnit;
        
                //  need more data?
                if ( (cBits + cBitsPerUnit) > 8 ) {
        
                    ASSERT((8 - cBits) == 0, cBits);
        
                    //  get next byte
                    pixel_in = *(inbuf++);
                    --icbinbuf;
        
                    //  start over
                    cBits = 0;
                }
                else {
        
                   //  shift packed pixel 
                   pixel_in <<= cBitsPerUnit;
                }
            }
        }
    }
    else {

        //  process all bytes
        while ( icbinbuf >= 0 ) {
    
            BYTE bTemp;

            //  fetch look up value
            bTemp = (pixel_in & FREQUENCY_MASK) >> FREQUENCY_SHIFT;

            //  used this nibble
            cBits += PACKED_PIXEL_ESC_NIBBLE;

            //  encoded byte
            if ( bTemp < PACKED_PIXEL_ESC_1 ) {

                //  store byte
                *(outbuf++) = pixel_clut[bTemp]; 
                ++cboutbuf;
            }
            else if ( bTemp == PACKED_PIXEL_ESC_1 ) {

                //  need more data?
                if ( cBits == PACKED_PIXEL_ESC_BYTE ) {
        
                    //  get next byte and just store it
                    *(outbuf++) = *(inbuf++);
                    --icbinbuf;
                    ++cboutbuf;
                }
                else {
        
                    //  shift packed pixel 
                    pixel_in <<= PACKED_PIXEL_ESC_NIBBLE;

                    //  fetch next byte
                    bTemp = *(inbuf++);
                    --icbinbuf;

                    //  create in byte
                    *(outbuf++) = (pixel_in & 0xF0) | ((bTemp & FREQUENCY_MASK) >> FREQUENCY_SHIFT);
                    ++cboutbuf;

                    //  save current byte
                    pixel_in = bTemp;
                }
            }
            else {

                TRACE((TC_TW,TT_TW_BLT,
                       "UnpackPixels: pixel_in1 %02X ", pixel_in));

                //  need more data?
                if ( cBits == PACKED_PIXEL_ESC_BYTE ) {
        
                    //  fetch next byte
                    pixel_in = *(inbuf++);
                    --icbinbuf;
        
                    //  start over
                    cBits = 0;
                }
                else {
        
                    //  shift packed pixel 
                    pixel_in <<= PACKED_PIXEL_ESC_NIBBLE; 
                }

                TRACE((TC_TW,TT_TW_BLT,
                       "UnpackPixels: pixel_in2 %02X ", pixel_in));

                //  used a nibble
                cBits += PACKED_PIXEL_ESC_NIBBLE;

                //  0-based-1 or 0-based-9?
                if ( !(pixel_in & 0x80) ) {

                    //  get count
                    repcount = (INT) (pixel_in >> 4) + 1;
                }
                else {

                    //  need more data?
                    if ( cBits == PACKED_PIXEL_ESC_BYTE ) {
            
                        //  get byte
                        pixel_in = *(inbuf++);
                        --icbinbuf;

                        //  get count
                        repcount = (INT) pixel_in + 9;
                    }
                    else {
    
                        //  get byte
                        bTemp = *(inbuf++);
                        --icbinbuf;

                        //  get count
                        repcount = (INT) (((pixel_in & 0x0F) << 4) | (bTemp >> 4)) + 9;

                        //  save 
                        pixel_in = bTemp;
                    }
                }

                TRACE((TC_TW,TT_TW_BLT,
                       "UnpackPixels: run of %u pixels", repcount));

                TRACEBUF((TC_TW,TT_TW_BLT,
                        inbuf-3, repcount+6));

                //  odd nibble boundary?
                if ( cBits == PACKED_PIXEL_ESC_NIBBLE ) {

                    //  nibble aligned, damn!
                    for ( i=0; i<repcount; i++ ) {

                        //  get next byte
                        bTemp = *(inbuf++);
                        --icbinbuf;

                        //  put out
                        pixel_in <<= 4;
                        *(outbuf++) = pixel_in | (bTemp >> 4);
                        ++cboutbuf;

                        //  save low nibble
                        pixel_in = bTemp;
                    }
                }
                else {

                    //  byte aligned, cool!
                    for ( i=0; i<repcount; ++i ) {

                        //  get next byte and just store it
                        *(outbuf++) = *(inbuf++);
                        --icbinbuf;
                        ++cboutbuf;
                    }
                }
            }

            //  are we done?
            if ( cboutbuf >= cbtotalbytes ) {
                break;
            }
                 
            //  need more data?
            if ( cBits == PACKED_PIXEL_ESC_BYTE ) {
    
                //  fetch next byte
                pixel_in = *(inbuf++);
                --icbinbuf;
    
                //  start over
                cBits = 0;
            }
            else {
    
               //  shift packed pixel 
               pixel_in <<= PACKED_PIXEL_ESC_NIBBLE; 
            }
        }
    }

    TRACE((TC_TW,TT_TW_BLT+TT_TW_ENTRY_EXIT,
           "UnpackPixels: exit - cboutbuf %x (%u)", cboutbuf, cboutbuf));

    //  how much?
    return( cboutbuf );
}
