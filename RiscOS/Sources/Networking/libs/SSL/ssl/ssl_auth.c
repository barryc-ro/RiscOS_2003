/* ssl/ssl_auth.c */
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
#include "lhash.h"
#include "ssl_locl.h"

#ifndef NOPROTO
static unsigned long conn_hash(SSL_SESSION *a);
#else
static unsigned long conn_hash();
#endif

static unsigned long conn_hash(a)
SSL_SESSION *a;
	{
	unsigned long l;

	l=      (a->session_id[0]     )|(a->session_id[1]<< 8L)|
		(a->session_id[1]<<16L)|(a->session_id[2]<<24L);
	return(l);
	}

static int session_cmp(a, b)
SSL_SESSION *a;
SSL_SESSION *b;
	{
	int i;

	i=a->session_id_length - b->session_id_length;
	if (i == 0)
		return(memcmp(a->session_id,b->session_id,
			a->session_id_length));
	else    return(1);
	}

int SSL_CTX_set_cipher_list(ctx,str)
SSL_CTX *ctx;
char *str;
	{
        return(ssl_make_cipher_list(&ctx->cipher_list,
		&ctx->num_cipher_list,str));
	}

SSL_CTX *SSL_CTX_new()
	{
	SSL_CTX *ret;
	
	ret=(SSL_CTX *)malloc(sizeof(SSL_CTX));
	if (ret == NULL) goto err;

	ret->num_cipher_list=0;
	ret->cipher_list=NULL;

	ret->cert=NULL;
	ret->sessions=lh_new(conn_hash,session_cmp);
	if (ret->sessions == NULL) goto err;

	ret->cert=CERTIFICATE_CTX_new();
	if (ret->cert == NULL) goto err;

	ret->new_session_cb=NULL;
	ret->get_session_cb=NULL;

	ret->sess_connect=0;
	ret->sess_connect_good=0;
	ret->sess_accept=0;
	ret->sess_accept_good=0;
	ret->sess_miss=0;
	ret->sess_timeout=0;
	ret->sess_hit=0;
	ret->sess_cb_hit=0;

	ret->references=1;

	ret->reverse=0;
	ret->cipher=NULL;
	ret->challenge=NULL;
	ret->master_key=NULL;
	ret->key_arg=NULL;
	ret->conn_id=NULL;
	ret->info_callback=NULL;

	return(ret);
err:
	if (ret != NULL) SSL_CTX_free(ret);
	return(NULL);
	}

void SSL_CTX_free(a)
SSL_CTX *a;
	{
	if (a == NULL) return;

	CRYPTO_w_lock(CRYPTO_LOCK_SSL_CTX);
	if (--a->references > 0)
		{
		CRYPTO_w_unlock(CRYPTO_LOCK_SSL_CTX);
		return;
		}
	CRYPTO_w_unlock(CRYPTO_LOCK_SSL_CTX);

	if (a->sessions != NULL)
		{
		SSL_flush_sessions(a,0);
		lh_free(a->sessions);
		}
	if (a->cert != NULL)
		CERTIFICATE_CTX_free(a->cert);
	if (a->cipher_list != NULL)
		{
		free(a->cipher_list[0]);
		free(a->cipher_list);
		}
	free(a);
	}
