/* crypto/evp/bio_b64.c */
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

#ifndef NOPROTO
static int b64_write(BIO *h,char *buf,int num);
static int b64_read(BIO *h,char *buf,int size);
/*static int b64_puts(BIO *h,char *str); */
/*static int b64_gets(BIO *h,char *str,int size); */
static long b64_ctrl(BIO *h,int cmd,long arg1,char *arg2);
static int b64_new(BIO *h);
static int b64_free(BIO *data);
#else
static int b64_write();
static int b64_read();
/*static int b64_puts(); */
/*static int b64_gets(); */
static long b64_ctrl();
static int b64_new();
static int b64_free();
#endif

#define BLOCK_SIZE	1024

typedef struct b64_struct
	{
	BIO *bio;		/* the bio for IO */
	int putenc;		/* encode on put, can flip it */
	unsigned char buf[EVP_ENCODE_LENGTH(BLOCK_SIZE)+10]
	int buf_len;
	int buf_off;
	EVP_ENCODE_CTX base64;
	} BIO_B64_CTX;

static BIO_METHOD methods_b64=
	{
	BIO_TYPE_B64,"base64 encoding",
	b64_write,
	b64_read,
	NULL, /* b64_puts, */
	NULL, /* b64_gets, */
	b64_ctrl,
	b64_new,
	b64_free,
	};

BIO_METHOD *BIO_b64()
	{
	return(&methods_b64);
	}

static int b64_new(bi)
BIO *bi;
	{
	BIO_B64_CTX *ctx;

	ctx=(BIO_B64_CTX *)malloc(sizeof(BIO_B64_CTX));
	if (ctx == NULL) return(0);
	ctx

	bi->init=0;
	bi->ptr=(char *)ctx;
	bi->flags=0;
	return(1);
	}

static int b64_free(a)
BIO *a;
	{
	if (a == NULL) return(0);
	free(a->ptr);
	a->ptr=NULL;
	a->init=0;
	a->flags=0;
	return(1);
	}
	
static int b64_read(b,out,outl)
BIO *b;
char *out;
int outl;
	{
	int ret=0;
	BIO_B64_CTX *ctx;

	if (out == NULL) return(0);
	ctx=(BIO_B64_CTX *)b->ptr;

	if ((ctx == NULL) || (ctx->bio == NULL)) return(0);

	ret=BIO_read(ctx->bio,out,outl);
	if (b->init)
		{
		if (ret > 0)
			{
			EVP_DigestUpdate(&(ctx->b64_ctx),(unsigned char *)out,
				(unsigned int)ret);
			}
		}
	return(ret);
	}

static int b64_write(b,in,inl)
BIO *b;
char *in;
int inl;
	{
	int ret=0;
	BIO_B64_CTX *ctx;

	if ((in == NULL) || (inl <= 0)) return(0);
	ctx=(BIO_B64_CTX *)b->ptr;

fprintf(stderr,"*");
	/* are we actually need to do the startup protocol */
	if ((ctx != NULL) && (ctx->bio != NULL))
		ret=BIO_write(ctx->bio,in,inl);
	if (b->init)
		{
		if (ret > 0)
			{
			EVP_DigestUpdate(&(ctx->b64_ctx),(unsigned char *)in,
				(unsigned int)ret);
			}
		}
	return(ret);
	}

static long b64_ctrl(b,cmd,num,ptr)
BIO *b;
int cmd;
long num;
char *ptr;
	{
	BIO_B64_CTX *ctx;
	EVP_B64 **ppmd;
	EVP_B64 *md;
	long ret=1;

	ctx=(BIO_B64_CTX *)b->ptr;

	switch (cmd)
		{
	case BIO_CTRL_RESET:
		if (b->init)
			EVP_DigestInit(&(ctx->b64_ctx),ctx->b64_ctx.digest);
		else
			ret=0;
		ret=BIO_ctrl(ctx->bio,cmd,num,ptr);
		break;
	case BIO_CTRL_INFO:
	case BIO_CTRL_GET:
		if (b->init)
			{
			ppmd=(EVP_B64 **)ptr;
			*ppmd=ctx->b64_ctx.digest;
			}
		else
			ret=0;
		break;
	case BIO_CTRL_SET:
		if (num == 0)
			ctx->bio=(BIO *)ptr;
		else if (num == 1)
			{
			md=(EVP_B64 *)ptr;
			EVP_DigestInit(&(ctx->b64_ctx),md);
			b->init=1;
			}
		break;
	default:
		ret=BIO_ctrl(ctx->bio,cmd,num,ptr);
		break;
		}
	return(ret);
	}

static int b64_gets(bp,buf,size)
BIO *bp;
char *buf;
int size;
	{
	BIO_B64_CTX *ctx;
	unsigned int ret;


	ctx=(BIO_B64_CTX *)bp->ptr;
	if (size < ctx->b64_ctx.digest->b64_size)
		return(0);
	EVP_DigestFinal(&(ctx->b64_ctx),(unsigned char *)buf,&ret);
	return((int)ret);
	}

/*
static int b64_puts(bp,str)
BIO *bp;
char *str;
	{
	return(-1);
	}
*/

