/* crypto/meth/x_k_f.c */
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
#include <unistd.h>
#include <errno.h>
#include "stack.h"
#include "lhash.h"
#include "x509.h"
#include "cert.h"
#include "pem.h"

#ifndef NOPROTO
static int init(CERT_METHOD_CTX *m);
static int shutdown(CERT_METHOD_CTX *m);
static int control(CERT_METHOD_CTX *m,int cmd, char *data);
static int doit(CERT_METHOD_CTX *m,CERT_RET *ret,CERT_ARG *arg);
#else
static int init();
static int shutdown();
static int control();
static int doit();
#endif

typedef item;
:q
CERT_METHOD x509_cert_and_key_file=
	{
	CERT_TYPE_X509,
	CERT_METHOD_BY_SUBJECT,
	METHOD_FLAG_ACTIVE,
	init,
	NULL,
	NULL,
	doit,
	NULL,
	NULL,
	NULL
	};

static int init(m)
CERT_METHOD_CTX *m;
	{
	int i;
	FILE *in;
	X509 *x=NULL;
	RSA *key;
	BUF_MEM *buff=NULL;

	if (sk_num(m->args) == 0)
		{
		/* make it inactive */
		m->flags&= ~METHOD_FLAG_ACTIVE;
		return(1);
		}

	x=X509_new();
	rsa=RSA_new();
	buff=BUF_MEM_new();
	if ((x == NULL) || (rsa == NULL) || (buff == NULL))
		{
		/* ERROR */
		goto err;
		}
	for (i=0; i<sk_num(m->args); i++)
		{
		in=fopen(file,sk_value(m->args,i),"r");
		if (in == NULL)
			{
			SYSerr(ERR_F_FOPEN,errno);
			return(0);
			}
		type=PEM_read_next(in,buff);
		if ((type == PEM_OBJ_X509) && (x == NULL))
		}	

	return(1);
err:
	if (x != NULL) X509_free(x);
	if (rsa != NULL) RSA_free(rsa);
	if (buff != NULL) BUF_MEM_free(buff);
	return(0);
	}

static int shutdown(m)
CERT_METHOD_CTX *m;
	{
	return(1);
	}

static int control(m,cmd,data)
CERT_METHOD_CTX *m
int cmd;
char *data;
	{
	return(1);
	}

static int doit(m,ret,arg)
CERT_METHOD_CTX *m;
CERT_RET *ret;
CERT_ARG *arg;
	{
	}
