/* apps/s_eio.h */
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

#ifndef HEADER_S_EIO_H
#define HEADER_S_EIO_H

#define EIO_EVENT_NOTHING	0x00
#define EIO_EVENT_READ		0x01
#define EIO_EVENT_WRITE		0x02
#define EIO_EVENT_EXCEPTION	0x04
#define EIO_EVENT_ACCEPT	0x08
#define EIO_EVENT_CONNECT	0x10
#define EIO_EVENT_CLOSE		0x20

#define EIO_EVENT_MASK		0x3F

typedef struct eio_ctx_st
	{
	int max_fd;
#ifdef WINDOWS
	HWND mesgwin;		/* destination for MSAAsyncSelect() */
#else
	fd_set read;
	fd_set write;
	fd_set exception;
#endif

	int num_handlers;
	struct eio_handle_st **handlers;
	/* if debug & 1, a sleep(1) is done each time around the select loop */
	int debug;
	} EIO_CTX;

/* These are registered for action, callback(fd,arg) */
typedef struct eio_handle_st
	{
	EIO_CTX *ctx;
	int state;
	int fd;
	int (*callback)(); /* ret 1 to keep in list, 0 to remove */

	/* The following are for use by the handlers. */
	struct eio_handle_st *peer;
	char *data;

	/* The Winsock WSAAsyncSelect() semantics are such that when I can
	 * write, I can keep on writing until I get a WSAEWOULDBLOCK error.
	 * Only then will I recieve another message when writing is ok again.
	 * So I need to keep track of this in the following variable 
	 * I should perhaps suround it with #ifdef WINDOWS but I will not bother */
	int writable;
	} EIO_HANDLE;

#ifndef NOPROTO
int EIO_add(EIO_CTX *ctx, EIO_HANDLE *hdl);
int EIO_remove(EIO_HANDLE *hdl);
void EIO_set_callback(EIO_HANDLE *hdr, int (*callback)());
int EIO_set_handle(EIO_HANDLE *hdr, int fd);
int EIO_get_state(EIO_HANDLE *hdl);
void EIO_set_state(EIO_HANDLE *dhl, int state);
EIO_HANDLE *EIO_HANDLE_new(void );
void EIO_HANDLE_free(EIO_HANDLE *a);
EIO_CTX *EIO_CTX_new(void );
int EIO_loop(EIO_CTX *ctx);
void EIO_CTX_free(EIO_CTX *a,void (*cb)());

#else

int EIO_add();
int EIO_remove();
void EIO_set_callback();
int EIO_set_handle();
int EIO_get_state();
void EIO_set_state();
EIO_HANDLE *EIO_HANDLE_new();
void EIO_HANDLE_free();
EIO_CTX *EIO_CTX_new();
int EIO_loop();
void EIO_CTX_free();

#endif

#endif
