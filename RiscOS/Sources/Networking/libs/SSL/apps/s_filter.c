/* apps/s_filter.c */
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
#include "err.h"
#include "x509.h"
#include "ssl.h"
#include "s_apps.h"

#undef PROG
#define PROG s_filter_main

/*#define TEST_CERT "client.pem" */ /* no default cert. */

#undef BUFSIZZ
#define BUFSIZZ	10*1024

extern int verify_depth;
extern int verify_error;

#define SSL_TO_CLEAR	1
#define CLEAR_TO_SSL	2
#define SSL_TO_SSL	3
#define CLEAR_TO_CLEAR	4

typedef struct connection_st
	{
	int filter;
	int fork;

	char *CAfile;	/* a CA file */
	char *CApath;	/* the CA path to CA's */
	int verify;	/* verify mode */
	char *cert_file;/* certificate to use */
	char *key_file; /* private key file */

	SSL *ssl[2];       /* SSL structure for reading/writing */
	char *host[2];	/* who has connected. */
	int port[2];	/* local port to listen too. */
	int socket[2];	/* socket */
	int readfd[2];   /* fd for normal read */
	int writefd[2];  /* fd for normal write */
	} CON;

SSL_CTX *f_ctx=NULL;

#ifdef undef
#ifndef NOPROTO
static int setup_connections (CON *c, int argc, char **argv);
static int process (CON *c);
static int get_connection(CON *con, int port, int *ret);
static void usage(void);
static void zero_con(CON *con);
#else
static int setup_connections ();
static int process ();
static int get_connection();
static void usage();
static void zero_con();
#endif

static void usage()
	{
	fprintf(stderr,"usage: client args\n");
	fprintf(stderr,"\n");
	}

static void zero_con(con)
CON *con;
	{
	int i;

	con->CAfile=	(char *)X509_get_default_cert_file();
	con->CApath=	(char *)X509_get_default_cert_dir();
	con->verify=	SSL_VERIFY_NONE;
	con->cert_file=	NULL;
	con->key_file=	NULL;
	con->filter=	SSL_TO_CLEAR; /* con1 to con2 */
	con->fork=	0;

	for (i=0; i<2; i++)
		{
		con->ssl[i]=	(SSL *)SSL_new(f_ctx);
		con->host[i]=	NULL;
		con->port[i]=	-1;
		con->socket[i]=	-1;
		con->readfd[i]=	-1;
		con->writefd[i]=-1;
		}
	}

#endif /* undef */

#ifdef undef
static CON *glob_con;
static int accept_sock;
#endif

int MAIN(argc, argv)
int argc;
char **argv;
	{
	EXIT(0);
	}
#ifdef undef
	CON con;
	char *cipher=NULL;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	verify_depth=0;
	verify_error=VERIFY_OK;

	f_ctx=SSL_CTX_new();
	if (f_ctx == NULL) { EXIT(1); }

	zero_con(&con);

	argc--;
	argv++;
	while (argc >= 1)
		{
		if ((*argv)[0] != '-') break;
		if	(strcmp(*argv,"-host1") == 0)
			{
			if (argc-- < 1) goto bad;
			con.host[0]= *(++argv);
			}
		else if	(strcmp(*argv,"-host2") == 0)
			{
			if (argc-- < 1) goto bad;
			con.host[1]= *(++argv);
			}
		else if	(strcmp(*argv,"-port1") == 0)
			{
			if (argc-- < 1) goto bad;
			con.port[0]=atoi(*(++argv));
			if (con.port[0] == 0) goto bad;
			}
		else if	(strcmp(*argv,"-port2") == 0)
			{
			if (argc-- < 1) goto bad;
			con.port[1]=atoi(*(++argv));
			if (con.port[1] == 0) goto bad;
			}

		else if	(strcmp(*argv,"-verify") == 0)
			{
			con.verify=SSL_VERIFY_PEER;
			if (argc-- < 1) goto bad;
			verify_depth=atoi(*(++argv));
			}
		else if	(strcmp(*argv,"-Verify") == 0)
			{
			con.verify=SSL_VERIFY_PEER|
				SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
			if (argc-- < 1) goto bad;
			verify_depth=atoi(*(++argv));
			}
		else if	(strcmp(*argv,"-cert") == 0)
			{
			if (argc-- < 1) goto bad;
			con.cert_file= *(++argv);
			}
		else if	(strcmp(*argv,"-key") == 0)
			{
			if (argc-- < 1) goto bad;
			con.key_file= *(++argv);
			}
		else if	(strcmp(*argv,"-CApath") == 0)
			{
			if (argc-- < 1) goto bad;
			con.CApath= *(++argv);
			}
		else if	(strcmp(*argv,"-CAfile") == 0)
			{
			if (argc-- < 1) goto bad;
			con.CAfile= *(++argv);
			}
		else if	(strcmp(*argv,"-cipher") == 0)
			{
			if (argc-- < 1) goto bad;
			cipher= *(++argv);
			}
		else if (strcmp(*argv,"-ssl2clear") == 0)
			con.filter=SSL_TO_CLEAR;
		else if (strcmp(*argv,"-clear2ssl") == 0)
			con.filter=CLEAR_TO_SSL;
		else if (strcmp(*argv,"-clear2clear") == 0)
			con.filter=CLEAR_TO_CLEAR;
		else if (strcmp(*argv,"-ssl2ssl") == 0)
			con.filter=SSL_TO_SSL;
		else
			{
			fprintf(stderr,"unknown option %s\n",*argv);
			goto bad;
			}
		argc--;
		argv++;
		}

	if ((!SSL_load_verify_locations(f_ctx,con.CAfile,con.CApath)) ||
		(!SSL_set_default_verify_paths(f_ctx)))
		{
		fprintf(stderr,"error seting default verify locations\n");
		ERR_print_errors(bio_err);
		EXIT(1);
		}

	if (!setup_connections(&con,argc,argv))
		{ EXIT(1); }

	if (con.ssl[0] != NULL)
		SSL_set_verify(con.ssl[0],con.verify,verify_callback);
	if (con.ssl[0] != NULL)
		SSL_set_verify(con.ssl[1],con.verify,verify_callback);

	
	if (cipher == NULL)
		SSL_CTX_set_cipher_list(f_ctx,getenv("SSL_CIPHER"));
	else
		SSL_CTX_set_cipher_list(f_ctx,cipher);

	if (con.ssl[0] != NULL)
		if (!set_cert_stuff(con.ssl[0],con.cert_file,con.key_file))
			{ EXIT(1); }

	if (con.ssl[1] != NULL)
		if (!set_cert_stuff(con.ssl[1],con.cert_file,con.key_file))
			{ EXIT(1); }

	argv++;
	argc--;

	process(&con);
	/* never return */

	EXIT(0);
bad:
	usage();
	EXIT(1);
	}

static int setup_connections(c, argc, argv)
CON *c;
int argc;
char **argv;
	{
	int zero,one,i;

	if ((c->port[0] == -1) && (c->host[0] != NULL))
		{
		fprintf(stderr,"you have not specified a port for host1\n");
		return(0);
		}

	if ((c->port[1] == -1) && (c->host[1] != NULL))
		{
		fprintf(stderr,"you have not specified a port for host[1]\n");
		return(0);
		}

	/* stdio(1) <convert> program(2) */
	if ((((int)c->port[0]) == -1) && (((int)c->port[1]) == -1))
		{
		c->socket[0]=-1;
		c->readfd[0]=fileno(stdin);
		c->writefd[0]=fileno(stdout);
		c->socket[1]=-1;
		if (argc > 0)
			{
#ifndef WINDOWS
			i=spawn(argc,argv,&(c->readfd[1]),&(c->writefd[1]));
			if (i <= 0)
#endif
				{
				perror("spawn");
				return(0);
				}
			}
		}
		/* program <convert> host[1] */
	else if ((c->port[0] == -1) || (c->port[1] == -1))
		{
		if (c->port[0] == -1)
			{ zero=0; one=1; }
		else	{ zero=1; one=0; }
		if (c->host[one] != NULL)
			{
			if (!init_client(&(c->socket[one]),
				c->host[one],c->port[one]))
				{
				perror("init_client");
				return(0);
				}
			}
		else
			{
			if (!get_connection(c,c->port[one],&(c->socket[one])))
				{
				perror("get_connection");
				return(0);
				}
			}
		c->readfd[one]=c->socket[one];
		c->writefd[one]=c->socket[one];
		c->socket[zero]= -1;
		if (argc > 0)
			{
#ifndef WINDOWS
			i=spawn(argc,argv,&(c->readfd[zero]),
				&(c->writefd[zero]));
			if (i <= 0)
#endif
				{
				perror("spawn");
				return(0);
				}
			}
		else
			{
			c->readfd[zero]=fileno(stdin);
			c->writefd[zero]=fileno(stdout);
			}
		}
	else /* ip:port <conv> ip:port */
		{
		for (i=0; i<2; i++)
			{
			if (c->host[i] != NULL)
				{
				if (!init_client(&(c->socket[i]),
					c->host[i],c->port[i]))
					{
					perror("init_client");
					return(0);
					}
				}
			else
				{
				if (!get_connection(c,c->port[i],
					&(c->socket[i])))
					{
					perror("get_connection");
					return(0);
					}
				}
			c->readfd[i]=c->socket[i];
			c->writefd[i]=c->socket[i];
			}
		}
	/* we now have connections setup. */	

	if (((c->filter == SSL_TO_SSL) && 
		((c->socket[0] == -1) || c->socket[1] == -1)) ||
		((c->filter == SSL_TO_CLEAR) && (c->socket[0] == -1)) ||
		((c->filter == CLEAR_TO_SSL) && (c->socket[1] == -1)))
		{
		fprintf(stderr,"SSL will only be run over a socket\n");
		return(0);
		}
/* NO SSL */
	c->ssl[0]=c->ssl[1]=NULL;
	return(1);
	}

static int process(c)
CON *c;
	{
	int width,w,i,j,skip,tot,n;
	fd_set readfds,oreadfds;
	char buf[BUFSIZZ];

	width=0;
	FD_ZERO(&readfds);
	for (i=0; i<2; i++)
		{
		FD_SET(c->readfd[i],&readfds);
		if (c->readfd[i] > width) width=c->readfd[i];
		}
	width++;
	for (;;)
		{
		n=0;
		w=0;
		skip=0;
		for (n=0; n<2; n++)
			{
			if ((c->ssl[n] != NULL) && SSL_pending(c->ssl[n]))
				skip=1;
			}

		if (!skip)
			{
			memcpy(&oreadfds,&readfds,sizeof(readfds));
			i=select(width,&oreadfds,NULL,NULL,NULL);
			if (i < 0) break;
fprintf(stderr,"%d\n",i);
			}
		for (n=0; n < 2; n++)
			{
			w=0;
			if (!skip && (c->ssl[n] == NULL))
				{
				if (FD_ISSET(c->readfd[n],&oreadfds))
					{
					i=read(c->readfd[n],buf,BUFSIZZ);
fprintf(stderr,"r=%d\n",i);
					if (i <= 0) goto done; /**/
					w++;
					}
				}
			else /* SSL_read */
				{
				if (SSL_pending(c->ssl[n]) ||
					(!skip &&
					  FD_ISSET(c->readfd[n],&oreadfds)))
					{
					i=SSL_read(c->ssl[n],buf,BUFSIZZ);
fprintf(stderr,"R=%d\n",i);
					if (i <= 0) goto done; /**/
					w++;
					}
				}
			if (w)
				{
				if (c->ssl[!n] != NULL)
					{
					if (SSL_write(c->ssl[!n],buf,
						(unsigned int)i) <= 0)
						goto done;
					}
				else
					{
					tot=0;

					for (;;)
						{
						j=write(c->writefd[!n],
							&(buf[tot]),
							(unsigned int)i);
						if (j <= 0) goto done;
						if (tot+j >= i) break;
						i-=j;
						tot+=j;
						}
					}
				}
			}
		}
done:
	if (c->socket[0] != -1) close(c->socket[0]);
	if (c->socket[1] != -1) close(c->socket[1]);
	return(1);
	}

static int get_connection(con, port, ret)
CON *con;
int port;
int *ret;
	{
#ifndef WINDOWS
	int sock;
	char *name;
	int accept_socket;

	if (!init_server(&accept_socket,port)) return(0);

	for (;;)
		{
		if (do_accept(accept_socket,&sock,&name) == 0)
			return(0);
		if (con->fork)
			{
			if (fork() == 0) break;
			free(name);
			close(sock);
			continue;
			}
		break;
		}
	*ret=sock;
	return(1);
#else
	return(0);
#endif
	}

#endif /* undef */
