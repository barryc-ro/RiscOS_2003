/* crypto/buffer/bss_sock.c */
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

#if !defined(NO_SOCK) || defined(BIO_FD)

#include <stdio.h>
#include <errno.h>
#define USE_SOCKETS
#include "cryptlib.h"
#include "buffer.h"

#ifndef BIO_FD
#ifndef NOPROTO
static int sock_write(BIO *h,char *buf,int num);
static int sock_read(BIO *h,char *buf,int size);
static int sock_puts(BIO *h,char *str);
static long sock_ctrl(BIO *h,int cmd,long arg1,char *arg2);
static int sock_new(BIO *h);
static int sock_free(BIO *data);
static int sock_should_retry(int s);
#else
static int sock_write();
static int sock_read();
static int sock_puts();
static long sock_ctrl();
static int sock_new();
static int sock_free();
static int sock_should_retry();
#endif

#else

#ifndef NOPROTO
static int fd_write(BIO *h,char *buf,int num);
static int fd_read(BIO *h,char *buf,int size);
static int fd_puts(BIO *h,char *str);
static long fd_ctrl(BIO *h,int cmd,long arg1,char *arg2);
static int fd_new(BIO *h);
static int fd_free(BIO *data);
static int fd_should_retry(int s);
#else
static int fd_write();
static int fd_read();
static int fd_puts();
static long fd_ctrl();
static int fd_new();
static int fd_free();
static int fd_should_retry();
#endif
#endif

#ifndef BIO_FD
static BIO_METHOD methods_sockp=
	{
	BIO_TYPE_MEM,"socket",
	sock_write,
	sock_read,
	sock_puts,
	NULL, /* sock_gets, */
	sock_ctrl,
	sock_new,
	sock_free,
	};

BIO_METHOD *BIO_s_socket()
	{
	return(&methods_sockp);
	}
#else
static BIO_METHOD methods_fdp=
	{
	BIO_TYPE_MEM,"file descriptor",
	fd_write,
	fd_read,
	fd_puts,
	NULL, /* fd_gets, */
	fd_ctrl,
	fd_new,
	fd_free,
	};

BIO_METHOD *BIO_s_fd()
	{
	return(&methods_fdp);
	}
#endif

#ifndef BIO_FD
static int sock_new(bi)
#else
static int fd_new(bi)
#endif
BIO *bi;
	{
	bi->init=0;
	bi->num=0;
	bi->ptr=NULL;
	bi->flags=0;
	return(1);
	}

#ifndef BIO_FD
static int sock_free(a)
#else
static int fd_free(a)
#endif
BIO *a;
	{
	if (a == NULL) return(0);
	if (a->shutdown)
		{
		if (a->init)
			{
#ifndef BIO_FD
			shutdown(a->num,2);
#ifdef WINDOWS
			closesocket(a->num);
#endif
#else
			close(a->num);
#endif

			}
		a->init=0;
		a->flags=0;
		}
	return(1);
	}
	
#ifndef BIO_FD
static int sock_read(b,out,outl)
#else
static int fd_read(b,out,outl)
#endif
BIO *b;
char *out;
int outl;
	{
	int ret=0;

	if (out != NULL)
		{
#if defined(WINDOWS) || !defined(BIO_FD)
		ret=recv(b->num,out,outl,0);
#else
		ret=read(b->num,out,outl);
#endif
		b->flags&= ~(BIO_FLAGS_RW|BIO_FLAGS_SHOULD_RETRY);
		if (ret <= 0)
			{
			b->flags|= BIO_FLAGS_READ;
#ifndef BIO_FD
			b->flags|=(sock_should_retry(b->num)?
				BIO_FLAGS_SHOULD_RETRY:0);
#else
			b->flags|=(fd_should_retry(b->num)?
				BIO_FLAGS_SHOULD_RETRY:0);
#endif
			}
		}
	return(ret);
	}

#ifndef BIO_FD
static int sock_write(b,in,inl)
#else
static int fd_write(b,in,inl)
#endif
BIO *b;
char *in;
int inl;
	{
	int ret;
	
#if defined(WINDOWS) && !defined(BIO_FD)
	ret=send(b->num,in,inl,0);
#else
	ret=write(b->num,in,inl);
#endif
	b->flags&= ~(BIO_FLAGS_RW|BIO_FLAGS_SHOULD_RETRY);
	if (ret <= 0)
		{
		b->flags|=BIO_FLAGS_WRITE;
#ifndef BIO_FD
		b->flags|=(sock_should_retry(b->num)?BIO_FLAGS_SHOULD_RETRY:0);
#else
		b->flags|=(fd_should_retry(b->num)?BIO_FLAGS_SHOULD_RETRY:0);
#endif
		}
	return(ret);
	}

#ifndef BIO_FD
static long sock_ctrl(b,cmd,num,ptr)
#else
static long fd_ctrl(b,cmd,num,ptr)
#endif
BIO *b;
int cmd;
long num;
char *ptr;
	{
	long ret=1;
	int *ip;

	switch (cmd)
		{
	case BIO_CTRL_RESET:
#ifdef BIO_FD
		ret=(long)lseek(b->num,0,0);
#else
		ret=0;
#endif
		break;
	case BIO_CTRL_INFO:
		ret=0;
		break;
	case BIO_CTRL_SET:
#ifndef BIO_FD
		sock_free(b);
#else
		fd_free(b);
#endif
		b->num= *((int *)ptr);
		b->shutdown=(int)num;
		b->init=1;
		break;
	case BIO_CTRL_GET:
		if (b->init)
			{
			ip=(int *)ptr;
			if (ip == NULL)
				ret=0;
			else
				*ip=b->num;
			}
		break;
	case BIO_CTRL_GET_CLOSE:
		ret=b->shutdown;
		break;
	case BIO_CTRL_SET_CLOSE:
		b->shutdown=(int)num;
		break;
	case BIO_CTRL_PENDING:
		ret=0;
		break;
	case BIO_CTRL_FLUSH:
		break;
	case BIO_CTRL_SHOULD_RETRY:
		ret=(long)BIO_should_retry(b);
		break;
	case BIO_CTRL_RETRY_TYPE:
		ret=(long)BIO_retry_type(b);
		break;
	default:
		ret=0;
		break;
		}
	return(ret);
	}

#ifdef undef
static int sock_gets(bp,buf,size)
BIO *bp;
char *buf;
int size;
	{
	return(-1);
	}
#endif

#ifndef BIO_FD
static int sock_puts(bp,str)
#else
static int fd_puts(bp,str)
#endif
BIO *bp;
char *str;
	{
	int n,ret;

	n=strlen(str);
#ifndef BIO_FD
	ret=sock_write(bp,str,n);
#else
	ret=fd_write(bp,str,n);
#endif
	return(ret);
	}

#ifndef BIO_FD
static int sock_should_retry(i)
#else
static int fd_should_retry(i)
#endif
int i;
	{
	if ((i == 0) || (i == -1))
		{
#ifndef BIO_FD
#ifdef WINDOWS
		errno=WSAGetLastError();
#endif
#ifdef WSAEWOULDBLOCK
		if (WSAEWOULDBLOCK == errno) return(1);
#endif
#endif
#ifdef EWOULDBLOCK
		if (EWOULDBLOCK == errno) return(1);
#endif
#ifdef EINTR
		if (EINTR == errno) return(1);
#endif
#ifdef EAGAIN
		if (EAGAIN == errno) return(1);
#endif
#ifdef EPROTO
		if (EPROTO == errno) return(1);
#endif
		}
	return(0);
	}

#endif
