/* crypto/bn/bn_sub.c */
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
#include "cryptlib.h"
#include "bn.h"

#ifndef NOPROTO
static void bn_SUB(BIGNUM *r, BIGNUM *a, BIGNUM *b);
#else
static void bn_SUB();
#endif

static void bn_SUB(r, a, b)
BIGNUM *r;
BIGNUM *a;
BIGNUM *b;
	{
	int max,min;
	register BN_ULONG t1,t2,*ap,*bp,*rp;
	int i,carry;
#if defined(IRIX_CC_BUG) && !defined(LINT)
	int dummy;
#endif

	max=a->top;
	min=b->top;
	ap=a->d;
	bp=b->d;
	rp=r->d;

	carry=0;
	for (i=0; i<min; i++)
		{
		t1= *(ap++);
		t2= *(bp++);
		if (carry)
			{
			carry=(t1 <= t2);
			t1=(t1-t2-1);
			}
		else
			{
			carry=(t1 < t2);
			t1=(t1-t2);
			}
#if defined(IRIX_CC_BUG) && !defined(LINT)
		dummy=t1;
#endif
		*(rp++)=t1&BN_MASK2;
		}
	if (carry) /* subtracted */
		{
		while (i < max)
			{
			i++;
			t1= *(ap++);
			t2=(t1-1)&BN_MASK2;
			*(rp++)=t2;
			if (t1 > t2) break;
			}
		}
	memcpy(rp,ap,sizeof(*rp)*(max-i));
/*	for (; i<max; i++)
		*(rp++)=*(ap++);*/

	r->top=max;
	bn_fix_top(r);
	}

int BN_sub(r, a, b)
BIGNUM *r;
BIGNUM *a;
BIGNUM *b;
	{
	int max,i;

	/*  a -  b	a-b
	 *  a - -b	a+b
	 * -a -  b	-(a+b)
	 * -a - -b	b-a
	 */
	if (a->neg)
		{
		if (b->neg)
			{
			a->neg=b->neg=0;
			i=BN_sub(r,b,a);
			if (a != r) a->neg=1;
			if (b != r) b->neg=1;
			}
		else
			{
			a->neg=0;
			i=BN_add(r,a,b);
			r->neg=a->neg=1;
			}
		return(i);
		}
	else
		{
		if (b->neg)
			{
			b->neg=0;
			i=BN_add(r,a,b);
			if (r != b) b->neg=1;
			return(i);
			}
		}

	max=(a->top > b->top)?a->top:b->top;
	if (BN_cmp(a,b) < 0)
		{
		if (bn_expand(r,max*BN_BITS2) == NULL) return(0);
		bn_SUB(r,b,a);
		r->neg=1;
		}
	else
		{
		if (bn_expand(r,max*BN_BITS2) == NULL) return(0);
		bn_SUB(r,a,b);
		r->neg=0;
		}
	return(1);
	}

