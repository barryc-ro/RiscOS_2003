/* crypto/meth/by_dir.c */
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
#include <sys/types.h>
#include <sys/stat.h>
#include "cryptlib.h"
#include "bn.h"
#include "rsa.h"
#include "envelope.h"
#include "x509.h"
#include "pem.h"
#include "meth_lcl.h"

#ifndef S_IFDIR
#define S_IFDIR         0x4000  /* directory */
#endif

#ifndef NOPROTO
static int by_dir_free(    METHOD_CTX *ctx,int type,char *arg,STACK **ret);
static int by_dir_init(    METHOD_CTX *ctx,int type,char *arg,STACK **ret);
static int by_dir_control( METHOD_CTX *ctx,int type,char *arg,STACK **ret);
static int by_dir_get(     METHOD_CTX *ctx,int type,char *arg,STACK **ret);
#else
static int by_dir_free();
static int by_dir_init();
static int by_dir_control();
static int by_dir_get();
#endif

static METHOD x509_by_dir_static={
	"X509 certificates loaded from a 'hash' directory",
	5,
		{
		by_dir_free,
		by_dir_init,
		NULL,
		by_dir_control,
		by_dir_get,
		},
	};

METHOD *x509_by_dir()
	{
	return(&x509_by_dir_static);
	}

static int by_dir_free(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
	char *dir;
	X509_LOOKUP_DIR *st=NULL;
	char *p;

	if (ctx->state != NULL)
		{
		st=(X509_LOOKUP_DIR *)ctx->state;
		for (;;)
			{
			dir=(char *)sk_pop(st->data);
			if (dir == NULL) break;
			free(dir);
			}
		sk_free(st->data);
		}
	free((char *)st);
	if (ctx->arg_type != NULL)
		sk_free(ctx->arg_type);
	if (ctx->args != NULL)
		{
		for (;;)
			{
			p=sk_pop(ctx->args);
			if (p == NULL) break;
			free(p);
			}
		sk_free(ctx->args);
		}
	ctx->arg_type=NULL;
	ctx->args=NULL;
	return(1);
	}

static int by_dir_init(ctx,type,arg,r)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **r;
	{
	FILE *in=NULL;
	STACK *sk;
	char *dir=NULL,*d;
	X509_LOOKUP_DIR *st=NULL;
	int i;
	int t,ret=0;

	if ((sk=sk_new_null()) == NULL) goto err;

	st=(X509_LOOKUP_DIR *)malloc(sizeof(X509_LOOKUP_DIR));
	ctx->state=(char *)st;
	if (st == NULL) goto err;
	st->data=sk;
	st->callback=NULL;

	for (i=0; i<sk_num(ctx->args); i++)
		{
		/* Stop VC-16 worrying about cast of 32bit ptr to int */
		t=(int)(long)sk_value(ctx->arg_type,i);
		if (t == METH_TYPE_CALLBACK)
			{
			st->callback=(int (*)())sk_value(ctx->args,i);
			}
		else if (t == METH_TYPE_DIR)
			{
#ifndef RISCOS
			struct stat sb;
#endif

			dir=sk_value(ctx->args,i);
#ifdef RISCOS
                        if ( !riscos_DirExists(dir) )
                            continue;
#else
			if (stat(dir,&sb) < 0) continue;
			if (!(sb.st_mode & S_IFDIR)) continue;
#endif
			/* ok, seems to be a directory, lets keep it */
			d=(char *)malloc(strlen(dir)+1);
			if (d == NULL) goto err;
			strcpy(d,dir);
			if (!sk_push(sk,d)) goto err;
			}
		}
	ret=1;
err:
	if (in != NULL) fclose(in);
	if (!ret)
		{
		if (sk != NULL)
			for (i=0; i<sk_num(sk); i++)
				free((X509_INFO *)sk_value(sk,i));
		}
	return(ret);
	}

static int by_dir_control(ctx,t,arg,ret)
METHOD_CTX *ctx;
int t;
char *arg;
STACK **ret;
	{
	FILE *fp;
	int i,j,type;
	X509_LOOKUP_DIR *st;
	char *dir;

	type=METH_TYPE(t);
	if (type == METH_CONTROL_DUMP)
		{
		st=(X509_LOOKUP_DIR *)ctx->state;
		fp=(FILE *)arg;
		fprintf(fp,"DUMP of %s\n",ctx->method->name);
		j=sk_num(st->data);
		for (i=0; i<j; i++)
			{
			dir=(char *)sk_value(st->data,i);
			fprintf(fp,"LOOKUP DIR %d of %d:%s\n",i+1,j,dir);
			}
		}
	return(0);
	}

static int by_dir_get(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
#undef BUFSIZE
#define BUFSIZE		1024
	char *pf1,*pf2,*p;
	STACK *sk;
	X509_LOOKUP_DIR *st;
	int i;
	int j;
	unsigned long hash;
	char buf[BUFSIZE];

	if (ret == NULL) return(0);
	if (*ret == NULL)
		{
		sk=sk_new_null();
		*ret=sk;
		}
	else
		sk= *ret;

	st=(X509_LOOKUP_DIR *)ctx->state;

	switch (type)
		{
	case METH_X509_BY_ALIAS:
		/* unsuported */
		return(0);
		/* break;*/
	case METH_X509_CA_BY_SUBJECT:
	case METH_X509_BY_SUBJECT:
		hash=X509_subject_name_hash((X509 *)arg);
		pf1=METH_POSTFIX_BY_SUBJECT;
		pf2=METH_POSTFIX_BY_SUBJECT_CRL;
		break;
	case METH_X509_BY_ISSUER_AND_SERIAL:
		hash=X509_issuer_and_serial_hash((X509 *)arg);
		pf1=METH_POSTFIX_BY_ISSUER_AND_SERIAL;
		pf2=NULL;
		break;
	case METH_X509_BY_ISSUER:
		hash=X509_issuer_name_hash((X509 *)arg);
		pf1=METH_POSTFIX_BY_ISSUER;
		pf2=METH_POSTFIX_BY_ISSUER_RSA;
		break;
	default:
		return(0);
		/* break;*/
		}
	for (i=0; i<sk_num(st->data); i++)
		{
		p=sk_value(st->data,i);
		j=strlen(p);
		if ((j+1+4+8) > BUFSIZE) continue;
		sprintf(buf,"%s/%08lx.%s",p,hash,pf1);
/* Not actually using this stuff right now anyway */
#ifndef WIN16
		fprintf(stderr,"%s\n",buf);

		if (pf2 != NULL)
			{
			sprintf(buf,"%s/%08lx.%s",p,hash,pf2);
			fprintf(stderr,"%s\n",buf);
			}
#endif
		}

	return(0);
	}
