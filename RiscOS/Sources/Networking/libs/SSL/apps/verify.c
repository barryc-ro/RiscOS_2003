/* apps/verify.c */
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
#include "apps.h"
#include "buffer.h"
#include "err.h"
#include "x509.h"
#include "pem.h"

#undef PROG
#define PROG	verify_main

#ifndef NOPROTO
static int MS_CALLBACK cb(int ok, X509 *xs, X509 *xi, int depth, int error);
static int check(CERTIFICATE_CTX *ctx,char *file);
#else
static int MS_CALLBACK cb();
static int check();
#endif

int MAIN(argc, argv)
int argc;
char **argv;
	{
	int i,ret=1;
	char *CApath=NULL,*CAfile=NULL;
	CERTIFICATE_CTX *cert_ctx=NULL;

	cert_ctx=CERTIFICATE_CTX_new();
	if (cert_ctx == NULL) goto end;
	ERR_load_crypto_strings();

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	if (!X509_set_default_verify_paths(cert_ctx))
		goto end;

	argc--;
	argv++;
	for (;;)
		{
		if (argc >= 1)
			{
			if (strcmp(*argv,"-CApath") == 0)
				{
				if (argc-- < 1) goto end;
				CApath= *(++argv);
				}
			else if (strcmp(*argv,"-CAfile") == 0)
				{
				if (argc-- < 1) goto end;
				CAfile= *(++argv);
				}
			else if (strcmp(*argv,"-help") == 0)
				goto end;
			else if (argv[0][0] == '-')
				goto end;
			else
				break;
			argc--;
			argv++;
			}
		else
			break;
		}

	if ((!X509_load_verify_locations(cert_ctx,CAfile,CApath)) ||
		(!X509_set_default_verify_paths(cert_ctx)))
		{
		fprintf(stderr,"X509_load_verify_locations\n");
		ERR_print_errors(bio_err);
		goto end;
		}

	if (argc < 1) check(cert_ctx,NULL);
	else
		for (i=0; i<argc; i++)
			check(cert_ctx,argv[i]);
	ret=0;
end:
	if (ret == 1)
		fprintf(stderr,"usage: verify [-CApath path] [-CAfile file] cert1 cert2 ...\n");
	if (cert_ctx != NULL) CERTIFICATE_CTX_free(cert_ctx);
	EXIT(ret);
	}

static int check(ctx,file)
CERTIFICATE_CTX *ctx;
char *file;
	{
	X509 *x=NULL;
	BIO *in=NULL;
	int i=0,ret=0;

	in=BIO_new(BIO_s_file());
	if (in == NULL)
		{
		ERR_print_errors(bio_err);
		goto end;
		}

	if (file == NULL)
		BIO_set_fp(in,stdin,BIO_NOCLOSE);
	else
		{
		if (BIO_read_filename(in,file) <= 0)
			{
			perror(file);
			goto end;
			}
		}

	x=PEM_read_bio_X509(in,NULL,NULL);
	if (x == NULL)
		{
		fprintf(stdout,"%s: unable to load certificate file\n",
			(file == NULL)?"stdin":file);
		ERR_print_errors(bio_err);
		goto end;
		}
	fprintf(stdout,"%s: ",(file == NULL)?"stdin":file);
	i=X509_cert_verify(ctx,x,cb);
	ret=0;
end:
	if (i)
		{
		fprintf(stdout,"OK\n");
		ret=1;
		}
	else
		ERR_print_errors(bio_err);
	if (x != NULL) X509_free(x);
	if (in != NULL) BIO_free(in);

	return(ret);
	}

static int MS_CALLBACK cb(ok, xs, xi, depth, error)
int ok;
X509 *xs;
X509 *xi;
int depth;
int error;
	{
	if (!ok)
		{
		char *s;
		/* since we are just checking the certificates, it is
		 * ok if they are self signed. */
		if (error == VERIFY_ERR_DEPTH_ZERO_SELF_SIGNED_CERT)
			ok=1;
		else
			{
			s=X509_NAME_oneline(X509_get_subject_name(xs));
			printf("%s\n",s);
			free(s);
			printf("error %d at %d depth lookup:%s\n",error,depth,
				X509_cert_verify_error_string(error));
			}
		}
	ERR_clear_error();
#ifdef LINT
	xi=xs; xs=xi;
#endif
	return(ok);
	}

