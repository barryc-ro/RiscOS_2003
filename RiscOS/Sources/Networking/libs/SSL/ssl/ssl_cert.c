/* ssl/ssl_cert.c */
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
#include "objects.h"
#include "ssl_locl.h"
#include "ssl_rsa.h"

CERT *ssl_cert_new()
	{
	CERT *ret;

	ret=(CERT *)malloc(sizeof(CERT));
	if (ret == NULL)
		{
		SSLerr(SSL_F_SSL_CERT_NEW,ERR_R_MALLOC_FAILURE);
		return(NULL);
		}
	ret->cert_type=0;
	ret->public_encrypt=NULL;
	ret->private_decrypt=NULL;
	ret->x509=NULL;
	ret->publickey=NULL;
	ret->privatekey=NULL;
	ret->references=1;
	return(ret);
	}

void ssl_cert_free(c)
CERT *c;
	{
	CRYPTO_w_lock(CRYPTO_LOCK_ssl_CERT);
	if (--c->references > 0)
		{
		CRYPTO_w_unlock(CRYPTO_LOCK_ssl_CERT);
		return;
		}
	CRYPTO_w_unlock(CRYPTO_LOCK_ssl_CERT);

	if (c->x509 != NULL) X509_free(c->x509);
	if (c->privatekey != NULL) EVP_PKEY_free(c->privatekey);
	if (c->publickey != NULL) EVP_PKEY_free(c->publickey);
	free(c);
	}

/* loads in the certificate from the server */
int ssl_set_certificate(s, type, len, data)
SSL *s;
int type;
int len;
unsigned char *data;
	{
	EVP_PKEY *pkey=NULL;
	CERT *c=NULL;
	int i;
	X509 *x509=NULL; /* will don't want to include the headers */
	int ret=0;

        s->session->verified = 0;

	x509=d2i_X509(NULL,&data,(long)len);
	if (x509 == NULL)
		{
		SSLerr(SSL_F_SSL_SET_CERTIFICATE,ERR_R_X509_LIB);
		goto err;
		}

	i=X509_cert_verify(s->ctx->cert,x509,s->verify_callback);
	s->session->verified = i; /* pdh */
	if ((s->verify_mode != SSL_VERIFY_NONE) && (!i))
		{
		SSLerr(SSL_F_SSL_SET_CERTIFICATE,ERR_R_X509_LIB);
		goto err;
		}

	/* cert for ssl */
	c=ssl_cert_new();
	if (c == NULL)
		{
		ret= -1;
		goto err;
		}

	/* cert for session */
	if (s->session->cert) ssl_cert_free(s->session->cert);
	s->session->cert=c;

	c->cert_type=type;
	c->x509=x509;
	pkey=X509_extract_key(x509);
	x509=NULL;
	if (pkey == NULL)
		{
		SSLerr(SSL_F_SSL_SET_CERTIFICATE,SSL_R_UNABLE_TO_EXTRACT_PUBLIC_KEY);
		goto err;
		}
	if (pkey->type != EVP_PKEY_RSA)
		{
		SSLerr(SSL_F_SSL_SET_CERTIFICATE,SSL_R_PUBLIC_KEY_NO_RSA);
		EVP_PKEY_free(pkey);
		goto err;
		}
	if (c->publickey != NULL) EVP_PKEY_free(c->publickey);
	c->publickey=pkey;
	if (!ssl_set_cert_type(c,SSL_CT_X509_CERTIFICATE))
		goto err;
	ret=1;
err:
	if (x509 != NULL) X509_free(x509);
	return(ret);
	}

int ssl_set_cert_type(c, type)
CERT *c;
int type;
	{
	c->cert_type=type;
	c->public_encrypt=ssl_rsa_public_encrypt;
	c->private_decrypt=ssl_rsa_private_decrypt;
	return(1);
	}
