/* crypto/meth/x509meth.c */
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
#include "stack.h"
#include "lhash.h"
#include "meth.h"
#include "x509.h"

typedef struct x_state_st
	{
	LHASH *CA_by_name_cache;
	} X_STATE;

#ifndef NOPROTO
static int x509_method_free(METHOD_CTX *ctx,int type,char *null,char **pnull);
static int x509_method_init(METHOD_CTX *ctx,int type,char *null,char **pnull);
static int x509_method_by_alias(METHOD_CTX *ctx,int type,char *arg,STACK **ret);
static int x509_method_CA_by_subject(METHOD_CTX *ctx,
		int type,char *arg,STACK **ret);
static int x509_method_by_subject(METHOD_CTX *ctx,
		int type,char *arg,STACK **ret);
static int x509_method_by_issuer_and_serial(METHOD_CTX *ctx,
		int type,char *arg,STACK **ret);
static int x509_method_by_issuer(METHOD_CTX *ctx,
		int type,char *arg,STACK **ret);
#else
static int x509_method_free();
static int x509_method_init();
static int x509_method_by_alias();
static int x509_method_CA_by_subject();
static int x509_method_by_subject();
static int x509_method_by_issuer_and_serial();
static int x509_method_by_issuer();
#endif

static METHOD x509_lookup_static={
	"X509 certificate lookup",
	6,
		{
		x509_method_free,
		x509_method_init,
		x509_method_by_alias,
		x509_method_CA_by_subject, /* CA by subject */
		x509_method_by_subject,
		x509_method_by_issuer_and_serial,
		x509_method_by_issuer,
		},
	};

METHOD *x509_lookup()
	{
	return(&x509_lookup_static);
	}

/* arguments are methods */

static int x509_method_free(ctx,type,null,pnull)
METHOD_CTX *ctx;
int type;
char *null;
char **pnull;
	{
	while (sk_num(ctx->args))
		{
		METH_free((METHOD_CTX *)sk_pop(ctx->args));
		}
	return(1);
	}

static int x509_method_init(ctx,type,null,pnull)
METHOD_CTX *ctx;
int type;
char *null;
char **pnull;
	{
	int i;
	int j;
	METHOD_CTX *m;
	X_STATE *s;

	s=(X_STATE *)malloc(sizeof(X_STATE));
	if (s == NULL)
		{
		METHerr(METH_F_X509_METHOD_INIT,ERR_R_MALLOC_FAILURE);
		return(-1);
		}

	s->CA_by_name_cache=
		lh_new(X509_subject_name_hash,X509_subject_name_cmp);
	if (s->CA_by_name_cache == NULL)
		{
		free(s);
		METHerr(METH_F_X509_METHOD_INIT,ERR_R_MALLOC_FAILURE);
		return(-1);
		}

	for (i=0; i<sk_num(ctx->args); i++)
		{
		m=(METHOD_CTX *)sk_value(ctx->args,i);
		j=METH_init(m);
		if (j == 0)
			m->flags&=(~METH_FLAG_ACTIVE);
		else if (j < 0)
			{
			ctx->error=i;
			return(0);
			}
		}
	return(1);
	}

static int x509_method_by_alias(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
	return(0);
	}

static int x509_method_CA_by_subject(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
	int i,j;
	METHOD_CTX *m;

	for (i=sk_num(ctx->args); i>=0; i--)
		{
		m=(METHOD_CTX *)sk_value(ctx->args,i);
		if (m->flags & METH_FLAG_ACTIVE)
			{
			j=METH_get(m,METH_X509_CA_BY_SUBJECT,arg,ret);
			if (j > 0) return(1);
			if (j < 0) return(-1);
			}
		}
	return(0);
	}

static int x509_method_by_subject(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
	int i,j;
	METHOD_CTX *m;

	for (i=sk_num(ctx->args); i>=0; i--)
		{
		m=(METHOD_CTX *)sk_value(ctx->args,i);
		if (m->flags & METH_FLAG_ACTIVE)
			{
			j=METH_get(m,METH_X509_BY_SUBJECT,arg,ret);
			if (j > 0) return(1);
			if (j < 0) return(-1);
			}
		}
	return(0);
	}

static int x509_method_by_issuer(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
	/* should do this one for SSLtelnet */
	return(0);
	}

static int x509_method_by_issuer_and_serial(ctx,type,arg,ret)
METHOD_CTX *ctx;
int type;
char *arg;
STACK **ret;
	{
	/* signature verification for pkcs#7 */
	return(0);
	}
