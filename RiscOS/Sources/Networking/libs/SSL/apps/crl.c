/* apps/crl.c */
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
#define PROG	crl_main

#undef POSTFIX
#define	POSTFIX	".rvk"

#define FORMAT_UNDEF	0
#define FORMAT_ASN1	1
#define FORMAT_TEXT	2
#define FORMAT_PEM	3

static char *ctl_usage[]={
"usage: crl args\n",
"\n",
" -inform arg     - input format - default PEM (one of DER, TXT or PEM)\n",
" -outform arg    - output format - default PEM\n",
" -in arg         - input file - default stdin\n",
" -out arg        - output file - default stdout\n",
" -hash           - print hash value\n",
" -issuer         - print issuer DN\n",
" -lastupdate     - lastUpdate field\n",
" -nextupdate     - nextUpdate field\n",
" -noout          - no CRL output\n",
NULL
};

#ifndef NOPROTO
static X509_CRL *load_crl(char *file, int format);
#else
static X509_CRL *load_crl();
#endif

int MAIN(argc, argv)
int argc;
char **argv;
	{
	X509_CRL *x=NULL;
	int ret=1,i,num,badops=0;
	BIO *out=NULL;
	int informat,outformat;
	char *infile=NULL,*outfile=NULL;
	char *str=NULL;
	int hash=0,issuer=0,lastupdate=0,nextupdate=0,noout=0;
	char **pp;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	informat=FORMAT_PEM;
	outformat=FORMAT_PEM;

	argc--;
	argv++;
	num=0;
	while (argc >= 1)
		{
#ifdef undef
		if	(strcmp(*argv,"-p") == 0)
			{
			if (--argc < 1) goto bad;
			if (!args_from_file(++argv,Nargc,Nargv)) { goto end; }*/
			}
#endif
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
		else if (strcmp(*argv,"-hash") == 0)
			hash= ++num;
		else if (strcmp(*argv,"-issuer") == 0)
			issuer= ++num;
		else if (strcmp(*argv,"-lastupdate") == 0)
			lastupdate= ++num;
		else if (strcmp(*argv,"-nextupdate") == 0)
			nextupdate= ++num;
		else if (strcmp(*argv,"-noout") == 0)
			noout= ++num;
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
		for (pp=ctl_usage; (*pp != NULL); pp++)
			fprintf(stderr,*pp);
		goto end;
		}

	ERR_load_crypto_strings();
	x=load_crl(infile,informat);
	if (x == NULL) { goto end; }

	if (num)
		{
		for (i=1; i<=num; i++)
			{
			if (issuer == i)
				{
				str=X509_NAME_oneline(x->crl->issuer);
				if (str == NULL)
					{
					fprintf(stderr,"unable to get issuer Name from CRL\n");
					ERR_print_errors(bio_err);
					goto end;
					}
				fprintf(stdout,"issuer= %s\n",str);
				free(str);
				}

			if (hash == i)
				{
				fprintf(stdout,"%08lx\n",
					X509_name_hash(x->crl->issuer));
				}
			if (lastupdate == i)
				{
				fprintf(stdout,"lastUpdate=%s\n",
					x->crl->lastUpdate);
				}
			if (nextupdate == i)
				{
				fprintf(stdout,"nextUpdate=%s\n",
					x->crl->nextUpdate);
				}
			}
		}

	if (noout) goto end;

	out=BIO_new(BIO_s_file());
	if (out == NULL)
		{
		ERR_print_errors(bio_err);
		goto end;
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
		i=(int)i2d_X509_CRL_bio(out,x);
	else if (outformat == FORMAT_PEM)
		i=PEM_write_bio_X509_CRL(out,x);
	else	{
		fprintf(stderr,"bad output format specified for outfile\n");
		goto end;
		}
	if (!i) { fprintf(stderr,"unable to write CRL\n"); goto end; }
	ret=0;
end:
	if (out != NULL) BIO_free(out);
	if (x != NULL) X509_CRL_free(x);
	EXIT(ret);
	}

static X509_CRL *load_crl(infile, format)
char *infile;
int format;
	{
	X509_CRL *x=NULL;
	BIO *in=NULL;

	in=BIO_new(BIO_s_file());
	if (in == NULL)
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
	if 	(format == FORMAT_ASN1)
		x=d2i_X509_CRL_bio(in,NULL);
	else if (format == FORMAT_PEM)
		x=PEM_read_bio_X509_CRL(in,NULL,NULL);
	else	{
		fprintf(stderr,"bad input format specified for input crl\n");
		goto end;
		}
	if (x == NULL)
		{
		fprintf(stderr,"unable to load CRL\n");
		ERR_print_errors(bio_err);
		goto end;
		}
	
end:
	if (in != NULL) BIO_free(in);
	return(x);
	}

