/* apps/s_eio.c */
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
#include <stdlib.h>
#include <string.h>
#define USE_SOCKETS
#define NON_MAIN
#include "apps.h"
#undef NON_MAIN
#undef USE_SOCKETS
#include "s_apps.h"
#include "s_eio.h"

#ifdef WIN16
/* I personally think it must be one of the most pathetic aspects of Win16
 * that it is not acutally possible to find out why functions fail.
 * It is interesting to note that to fix this problem, Win32 decided to
 * let BOOL have three values, TRUE, FALSE and -1 (for error).
 * Good one microsoft.  I think the functions should at least be redefined
 * to not return type BOOL in Win32 */
int GetLastError()
	{
	return(0);
	}
#endif

int EIO_add(ctx,hdl)
EIO_CTX *ctx;
EIO_HANDLE *hdl;
	{
	EIO_HANDLE **hp;
	int i,j;

	if (hdl->fd+1 >= ctx->num_handlers) /* bump up table */
		{
		i=hdl->fd+1+10; /* allocate extra space */
		hp=ctx->handlers;
		if (hp == NULL)
			{
			hp=(EIO_HANDLE **)malloc(i*sizeof(EIO_HANDLE *));
			}
		else
			{
			hp=(EIO_HANDLE **)realloc(hp,i*sizeof(EIO_HANDLE *));
			if (hp == NULL) return(0);
			}
		for (j=ctx->num_handlers; j<i; j++)
			hp[j]=NULL;
		ctx->num_handlers=i;
		ctx->handlers=hp;
		}
	hp=ctx->handlers;
	if (hp[hdl->fd] != NULL)
		return(0);
	else
		hp[hdl->fd]=hdl;

	hdl->ctx=ctx;
	i=hdl->state;
	hdl->state=EIO_EVENT_NOTHING;
	EIO_set_state(hdl,i);
	return(1);
	}

int EIO_remove(hdl)
EIO_HANDLE *hdl;
	{
	EIO_HANDLE **hp,*h;
	EIO_CTX *ctx;
	int i;

	if (hdl->ctx == NULL) return(0);
	ctx=hdl->ctx;
	hp=ctx->handlers;
	EIO_set_state(hdl,EIO_EVENT_NOTHING);

	if (hp[hdl->fd] == NULL)
		return(0);
	else
		{
		h=hp[hdl->fd];
		hp[hdl->fd]=NULL;
		if (hdl->fd+1 == ctx->max_fd)
			{
			for (i=ctx->max_fd-1; i>0; i--)
				if (hp[i] != NULL) break;
			ctx->max_fd=i;
			}
		}
	return(1);
	}

void EIO_set_callback(hdl,callback)
EIO_HANDLE *hdl;
int (*callback)();
	{
	hdl->callback=callback;
	}


int EIO_set_handle(hdl,fd)
EIO_HANDLE *hdl;
int fd;
	{
	hdl->fd=fd;
	if (hdl->ctx != NULL)
		{
		/* we need to 'remove' then 'add' */
		return(0);
		}
	return(1);
	}

int EIO_get_state(hdl)
EIO_HANDLE *hdl;
	{
	return(hdl->state);
	}

EIO_HANDLE *EIO_HANDLE_new()
	{
	EIO_HANDLE *ret;

	ret=(EIO_HANDLE *)malloc(sizeof(EIO_HANDLE));
	ret->ctx=NULL;
	ret->fd=0;
	ret->state=0;
	ret->callback=NULL;
	ret->peer=NULL;
	ret->data=NULL;
	ret->writable=0;
	return(ret);
	}

void EIO_HANDLE_free(a)
EIO_HANDLE *a;
	{
	free((char *)a);
	}

void EIO_CTX_free(a,cb)
EIO_CTX *a;
void (*cb)();
	{
	EIO_HANDLE **hpp;
	int i;

	hpp=a->handlers;

	for (i=0; i<a->num_handlers; i++)
		{
		if (hpp[i] != NULL)
			{
			if (cb != NULL) cb(hpp[i]->data);
			EIO_HANDLE_free(hpp[i]);
			}
		}
	free((char *)a->handlers);
	}

#ifdef WINDOWS

#define CLASS_NAME	"SSLeay s_multzw"
EIO_CTX *eio_ctx=NULL;

LRESULT CALLBACK message_callback(HWND winhdl,UINT message,
	WPARAM wParam,LPARAM param2);

int mesgwin_init()
	{
	static int started=0;
	static ATOM eio_class;
	WNDCLASS wc;

	if (started) return(0);
	started=1;

	wc.style=0;
	wc.lpfnWndProc=message_callback;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=0;
	wc.hIcon=0;
	wc.hCursor=0;
	wc.hbrBackground=0;
	wc.lpszMenuName=NULL;
	wc.lpszClassName=CLASS_NAME;

	if ((eio_class=RegisterClass(&wc)) == 0)
		{
		/* I personally think it is pathetic that Win16 will not
		 * actually let you find out why things failed :-( */
		fprintf(stderr,"RegisterClass error:%d\n",GetLastError());
/*		return(0); */
		}
	return(1);
	}
#endif

EIO_CTX *EIO_CTX_new()
	{
	EIO_CTX *ret;

	ret=(EIO_CTX *)malloc(sizeof(EIO_CTX));
	ret->max_fd=0;
#ifdef WINDOWS
	if (!mesgwin_init())
		return(NULL);
	ret->mesgwin=CreateWindow(
		CLASS_NAME,"",0,
		CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,
		0,0,0,0);
	if (ret->mesgwin == 0)
		{
		long i;

		i=GetLastError();
		fprintf(stderr,"CreateWindow() error %ld\n",i);
		free(ret);
		return(NULL);
		}
#else
	FD_ZERO(&(ret->read));
	FD_ZERO(&(ret->write));
	FD_ZERO(&(ret->exception));
#endif
	ret->num_handlers=0;
	ret->handlers=NULL;
	ret->debug=0;
	return(ret);
	}

#ifdef WINDOWS
/* This is where things get interesting.  I am going to use
 * WSAAsyncSelect() to implement callbacks.  Each EIO_HANDLE will
 * register which message types it will accept and then the
 * EIO_loop() will just be a message loop.
 */

#define EIO_MESSAGE	(WM_USER+443)	/* message 5BB */

void EIO_set_state(hdl,state)
EIO_HANDLE *hdl;
int state;
	{
	int i;
	EIO_CTX *ctx;
	long l=0L;
	
	hdl->state=state;

	ctx=hdl->ctx;
	if (ctx == NULL) return;

	if (state & EIO_EVENT_READ)
		l|=(FD_READ|FD_CLOSE);
	if (state & EIO_EVENT_WRITE)
		l|=(FD_WRITE|FD_CLOSE);
	if (state & EIO_EVENT_EXCEPTION)
		l|=(FD_OOB|FD_CLOSE);
	if (state & EIO_EVENT_ACCEPT)
		l|=FD_ACCEPT;
	if (state & EIO_EVENT_CONNECT)
		l|=FD_CONNECT;
	if (state & EIO_EVENT_CLOSE)
		l|=FD_CLOSE;

	if (ctx == NULL) return;

	/* We now adjust the ctx fd list to modify the
	 * 'top' fd we listen to if required */
	i=ctx->max_fd;
	/* Check if we lost the top fd so scale things back 
	 * while this is not needed for WINDOWS, I will leave it in. */
	if ((state == 0) && (hdl->fd+1 == i))
		{
		i--;
		ctx->max_fd=i;
		}
	else if ((state != 0) && (hdl->fd >= ctx->max_fd))
		{
		ctx->max_fd=hdl->fd+1;
		}
	i=WSAAsyncSelect(hdl->fd,ctx->mesgwin,EIO_MESSAGE,l);
	if (i != 0)
		{
		fprintf(stderr,"WSAAsyncSelect error: sock fd=%d err=%d,%d\n",
			hdl->fd,i,sock_err());
		}
	}

int EIO_loop(ctx)
EIO_CTX *ctx;
	{
	long i;
	int ret;
	MSG msg;

	eio_ctx=ctx;
	for (;;)
		{
		i=GetMessage(&msg,ctx->mesgwin,0,0);
		fprintf(stderr,"got message\n");
		if (i == 0) { ret=1; break; }
		if (i < 0)  { ret=0; break; }
		TranslateMessage(&msg);
		i=DispatchMessage(&msg);
		if (i <= 0) return((int)i);
		}
	return(ret);
	}

LRESULT CALLBACK message_callback(winhdl,message,wParam,lParam)
HWND winhdl;
UINT message;
WPARAM wParam;
LPARAM lParam;
	{
	EIO_HANDLE *hp;
	int code,err,ret=1;
	unsigned int fd;
	
	fd=wParam;
	fprintf(stderr,"fd=%d message=%02X lParam=%08X\n",
		fd,message,lParam);

	if (message != EIO_MESSAGE)
		goto end;

	err=WSAGETSELECTERROR(lParam);
	code=WSAGETSELECTEVENT(lParam);
		
	if ((int)fd >= eio_ctx->max_fd)
		{
		fprintf(stderr,"fd returned out of range\n");
		return(0);
		}
	hp=eio_ctx->handlers[fd];
	if (hp == NULL)
		{
		fprintf(stderr,"internal error, fd=%d\n",fd);
		return(0);
		}
	if ((hp->state & EIO_EVENT_MASK) &&
		(hp->callback != NULL))
		{
		fprintf(stderr,"process message %02X %02X\n",hp->state,code);
		/* This processing is 'spread out' mostly for
		 * debuging */
		if ((code & FD_READ) && (hp->state & EIO_EVENT_READ))
			ret=hp->callback(hp,EIO_EVENT_READ);
		else if ((code & FD_WRITE) && (hp->state & EIO_EVENT_WRITE))
			ret=hp->callback(hp,EIO_EVENT_WRITE);
		else if ((code & FD_OOB) && (hp->state & EIO_EVENT_EXCEPTION))
			ret=hp->callback(hp,EIO_EVENT_EXCEPTION);
		else if ((code & FD_ACCEPT) && (hp->state & EIO_EVENT_ACCEPT))
			ret=hp->callback(hp,EIO_EVENT_ACCEPT);
		else if ((code & FD_CONNECT) && (hp->state & EIO_EVENT_CONNECT))
			ret=hp->callback(hp,EIO_EVENT_CONNECT);
		else if ((code & FD_CLOSE) && (hp->state & EIO_EVENT_CLOSE))
			ret=hp->callback(hp,EIO_EVENT_CLOSE);
		}
end:
	return(ret);
	}


#else /* ! WINDOWS */

void EIO_set_state(hdl,state)
EIO_HANDLE *hdl;
int state;
	{
	int ch,i;
	EIO_CTX *ctx;
	EIO_HANDLE **h;
	
	ch=hdl->state ^ state;	/* Gives the ones that have changed */
	hdl->state=state;

	ctx=hdl->ctx;
	if (ctx == NULL) return;

	if (ch == 0) return;
	if (ch & EIO_EVENT_READ)
		{
		if (state & (EIO_EVENT_READ|EIO_EVENT_ACCEPT))
			{ FD_SET(  hdl->fd,&(ctx->read)); }
		else	{ FD_CLR(hdl->fd,&(ctx->read)); }
		}
	if (ch & EIO_EVENT_WRITE)
		{
		if (state & (EIO_EVENT_WRITE|EIO_EVENT_CONNECT))
			{ FD_SET(  hdl->fd,&(ctx->write)); }
		else	{ FD_CLR(hdl->fd,&(ctx->write)); }
		}
	if (ch & EIO_EVENT_EXCEPTION)
		{
		if (state & EIO_EVENT_EXCEPTION)
			{ FD_SET(  hdl->fd,&(ctx->exception)); }
		else	{ FD_CLR(hdl->fd,&(ctx->exception)); }
		}

	if (ctx == NULL) return;

	/* We now adjust the ctx fd list to modify the
	 * 'top' fd we listen to if required */
	i=ctx->max_fd;
	/* Check if we lost the top fd so scale things back */
	if ((state == 0) && (hdl->fd+1 == i))
		{
		i--;
		h=ctx->handlers;
		for (;;)
			{
			if (i == 0) break;
			if ((h[i-1] != NULL) && (
				!FD_ISSET(i-1,&(ctx->read)) &&
				!FD_ISSET(i-1,&(ctx->write)) &&
				!FD_ISSET(i-1,&(ctx->exception))))
				i--;
			else
				break;
			}
		ctx->max_fd=i;
		}
	else if ((state != 0) && (hdl->fd >= ctx->max_fd))
		{
		ctx->max_fd=hdl->fd+1;
		}
	}

int EIO_loop(ctx)
EIO_CTX *ctx;
	{
	int n,i,ret;
	fd_set read_fds;
	fd_set write_fds;
	fd_set exception_fds;
	EIO_HANDLE *hp;

	for (;;)
		{
		if (ctx->max_fd <= 0) 
			{
			ret=1;
			break;
			}
		memcpy((char *)&read_fds,(char *)&(ctx->read),
			sizeof(read_fds));
		memcpy((char *)&write_fds,(char *)&(ctx->write),
			sizeof(write_fds));
		memcpy((char *)&exception_fds,(char *)&(ctx->exception),
			sizeof(exception_fds));
		if (ctx->debug & 0x01) sleep(1);
		n=select(ctx->max_fd,&read_fds,&write_fds,&exception_fds,NULL);
		if (n <= 0)
			{
			/* error callback */
			}
		if (n >= ctx->num_handlers)
			{
			fprintf(stderr,"FATAL ERROR, this should not happen\n");
			abort();
			}
		for (i=0; i<ctx->max_fd; i++)
			{
			hp=ctx->handlers[i];
			if (hp == NULL) continue;
			if ((hp->state & EIO_EVENT_MASK) &&
				(hp->callback != NULL))
				{
				if ((hp->state & (EIO_EVENT_READ|EIO_EVENT_ACCEPT)) &&
					FD_ISSET(i,&read_fds))
					{
					ret=hp->callback(hp,EIO_EVENT_READ);
					if (ret == 0) continue;
					if (ret < 0) goto out;
					n--;
					}
				if ((hp->state & (EIO_EVENT_WRITE|EIO_EVENT_CONNECT)) &&
					FD_ISSET(i,&write_fds))
					{
					ret=hp->callback(hp,EIO_EVENT_WRITE);
					if (ret == 0) continue;
					if (ret < 0) goto out;
					n--;
					}
				if ((hp->state & EIO_EVENT_EXCEPTION) &&
					FD_ISSET(i,&exception_fds))
					{
					ret=hp->callback(hp,EIO_EVENT_EXCEPTION);
					if (ret == 0) continue;
					if (ret < 0) goto out;
					n--;
					}
				}
			if (n == 0) break;
			}
		}
out:
	return(ret);
	}
#endif
