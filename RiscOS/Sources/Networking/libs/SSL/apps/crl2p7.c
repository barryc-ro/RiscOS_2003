/* apps/crl2p7.c */
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

/* apps/crl2pcks7.c */
/* Copyright (C) 1995 Eric Young (eay@mincom.oz.au)
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

/* This was written by Gordon Chaffee <chaffee@plateau.cs.berkeley.edu>
 * and donated 'to the cause' along with lots and lots of other fixes to
 * the library. */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "apps.h"
#include "err.h"
#include "envelope.h"
#include "x509.h"
#include "pkcs7.h"
#include "pem.h"
#include "objects.h"

#ifndef NOPROTO
static int add_certs_from_file(STACK *stack, char *certfile);
#else
static int add_certs_from_file();
#endif

#define PROG	crl2pkcs7_main

/* -inform arg	- input format - default PEM (one of DER, TXT or PEM)
 * -outform arg - output format - default PEM
 * -in arg	- input file - default stdin
 * -out arg	- output file - default stdout
 */

int MAIN(argc, argv)
int argc;
char **argv;
	{
	int i,badops=0;
	BIO *in=NULL,*out=NULL;
	int informat,outformat;
	char *infile,*outfile,*prog,*certfile;
	PKCS7 *p7 = NULL;
	PKCS7_SIGNED *p7s = NULL;
	X509_CRL *crl=NULL;
	STACK *crl_stack=NULL;
	STACK *cert_stack=NULL;
	int ret=1;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	infile=NULL;
	outfile=NULL;
	informat=FORMAT_PEM;
	outformat=FORMAT_PEM;
	certfile=NULL;

	prog=argv[0];
	argc--;
	argv++;
	while (argc >= 1)
		{
		if 	(strcmp(*argv,"-inform") == 0)
			{
			if (--argc < 1) goto bad;
			informat=str2fmt(*(++argv));
			}
		else if (strcmp(*argv,"-outform") == 0)
			{
			if (--argc < 1) goto bad;
			outformat=str2fmt(*(++argv));
			}
		else if (strcmp(*argv,"-in") == 0)
			{
			if (--argc < 1) goto bad;
			infile= *(++argv);
			}
		else if (strcmp(*argv,"-out") == 0)
			{
			if (--argc < 1) goto bad;
			outfile= *(++argv);
			}
		else if (strcmp(*argv,"-certfile") == 0)
			{
			if (--argc < 1) goto bad;
			certfile=*(++argv);
			}
		else
			{
			fprintf(stderr,"unknown option %s\n",*argv);
			badops=1;
			break;
			}
		argc--;
		argv++;
		}

	if (badops)
		{
bad:
		fprintf(stderr,"%s [options] <infile >outfile\n",prog);
		fprintf(stderr,"where options are\n");
		fprintf(stderr," -inform arg    input format - one of DER TXT PEM\n");
		fprintf(stderr," -outform arg   output format - one of DER TXT PEM\n");
		fprintf(stderr," -in arg        inout file\n");
		fprintf(stderr," -out arg       output file\n");
		fprintf(stderr," -certfile arg  certificates file of chain to a trusted CA\n");
		EXIT(1);
		}

	ERR_load_crypto_strings();

	in=BIO_new(BIO_s_file());
	out=BIO_new(BIO_s_file());
	if ((in == NULL) || (out == NULL))
		{
		ERR_print_errors(bio_err);
		goto end;
		}

	if (infile == NULL)
		BIO_set_fp(in,stdin,BIO_NOCLOSE);
	else
		{
		if (BIO_read_filename(in,infile) <= 0)
			{
			perror(infile);
			goto end;
			}
		}

	if 	(informat == FORMAT_ASN1)
		crl=d2i_X509_CRL_bio(in,NULL);
	else if (informat == FORMAT_PEM)
		crl=PEM_read_bio_X509_CRL(in,NULL,NULL);
	else	{
		fprintf(stderr,"bad input format specified for input crl\n");
		goto end;
		}
	if (crl == NULL)
		{
		fprintf(stderr,"unable to load CRL\n");
		ERR_print_errors(bio_err);
		goto end;
		}
	
	if ((p7=PKCS7_new()) == NULL) goto end;
	if ((p7s=PKCS7_SIGNED_new()) == NULL) goto end;
	p7->type=OBJ_nid2obj(NID_pkcs7_signed);
	p7->d.sign=p7s;
	p7s->contents->type=OBJ_nid2obj(NID_pkcs7_data);

	if (!ASN1_INTEGER_set(p7s->version,1)) goto end;
	if ((crl_stack=sk_new(NULL)) == NULL) goto end;
	p7s->crl=crl_stack;
	sk_push(crl_stack,(char *)crl);
	crl=NULL; /* now part of p7 for freeing */

	if ((cert_stack=sk_new(NULL)) == NULL) goto end;
	p7s->cert=cert_stack;

	if (certfile != NULL) 
		{
		if (add_certs_from_file(cert_stack,certfile) < 0)
			{
			fprintf(stderr,"error loading certificates\n");
			ERR_print_errors(bio_err);
			goto end;
			}
		}

	if (outfile == NULL)
		BIO_set_fp(out,stdout,BIO_NOCLOSE);
	else
		{
		if (BIO_write_filename(out,outfile) <= 0)
			{
			perror(outfile);
			goto end;
			}
		}

	if 	(outformat == FORMAT_ASN1)
		i=i2d_PKCS7_bio(out,p7);
	else if (outformat == FORMAT_PEM)
		i=PEM_write_bio_PKCS7(out,p7);
	else	{
		fprintf(stderr,"bad output format specified for outfile\n");
		goto end;
		}
	if (!i)
		{
		fprintf(stderr,"unable to write pkcs7 object\n");
		ERR_print_errors(bio_err);
		goto end;
		}
	ret=0;
end:
	if (in != NULL) BIO_free(in);
	if (out != NULL) BIO_free(out);
	if (p7 != NULL) PKCS7_free(p7);
	if (crl != NULL) X509_CRL_free(crl);

	EXIT(ret);
	}

/*
 *----------------------------------------------------------------------
 * int add_certs_from_file
 *
 *	Read a list of certificates to be checked from a file.
 *
 * Results:
 *	number of certs added if successful, -1 if not.
 *----------------------------------------------------------------------
 */
static int add_certs_from_file(stack,certfile)
STACK *stack;
char *certfile;
	{
	struct stat st;
	BIO *in=NULL;
	int count=0;
	int ret= -1;
	STACK *sk=NULL;
	X509_INFO *xi;

	if ((stat(certfile,&st) != 0))
		{
		fprintf(stderr,"unable to file the file, %s\n",certfile);
		goto end;
		}

	in=BIO_new(BIO_s_file());
	if ((in == NULL) || (BIO_read_filename(in,certfile) <= 0))
		{
		SYSerr(ERR_F_FOPEN,errno);
		goto end;
		}

	/* This loads from a file, a stack of x509/crl/pkey sets */
	sk=PEM_X509_INFO_read_bio(in,NULL,NULL);
	if (sk == NULL) goto end;

	/* scan over it and pull out the CRL's */
	while (sk_num(sk))
		{
		xi=(X509_INFO *)sk_shift(sk);
		if (xi->x509 != NULL)
			{
			sk_push(stack,(char *)xi->x509);
			xi->x509=NULL;
			count++;
			}
		X509_INFO_free(xi);
		}

	ret=count;
end:
 	/* never need to free x */
	if (in != NULL) BIO_free(in);
	if (sk != NULL) sk_free(sk);
	return(ret);
	}

