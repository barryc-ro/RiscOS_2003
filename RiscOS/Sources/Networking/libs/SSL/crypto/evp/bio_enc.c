/* crypto/evp/bio_enc.c */
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
#include "envelope.h"

/* BIO_put encrypts
 * BIO_get decrypts
 * BIO_gets returns the cipher strucutre
 */

#ifndef NOPROTO
static int enc_write(BIO *h,char *buf,int num);
static int enc_read(BIO *h,char *buf,int size);
/*static int enc_puts(BIO *h,char *str); */
static int enc_gets(BIO *h,char *str,int size);
static long enc_ctrl(BIO *h,int cmd,long arg1,char *arg2);
static int enc_new(BIO *h);
static int enc_free(BIO *data);
#else
static int enc_write();
static int enc_read();
/*static int enc_puts(); */
static int enc_gets();
static long enc_ctrl();
static int enc_new();
static int enc_free();
#endif

typedef struct enc_struct
	{
	BIO *bio;		/* the bio for IO */
	EVP_MD_CTX enc_ctx;	/* the digest to use */
	} BIO_MD_CTX;

static BIO_METHOD methods_md=
	{
	BIO_TYPE_MEM,"message digest",
	enc_write,
	enc_read,
	NULL, /* enc_puts, */
	enc_gets,
	enc_ctrl,
	enc_new,
	enc_free,
	};

BIO_METHOD *BIO_md()
	{
	return(&methods_md);
	}

static int enc_new(bi)
BIO *bi;
	{
	BIO_MD_CTX *ctx;

	ctx=(BIO_MD_CTX *)malloc(sizeof(BIO_MD_CTX));
	if (ctx == NULL) return(0);

	bi->init=0;
	bi->ptr=(char *)ctx;
	bi->flags=0;
	return(1);
	}

static int enc_free(a)
BIO *a;
	{
	if (a == NULL) return(0);
	free(a->ptr);
	a->ptr=NULL;
	a->init=0;
	a->flags=0;
	return(1);
	}
	
static int enc_read(b,out,outl)
BIO *b;
char *out;
int outl;
	{
	int ret=0;
	BIO_MD_CTX *ctx;

	if (out == NULL) return(0);
	ctx=(BIO_MD_CTX *)b->ptr;

	if ((ctx == NULL) || (ctx->bio == NULL)) return(0);

	ret=BIO_read(ctx->bio,out,outl);
	if (b->init)
		{
		if (ret > 0)
			{
			EVP_DigestUpdate(&(ctx->enc_ctx),(unsigned char *)out,
				(unsigned int)ret);
			}
		}
	return(ret);
	}

static int enc_write(b,in,inl)
BIO *b;
char *in;
int inl;
	{
	int ret=0;
	BIO_MD_CTX *ctx;

	if ((in == NULL) || (inl <= 0)) return(0);
	ctx=(BIO_MD_CTX *)b->ptr;

fprintf(stderr,"*");
	/* are we actually need to do the startup protocol */
	if ((ctx != NULL) && (ctx->bio != NULL))
		ret=BIO_write(ctx->bio,in,inl);
	if (b->init)
		{
		if (ret > 0)
			{
			EVP_DigestUpdate(&(ctx->enc_ctx),(unsigned char *)in,
				(unsigned int)ret);
			}
		}
	return(ret);
	}

static long enc_ctrl(b,cmd,num,ptr)
BIO *b;
int cmd;
long num;
char *ptr;
	{
	BIO_MD_CTX *ctx;
	EVP_MD **ppmd;
	EVP_MD *md;
	long ret=1;

	ctx=(BIO_MD_CTX *)b->ptr;

	switch (cmd)
		{
	case BIO_CTRL_RESET:
		if (b->init)
			EVP_DigestInit(&(ctx->enc_ctx),ctx->enc_ctx.digest);
		else
			ret=0;
		ret=BIO_ctrl(ctx->bio,cmd,num,ptr);
		break;
	case BIO_CTRL_INFO:
	case BIO_CTRL_GET:
		if (b->init)
			{
			ppmd=(EVP_MD **)ptr;
			*ppmd=ctx->enc_ctx.digest;
			}
		else
			ret=0;
		break;
	case BIO_CTRL_SET:
		if (num == 0)
			ctx->bio=(BIO *)ptr;
		else if (num == 1)
			{
			md=(EVP_MD *)ptr;
			EVP_DigestInit(&(ctx->enc_ctx),md);
			b->init=1;
			}
		break;
	default:
		ret=BIO_ctrl(ctx->bio,cmd,num,ptr);
		break;
		}
	return(ret);
	}

static int enc_gets(bp,buf,size)
BIO *bp;
char *buf;
int size;
	{
	BIO_MD_CTX *ctx;
	unsigned int ret;


	ctx=(BIO_MD_CTX *)bp->ptr;
	if (size < ctx->enc_ctx.digest->enc_size)
		return(0);
	EVP_DigestFinal(&(ctx->enc_ctx),(unsigned char *)buf,&ret);
	return((int)ret);
	}

/*
static int enc_puts(bp,str)
BIO *bp;
char *str;
	{
	return(-1);
	}
*/

