/* apps/s_server.c */
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
#include "lhash.h"
#define USE_SOCKETS
#include "apps.h"
#include "err.h"
#include "pem.h"
#include "x509.h"
#include "ssl.h"
#include "s_apps.h"

#ifndef NOPROTO
static int sv_body(char *hostname, int s);
static void close_accept_socket(void );
static void sv_usage(void);
static int init_ssl_connection(SSL *s);
static void print_stats(FILE *fp,SSL_CTX *ctx);
#else
static int sv_body();
static void close_accept_socket();
static void sv_usage();
static int init_ssl_connection();
static void print_stats();
#endif

#undef BUFSIZZ
#define BUFSIZZ	8*1024
static int accept_socket=-1;

#define TEST_CERT	"server.pem"
#undef PROG
#define PROG		s_server_main

extern int verify_depth;
extern int verify_error;

static char *cipher=NULL;
int verify=SSL_VERIFY_NONE;
char *s_cert_file=TEST_CERT,*s_key_file=NULL;
#ifdef FIONBIO
static int s_nbio=0;
#endif
static SSL_CTX *ctx=NULL;

static BIO *bio_s_out=NULL;

static void sv_usage()
	{
	fprintf(stderr,"usage: s_server [args ...]\n");
	fprintf(stderr,"\n");
	fprintf(stderr," -port arg     - port to connect to (default is %d\n",PORT);
	fprintf(stderr," -verify arg   - turn on peer certificate verification\n");
	fprintf(stderr," -Verify arg   - turn on peer certificate verification, must have a cert.\n");
	fprintf(stderr," -cert arg     - certificate file to use, PEM format assumed\n");
	fprintf(stderr,"                 (default is %s)\n",TEST_CERT);
	fprintf(stderr," -key arg      - RSA file to use, PEM format assumed, in cert file if\n");
	fprintf(stderr,"                 not specified (default is %s)\n",TEST_CERT);
#ifdef FIONBIO
	fprintf(stderr," -nbio         - Run with non-blocking IO\n");
#endif
	fprintf(stderr," -CApath arg   - PEM format directory of CA's\n");
	fprintf(stderr," -CAfile arg   - PEM format file of CA's\n");
	fprintf(stderr," -cipher       - the only ciphers to use, ':' seperated list of the following\n");
	fprintf(stderr,"                  NULL-MD5     RC4-MD5      EXP-RC4-MD5 \n");
        fprintf(stderr,"                 IDEA-CBC-MD5 RC2-CBC-MD5  EXP-RC2-CBC-MD5\n");
        fprintf(stderr,"                 DES-CBC-MD5  DES-CBC-SHA  DES-CBC3-MD5\n");
        fprintf(stderr,"                 DES-CBC3-SHA DES-CFB-M1\n");
	}

int MAIN(argc, argv)
int argc;
char *argv[];
	{
	int port=PORT;
	char *CApath=NULL,*CAfile=NULL;
	int badop=0;
	int ret=1;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);
	if (bio_s_out == NULL)
		if ((bio_s_out=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_s_out,stdout,BIO_NOCLOSE);

	ctx=SSL_CTX_new();
	if (ctx == NULL)
		goto end;

	verify_depth=0;
	verify_error=VERIFY_OK;
#ifdef FIONBIO
	s_nbio=0;
#endif

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
		else if	(strcmp(*argv,"-verify") == 0)
			{
			verify=SSL_VERIFY_PEER;
			if (--argc < 1) goto bad;
			verify_depth=atoi(*(++argv));
			fprintf(stderr,"verify depth is %d\n",verify_depth);
			}
		else if	(strcmp(*argv,"-Verify") == 0)
			{
			verify=SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
			if (--argc < 1) goto bad;
			verify_depth=atoi(*(++argv));
			fprintf(stderr,"verify depth is %d, must return a certificate\n",verify_depth);
			}
		else if	(strcmp(*argv,"-cert") == 0)
			{
			if (--argc < 1) goto bad;
			s_cert_file= *(++argv);
			}
		else if	(strcmp(*argv,"-key") == 0)
			{
			if (--argc < 1) goto bad;
			s_key_file= *(++argv);
			}
		else if	(strcmp(*argv,"-CApath") == 0)
			{
			if (--argc < 1) goto bad;
			CApath= *(++argv);
			}
		else if	(strcmp(*argv,"-cipher") == 0)
			{
			if (--argc < 1) goto bad;
			cipher= *(++argv);
			}
		else if	(strcmp(*argv,"-CAfile") == 0)
			{
			if (--argc < 1) goto bad;
			CAfile= *(++argv);
			}
#ifdef FIONBIO	
		else if	(strcmp(*argv,"-nbio") == 0)
			{ s_nbio=1; }
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
		sv_usage();
		goto end;
		}

	if (cipher == NULL) cipher=getenv("SSL_CIPHER");

	if (s_cert_file == NULL)
		{
		fprintf(stderr,"You must specify a certificate file for the server to use\n");
		goto end;
		}
	SSL_load_error_strings();

	SSL_debug("server.log");
	if ((!SSL_load_verify_locations(ctx,CAfile,CApath)) ||
		(!SSL_set_default_verify_paths(ctx)))
		{
		fprintf(stderr,"X509_load_verify_locations\n");
		ERR_print_errors(bio_err);
		goto end;
		}

	if (cipher != NULL)
		SSL_CTX_set_cipher_list(ctx,cipher);
	fprintf(stderr,"ACCEPT\n");
	do_server(port,&accept_socket,sv_body);
	print_stats(stderr,ctx);
	ret=0;
end:
	if (ctx != NULL) SSL_CTX_free(ctx);
	EXIT(ret);
	}

static void print_stats(fp,ctx)
FILE *fp;
SSL_CTX *ctx;
	{
	fprintf(fp,"%4ld items in the session cache\n",
		SSL_CTX_sess_number(ctx));
	fprintf(fp,"%4ld client connects (SSL_connect())\n",
		SSL_CTX_sess_connect(ctx));
	fprintf(fp,"%4ld client connects that finished\n",
		SSL_CTX_sess_connect_good(ctx));
	fprintf(fp,"%4ld server connects (SSL_accept())\n",
		SSL_CTX_sess_accept(ctx));
	fprintf(fp,"%4ld server connects that finished\n",
		SSL_CTX_sess_accept_good(ctx));
	fprintf(fp,"%4ld session cache hits\n",SSL_CTX_sess_hits(ctx));
	fprintf(fp,"%4ld session cache misses\n",SSL_CTX_sess_misses(ctx));
	fprintf(fp,"%4ld session cache timeouts\n",SSL_CTX_sess_timeouts(ctx));
	}

static int sv_body(hostname, s)
char *hostname;
int s;
	{
	char *buf=NULL;
	fd_set readfds;
	int ret=1,width;
	int k,i;
	static SSL *con=NULL;
	unsigned long l=1;

	if ((buf=malloc(BUFSIZZ)) == NULL)
		{
		fprintf(stderr,"out of memory\n");
		goto err;
		}
#ifdef FIONBIO	
	if (s_nbio)
		{
		unsigned long l=1;

		fprintf(stderr,"turning on non blocking io\n");
		socket_ioctl(s,FIONBIO,&l);
		}
#endif

	fprintf(stderr,"CONNECTED\n");
	if (con == NULL)
		{
		con=(SSL *)SSL_new(ctx);
		SSL_set_verify(con,verify,verify_callback);
		if (!set_cert_stuff(con,s_cert_file,s_key_file))
			{
			ret=-1;
			goto err;
			}
		}
	SSL_clear(con);
	SSL_set_fd(con,s);
	verify_error=VERIFY_OK;

	width=s+1;
	for (;;)
		{
		FD_ZERO(&readfds);
#ifndef WINDOWS
		FD_SET(fileno(stdin),&readfds);
#endif
		FD_SET(s,&readfds);
		select(width,&readfds,NULL,NULL,NULL);
		if (FD_ISSET(fileno(stdin),&readfds))
			{
			i=read(fileno(stdin),buf,BUFSIZZ);
			if ((i <= 0) || (buf[0] == 'Q'))
				{
				fprintf(stderr,"DONE\n");
				shutdown(s,2);
				close_accept_socket();
				ret= -11;
				goto err;
				}
			if (buf[0] == 'S')
				{
				print_stats(stderr,SSL_get_SSL_CTX(con));
				}
			l=k=0;
			for (;;)
				{
				/* should do a select for the write */
				k=SSL_write(con,&(buf[l]),(unsigned int)i);
				if (
#ifdef FIONBIO
					s_nbio &&
#endif
					should_retry(k))
					{
					fprintf(stderr,"Write BLOCK\n");
					continue;
					}
				if (k <= 0)
					{
					ERR_print_errors(bio_err);
					fprintf(stderr,"DONE\n");
					ret=1;
					goto err;
					}
				l+=k;
				i-=k;
				if (i <= 0) break;
				}
			}
		if (FD_ISSET(s,&readfds))
			{
			if (!SSL_is_init_finished(con))
				{
				i=init_ssl_connection(con);
				
				if (i < 0)
					{
					ret=0;
					goto err;
					}
				else if (i == 0)
					{
					ret=1;
					goto err;
					}
				}
			else
				{
				i=SSL_read(con,(char *)buf,BUFSIZZ);
				if (
#ifdef FIONBIO
					s_nbio &&
#endif
					should_retry(i))
					{
					fprintf(stderr,"Read BLOCK\n");
					}
				else if (i <= 0)
					{
					ERR_print_errors(bio_err);
					fprintf(stderr,"DONE\n");
					ret=1;
					goto err;
					}
				else
					write(fileno(stdout),buf,
						(unsigned int)i);
				}
			}
		}
err:
/*	if (con != NULL) SSL_free(con);  */
	fprintf(stderr,"CONNECTION CLOSED\n");
	if (buf != NULL)
		{
		memset(buf,0,BUFSIZZ);
		free(buf);
		}
	if (ret >= 0)
		fprintf(stderr,"ACCEPT\n");
	return(ret);
	}

static void close_accept_socket()
	{
	fprintf(stderr,"shutdown\n");
	if (accept_socket >= 0)
		shutdown(accept_socket,2);
	}

static int init_ssl_connection(con)
SSL *con;
	{
	int i;
	char *str;
	X509 *peer;

	if ((i=SSL_accept(con)) <= 0)
		{
		if (should_retry(i))
			{
			fprintf(stderr,"DELAY\n");
			return(1);
			}

		fprintf(stderr,"ERROR\n");
		if (verify_error != VERIFY_OK)
			{
			fprintf(stderr,"verify error:%s\n",
				X509_cert_verify_error_string(verify_error));
			}
		else
			ERR_print_errors(bio_err);
		return(0);
		}

	PEM_write_bio_SSL_SESSION(bio_s_out,SSL_get_session(con));

	peer=SSL_get_peer_certificate(con);
	if (peer != NULL)
		{
		printf("Client certificate\n");
		PEM_write_bio_X509(bio_s_out,peer);
		str=X509_NAME_oneline(X509_get_subject_name(peer));
		printf("subject=%s\n",str);
		free(str);
		str=X509_NAME_oneline(X509_get_issuer_name(peer));
		printf("issuer=%s\n",str);
		free(str);
		X509_free(peer);
		}
	printf("CIPHER is %s\n",SSL_get_cipher(con));
	return(1);
	}
