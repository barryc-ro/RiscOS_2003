/* crypto/bn/bn_div.c */
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
#include "bn_lcl.h"

#ifdef undef
int BN_div(dv, rem, m, d,ctx)
BIGNUM *dv;
BIGNUM *rem;
BIGNUM *m;
BIGNUM *d;
BN_CTX *ctx;
	{
	int i,nm,nd;
	BIGNUM *D;

	if (BN_is_zero(d))
		{
		BNerr(BN_F_BN_DIV,BN_R_DIV_BY_ZERO);
		return(0);
		}

	if (BN_cmp(m,d) < 0)
		{
		if (rem != NULL)
			{ if (BN_copy(rem,m) == NULL) return(0); }
		if (dv != NULL) BN_zero(dv);
		return(1);
		}

	D=ctx->bn[ctx->tos];
	if (dv == NULL) dv=ctx->bn[ctx->tos+1];
	if (rem == NULL) rem=ctx->bn[ctx->tos+2];

	nd=BN_num_bits(d);
	nm=BN_num_bits(m);
	if (BN_copy(D,d) == NULL) return(0);
	if (BN_copy(rem,m) == NULL) return(0);

	/* The next 2 are needed so we can do a dv->d[0]|=1 later
	 * since BN_lshift1 will only work once there is a value :-) */
	BN_zero(dv);
	dv->top=1;

	if (!BN_lshift(D,D,nm-nd)) return(0);
	for (i=nm-nd; i>=0; i--)
		{
		if (!BN_lshift1(dv,dv)) return(0);
		if (BN_cmp(rem,D) >= 0)
			{
			dv->d[0]|=1;
			if (!BN_sub(rem,rem,D)) return(0);
			}
/* CAN IMPROVE */
		if (!BN_rshift1(D,D)) return(0);
		}
	dv->neg=m->neg^d->neg;
	return(1);
	}

#else

int BN_div(dv, rm, num, div,ctx)
BIGNUM *dv;
BIGNUM *rm;
BIGNUM *num;
BIGNUM *div;
BN_CTX *ctx;
	{
	int norm_shift,i,j,loop;
	BIGNUM *tmp,wnum,*snum,*sdiv,*res;
	BN_ULONG *resp,*wnump;
	BN_ULONG d0,d1;
	int num_n,div_n;

	if (BN_is_zero(num))
		{
		BNerr(BN_F_BN_DIV,BN_R_DIV_BY_ZERO);
		return(0);
		}

	if (BN_cmp(num,div) < 0)
		{
		if (rm != NULL)
			{ if (BN_copy(rm,num) == NULL) return(0); }
		if (dv != NULL) BN_zero(dv);
		return(1);
		}

	tmp=ctx->bn[ctx->tos]; 
	snum=ctx->bn[ctx->tos+1];
	sdiv=ctx->bn[ctx->tos+2];
	if (dv == NULL)
		res=ctx->bn[ctx->tos+3];
	else	res=dv;

	/* First we normalise the numbers */
	norm_shift=BN_BITS2-((BN_num_bits(div))%BN_BITS2);
	BN_lshift(sdiv,div,norm_shift);
	norm_shift+=BN_BITS2;
	BN_lshift(snum,num,norm_shift);
	div_n=sdiv->top;
	num_n=snum->top;
	loop=num_n-div_n;

	/* Lets setup a 'window' into snum
	 * This is the part that corresponds to the current
	 * 'area' being divided */
	wnum.d=	 &(snum->d[loop]);
	wnum.top= div_n;
	wnum.max= snum->max; /* a bit of a lie */
	wnum.neg= 0;

	/* Get the top 2 words of sdiv */
	/* i=sdiv->top; */
	d0=sdiv->d[div_n-1];
	d1=(div_n == 1)?0:sdiv->d[div_n-2];

	/* pointer to the 'top' of snum */
	wnump= &(snum->d[num_n-1]);

	/* Setup to 'res' */
	res->neg=0;
	res->top=loop;
	bn_expand(res,(loop+1)*BN_BITS2);
	resp= &(res->d[loop-1]);

	/* space for temp */
	bn_expand(tmp,(div_n+1)*BN_BITS2);

	if (BN_cmp(&wnum,sdiv) >= 0)
		{
		BN_sub(&wnum,&wnum,sdiv);
		*resp=1;
		res->d[res->top-1]=1;
		}
	else
		res->top--;
	resp--;

	for (i=0; i<loop-1; i++)
		{
		BN_ULONG q,n0,n1;
		BN_ULONG l0;

		wnum.d--; wnum.top++;
		n0=wnump[0];
		n1=wnump[-1];
		if (n0 == d0)
			q=BN_MASK2;
		else
			q=bn_div64(n0,n1,d0);
		{
#ifdef BN_LLONG
		BN_ULLONG t1,t2,t3;
		t1=((BN_ULLONG)n0<<BN_BITS2)|n1;
		for (;;)
			{
			t2=(BN_ULLONG)d1*q;
			t3=t1-(BN_ULLONG)q*d0;
			if ((t3>>BN_BITS2) ||
				(t2 <= ((t3<<BN_BITS2)+wnump[-2])))
				break;
			q--;
			}
#else
		BN_ULONG t1l,t1h,t2l,t2h,t3l,t3h,ql,qh,t3t;
		t1h=n0;
		t1l=n1;
		for (;;)
			{
			t2l=LBITS(d1); t2h=HBITS(d1);
			ql =LBITS(q);  qh =HBITS(q);
			mul64(t2l,t2h,ql,qh); /* t2=(BN_ULLONG)d1*q; */

			t3t=LBITS(d0); t3h=HBITS(d0);
			mul64(t3t,t3h,ql,qh); /* t3=t1-(BN_ULLONG)q*d0; */
			t3l=(t1l-t3t);
			if (t3l > t1l) t3h++;
			t3h=(t1h-t3h);

			/*if ((t3>>BN_BITS2) ||
				(t2 <= ((t3<<BN_BITS2)+wnump[-2])))
				break; */
			if (t3h) break;
			if (t2h < t3l) break;
			if ((t2h == t3l) && (t2l <= wnump[-2])) break;

			q--;
			}
#endif
		}
		l0=bn_mul_word(tmp->d,sdiv->d,div_n,q);
		if (l0)
			tmp->d[div_n]=l0;
		else
			tmp->d[div_n]=0;
		for (j=div_n+1; j>0; j--)
			if (tmp->d[j-1]) break;
		tmp->top=j;

		j=wnum.top;
		BN_sub(&wnum,&wnum,tmp);
		snum->top=snum->top+wnum.top-j;

		if (wnum.neg)
			{
			q--;
			j=wnum.top;
			BN_add(&wnum,&wnum,sdiv);
			snum->top+=wnum.top-j;
			}
		*(resp--)=q;
		wnump--;
		}
	if (rm != NULL)
		BN_rshift(rm,snum,norm_shift);
	return(1);
	}

#endif
