/* crypto/meth/meth_lib.c */
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
#include "stack.h"
#include "meth.h"

METHOD_CTX *meth_new(meth)
METHOD *meth;
	{
	METHOD_CTX *ret=NULL;

	if (meth == NULL)
		{
		METHerr(METH_F_METH_NEW,METH_R_NO_METH_STRUCTURE_PASSED);
		goto err;
		}
	if ((ret=(METHOD_CTX *)malloc(sizeof(METHOD_CTX))) == NULL)
		{
		METHerr(METH_F_METH_NEW,ERR_R_MALLOC_FAILURE);
		goto err;
		}
	ret->method=meth;
	ret->flags=0;
	ret->error=0;
	if (((ret->arg_type=sk_new(NULL)) == NULL) ||
		((ret->args=sk_new(NULL)) == NULL))
		{
		METHerr(METH_F_METH_NEW,ERR_R_MALLOC_FAILURE);
		goto err;
		}
	ret->state=NULL;
	return(ret);
err:
	if (ret != NULL) sk_free(ret->args);
	return(NULL);
	}

int meth_arg(ctx,type,arg)
METHOD_CTX *ctx;
int type;
char *arg;
	{
	int i;

	ctx->flags|=METH_FLAG_ACTIVE;
	i=sk_push(ctx->arg_type,(char *)type);
	if (!i) return(0);
	i=sk_push(ctx->args,arg);
	return(i);
	}

int meth_call(ctx,t,arg,rp)
METHOD_CTX *ctx;
int t;
char *arg;
char **rp;
	{
	int ret;
	METHOD *m;
	int type,stype;

	type=METH_TYPE(t);
	stype=METH_SUB_TYPE(t);
	m=ctx->method;
	if (m->num_func <= type)
		{
		METHerr(METH_F_METH_CALL,METH_R_FUNCTION_NUMBER_TO_LARGE);
		return(0);
		}
	if (m->func[type] == NULL)
		{
		METHerr(METH_F_METH_CALL,METH_R_INVALID_FUNCTION);
		return(0);
		}
	ret=m->func[type](ctx,stype,arg,rp);
	if (type == METH_FREE)
		{
		sk_free(ctx->args);
		free(ctx);
		}
	return(ret);
	}
