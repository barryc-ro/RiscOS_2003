/* crypto/rc2/rc2_ecb.c */
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

#include "rc2.h"
#include "rc2_locl.h"

char *RC2_version="RC2 part of SSLeay 0.6.0 21-Jun-1996";

/* RC2 as implemented frm a posting from
 * Newsgroups: sci.crypt
 * Sender: pgut01@cs.auckland.ac.nz (Peter Gutmann)
 * Subject: Specification for Ron Rivests Cipher No.2
 * Message-ID: <4fk39f$f70@net.auckland.ac.nz>
 * Date: 11 Feb 1996 06:45:03 GMT
 */

void RC2_ecb_encrypt(in, out, ks, encrypt)
unsigned char *in;
unsigned char *out;
RC2_KEY *ks;
int encrypt;
	{
	unsigned long l,d[2];

	c2l(in,l); d[0]=l;
	c2l(in,l); d[1]=l;
	RC2_encrypt(d,ks,encrypt);
	l=d[0]; l2c(l,out);
	l=d[1]; l2c(l,out);
	l=d[0]=d[1]=0;
	}

#define rot(x,n)

void RC2_encrypt(d,key,encrypt)
unsigned long *d;
RC2_KEY *key;
int encrypt;
	{
	int i,n;
	register RC2_INT *p0,*p1;
	register RC2_INT x0,x1,x2,x3,t;
	unsigned long l;

	l=d[0];
	x0=l&0xffff;
	x1=(l>>16);
	l=d[1];
	x2=l&0xffff;
	x3=(l>>16);

	n=3;
	i=5;
	if (encrypt)
		{
		p0=p1= &(key->data[0]);
		for (;;)
			{
			t=(x0+(x1& ~x3)+(x2&x3)+ *(p0++))&0xffff;
			x0=(t<<1)|(t>>15);
			t=(x1+(x2& ~x0)+(x3&x0)+ *(p0++))&0xffff;
			x1=(t<<2)|(t>>14);
			t=(x2+(x3& ~x1)+(x0&x1)+ *(p0++))&0xffff;
			x2=(t<<3)|(t>>13);
			t=(x3+(x0& ~x2)+(x1&x2)+ *(p0++))&0xffff;
			x3=(t<<5)|(t>>11);

			if (--i == 0)
				{
				if (--n == 0) break;
				i=(n == 2)?6:5;

				x0+=p1[x3&0x3f];
				x1+=p1[x0&0x3f];
				x2+=p1[x1&0x3f];
				x3+=p1[x2&0x3f];
				}
			}
		}
	else
		{
		p0= &(key->data[63]);
		p1= &(key->data[0]);
		for (;;)
			{
			t=((x3<<11)|(x3>>5))&0xffff;
			x3=(t-(x0& ~x2)-(x1&x2)- *(p0--))&0xffff;
			t=((x2<<13)|(x2>>3))&0xffff;
			x2=(t-(x3& ~x1)-(x0&x1)- *(p0--))&0xffff;
			t=((x1<<14)|(x1>>2))&0xffff;
			x1=(t-(x2& ~x0)-(x3&x0)- *(p0--))&0xffff;
			t=((x0<<15)|(x0>>1))&0xffff;
			x0=(t-(x1& ~x3)-(x2&x3)- *(p0--))&0xffff;

			if (--i == 0)
				{
				if (--n == 0) break;
				i=(n == 2)?6:5;

				x3=(x3-p1[x2&0x3f])&0xffff;
				x2=(x2-p1[x1&0x3f])&0xffff;
				x1=(x1-p1[x0&0x3f])&0xffff;
				x0=(x0-p1[x3&0x3f])&0xffff;
				}
			}
		}

	d[0]=(x0&0xffff)|((x1&0xffff)<<16);
	d[1]=(x2&0xffff)|((x3&0xffff)<<16);
	}
