/* crypto/bn/bn_shift.c */
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

#if defined(SSL_DEBUG)
#define ptrace fprintf
#define pflush fflush
#else
#define ptrace 1?0:fprintf
#define pflush 1?0:fflush
#endif

int BN_lshift1(r, a)
BIGNUM *r;
BIGNUM *a;
	{
	register BN_ULONG *ap,*rp,t,c;
	int i;

	if (r != a)
		{
		r->neg=a->neg;
		if (bn_expand(r,(a->top+1)*BN_BITS2) == NULL) return(0);
		r->top=a->top;
		}
	else
		{
		if (bn_expand(r,(a->top+1)*BN_BITS2) == NULL) return(0);
		}
	ap=a->d;
	rp=r->d;
	c=0;
	for (i=0; i<a->top; i++)
		{
		t= *(ap++);
		*(rp++)=((t<<1)|c)&BN_MASK2;
		c=(t & BN_TBIT)?1:0;
		}
	if (c)
		{
		*rp=1;
		r->top++;
		}
	return(1);
	}

int BN_rshift1(r, a)
BIGNUM *r;
BIGNUM *a;
	{
	BN_ULONG *ap,*rp,t,c;
	int i;

	if (BN_is_zero(a))
		{
		BN_zero(r);
		return(1);
		}
	if (a != r)
		{
		if (bn_expand(r,a->top*BN_BITS2) == NULL) return(0);
		r->top=a->top;
		r->neg=a->neg;
		}
	ap=a->d;
	rp=r->d;
	c=0;
	for (i=a->top-1; i>=0; i--)
		{
		t=ap[i];
		rp[i]=((t>>1)&BN_MASK2)|c;
		c=(t&1)?BN_TBIT:0;
		}
	bn_fix_top(r);
	return(1);
	}

extern void *stack_pointer( void );

int BN_lshift(r, a, n)
BIGNUM *r;
BIGNUM *a;
int n;
	{
	static int i,nw,lb,rb;
	static BN_ULONG *t,*f;
	static BN_ULONG l;

        ptrace( stderr, "bnls:1 (R13~=%p)\n", stack_pointer() );

	r->neg=a->neg;
	if (bn_expand(r,(a->top*BN_BITS2)+n) == NULL) return(0);

        ptrace( stderr, "bnls:2 r=%p r->d=%p\n", r, r ? r->d : NULL );
	pflush( stderr );
/*
	ptrace( stderr, "r=%p n=%d a=%p\n", r, n, a );
	pflush( stderr );

	ptrace( stderr, "r=%p r->d=%p a->top=%d n=%d\n", r, r->d, a->top,n );
	pflush( stderr );
*/
	nw=n/BN_BITS2;
	lb=n%BN_BITS2;
	rb=BN_BITS2-lb;
	f=a->d;
	t=r->d;
        ptrace( stderr, "t=%p\n", t );
	pflush( stderr );

	t[a->top+nw]=0;

        ptrace( stderr, "bnls:3\n" );
	pflush( stderr );

	if (lb == 0)
		for (i=a->top-1; i>=0; i--)
			t[nw+i]=f[i];
	else
		for (i=a->top-1; i>=0; i--)
			{
			l=f[i];
			t[nw+i+1]|=(l>>rb)&BN_MASK2;
			t[nw+i]=(l<<lb)&BN_MASK2;
			}
        ptrace( stderr, "bnls:4\n" );
/*
        ptrace( stderr, "t=%p nw=%d sizeof(t[0])=%d r=%p a=%p\n",
	       t,nw,sizeof(BN_ULONG),r,a );
#ifdef SSL_DEBUG
	pflush( stderr );
#endif
*/
	memset(t,0,nw*sizeof(BN_ULONG));
/*	for (i=0; i<nw; i++)
		t[i]=0;*/
	r->top=a->top+nw+1;
        ptrace( stderr, "bnls:5\n" );

	bn_fix_top(r);
        ptrace( stderr, "bnls:6\n" );
	return(1);
	}

int BN_rshift(r, a, n)
BIGNUM *r;
BIGNUM *a;
int n;
	{
	int i,j,nw,lb,rb;
	BN_ULONG *t,*f;
	BN_ULONG l;

	r->neg=a->neg;
	nw=n/BN_BITS2;
	rb=n%BN_BITS2;
	lb=BN_BITS2-rb;
	if (nw > a->top)
		{
		BN_zero(r);
		return(1);
		}
	if (bn_expand(r,(a->top-nw+1)*BN_BITS2) == NULL) return(0);

	f= &(a->d[nw]);
	t=r->d;
	j=a->top-nw;
	r->top=j;

	if (rb == 0)
		{
		for (i=j+1; i > 0; i--)
			*(t++)= *(f++);
		}
	else
		{
		l= *(f++);
		for (i=0; i<j; i++)
			{
			*t =(l>>rb)&BN_MASK2;
			l= *(f++);
			*(t++) |=(l<<lb)&BN_MASK2;
			}
		}
	*t=0;
	bn_fix_top(r);
	return(1);
	}
