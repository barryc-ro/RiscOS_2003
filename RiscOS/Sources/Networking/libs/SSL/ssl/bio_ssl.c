/* ssl/bio_ssl.c */
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
#include <string.h>
#include <errno.h>
#include "buffer.h"
#include "err.h"
#include "ssl.h"

#ifndef NOPROTO
static int ssl_write(BIO *h,char *buf,int num);
static int ssl_read(BIO *h,char *buf,int size);
static int ssl_puts(BIO *h,char *str);
static long ssl_ctrl(BIO *h,int cmd,long arg1,char *arg2);
static int ssl_new(BIO *h);
static int ssl_free(BIO *data);
static int ssl_startup(BIO *b);
#else
static int ssl_write();
static int ssl_read();
static int ssl_puts();
static long ssl_ctrl();
static int ssl_new();
static int ssl_free();
static int ssl_startup();
#endif

static BIO_METHOD methods_sslp=
	{
	BIO_TYPE_SSL,"ssl",
	ssl_write,
	ssl_read,
	ssl_puts,
	NULL, /* ssl_gets, */
	ssl_ctrl,
	ssl_new,
	ssl_free,
	};

BIO_METHOD *BIO_f_ssl()
	{
	return(&methods_sslp);
	}

static int ssl_new(bi)
BIO *bi;
	{
	bi->init=0;
	bi->ptr=NULL;	/* The SSL structure */
	bi->flags=0;
	return(1);
	}

static int ssl_free(a)
BIO *a;
	{
	if (a == NULL) return(0);
	if (a->shutdown)
		{
		if (a->init) SSL_free((SSL *)a->ptr);
		a->init=0;
		a->flags=0;
		a->ptr=NULL;
		}
	return(1);
	}
	
static int ssl_read(b,out,outl)
BIO *b;
char *out;
int outl;
	{
	int ret=0;
	SSL *ssl;

	if (out == NULL) return(0);
	ssl=(SSL *)b->ptr;

	/* are we actually need to do the startup protocol */
	if (!SSL_is_init_finished(ssl))
		{
		ret=ssl_startup(b);
		if (ret <= 0) return(ret);
		}

	/* connection is establish, lets do a read */
	ret=SSL_read(ssl,out,outl);

	if (ret <= 0)
		{
		b->flags&= ~(BIO_FLAGS_RW|BIO_FLAGS_SHOULD_RETRY);
		b->flags|= BIO_FLAGS_READ;
		b->flags|=(BIO_should_retry(ssl->rbio)?
			BIO_FLAGS_SHOULD_RETRY:0);
		}
	return(ret);
	}

static int ssl_write(b,in,inl)
BIO *b;
char *in;
int inl;
	{
	int ret;
	SSL *ssl;

	ssl=(SSL *)b->ptr;

	/* are we actually need to do the startup protocol */
	if (!SSL_is_init_finished(ssl))
		{
		ret=ssl_startup(b);
		if (ret <= 0) return(ret);
		}

	ret=SSL_write(ssl,in,inl);

	if (ret <= 0)
		{
		b->flags&= ~(BIO_FLAGS_RW|BIO_FLAGS_SHOULD_RETRY);
		b->flags|=BIO_FLAGS_WRITE;
		b->flags|=(BIO_should_retry(ssl->wbio)?
			BIO_FLAGS_SHOULD_RETRY:0);
		}
	return(ret);
	}

static long ssl_ctrl(b,cmd,num,ptr)
BIO *b;
int cmd;
long num;
char *ptr;
	{
	SSL **sslp,*ssl;
	long ret=1;

	ssl=(SSL *)b->ptr;
	switch (cmd)
		{
	case BIO_CTRL_RESET:
		SSL_clear(ssl);
		break;
	case BIO_CTRL_EOF:
	case BIO_CTRL_INFO:
		ret=0;
		break;
	case BIO_CTRL_SET:
		ssl_free(b);
		b->shutdown=(int)num;
		b->ptr=ptr;
		b->init=1;
		break;
	case BIO_CTRL_GET:
		if (ptr != NULL)
			{
			sslp=(SSL **)ptr;
			*sslp=ssl;
			}
		break;
	case BIO_CTRL_GET_CLOSE:
		ret=b->shutdown;
		break;
	case BIO_CTRL_SET_CLOSE:
		b->shutdown=(int)num;
		break;
	case BIO_CTRL_PENDING:
		ret=SSL_pending(ssl);
		break;
	case BIO_CTRL_SHOULD_RETRY:
		ret=b->flags&BIO_FLAGS_SHOULD_RETRY;
		break;
	case BIO_CTRL_RETRY_TYPE:
		ret=b->flags&BIO_FLAGS_RW;
		break;
	case BIO_CTRL_FLUSH:
		ret=BIO_ctrl(ssl->wbio,cmd,num,ptr);
		break;
	default:
		return(0);
		break;
		}
	return(ret);
	}

#ifdef undef
static int ssl_gets(bp,buf,size)
BIO *bp;
char *buf;
int size;
	{
	return(-1);
	}
#endif

static int ssl_puts(bp,str)
BIO *bp;
char *str;
	{
	int n,ret;

	n=strlen(str);
	ret=ssl_write(bp,str,n);
	return(ret);
	}

static int ssl_startup(b)
BIO *b;
	{
	int ret,f;
	SSL *ssl;
	
	ssl=(SSL *)b->ptr;

	if (SSL_in_accept_init(ssl))
		{
		ret=SSL_accept(ssl);

		if (ret <= 0)
			{
			f=b->flags& ~(BIO_FLAGS_RW|BIO_FLAGS_SHOULD_RETRY);

			if (SSL_want_read(ssl))
				{
				f|=BIO_FLAGS_READ;
				if (BIO_should_retry(ssl->rbio))
					f|=BIO_FLAGS_SHOULD_RETRY;
				}
			if (SSL_want_write(ssl))
				{
				f|=BIO_FLAGS_WRITE;
				if (BIO_should_retry(ssl->wbio))
					f|=BIO_FLAGS_SHOULD_RETRY;
				}
			b->flags=f;
			return(ret);
			}
		/* Everything worked */
		}
	else if (SSL_in_connect_init(ssl))
		{
		ret=SSL_connect(ssl);

		if (ret <= 0)
			{
			f=b->flags& ~(BIO_FLAGS_RW|BIO_FLAGS_SHOULD_RETRY);

			if (SSL_want_read(ssl))
				{
				f|=BIO_FLAGS_READ;
				if (BIO_should_retry(ssl->rbio))
					f|=BIO_FLAGS_SHOULD_RETRY;
				}
			if (SSL_want_write(ssl))
				{
				f|=BIO_FLAGS_WRITE;
				if (BIO_should_retry(ssl->wbio))
					f|=BIO_FLAGS_SHOULD_RETRY;
				}
			b->flags=f;
			return(ret);
			}
		/* Everything worked */
		}
	else
		{
		/* Unknown state */
		SSLerr(SSL_F_SSL_STARTUP,SSL_R_UNDEFINED_INIT_STATE);
		return(-1);
		}
	return(1);
	}
