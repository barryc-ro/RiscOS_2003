/* apps/s_mult.c */
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
#include <signal.h>

#include "lhash.h"
#include "buffer.h"
#include "rsa.h"
#include "x509.h"
#include "pem.h"
#include "ssl.h"
#define USE_SOCKETS
#include "apps.h"
#include "s_apps.h"
#include "s_eio.h"
#include "err.h"

#undef PROG
#define PROG	s_mult_main

typedef struct buf_st
	{
	char *data; /* extra data, a SSL if not null */
	int buffer_size;
        int buffer_num_written;
        int buffer_num;
        char *buffer;
        int references;
	} BUF_STUFF;

#ifndef NOPROTO
static int do_accept_handler(EIO_HANDLE *dh);
static int do_stdin(EIO_HANDLE *dh);
static int body(int);
static int do_normal_fd(EIO_HANDLE *dh,int type);
static int do_ssl_fd(EIO_HANDLE *dh,int type);
BUF_STUFF *BUF_STUFF_new(void);
void BUF_STUFF_free(BUF_STUFF *a);
void fd_shutdown(EIO_HANDLE *dh);
#else
static int do_accept_handler();
static int do_stdin();
static int body();
static int do_normal_fd();
static int do_ssl_fd();
BUF_STUFF *BUF_STUFF_new();
void BUF_STUFF_free();
void fd_shutdown();
#endif

static void usage()
	{
	fprintf(stderr,"usage: s_mult [args ...]\n");
	fprintf(stderr,"\n");
	fprintf(stderr," -port arg	-port to connect to (default is %d\n",PORT);
	fprintf(stderr," -echo          - reflect back what we are sent\n");
	fprintf(stderr," -bsize arg     - buffer size\n");
	fprintf(stderr," -ssl           - accept SSL connection\n");
	fprintf(stderr," -v             - Verbose mode\n");
	fprintf(stderr," -pause         - Have a sleep after each read/write,\n");
	fprintf(stderr,"                  good for testing non-blocking IO\n");
#ifdef FIONBIO
	fprintf(stderr," -nbio          - Run with non-blocking IO\n");
#endif
	}

static int echo=0;
static int m_nbio=0;
static int bsize=1024*10;
static int m_Pause=0;
static int verbose=0;
static int ssl=0;
static SSL_CTX *m_ctx=NULL;
static char *key_file=NULL;
static char *cert_file="./server.pem";
static RSA *key=NULL;
static X509 *cert=NULL;

int MAIN(argc, argv)
int argc;
char *argv[];
	{
	int port,badop=0;
	int ret=1;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	/* For when the other end goes berko */
#ifdef SIGPIPE
	signal(SIGPIPE,SIG_IGN);
#endif

	port=PORT;
	m_nbio=0;

	argc--;
	argv++;
	while (argc >= 1)
		{
		if	(strcmp(*argv,"-port") == 0)
			{
			if (--argc < 1) goto bad;
			port=atoi(*(++argv));
			if (port == 0) goto bad;
			}
		else if (strcmp(*argv,"-bsize") == 0)
			{
			if (--argc < 1) goto bad;
			bsize=atoi(*(++argv));
			if (bsize == 0) goto bad;
			}
		else if (strcmp(*argv,"-cert") == 0)
			{
			if (--argc < 1) goto bad;
			cert_file= *(++argv);
			}
		else if (strcmp(*argv,"-key") == 0)
			{
			if (--argc < 1) goto bad;
			key_file= *(++argv);
			}
                else if (strcmp(*argv,"-echo") == 0)
                        { echo=1; }
                else if (strcmp(*argv,"-v") == 0)
                        { verbose=1; }
                else if (strcmp(*argv,"-pause") == 0)
                        { m_Pause=1; }
                else if (strcmp(*argv,"-ssl") == 0)
                        { ssl=1; }
#ifdef FIONBIO
                else if (strcmp(*argv,"-nbio") == 0)
                        { m_nbio=1; }
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

	if ((key_file == NULL) && (cert_file != NULL))
		key_file=cert_file;

	if (ssl)
		{
		if ((m_ctx=SSL_CTX_new()) == NULL)
			goto end;
		}
	ret=body(port);
	if (ret == 0)
		{
		SSL_load_error_strings();
		ERR_print_errors(bio_err);
		}
	fprintf(stderr,"Done %d\n",ret);
	if (m_ctx != NULL) SSL_CTX_free(m_ctx);
end:
	EXIT(0);
	}

static int body(port)
int port;
	{
	int ret=0;
	int asock;
	EIO_CTX *dctx;
	EIO_HANDLE *ah;
	BIO *in=NULL;

	if (!init_server(&asock,port))
		return(0);

	if ((dctx=EIO_CTX_new()) == NULL)
		{
		fprintf(stderr,"error initalising EIO_CTX\n");
		return(0);
		}

	if (ssl)
		{
		in=BIO_new(BIO_s_file());
		if (in == NULL)
			{
			ERR_print_errors(bio_err);
			goto err;
			}

		if (BIO_read_filename(in,key_file) <= 0)
			{
			fprintf(stderr,"error opening key file\n");
			perror(key_file);
			goto err;
			}
		if ((key=PEM_read_bio_RSAPrivateKey(in,NULL,NULL)) == NULL)
			goto err;

		if (BIO_read_filename(in,cert_file) <= 0)
			{
			fprintf(stderr,"error opening certificate file\n");
			perror(cert_file);
			goto err;
			}

		if ((cert=PEM_read_bio_X509(in,NULL,NULL)) == NULL) goto err;
		}

	/* Setup an accept socket */
	ah=EIO_HANDLE_new();
	EIO_set_handle(ah,asock);
	EIO_set_callback(ah,(int (*)())do_accept_handler);
	EIO_add(dctx,ah);
	EIO_set_state(ah,EIO_EVENT_ACCEPT|EIO_EVENT_READ);

#ifndef WINDOWS
	/* Lets also read from stdin */
	ah=EIO_HANDLE_new();
	EIO_set_handle(ah,fileno(stdin));
	EIO_set_callback(ah,(int (*)())do_stdin);
	EIO_add(dctx,ah);
	EIO_set_state(ah,EIO_EVENT_READ);
#endif

	fprintf(stderr,"start\n");
	if (in != NULL)
		{
		BIO_free(in);
		in=NULL;
		}
	ret=EIO_loop(dctx);
err:
	if (in != NULL) BIO_free(in);
	SHUTDOWN(asock);
	sock_cleanup();
	EIO_CTX_free(dctx,BUF_STUFF_free);
	return(ret);
	}

static int do_accept_handler(dh)
EIO_HANDLE *dh;
	{
	int asock,sock;

	asock=dh->fd;

	if (verbose) printf("ACCEPT\n");
	if (!do_accept(dh->fd,&sock,NULL))
		{
		fprintf(stderr,"accept failed - shuting down accept socket\n");
		fd_shutdown(dh);
		}
	else
		{
		EIO_HANDLE *hr,*hw;
		BUF_STUFF *b;
		FILE *out=NULL;

		fprintf(stderr,"ACCEPT fd=%d\n",sock);
#ifdef FIONBIO
		if (m_nbio)
			{
			unsigned long i=1;

			socket_ioctl(sock,FIONBIO,&i);
			}
#endif
		b=BUF_STUFF_new();
		hr=EIO_HANDLE_new();
		hr->data=(char *)b;
		EIO_set_handle(hr,sock);
		if (ssl)
			{
			SSL *s;

			s=(SSL *)SSL_new(m_ctx);
			if (m_Pause) s->debug=1;
			SSL_use_certificate(s,cert);
			SSL_use_RSAPrivateKey(s,key);
			SSL_set_fd(s,sock);
			/* put the SSL data in the shared struct */
			b->data=(char *)s;
			EIO_set_callback(hr,(int (*)())do_ssl_fd);
			}
		else
			EIO_set_callback(hr,(int (*)())do_normal_fd);
		EIO_set_state(hr,EIO_EVENT_READ);
		EIO_add(dh->ctx,hr);
		hr->peer=hr;
		if (!echo)
			{
			hw=EIO_HANDLE_new();
			if ((out=fopen(NUL_DEV,"w")) == NULL)
				{
				perror(NUL_DEV);
				/* continue anyway */
				}
			EIO_set_handle(hw,fileno(out));
			/* EIO_set_handle(hw,dup(fileno(stdout)));*/
			EIO_set_callback(hw,(int (*)())do_normal_fd);
			b->references++;
			hw->data=(char *)b;
			hr->peer=hw;
			hw->peer=hr;
			EIO_set_state(hw,EIO_EVENT_NOTHING);
			EIO_add(dh->ctx,hw);
			}
		}
	return(1);
	}

static int do_normal_fd(dh,type)
EIO_HANDLE *dh;
int type;
	{
	int i,j,ret=1;
	BUF_STUFF *b;

	b=(BUF_STUFF *)dh->data;

	if (type == EIO_EVENT_READ)
		{
		i=SSLeay_Read(dh->fd,b->buffer,b->buffer_size);
		if (verbose)
			printf("READ (%3d) %4d -> %4d\n",dh->fd,b->buffer_size,i);
		if (i == 0)
			{
			fprintf(stderr,"closed\n");
			fd_shutdown(dh);
			dh=NULL;
			ret=0;
			}
		else if (i < 0)
			{
			if (should_retry(i))
				dh->writable=0;
			else
				{
				fprintf(stderr,"fd=%d ret=%d errno=%d\n",
					dh->fd,i,sock_err());
				EIO_set_state(dh,EIO_EVENT_NOTHING);
				fd_shutdown(dh);
				ret=0;
				}
			}
		else
			{
			b->buffer_num=i;
			b->buffer_num_written=0;
			EIO_set_state(dh->peer,EIO_EVENT_WRITE);
			}
		if ((dh != NULL) && (dh != dh->peer))
			{
			EIO_set_state(dh,EIO_EVENT_NOTHING);
			}
		}
	else if (type & EIO_EVENT_WRITE)
		{
		j=b->buffer_num_written;
		i=SSLeay_Write(dh->fd,&(b->buffer[j]),b->buffer_num-j);
		if (verbose)
			printf("WRITE(%3d) %4d -> %4d\n",dh->fd,b->buffer_num-j,i);
		if (i == 0) /* shutdown */
			{
			EIO_set_state(dh,EIO_EVENT_NOTHING);
			fd_shutdown(dh);
			ret=0;
			dh=NULL;
			}
		else if (i < 0) /* error */
			{
			if (should_retry(i))
				dh->writable=0;
			else
				{
				fprintf(stderr,"write error fd=%d err=%d\n",
					dh->fd,sock_err());
				/* else, lets shut it down */
				EIO_set_state(dh,EIO_EVENT_NOTHING);
				fd_shutdown(dh);
				ret=0;
				}
			}
		else
			{
			j+=i;
			if (j >= b->buffer_num)
				{
				if (dh->peer != dh)
					EIO_set_state(dh,EIO_EVENT_NOTHING);
				EIO_set_state(dh->peer,EIO_EVENT_READ);
				}
			else
				{ /* keep on writing */
				b->buffer_num_written=j;
				}
			}
		}
	/* This one only happens under WinSock */
	else if (type & EIO_EVENT_CLOSE)
		{
		EIO_set_state(dh,EIO_EVENT_NOTHING);
		fd_shutdown(dh);
		printf("socket shutdown\n");
		ret=0;
		}
	return(ret);
	}

static int do_ssl_fd(dh,type)
EIO_HANDLE *dh;
int type;
	{
	int i,j,ret=1;
	BUF_STUFF *b;
	SSL *s;

	b=(BUF_STUFF *)dh->data;
	s=(SSL *)b->data;

fprintf(stderr,"Callback type=%d\n",type);
	/* This one only happens under WinSock */
	if (type & EIO_EVENT_CLOSE)
		{
		EIO_set_state(dh,EIO_EVENT_NOTHING);
		fd_shutdown(dh);
		printf("socket shutdown\n");
		ret=0;
		}
	/* If the connection is not established, we need to call
	 * SSL_accept */
	else if (!SSL_is_init_finished(s))
		{
		if (verbose)
			fprintf(stderr,"SSL_accept\n");
		i=SSL_accept(s);
		if (i <= 0)
			{
			if (verbose)
				fprintf(stderr,"SSL_accept(%3X:%s) -> %d\n",
					SSL_state(s),SSL_state_string_long(s),i);
			if (should_retry(i))
				{
				/* We would have blocked in SSL_accept()
				 * so we need to 'redo' SSL_accept()
				 * when more of the correct operations
				 * can be done on the socket. */
				if (SSL_want_read(s))
					{
					if (verbose)
						fprintf(stderr,"re-read\n");
					if (type != EIO_EVENT_READ)
						{
						fprintf(stderr,"bad\n");
						abort();
						}
					EIO_set_state(dh,EIO_EVENT_READ);
					}
				else
					{
					if (verbose)
						fprintf(stderr,"re-write\n");
					if (type != EIO_EVENT_WRITE)
						{
						fprintf(stderr,"bad2\n");
						abort();
						}
					EIO_set_state(dh,EIO_EVENT_WRITE);
					}
				}
			else /* Error in SSL_accept */
				{
				if (verbose)
					printf("SSL_accept() error=%d\n",
						sock_err());
				/* we shut things down */
				fd_shutdown(dh);
				ret=0;
				}
			goto end;
			}
		else
			{
			/* Finished setup */
			if (verbose)
				fprintf(stderr,"SSL_accept finished\n");
			EIO_set_state(dh,EIO_EVENT_READ);
			}
		}
	else if (type == EIO_EVENT_READ)
		{
		i=SSL_read(s,b->buffer,b->buffer_size);
		if (verbose)
			printf("SSL READ (%3d) %4d -> %4d\n",dh->fd,b->buffer_size,i);
		if (i <= 0)
			{
			if (!should_retry(i))
				{
				fprintf(stderr,"fd=%d ret=%d shutdown\n",
					dh->fd,i);
				fd_shutdown(dh);
				dh=NULL;
				ret=0;
				}
			else if (verbose) fprintf(stderr,"retry\n");
			}
		else
			{
			b->buffer_num=i;
			b->buffer_num_written=0;
			if (echo)
				EIO_set_state(dh->peer,EIO_EVENT_WRITE);
			}
		if ((dh != NULL) && (dh != dh->peer) && (echo))
			EIO_set_state(dh,EIO_EVENT_NOTHING);
		}
	/* The following will not get run if echo mode is not on */
	else if (type == EIO_EVENT_WRITE)
		{
		j=b->buffer_num_written;
		i=SSL_write(s,&(b->buffer[j]),b->buffer_num-j);
		if (verbose)
			printf("SSL WRITE(%3d) %4d -> %4d\n",dh->fd,b->buffer_num-j,i);
		if (i <= 0)
			{
			if (!should_retry(i))
				{
				fprintf(stderr,"fd=%d ret=%d\n",dh->fd,i);
				fd_shutdown(dh);
				ret=0;
				}
			if (verbose) fprintf(stderr,"retry\n");
			}
		else
			{
			j+=i;
			if (j >= b->buffer_num)
				{
				if (dh->peer != dh)
					EIO_set_state(dh,EIO_EVENT_NOTHING);
				EIO_set_state(dh->peer,EIO_EVENT_READ);
				}
			else
				{ /* keep on writing */
				b->buffer_num_written=j;
				}
			}
		}
end:
	return(ret);
	}

static int do_stdin(dh)
EIO_HANDLE *dh;
	{
	int i;
	char buf[1024];

	i=read(dh->fd,buf,1024);
	if (strncmp(buf,"quit",4) == 0)
		return(-1);
	else if (strncmp(buf,"q",1) == 0)
		return(-1);
	else if (strncmp(buf,"stats",5) == 0)
		{
		fprintf(stderr,"%4ld items in the session cache\n",
			SSL_CTX_sess_number(m_ctx));
		fprintf(stderr,"%4ld client connects (SSL_connect())\n",
			SSL_CTX_sess_connect(m_ctx));
		fprintf(stderr,"%4ld client connects that finished\n",
			SSL_CTX_sess_connect_good(m_ctx));
		fprintf(stderr,"%4ld server connects (SSL_accept())\n",
			SSL_CTX_sess_accept(m_ctx));
		fprintf(stderr,"%4ld server connects that finished\n",
			SSL_CTX_sess_accept_good(m_ctx));
		fprintf(stderr,"%4ld session cache hits\n",
			SSL_CTX_sess_hits(m_ctx));
		fprintf(stderr,"%4ld session cache misses\n",
			SSL_CTX_sess_misses(m_ctx));
		fprintf(stderr,"%4ld session cache timeouts\n",
			SSL_CTX_sess_timeouts(m_ctx));
		}
	else
		{
		fprintf(stderr,"commands: stats quit\n");
		}
	return(1);
	}


BUF_STUFF *BUF_STUFF_new()
	{
	BUF_STUFF *ret;

	ret=(BUF_STUFF *)malloc(sizeof(BUF_STUFF));
	ret->buffer=(char *)malloc(bsize);
	ret->buffer_size=bsize;
	ret->buffer_num_written=0;
	ret->buffer_num=0;
	ret->references=1;
	return(ret);
	}

void BUF_STUFF_free(a)
BUF_STUFF *a;
	{
	if (a == NULL) return;
	if (--a->references > 0) return;
	if (a->data != NULL) SSL_free((SSL *)a->data);
	free(a->buffer);
	free(a);
	}

void fd_shutdown(dh)
EIO_HANDLE *dh;
	{
	SHUTDOWN(dh->fd);
	EIO_set_state(dh,EIO_EVENT_NOTHING);
	EIO_remove(dh);
	EIO_HANDLE_free(dh);
	}
