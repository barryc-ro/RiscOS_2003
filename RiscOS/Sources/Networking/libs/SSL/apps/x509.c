/* apps/x509.c */
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

/* pdh 26-Jun-96 Added -add option */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apps.h"
#include "buffer.h"
#include "asn1.h"
#include "err.h"
#include "bn.h"
#include "rsa.h"
#include "envelope.h"
#include "x509.h"
#include "objects.h"
#include "pem.h"

#undef PROG
#define PROG x509_main

#undef POSTFIX
#define	POSTFIX	".srl"
#define DEF_DAYS	30

#define FORMAT_UNDEF	0
#define FORMAT_ASN1	1
#define FORMAT_TEXT	2
#define FORMAT_PEM	3

#define CERT_HDR	"certificate"

static char *usage[]={
"usage: x509 args\n",
" -inform arg     - input format - default PEM (one of DER, NET or PEM)\n",
" -outform arg    - output format - default PEM (one of DER, NET or PEM\n",
" -keyform arg    - rsa key format - default PEM\n",
" -CAform arg     - CA format - default PEM\n",
" -CAkeyform arg  - CA key format - default PEM\n",
" -in arg         - input file - default stdin\n",
" -out arg        - output file - default stdout\n",
" -serial         - print serial number value\n",
" -hash           - print hash value\n",
" -subject        - print subject DN\n",
" -issuer         - print issuer DN\n",
" -startdate      - notBefore field\n",
" -enddate        - notAfter field\n",
" -noout          - no certificate output\n",

" -days arg       - How long till expiry of a signed certificate - def 30 days\n",
" -signkey arg    - self sign cert with arg\n",
" -x509toreq      - output a certification request object\n",
" -req            - input is a certificate request, sign and output.\n",
" -CA arg         - set the CA certificate, must be PEM format.\n",
" -CAkey arg      - set the CA RSA key, must be PEM format\n",
"                   missing, it is asssumed to be in the CA file.\n",
" -CAcreateserial - create serial number file if it does not exist\n",
" -CAserial       - serial file\n",
" -text           - print the certitificate in text form\n",

" -add            - add the certificate to the default !CertMap file\n",
NULL
};

#ifndef NOPROTO
static int MS_CALLBACK callb(int ok, X509 *xs, X509 *xi, int depth, int error);
static RSA *load_key(char *file, int format);
static X509 *load_cert(char *file, int format);
static int sign (X509 *x, EVP_PKEY *pkey,int days);
static int certify (CERTIFICATE_CTX *ctx,char *CAfile, X509 *x,
	X509 *xca, EVP_PKEY *pkey,char *serial, int create, int days);
#else
static int MS_CALLBACK callb();
static RSA *load_key();
static X509 *load_cert();
static int sign ();
static int certify ();
#endif

int MAIN(argc, argv)
int argc;
char **argv;
	{
	int ret=1;
	X509_REQ *req=NULL;
	X509 *x=NULL,*xca=NULL;
	RSA *rsa=NULL,*CArsa=NULL;
	int i,num,badops=0;
	BIO *out=NULL;
	BIO *STDout=NULL;
	int informat,outformat,keyformat,CAformat,CAkeyformat;
	char *infile=NULL,*outfile=NULL,*keyfile=NULL,*CAfile=NULL;
	char *CAkeyfile=NULL,*str=NULL,*CAserial=NULL;
	int text=0,serial=0,hash=0,subject=0,issuer=0,startdate=0,enddate=0;
	int noout=0,sign_flag=0,CA_flag=0,CA_createserial=0, add=0;
	int x509req=0,days=DEF_DAYS,reqfile=0;
	char **pp;
	CERTIFICATE_CTX *ctx=NULL;
	X509_REQ *rq=NULL;
	RSA *r=NULL;

#ifdef RISCOS_RM
        SSL_Library_Initialise();
#endif

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	if ((STDout=BIO_new(BIO_s_file())) != NULL)
		BIO_set_fp(STDout,stdout,BIO_NOCLOSE);

	informat=FORMAT_PEM;
	outformat=FORMAT_PEM;
	keyformat=FORMAT_PEM;
	CAformat=FORMAT_PEM;
	CAkeyformat=FORMAT_PEM;

	ctx=CERTIFICATE_CTX_new();
	if (ctx == NULL) goto end;
	argc--;
	argv++;
	num=0;
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
		else if (strcmp(*argv,"-keyform") == 0)
			{
			if (--argc < 1) goto bad;
			keyformat=str2fmt(*(++argv));
			}
		else if (strcmp(*argv,"-req") == 0)
			reqfile=1;
		else if (strcmp(*argv,"-CAform") == 0)
			{
			if (--argc < 1) goto bad;
			CAformat=str2fmt(*(++argv));
			}
		else if (strcmp(*argv,"-CAkeyform") == 0)
			{
			if (--argc < 1) goto bad;
			CAformat=str2fmt(*(++argv));
			}
		else if (strcmp(*argv,"-days") == 0)
			{
			if (--argc < 1) goto bad;
			days=atoi(*(++argv));
			if (days == 0)
				{
				fprintf(stderr,"bad number of days\n");
				goto bad;
				}
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
		else if (strcmp(*argv,"-signkey") == 0)
			{
			if (--argc < 1) goto bad;
			keyfile= *(++argv);
			sign_flag= ++num;
			}
		else if (strcmp(*argv,"-CA") == 0)
			{
			if (--argc < 1) goto bad;
			CAfile= *(++argv);
			CA_flag= ++num;
			}
		else if (strcmp(*argv,"-CAkey") == 0)
			{
			if (--argc < 1) goto bad;
			CAkeyfile= *(++argv);
			}
		else if (strcmp(*argv,"-CAserial") == 0)
			{
			if (--argc < 1) goto bad;
			CAserial= *(++argv);
			}
		else if (strcmp(*argv,"-serial") == 0)
			serial= ++num;
		else if (strcmp(*argv,"-x509toreq") == 0)
			x509req= ++num;
		else if (strcmp(*argv,"-text") == 0)
			text= ++num;
		else if (strcmp(*argv,"-hash") == 0)
			hash= ++num;
		else if (strcmp(*argv,"-add") == 0)
			add= ++num;
		else if (strcmp(*argv,"-subject") == 0)
			subject= ++num;
		else if (strcmp(*argv,"-issuer") == 0)
			issuer= ++num;
		else if (strcmp(*argv,"-dates") == 0)
			{
			startdate= ++num;
			enddate= ++num;
			}
		else if (strcmp(*argv,"-startdate") == 0)
			startdate= ++num;
		else if (strcmp(*argv,"-enddate") == 0)
			enddate= ++num;
		else if (strcmp(*argv,"-noout") == 0)
			noout= ++num;
		else if (strcmp(*argv,"-CAcreateserial") == 0)
			CA_createserial= ++num;
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
		for (pp=usage; (*pp != NULL); pp++)
			fprintf(stderr,*pp);
		goto end;
		}

	ERR_load_crypto_strings();

	if (!X509_set_default_verify_paths(ctx))
		{
		ERR_print_errors(bio_err);
		goto end;
		}

	if ((CAkeyfile == NULL) && (CA_flag) && (CAformat == FORMAT_PEM))
		{ CAkeyfile=CAfile; }
	else if ((CA_flag) && (CAkeyfile == NULL))
		{
		fprintf(stderr,"need to specify a CAkey if using the CA command\n");
		goto end;
		}

	if (reqfile)
		{
		EVP_PKEY *pkey;
		char *c;
		X509_CINF *ci;
		BIO *in;

		if (!sign_flag && !CA_flag)
			{
			fprintf(stderr,"We need a private key to sign with\n");
			goto end;
			}
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
		req=PEM_read_bio_X509_REQ(in,NULL,NULL);
		BIO_free(in);

		if (req == NULL) { perror(infile); goto end; }

		if (	(req->req_info == NULL) ||
			(req->req_info->pubkey == NULL) ||
			(req->req_info->pubkey->public_key == NULL) ||
			(req->req_info->pubkey->public_key->data == NULL))
			{
			fprintf(stderr,"The certificate request appears to corrupted\n");
			fprintf(stderr,"It does not contain a public key\n");
			goto end;
			}
		if ((pkey=X509_REQ_extract_key(req)) == NULL)
	                {
	                fprintf(stderr,"error unpacking public key\n");
	                goto end;
	                }
		i=X509_REQ_verify(req,pkey);
		EVP_PKEY_free(pkey);
	        if (!i)
			{
			fprintf(stderr,"Signature did not match the certificate request\n");
			goto end;
			}
		else
			fprintf(stderr,"Signature ok\n");

		c=X509_NAME_oneline(req->req_info->subject);
		fprintf(stderr,"subject=%s\n",c);
		free(c);
		if ((x=X509_new()) == NULL) goto end;
		ci=x->cert_info;

		ASN1_INTEGER_set(ci->serialNumber,0);

		X509_NAME_free(ci->issuer);
	        if ((ci->issuer=X509_NAME_dup(req->req_info->subject)) == NULL)
			goto end;

		X509_NAME_free(ci->subject);
	        ci->subject=req->req_info->subject;
	        req->req_info->subject=NULL;;

		X509_gmtime_adj(ci->validity->notBefore,0);
	        X509_gmtime_adj(ci->validity->notAfter,(long)60*60*24*days);

		X509_PUBKEY_free(ci->key);
		ci->key=req->req_info->pubkey;
	        req->req_info->pubkey=NULL;
		}
	else
		x=load_cert(infile,informat);

	if (x == NULL) goto end;
	if (CA_flag)
		{
		xca=load_cert(CAfile,CAformat);
		if (xca == NULL) goto end;
		}

	if (!noout || text)
		{
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
		}

	if (num)
		{
		for (i=1; i<=num; i++)
			{
			if (issuer == i)
				{
				str=X509_NAME_oneline(X509_get_issuer_name(x));
				if (str == NULL)
					{
					fprintf(stderr,"unable to get issuer Name from certificate\n");
					ERR_print_errors(bio_err);
					goto end;
					}
				fprintf(stdout,"issuer= %s\n",str);
				free(str);
				}

			if (subject == i)
				{
				str=X509_NAME_oneline(X509_get_subject_name(x));
				if (str == NULL)
					{
					fprintf(stderr,"unable to get subject Name from certificate\n");
					ERR_print_errors(bio_err);
					goto end;
					}
				fprintf(stdout,"subject=%s\n",str);
				free(str);
				}
			if (serial == i)
				{
				fprintf(stdout,"serial ");
				i2a_ASN1_INTEGER(STDout,x->cert_info->serialNumber);
				}
			if (hash == i)
				{
				fprintf(stdout,"%08lx\n",
					X509_subject_name_hash(x));
				}
			/* start pdh code */
		        if ( add == i )
		                {
		                long hash = X509_subject_name_hash(x);
		                FILE *f;

		                f = fopen( "!CertMap", "a" );
		                if ( !f )
		                        {
		                        fprintf( stderr, "Can't open !CertMap for output\n" );
		                        goto end;
		                        }
		                fprintf( f, "%08lx %s\n", hash, infile );
		                fclose( f );
		                }
		        /* end pdh code */
			if (text == i)
				{
				X509_print(out,x);
				}
			if (startdate == i)
				{
				BIO_puts(STDout,"notBefore=");
				ASN1_UTCTIME_print(STDout,
					x->cert_info->validity->notBefore);
				BIO_puts(STDout,"\n");
				}
			if (enddate == i)
				{
				BIO_puts(STDout,"notAfter=");
				ASN1_UTCTIME_print(STDout,
					x->cert_info->validity->notAfter);
				BIO_puts(STDout,"\n");
				}

			/* should be in the library */
			if (sign_flag == i)
				{
				EVP_PKEY pkey;

				fprintf(stderr,"Getting Private key\n");
				if (rsa == NULL)
					{
					rsa=load_key(keyfile,keyformat);
					if (rsa == NULL) goto end;
					}
				pkey.type=EVP_PKEY_RSA;
				pkey.pkey.rsa=rsa;
				if (!sign(x,&pkey,days)) goto end;
				}
			if (CA_flag == i)
				{
				EVP_PKEY pkey;

				fprintf(stderr,"Getting CA Private Key\n");
				if (CAkeyfile != NULL)
					{
					CArsa=load_key(CAkeyfile,CAkeyformat);
					if (CArsa == NULL) goto end;
					}
				pkey.type=EVP_PKEY_RSA;
				pkey.pkey.rsa=CArsa;
				if (!certify(ctx,CAfile,x,xca,&pkey,CAserial,
					CA_createserial,days))
					goto end;
				}
			if (x509req == i)
				{
				EVP_PKEY pkey;

				fprintf(stderr,"Getting request Private Key\n");
				if (keyfile == NULL)
					{
					fprintf(stderr,"no request key file specified\n");
					goto end;
					}
				else
					{
					r=load_key(keyfile,FORMAT_PEM);
					if (r == NULL) goto end;
					}

				fprintf(stderr,"Generating certificate request\n");

				pkey.type=EVP_PKEY_RSA;
				pkey.pkey.rsa=r;
				rq=(X509_REQ *)X509_to_X509_REQ(x,&pkey);
				if (rq == NULL)
					{
					ERR_print_errors(bio_err);
					goto end;
					}
				if (!noout)
					{
					X509_REQ_print(out,rq);
					PEM_write_bio_X509_REQ(out,rq);
					}
				noout=1;
				}
			}
		}

	if (noout)
		{
		ret=0;
		goto end;
		}

	if 	(outformat == FORMAT_ASN1)
		i=i2d_X509_bio(out,x);
	else if (outformat == FORMAT_PEM)
		i=PEM_write_bio_X509(out,x);
	else if (outformat == FORMAT_NETSCAPE)
		{
		ASN1_HEADER ah;
		ASN1_OCTET_STRING os;

		os.data=(unsigned char *)CERT_HDR;
		os.length=strlen(CERT_HDR);
		ah.header= &os;
		ah.data=(char *)x;
		ah.meth=ASN1_X509_meth();

		/* no macro for this one yet */
		i=ASN1_i2d_bio(i2d_ASN1_HEADER,out,(unsigned char *)&ah);
		}
	else	{
		fprintf(stderr,"bad output format specified for outfile\n");
		goto end;
		}
	if (!i) {
		fprintf(stderr,"unable to write certificate\n");
		ERR_print_errors(bio_err);
		goto end;
		}
	ret=0;
end:
	if (out != NULL) BIO_free(out);
	if (STDout != NULL) BIO_free(STDout);
	if (ctx != NULL) CERTIFICATE_CTX_free(ctx);
	if (req != NULL) X509_REQ_free(req);
	if (x != NULL) X509_free(x);
	if (xca != NULL) X509_free(xca);
	if (rsa != NULL) RSA_free(rsa);
	if (CArsa != NULL) RSA_free(CArsa);
	if (rq != NULL) X509_REQ_free(rq);
	if (r != NULL) RSA_free(r);
	EXIT(ret);
	}

static int certify(ctx,CAfile, x, xca, pkey, serialfile, create,days)
CERTIFICATE_CTX *ctx;
char *CAfile;
X509 *x;
X509 *xca;
EVP_PKEY *pkey;
char *serialfile;
int create;
int days;
	{
	int ret=0;
	BIO *io=NULL;
	MS_STATIC char *buf=NULL,buf2[1024];
	BIGNUM *serial=NULL;
	ASN1_INTEGER *bs=NULL,bs2;

	buf=(char *)malloc(EVP_PKEY_size(pkey)*2+
		((serialfile == NULL)
			?(strlen(CAfile)+strlen(POSTFIX)+1)
			:(strlen(serialfile)))+1);
	if (buf == NULL) { fprintf(stderr,"out of mem\n"); goto end; }
	if (serialfile == NULL)
		{
		strcpy(buf,CAfile);
		strcat(buf,POSTFIX);
		}
	else
		strcpy(buf,serialfile);
	serial=BN_new();
	bs=ASN1_INTEGER_new();
	if ((serial == NULL) || (bs == NULL))
		{
		ERR_print_errors(bio_err);
		goto end;
		}

	io=BIO_new(BIO_s_file());
	if (io == NULL)
		{
		ERR_print_errors(bio_err);
		goto end;
		}

	if (BIO_read_filename(io,buf) <= 0)
		{
		if (!create)
			{
			perror(buf);
			goto end;
			}
		else
			{
			ASN1_INTEGER_set(bs,0);
			BN_zero(serial);
			}
		}
	else
		{
		if (!a2i_ASN1_INTEGER(io,bs,buf2,1024))
			{
			fprintf(stderr,"unable to load serial number from %s\n",buf);
			ERR_print_errors(bio_err);
			goto end;
			}
		else
			{
			serial=BN_bin2bn(bs->data,bs->length,serial);
			if (serial == NULL)
				{
				fprintf(stderr,"error converting bin 2 bn");
				goto end;
				}
			}
		}

	if (!BN_add_word(serial,1))
		{ fprintf(stderr,"add_word failure\n"); goto end; }
	bs2.data=(unsigned char *)buf2;
	bs2.length=BN_bn2bin(serial,bs2.data);

	if (BIO_write_filename(io,buf) <= 0)
		{
		fprintf(stderr,"error attempting to write serial number file\n");
		perror(buf);
		goto end;
		}
	i2a_ASN1_INTEGER(io,&bs2);
	BIO_puts(io,"\n");
	BIO_free(io);
	io=NULL;

	if (!X509_add_cert(ctx,x)) goto end;
	/* Must up the references since X509_add_cert thinks it can keep
	 * the cert for it's very own :-) */
	x->references++;
	/* NOTE: this certificate can/should be self signed */
	if (!X509_cert_verify(ctx,x,callb))
		goto end;

	X509_NAME_free(x->cert_info->issuer);
	x->cert_info->issuer=X509_NAME_dup(xca->cert_info->subject);
	if (x->cert_info->validity->notBefore != NULL)
		free(x->cert_info->validity->notBefore);
	if ((x->cert_info->validity->notBefore=(char *)malloc(100)) == NULL)
		goto end;
	ASN1_INTEGER_free(x->cert_info->serialNumber);
	x->cert_info->serialNumber=bs;
	bs=NULL;

	if (x->cert_info->validity->notBefore == NULL)
		{
		fprintf(stderr,"out of mem\n");
		goto end;
		}

	X509_gmtime_adj(x->cert_info->validity->notBefore,0);
	/* hardwired expired */
	X509_gmtime_adj(x->cert_info->validity->notAfter,(long)60*60*24*days);
	if (!X509_sign(x,pkey,EVP_md5()))
		{
		ERR_print_errors(bio_err);
		goto end;
		}
	ret=1;
end:
	if (buf != NULL) free(buf);
	if (bs != NULL) ASN1_INTEGER_free(bs);
	if (io != NULL)	BIO_free(io);
	if (serial != NULL) BN_free(serial);
	return(ret);
	}

static int MS_CALLBACK callb(ok, xs, xi, depth, error)
int ok;
X509 *xs;
X509 *xi;
int depth;
int error;
	{
	/* it is ok to use a self signed certificate */
	if ((!ok) && (error == VERIFY_ERR_DEPTH_ZERO_SELF_SIGNED_CERT))
		return(1);

	/* BAD we should have gotten an error :-) */
	if (ok)
		printf("error with certificate to be certified - should be self signed\n");
	else
		{
		char *s;

		s=X509_NAME_oneline(X509_get_subject_name(xs));
		printf("%s\n",s);
		free(s);
		printf("error with certificate - error %d at depth %d\n%s\n",
			error,depth,X509_cert_verify_error_string(error));
		}
#ifdef LINT
	xi=xs; xs=xi;
#endif
	return(0);
	}

static RSA *load_key(file, format)
char *file;
int format;
	{
	BIO *key=NULL;
	RSA *rsa=NULL;

	if (file == NULL)
		{
		fprintf(stderr,"no keyfile specified\n");
		goto end;
		}
	key=BIO_new(BIO_s_file());
	if (key == NULL)
		{
		ERR_print_errors(bio_err);
		goto end;
		}
	if (BIO_read_filename(key,file) <= 0)
		{
		perror(file);
		goto end;
		}
	if	(format == FORMAT_ASN1)
		rsa=d2i_RSAPrivateKey_bio(key,NULL);
	else if (format == FORMAT_PEM)
		rsa=PEM_read_bio_RSAPrivateKey(key,NULL,NULL);
	else
		{
		fprintf(stderr,"bad input format specified for key\n");
		goto end;
		}
end:
	if (key != NULL) BIO_free(key);
	if (rsa == NULL)
		fprintf(stderr,"unable to load Private Key\n");
	return(rsa);
	}

static X509 *load_cert(file, format)
char *file;
int format;
	{
	ASN1_HEADER *ah=NULL;
	BUF_MEM *buf=NULL;
	X509 *x=NULL;
	BIO *cert;

	if ((cert=BIO_new(BIO_s_file())) == NULL)
		{
		ERR_print_errors(bio_err);
		goto end;
		}

	if (file == NULL)
		BIO_set_fp(cert,stdin,BIO_NOCLOSE);
	else
		{
		if (BIO_read_filename(cert,file) <= 0)
			{
			perror(file);
			goto end;
			}
		}
	if 	(format == FORMAT_ASN1)
		x=d2i_X509_bio(cert,NULL);
	else if (format == FORMAT_NETSCAPE)
		{
		unsigned char *p,*op;
		int size=0,i;

		/* We sort of have to do it this way because it is sort of nice
		 * to read the header first and check it, then
		 * try to read the certificate */
		buf=BUF_MEM_new();
		for (;;)
			{
			if ((buf == NULL) || (!BUF_MEM_grow(buf,size+1024*10)))
				goto end;
			i=BIO_read(cert,&(buf->data[size]),1024*10);
			size+=i;
			if (i == 0) break;
			if (i < 0)
				{
				perror("reading certificate");
				goto end;
				}
			}
		p=(unsigned char *)buf->data;
		op=p;

		/* First load the header */
		if ((ah=d2i_ASN1_HEADER(NULL,&p,(long)size)) == NULL)
			goto end;
		if ((ah->header == NULL) || (ah->header->data == NULL) ||
			(strncmp(CERT_HDR,(char *)ah->header->data,
			ah->header->length) != 0))
			{
			fprintf(stderr,"Error reading header on certificate\n");
			goto end;
			}
		/* header is ok, so now read the object */
		p=op;
		ah->meth=ASN1_X509_meth();
		if ((ah=d2i_ASN1_HEADER(&ah,&p,(long)size)) == NULL)
			goto end;
		x=(X509 *)ah->data;
		ah->data=NULL;
		}
	else if (format == FORMAT_PEM)
		x=PEM_read_bio_X509(cert,NULL,NULL);
	else	{
		fprintf(stderr,"bad input format specified for input cert\n");
		goto end;
		}
end:
	if (x == NULL)
		{
		fprintf(stderr,"unable to load certificate\n");
		ERR_print_errors(bio_err);
		}
	if (ah != NULL) ASN1_HEADER_free(ah);
	if (cert != NULL) BIO_free(cert);
	if (buf != NULL) BUF_MEM_free(buf);
	return(x);
	}

static int sign(x, pkey, days)
X509 *x;
EVP_PKEY *pkey;
int days;
	{
	int j;
	unsigned char *p;

	X509_NAME_free(x->cert_info->issuer);
	x->cert_info->issuer=X509_NAME_dup(x->cert_info->subject);
	X509_gmtime_adj(x->cert_info->validity->notBefore,0);
	/* Lets just make it 12:00am GMT, Jan 1 1970 */
	memcpy(x->cert_info->validity->notBefore,"700101120000Z",13);
	/* 28 days to be certified */
	X509_gmtime_adj(x->cert_info->validity->notAfter,(long)60*60*24*days);

	j=i2d_PublicKey(pkey,NULL);
	if (x->cert_info->key->public_key->data != NULL)
		free(x->cert_info->key->public_key->data);
	p=x->cert_info->key->public_key->data=(unsigned char *)malloc(
		(unsigned int)j+10);
	if (p == NULL) { fprintf(stderr,"out of memory\n"); return(0); }
	x->cert_info->key->public_key->length=j;
	if (!i2d_PublicKey(pkey,&p))
                {
		ERR_print_errors(bio_err);
		return(0);
		}

	if (!X509_sign(x,pkey,EVP_md5()))
		{
		ERR_print_errors(bio_err);
		return(0);
		}
	return(1);
	}
