/* crypto/bn/bn_mont.c */
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

/* EXPERIMENTAL, DON'T USE THIS FUNCTION */

/* r != a */
int BN_mont(r, a, m, sub, ctx)
BIGNUM *r;
BIGNUM *a;
BIGNUM *m;
BIGNUM *sub;
BN_CTX *ctx;
	{
	int j,nbits;
	BIGNUM *t1,*t2,*t3;
	BN_ULONG w,*jp;

BN_print(stderr,a); fprintf(stderr," %% ");
BN_print(stderr,m); fprintf(stderr,"\n");
	if (BN_ucmp(a,m) < 0)
		return((BN_copy(r,a) == NULL)?0:1);

	t1=ctx->bn[ctx->tos];
	t2=ctx->bn[ctx->tos+1];
	t3=ctx->bn[ctx->tos+2];
	nbits=BN_num_bits(m);
	if (sub == NULL)
		{
		sub=ctx->bn[ctx->tos+3];
		BN_set_word(sub,1);
		BN_lshift(sub,sub,nbits);
BN_print(stderr,sub); fprintf(stderr," shift\n");
		BN_sub(sub,sub,m);
BN_print(stderr,m); fprintf(stderr," sub m\n");
BN_print(stderr,sub); fprintf(stderr," sub\n");
		}

	BN_rshift(t1,a,nbits);
	BN_copy(t2,a);
	BN_mask_bits(t2,nbits);

	/* t1 is the 'top' words that need to be reduced.
	 * t1 has plenty of 'expansion' space.
	 * t2 contains the 'remainder' */
	w=0;
	jp=t1->d;
	j=t1->top;
	while (t1->top)
		{
BN_print(stderr,sub); fprintf(stderr," * ");
BN_print(stderr,t1); fprintf(stderr," - "); 
		bn_mul_add_word(w,t2->d,sub->d,sub->top,t1->d[0]);
BN_print(stderr,t2); fprintf(stderr," + %08lX\n",w);
		t1->d++;
		if (--t1->top)
			BN_add_word(t1,w);
		fprintf(stderr,"<");
		BN_print(stderr,t2); fprintf(stderr,"><");
		BN_print(stderr,t1); fprintf(stderr," %d>\n",t1->top);
		}
	t1->d=jp;
	t1->top=j;
exit(0);

/*
BN_print(stderr,r); fprintf(stderr," 'a' after shifting %d bits\n",nbits);
	BN_mul(r,sub,tmp);
BN_print(stderr,tmp); fprintf(stderr," 'a' after multiplying by the 'mod'\n");
BN_print(stderr,a); fprintf(stderr," original 'a'\n");
	BN_add(r,a,r);
BN_print(stderr,r); fprintf(stderr," and?\n");
exit(1);
*/

	if (!BN_copy(r,a)) return(0);

/*
	nm=BN_num_bits(rem);
	nd=BN_num_bits(d);
	if (!BN_lshift(dv,d,nm-nd)) return(0);
	for (i=nm-nd; i>=0; i--)
		{
		if (BN_cmp(rem,dv) >= 0)
			{
			if (!BN_sub(rem,rem,dv)) return(0);
			}
		if (!BN_rshift1(dv,dv)) return(0);
		}
*/
	return(1);
	}

