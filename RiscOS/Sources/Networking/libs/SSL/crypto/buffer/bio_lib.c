/* crypto/buffer/bio_lib.c */
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
#include <errno.h>
#include "cryptlib.h"
#include "buffer.h"

BIO *BIO_new(method)
BIO_METHOD *method;
	{
	BIO *ret=NULL;

	ret=(BIO *)malloc(sizeof(BIO));
	if (ret == NULL)
		{
		BUFerr(BUF_F_BIO_NEW,ERR_R_MALLOC_FAILURE);
		return(NULL);
		}
	ret->method=method;
	ret->callback=NULL;
	ret->cb_arg=NULL;
	ret->init=0;
	ret->shutdown=1;
	ret->num=0;
	ret->flags=0;
	ret->ptr=NULL;
	if (method->create != NULL)
		if (!method->create(ret))
			{
			ret=NULL;
			free(ret);
			}
	return(ret);
	}

int BIO_free(a)
BIO *a;
	{
	int ret=0,i;

	if ((a->callback != NULL) &&
		((i=(int)a->callback(a,BIO_CB_FREE,NULL,0,0L,0L)) <= 0))
			return(i);

	if (a->method->destroy == NULL) return(1);
	ret=a->method->destroy(a);
	free(a);
	return(1);
	}

int BIO_read(b,out,outl)
BIO *b;
char *out;
int outl;
	{
	int i;

	if ((b == NULL) || (b->method == NULL) || (b->method->bread == NULL))
		{
		BUFerr(BUF_F_BIO_READ,BUF_R_UNSUPPORTED_METHOD);
		return(-2);
		}

	if ((b->callback != NULL) &&
		((i=(int)b->callback(b,BIO_CB_READ,out,outl,0L,0L)) <= 0))
			return(i);

	if (!b->init)
		{
		BUFerr(BUF_F_BIO_READ,BUF_R_UNINITALISED);
		return(-2);
		}

	i=b->method->bread(b,out,outl);

	if (b->callback != NULL)
		i=(int)b->callback(b,BIO_CB_READ|BIO_CB_RETURN,out,outl,
			0L,(long)i);
	return(i);
	}

int BIO_write(b,in,inl)
BIO *b;
char *in;
int inl;
	{
	int i;

	if ((b == NULL) || (b->method == NULL) || (b->method->bwrite == NULL))
		{
		BUFerr(BUF_F_BIO_WRITE,BUF_R_UNSUPPORTED_METHOD);
		return(-2);
		}

	if ((b->callback != NULL) &&
		((i=(int)b->callback(b,BIO_CB_WRITE,in,inl,0L,0L)) <= 0))
			return(i);

	if (!b->init)
		{
		BUFerr(BUF_F_BIO_WRITE,BUF_R_UNINITALISED);
		return(-2);
		}

	i=b->method->bwrite(b,in,inl);

	if (b->callback != NULL)
		i=(int)b->callback(b,BIO_CB_WRITE|BIO_CB_RETURN,in,inl,
			0L,(long)i);
	return(i);
	}

int BIO_puts(b,in)
BIO *b;
char *in;
	{
	int i;

	if ((b == NULL) || (b->method == NULL) || (b->method->bputs == NULL))
		{
		BUFerr(BUF_F_BIO_PUTS,BUF_R_UNSUPPORTED_METHOD);
		return(-2);
		}

	if ((b->callback != NULL) &&
		((i=(int)b->callback(b,BIO_CB_PUTS,in,0,0L,0L)) <= 0))
			return(i);

	if (!b->init)
		{
		BUFerr(BUF_F_BIO_PUTS,BUF_R_UNINITALISED);
		return(-2);
		}

	i=b->method->bputs(b,in);

	if (b->callback != NULL)
		i=(int)b->callback(b,BIO_CB_PUTS|BIO_CB_RETURN,in,0,
			0L,(long)i);
	return(i);
	}

int BIO_gets(b,in,inl)
BIO *b;
char *in;
int inl;
	{
	int i;

	if ((b == NULL) || (b->method == NULL) || (b->method->bgets == NULL))
		{
		BUFerr(BUF_F_BIO_GETS,BUF_R_UNSUPPORTED_METHOD);
		return(-2);
		}

	if ((b->callback != NULL) &&
		((i=(int)b->callback(b,BIO_CB_GETS,in,inl,0L,0L)) <= 0))
			return(i);

	if (!b->init)
		{
		BUFerr(BUF_F_BIO_GETS,BUF_R_UNINITALISED);
		return(-2);
		}

	i=b->method->bgets(b,in,inl);

	if (b->callback != NULL)
		i=(int)b->callback(b,BIO_CB_GETS|BIO_CB_RETURN,in,inl,
			0L,(long)i);
	return(i);
	}

long BIO_ctrl(b,cmd,larg,parg)
BIO *b;
int cmd;
long larg;
char *parg;
	{
	long ret;

	if ((b == NULL) || (b->method == NULL) || (b->method->ctrl == NULL))
		{
		BUFerr(BUF_F_BIO_CTRL,BUF_R_UNSUPPORTED_METHOD);
		return(-2);
		}

	if ((b->callback != NULL) &&
		((ret=b->callback(b,BIO_CB_CTRL,parg,cmd,larg,0L)) <= 0))
			return(ret);

	ret=b->method->ctrl(b,cmd,larg,parg);

	if (b->callback != NULL)
		ret=b->callback(b,BIO_CB_CTRL|BIO_CB_RETURN,parg,cmd,
			larg,ret);
	return(ret);
	}

