/* lib/rc4/rc4_enc.c */
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

/* WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 *
 * Always modify rc2_enc.org since rc2_enc.c is automatically generated from
 * it during SSLeay 0.6.0 21-Jun-1996
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING 
 */

#include "rc4.h"

/* if this is defined data[i] is used instead of *data, this is a %20
 * speedup on x86 */
#undef RC4_INDEX

char *RC4_version="RC4 part of SSLeay 0.6.0 21-Jun-1996";

char *RC4_options()
	{
#ifdef RC4_INDEX
	if (sizeof(RC4_INT) == 1)
		return("rc4(idx,char)");
	else
		return("rc4(idx,int)");
#else
	if (sizeof(RC4_INT) == 1)
		return("rc4(ptr,char)");
	else
		return("rc4(ptr,int)");
#endif
	}

/* RC4 as implemented from a posting from
 * Newsgroups: sci.crypt
 * From: sterndark@netcom.com (David Sterndark)
 * Subject: RC4 Algorithm revealed.
 * Message-ID: <sternCvKL4B.Hyy@netcom.com>
 * Date: Wed, 14 Sep 1994 06:35:31 GMT
 */

void RC4_set_key(key, len, data)
RC4_KEY *key;
int len;
register unsigned char *data;
	{
        register RC4_INT tmp;
        register int id1,id2;
        register RC4_INT *d;
        unsigned int i;
        
        d=&(key->data[0]);
	for (i=0; i<256; i++)
		d[i]=i;
        key->x = 0;     
        key->y = 0;     
        id1=id2=0;     

#define SK_LOOP(n) { \
		tmp=d[(n)]; \
		id2 = (data[id1] + tmp + id2) & 0xff; \
		if (++id1 == len) id1=0; \
		d[(n)]=d[id2]; \
		d[id2]=tmp; }

	for (i=0; i < 256; i+=4)
		{
		SK_LOOP(i+0);
		SK_LOOP(i+1);
		SK_LOOP(i+2);
		SK_LOOP(i+3);
		}
	}
    
void RC4(key, len, indata, outdata)
RC4_KEY *key;
unsigned long len;
unsigned char *indata;
unsigned char *outdata;
	{
        register RC4_INT *d;
        register RC4_INT x,y,tx;
	int i;
        
        x=key->x;     
        y=key->y;     
        d=key->data; 

#define LOOP(in,out) \
		x=((x+1)&0xff); \
		tx=d[x]; \
		y=(tx+y)&0xff; \
		d[x]=d[y]; \
		d[y]=tx; \
		(out) = d[(tx+d[x])&0xff]^ (in);

#ifndef RC4_INDEX
#define RC4_LOOP(a,b)	LOOP(*((a)++),*((b)++))
#else
#define RC4_LOOP(a,b)	LOOP(a[i],b[i])
	indata+=len;
	outdata+=len;
#endif

	i= -(int)len;
        for (;;)
		{               
		RC4_LOOP(indata,outdata); if (++i == 0) break;
		RC4_LOOP(indata,outdata); if (++i == 0) break;
		RC4_LOOP(indata,outdata); if (++i == 0) break;
		RC4_LOOP(indata,outdata); if (++i == 0) break;
		RC4_LOOP(indata,outdata); if (++i == 0) break;
		RC4_LOOP(indata,outdata); if (++i == 0) break;
		RC4_LOOP(indata,outdata); if (++i == 0) break;
		RC4_LOOP(indata,outdata); if (++i == 0) break;
		}               
	key->x=x;     
	key->y=y;
	}
