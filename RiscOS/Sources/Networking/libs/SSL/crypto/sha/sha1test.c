/* crypto/sha/sha1test.c */
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sha.h"

#undef SHA_0 /* FIPS 180 */
#define  SHA_1 /* FIPS 180-1 */

char *test[]={
	"abc",
	"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	NULL,
	};

#ifdef SHA_0
char *ret[]={
	"0164b8a914cd2a5e74c4f7ff082c4d97f1edf880",
	"d2516ee1acfa5baf33dfc1c471e438449ef134c8",
	};
char *bigret=
	"3232affa48628a26653b5aaa44541fd90d690603";
#endif
#ifdef SHA_1
char *ret[]={
	"a9993e364706816aba3e25717850c26c9cd0d89d",
	"84983e441c3bd26ebaae4aa1f95129e5e54670f1",
	};
char *bigret=
	"34aa973cd4c4daa4f61eeb2bdbad27316534016f";
#endif

#ifndef NOPROTO
static char *pt(unsigned char *md);
#else
static char *pt();
#endif

int main(argc,argv)
int argc;
char *argv[];
	{
	int i,err=0;
	unsigned char **P,**R;
	static unsigned char buf[1000];
	char *p,*r;
	SHA_CTX c;
	unsigned char md[SHA_DIGEST_LENGTH];

	P=(unsigned char **)test;
	R=(unsigned char **)ret;
	i=1;
	while (*P != NULL)
		{
		p=pt(SHA1(*P,(unsigned long)strlen((char *)*P),NULL));
		if (strcmp(p,(char *)*R) != 0)
			{
			printf("error calculating SHA1 on '%s'\n",*P);
			printf("got %s instead of %s\n",p,*R);
			err++;
			}
		else
			printf("test %d ok\n",i);
		i++;
		R++;
		P++;
		}

	memset(buf,'a',1000);
	SHA1_Init(&c);
	for (i=0; i<1000; i++)
		SHA1_Update(&c,buf,1000);
	SHA1_Final(md,&c);
	p=pt(md);

	r=bigret;
	if (strcmp(p,r) != 0)
		{
		printf("error calculating SHA1 on '%s'\n",p);
		printf("got %s instead of %s\n",p,r);
		err++;
		}
	else
		printf("test 3 ok\n");
	exit(err);
	return(0);
	}

static char *pt(md)
unsigned char *md;
	{
	int i;
	static char buf[80];

	for (i=0; i<SHA_DIGEST_LENGTH; i++)
		sprintf(&(buf[i*2]),"%02x",md[i]);
	return(buf);
	}
