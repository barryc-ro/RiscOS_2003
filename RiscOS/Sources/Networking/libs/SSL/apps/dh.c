/* apps/dh.c */
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
#include <time.h>
#include <string.h>
#include "apps.h"
#include "buffer.h"
#include "err.h"
#include "bn.h"
#include "dh.h"
#include "x509.h"
#include "pem.h"

#undef PROG
#define PROG	dh_main

/* -inform arg	- input format - default PEM (one of DER, TXT or PEM)
 * -outform arg - output format - default PEM
 * -in arg	- input file - default stdin
 * -out arg	- output file - default stdout
 * -check	- check the parameters are ok
 * -noout
 * -text
 * -C
 * -noout
 */

int MAIN(argc, argv)
int argc;
char **argv;
	{
	DH *dh=NULL;
	int i,badops=0,text=0;
	BIO *in=NULL,*out=NULL;
	int informat,outformat,check=0,noout=0,C=0,ret=1;
	char *infile,*outfile,*prog;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	infile=NULL;
	outfile=NULL;
	informat=FORMAT_PEM;
	outformat=FORMAT_PEM;

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
		else if (strcmp(*argv,"-check") == 0)
			check=1;
		else if (strcmp(*argv,"-text") == 0)
			text=1;
		else if (strcmp(*argv,"-C") == 0)
			C=1;
		else if (strcmp(*argv,"-noout") == 0)
			noout=1;
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
		fprintf(stderr," -inform arg   input format - one of DER TXT PEM\n");
		fprintf(stderr," -outform arg  output format - one of DER TXT PEM\n");
		fprintf(stderr," -in arg       inout file\n");
		fprintf(stderr," -out arg      output file\n");
		fprintf(stderr," -check        check the DH parameters\n");
		fprintf(stderr," -text         check the DH parameters\n");
		fprintf(stderr," -C            Output C code\n");
		fprintf(stderr," -noout        no output\n");
		goto end;
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

	if	(informat == FORMAT_ASN1)
		dh=d2i_DHparams_bio(in,NULL);
	else if (informat == FORMAT_PEM)
		dh=PEM_read_bio_DHparams(in,NULL,NULL);
	else
		{
		fprintf(stderr,"bad input format specified\n");
		goto end;
		}
	if (dh == NULL)
		{
		fprintf(stderr,"unable to load DH parameters\n");
		ERR_print_errors(bio_err);
		goto end;
		}

	

	if (text)
		{
		DHparams_print(out,dh);
#ifdef undef
		printf("p=");
		BN_print(stdout,dh->p);
		printf("\ng=");
		BN_print(stdout,dh->g);
		printf("\n");
		if (dh->length != 0)
			printf("recomented private length=%ld\n",dh->length);
#endif
		}
	
	if (check)
		{
		int i;

		if (!DH_check(dh,&i))
			{
			ERR_print_errors(bio_err);
			goto end;
			}
		if (i & DH_CHECK_P_NOT_PRIME)
			printf("p value is not prime\n");
		if (i & DH_CHECK_P_NOT_STRONG_PRIME)
			printf("p value is not a strong prime\n");
		if (i & DH_UNABLE_TO_CHECK_GENERATOR)
			printf("unable to check the generator value\n");
		if (i & DH_NOT_SUITABLE_GENERATOR)
			printf("the g value is not a generator\n");
		if (i == 0)
			printf("DH parameters appear to be ok.\n");
		}
	if (C)
		{
		unsigned char *data;
		int len,l;

		len=BN_num_bytes(dh->p);
		data=(unsigned char *)malloc(len);
		if (data == NULL)
			{
			perror("malloc");
			goto end;
			}
		l=BN_bn2bin(dh->p,data);
		printf("unsigned char dh%d_p={",BN_num_bits(dh->p));
		for (i=0; i<l; i++)
			{
			if ((i%12) == 0) printf("\n\t");
			printf("0x%02X,",data[i]);
			}
		printf("\n\t};\n");

		l=BN_bn2bin(dh->g,data);
		printf("unsigned char dh%d_g={",BN_num_bits(dh->p));
		for (i=0; i<l; i++)
			{
			if ((i%12) == 0) printf("\n\t");
			printf("0x%02X,",data[i]);
			}
		printf("\n\t};\n");
		}

	if (!noout)
		{
		if 	(outformat == FORMAT_ASN1)
			i=i2d_DHparams_bio(out,dh);
		else if (outformat == FORMAT_PEM)
			i=PEM_write_bio_DHparams(out,dh);
		else	{
			fprintf(stderr,"bad output format specified for outfile\n");
			goto end;
			}
		if (!i)
			{
			fprintf(stderr,"unable to write DH paramaters\n");
			ERR_print_errors(bio_err);
			goto end;
			}
		}
	ret=0;
end:
	if (in != NULL) BIO_free(in);
	if (out != NULL) BIO_free(out);
	if (dh != NULL) DH_free(dh);
	EXIT(ret);
	}
