/***************************************************************************
 *
 *  File:        bits.c
 *
 *  Description: MPEG Audio decoder bitstream interface
 *               word version
 *
 *  Version:     0.0 29 Jan 1998
 *
 *  Author:      Pete Goodliffe (pgoodliffe@acorn.com)
 *
 *  Modification History:
 *
 *    Date         Name                  Description
 *    ----------------------------------------------------------------------
 *    10 Dec 1997  Pete Goodliffe        Fiddled away from files into buffers
 *
 ***************************************************************************/

////////////////////////////////////////////////////////////////////////////
// See comments in mpadec.c and bits.c
// This version of the bitstream input routines works by reading a word at
//   a time. This should be quicker, however since the data is not
//   neccessarily a multiple of four bytes, it might read unset or protected
//   memory at the end of the buffer.
//   This probably wouldn't cause RISC OS to fall over, but for sweet dreams
//   its been disabled and left here for reference.
//
// Pete Goodliffe (29 Jan 1998)

/* bits.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpa.h"
#include "bits.h"
#include "use_asm.h"


////////////////////////////////////////////////////////////////////////////
// the bitstream structure and associated data types

typedef uint32 bufelem;		/* what the buffer is built from */
#define EBITS 32			/* FIXED */

// the old struct for complicated useage
//
//typedef struct
//{
//    int      bitoffset;		/* # bits not yet read in current element */
//    bufelem *bcur;		/* -> current element in buffer */
//    bufelem *buf_lim;		/* -> end of buffer */
//    int      buf_size;		/* size of buffer (in bufelems) */
//    bufelem *buf;		/* bit stream buffer base */
//    int      buf_offset;	/* byte offset in stream of start of buffer  */
//    int      eobs;		/* all bits in stream have been read */
////    FILE    *pt;		/* pointer to bit stream device */
//} bitstream;

typedef struct {
    bufelem *buf;     // pointer to buffer holding bitstream
    bufelem *bcur;    // pointer to current bufelem in it
    bufelem *buf_lim; // pointer to end of buffer
    bufelem  bcur_be; // bcur in big endian format
    int buf_offset;   // bytes offset in stream of start of buffer
    int bitoffset;    // next bit number in bcur
} bitstream;

// macro to twiddle bufelem into big endian format
//     asumming 4 chars per bufelem
#define BEND(r) \
        (r << 24 | r >> 24 | (r & 0x00FF0000) >> 8 | (r & 0x0000FF00) << 8)

bitstream the_bs;

////////////////////////////////////////////////////////////////////////////
// open_bit_stream

void mpeg_set_bit_stream (char *buffer, int offset, int bufferlen) {

    bitstream *bs = &the_bs;
    if (((int)buffer)%4) { // buffer isn't on word boundary: arse
        int disp = ((int)buffer)%4;
//        void tortoise(void);
//        printf("non aligned adress (disp=%i), so old stuff was:", disp);
//        tortoise();
        bs->buf = (bufelem*)(buffer-disp);
        bs->bcur = (bufelem*)(buffer-disp);
        bs->buf_lim = (bufelem*)(buffer + bufferlen);
        bs->bcur_be = BEND(*(bs->bcur));
        bs->bitoffset = EBITS-(disp*8);
        bs->buf_offset = offset-disp;
//        printf("and new stuff is (buffer=%x):", (int)buffer);
//        tortoise();
    } else { // buffer is on word boundary: simple
//        void tortoise(void);
//        printf("aligned adress, no probs, old stuff:");
//        tortoise();
        bs->buf = (bufelem*)buffer;
        bs->bcur = (bufelem*)buffer;
        bs->buf_lim = (bufelem*)(buffer + bufferlen);
        bs->bcur_be = BEND(*(bs->bcur));
        bs->bitoffset = EBITS;
        bs->buf_offset = offset;
//        printf("new stuff is (buffer=%x):", (int)buffer);
//        tortoise();
    }
}


////////////////////////////////////////////////////////////////////////////
// getbits: now in assembly code

uint32 getbits (int toget)
{
    int bitx;
    bufelem *bcur;
    bufelem bcur_be;
    bitstream *bs = &the_bs;
    /* Assumes max value of toget <= EBITS, so can always fetch in 2 loads */
    bitx = bs->bitoffset;
    bcur = bs->bcur;
    bcur_be = bs->bcur_be;

    if (toget <= bitx)
    {
	uint32 tmp = bcur_be & ~(~0 << bitx);
	uint32 val;
	bitx -= toget;
	val = tmp >> bitx;
	bs->bitoffset = bitx;
//!!//fprintf(stderr, "g1:&%x ", val);
	return val;
    }
    else if (bitx != 0)
    {
	uint32 val;
	val  = bcur_be << (32-bitx) >> (32-toget);
	bcur++;
        printf("%p > %p?\n", bcur, bs->buf_lim);
	if (bcur > bs->buf_lim) {
	    printf("   Yes!!!\n");
	    exit(1);
	}
	bcur_be = BEND(*bcur);
	bitx += EBITS - toget;
	val |= bcur_be >> bitx;
	bs->bcur = bcur;
	bs->bitoffset = bitx;
	bs->bcur_be = bcur_be;
//!!//fprintf(stderr, "g2:&%x ", val);
	return val;
    }
    else
    {
	uint32 val;
	bitx += EBITS - toget;
	bcur++;
        printf("%p > %p?\n", bcur, bs->buf_lim);
	if (bcur > bs->buf_lim) {
	    printf("   Yes!!!\n");
	    exit(1);
	}
        bcur_be = BEND(*bcur);
	val = bcur_be >> bitx;
	bs->bcur = bcur;
	bs->bitoffset = bitx;
	bs->bcur_be = bcur_be;
//!!//fprintf(stderr, "g3:&%x ", val);
	return val;
    }
}

////////////////////////////////////////////////////////////////////////////
// bitposition

uint32 bitposition ()
{
    bitstream *bs = &the_bs;
    uint32 r = (bs->buf_offset + ((char *)bs->bcur - (char *)bs->buf)) * 8 +
		EBITS - bs->bitoffset;

    return r;
}


////////////////////////////////////////////////////////////////////////////
// ensure_bits

int ensure_bits (int needbits)
{
    return 1;
}


/* EOF bits.c */
/*void tortoise() { // last ditch debugging effort 8 Dec 1997
    bitstream *bs = &the_bs;


    printf("\nbuf:        %x\n", (int)bs->buf);
    printf("bcur:       %x\n", (int)bs->bcur);
    printf("bcur_be:    %x\n", bs->bcur_be);
    printf("buf_offset: %i\n", bs->buf_offset);
    printf("bitoffset:  %i\n\n", bs->bitoffset);
}*/
