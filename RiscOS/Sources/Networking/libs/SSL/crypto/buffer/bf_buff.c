/* crypto/buffer/bf_buff.c */
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
static int buffer_write(BIO *h,char *buf,int num);
static int buffer_read(BIO *h,char *buf,int size);
static int buffer_puts(BIO *h,char *str);
static int buffer_gets(BIO *h,char *str,int size);
static long buffer_ctrl(BIO *h,int cmd,long arg1,char *arg2);
static int buffer_new(BIO *h);
static int buffer_free(BIO *data);
#else
static int buffer_write();
static int buffer_read();
static int buffer_puts();
static int buffer_gets();
static long buffer_ctrl();
static int buffer_new();
static int buffer_free();
#endif

#define DEFAULT_BUFFER_SIZE	1024

static BIO_METHOD methods_buffer=
	{
	BIO_TYPE_BUFFER,"buffer",
	buffer_write,
	buffer_read,
	buffer_puts,
	buffer_gets,
	buffer_ctrl,
	buffer_new,
	buffer_free,
	};

BIO_METHOD *BIO_f_buffer()
	{
	return(&methods_buffer);
	}

static int buffer_new(bi)
BIO *bi;
	{
	BIO_F_BUFFER_CTX *ctx;

	ctx=(BIO_F_BUFFER_CTX *)malloc(sizeof(BIO_F_BUFFER_CTX));
	if (ctx == NULL) return(0);
	ctx->bio=NULL;
	ctx->ibuf=(char *)malloc(DEFAULT_BUFFER_SIZE);
	if (ctx->ibuf == NULL) { free(ctx); return(0); }
	ctx->obuf=(char *)malloc(DEFAULT_BUFFER_SIZE);
	if (ctx->obuf == NULL) { free(ctx->ibuf); free(ctx); return(0); }
	ctx->buf_size=DEFAULT_BUFFER_SIZE;
	ctx->ibuf_len=0;
	ctx->ibuf_off=0;
	ctx->obuf_len=0;
	ctx->obuf_off=0;

	bi->init=0;
	bi->ptr=(char *)ctx;
	bi->flags=0;
	return(1);
	}

static int buffer_free(a)
BIO *a;
	{
	BIO_F_BUFFER_CTX *b;

	if (a == NULL) return(0);
	b=(BIO_F_BUFFER_CTX *)a->ptr;
	if (b->ibuf != NULL) free(b->ibuf);
	if (b->obuf != NULL) free(b->obuf);
	free(a->ptr);
	a->ptr=NULL;
	a->init=0;
	a->flags=0;
	return(1);
	}
	
static int buffer_read(b,out,outl)
BIO *b;
char *out;
int outl;
	{
	int i,num=0;
	BIO_F_BUFFER_CTX *ctx;

	if (out == NULL) return(0);
	ctx=(BIO_F_BUFFER_CTX *)b->ptr;

	if ((ctx == NULL) || (ctx->bio == NULL)) return(0);
	num=0;

start:
	i=ctx->ibuf_len;
	/* If there is stuff left over, grab it */
	if (i != 0)
		{
		if (i > outl) i=outl;
		memcpy(out,&(ctx->ibuf[ctx->ibuf_off]),i);
		ctx->ibuf_off+=i;
		ctx->ibuf_len-=i;
		num+=i;
		if (outl == i)  return(num);
		outl-=i;
		out+=i;
		}

	/* We may have done a partial read. try to do more.
	 * We have nothing in the buffer.
	 * If we get an error and have read some data, just return it
	 * and let them retry to get the error again.
	 * copy direct to parent address space */
	if (outl > ctx->buf_size)
		{
		for (;;)
			{
			i=BIO_read(ctx->bio,out,outl);
			if (i < 0) return((num > 0)?num:i);
			if (i == 0) return(num);
			num+=i;
			if (outl == i) return(num);
			out+=i;
			outl-=i;
			}
		}
	/* else */

	/* we are going to be doing some buffering */
	i=BIO_read(ctx->bio,ctx->ibuf,ctx->buf_size);
	if (i < 0) return((num > 0)?num:i);
	if (i == 0) return(num);
	ctx->ibuf_off=0;
	ctx->ibuf_len=i;

	/* Lets re-read using ourselves :-) */
	goto start;
	}

static int buffer_write(b,in,inl)
BIO *b;
char *in;
int inl;
	{
	int i,num=0;
	BIO_F_BUFFER_CTX *ctx;

	if ((in == NULL) || (inl <= 0)) return(0);
	ctx=(BIO_F_BUFFER_CTX *)b->ptr;
	if ((ctx == NULL) || (ctx->bio == NULL)) return(0);

start:
	i=ctx->buf_size-(ctx->obuf_len+ctx->obuf_off);
	/* add to buffer and return */
	if (i > inl)
		{
		memcpy(&(ctx->obuf[ctx->obuf_off+ctx->obuf_len]),in,inl);
		ctx->obuf_len+=inl;
		return(inl);
		}
	/* else */
	/* stuff already in buffer, so add to it first, then flush */
	if (ctx->obuf_len != 0)
		{
		if (i > 0) /* lets fill it up if we can */
			{
			memcpy(&(ctx->obuf[ctx->obuf_off+ctx->obuf_len]),in,i);
			in+=i;
			inl-=i;
			num+=i;
			ctx->obuf_len+=i;
			}
		/* we now have a full buffer needing flushing */
		for (;;)
			{
			i=BIO_write(ctx->bio,&(ctx->obuf[ctx->obuf_off]),
				ctx->obuf_len);
			if (i < 0) return((num > 0)?num:i);
			if (i == 0) return(num);
			ctx->obuf_off+=i;
			ctx->obuf_len-=i;
			if (ctx->obuf_len == 0) break;
			}
		}
	/* we only get here if the buffer has been flushed and we
	 * still have stuff to write */
	ctx->obuf_off=0;

	/* we now have inl bytes to write */
	while (inl >= ctx->buf_size)
		{
		i=BIO_write(ctx->bio,in,inl);
		if (i < 0) return((num > 0)?num:i);
		if (i == 0) return(num);
		num+=i;
		in+=i;
		inl-=i;
		if (inl == 0) return(num);
		}

	/* copy the rest into the buffer since we have only a small 
	 * amount left */
	goto start;
	}

static long buffer_ctrl(b,cmd,num,ptr)
BIO *b;
int cmd;
long num;
char *ptr;
	{
	BIO_F_BUFFER_CTX *ctx;
	long ret=1;
	char *p1,*p2;
	int r;

	ctx=(BIO_F_BUFFER_CTX *)b->ptr;

	switch (cmd)
		{
	case BIO_CTRL_RESET:
		ctx->ibuf_off=0;
		ctx->ibuf_len=0;
		ctx->obuf_off=0;
		ctx->obuf_len=0;
		ret=BIO_ctrl(ctx->bio,cmd,num,ptr);
		break;
	case BIO_CTRL_INFO:
	case BIO_CTRL_GET:
	case BIO_CTRL_PENDING:
		ret=(long)ctx->ibuf_len;
		break;
	case BIO_CTRL_SET:
		if ((num > DEFAULT_BUFFER_SIZE) && (num != ctx->buf_size))
			{
			p1=(char *)malloc((int)num);
			p2=(char *)malloc((int)num);
			if ((p1 == NULL) || (p2 == NULL))
				{
				if (p1 != NULL) free(p1);
				ret=0;
				}
			else
				{
				free(ctx->ibuf);
				ctx->ibuf=p1;
				ctx->ibuf_off=0;
				ctx->ibuf_len=0;

				free(ctx->obuf);
				ctx->obuf=p2;
				ctx->obuf_off=0;
				ctx->obuf_len=0;
				ctx->buf_size=(int)num;
				}
			}
		break;
	case BIO_CTRL_FLUSH:
		r=BIO_write(ctx->bio,&(ctx->obuf[ctx->obuf_off]),
			ctx->obuf_len);
		if (ret <= 0) return((long)r);
		if (ret == ctx->obuf_len)
			{
			ctx->obuf_len=0;
			ctx->obuf_off=0;
			}
		else
			{
			ctx->obuf_len-=r;
			ctx->obuf_off+=r;
			}
		ret=(long)r;
		break;
	default:
		ret=BIO_ctrl(ctx->bio,cmd,num,ptr);
		break;
		}
	return(ret);
	}

static int buffer_gets(bp,buf,size)
BIO *bp;
char *buf;
int size;
	{
	BIO_F_BUFFER_CTX *ctx;
	int num=0,i;
	char *p;

	ctx=(BIO_F_BUFFER_CTX *)bp->ptr;
	size--;

	for (;;)
		{
		p= &(ctx->ibuf[ctx->ibuf_off]);
		for (i=0; (i<ctx->ibuf_len) && (i<(size-1)); i++)
			{
			*(buf++)=p[i];
			if (p[i] == '\n') break;
			}
		num+=i;
		size-=i;
		ctx->ibuf_len-=i;
		ctx->ibuf_off+=i;
		if (p[i] == '\n')
			{
			buf[i+1]='\0';
			return(num);
			}
		else	/* read another chunk */
			{
			i=BIO_read(ctx->bio,ctx->ibuf,ctx->buf_size);
			if (i < 0) return((num > 0)?num:i);
			if (i == 0) return(num);
			ctx->ibuf_len=i;
			ctx->ibuf_off=0;
			}
		}
	}

static int buffer_puts(bp,str)
BIO *bp;
char *str;
	{
	return(buffer_write(bp,str,strlen(str)));
	}

