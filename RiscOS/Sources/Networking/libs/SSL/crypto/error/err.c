/* crypto/error/err.c */
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
#include <string.h>
#include "buffer.h"
#include "lhash.h"
#include "err.h"
#include "crypto.h"

#define NUM_ERRORS	10
static unsigned long err_buffer[NUM_ERRORS];
static char *err_file[NUM_ERRORS];
static int err_line[NUM_ERRORS];
static int top=0,bottom=0;
static LHASH *error_hash=NULL;

#ifndef NOPROTO
static unsigned long err_hash(ERR_STRING_DATA *a);
static int err_cmp(ERR_STRING_DATA *a, ERR_STRING_DATA *b);
#else
static unsigned long err_hash();
static int err_cmp();
#endif

static ERR_STRING_DATA ERR_str_libraries[]=
	{
{ERR_PACK(ERR_LIB_NONE,0,0)		,"unknown library"},
{ERR_PACK(ERR_LIB_SYS,0,0)		,"system library"},
{ERR_PACK(ERR_LIB_BN,0,0)		,"bignum routines"},
{ERR_PACK(ERR_LIB_RSA,0,0)		,"rsa routines"},
{ERR_PACK(ERR_LIB_DH,0,0)		,"Diffie-Hellman routines"},
{ERR_PACK(ERR_LIB_EVP,0,0)		,"digital envelope routines"},
{ERR_PACK(ERR_LIB_BUF,0,0)		,"memory buffer routines"},
{ERR_PACK(ERR_LIB_OBJ,0,0)		,"object identifier routines"},
{ERR_PACK(ERR_LIB_PEM,0,0)		,"PEM routines"},
{ERR_PACK(ERR_LIB_ASN1,0,0)		,"asn1 encoding routines"},
{ERR_PACK(ERR_LIB_X509,0,0)		,"x509 certificate routines"},
{ERR_PACK(ERR_LIB_CONF,0,0)		,"configuation file routines"},
{ERR_PACK(ERR_LIB_METH,0,0)		,"X509 lookup 'method' routines"},
{ERR_PACK(ERR_LIB_SSL,0,0)		,"SSL routines"},
{ERR_PACK(ERR_LIB_RSAREF,0,0)		,"RSAref routines"},
{0,NULL},
	};

static ERR_STRING_DATA ERR_str_functs[]=
	{
	{ERR_PACK(0,ERR_F_FOPEN,0),     "fopen"},
	{0,NULL},
	};

static ERR_STRING_DATA ERR_str_reasons[]=
	{
{ERR_R_FATAL                             ,"fatal"},
{ERR_R_SYS_LIB				,"system lib"},
{ERR_R_BN_LIB				,"BN lib"},
{ERR_R_RSA_LIB				,"RSA lib"},
{ERR_R_DH_LIB				,"DH lib"},
{ERR_R_EVP_LIB				,"EVP lib"},
{ERR_R_BUF_LIB				,"BUF lib"},
{ERR_R_OBJ_LIB				,"OBJ lib"},
{ERR_R_PEM_LIB				,"PEM lib"},
{ERR_R_X509_LIB				,"X509 lib"},
{ERR_R_METH_LIB				,"METH lib"},
{ERR_R_ASN1_LIB				,"ASN1 lib"},
{ERR_R_CONF_LIB				,"CONF lib"},
{ERR_R_SSL_LIB				,"SSL lib"},
{ERR_R_MALLOC_FAILURE			,"malloc failure"},
{ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED	,"called a fuction you should not call"},
{0,NULL},
	};


void ERR_load_ERR_strings()
	{
	static int init=1;

	if (init)
		{
		CRYPTO_w_lock(CRYPTO_LOCK_ERR);
		if(!init)
			{
			CRYPTO_w_unlock(CRYPTO_LOCK_ERR);
			return;
			}
		CRYPTO_w_unlock(CRYPTO_LOCK_ERR);


		init=0;
		ERR_load_strings(0,ERR_str_libraries);
		ERR_load_strings(0,ERR_str_reasons);
		ERR_load_strings(ERR_LIB_SYS,ERR_str_functs);
		}
	}

void ERR_load_strings(lib,str)
int lib;
ERR_STRING_DATA *str;
	{
	CRYPTO_w_lock(CRYPTO_LOCK_ERR_HASH);

	if (error_hash == NULL)
		{
		error_hash=lh_new(err_hash,err_cmp);
		if (error_hash == NULL) return;
		ERR_load_ERR_strings();
		}

	while (str->error)
		{
		str->error|=ERR_PACK(lib,0,0);
		lh_insert(error_hash,(char *)str);
		str++;
		}

	CRYPTO_w_unlock(CRYPTO_LOCK_ERR_HASH);
	}

void ERR_free_strings()
	{
	CRYPTO_w_lock(CRYPTO_LOCK_ERR);

	if (error_hash != NULL)
		{
		lh_free(error_hash);
		error_hash=NULL;
		}

	CRYPTO_w_unlock(CRYPTO_LOCK_ERR);
	}

/* This needs to be on a per thread basis */

void ERR_put_error(lib,func,reason,file,line)
int lib,func,reason;
char *file;
int line;
	{
	CRYPTO_w_lock(CRYPTO_LOCK_ERR);

	top=(top+1)%NUM_ERRORS;
	if (top == bottom)
		bottom=(bottom+1)%NUM_ERRORS;
	err_buffer[top]=ERR_PACK(lib,func,reason);
	err_file[top]=file;
	err_line[top]=line;

	CRYPTO_w_unlock(CRYPTO_LOCK_ERR);
	}

void ERR_clear_error()
	{
	int i;

	CRYPTO_w_lock(CRYPTO_LOCK_ERR);

	for (i=0; i<NUM_ERRORS; i++)
		{
		err_buffer[i]=0;
		err_file[i]="";
		err_line[i]= -1;
		}
	top=bottom=0;

	CRYPTO_w_unlock(CRYPTO_LOCK_ERR);
	}

unsigned long ERR_peek_error()
	{	
	int i;

	CRYPTO_r_lock(CRYPTO_LOCK_ERR);

	if (bottom == top) return(0);
	i=(bottom+1)%NUM_ERRORS;

	CRYPTO_r_unlock(CRYPTO_LOCK_ERR);

	return(err_buffer[i]);
	}

unsigned long ERR_get_error()
	{
	int i;
	unsigned long ret;

	CRYPTO_w_lock(CRYPTO_LOCK_ERR);

	if (bottom == top) return(0);
	i=(bottom+1)%NUM_ERRORS;
	bottom=i;
	ret=err_buffer[i];
	err_buffer[i]=0;

	CRYPTO_w_unlock(CRYPTO_LOCK_ERR);

	return(ret);
	}

unsigned long ERR_peek_error_line(file,line)
char **file;
int *line;
	{	
	int i;


	CRYPTO_r_lock(CRYPTO_LOCK_ERR);

	if (bottom == top)
		{
		CRYPTO_r_unlock(CRYPTO_LOCK_ERR);
		return(0);
		}

	i=(bottom+1)%NUM_ERRORS;
	*file=err_file[i];
	*line=err_line[i];

	CRYPTO_r_unlock(CRYPTO_LOCK_ERR);

	return(err_buffer[i]);
	}

unsigned long ERR_get_error_line(file,line)
char **file;
int *line;
	{
	int i;
	unsigned long ret;

	CRYPTO_r_lock(CRYPTO_LOCK_ERR);

	if (bottom == top)
		{
		CRYPTO_r_unlock(CRYPTO_LOCK_ERR);
		return(0);
		}

	i=(bottom+1)%NUM_ERRORS;
	bottom=i;
	ret=err_buffer[i];
	*file=err_file[i];
	*line=err_line[i];
	err_buffer[i]=0;
	err_file[i]="";
	err_line[i]= -1;

	CRYPTO_r_unlock(CRYPTO_LOCK_ERR);
	return(ret);
	}

/* BAD for multi-threaded, uses a local buffer */
char *ERR_error_string(e,ret)
unsigned long e;
char *ret;
	{
	static char buf[256];
	char *ls,*fs,*rs;
	unsigned long l,f,r;
	int i;

	CRYPTO_r_lock(CRYPTO_LOCK_ERR);

	l=ERR_GET_LIB(e);
	f=ERR_GET_FUNC(e);
	r=ERR_GET_REASON(e);

	ls=ERR_lib_error_string(e);
	fs=ERR_func_error_string(e);
	rs=ERR_reason_error_string(e);

	if (ret == NULL) ret=buf;

	sprintf(&(ret[0]),"error:%08lX:",e);
	i=strlen(ret);
	if (ls == NULL)
		sprintf(&(ret[i]),":lib(%ld) ",l);
	else	sprintf(&(ret[i]),"%s",ls);
	i=strlen(ret);
	if (fs == NULL)
		sprintf(&(ret[i]),":func(%ld) ",f);
	else	sprintf(&(ret[i]),":%s",fs);
	i=strlen(ret);
	if (rs == NULL)
		sprintf(&(ret[i]),":reason(%ld)",r);
	else	sprintf(&(ret[i]),":%s",rs);

	CRYPTO_r_unlock(CRYPTO_LOCK_ERR);

	return(ret);
	}

LHASH *err_data()
	{
	return(error_hash);
	}

char *ERR_lib_error_string(e)
unsigned long e;
	{
	ERR_STRING_DATA d,*p=NULL;
	unsigned long l;

	l=ERR_GET_LIB(e);

	CRYPTO_r_lock(CRYPTO_LOCK_ERR_HASH);

	if (error_hash != NULL)
		{
		d.error=ERR_PACK(l,0,0);
		p=(ERR_STRING_DATA *)lh_retrieve(error_hash,(char *)&d);
		}

	CRYPTO_r_unlock(CRYPTO_LOCK_ERR_HASH);

	return((p == NULL)?NULL:p->string);
	}

char *ERR_func_error_string(e)
unsigned long e;
	{
	ERR_STRING_DATA d,*p=NULL;
	unsigned long l,f;

	l=ERR_GET_LIB(e);
	f=ERR_GET_FUNC(e);

	CRYPTO_r_lock(CRYPTO_LOCK_ERR_HASH);

	if (error_hash != NULL)
		{
		d.error=ERR_PACK(l,f,0);
		p=(ERR_STRING_DATA *)lh_retrieve(error_hash,(char *)&d);
		}

	CRYPTO_r_unlock(CRYPTO_LOCK_ERR_HASH);

	return((p == NULL)?NULL:p->string);
	}

char *ERR_reason_error_string(e)
unsigned long e;
	{
	ERR_STRING_DATA d,*p=NULL;
	unsigned long l,r;

	l=ERR_GET_LIB(e);
	r=ERR_GET_REASON(e);

	CRYPTO_r_lock(CRYPTO_LOCK_ERR_HASH);

	if (error_hash != NULL)
		{
		d.error=ERR_PACK(l,0,r);
		p=(ERR_STRING_DATA *)lh_retrieve(error_hash,(char *)&d);
		if (p == NULL)
			{
			d.error=ERR_PACK(0,0,r);
			p=(ERR_STRING_DATA *)lh_retrieve(error_hash,
				(char *)&d);
			}
		}

	CRYPTO_r_unlock(CRYPTO_LOCK_ERR_HASH);

	return((p == NULL)?NULL:p->string);
	}

#ifndef WIN16
void ERR_print_errors_fp(fp)
FILE *fp;
	{
	unsigned long l;
	char buf[200];
	char *file;
	int line;

	while ((l=ERR_get_error_line(&file,&line)) != 0)
		fprintf(fp,"%s:%s:%d\n",ERR_error_string(l,buf),file,line);
	}
#endif

void ERR_print_errors(bp)
BIO *bp;
	{
	unsigned long l;
	char buf[256];
	char buf2[256];
	char *file;
	int line;

	while ((l=ERR_get_error_line(&file,&line)) != 0)
		{
		sprintf(buf2,"%s:%s:%d\n",ERR_error_string(l,buf),file,line);
		BIO_puts(bp,buf2);
		}
	}

static unsigned long err_hash(a)
ERR_STRING_DATA *a;
	{
	unsigned long ret,l;

	l=a->error;
	ret=l^ERR_GET_LIB(l)^ERR_GET_FUNC(l);
	return(ret^ret%19*13);
	}

static int err_cmp(a,b)
ERR_STRING_DATA *a,*b;
	{
	return((int)(a->error-b->error));
	}

