/* crypto/sha/sha_locl.h */
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

#include <stdlib.h>
#include <string.h>

/* one or the other needs to be defined */
#ifndef SHA_1 /* FIPE 180-1 */
#define SHA_0 /* FIPS 180   */
#endif

#define ULONG	unsigned long
#define UCHAR	unsigned char
#define UINT	unsigned int

#ifdef NOCONST
#define const
#endif

#define c2nl(c,l)	(l =(((unsigned long)(*((c)++)))<<24), \
			 l|=(((unsigned long)(*((c)++)))<<16), \
			 l|=(((unsigned long)(*((c)++)))<< 8), \
			 l|=(((unsigned long)(*((c)++)))    ))

#define p_c2nl(c,l,n)	{ \
			switch (n) { \
			case 0: l =((unsigned long)(*((c)++)))<<24; \
			case 1: l|=((unsigned long)(*((c)++)))<<16; \
			case 2: l|=((unsigned long)(*((c)++)))<< 8; \
			case 3: l|=((unsigned long)(*((c)++))); \
				} \
			}

/* NOTE the pointer is not incremented at the end of this */
#define c2nl_p(c,l,n)	{ \
			l=0; \
			(c)+=n; \
			switch (n) { \
			case 3: l =((unsigned long)(*(--(c))))<< 8; \
			case 2: l|=((unsigned long)(*(--(c))))<<16; \
			case 1: l|=((unsigned long)(*(--(c))))<<24; \
				} \
			}

#define p_c2nl_p(c,l,sc,len) { \
			switch (sc) \
				{ \
			case 0: l =((unsigned long)(*((c)++)))<<24; \
				if (--len == 0) break; \
			case 1: l|=((unsigned long)(*((c)++)))<<16; \
				if (--len == 0) break; \
			case 2: l|=((unsigned long)(*((c)++)))<< 8; \
				} \
			}

#define nl2c(l,c)	(*((c)++)=(unsigned char)(((l)>>24)&0xff), \
			 *((c)++)=(unsigned char)(((l)>>16)&0xff), \
			 *((c)++)=(unsigned char)(((l)>> 8)&0xff), \
			 *((c)++)=(unsigned char)(((l)    )&0xff))

/* I have taken some of this code from my MD5 implementation */

#if defined(WIN32)
#define ROTATE(a,n)     _lrotl(a,n)
#else
#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))
#endif

/* A nice byte order reversal from Wei Dai <weidai@eskimo.com> */
#if defined(WIN32)
/* 5 instructions with rotate instruction, else 9 */
#define Endian_Reverse32(a) \
	{ \
	unsigned long l=(a); \
	((ROTATE(l,8)&0xFF00FF00)|(ROTATE(l,24)&0x00FF00FF)); \
	}
#else
/* 6 instructions with rotate instruction, else 8 */
#define Endian_Reverse32(a) \
	{ \
	unsigned long l=(a); \
	l=(((l&0xFF00FF00)>>8L)|((l&0x00FF00FF)<<8L)); \
	(a)=ROTATE(l,16L); \
	}
#endif

/* As  pointed out by Wei Dai <weidai@eskimo.com>, F() below can be
 * simplified to the code in F_00_19.  Wei attributes these optimisations
 * to Peter Gutmann's SHS code, and he attributes it to Rich Schroeppel.
 * #define F(x,y,z) (((x) & (y))  |  ((~(x)) & (z)))
 * I've just become aware of another tweak to be made, again from Wei Dai,
 * in F_40_59, (x&a)|(y&a) -> (x|y)&a
 */
#define	F_00_19(b,c,d)	((((c) ^ (d)) & (b)) ^ (d)) 
#define	F_20_39(b,c,d)	((b) ^ (c) ^ (d))
#define F_40_59(b,c,d)	(((b) & (c)) | (((b)|(c)) & (d))) 
#define	F_60_79(b,c,d)	F_20_39(b,c,d)

#ifdef SHA_0
#define Xupdate(a,i) \
	X[(i)&0x0f]=(a)=\
		(X[(i)&0x0f]^X[((i)+2)&0x0f]^X[((i)+8)&0x0f]^X[((i)+13)&0x0f]);
#endif
#ifdef SHA_1
#define Xupdate(a,i) \
	(a)=(X[(i)&0x0f]^X[((i)+2)&0x0f]^X[((i)+8)&0x0f]^X[((i)+13)&0x0f]); \
	X[(i)&0x0f]=(a)=ROTATE((a),1);
#endif

#define BODY_00_15(i,a,b,c,d,e,f) \
	(f)=X[i]+(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_16_19(i,a,b,c,d,e,f) \
	Xupdate(f,i); \
	(f)+=(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_20_39(i,a,b,c,d,e,f) \
	Xupdate(f,i); \
	(f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_40_59(i,a,b,c,d,e,f) \
	Xupdate(f,i); \
	(f)+=(e)+K_40_59+ROTATE((a),5)+F_40_59((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_60_79(i,a,b,c,d,e,f) \
	Xupdate(f,i); \
	(f)=X[(i)&0x0f]+(e)+K_60_79+ROTATE((a),5)+F_60_79((b),(c),(d)); \
	(b)=ROTATE((b),30);

