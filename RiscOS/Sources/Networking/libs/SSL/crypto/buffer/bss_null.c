/* crypto/buffer/bss_null.c */
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

#ifndef NOPROTO
static int null_write(BIO *h,char *buf,int num);
static int null_read(BIO *h,char *buf,int size);
static int null_puts(BIO *h,char *str);
static int null_gets(BIO *h,char *str,int size);
static long null_ctrl(BIO *h,int cmd,long arg1,char *arg2);
static int null_new(BIO *h);
static int null_free(BIO *data);
#else
static int null_write();
static int null_read();
static int null_puts();
static int null_gets();
static long null_ctrl();
static int null_new();
static int null_free();
#endif

static BIO_METHOD null_method=
	{
	BIO_TYPE_NULL,"NULL",
	null_write,
	null_read,
	null_puts,
	null_gets,
	null_ctrl,
	null_new,
	null_free,
	};

BIO_METHOD *BIO_s_null()
	{
	return(&null_method);
	}

static int null_new(bi)
BIO *bi;
	{
	bi->init=1;
	bi->num=0;
	bi->ptr=(NULL);
	return(1);
	}

static int null_free(a)
BIO *a;
	{
	if (a == NULL) return(0);
	return(1);
	}
	
static int null_read(b,out,outl)
BIO *b;
char *out;
int outl;
	{
	return(0);
	}

static int null_write(b,in,inl)
BIO *b;
char *in;
int inl;
	{
	return(inl);
	}

static long null_ctrl(b,cmd,num,ptr)
BIO *b;
int cmd;
long num;
char *ptr;
	{
	long ret=1;

	switch (cmd)
		{
	case BIO_CTRL_RESET:
	case BIO_CTRL_EOF:
	case BIO_CTRL_SET:
	case BIO_CTRL_SET_CLOSE:
	case BIO_CTRL_FLUSH:
		ret=1;
		break;
	case BIO_CTRL_GET_CLOSE:
	case BIO_CTRL_INFO:
	case BIO_CTRL_GET:
	case BIO_CTRL_PENDING:
	case BIO_CTRL_SHOULD_RETRY:
	case BIO_CTRL_RETRY_TYPE:
	default:
		ret=0;
		break;
		}
	return(ret);
	}

static int null_gets(bp,buf,size)
BIO *bp;
char *buf;
int size;
	{
	return(0);
	}

static int null_puts(bp,str)
BIO *bp;
char *str;
	{
	if (str == NULL) return(0);
	return(strlen(str));
	}

