/* crypto/dh/dh_key.c */
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
#include "rand.h"
#include "dh.h"

int DH_generate_key(dh)
DH *dh;
	{
	int ok=0;
	unsigned int i;
	BN_CTX *ctx=NULL;
	BIGNUM *public=NULL,*private=NULL;

	ctx=BN_CTX_new();
	if (ctx == NULL) goto err;

	if (dh->private == NULL)
		{
		i=dh->length;
		if (i == 0)
			{
			/* Make the number p-1 bits long */
			i=BN_num_bits(dh->p)-1;
			}
		private=BN_new();
		if (private == NULL) goto err;
		if (!BN_rand(private,i,0,0)) goto err;
		}
	else
		private=dh->private;

	if (dh->public == NULL)
		{
		public=BN_new();
		if (public == NULL) goto err;
		}
	else
		public=dh->public;

	if (!BN_mod_exp(public,dh->g,private,dh->p,ctx)) goto err;
		
	dh->public=public;
	dh->private=private;
	ok=1;
err:
	if (ok != 1)
		DHerr(DH_F_DH_GENERATE_KEY,ERR_R_BN_LIB);

	if ((public != NULL)  && (dh->public == NULL))  BN_free(public);
	if ((private != NULL) && (dh->private == NULL)) BN_free(private);
	if (ctx != NULL) BN_CTX_free(ctx);
	return(ok);
	}

int DH_compute_key(key,public,dh)
unsigned char *key;
BIGNUM *public;
DH *dh;
	{
	BN_CTX *ctx;
	BIGNUM *tmp;
	int ret=-1;

	ctx=BN_CTX_new();
	if (ctx == NULL) goto err;
	tmp=ctx->bn[ctx->tos++];
	
	if (dh->private == NULL)
		{
		DHerr(DH_F_DH_COMPUTE_KEY,DH_R_NO_PRIVATE_VALUE);
		goto err;
		}
	if (!BN_mod_exp(tmp,public,dh->private,dh->p,ctx))
		{
		DHerr(DH_F_DH_COMPUTE_KEY,ERR_R_BN_LIB);
		goto err;
		}

	ret=BN_bn2bin(tmp,key);
err:
	if (ctx != NULL) BN_CTX_free(ctx);
	return(ret);
	}
