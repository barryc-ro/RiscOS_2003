/*************************************************************************
*
*  TWDIST
*     Structures for Thinwire Distributed Objects
*
*  Copyright Citrix Systems Inc. 1994
*
*  $Log$
*  
*     Rev 1.2   21 Apr 1997 16:57:38   TOMA
*  update
*  
*     Rev 1.1   06 May 1996 17:02:06   jeffm
*  allow include recursion
*  
*     Rev 1.0   21 Jul 1994 15:31:42   andys
*  Initial revision.
*  
*     Rev 1.0   01 Jul 1994 14:12:08   andys
*  Initial revision.
*
**************************************************************************/

#ifndef __THINDO_H__
#define __THINDO_H__

#define TW_CRC_INITIALVALUE 0xFFFFFFFF
#define TW_MAX_PATH_SAVED  13
#define TW_MAX_DO_COMMENT  80
#define TW_BM_SIGNATURE       (('m'<<8)|'B')
#define TW_DOBFILE_SIGNATURE  (('o'<<8)|'D')

#define DOB_EXTENSION_NODOT "DIM"

// header for each bitmap in a file; bitmap follows header immediately
// next header will be
struct _twdoFileHdr {
unsigned int  doSign;          // signature     'Bm'
unsigned int  doFlags;         // flags (reserved)
char          doSrcFile[TW_MAX_PATH_SAVED+1];
char          doResName[TW_MAX_PATH_SAVED+1];
unsigned int  dobmWidth;       // bitmap width in pixels
unsigned int  dobmHeight;      // bitmap height in pixels
unsigned int  dobmWidthBytes;  // bitmap width in bytes
unsigned char dobmPlanes;      // number of color planes
unsigned char dobmBitsPixel;   // number of adjacent color bits
unsigned long dobmCRC;         // CRC of Bitmap
unsigned long dobmCRCDIB;      // CRC of DIB Bitmap (reserved)
unsigned long dobmSize;        // size of this bitmap entry (hdr + bitmap size)
};
typedef struct _twdoFileHdr TWDOFILEHDR;
typedef struct _twdoFileHdr far * PTWDOFILEHDR;

// header for entire file
struct _twdoMastHdr {
unsigned int  domSign;         // signature     'Do'
unsigned int  domFlags;        // flags (reserved)
unsigned int  domCount;        // number of entries in the file
char domComment[TW_MAX_DO_COMMENT];
};
typedef struct _twdoMastHdr TWDOMASTHDR;
typedef struct _twdoMastHdr far * PTWDOMASTHDR;

/* XLATOFF */
unsigned long far BitMapCRCSegment(unsigned long startcrc,
                                  char far * pbitmap, unsigned bytes);
/* XLATON */


/* ASM

;************************************************************************
;
; CTX_32OPTCRC macro
;
;32optcrc is a 32 bit crc optimized version of opcrc.
;because of a problem with bitmaps we are moving to a 32 bit crc for bitmaps
;we would move to it for everything but because we have shipped and because
;it would mean adding another word to every crc save area array we are
;only using it for bitmaps, which is where it makes sense to need it.
;for NT we will move to a 32 bit crc exclusively.
;
;NOTE: initial value of crc should be ffffffff's not 0's

;on entry:      cx has number of bytes to do crc on
;               es:[di] points to those bytes
;               ebx has the running crc
;on exit:       es:[di] ponts to first byte after last byte crc'd
;               ebx has the running crc
;               ***    cx destroyed
;               ****** eax destroyed
;
;************************************************************************

CTX_32OPTCRC macro
        local  ctx_32optcrc_loop
IFNDEF ctx_32optcrctb
        externD ctx_32optcrctb
ENDIF
ctx_32optcrc_loop label near
        mov     al,es:[di]
        inc     di
;this is the crc engine
;on entry al=byte and ebx=running crc
;on exit eax destroyed and ebx has the running crc
        xor     al,bl
.386
        and     eax,0FFH
        shr     ebx,8
        xor     ebx,ds:ctx_32optcrctb[eax*4]
.286
;end of crc engine
        dec     cx               ;this 2 instruction sequence saves
        jnz     ctx_32optcrc_loop  ;2 cycles over the loop instruction on a 486
        endm

;************************************************************************
;
; CTX_32BYTECRC macro
;
;32bytecrc is a 32 bit crc optimized version of bytecrc
;because of a problem with bitmaps we are moving to a 32 bit crc for bitmaps
;we would move to it for everything but because we have shipped and because
;it would mean adding another word to every crc save area array we are
;only using it for bitmaps, which is where it makes sense to need it.
;for NT we will move to a 32 bit crc exclusively.
;
;NOTE: initial value of crc should be ffffffff's not 0's

;on entry:      al has the byte to do crc on
;               ebx has the running crc
;on exit:
;               ebx has the running crc
;               ****** eax destroyed
;
;************************************************************************

CTX_32BYTECRC macro
IFNDEF   ctx_32optcrctb
        externD ctx_32optcrctb
ENDIF
;this is the crc engine
;on entry al=byte and ebx=running crc
;on exit eax destroyed and ebx has the running crc
        xor     al,bl
.386
        and     eax,0FFH
        shr     ebx,8
        xor     ebx,ds:ctx_32optcrctb[eax*4]
.286
;end of crc engine
        endm

*/
#endif // __THINDO_H__
