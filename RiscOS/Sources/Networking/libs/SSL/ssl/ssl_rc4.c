/* ssl/ssl_rc4.c */
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

#ifndef NO_RC4
#include <stdio.h>
#include "rc4.h"
#include "md5.h"
#include "ssl_locl.h"
#include "ssl_rc4.h"

int ssl_enc_rc4_init(s, client)
SSL *s;
int client;
	{
	unsigned char *md5d1,*md5d2;
	RC4_STATE *rc4s;

	if (s->crypt_state == NULL)
		s->crypt_state=(char *)malloc(sizeof(RC4_STATE));
	if (s->crypt_state == NULL) goto err;
	rc4s=(RC4_STATE *)s->crypt_state;

	if (s->key_material != NULL)
		{
		memset(s->key_material,0,s->key_material_length);
		free(s->key_material);
		}
	md5d1=s->key_material=(unsigned char *)malloc(MD5_DIGEST_LENGTH*2);
	if (s->key_material == NULL) goto err;
	s->key_material_length=MD5_DIGEST_LENGTH*2;
	md5d2=&(s->key_material[MD5_DIGEST_LENGTH]);
	
	ssl_generate_key_material(s);

	if (client)
		{
		s->read_key= md5d1;
		s->write_key=md5d2;
		}
	else
		{
		s->write_key=md5d1;
		s->read_key= md5d2;
		}
	RC4_set_key(&(rc4s->rc4_read_ks),MD5_DIGEST_LENGTH,s->read_key);
	RC4_set_key(&(rc4s->rc4_write_ks),MD5_DIGEST_LENGTH,s->write_key);
	return(1);
err:
	SSLerr(SSL_F_SSL_ENC_RC4_INIT,ERR_R_MALLOC_FAILURE);
	return(0);
	}

/* read/writes from s->mac_data using length for encrypt and decrypt.
 * it sets the s->padding, s->length and s->pad_data ptr if we
 * are encrypting */
void ssl_enc_rc4(s)
SSL *s;
	{
	RC4_STATE *rc4s;
	unsigned long l;

	rc4s=(RC4_STATE *)s->crypt_state;

	l=s->length;

	if (s->send)
		RC4(&(rc4s->rc4_write_ks),l,s->mac_data,s->mac_data);
	else
		RC4(&(rc4s->rc4_read_ks),l,s->mac_data,s->mac_data);
	}
#endif
