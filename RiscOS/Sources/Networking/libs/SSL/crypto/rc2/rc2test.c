/* crypto/rc2/rc2test.c */
/* Copyright (C) 1995-1996 Eric Young (eay@mincom.oz.au)
 * All rights reserved.
 * 
 * This file is part of an SSL implementation written
 * by Eric Young (eay@mincom.oz.au).
 * The implementation was written so as to conform with Netscapes SSL
 * specification.  This library and applications are
 * FREE FOR COMMERCIAL AND NON-COMMERCIAL USE
 * as long as the following conditions are aheared to.
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.  If this code is used in a product,
 * Eric Young should be given attribution as the author of the parts used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Eric Young (eay@mincom.oz.au)
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

/* This has been a quickly hacked 'ideatest.c'.  When I add tests for other
 * RC2 modes, more of the code will be uncommented. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rc2.h"

#define NTESTS		2

unsigned char RC2key[NTESTS][8]={
	/* test vector from nci/state dept (only 8 bytes valid) */
    {0x59,0x45,0x9a,0xf9,0x27,0x84,0x74,0xca},
    {0x33,0x30,0x65,0xa6,0xb8,0xfc,0x77,0xb1}
};

unsigned char RC2plain[NTESTS][8]={
	/* test vector from nci/state dept */
	{0xd5,0x58,0x75,0x12,0xce,0xef,0x77,0x93},
	{0x6c,0x60,0x97,0xc1,0xe5,0x63,0x76,0x51}
};

unsigned char RC2cipher[NTESTS][8]={
	/* test vector from nci/state dept */
	{0x87,0xd7,0x02,0xbc,0xf0,0x1d,0xfb,0xc8},
};

/************/

static void error (const char *msg, const unsigned char *buf1, const unsigned char *buf2)
{
    int i;
    printf("ebc rc2 error %s\n", msg);
    printf("got     :");
    for (i=0; i<8; i++)
	printf("%02X ",buf1[i]);
    printf("\n");
    printf("expected:");
    for (i=0; i<8; i++)
	printf("%02X ",buf2[i]);
    printf("\n");
}

int main(argc,argv)
int argc;
char *argv[];
{
 	int i,n,err=0;
	RC2_KEY key; 
	unsigned char buf[8],buf2[8];

	for (n = 0; n < NTESTS; n++)
	{
		printf("\nTest %d\n", n);
		
		RC2_set_key(&key,8,&RC2key[n][0]);
		
		RC2_ecb_encrypt(&(RC2plain[n][0]),buf,&key, RC2_ENCRYPT);
		
		if (memcmp(&(RC2cipher[n][0]),buf,8) != 0)
		    error("encrypting", buf, RC2cipher[n]);
		
		
		RC2_ecb_encrypt(&(RC2cipher[n][0]),buf,&key, RC2_DECRYPT);
		
		if (memcmp(&(RC2plain[n][0]),buf,8) != 0)
		    error("decrypting", buf, RC2plain[n]);
	}

	exit(err);
	return(err);
}

