/* crypto/x509/x509_vrf.c */
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
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cryptlib.h"
#include "lhash.h"
#include "buffer.h"
#include "md5.h"
#include "rsa.h"
#include "envelope.h"
#include "x509.h"
#include "objects.h"
#include "pem.h"

#ifndef NOPROTO
static int verify(CERTIFICATE_CTX *c, X509 *xs, int (*cb)(), int depth);
static int null_callback(int e, X509 *a, X509 *b, int c, int d);
#else
static int verify();
static int null_callback();
#endif

#ifdef DEBUG
void RSA_print_bytes();
#endif

char *X509_version="X509 part of SSLeay 0.6.0 21-Jun-1996";

int X509_add_cert_file(ctx,file, type)
CERTIFICATE_CTX *ctx;
char *file;
int type;
	{
	int ret=0;
	BIO *in=NULL;
	int i,count=0;
	X509 *x=NULL;

#ifndef WIN16
	in=BIO_new(BIO_s_file());
#else
	in=BIO_new(BIO_s_file_internal_w16());
#endif
	if (in == NULL)
		{
		SYSerr(ERR_F_FOPEN,errno);
		X509err(X509_F_X509_ADD_CERT_FILE,ERR_R_SYS_LIB);
		goto err;
		}
	if (BIO_read_filename(in,file) <= 0)
		{
		SYSerr(ERR_F_FOPEN,errno);
		X509err(X509_F_X509_ADD_CERT_FILE,ERR_R_SYS_LIB);
		goto err;
		}

	if (type == X509_FILETYPE_PEM)
		{
		for (;;)
			{
			x=PEM_read_bio_X509(in,NULL,NULL);
			if (x == NULL)
				{

				if ((ERR_GET_REASON(ERR_peek_error()) ==
					PEM_R_NO_START_LINE) && (count > 0))
					{
					ERR_clear_error();
					break;
					}
				else
					{
					X509err(X509_F_X509_ADD_CERT_FILE,
						ERR_R_PEM_LIB);
					goto err;
					}
				}
			i=X509_add_cert(ctx,x);
			if (!i) goto err;
			x=NULL;
			count++;
			}
		ret=count;
		}
	else if (type == X509_FILETYPE_ASN1)
		{
		x=d2i_X509_bio(in,NULL);
		if (x == NULL)
			{
			X509err(X509_F_X509_ADD_CERT_FILE,ERR_R_ASN1_LIB);
			goto err;
			}
		i=X509_add_cert(ctx,x);
		if (!i) goto err;
		x=NULL;
		ret=i;
		}
	else
		{
		X509err(X509_F_X509_ADD_CERT_FILE,X509_R_BAD_X509_FILETYPE);
		goto err;
		}
err:
	if (x != NULL) X509_free(x);
	if (in != NULL) BIO_free(in);
	return(ret);
	}

int X509_add_cert_dir(ctx,dir, type)
CERTIFICATE_CTX *ctx;
char *dir;
int type;
	{
	int j,len;
	int *ip;
	char *s,*ss,*p;
	char **pp;

	if (dir == NULL) return(0);

	s=dir;
	p=s;
	for (;;)
		{
		if ((*p == LIST_SEPERATOR_CHAR) || (*p == '\0'))
			{
			ss=s;
			s=p+1;
			len=(int)(p-ss);
			if (len == 0) continue;
			for (j=0; j<ctx->num_dirs; j++)
				if (strncmp(ctx->dirs[j],ss,(unsigned int)len) == 0)
					continue;
			if (ctx->num_dirs_alloced < (ctx->num_dirs+1))
				{
				ctx->num_dirs_alloced+=10;
				pp=(char **)malloc(ctx->num_dirs_alloced*
					sizeof(char *));
				ip=(int *)malloc(ctx->num_dirs_alloced*
					sizeof(int));
				if ((pp == NULL) || (ip == NULL))
					{
					X509err(X509_F_X509_ADD_CERT_DIR,ERR_R_MALLOC_FAILURE);
					return(0);
					}
				memcpy(pp,ctx->dirs,(ctx->num_dirs_alloced-10)*
					sizeof(char *));
				memcpy(ip,ctx->dirs_type,(ctx->num_dirs_alloced-10)*
					sizeof(int));
				if (ctx->dirs != NULL) free(ctx->dirs);
				if (ctx->dirs_type != NULL) free(ctx->dirs_type);
				ctx->dirs=pp;
				ctx->dirs_type=ip;
				}
			ctx->dirs_type[ctx->num_dirs]=type;
			ctx->dirs[ctx->num_dirs]=(char *)malloc((unsigned int)len+1);
			if (ctx->dirs[ctx->num_dirs] == NULL) return(0);
			strncpy(ctx->dirs[ctx->num_dirs],ss,(unsigned int)len);
			ctx->dirs[ctx->num_dirs][len]='\0';
			ctx->num_dirs++;
			}
		if (*p == '\0') break;
		p++;
		}
	return(1);
	}

static int null_callback(e,a,b,c,d)
int e;
X509 *a;
X509 *b;
int c;
int d;
	{
#ifdef LINT
	a=b; b=a; c=d; d=c;
#endif
	return(e);
	}

int X509_cert_verify(ctx,xs, cb)
CERTIFICATE_CTX *ctx;
X509 *xs;
int (*cb)();
	{
	int ret;

	if (cb == NULL)
		ret=verify(ctx,xs,null_callback,0);
	else	ret=verify(ctx,xs,cb,0);
	return(ret);
	}

static int verify(ctx,xs, cb, depth)
CERTIFICATE_CTX *ctx;
X509 *xs;
int (*cb)();
int depth;
	{
	X509 *xi=NULL;
	BUF_MEM *buf=NULL;
	EVP_PKEY *pkey=NULL;
	int ret,i;

	xi=X509_get_cert(ctx,X509_get_issuer_name(xs),NULL);
	if (xi == NULL)
		{
		if ((X509_name_cmp(xs->cert_info->subject,xs->cert_info->issuer)
			== 0) && (depth == 0))
			{
			ret=(*cb)(0,xs,xi,depth,
				VERIFY_ERR_DEPTH_ZERO_SELF_SIGNED_CERT);
			}
		else
			{
			ret=(*cb)(0,xs,xi,depth,
				VERIFY_ERR_UNABLE_TO_GET_ISSUER);
			}
		goto end;
		}

	if ((pkey=X509_extract_key(xi)) == NULL)
		{
		ret=(*cb)(0,xs,xi,depth,
			VERIFY_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY);
		goto end;
		}

	if (!X509_verify(xs,pkey)) /* EAY */
		{
		ret=(*cb)(0,xs,xi,depth,VERIFY_ERR_SIGNATURE_FAILURE);
		goto end;
		}

	i=X509_cmp_current_time(xs->cert_info->validity->notBefore);
	if (i == 0)
		{
		ret=(*cb)(0,xs,xi,depth,VERIFY_ERR_ERROR_IN_NOT_BEFORE_FIELD);
		goto end;
		}
	if (i > 0)
		{
		ret=(*cb)(0,xs,xi,depth,VERIFY_ERR_CERT_NOT_YET_VALID);
		goto end;
		}

	i=X509_cmp_current_time(xs->cert_info->validity->notAfter);
	if (i == 0)
		ret=((*cb)(0,xs,xi,depth,
			VERIFY_ERR_ERROR_IN_NOT_AFTER_FIELD));
	else if (i < 0)
		ret=(*cb)(0,xs,xi,depth,VERIFY_ERR_CERT_HAS_EXPIRED);
	
	else if (X509_name_cmp(xs->cert_info->subject,xs->cert_info->issuer)
		!= 0)
		{
		/* say this one is ok and check the parent */
		i=(*cb)(1,xs,xi,depth,VERIFY_OK);
		if (!i)
			ret=0;
		else
			ret=verify(ctx,xi,cb,depth+1);
		}
	else
		{
		/* hmm... a self signed cert was passed, same as not
		 * being able to lookup parent - bad. */
		if (depth == 0)
			{
			ret=(*cb)(0,xs,xi,depth,
				VERIFY_ERR_DEPTH_ZERO_SELF_SIGNED_CERT);
			}
		else
			{
			/* self signed CA, we are happy with this */
			ret=(*cb)(1,xs,xi,depth,VERIFY_ROOT_OK);
			}
		}
end:
	if (buf != NULL) BUF_MEM_free(buf);
	if (pkey != NULL)EVP_PKEY_free(pkey);
	return(ret);
	}

int X509_cmp_current_time(str)
char *str;
	{
	time_t offset;
	char buff1[100],buff2[100],*p;
	int i,j;

	p=buff1;
	i=strlen(str);
	if ((i < 11) || (i > 15)) return(0);
	memcpy(p,str,10);
	p+=10;
	str+=10;

	if ((*str == 'Z') || (*str == '-') || (*str == '+'))
		{ *(p++)='0'; *(p++)='0'; }
	else	{ *(p++)= *(str++); *(p++)= *(str++); }
	*(p++)='Z';
	*(p++)='\0';

	if (*str == 'Z')
		offset=0;
	else
		{
		if ((*str != '+') && (str[5] != '-'))
			return(0);
		offset=((str[1]-'0')*10+(str[2]-'0'))*60;
		offset+=(str[3]-'0')*10+(str[4]-'0');
		if (*str == '-')
			offset-=offset;
		}
	X509_gmtime_adj(buff2,offset);

	i=(buff1[0]-'0')*10+(buff1[1]-'0');
	if (i < 70) i+=100;
	j=(buff2[0]-'0')*10+(buff2[1]-'0');
	if (j < 70) j+=100;

	if (i < j) return (-1);
	if (i > j) return (1);
	i=strcmp(buff1,buff2);
	if (i == 0) /* wait a second then return younger :-) */
		return(-1);
	else
		return(i);
	}

ASN1_UTCTIME *X509_gmtime_adj(s, adj)
ASN1_UTCTIME *s;
long adj;
	{
	time_t t;

	time(&t);
	t+=adj;
	return(ASN1_UTCTIME_set(s,t));
	}

char *X509_cert_verify_error_string(n)
int n;
	{
	static char buf[100];

	switch (n)
		{
	case VERIFY_OK:
		return("ok");
	case VERIFY_ERR_UNABLE_TO_GET_ISSUER:
		return("unable to get issuer certificate");
	case VERIFY_ERR_UNABLE_TO_DECRYPT_SIGNATURE:
		return("unable to decrypt certificate's signature");
	case VERIFY_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY:
		return("unable to decode issuer public key");
	case VERIFY_ERR_UNABLE_TO_UNPACK_SIGNATURE:
		return("unable to unpack certificate's signature");
	case VERIFY_ERR_UNKNOWN_X509_SIG_ALGORITHM:
		return("unknown signature algorithm");
	case VERIFY_ERR_SIG_DIGEST_LENGTH_WRONG:
		return("signature digest length has come out wrong");
	case VERIFY_ERR_SIGNATURE_FAILURE:
		return("signature is wrong");
	case VERIFY_ERR_CERT_NOT_YET_VALID:
		return("certificate is not yet valid");
	case VERIFY_ERR_CERT_HAS_EXPIRED:
		return("certificate has expired");
	case VERIFY_ERR_ERROR_IN_NOT_BEFORE_FIELD:
		return("format error in certificate's notBefore field");
	case VERIFY_ERR_ERROR_IN_NOT_AFTER_FIELD:
		return("format error in certificate's notAfter field");
	case VERIFY_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
		return("self signed certificate");
	default:
		sprintf(buf,"error number %d",n);
		return(buf);
		}
	}

EVP_PKEY *X509_extract_key(x)
X509 *x;
	{
	EVP_PKEY *ret=NULL;
	long j;
	int type;
	unsigned char *p;

	type=OBJ_obj2nid(x->cert_info->key->algor->algorithm);
	p=x->cert_info->key->public_key->data;
	j=x->cert_info->key->public_key->length;
	if ((ret=d2i_PublicKey(type,NULL,&p,(long)j)) == NULL)
		X509err(X509_F_X509_EXTRACT_KEY,X509_R_ERR_ASN1_LIB);
	return(ret);
	}

