/* apps/s_client.c */
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
#include "apps.h"
#include "x509.h"
#include "ssl.h"
#include "err.h"
#include "pem.h"
#include "s_apps.h"

#undef PROG
#define PROG	s_client_main

/*#define SSL_HOST_NAME	"www.netscape.com" */
/*#define SSL_HOST_NAME	"193.118.187.102" */
#define SSL_HOST_NAME	"localhost"

/*#define TEST_CERT "client.pem" */ /* no default cert. */

#undef BUFSIZZ
#define BUFSIZZ 1024*8

extern int verify_depth;
extern int verify_error;

#ifdef FIONBIO
static int c_nbio=0;
#endif
static int c_Pause=0;

#ifndef NOPROTO
static void sc_usage(void);
static void MS_CALLBACK client_info_callback(SSL *s,int mode,int ret);
#else
static void sc_usage();
static void MS_CALLBACK client_info_callback();
#endif

static BIO *bio_c_out=NULL;

static void sc_usage()
	{
	fprintf(stderr,"usage: client args\n");
	fprintf(stderr,"\n");
	fprintf(stderr," -host arg     - host or IP to connect to (default is %s)\n",SSL_HOST_NAME);
	fprintf(stderr," -port arg     - port to connect to (default is %d\n",PORT);
	fprintf(stderr," -verify arg   - turn on peer certificate verification\n");
	fprintf(stderr," -cert arg     - certificate file to use, PEM format assumed\n");
	fprintf(stderr," -key arg      - Private key file to use, PEM format assumed, in cert file if\n");
	fprintf(stderr,"                 not specified but cert file is.\n");
	fprintf(stderr," -CApath arg   - PEM format directory of CA's\n");
	fprintf(stderr," -CAfile arg   - PEM format file of CA's\n");
	fprintf(stderr," -reconnect    - Drop and re-make the connection with the same Session-ID\n");
	fprintf(stderr," -pause        - sleep(1) after each read(2) and write(2) system call\n");
	fprintf(stderr," -state        - print the 'ssl' states\n");
#ifdef FIONBIO
	fprintf(stderr," -nbio         - Run with non-blocking IO\n");
#endif
	fprintf(stderr," -cipher       - prefered cipher to use, ':' seperated list of the following\n");
	fprintf(stderr,"                 NULL-MD5     RC4-MD5      EXP-RC4-MD5 \n");
	fprintf(stderr,"                 IDEA-CBC-MD5 RC2-CBC-MD5  EXP-RC2-CBC-MD5\n");
	fprintf(stderr,"                 DES-CBC-MD5  DES-CBC-SHA  DES-CBC3-MD5\n");
	fprintf(stderr,"                 DES-CBC3-SHA DES-CFB-M1\n");

	}

int MAIN(argc, argv)
int argc;
char **argv;
	{
	SSL *con=NULL,*con2=NULL;
	int s,i,j,k,l,width,state=0;
	char *buf=NULL,*p;
	fd_set readfds;
	int port=PORT;
	char *host=SSL_HOST_NAME;
	char *cert_file=NULL,*key_file=NULL;
	char *CApath=NULL,*CAfile=NULL,*cipher=NULL;
	int reconnect=0,badop=0,verify=SSL_VERIFY_NONE;
	X509 *peer;
	SSL_CTX *ctx=NULL;
	int ret=1;
	/*static struct timeval timeout={10,0};*/

#ifdef RISCOS_RM
        SSL_Library_Initialise();
#endif
	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);
	if (bio_c_out == NULL)
		if ((bio_c_out=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_c_out,stdout,BIO_NOCLOSE);

	if ((buf=malloc(BUFSIZZ)) == NULL)
		{
		fprintf(stderr,"out of memory\n");
		goto end;
		}

	ctx=SSL_CTX_new();
	if (ctx == NULL) goto end;
	verify_depth=0;
	verify_error=VERIFY_OK;
#ifdef FIONBIO
	c_nbio=0;
#endif

	argc--;
	argv++;
	while (argc >= 1)
		{
		if	(strcmp(*argv,"-host") == 0)
			{
			if (--argc < 1) goto bad;
			host= *(++argv);
			}
		else if	(strcmp(*argv,"-port") == 0)
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
		else if	(strcmp(*argv,"-cert") == 0)
			{
			if (--argc < 1) goto bad;
			cert_file= *(++argv);
			}
		else if	(strcmp(*argv,"-pause") == 0)
			c_Pause=1;
		else if	(strcmp(*argv,"-state") == 0)
			state=1;
		else if	(strcmp(*argv,"-key") == 0)
			{
			if (--argc < 1) goto bad;
			key_file= *(++argv);
			}
		else if	(strcmp(*argv,"-reconnect") == 0)
			{
			reconnect=1;
			}
		else if	(strcmp(*argv,"-CApath") == 0)
			{
			if (--argc < 1) goto bad;
			CApath= *(++argv);
			}
		else if	(strcmp(*argv,"-CAfile") == 0)
			{
			if (--argc < 1) goto bad;
			CAfile= *(++argv);
			}
		else if	(strcmp(*argv,"-cipher") == 0)
			{
			if (--argc < 1) goto bad;
			cipher= *(++argv);
			}
#ifdef FIONBIO
		else if (strcmp(*argv,"-nbio") == 0)
			{ c_nbio=1; }
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
		sc_usage();
		goto end;
		}

	if (state) SSL_CTX_set_info_callback(ctx,client_info_callback);

	SSL_load_error_strings();

	if ((!SSL_load_verify_locations(ctx,CAfile,CApath)) ||
		(!SSL_set_default_verify_paths(ctx)))
		{
		fprintf(stderr,"error seting default verify locations\n");
		ERR_print_errors(bio_err);
		goto end;
		}

	if (init_client(&s,host,port) == 0)
		{
		perror("conecting");
		SHUTDOWN(s);
		goto end;
		}
	fprintf(stderr,"CONNECTED\n");

#ifdef FIONBIO
	if (c_nbio)
		{
		unsigned long l=1;
		fprintf(stderr,"turning on non blocking io\n");
		socket_ioctl(s,FIONBIO,&l);
		}
#endif
	con=(SSL *)SSL_new(ctx);
	if (c_Pause & 0x01) con->debug=1;
	SSL_set_fd(con,s);
	SSL_set_verify(con,verify,verify_callback);

	if (cipher == NULL)
		SSL_CTX_set_cipher_list(ctx,getenv("SSL_CIPHER"));
	else
		SSL_CTX_set_cipher_list(ctx,cipher);

	if (!set_cert_stuff(con,cert_file,key_file))
		{
		SHUTDOWN(SSL_get_fd(con));
		goto end;
		}

	/* ok, lets connect */
	for (;;)
		{
		width=SSL_get_fd(con)+1;
		for (;;)
			{
			i=SSL_connect(con);
			if (should_retry(i))
				{
				fprintf(stderr,"DELAY\n");

				FD_ZERO(&readfds);
				FD_SET(SSL_get_fd(con),&readfds);
				select(width,&readfds,NULL,NULL,NULL);
				continue;
				}
			break;
			}
		if (i <= 0)
			{
			fprintf(stderr,"SSL_connect ERROR\n");
			if (verify_error != VERIFY_OK)
				{
				fprintf(stderr,"verify error:%s\n",
					X509_cert_verify_error_string(verify_error));
				}
			else
				{
				fprintf(stderr,"SSL_connect was in %s\n",
					SSL_state_string_long(con));
				ERR_print_errors(bio_err);
				fprintf(stderr,"(%d) errno=%d\n",i,sock_err());
				}
			SHUTDOWN(SSL_get_fd(con));
			goto end;
			}
		if (reconnect)
			{
			reconnect=0;

			i=SSL_write(con,"bye\n",4);
			SHUTDOWN(SSL_get_fd(con));

			if (init_client(&s,host,port) == 0)
				{ perror("conecting"); goto end; }

#ifdef FIONBIO
			if (c_nbio)
				{
				unsigned long l=1;
				fprintf(stderr,"turning on non blocking io\n");
				socket_ioctl(s,FIONBIO,&l);
				}
#endif
			fprintf(stderr,"drop the connection and reconnect with the same session id\n");
			/* test a second connection */
			/* NOTE: It is a new SSL structure so we need to set the
			 * certificate and private key for this SSL structure */
			con2=SSL_new(ctx);
			if (c_Pause & 0x01) con->debug=1;
			SSL_set_fd(con2,s);
			SSL_set_session(con2,SSL_get_session(con));
			if (!set_cert_stuff(con2,cert_file,key_file))
				{
				SHUTDOWN(SSL_get_fd(con2));
				goto end;
				}
			SSL_free(con);
			con=con2;
			con2=NULL;
			}
		else
			break;
		}
	width=s+1;

	peer=SSL_get_peer_certificate(con);
	if (peer != NULL)
		{
		char *str;

		printf("Server certificate\n");
		PEM_write_bio_X509(bio_c_out,peer);
		str=X509_NAME_oneline(X509_get_subject_name(peer));
		printf("subject=%s\n",str);
		free(str);
		str=X509_NAME_oneline(X509_get_issuer_name(peer));
		printf("issuer=%s\n",str);
		free(str);
		X509_free(peer);
		}


	p=SSL_get_shared_ciphers(con,buf,BUFSIZ);
	if (p != NULL)
		{
		printf("---\nCiphers common between both SSL endpoints:\n");
		j=i=0;
		while (*p)
			{
			if (*p == ':')
				{
				for (; j<15; j++)
					putc(' ',stdout);
				i++;
				j=0;
				if (i%3)
					putc(' ',stdout);
				else	putc('\n',stdout);
				}
			else
				{
				putc(*p,stdout);
				j++;
				}
			p++;
			}
		putc('\n',stdout);
		}

/*	printf("---\nCipher is %s\n",SSL_get_cipher(con));*/
	SSL_SESSION_print(bio_c_out,SSL_get_session(con));


	for (;;)
		{
		  struct timeval tv;
		  memset( &tv, 0, sizeof( tv ) );
		FD_ZERO(&readfds);
#if !defined(WINDOWS) && !defined(RISCOS)
		FD_SET(fileno(stdin),&readfds);
#endif
		FD_SET(SSL_get_fd(con),&readfds);

/*		printf("pending=%d\n",SSL_pending(con));*/
		i=select(width,&readfds,NULL,NULL,&tv);
		if ( i < 0)
			{
			fprintf(stderr,"bad select %d\n",sock_err());
			SHUTDOWN(SSL_get_fd(con));
			ret=0;
			goto end;
			}
#ifdef RISCOS
                if ( (i=riscos_ReadTTY( buf, BUFSIZZ)) != 0 )
                        {
                        if ( i == -1 )  /* detect escape condition */
#else
#ifndef WINDOWS
		if (FD_ISSET(fileno(stdin),&readfds))
#endif
			{
			i=read(fileno(stdin),buf,BUFSIZZ);
			if ((i <= 0) || (buf[0] == 'Q'))
#endif

				{
				fprintf(stderr,"DONE\n");
				SHUTDOWN(SSL_get_fd(con));
				ret=0;
				goto end;
				}

			k=l=0;
			for (;;)
				{
				k=SSL_write(con,&(buf[l]),(unsigned int)i);
				if (should_retry(k))
					{
					fprintf(stderr,"W BLOCK\n");
					continue;
					}
				if (k < 0)
					{
					ERR_print_errors(bio_err);
					SHUTDOWN(SSL_get_fd(con));
					ret=0;
					goto end;
					}
				l+=k;
				i-=k;
				if (i <= 0) break;
				}
			}
		if (FD_ISSET(SSL_get_fd(con),&readfds))
			{
			i=SSL_read(con,buf,BUFSIZZ);
			if (should_retry(i))
				{
				fprintf(stderr,"R BLOCK\n");
				}
			else if (i < 0)
				{
				ERR_print_errors(bio_err);
				fprintf(stderr,"SSL_read ERROR\n");
				break;
				}
			else if (i == 0)
				{
				fprintf(stderr,"DONE\n");
				break;
				}
			else
				{
				fwrite(buf,i,1,stdout);
				fflush(stdout);
				}
			}
		}
	SHUTDOWN(SSL_get_fd(con));
	ret=0;
end:
	if (con != NULL) SSL_free(con);
	if (con2 != NULL) SSL_free(con2);
	if (ctx != NULL) SSL_CTX_free(ctx);
	if (buf != NULL)
		{
		memset(buf,0,BUFSIZZ);
		free(buf);
		}
	EXIT(ret);
	}

static void MS_CALLBACK client_info_callback(s,where,ret)
SSL *s;
int where;
int ret;
	{
	if (where == SSL_CB_CONNECT_LOOP)
		fprintf(stderr,"SSL_connect:%s\n",SSL_state_string(s));
	else if (where == SSL_CB_CONNECT_EXIT)
		{
		if (ret == 0)
			fprintf(stderr,"SSL_connect:failed in %s\n",
				SSL_state_string(s));
		else if (ret < 0)
			fprintf(stderr,"SSL_connect:error in %s\n",
				SSL_state_string(s));
		}
	}
