/* apps/ca.c */
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
#include <sys/types.h>
#include <sys/stat.h>
#include "apps.h"
#include "buffer.h"
#include "err.h"
#include "bn.h"
#include "rsa.h"
#include "txt_db.h"
#include "envelope.h"
#include "x509.h"
#include "objects.h"
#include "pem.h"
#include "conf.h"

#ifndef W_OK
#include <sys/file.h>
#endif

#undef PROG
#define PROG ca_main

#define BASE_SECTION	"ca"
#define CONFIG_FILE "lib/ssleay.conf"

#define ENV_DEFAULT_CA "default_ca"

#define ENV_DIR			"dir"
#define ENV_CERTS		"certs"
#define ENV_CRL_DIR		"crl_dir"
#define ENV_CA_DB		"CA_DB"
#define ENV_NEW_CERTS_DIR	"new_certs_dir"
#define ENV_CERTIFICATE 	"certificate"
#define ENV_SERIAL		"serial"
#define ENV_CRL			"crl"
#define ENV_PRIVATE_KEY		"private_key"
#define ENV_RANDFILE		"RANDFILE"
#define ENV_DEFAULT_DAYS 	"default_days"
#define ENV_DEFAULT_CRL_DAYS 	"default_crl_days"
#define ENV_DEFAULT_CRL_HOURS 	"default_crl_hours"
#define ENV_DEFAULT_MD		"default_md"
#define ENV_POLICY      	"policy"

#define ENV_DATABASE		"database"

#define DB_type         0
#define DB_exp_date     1
#define DB_rev_date     2
#define DB_serial       3       /* index - unique */
#define DB_file         4       
#define DB_name         5       /* index - unique for active */
#define DB_NUMBER       6

#define DB_TYPE_REV	'R'
#define DB_TYPE_EXP	'E'
#define DB_TYPE_VAL	'V'

static char *ca_usage[]={
"usage: ca args\n",
"\n",
" -verbose        - Talk alot while doing things\n",
" -config file    - A config file\n",
" -name arg       - The particular CA definition to use\n",
" -gencrl         - Generate a new CRL\n",
" -crldays days   - Days is when the next CRL is due\n",
" -crlhours hours - Hours is when the next CRL is due\n",
" -days arg       - number of days to certify the certificate for\n",
" -md arg         - md to use, one of md2, md5, sha or sha1\n",
" -policy arg     - The CA 'policy' to support\n",
" -keyfile arg    - PEM RSA private key file\n",
" -key arg        - key to decode the RSA private key if it is encrypted\n",
" -cert           - The CA certificate\n",
" -in file        - The input PEM encoded certificate request(s)\n",
" -out file       - Where to put the output file(s)\n",
" -outdir dir     - Where to put output certificates\n",
" -infiles ....   - The last argument, requests to process\n",
NULL
};

#ifndef NOPROTO
static void lookup_fail(char *name,char *tag);
static int MS_CALLBACK key_callback(char *buf,int len,int verify);
static unsigned long index_serial_hash(char **a);
static int index_serial_cmp(char **a, char **b);
static unsigned long index_name_hash(char **a);
static int index_name_qual(char **a);
static int index_name_cmp(char **a,char **b);
static BIGNUM *load_serial(char *serialfile);
static int save_serial(char *serialfile, BIGNUM *serial);
static int certify(X509 **xret, char *infile,RSA *rsa,X509 *x509,
	EVP_MD *dgst,STACK *policy,TXT_DB *db,BIGNUM *serial,int days,
	int batch, int verbose);
static int cmp_on_nid(X509_NAME_ENTRY **a,X509_NAME_ENTRY **b);
static void write_new_certificate(BIO *bp, X509 *x);
#else
static void lookup_fail();
static int MS_CALLBACK key_callback();
static unsigned long index_serial_hash();
static int index_serial_cmp();
static unsigned long index_name_hash();
static int index_name_qual();
static int index_name_cmp();
static BIGNUM *load_serial();
static int save_serial();
static int certify();
static int cmp_on_nid();
static void write_new_certificate();
#endif

static LHASH *conf;
static char *key=NULL;
static char *section=NULL;

int MAIN(argc, argv)
int argc;
char **argv;
	{
	int total=0;
	int total_done=0;
	int badops=0;
	int ret=1;
	int req=0;
	int verbose=0;
	int gencrl=0;
	long crldays=0;
	long crlhours=0;
	long errorline=-1;
	char *configfile=NULL;
	char *md=NULL;
	char *policy=NULL;
	char *keyfile=NULL;
	char *certfile=NULL;
	char *infile=NULL;
	char *outfile=NULL;
	char *outdir=NULL;
	char *serialfile=NULL;
	BIGNUM *serial=NULL;
	int days=0;
	int batch=0;
	RSA *rsa=NULL;
	X509 *x509=NULL;
	X509 *x=NULL;
	BIO *in=NULL,*out=NULL,*Sout=NULL,*Cout=NULL;
	char *dbfile=NULL;
	TXT_DB *db=NULL;
	X509_CRL *crl=NULL;
	X509_CRL_INFO *ci=NULL;
	X509_REVOKED *r=NULL;
	char **pp,*p,*f;
	int i,j,k,n,m;
	long l;
	EVP_MD *dgst=NULL;
	STACK *attribs=NULL;
	STACK *cert_sk=NULL;
#undef BSIZE
#define BSIZE 256
	char buf[3][BSIZE];

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	argc--;
	argv++;
	while (argc >= 1)
		{
		if	(strcmp(*argv,"-verbose") == 0)
			verbose=1;
		else if	(strcmp(*argv,"-config") == 0)
			{
			if (--argc < 1) goto bad;
			configfile= *(++argv);
			}
		else if (strcmp(*argv,"-name") == 0)
			{
			if (--argc < 1) goto bad;
			section= *(++argv);
			}
		else if (strcmp(*argv,"-days") == 0)
			{
			if (--argc < 1) goto bad;
			days=atoi(*(++argv));
			}
		else if (strcmp(*argv,"-md") == 0)
			{
			if (--argc < 1) goto bad;
			md= *(++argv);
			}
		else if (strcmp(*argv,"-policy") == 0)
			{
			if (--argc < 1) goto bad;
			policy= *(++argv);
			}
		else if (strcmp(*argv,"-keyfile") == 0)
			{
			if (--argc < 1) goto bad;
			keyfile= *(++argv);
			}
		else if (strcmp(*argv,"-key") == 0)
			{
			if (--argc < 1) goto bad;
			key= *(++argv);
			}
		else if (strcmp(*argv,"-cert") == 0)
			{
			if (--argc < 1) goto bad;
			certfile= *(++argv);
			}
		else if (strcmp(*argv,"-in") == 0)
			{
			if (--argc < 1) goto bad;
			infile= *(++argv);
			req=1;
			}
		else if (strcmp(*argv,"-out") == 0)
			{
			if (--argc < 1) goto bad;
			outfile= *(++argv);
			}
		else if (strcmp(*argv,"-outdir") == 0)
			{
			if (--argc < 1) goto bad;
			outdir= *(++argv);
			}
		else if (strcmp(*argv,"-batch") == 0)
			batch=1;
		else if (strcmp(*argv,"-gencrl") == 0)
			gencrl=1;
		else if (strcmp(*argv,"-crldays") == 0)
			{
			if (--argc < 1) goto bad;
			crldays= atol(*(++argv));
			}
		else if (strcmp(*argv,"-crlhours") == 0)
			{
			if (--argc < 1) goto bad;
			crlhours= atol(*(++argv));
			}
		else if (strcmp(*argv,"-infiles") == 0)
			{
			argc--;
			argv++;
			req=1;
			break;
			}
		else
			{
bad:
			fprintf(stderr,"unknown option %s\n",*argv);
			badops=1;
			break;
			}
		argc--;
		argv++;
		}

	if (badops)
		{
		char **pp;

		for (pp=ca_usage; (*pp != NULL); pp++)
			fprintf(stderr,*pp);
		goto err;
		}

	ERR_load_crypto_strings();

	/*****************************************************************/
	if (configfile == NULL)
		{
		/* We will just use 'buf[0]' as a temporary buffer.  */
		strncpy(buf[0],X509_get_default_cert_area(),
			sizeof(buf[0])-2-sizeof(CONFIG_FILE));
		strcat(buf[0],"/");
		strcat(buf[0],CONFIG_FILE);
		configfile=buf[0];
		}
	if ((conf=CONF_load(NULL,configfile,&errorline)) == NULL)
		{
		if (errorline <= 0)
			fprintf(stderr,"error loading the config file '%s'\n",
				configfile);
		else
			fprintf(stderr,"error on line %ld of config file '%s'\n"
				,errorline,configfile);
		goto err;
		}

	/* Lets get the config section we are using */
	if (section == NULL)
		{
		section=CONF_get_string(conf,BASE_SECTION,ENV_DEFAULT_CA);
		if (section == NULL)
			{
			lookup_fail(BASE_SECTION,ENV_DEFAULT_CA);
			goto err;
			}
		}

	in=BIO_new(BIO_s_file());
	out=BIO_new(BIO_s_file());
	Sout=BIO_new(BIO_s_file());
	Cout=BIO_new(BIO_s_file());
	if ((in == NULL) || (out == NULL) || (Sout == NULL) || (Cout == NULL))
		{
		ERR_print_errors(bio_err);
		goto err;
		}

	/*****************************************************************/
	/* we definitly need an RSA key, so lets get it */
	if ((keyfile == NULL) && ((keyfile=CONF_get_string(conf,
		section,ENV_PRIVATE_KEY)) == NULL))
		{
		lookup_fail(section,ENV_PRIVATE_KEY);
		goto err;
		}
	if (BIO_read_filename(in,keyfile) <= 0)
		{
		perror(keyfile);
		fprintf(stderr,"trying to load CA private key\n");
		goto err;
		}
	if (key == NULL)
		rsa=PEM_read_bio_RSAPrivateKey(in,NULL,NULL);
	else
		{
		rsa=PEM_read_bio_RSAPrivateKey(in,NULL,key_callback);
		memset(key,0,strlen(key));
		}
	if (rsa == NULL)
		{
		fprintf(stderr,"unable to load CA private key\n");
		goto err;
		}

	/*****************************************************************/
	/* we need a certificate */
	if ((certfile == NULL) && ((certfile=CONF_get_string(conf,
		section,ENV_CERTIFICATE)) == NULL))
		{
		lookup_fail(section,ENV_CERTIFICATE);
		goto err;
		}
        if (BIO_read_filename(in,certfile) <= 0)
		{
		perror(certfile);
		fprintf(stderr,"trying to load CA certificate\n");
		goto err;
		}
	x509=PEM_read_bio_X509(in,NULL,NULL);
	if (x509 == NULL)
		{
		fprintf(stderr,"unable to load CA certificate\n");
		goto err;
		}

	/*****************************************************************/
	/* lookup where to write new certificates */
	if ((outdir == NULL) && (req))
		{
		struct stat sb;

		if ((outdir=CONF_get_string(conf,section,ENV_NEW_CERTS_DIR))
			== NULL)
			{
			fprintf(stderr,"there needs to be defined a directory for new certificate to be placed in\n");
			goto err;
			}
		if (access(outdir,R_OK|W_OK|X_OK) != 0)
			{
			fprintf(stderr,"I am unable to acces the %s directory\n",outdir);
			perror(outdir);
			goto err;
			}

		if (stat(outdir,&sb) != 0)
			{
			fprintf(stderr,"unable to stat(%s)\n",outdir);
			perror(outdir);
			goto err;
			}
		if (!(sb.st_mode & S_IFDIR))
			{
			fprintf(stderr,"%s need to be a directory\n",outdir);
			perror(outdir);
			goto err;
			}
		}

	/*****************************************************************/
	/* we need to load the database file */
	if ((dbfile=CONF_get_string(conf,section,ENV_DATABASE)) == NULL)
		{
		lookup_fail(section,ENV_DATABASE);
		goto err;
		}
        if (BIO_read_filename(in,dbfile) <= 0)
		{
		perror(dbfile);
		fprintf(stderr,"unable to open '%s'\n",dbfile);
		goto err;
		}
	db=TXT_DB_read(in,DB_NUMBER);
	if (db == NULL) goto err;

	/* Lets check some fields */
	for (i=0; i<sk_num(db->data); i++)
		{
		char **pp,*p;

		pp=(char **)sk_value(db->data,i);
		if ((pp[DB_type][0] != DB_TYPE_REV) &&
			(pp[DB_rev_date][0] != '\0'))
			{
			fprintf(stderr,"entry %d: not, revoked yet has a revokation date\n",i+1);
			goto err;
			}
		if ((pp[DB_type][0] == DB_TYPE_REV) &&
			!ASN1_UTCTIME_check(pp[DB_rev_date]))
			{
			fprintf(stderr,"entry %d: invalid revokation date\n",
				i+1);
			goto err;
			}
		if (!ASN1_UTCTIME_check(pp[DB_exp_date]))
			{
			fprintf(stderr,"entry %d: invalid expiry date\n",i+1);
			goto err;
			}
		p=pp[DB_serial];
		j=strlen(p);
		if ((j&1) || (j < 2))
			{
			fprintf(stderr,"entry %d: bad serial number length (%d)\n",i+1,j);
			goto err;
			}
		while (*p)
			{
			if (!(	((*p >= '0') && (*p <= '9')) ||
				((*p >= 'A') && (*p <= 'F')) ||
				((*p >= 'a') && (*p <= 'f')))  )
				{
				fprintf(stderr,"entry %d: bad serial number characters, char pos %d, char is '%c'\n",i+1,p-pp[DB_serial],*p);
				goto err;
				}
			p++;
			}
		}
	if (verbose)
		{
		BIO_set_fp(out,stdout,BIO_NOCLOSE); /* cannot fail */
		TXT_DB_write(out,db);
		fprintf(stderr,"%d entries loaded from the database\n",
			db->data->num);
		fprintf(stderr,"generating indexs\n");
		}
	
	if (!TXT_DB_create_index(db,DB_serial,NULL,index_serial_hash,
		index_serial_cmp))
		{
		fprintf(stderr,"error creating serial number index:(%ld,%ld,%ld)\n",db->error,db->arg1,db->arg2);
		goto err;
		}

	if (!TXT_DB_create_index(db,DB_name,index_name_qual,index_name_hash,
		index_name_cmp))
		{
		fprintf(stderr,"error creating name index:(%ld,%ld,%ld)\n",
			db->error,db->arg1,db->arg2);
		goto err;
		}

	/*****************************************************************/
	if (req)
		{
		if ((md == NULL) && ((md=CONF_get_string(conf,
			section,ENV_DEFAULT_MD)) == NULL))
			{
			lookup_fail(section,ENV_DEFAULT_MD);
			goto err;
			}
		if ((dgst=EVP_get_MDbyname(md)) == NULL)
			{
			fprintf(stderr,"%s is an unsuported message digest type\n",md);
			goto err;
			}
		if (verbose)
			fprintf(stderr,"message digest is %s\n",
				OBJ_nid2ln(dgst->type));
		if ((policy == NULL) && ((policy=CONF_get_string(conf,
			section,ENV_POLICY)) == NULL))
			{
			lookup_fail(section,ENV_POLICY);
			goto err;
			}
		if (verbose)
			fprintf(stderr,"policy is %s\n",policy);

		if ((serialfile=CONF_get_string(conf,section,ENV_SERIAL))
			== NULL)
			{
			lookup_fail(section,ENV_SERIAL);
			goto err;
			}

		if (days == 0)
			{
			days=(int)CONF_get_number(conf,section,
				ENV_DEFAULT_DAYS);
			}
		if (days == 0)
			{
			fprintf(stderr,"cannot lookup how many days to certify for\n");
			goto err;
			}

		if ((serial=load_serial(serialfile)) == NULL)
			{
			fprintf(stderr,"error while loading serial number\n");
			goto err;
			}
		if (verbose)
			{
			if ((f=BN_bn2ascii(serial)) == NULL) goto err;
			fprintf(stderr,"next serial number is %s\n",f);
			free(f);
			}

		if ((attribs=CONF_get_section(conf,policy)) == NULL)
			{
			fprintf(stderr,"unable to find 'section' for %s\n",policy);
			goto err;
			}

		if (outfile != NULL)
			{

			if (BIO_write_filename(Sout,outfile) <= 0)
				{
				perror(outfile);
				goto err;
				}
			}
		else
			BIO_set_fp(Sout,stdout,BIO_NOCLOSE);

		if ((cert_sk=sk_new_null()) == NULL)
			{
			fprintf(stderr,"malloc failure\n");
			goto err;
			}
		if (infile != NULL)
			{
			total++;
			j=certify(&x,infile,rsa,x509,dgst,attribs,db,
				serial,days,batch,verbose);
			if (j < 0) goto err;
			if (j > 0)
				{
				total_done++;
				fprintf(stderr,"\n");
				if (!BN_add_word(serial,1)) goto err;
				if (!sk_push(cert_sk,(char *)x))
					{
					fprintf(stderr,"malloc failure\n");
					goto err;
					}
				}
			}
		for (i=0; i<argc; i++)
			{
			total++;
			j=certify(&x,argv[i],rsa,x509,dgst,attribs,db,
				serial,days,batch,verbose);
			if (j < 0) goto err;
			if (j > 0)
				{
				total_done++;
				fprintf(stderr,"\n");
				if (!BN_add_word(serial,1)) goto err;
				if (!sk_push(cert_sk,(char *)x))
					{
					fprintf(stderr,"malloc failure\n");
					goto err;
					}
				}
			}	
		/* we have a stack of newly certified certificates
		 * and a data base and serial number that need
		 * updating */

		if (sk_num(cert_sk) > 0)
			{
			if (!batch)
				{
				fprintf(stderr,"\n%d out of %d certificate requests certified, commit? [y/n]",total_done,total);
				fflush(stderr);
				buf[0][0]='\0';
				fgets(buf[0],10,stdin);
				if ((buf[0][0] != 'y') && (buf[0][0] != 'Y'))
					{
					fprintf(stderr,"CERTIFICATION CANCELED\n"); 
					ret=0;
					goto err;
					}
				}

			fprintf(stderr,"Write out database with %d new entries\n",sk_num(cert_sk));

			strncpy(buf[0],serialfile,BSIZE-4);
			strcat(buf[0],".new");

			if (!save_serial(buf[0],serial)) goto err;

			strncpy(buf[1],dbfile,BSIZE-4);
			strcat(buf[1],".new");
			if (BIO_write_filename(out,buf[1]) <= 0)
				{
				perror(dbfile);
				fprintf(stderr,"unable to open '%s'\n",dbfile);
				goto err;
				}
			l=TXT_DB_write(out,db);
			if (l <= 0) goto err;
			}
	
		if (verbose)
			fprintf(stderr,"writing new certificates\n");
		for (i=0; i<sk_num(cert_sk); i++)
			{
			int j,k;
			unsigned char *n,*in;

			x=(X509 *)sk_value(cert_sk,i);

			j=x->cert_info->serialNumber->length;
			in=(unsigned char *)x->cert_info->serialNumber->data;
			
			strncpy(buf[2],outdir,BSIZE-(j*2)-6);
			strcat(buf[2],"/");
			n=(unsigned char *)&(buf[2][strlen(buf[2])]);
			if (j > 0)
				{
				for (k=0; k<j; k++)
					{
					sprintf((char *)n,"%02X",*(in++));
					n+=2;
					}
				}
			else
				{
				*(n++)='0';
				*(n++)='0';
				}
			*(n++)='.'; *(n++)='p'; *(n++)='e'; *(n++)='m';
			*n='\0';
			if (verbose)
				fprintf(stderr,"writing %s\n",buf[2]);

			if (BIO_write_filename(Cout,buf[2]) <= 0)
				{
				perror(buf[2]);
				goto err;
				}
			write_new_certificate(Cout,x);
			write_new_certificate(Sout,x);
			}

		if (sk_num(cert_sk))
			{
			/* Rename the database and the serial file */
			strncpy(buf[2],serialfile,BSIZE-4);
			strcat(buf[2],".old");
			if (rename(serialfile,buf[2]) < 0)
				{
				fprintf(stderr,"unabel to rename %s to %s\n",
					serialfile,buf[2]);
				perror("reason");
				goto err;
				}
			if (rename(buf[0],serialfile) < 0)
				{
				fprintf(stderr,"unabel to rename %s to %s\n",
					buf[0],serialfile);
				perror("reason");
				rename(buf[2],serialfile);
				goto err;
				}

			strncpy(buf[2],dbfile,BSIZE-4);
			strcat(buf[2],".old");
			if (rename(dbfile,buf[2]) < 0)
				{
				fprintf(stderr,"unabel to rename %s to %s\n",
					dbfile,buf[2]);
				perror("reason");
				goto err;
				}
			if (rename(buf[1],dbfile) < 0)
				{
				fprintf(stderr,"unabel to rename %s to %s\n",
					buf[1],dbfile);
				perror("reason");
				rename(buf[2],dbfile);
				goto err;
				}
			fprintf(stderr,"Data Base Updated\n");
			}
		}
	
	/*****************************************************************/
	if (gencrl)
		{
		EVP_PKEY pkey;

		if (!crldays && !crlhours)
			{
			crldays=CONF_get_number(conf,section,
				ENV_DEFAULT_CRL_DAYS);
			crlhours=CONF_get_number(conf,section,
				ENV_DEFAULT_CRL_HOURS);
			}
		if ((crldays == 0) && (crlhours == 0))
			{
			fprintf(stderr,"cannot lookup how long until the next CRL is issuer\n");
			goto err;
			}

		if (verbose) fprintf(stderr,"making CRL\n");
		if ((crl=X509_CRL_new()) == NULL) goto err;
		ci=crl->crl;
		X509_NAME_free(ci->issuer);
		ci->issuer=X509_NAME_dup(x509->cert_info->subject);
		if (ci->issuer == NULL) goto err;

		X509_gmtime_adj(ci->lastUpdate,0);
		X509_gmtime_adj(ci->nextUpdate,(crldays*24+crlhours)*60*60);

		for (i=0; i<sk_num(db->data); i++)
			{
			pp=(char **)sk_value(db->data,i);
			if (pp[DB_type][0] == DB_TYPE_REV)
				{
				if ((r=X509_REVOKED_new()) == NULL) goto err;
				strcpy(r->revocationDate,pp[DB_rev_date]);
				j=(int)strlen(pp[DB_serial])/2;
				if ((p=(char *)malloc(j+1)) == NULL)
					{
					fprintf(stderr,"malloc failure\n");
					goto err;
					}
				r->serialNumber->length=j;
				r->serialNumber->data=(unsigned char *)p;
				f=pp[DB_serial];
				for (; j; j--)
					{
					m=0;
					for (k=0; k<2; k++)
						{
						n= *(f++);
						if ((n >= '0') && (n <= '9'))
							n-='0';
						else if ((n >= 'a')
							&& (n <= 'z'))
							n=n-'a'+10;
						else if ((n >= 'A')
							&& (n <= 'Z'))
							n=n-'A'+10;
						m=(m<<4)|n;
						}
					*(p++)=m;
					}
				sk_push(ci->revoked,(char *)r);
				}
			}
		/* sort the data so it will be written in serial
		 * number order */
		sk_find(ci->revoked,NULL);
		for (i=0; i<sk_num(ci->revoked); i++)
			{
			r=(X509_REVOKED *)sk_value(ci->revoked,i);
			r->sequence=i;
			}

		/* we how have a CRL */
		if (verbose) fprintf(stderr,"signing CRL\n");
		pkey.type=EVP_PKEY_RSA;
		pkey.pkey.rsa=rsa;
		if (md != NULL)
			{
			if ((dgst=EVP_get_MDbyname(md)) == NULL)
				{
				fprintf(stderr,"%s is an unsuported message digest type\n",md);
				goto err;
				}
			}
		else
			dgst=EVP_md5();
		if (!X509_CRL_sign(crl,&pkey,dgst)) goto err;

		PEM_write_bio_X509_CRL(Sout,crl);
		}
	/*****************************************************************/
	ret=0;
err:
	if (Cout != NULL) BIO_free(Cout);
	if (Sout != NULL) BIO_free(Sout);
	if (out != NULL) BIO_free(out);
	if (in != NULL) BIO_free(in);

	if (cert_sk != NULL)
		{
		while ((x=(X509 *)sk_pop(cert_sk)) != NULL)
			X509_free(x);
		sk_free(cert_sk);
		}
	if (ret) ERR_print_errors(bio_err);
	if (serial != NULL) BN_free(serial);
	if (db != NULL) TXT_DB_free(db);
	if (rsa != NULL) RSA_free(rsa);
	if (x509 != NULL) X509_free(x509);
	if (crl != NULL) X509_CRL_free(crl);
	if (conf != NULL) CONF_free(conf);
	EXIT(ret);
	}

static void lookup_fail(name,tag)
char *name;
char *tag;
	{
	fprintf(stderr,"variable lookup failed for %s::%s\n",name,tag);
	}

static int MS_CALLBACK key_callback(buf,len,verify)
char *buf;
int len,verify;
	{
	int i;

	if (key == NULL) return(0);
	i=strlen(key);
	i=(i > len)?len:i;
	memcpy(buf,key,i);
	return(i);
	}

static unsigned long index_serial_hash(a)
char **a;
	{
	char *n;

	n=a[DB_serial];
	while (*n == '0') n++;
	return(lh_strhash(n));
	}

static int index_serial_cmp(a,b)
char **a;
char **b;
	{
	char *aa,*bb;

	for (aa=a[DB_serial]; *aa == '0'; aa++);
	for (bb=b[DB_serial]; *bb == '0'; bb++);
	return(strcmp(aa,bb));
	}

static unsigned long index_name_hash(a)
char **a;
	{ return(lh_strhash(a[DB_name])); }

static int index_name_qual(a)
char **a;
	{ return(a[0][0] == 'V'); }

static int index_name_cmp(a,b)
char **a;
char **b;
	{ return(strcmp(a[DB_name],b[DB_name])); }

static BIGNUM *load_serial(serialfile)
char *serialfile;
	{
	BIO *in=NULL;
	BIGNUM *ret=NULL;
	MS_STATIC char buf[1024];
	ASN1_INTEGER *ai=NULL;

	if ((in=BIO_new(BIO_s_file())) == NULL)
		{
		ERR_print_errors(bio_err);
		goto err;
		}

	if (BIO_read_filename(in,serialfile) <= 0)
		{
		perror(serialfile);
		goto err;
		}
	ai=ASN1_INTEGER_new();
	if (ai == NULL) goto err;
	if (!a2i_ASN1_INTEGER(in,ai,buf,1024))
		{
		fprintf(stderr,"unable to load number from %s\n",
			serialfile);
		goto err;
		}
	ret=ASN1_INTEGER_to_BN(ai,NULL);
	if (ret == NULL)
		{
		fprintf(stderr,"error converting number from bin to BIGNUM");
		goto err;
		}
err:
	if (in != NULL) BIO_free(in);
	if (ai != NULL) ASN1_INTEGER_free(ai);
	return(ret);
	}

static int save_serial(serialfile,serial)
char *serialfile;
BIGNUM *serial;
	{
	BIO *out;
	int ret=0;
	ASN1_INTEGER *ai=NULL;

	out=BIO_new(BIO_s_file());
	if (out == NULL)
		{
		ERR_print_errors(bio_err);
		goto err;
		}
	if (BIO_write_filename(out,serialfile) <= 0)
		{
		perror(serialfile);
		goto err;
		}

	if ((ai=BN_to_ASN1_INTEGER(serial,NULL)) == NULL)
		{
		fprintf(stderr,"error converting serial to ASN.1 format\n");
		goto err;
		}
	i2a_ASN1_INTEGER(out,ai);
	BIO_puts(out,"\n");
	ret=1;
err:
	if (out != NULL) BIO_free(out);
	if (ai != NULL) ASN1_INTEGER_free(ai);
	return(ret);
	}

static int certify(xret,infile,rsa,x509,dgst,policy,db,serial,days,
	batch,verbose)
X509 **xret;
char *infile;
RSA *rsa;
X509 *x509;
EVP_MD *dgst;
STACK *policy;
TXT_DB *db;
BIGNUM *serial;
int days;
int batch;
int verbose;
	{
	BIO *in=NULL,*b_err=NULL;
	STACK *name=NULL,*CAname=NULL;
	STACK *subject=NULL;
	X509_REQ *req=NULL;
	X509 *ret=NULL;
	X509_CINF *ci;
	X509_NAME_ENTRY *ne;
	X509_NAME_ENTRY tmp,*tne,*push;
	int ok=-1,i,j;
	char *p;
	CONF_VALUE *cv;
	char *row[DB_NUMBER],**rrow,**irow=NULL;
	EVP_PKEY *pkey=NULL;
	EVP_PKEY static_pkey;

	for (i=0; i<DB_NUMBER; i++)
		row[i]=NULL;

	in=BIO_new(BIO_s_file());
	b_err=BIO_new(BIO_s_file());
	if ((in == NULL) || (b_err == NULL))
		{
		ERR_print_errors(bio_err);
		goto err;
		}
	BIO_set_fp(b_err,stderr,BIO_NOCLOSE);

	if (BIO_read_filename(in,infile) <= 0)
		{
		perror(infile);
		goto err;
		}
	if ((req=PEM_read_bio_X509_REQ(in,NULL,NULL)) == NULL)
		{
		fprintf(stderr,"Error reading certificate request in %s\n",
			infile);
		goto err;
		}
	if (verbose)
		X509_REQ_print(b_err,req);

	fprintf(stderr,"Check that the request matches the signature\n");

	if (	(req->req_info == NULL) ||
		(req->req_info->pubkey == NULL) ||
		(req->req_info->pubkey->public_key == NULL) ||
		(req->req_info->pubkey->public_key->data == NULL))
		{
		fprintf(stderr,"The certificate request appears to corrupted\n");
		fprintf(stderr,"It does not contain a publik key\n");
		goto err;
		}
	if ((pkey=X509_REQ_extract_key(req)) == NULL)
		{
		fprintf(stderr,"error unpacking public key\n");
		goto err;
		}
	i=X509_REQ_verify(req,pkey);
	EVP_PKEY_free(pkey);
	if (!i)
		{
		ok=0;
		fprintf(stderr,"Signature did not match the certificate request\n");
		goto err;
		}
	else
		fprintf(stderr,"Signature ok\n");

	fprintf(stderr,"The Subjects Distinguished Name is as follows\n");
	name=req->req_info->subject;
	for (i=0; i<sk_num(name); i++)
		{
		ne=(X509_NAME_ENTRY *)sk_value(name,i);
		j=i2a_ASN1_OBJECT(b_err,ne->object);
		for (j=22-j; j>0; j--)
			fputc(' ',stderr);
		fputc(':',stderr);
		if (ne->value->type == V_ASN1_PRINTABLESTRING)
			fprintf(stderr,"PRINTABLE:'");
		else if (ne->value->type == V_ASN1_T61STRING)
			fprintf(stderr,"T61STRING:'");
		else if (ne->value->type == V_ASN1_IA5STRING)
			fprintf(stderr,"IA5STRING:'");
		else
			fprintf(stderr,"ASN.1 %2d:'",ne->value->type);

		/* check some things */
		if ((OBJ_obj2nid(ne->object) == NID_pkcs9_emailAddress) &&
			(ne->value->type != V_ASN1_IA5STRING))
			{
			fprintf(stderr,"\nemailAddress type needs to be of type IA5STRING\n");
			goto err;
			}
		j=ASN1_PRINTABLE_type(ne->value->data);
		if (	((j == V_ASN1_T61STRING) &&
			 (ne->value->type != V_ASN1_T61STRING)) ||
			((j == V_ASN1_IA5STRING) &&
			 (ne->value->type == V_ASN1_PRINTABLESTRING)))
			{
			fprintf(stderr,"\nThe string contains characters that are ilegal for the ASN.1 type\n");
			goto err;
			}

			
		p=(char *)ne->value->data;
		for (j=ne->value->length; j>0; j--)
			{
			if ((*p >= ' ') && (*p <= '~'))
				fprintf(stderr,"%c",*p);
			else if (*p & 0x80)
				fprintf(stderr,"\\0x%02X",*p);
			else if ((unsigned char)*p == 0xf7)
				fprintf(stderr,"^?");
			else	fprintf(stderr,"^%c",*p+'@');
			p++;
			}
		fprintf(stderr,"'\n");
		}

	/* Ok, now we check the 'policy' stuff. */
	if ((subject=sk_new_null()) == NULL)
		{
		fprintf(stderr,"malloc failure\n");
		goto err;
		}

	/* take a copy of the issuer name before we mess with it. */
	CAname=X509_NAME_dup(x509->cert_info->subject);
	if (CAname == NULL) goto err;
	/* WARNING WARNING - this will reorder the 'name' objects and
	 * so don't even think about trying to i2d_X509() on x509.
	 * without fixing the 'set' fields. */
	CAname->comp=cmp_on_nid;
	name->comp=cmp_on_nid;

	for (i=0; i<sk_num(policy); i++)
		{
		cv=(CONF_VALUE *)sk_value(policy,i); /* get the object id */
		if ((j=OBJ_txt2nid(cv->name)) == NID_undef)
			{
			fprintf(stderr,"%s:unknown object type in 'policy' configuration\n",cv->name);
			goto err;
			}
		
		/* lookup the object in the supplied name list */
		tmp.object=OBJ_nid2obj(j);
		j=sk_find(name,(char *)&tmp);
		tne=(j >= 0)?((X509_NAME_ENTRY *)sk_value(name,j)):NULL;

		/* depending on the 'policy', decide what to do. */
		push=NULL;
		if (strcmp(cv->value,"optional") == 0)
			{
			if (tne != NULL)
				push=tne;
			}
		else if (strcmp(cv->value,"supplied") == 0)
			{
			if (tne == NULL)
				{
				fprintf(stderr,"The %s field needed to be supplied and was missing\n",cv->name);
				goto err;
				}
			push=tne;
			}
		else if (strcmp(cv->value,"match") == 0)
			{
			if (tne == NULL)
				{
				fprintf(stderr,"The manditory %s field was missing\n",cv->name);
				goto err;
				}
			j=sk_find(CAname,(char *)&tmp);
			if (j < 0)
				{
				fprintf(stderr,"The %s field does not exist in the CA certificate,\nthe 'policy' is missconfigured\n",cv->name);
				goto err;
				}

			push=(X509_NAME_ENTRY *)sk_value(CAname,j);
			if (ASN1_BIT_STRING_cmp(tne->value,push->value) != 0)
				{
				fprintf(stderr,"The %s field needed to be the same in the\nCA certificate (%s) and the request (%s)\n",cv->name,push->value->data,tne->value->data);
				goto err;
				}
			}
		else
			{
			fprintf(stderr,"%s:invalid type in 'policy' configuration\n",cv->value);
			goto err;
			}

		if (push != NULL)
			{
			if (	((push=X509_NAME_ENTRY_dup(push)) == NULL) ||
				(!sk_push(subject,(char *)push)))
				{
				if (push != NULL)
					X509_NAME_ENTRY_free(push);
				fprintf(stderr,"malloc failure\n");
				goto err;
				}
			}
		}

	/* We now have subject containing valid names, we just need to
	 * set the 'set' numbers */
	for (i=0; i<sk_num(subject); i++)
		{
		ne=(X509_NAME_ENTRY *)sk_value(subject,i);
		ne->set=i;
		}

	if (verbose)
		fprintf(stderr,"The subject name apears to be ok, checking data base for clashes\n");

	row[DB_name]=X509_NAME_oneline(subject);
	row[DB_serial]=BN_bn2ascii(serial);
	if ((row[DB_name] == NULL) || (row[DB_serial] == NULL))
		{
		fprintf(stderr,"malloc failur\n");
		goto err;
		}

	rrow=TXT_DB_get_by_index(db,DB_name,row);
	if (rrow != NULL)
		{
		fprintf(stderr,"ERROR:There is already a certificate for %s\n",
			row[DB_name]);
		}
	else
		{
		rrow=TXT_DB_get_by_index(db,DB_serial,row);
		if (rrow != NULL)
			{
			fprintf(stderr,"ERROR:Serial number %s has already been issued,\n",
				row[DB_serial]);
			fprintf(stderr,"      check the database/serial_file for corruption\n");
			}
		}

	if (rrow != NULL)
		{
		fprintf(stderr,
			"The matching entry has the following details\n");
		if (rrow[DB_type][0] == 'E')
			p="Expired";
		else if (rrow[DB_type][0] == 'R')
			p="Revoked";
		else if (rrow[DB_type][0] == 'V')
			p="Valid";
		else
			p="\ninvalid type, Data base error\n";
		fprintf(stderr,"Type          :%s\n",p);;
		if (rrow[DB_type][0] == 'R')
			{
			p=rrow[DB_exp_date]; if (p == NULL) p="undef";
			fprintf(stderr,"Was revoked on:%s\n",p);
			}
		p=rrow[DB_exp_date]; if (p == NULL) p="undef";
		fprintf(stderr,"Expires on    :%s\n",p);
		p=rrow[DB_serial]; if (p == NULL) p="undef";
		fprintf(stderr,"Serial Number :%s\n",p);
		p=rrow[DB_file]; if (p == NULL) p="undef";
		fprintf(stderr,"File name     :%s\n",p);
		p=rrow[DB_name]; if (p == NULL) p="undef";
		fprintf(stderr,"Subject Name  :%s\n",p);
		ok=0;
		goto err;
		}

	/* We are now totaly happy, lets make and sign the certificate */
	if (verbose)
		fprintf(stderr,"Everything appears to be ok, creating and signing the certificate\n");

	if ((ret=X509_new()) == NULL) goto err;
	ci=ret->cert_info;
/*#ifdef undef */
	if ((req->req_info->attributes != NULL) &&
		(sk_num(req->req_info->attributes) > 0))
		{
		if (ci->version == NULL)
			if ((ci->version=ASN1_INTEGER_new()) == NULL)
				goto err;
		ASN1_INTEGER_set(ci->version,2);
		j=1;
		}
	else
/* #endif */
		j=0;
	/* else ci->version=NULL; */

	if (BN_to_ASN1_INTEGER(serial,ci->serialNumber) == NULL)
		goto err;
	X509_NAME_free(ci->issuer);
	if ((ci->issuer=X509_NAME_dup(x509->cert_info->subject)) == NULL)
		goto err;

	fprintf(stderr,"Certificate is to be certified until ");
	X509_gmtime_adj(ci->validity->notBefore,0);
	X509_gmtime_adj(ci->validity->notAfter,(long)60*60*24*days);
	ASN1_UTCTIME_print(b_err,ci->validity->notAfter);
	fprintf(stderr," (%d days)\n",days);

	X509_NAME_free(ci->subject);
	ci->subject=subject;
	subject=NULL;

	X509_PUBKEY_free(ci->key);
	ci->key=req->req_info->pubkey;
	req->req_info->pubkey=NULL;

/* #ifdef 1 */
	if (j)
		{
		ci->attributes=req->req_info->attributes;
		req->req_info->attributes=NULL;
		}
/* #endif */

	if (!batch)
		{
		char buf[10];

		fprintf(stderr,"Sign the certificate? [y/n]:");
		fflush(stderr);
		buf[0]='\0';
		fgets(buf,10,stdin);
		if (!((buf[0] == 'y') || (buf[0] == 'Y')))
			{
			fprintf(stderr,"CERTIFICATE WILL NOT BE CERTIFIED\n");
			ok=0;
			goto err;
			}
		}
	static_pkey.type=EVP_PKEY_RSA;
	static_pkey.pkey.rsa=rsa;
	if (!X509_sign(ret,&static_pkey,dgst))
		goto err;

	/* We now just add it to the database */
	row[DB_type]=(char *)malloc(2);
	row[DB_exp_date]=(char *)ASN1_UTCTIME_dup(ci->validity->notAfter);
	row[DB_rev_date]=NULL;
	/* row[DB_serial] done already */
	row[DB_file]=(char *)malloc(8);
	/* row[DB_name] done already */

	if ((row[DB_type] == NULL) || (row[DB_exp_date] == NULL) ||
		(row[DB_file] == NULL))
		{
		fprintf(stderr,"malloc failure\n");
		goto err;
		}
	strcpy(row[DB_file],"unknown");
	row[DB_type][0]='V';
	row[DB_type][1]='\0';

	if ((irow=(char **)malloc(sizeof(char *)*(DB_NUMBER+1))) == NULL)
		{
		fprintf(stderr,"malloc failure\n");
		goto err;
		}

	for (i=0; i<DB_NUMBER; i++)
		{
		irow[i]=row[i];
		row[i]=NULL;
		}
	irow[DB_NUMBER]=NULL;

	if (!TXT_DB_insert(db,irow))
		{
		fprintf(stderr,"failed to update database\n");
		fprintf(stderr,"TXT_DB error number %ld\n",db->error);
		goto err;
		}
	ok=1;
err:
	for (i=0; i<DB_NUMBER; i++)
		if (row[i] != NULL) free(row[i]);

	if (req != NULL) X509_REQ_free(req);
	if (in != NULL) BIO_free(in);
	if (b_err != NULL) BIO_free(b_err);
	if (CAname != NULL)
		X509_NAME_free(CAname);
	if (ok <= 0)
		{
		if (ret != NULL) X509_free(ret);
		ret=NULL;
		if (subject != NULL)
			{
			for (i=0; i<sk_num(subject); i++)
				{
				ne=(X509_NAME_ENTRY *)sk_value(subject,i);
				X509_NAME_ENTRY_free(ne);
				}
			sk_free(subject);
			}
		}
	else
		*xret=ret;
	return(ok);
	}

static int cmp_on_nid(a,b)
X509_NAME_ENTRY **a,**b;
	{
	return(OBJ_obj2nid((*a)->object)-OBJ_obj2nid((*b)->object));
	}

static void write_new_certificate(bp,x)
BIO *bp;
X509 *x;
	{
	char *f;

	f=X509_NAME_oneline(X509_get_issuer_name(x));
	BIO_puts(bp,"issuer :");
	BIO_puts(bp,f);
	BIO_puts(bp,"\n"); free(f);
	f=X509_NAME_oneline(X509_get_subject_name(x));
	BIO_puts(bp,"subject:");
	BIO_puts(bp,f);
	BIO_puts(bp,"\n"); free(f);
	BIO_puts(bp,"serial :");
	i2a_ASN1_INTEGER(bp,x->cert_info->serialNumber);
	BIO_puts(bp,"\n\n");
	X509_print(bp,x);
	BIO_puts(bp,"\n");
	PEM_write_bio_X509(bp,x);
	BIO_puts(bp,"\n");
	}

