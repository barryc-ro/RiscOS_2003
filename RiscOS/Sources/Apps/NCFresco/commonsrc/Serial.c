/*
 * Program:	Serial.c - serial number checking code
 *
 * Project:	InetSuite project
 *
 * Authors:	Nick Smith & Nicko van Someren
 *              ANT Limited
 *              Cambridge
 *              Internet: nas@ant.co.uk
 *
 * Date:	21 July 1995
 * Last Edited:	21 July 1995
 *
 * Copyright 1995 by ANT Limited
 */

#include <stdio.h>
#include <ctype.h>
#include "bittabs.h"
#include "Serial.h"

/* Code to check serial number values */
typedef union
{
  unsigned char c[12];
  unsigned short s[6];
  unsigned int i[3];
} bits96;

/* The constant string is something people will not think twice about seeing. */
#if 0
static unsigned char antltd[] = "\251ANT Ltd.";
#else
static unsigned char antltd[] = "\251ANTver2";
#endif

/* The seed values are taken from the code of the program so they are hard to spot! */
static unsigned int seeds[12] =
{
  0xe2813004, 0xe7d03003, 0xe7d2c001, 0xe153000c,
  0x13a00000, 0x11b0f00e, 0xe2811001, 0xe3510008,
  0xbafffff6, 0xe3a00001, 0xe1b0f00e, 0xe92d4010
};


/* Fill a bits96 value with a legal value */
#if 0
static void sn_fill_bits(bits96 *bits, int n)
{
    int i;

    bits->i[0] = (unsigned int) n;
    for (i = 0; i < 8; i++)
    {
	bits->c[i+4] = antltd[i];
    }
}
#endif

static void sn_bit_shuffle_forw(bits96 *bits)
{
    unsigned char *shfl = bittab_forw;
    int i;
    bits96 bns;
    int x;

    bns.i[0] = bns.i[1] = bns.i[2] = 0;

    for (i = 0; i < 96; i++)
    {
	x = shfl[i];
	if (bits->c[i / 8] & (1 << (i % 8)))
	    bns.c[x / 8] |= (1 << (x % 8));
    }

    *bits = bns;
}

static void sn_eor_words_forw(bits96 *bits, unsigned int seed)
{
    unsigned int x;
    int i;

    x = 0;
    for(i=0; i < 12; i++)
    {
	bits->c[i] ^= (x ^ (seed >> ((i % 4) * 8)));
	x = bits->c[i];
    }
}

static void sn_12pass_forw(bits96 *bits)
{
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[0]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[1]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[2]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[3]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[4]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[5]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[6]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[7]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[8]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[9]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[10]);
    sn_bit_shuffle_forw(bits);
    sn_eor_words_forw(bits, seeds[11]);
}

static void sn_ascii_encode(bits96 *bits, char *buffer)
{
    int i, j;
    int x, y;

    for(i=0, j=0; i < 12; i += 3)
    {
	x = (bits->c[i+2] << 16) + (bits->c[i+1] << 8) + (bits->c[i+0] << 0);

	y = (x >> 0) & 0x3f;
	buffer[j++] = ascii_encode[y];

	y = (x >> 6) & 0x3f;
	buffer[j++] = ascii_encode[y];

	y = (x >> 12) & 0x3f;
	buffer[j++] = ascii_encode[y];

	y = (x >> 18) & 0x3f;
	buffer[j++] = ascii_encode[y];
    }

    buffer[j] = 0;
}

#if 0
void serial_encode(char *buffer, int n)
{
    bits96 bns;

    sn_fill_bits(&bns, n);
    sn_12pass_forw(&bns);

    sn_ascii_encode(&bns, buffer+31);

    sn_bit_shuffle_forw((bits96 *) &buffer[0]);
    sn_bit_shuffle_forw((bits96 *) &buffer[12]);
    sn_bit_shuffle_forw((bits96 *) &buffer[24]);
    sn_bit_shuffle_forw((bits96 *) &buffer[36]);
}
#endif

static void sn_ascii_decode(char *buffer, bits96 *bits)
{
  int i, j;
  int x, y;

  for (j = 0, i=0; j < 16; i += 3)
  {
    x = 0;
    y = ascii_decode[buffer[j++] & 0x7f];
    x += (y << 0);
    y = ascii_decode[buffer[j++] & 0x7f];
    x += (y << 6);
    y = ascii_decode[buffer[j++] & 0x7f];
    x += (y << 12);
    y = ascii_decode[buffer[j++] & 0x7f];
    x += (y << 18);
    bits->c[i+0] = (x >>  0) & 0xff;
    bits->c[i+1] = (x >>  8) & 0xff;
    bits->c[i+2] = (x >> 16) & 0xff;
  }
}

static void sn_bit_shuffle_back(bits96 *bits)
{
  unsigned char *shfl = bittab_back;
  int i;
  bits96 bns;
  int x;

  bns.i[0] = bns.i[1] = bns.i[2] = 0;
  for (i = 0; i < 96; i++)
  {
    x = shfl[i];
    if (bits->c[i / 8] & (1 << (i % 8))) bns.c[x / 8] |= (1 << (x % 8));
  }
  *bits = bns;
}

static void sn_eor_words_back(bits96 *bits, unsigned int seed)
{
  unsigned int x, y;
  int i;

  x = 0;
  for (i=0; i < 12; i++)
  {
    y = bits->c[i];
    bits->c[i] ^= (x ^ (seed >> ((i % 4) * 8)));
    x = y;
  }
}

static void sn_12pass_back(bits96 *bits)
{
  sn_eor_words_back(bits, seeds[11]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[10]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[9]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[8]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[7]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[6]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[5]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[4]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[3]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[2]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[1]);
  sn_bit_shuffle_back(bits);
  sn_eor_words_back(bits, seeds[0]);
  sn_bit_shuffle_back(bits);
}

/*  Verify serial number.
 */
extern int VerifySerial(char *buffer)
{
  bits96 bns;
  int i,j;

  /* Decode 4 x 96 bits of buffer (destructive action on the buffer, leaving
   * the real name & serial strings in plain ASCII text).
   */
#if !DEVELOPMENT && !defined(STBWEB_ROM)
  sn_bit_shuffle_back((bits96 *) &buffer[0]);
  sn_bit_shuffle_back((bits96 *) &buffer[12]);
  sn_bit_shuffle_back((bits96 *) &buffer[24]);
  sn_bit_shuffle_back((bits96 *) &buffer[36]);
#endif

  sn_ascii_decode(&buffer[31], &bns);
  sn_12pass_back(&bns);
  for (i = 0; i < 8; i++)
  {
      if (bns.c[i+4] != antltd[i])
	  return(0);
#if 1
      bns.c[i+4] = antltd[7-i];
#endif
  }

  j = bns.i[0];

  sn_12pass_forw(&bns);
  sn_ascii_encode(&bns, &buffer[31]);

  return j;
}

/* eof */
