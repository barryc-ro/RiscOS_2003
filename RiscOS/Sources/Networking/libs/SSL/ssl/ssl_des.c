/* ssl/ssl_des.c */
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
#include "des.h"
#include "md5.h"
#include "ssl_locl.h"
#include "ssl_des.h"

int ssl_enc_des_cbc_init(s, client)
SSL *s;
int client;
	{
	unsigned char md5d[MD5_DIGEST_LENGTH];
	DES_CBC_STATE *ds;

	if (s->crypt_state == NULL)
		s->crypt_state=(char *)malloc(sizeof(DES_CBC_STATE));
	if (s->crypt_state == NULL) goto err;

	ds=(DES_CBC_STATE *)s->crypt_state;

	if (s->key_material != NULL)
		{
		memset(s->key_material,0,s->key_material_length);
		free(s->key_material);
		}
	s->key_material=(unsigned char *)malloc(MD5_DIGEST_LENGTH);
	if (s->key_material == NULL) goto err;
	s->key_material_length=MD5_DIGEST_LENGTH;

	ssl_generate_key_material(s);

	/* duplicate because we don't want cleared parity in recorded
	 * version */
	memcpy(md5d,s->key_material,MD5_DIGEST_LENGTH);

	des_set_odd_parity((C_Block *)&(md5d[0]));
	des_set_odd_parity((C_Block *)&(md5d[8]));

	if (client)
		{
		des_set_key((C_Block *)&(md5d[0]),ds->read_ks);
		des_set_key((C_Block *)&(md5d[8]),ds->write_ks);
		s->read_key= &(s->key_material[0]);
		s->write_key= &(s->key_material[8]);
		}
	else
		{
		des_set_key((C_Block *)&(md5d[0]),ds->write_ks);
		des_set_key((C_Block *)&(md5d[8]),ds->read_ks);
		s->write_key= &(s->key_material[0]);
		s->read_key= &(s->key_material[8]);
		}
	memcpy(ds->read_iv,s->session->key_arg,DES_KEY_SZ);
	memcpy(ds->write_iv,s->session->key_arg,DES_KEY_SZ);
	memset(md5d,0,MD5_DIGEST_LENGTH);
	return(1);
err:
	SSLerr(SSL_F_SSL_ENC_DES_CBC_INIT,ERR_R_MALLOC_FAILURE);
	return(0);
	}

/* read/writes from s->mac_data using length for encrypt and 
 * decrypt.  It sets the s->padding, s->length and s->pad_data ptr if we
 * are encrypting */
void ssl_enc_des_cbc(s)
SSL *s;
	{
	DES_CBC_STATE *ds;
	unsigned long l;

	ds=(DES_CBC_STATE *)s->crypt_state;

	l=s->length;
	l=(l+7)/8*8;

#ifdef DES_DEBUG
	SSL_TRACE(SSL_ERR,"DES CBC length=%d\n",s->length);
	ssl_print_bytes(SSL_ERR,l,s->mac_data);
	SSL_TRACE(SSL_ERR,"\n");
#endif
	if (s->send)
		{
		des_ncbc_encrypt((des_cblock *)s->mac_data,
			(des_cblock *)s->mac_data,(long)l,ds->write_ks,
			(des_cblock *)ds->write_iv,
			DES_ENCRYPT);
		}
	else
		{
		des_ncbc_encrypt((des_cblock *)s->mac_data,
			(des_cblock *)s->mac_data,(long)l,
			ds->read_ks,(des_cblock *)ds->read_iv,DES_DECRYPT);
		}

#ifdef DES_DEBUG
	ssl_print_bytes(SSL_ERR,l,s->mac_data);
	SSL_TRACE(SSL_ERR,"\n");
#endif
	}

int ssl_enc_des_ede3_cbc_init(s, client)
SSL *s;
int client;
	{
	unsigned char md5d[MD5_DIGEST_LENGTH*3];
	DES_EDE3_CBC_STATE *ds;

	if (s->crypt_state == NULL)
		s->crypt_state=(char *)malloc(sizeof(DES_EDE3_CBC_STATE));
	if (s->crypt_state == NULL) goto err;
	ds=(DES_EDE3_CBC_STATE *)s->crypt_state;

	if (s->key_material != NULL)
		{
		memset(s->key_material,0,s->key_material_length);
		free(s->key_material);
		}
	s->key_material=(unsigned char *)malloc(MD5_DIGEST_LENGTH*3);
	if (s->key_material == NULL) goto err;
	s->key_material_length=MD5_DIGEST_LENGTH*3;

	ssl_generate_key_material(s);

	memcpy(md5d,s->key_material,MD5_DIGEST_LENGTH*3);
	des_set_odd_parity((C_Block *)&(md5d[ 0]));
	des_set_odd_parity((C_Block *)&(md5d[ 8]));
	des_set_odd_parity((C_Block *)&(md5d[16]));
	des_set_odd_parity((C_Block *)&(md5d[24]));
	des_set_odd_parity((C_Block *)&(md5d[32]));
	des_set_odd_parity((C_Block *)&(md5d[40]));

	if (client)
		{
		des_set_key((C_Block *)&(md5d[ 0]),ds->read_ks0);
		des_set_key((C_Block *)&(md5d[ 8]),ds->read_ks1);
		des_set_key((C_Block *)&(md5d[16]),ds->read_ks2);
		des_set_key((C_Block *)&(md5d[24]),ds->write_ks0);
		des_set_key((C_Block *)&(md5d[32]),ds->write_ks1);
		des_set_key((C_Block *)&(md5d[40]),ds->write_ks2);
		s->read_key= &(s->key_material[0]);
		s->write_key= &(s->key_material[24]);
		}
	else
		{
		des_set_key((C_Block *)&(md5d[ 0]),ds->write_ks0);
		des_set_key((C_Block *)&(md5d[ 8]),ds->write_ks1);
		des_set_key((C_Block *)&(md5d[16]),ds->write_ks2);
		des_set_key((C_Block *)&(md5d[24]),ds->read_ks0);
		des_set_key((C_Block *)&(md5d[32]),ds->read_ks1);
		des_set_key((C_Block *)&(md5d[40]),ds->read_ks2);
		s->write_key= &(s->key_material[0]);
		s->read_key= &(s->key_material[24]);
		}
	memcpy(ds->read_iv,s->session->key_arg,DES_KEY_SZ);
	memcpy(ds->write_iv,s->session->key_arg,DES_KEY_SZ);

	memset(md5d,0,MD5_DIGEST_LENGTH*3);
	return(1);
err:
	SSLerr(SSL_F_SSL_ENC_DES_EDE3_CBC_INIT,ERR_R_MALLOC_FAILURE);
	return(0);
	}

/* read/writes from s->mac_data using length for encrypt and 
 * decrypt.  It sets the s->padding, s->length and s->pad_data ptr if we
 * are encrypting */
void ssl_enc_des_ede3_cbc(s)
SSL *s;
	{
	DES_EDE3_CBC_STATE *ds;
	unsigned long l;

	ds=(DES_EDE3_CBC_STATE *)s->crypt_state;

	l=s->length;
	l=(l+7)/8*8;

#ifdef DES_DEBUG
	SSL_TRACE(SSL_ERR,"DES EDE3 length=%d\n",s->length);
	ssl_print_bytes(SSL_ERR,l,s->mac_data);
	SSL_TRACE(SSL_ERR,"\n");
#endif
	if (s->send)
		{
		des_ede3_cbc_encrypt((des_cblock *)s->mac_data,
			(des_cblock *)s->mac_data,(long)l,
			ds->write_ks0,
			ds->write_ks1,
			ds->write_ks2,
			(des_cblock *)ds->write_iv,
			DES_ENCRYPT);
		}
	else
		{
		des_ede3_cbc_encrypt((des_cblock *)s->mac_data,
			(des_cblock *)s->mac_data,(long)l,
			ds->read_ks0,
			ds->read_ks1,
			ds->read_ks2,
			(des_cblock *)ds->read_iv,
			DES_DECRYPT);
		}

#ifdef DES_DEBUG
	ssl_print_bytes(SSL_ERR,l,s->mac_data);
	SSL_TRACE(SSL_ERR,"\n");
#endif
	}

int ssl_enc_des_cfb_init(s, client)
SSL *s;
int client;
	{
	unsigned char md5d[MD5_DIGEST_LENGTH];
	DES_CFB_STATE *ds;

	if (s->crypt_state == NULL)
		s->crypt_state=(char *)malloc(sizeof(DES_CFB_STATE));
	if (s->crypt_state == NULL) goto err;
	ds=(DES_CFB_STATE *)s->crypt_state;

	if (s->key_material != NULL)
		{
		memset(s->key_material,0,s->key_material_length);
		free(s->key_material);
		}
	s->key_material=(unsigned char *)malloc(MD5_DIGEST_LENGTH);
	if (s->key_material == NULL) goto err;
	s->key_material_length=MD5_DIGEST_LENGTH;

	ssl_generate_key_material(s);

	/* duplicate because we don't want cleared parity in recorded
	 * version */
	memcpy(md5d,s->key_material,MD5_DIGEST_LENGTH);

	des_set_odd_parity((C_Block *)&(md5d[0]));
	des_set_odd_parity((C_Block *)&(md5d[8]));

	if (client)
		{
		des_set_key((C_Block *)&(md5d[0]),ds->read_ks);
		des_set_key((C_Block *)&(md5d[8]),ds->write_ks);
		s->read_key= &(s->key_material[0]);
		s->write_key= &(s->key_material[8]);
		}
	else
		{
		des_set_key((C_Block *)&(md5d[0]),ds->write_ks);
		des_set_key((C_Block *)&(md5d[8]),ds->read_ks);
		s->write_key= &(s->key_material[0]);
		s->read_key= &(s->key_material[8]);
		}
	ds->readn=0;
	ds->writen=0;
	memcpy(ds->read_iv,s->session->key_arg,DES_KEY_SZ);
	memcpy(ds->write_iv,s->session->key_arg,DES_KEY_SZ);
	memset(md5d,0,MD5_DIGEST_LENGTH);
	return(1);
err:
	SSLerr(SSL_F_SSL_ENC_DES_CFB_INIT,ERR_R_MALLOC_FAILURE);
	return(0);
	}

/* read/writes from s->mac_data using length for encrypt and 
 * decrypt.  It sets the s->padding, s->length and s->pad_data ptr if we
 * are encrypting */
void ssl_enc_des_cfb(s)
SSL *s;
	{
	DES_CFB_STATE *ds;
	unsigned long l;

	ds=(DES_CFB_STATE *)s->crypt_state;

	l=s->length;

#ifdef DES_DEBUG
	SSL_TRACE(SSL_ERR,"DES CFB length=%d\nn=%d riv=",s->length,
		ds->readn);
	ssl_print_bytes(SSL_ERR,8,ds->read_iv);
	SSL_TRACE(SSL_ERR,"\nn=%d wiv=",ds->writen);
	ssl_print_bytes(SSL_ERR,8,ds->write_iv);
	SSL_TRACE(SSL_ERR,"\n");
	ssl_print_bytes(SSL_ERR,l,s->mac_data);
	SSL_TRACE(SSL_ERR,"\n");
#endif
	if (s->send)
		{
		des_cfb64_encrypt((unsigned char *)s->mac_data,
			(unsigned char *)s->mac_data,(long)l,ds->write_ks,
			(des_cblock *)ds->write_iv,&ds->writen,
			DES_ENCRYPT);
		}
	else
		{
		des_cfb64_encrypt((unsigned char *)s->mac_data,
			(unsigned char *)s->mac_data,(long)l,ds->read_ks,
			(des_cblock *)ds->read_iv,&ds->readn,
			DES_DECRYPT);
		}

#ifdef DES_DEBUG
	ssl_print_bytes(SSL_ERR,l,s->mac_data);
	SSL_TRACE(SSL_ERR,"\n");
#endif
	}
