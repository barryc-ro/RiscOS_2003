/* crypto/meth/by_file.c */
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
#include "cryptlib.h"
#include "bn.h"
#include "rsa.h"
#include "objects.h"
#include "envelope.h"
#include "x509.h"
#include "pem.h"
#include "meth_lcl.h"

typedef struct state_st
	{
	LHASH *CA_by_name_cache;
	} CA_STATE;

#ifndef NOPROTO
static int by_file_free(    METHOD_CTX *ctx,int type,char *arg,STACK **ret);
static int by_file_init(    METHOD_CTX *ctx,int type,char *arg,STACK **ret);
static int by_file_control( METHOD_CTX *ctx,int type,char *arg,STACK **ret);
static int by_file_get(     METHOD_CTX *ctx,int type,char *arg,STACK **ret);
#else
static int by_file_free();
static int by_file_init();
static int by_file_control();
static int by_file_get();
#endif

static METHOD x509_by_file_static={
	"X509 certificates loaded by 'hand' or from a file",
	5,
		{
		by_file_free,
		by_file_init,
		NULL,
		by_file_control,
		by_file_get,
		},
	};

METHOD *x509_by_file()
	{
	return(&x509_by_file_static);
	}

static int by_file_free(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
	X509_INFO *xi=NULL;
	X509_LOOKUP_FILE *st=NULL;
	char *p;
	int i;

	if (ctx->state != NULL)
		{
		st=(X509_LOOKUP_FILE *)ctx->state;
		for (;;)
			{
			xi=(X509_INFO *)sk_pop(st->data);
			if (xi == NULL) break;
			X509_INFO_free(xi);
			}
		sk_free(st->data);
		}
	for (i=0; i<METH_X509_NUMBER; i++)
		{
		if (st->index[i] != NULL)
			lh_free(st->index[i]);
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

/* needs to be reworked */
static int by_file_init(ctx,type,arg,r)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **r;
	{
	FILE *in=NULL;
	STACK *sk;
	X509_INFO *xi=NULL;
	X509_LOOKUP_FILE *st=NULL;
	int i;
	int t,ret=0;

	if ((sk=sk_new_null()) == NULL) goto err;

	st=(X509_LOOKUP_FILE *)malloc(sizeof(X509_LOOKUP_FILE));
	ctx->state=(char *)st;
	if (st == NULL) goto err;
	for (i=0; i<METH_X509_NUMBER; i++)
		st->index[i]=NULL;
	st->data=sk;
	st->callback=NULL;

	for (i=0; i<sk_num(ctx->args); i++)
		{
		/* Stop people worrying about loosing pointer details VC-16 */
		t=(int)(long)sk_value(ctx->arg_type,i);
		if (t == METH_TYPE_CALLBACK)
			{
			st->callback=(int (*)())sk_value(ctx->args,i);
			}
		else if (t == METH_TYPE_X509_INF0)
			{
			xi=(X509_INFO *)sk_value(ctx->args,i);

			CRYPTO_w_lock(CRYPTO_LOCK_X509_INFO);
			xi->references++;
			CRYPTO_w_unlock(CRYPTO_LOCK_X509_INFO);

			sk_push(sk,(char *)xi);
			}
		else if (t == METH_TYPE_FILE)
			{
#ifndef WIN16
			in=fopen(sk_value(ctx->args,i),"r");
			if (in == NULL) continue;

			if (PEM_X509_INFO_read(in,sk,
				(int (*)())st->callback) == NULL) goto err;
			fclose(in);
			in=NULL;
#endif
			}
		}
	ret=1;
err:
	if (in != NULL) fclose(in);
	if (!ret)
		{
		if (sk != NULL)
			for (i=0; i<sk_num(sk); i++)
				X509_INFO_free((X509_INFO *)sk_value(sk,i));
		}
	else
		st->data=sk;
	return(ret);
	}

/* This needs to be reworked */
static int by_file_control(ctx,t,arg,ret)
METHOD_CTX *ctx;
int t;
char *arg;
STACK **ret;
	{
	FILE *fp;
	int i,j,type;
	X509_LOOKUP_FILE *st;
	X509_INFO *xi;
	char *s;

	type=METH_TYPE(t);
	if (type == METH_CONTROL_DUMP)
		{
		st=(X509_LOOKUP_FILE *)ctx->state;
		fp=(FILE *)arg;
		fprintf(fp,"DUMP of %s\n",ctx->method->name);
		j=sk_num(st->data);
		for (i=0; i<j; i++)
			{
			xi=(X509_INFO *)sk_value(st->data,i);
			fprintf(fp,"X509_INFO %d of %d\n",i+1,j);
			if (xi->x509 != NULL)
				{
				s=X509_NAME_oneline(xi->x509->cert_info->issuer);
				fprintf(fp,"X509 issuer :%s\n",s);
				free(s);
				fprintf(fp,"X509 serial :");
#ifdef undef
				i2a_ASN1_INTEGER(fp,xi->x509->cert_info->serialNumber);
#endif
				s=X509_NAME_oneline(xi->x509->cert_info->subject);
				fprintf(fp,"X509 subject:%s\n",s);
				free(s);
				}
			if (xi->crl != NULL)
				{
				s=X509_NAME_oneline(xi->crl->crl->issuer);
				fprintf(fp,"CRL issuer  :%s\n",s);
				free(s);
				}
			if (xi->x_pkey != NULL)
				{
				fprintf(fp,"PrivKey:\n");
				}
/*			if (xi->rsa_data != NULL)
				{
				ct=xi->rsa_cipher.cipher->type;
				fprintf(fp,"RSA Encrypted: %s\n",OBJ_nid2ln(ct));
				} */
			fprintf(fp,"\n");
			}
		}
	return(0);
	}

static int by_file_get(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
	return(0);
	}
