/* crypto/bn/bn_add.c */
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

/* r can == a or b */
int BN_add(r, a, b)
BIGNUM *r;
BIGNUM *a;
BIGNUM *b;
	{
	register int i;
	int max,min;
	BN_ULONG *ap,*bp,*rp,carry,t1,t2;
	BIGNUM *tmp;

	/*  a +  b	a+b
	 *  a + -b	a-b
	 * -a +  b	b-a
	 * -a + -b	-(a+b)
	 */
	if (a->neg ^ b->neg)
		{
		if (a->neg)
			{ a->neg=0; i=BN_sub(r,b,a); if (a != r) a->neg=1; }
		else
			{ b->neg=0; i=BN_sub(r,a,b); if (b != r) b->neg=1; }
		return(i);
		}
	if (a->neg) /* both are neg */
		{
		a->neg=0; b->neg=0; i=BN_add(r,a,b);
		if (a != r) a->neg=1;
		if (b != r) b->neg=1;
		return(i);
		}
	if (a->top < b->top)
		{ tmp=a; a=b; b=tmp; }
		
	max=a->top;
	min=b->top;
	if (bn_expand(r,(max+1)*BN_BITS2) == NULL) return(0);
	r->top=max;
	r->neg=0;

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
			carry=(t2 >= ((~t1)&BN_MASK2));
			t2=(t1+t2+1)&BN_MASK2;
			}
		else
			{
			t2=(t1+t2)&BN_MASK2;
			carry=(t2 < t1);
			}
		*(rp++)=t2;
		}
	if (carry)
		{
		while (i < max)
			{
			t1= *(ap++);
			t2=(t1+1)&BN_MASK2;
			*(rp++)=t2;
			carry=(t2 < t1);
			i++;
			if (!carry) break;
			}
		if ((i >= max) && carry)
			{
			*(rp++)=1;
			r->top++;
			}
		}
	for (; i<max; i++)
		*(rp++)= *(ap++);
	memcpy(rp,ap,sizeof(*ap)*(max-i));
	return(1);
	}

