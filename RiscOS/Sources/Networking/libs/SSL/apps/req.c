/* apps/req.c */
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
#include "rsa.h"
#include "envelope.h"
#include "rand.h"
#include "conf.h"
#include "err.h"
#include "asn1.h"
#include "x509.h"
#include "objects.h"
#include "pem.h"

#define SECTION		"req"

#define BITS		"default_bits"
#define KEYFILE		"default_keyfile"
#define DISTINGUISHED_NAME	"distinguished_name"
#define ATTRIBUTES	"attributes"

#define DEFAULT_KEY_LENGTH	512
#define MIN_KEY_LENGTH		384

#undef PROG
#define PROG	req_main

/* -inform arg	- input format - default PEM (one of DER, TXT or PEM)
 * -outform arg - output format - default PEM
 * -in arg	- input file - default stdin
 * -out arg	- output file - default stdout
 * -verify	- check request signature
 * -noout	- don't print stuff out.
 * -text	- print out human readable text.
 * -nodes	- no des encryption
 * -config file	- Load configuration file.
 * -key file	- make a request using key in file (or use it for verification).
 * -keyform	- key file format.
 * -newkey	- make a key and a request.
 * -x509	- output a self signed X509 structure instead.
 * -asn1-kludge	- output new certificate request in a format that some CA's
 *		  require.  This format is wrong
 */

#ifndef NOPROTO
static int make_REQ(X509_REQ *req,RSA *rsa,int attribs);
static int add_attribute_object(X509_NAME *n, char *text, char *def, 
	char *value, int nid,int min,int max);
static int add_DN_object(X509_NAME *n, char *text, char *def, char *value,
	int nid,int min,int max);
static void MS_CALLBACK req_cb(int p,int n);
static int fix_data(int nid,int *type,int len,int min,int max);
#else
static int make_REQ();
static int add_object();
static void MS_CALLBACK req_cb();
static int fix_data();
#endif

#ifndef MONOLITH
static LHASH *config=NULL;
#endif
static LHASH *conf=NULL;

int MAIN(argc, argv)
int argc;
char **argv;
	{
	int ex=1,x509=0,days=30;
	X509 *x509ss=NULL;
	X509_REQ *req=NULL;
	RSA *rsa=NULL;
	int i,badops=0,new=0,newkey= -1;
	BIO *in=NULL,*out=NULL;
	int informat,outformat,verify=0,noout=0,text=0,keyform=FORMAT_PEM;
	int nodes=0,kludge=0;
	char *infile,*outfile,*prog,*keyfile=NULL,*template=NULL,*keyout=NULL;
	EVP_CIPHER *cipher=EVP_des_ede3_cbc();
	char *p;

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
		else if (strcmp(*argv,"-key") == 0)
			{
			if (--argc < 1) goto bad;
			keyfile= *(++argv);
			}
		else if (strcmp(*argv,"-new") == 0)
			new=1;
		else if (strcmp(*argv,"-config") == 0)
			{	
			if (--argc < 1) goto bad;
			template= *(++argv);
			}
		else if (strcmp(*argv,"-keyform") == 0)
			{
			if (--argc < 1) goto bad;
			keyform=str2fmt(*(++argv));
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
		else if (strcmp(*argv,"-keyout") == 0)
			{
			if (--argc < 1) goto bad;
			keyout= *(++argv);
			}
		else if (strcmp(*argv,"-newkey") == 0)
			{
			if (--argc < 1) goto bad;
			newkey= atoi(*(++argv));
			new=1;
			}
		else if (strcmp(*argv,"-verify") == 0)
			verify=1;
		else if (strcmp(*argv,"-nodes") == 0)
			nodes=1;
		else if (strcmp(*argv,"-noout") == 0)
			noout=1;
		else if (strcmp(*argv,"-text") == 0)
			text=1;
		else if (strcmp(*argv,"-x509") == 0)
			x509=1;
		else if (strcmp(*argv,"-asn1-kludge") == 0)
			kludge=1;
		else if (strcmp(*argv,"-days") == 0)
			{
			if (--argc < 1) goto bad;
			days= atoi(*(++argv));
			if (days == 0) days=30;
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
		fprintf(stderr,"where options  are\n");
		fprintf(stderr," -inform arg    input format - one of DER TXT PEM\n");
		fprintf(stderr," -outform arg   output format - one of DER TXT PEM\n");
		fprintf(stderr," -in arg        inout file\n");
		fprintf(stderr," -out arg       output file\n");
		fprintf(stderr," -text          text form of request\n");
		fprintf(stderr," -noout         do not output REQ\n");
		fprintf(stderr," -verify        verify signature on REQ\n");
		fprintf(stderr," -nodes         don't encrypt the output key\n");
		fprintf(stderr," -key file	using the RSA key contained in file\n");
		fprintf(stderr," -keyform arg	key file format\n");
		fprintf(stderr," -newkey bits	generate a new key\n");
		fprintf(stderr," -keyout arg    file to send the key to\n");
		fprintf(stderr," -config file   request templace file.\n");
		fprintf(stderr," -new           new request.\n");
		fprintf(stderr," -x509          output a x509 structure instead of a cert. req.\n");
		fprintf(stderr," -days          number of days a x509 generated by -x509 is valid for.\n");
		fprintf(stderr," -asn1-kludge   Output the 'request' in a format that is wrong but some CA's\n");
		fprintf(stderr,"                have been reported as requiring\n");
		goto end;
		}

	ERR_load_crypto_strings();

	if (template != NULL)
		{
		long errline;

		conf=CONF_load(NULL,template,&errline);
		if (conf == NULL)
			{
			fprintf(stderr,"error on line %ld of %s\n",errline,template);
			ERR_print_errors(bio_err);
			goto end;
			}
		}
	else
		conf=config;

	in=BIO_new(BIO_s_file());
	out=BIO_new(BIO_s_file());
	if ((in == NULL) || (out == NULL))
		{
		ERR_print_errors(bio_err);
		goto end;
		}

	if (keyfile != NULL)
		{
		if (BIO_read_filename(in,keyfile) <= 0)
			{
			perror(keyfile);
			goto end;
			}

		if (keyform == FORMAT_ASN1)
			rsa=d2i_RSAPrivateKey_bio(in,NULL);
		else if (keyform == FORMAT_PEM)
			rsa=PEM_read_bio_RSAPrivateKey(in,NULL,NULL);
		else
			{
			fprintf(stderr,"bad input format specified for X509 request\n");
			goto end;
			}

		if (rsa == NULL)
			{
			fprintf(stderr,"unable to load RSA key\n");
			ERR_print_errors(bio_err);
			goto end;
			}
		}

	if (new && (rsa == NULL))
		{
		char *randfile;
		char buffer[200];

		if ((randfile=CONF_get_string(conf,SECTION,"RANDFILE")) == NULL)
			randfile=RAND_file_name(buffer,200);
		if ((randfile == NULL) || !RAND_load_file(randfile,1024L*1024L))
			{
			fprintf(stderr,"unable to load 'random state'\n");
			fprintf(stderr,"What this means is that the random number generator has not been seeded\n");
			fprintf(stderr,"with much random data.\n");
			fprintf(stderr,"Consider setting the RANDFILE environment variable to point at a file that\n");
			fprintf(stderr,"'random' data can be kept in.\n");
			}
		if (newkey <= 0)
			{
			newkey=(int)CONF_get_number(conf,SECTION,BITS);
			if (newkey <= 0)
				newkey=DEFAULT_KEY_LENGTH;
			}
		if (newkey < MIN_KEY_LENGTH)
			{
			fprintf(stderr,"RSA private key length is too short,\n");
			fprintf(stderr,"it needs to be at least %d bits, not %d\n",MIN_KEY_LENGTH,newkey);
			goto end;
			}
		fprintf(stderr,"Generating a %d bit private key\n",newkey);
		rsa=RSA_generate_key(newkey,0x10001,req_cb);
		if ((randfile == NULL) || (RAND_write_file(randfile) == 0))
			fprintf(stderr,"unable to write 'random state'\n");

		if (rsa == NULL)
			{
			ERR_print_errors(bio_err);
			goto end;
			}

		if (RSA_size(rsa)*8 < MIN_KEY_LENGTH)
			{
			fprintf(stderr,"RSA private key length is too short,\n");
			fprintf(stderr,"it needs to be at least %d bits, not %d\n",MIN_KEY_LENGTH,newkey);
			goto end;
			}

		if (keyout == NULL)
			keyout=CONF_get_string(conf,SECTION,KEYFILE);

		if (keyout == NULL)
			{
			fprintf(stderr,"writing new private key to stdout\n");
			BIO_set_fp(out,stdout,BIO_NOCLOSE);
			}
		else
			{
			fprintf(stderr,"writing new private key to '%s'\n",keyout);
			if (BIO_write_filename(out,keyout) <= 0)
				{
				perror(keyout);
				goto end;
				}
			}

		p=CONF_get_string(conf,SECTION,"encrypt_rsa_key");
		if ((p != NULL) && (strcmp(p,"no") == 0))
			cipher=NULL;
		if (nodes) cipher=NULL;
		
		i=0;
loop:
		if (!PEM_write_bio_RSAPrivateKey(out,rsa,cipher,
			NULL,0,NULL))
			{
			if ((ERR_GET_REASON(ERR_peek_error()) ==
				PEM_R_READ_KEY) && (i < 3))
				{
				ERR_clear_error();
				i++;
				goto loop;
				}
			ERR_print_errors(bio_err);
			goto end;
			}
		fprintf(stderr,"-----\n");
		}

	if (!new)
		{
		/* Since we are using a pre-existing certificate
		 * request, the kludge 'format' info should not be
		 * changed. */
		kludge= -1;
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

		if	(informat == FORMAT_ASN1)
			req=d2i_X509_REQ_bio(in,NULL);
		else if (informat == FORMAT_PEM)
			req=PEM_read_bio_X509_REQ(in,NULL,NULL);
		else
			{
			fprintf(stderr,"bad input format specified for X509 request\n");
			goto end;
			}
		if (req == NULL)
			{
			fprintf(stderr,"unable to load X509 request\n");
			ERR_print_errors(bio_err);
			goto end;
			}
		}

	if (new || x509)
		{
		if (rsa == NULL)
			{
			fprintf(stderr,"you need to specify a private key\n");
			goto end;
			}
		if (req == NULL)
			{
			req=X509_REQ_new();
			if (req == NULL)
				{
				ERR_print_errors(bio_err);
				goto end;
				}

			i=make_REQ(req,rsa,!x509);
			if (kludge >= 0)
				req->req_info->req_kludge=kludge;
			if (!i)
				{
				fprintf(stderr,"problems making Certificate Request\n");
				ERR_print_errors(bio_err);
				goto end;
				}
			}
		if (x509)
			{
			EVP_PKEY pkey;
			X509_CINF *ci;
			X509_REQ_INFO *ri;

			if ((x509ss=X509_new()) == NULL) goto end;
			ci=x509ss->cert_info;
			ri=req->req_info;

			/* don't set the version number, for starters
			 * the field is null and second, null is v0 
			 * if (!ASN1_INTEGER_set(ci->version,0L)) goto end;
			 */
			if (!ASN1_INTEGER_set(ci->serialNumber,0L)) goto end;

			/* steal ri->subject */
			X509_NAME_free(ci->issuer);
			ci->issuer=ri->subject;
			ri->subject=NULL;

			X509_gmtime_adj(ci->validity->notBefore,0);
			X509_gmtime_adj(ci->validity->notAfter,
				(long)60*60*24*days);

			X509_NAME_free(ci->subject);
			ci->subject=X509_NAME_dup(ci->issuer);

			X509_PUBKEY_free(ci->key);
			ci->key=ri->pubkey;
			ri->pubkey=NULL;

			pkey.type=EVP_PKEY_RSA;
			pkey.pkey.rsa=rsa;
			if (!(i=X509_sign(x509ss,&pkey,EVP_md5())))
				goto end;
			}
		else
			{
			EVP_PKEY pkey;

			pkey.type=EVP_PKEY_RSA;
			pkey.pkey.rsa=rsa;

			if (!(i=X509_REQ_sign(req,&pkey,EVP_md5())))
				goto end;
			}
		}

	if (verify && !x509)
		{
		EVP_PKEY p;
		EVP_PKEY *pkey=NULL;

		if (rsa == NULL)
			{
			pkey=X509_REQ_extract_key(req);
			if (pkey == NULL)
				{
				ERR_print_errors(bio_err);
				goto end;
				}
			}
		else
			{
			p.type=EVP_PKEY_RSA;
			p.pkey.rsa=rsa;
			pkey= &p;
			}

		i=X509_REQ_verify(req,pkey);
		if (rsa == NULL)
			EVP_PKEY_free(pkey);
		if (i < 0)
			{
			ERR_print_errors(bio_err);
			goto end;
			}
		else if (i == 0)
			{
			ERR_print_errors(bio_err);
			fprintf(stderr,"verify failure\n");
			}
		else /* if (i > 0) */
			fprintf(stderr,"verify OK\n");
		}

	if (noout && !text)
		{
		ex=0;
		goto end;
		}

	if (outfile == NULL)
		BIO_set_fp(out,stdout,BIO_NOCLOSE);
	else
		{
		if ((keyout != NULL) && (strcmp(outfile,keyout) == 0))
			i=(int)BIO_append_filename(out,outfile);
		else
			i=(int)BIO_write_filename(out,outfile);
		if (!i)
			{
			perror(outfile);
			goto end;
			}
		}

	if (text)
		{
		if (x509)
			X509_print(out,x509ss);
		else	
			X509_REQ_print(out,req);
		}

	if (!noout && !x509)
		{
		if 	(outformat == FORMAT_ASN1)
			i=i2d_X509_REQ_bio(out,req);
		else if (outformat == FORMAT_PEM)
			i=PEM_write_bio_X509_REQ(out,req);
		else	{
			fprintf(stderr,"bad output format specified for outfile\n");
			goto end;
			}
		if (!i)
			{
			fprintf(stderr,"unable to write X509 request\n");
			ERR_print_errors(bio_err);
			goto end;
			}
		}
	if (!noout && x509 && (x509ss != NULL))
		{
		if 	(outformat == FORMAT_ASN1)
			i=i2d_X509_bio(out,x509ss);
		else if (outformat == FORMAT_PEM)
			i=PEM_write_bio_X509(out,x509ss);
		else	{
			fprintf(stderr,"bad output format specified for outfile\n");
			goto end;
			}
		if (!i)
			{
			fprintf(stderr,"unable to write X509 certificate\n");
			ERR_print_errors(bio_err);
			goto end;
			}
		}
	ex=0;
end:
	if ((conf != NULL) && (conf != config)) CONF_free(conf);
	if (in != NULL) BIO_free(in);
	if (out != NULL) BIO_free(out);
	if (rsa != NULL) RSA_free(rsa);
	if (req != NULL) X509_REQ_free(req);
	if (x509ss != NULL) X509_free(x509ss);
	EXIT(ex);
	}

static int make_REQ(req,rsa,attribs)
X509_REQ *req;
RSA *rsa;
int attribs;
	{
	X509_ALGOR *a;
	int ret=0,i;
	unsigned char *s,*p;
	ASN1_OBJECT *obj;
	X509_REQ_INFO *ri;
	char buf[100];
	int nid,min,max;
	char *type,*def,*tmp,*value,*tmp_attr;
	STACK *sk,*attr=NULL;
	CONF_VALUE *v;
	
	tmp=CONF_get_string(conf,SECTION,DISTINGUISHED_NAME);
	if (tmp == NULL)
		{
		fprintf(stderr,"unable to find '%s' in config\n",
			DISTINGUISHED_NAME);
		goto err;
		}
	sk=CONF_get_section(conf,tmp);
	if (sk == NULL)
		{
		fprintf(stderr,"unable to get '%s' section\n",tmp);
		goto err;
		}

	tmp_attr=CONF_get_string(conf,SECTION,ATTRIBUTES);
	if (tmp_attr == NULL)
		attr=NULL;
	else
		{
		attr=CONF_get_section(conf,tmp_attr);
		if (attr == NULL)
			{
			fprintf(stderr,"unable to get '%s' section\n",tmp_attr);
			goto err;
			}
		}

	ri=req->req_info;

	fprintf(stderr,"You are about to be asked to enter information that will be incorperated\n");
	fprintf(stderr,"into your certificate request.\n");
	fprintf(stderr,"What you are about to enter is what is called a Distinguished Name or a DN.\n");
	fprintf(stderr,"There are quite a few fields but you can leave some blank\n");
	fprintf(stderr,"For some fields there will be a default value,\n");
	fprintf(stderr,"If you enter '.', the field will be left blank.\n");
	fprintf(stderr,"-----\n");

	/* setup version number */
	if (!ASN1_INTEGER_set(ri->version,0L)) goto err; /* version 1 */

	if (sk_num(sk))
		{
		i=-1;
start:		for (;;)
			{
			i++;
			if ((int)sk_num(sk) <= i) break;

			v=(CONF_VALUE *)sk_value(sk,i);
			type=v->name;
			if ((nid=OBJ_txt2nid(type)) == NID_undef)
				goto start;

			sprintf(buf,"%s_default",type);
			if ((def=CONF_get_string(conf,tmp,buf)) == NULL)
				def="";
				
			sprintf(buf,"%s_value",type);
			if ((value=CONF_get_string(conf,tmp,buf)) == NULL)
				value=NULL;

			sprintf(buf,"%s_min",type);
			min=(int)CONF_get_number(conf,tmp,buf);

			sprintf(buf,"%s_max",type);
			max=(int)CONF_get_number(conf,tmp,buf);

			if (!add_DN_object(ri->subject,v->value,def,value,nid,
				min,max))
				goto err;
			}
		if (sk_num(ri->subject) == 0)
			{
			fprintf(stderr,"error, no objects specified in config file\n");
			goto err;
			}

		if (attribs)
			{
			if ((attr != NULL) && (sk_num(attr) > 0))
				{
				fprintf(stderr,"\nPlease enter the following 'extra' attributes\n");
				fprintf(stderr,"to be sent with your certificate request\n");
				}

			i=-1;
start2:			for (;;)
				{
				i++;
				if ((attr == NULL) || ((int)sk_num(attr) <= i))
					break;

				v=(CONF_VALUE *)sk_value(attr,i);
				type=v->name;
				if ((nid=OBJ_txt2nid(type)) == NID_undef)
					goto start2;

				sprintf(buf,"%s_default",type);
				if ((def=CONF_get_string(conf,tmp_attr,buf))
					== NULL)
					def="";
				
				sprintf(buf,"%s_value",type);
				if ((value=CONF_get_string(conf,tmp_attr,buf))
					== NULL)
					value=NULL;

				sprintf(buf,"%s_min",type);
				min=(int)CONF_get_number(conf,tmp_attr,buf);

				sprintf(buf,"%s_max",type);
				max=(int)CONF_get_number(conf,tmp_attr,buf);

				if (!add_attribute_object(ri->attributes,
					v->value,def,value,nid,min,max))
					goto err;
				}
			}
		}
	else
		{
		fprintf(stderr,"No template, please set one up.\n");
		goto err;
		}

	/* copy public key and make algorithm*/
	a=ri->pubkey->algor;

	/* set the algorithm */
	obj=OBJ_nid2obj(NID_rsaEncryption);
	if (obj == NULL) goto err;
	ASN1_OBJECT_free(a->algorithm);
	a->algorithm=obj;

	/* set the parameter */
	if ((a->parameter == NULL) || (a->parameter->type != V_ASN1_NULL))
		{
		ASN1_TYPE_free(a->parameter);
		a->parameter=ASN1_TYPE_new();
		a->parameter->type=V_ASN1_NULL;
		}

	/* The public key */
	i=i2d_RSAPublicKey(rsa,NULL);
	s=(unsigned char *)malloc((unsigned int)i+1);
	if (s == NULL)
		{
		fprintf(stderr,"malloc failure\n");
		goto err;
		}
	p=s;
	i2d_RSAPublicKey(rsa,&p);
	ri->pubkey->public_key->length=i;
	if (ri->pubkey->public_key->data != NULL)
		free(ri->pubkey->public_key->data);
	ri->pubkey->public_key->data=s;

	ret=1;
err:
	return(ret);
	}

static int add_DN_object(n,text,def,value,nid,min,max)
X509_NAME *n;
char *text;
char *def;
char *value;
int nid;
int min;
int max;
	{
	int i,j,z;
	X509_NAME_ENTRY *ne=NULL;
	MS_STATIC char buf[1024];

start:
	fprintf(stderr,"%s [%s]:",text,def);
	fflush(stderr);
	if (value != NULL)
		{
		strcpy(buf,value);
		strcat(buf,"\n");
		fprintf(stderr,"%s\n",value);
		}
	else
		{
		buf[0]='\0';
		fgets(buf,1024,stdin);
		}

	if (buf[0] == '\0') return(0);
	else if (buf[0] == '\n')
		{
		if ((def == NULL) || (def[0] == '\0'))
			return(1);
		strcpy(buf,def);
		strcat(buf,"\n");
		}
	else if ((buf[0] == '.') && (buf[1] == '\n')) return(1);

	i=strlen(buf);
	if (buf[i-1] != '\n')
		{
		fprintf(stderr,"weird input :-(\n");
		return(0);
		}
	buf[--i]='\0';

	/* add object plus value */
	if (sk_num(n) == 0)
		j=0;
	else
		j=((X509_NAME_ENTRY *)(sk_value(n,sk_num(n)-1)))->set+1;

	if (ne == NULL)
		{
		if ((ne=X509_NAME_ENTRY_new()) == NULL)
			goto err;
		}

	ne->object=OBJ_nid2obj(nid);
	ne->set=j;

	ne->value->type=ASN1_PRINTABLE_type((unsigned char *)buf);

	z=fix_data(nid,&ne->value->type,i,min,max);
	if (z == 0)
		{
		if (value == NULL)
			goto start;
		else	goto err;
		}

	ne->value->length=i;
	ne->value->data=(unsigned char *)malloc(i+1);
	if (ne->value->data == NULL)
		{ fprintf(stderr,"malloc failure\n"); goto err; }
	memcpy(ne->value->data,buf,i+1);
	if (!sk_push(n,(char *)ne)) goto err;
	return(1);
err:
	if (ne != NULL) X509_NAME_ENTRY_free(ne);
	return(0);
	}

static int add_attribute_object(n,text,def,value,nid,min,max)
STACK *n;
char *text;
char *def;
char *value;
int nid;
int min;
int max;
	{
	int i,z;
	X509_ATTRIBUTE *ae=NULL;
	static char buf[1024];
	ASN1_BIT_STRING *bs=NULL;

start:
	fprintf(stderr,"%s [%s]:",text,def);
	fflush(stderr);
	if (value != NULL)
		{
		strcpy(buf,value);
		strcat(buf,"\n");
		fprintf(stderr,"%s\n",value);
		}
	else
		{
		buf[0]='\0';
		fgets(buf,1024,stdin);
		}

	if (buf[0] == '\0') return(0);
	else if (buf[0] == '\n')
		{
		if ((def == NULL) || (def[0] == '\0'))
			return(1);
		strcpy(buf,def);
		strcat(buf,"\n");
		}
	else if ((buf[0] == '.') && (buf[1] == '\n')) return(1);

	i=strlen(buf);
	if (buf[i-1] != '\n')
		{
		fprintf(stderr,"weird input :-(\n");
		return(0);
		}
	buf[--i]='\0';

	/* add object plus value */
	if (ae == NULL)
		{
		if ((ae=X509_ATTRIBUTE_new()) == NULL)
			goto err;
		if ((ae->value.single=ASN1_TYPE_new()) == NULL)
			goto err;
		}

	ae->object=OBJ_nid2obj(nid);
	ae->nid=nid;
	ae->set=0;

	if ((bs=ASN1_BIT_STRING_new()) == NULL) goto err;

	bs->type=ASN1_PRINTABLE_type((unsigned char *)buf);

	z=fix_data(nid,&bs->type,i,min,max);
	if (z == 0)
		{
		if (value == NULL)
			goto start;
		else	goto err;
		}

	bs->length=i;
	bs->data=(unsigned char *)malloc(i+1);
	if (bs->data == NULL)
		{ fprintf(stderr,"malloc failure\n"); goto err; }
	memcpy(bs->data,buf,i+1);

	ae->value.single->type=bs->type;
	ae->value.single->value.ptr=(char *)bs;
	bs=NULL;

	if (!sk_push(n,(char *)ae)) goto err;
	return(1);
err:
	if (ae != NULL) X509_ATTRIBUTE_free(ae);
	if (bs != NULL) ASN1_BIT_STRING_free(bs);
	return(0);
	}

static void MS_CALLBACK req_cb(p, n)
int p;
int n;
	{
	int c='*';

	if (p == 0) c='.';
	if (p == 1) c='+';
	if (p == 2) c='*';
	if (p == 3) c='\n';
	fputc(c,stderr);
	fflush(stderr);
#ifdef LINT
	p=n;
#endif
	}

static int fix_data(nid,type,len,min,max)
int nid;
int *type;
int len,min,max;
	{
	if (nid == NID_pkcs9_emailAddress)
		*type=V_ASN1_IA5STRING;
	if ((nid == NID_commonName) && (*type == V_ASN1_IA5STRING))
		*type=V_ASN1_T61STRING;
	if ((nid == NID_pkcs9_challengePassword) &&
		(*type == V_ASN1_IA5STRING))
		*type=V_ASN1_T61STRING;

	if ((nid == NID_pkcs9_unstructuredName) &&
		(*type == V_ASN1_T61STRING))
		{
		fprintf(stderr,"invalid characters in string, please re-enter the string\n");
		return(0);
		}
	if (nid == NID_pkcs9_unstructuredName)
		*type=V_ASN1_IA5STRING;

	if (len < min)
		{
		fprintf(stderr,"string is too short, it needs to be at least %d bytes long\n",min);
		return(0);
		}
	if ((max != 0) && (len > max))
		{
		fprintf(stderr,"string is too long, it needs to be less than  %d bytes long\n",max);
		return(0);
		}
	return(1);
	}
