/* crypto/buffer/bss_file.c */
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

/*ferror()
feor() */

#include <stdio.h>
#include <errno.h>
#include "cryptlib.h"
#include "buffer.h"

#ifndef NOPROTO
static int MS_CALLBACK file_write(BIO *h,char *buf,int num);
static int MS_CALLBACK file_read(BIO *h,char *buf,int size);
static int MS_CALLBACK file_puts(BIO *h,char *str);
static int MS_CALLBACK file_gets(BIO *h,char *str,int size);
static long MS_CALLBACK file_ctrl(BIO *h,int cmd,long arg1,char *arg2);
static int MS_CALLBACK file_new(BIO *h);
static int MS_CALLBACK file_free(BIO *data);
#else
static int MS_CALLBACK file_write();
static int MS_CALLBACK file_read();
static int MS_CALLBACK file_puts();
static int MS_CALLBACK file_gets();
static long MS_CALLBACK file_ctrl();
static int MS_CALLBACK file_new();
static int MS_CALLBACK file_free();
#endif

static BIO_METHOD methods_filep=
	{
	BIO_TYPE_MEM,"FILE pointer",
	file_write,
	file_read,
	file_puts,
	file_gets,
	file_ctrl,
	file_new,
	file_free,
	};

#if !defined(WIN16) || defined(APPS_WIN16)
BIO_METHOD *BIO_s_file()
	{
	return(&methods_filep);
	}
#else
BIO_METHOD *BIO_s_file_internal_w16()
	{
	return(&methods_filep);
	}
#endif

static int MS_CALLBACK file_new(bi)
BIO *bi;
	{
	bi->init=0;
	bi->num=0;
	bi->ptr=NULL;
	return(1);
	}

static int MS_CALLBACK file_free(a)
BIO *a;
	{
	if (a == NULL) return(0);
	if (a->shutdown)
		{
		if ((a->init) && (a->ptr != NULL))
			{
			fclose((FILE *)a->ptr);
			a->ptr=NULL;
			}
		a->init=0;
		}
	return(1);
	}
	
static int MS_CALLBACK file_read(b,out,outl)
BIO *b;
char *out;
int outl;
	{
	int ret=0;

	if (b->init && (out != NULL))
		ret=fread(out,1,(int)outl,(FILE *)b->ptr);
	return(ret);
	}

static int MS_CALLBACK file_write(b,in,inl)
BIO *b;
char *in;
int inl;
	{
	int ret=0;

	if (b->init && (in != NULL))
		ret=fwrite(in,1,(int)inl,(FILE *)b->ptr);
	return(ret);
	}

static long MS_CALLBACK file_ctrl(b,cmd,num,ptr)
BIO *b;
int cmd;
long num;
char *ptr;
	{
	long ret=1;
	FILE *fp=(FILE *)b->ptr;
	FILE **fpp;
	char *p;

	switch (cmd)
		{
	case BIO_CTRL_RESET:
		ret=(long)fseek(fp,num,0);
		break;
	case BIO_CTRL_EOF:
		ret=(long)feof(fp);
		break;
	case BIO_CTRL_INFO:
		ret=ftell(fp);
		break;
	case BIO_CTRL_SET:
		file_free(b);
		b->shutdown=(int)num;
		b->ptr=(char *)ptr;
		b->init=1;
		break;
	case BIO_CTRL_SET_FILENAME:
		file_free(b);
		b->shutdown=(int)num&BIO_CLOSE;
		if (num & BIO_FP_APPEND)
			{
			if (num & BIO_FP_READ)
				p="a+";
			else	p="a";
			}
		else if ((num & BIO_FP_READ) && (num & BIO_FP_WRITE))
			p="r+";
		else if (num & BIO_FP_WRITE)
			p="w";
		else if (num & BIO_FP_READ)
			p="r";
		else
			{
			BUFerr(BUF_F_FILE_CTRL,BUF_R_BAD_FOPEN_MODE);
			ret=0;
			break;
			}
		fp=fopen(ptr,p);
		if (fp == NULL)
			{
			BUFerr(BUF_F_FILE_CTRL,ERR_R_SYS_LIB);
			ret=0;
			break;
			}
		b->ptr=(char *)fp;
		b->init=1;
		break;
	case BIO_CTRL_GET:
		/* the ptr parameter is actually a FILE ** in this case. */
		if (ptr != NULL)
			{
			fpp=(FILE **)ptr;
			*fpp=(FILE *)b->ptr;
			}
		break;
	case BIO_CTRL_GET_CLOSE:
		ret=(long)b->shutdown;
		break;
	case BIO_CTRL_SET_CLOSE:
		b->shutdown=(int)num;
		break;
	case BIO_CTRL_FLUSH:
		fflush((FILE *)b->ptr);
		break;
	case BIO_CTRL_PENDING:
	case BIO_CTRL_PUSH:
	case BIO_CTRL_POP:
	case BIO_CTRL_SHOULD_RETRY:
	case BIO_CTRL_RETRY_TYPE:
	default:
		ret=0;
		break;
		}
	return(ret);
	}

static int MS_CALLBACK file_gets(bp,buf,size)
BIO *bp;
char *buf;
int size;
	{
	int ret=0;

	buf[0]='\0';
	fgets(buf,size,(FILE *)bp->ptr);
	if (buf[0] != '\0')
		ret=strlen(buf);
	return(ret);
	}

static int MS_CALLBACK file_puts(bp,str)
BIO *bp;
char *str;
	{
	int n,ret;

	n=strlen(str);
	ret=file_write(bp,str,n);
	return(ret);
	}
