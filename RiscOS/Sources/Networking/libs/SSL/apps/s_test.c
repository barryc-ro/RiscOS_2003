/* apps/s_test.c */
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
#include "s_disp.h"
#include "apps.h"

#undef PROG
#define PROG	s_mult_main

typedef struct buf_st
	{
	int buffer_size;
        int buffer_num_written;
        int buffer_num;
        char *buffer;
	} BUF_STUFF;

static int read_bytes(EIO_HANDLE *dh);
static int write_bytes(EIO_HANDLE *dh);
static int body();

static  void usage()
	{
	fprintf(stderr,"usage: s_mult [args ...]\n");
	fprintf(stderr,"\n");
	fpritnf(stderr," -port arg	-port to connect to (default is %d\n",PORT);
#ifdef FIONBIO
	fprintf(stderr," -nbio         - Run with non-blocking IO\n");
#endif
	}

static int nbio;

int MAIN(argc, argv)
int argc;
char *argv[];
	{
	int port;
	int ret=1;
	BUF_STUFF b;
	EIO_CTX *ctx;
	EIO_HANDLE *dhr,*dhw;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	port=PORT;
	nbio=0;

	argc--;
	argv++;
	while (argc >= 1)
		{
		if	(strcmp(*argv,"-port) == 0)
			{
			if (--argc < 1) goto bad;
			port=atoi(*(++argv));
			if (port == 0) goto bad;
			}
#ifdef FIONBIO
                else if (strcmp(*argv,"-nbio") == 0)
                        { nbio=1; }
#endif
		else
			{
			fprintf(stderr,"unknown option %s\n",*argv);
			badop=1;
			break;
			}
		argc--;
		argv++;
		}
	if (badop)
		{
bad:
		usage();
		goto end;
		}

	ret=body(port);
end:
	EXIT(0);
	}

static int body(port)
int port;
	{
	int asock;

	if (!init_server(&asock,port))
		{
		return(0);
		}

	b.buffer_num_written=0;
	b.buffer_num=0;
	b.buffer_size=102400;
	b.buffer=(char *)malloc(102400);

	ctx=EIO_CTX_new();

	dhr=EIO_HANDLE_new();
	dhw=EIO_HANDLE_new();
	dhr->data=(char *)&b;
	dhw->data=(char *)&b;

	EIO_set_handle(dhr,fileno(stdin));
	EIO_set_handle(dhw,fileno(stdout));
	EIO_set_callback(dhr,(int (*)())read_bytes);
	EIO_set_callback(dhw,(int (*)())write_bytes);
	dhw->peer=dhr;
	dhr->peer=dhw;

	EIO_add(ctx,dhr);
	EIO_add(ctx,dhw);

	EIO_set_state(dhr,EIO_EVENT_READ);

	EIO_loop(ctx);

	}

int read_bytes(dh)
EIO_HANDLE *dh;
	{
	int i;
	BUF_STUFF *b;

	b=(BUF_STUFF *)dh->data;
	i=read(dh->fd,b->buffer,b->buffer_size);
fprintf(stderr,"READ %d\n",i);
	EIO_set_state(dh,EIO_EVENT_NOTHING);
	b->buffer_num=i;
	b->buffer_num_written=0;
	if (i > 0)
		EIO_set_state(dh->peer,EIO_EVENT_WRITE);
	return(1);
	}

int write_bytes(dh)
EIO_HANDLE *dh;
	{
	int i,j;
	BUF_STUFF *b;

	b=(BUF_STUFF *)dh->data;
	j=b->buffer_num_written;
	i=write(dh->fd,&(b->buffer[j]),b->buffer_num-j);
fprintf(stderr,"WRTIE %d\n",i);
	if (i <= 0)
		EIO_set_state(dh,EIO_EVENT_NOTHING);
	else
		{
		j+=i;
		if (j >= b->buffer_num)
			{
			EIO_set_state(dh,EIO_EVENT_NOTHING);
			EIO_set_state(dh->peer,EIO_EVENT_READ);
			}
		else
			{
			/* keep on writing */
			b->buffer_num_written=j;
			}
		}
	return(1);
	}
