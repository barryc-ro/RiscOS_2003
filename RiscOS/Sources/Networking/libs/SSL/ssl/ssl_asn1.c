/* ssl/ssl_asn1.c */
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
#include <stdlib.h>
#include "asn1_mac.h"
#include "objects.h"
#include "ssl_locl.h"

typedef struct ssl_session_asn1_st
	{
	ASN1_INTEGER version;
	ASN1_INTEGER ssl_version;
	ASN1_OCTET_STRING cipher;
	ASN1_OCTET_STRING master_key;
	ASN1_OCTET_STRING session_id;
	ASN1_OCTET_STRING key_arg;
	} SSL_SESSION_ASN1;

/*
 * ASN1err(SSL_F_I2D_SSL_SESSION,SSL_R_CIPHER_CODE_TOO_LONG);
 * ASN1err(SSL_F_D2I_SSL_SESSION,SSL_R_UNSUPORTED_CIPHER);
 */

int i2d_SSL_SESSION(in,pp)
SSL_SESSION *in;
unsigned char **pp;
	{
#define LSIZE2 (sizeof(long)*2)
	unsigned char buf[4],ibuf1[LSIZE2],ibuf2[LSIZE2];
	SSL_SESSION_ASN1 a;
	M_ASN1_I2D_vars(in);

	if ((in == NULL) || (in->cipher == NULL))
		return(0);

	/* Note that I cheat in the following 2 assignments.  I know
	 * that if the ASN1_INTERGER passed to ASN1_INTEGER_set
	 * is > sizeof(long)+1, the buffer will not be re-malloc()ed.
	 * This is a bit evil but makes things simple, no dynamic allocation
	 * to clean up :-) */
	a.version.length=LSIZE2;
	a.version.type=V_ASN1_INTEGER;
	a.version.data=ibuf1;
	ASN1_INTEGER_set(&(a.version),SSL_SESSION_ASN1_VERSION);

	a.ssl_version.length=LSIZE2;
	a.ssl_version.type=V_ASN1_INTEGER;
	a.ssl_version.data=ibuf2;
	ASN1_INTEGER_set(&(a.ssl_version),SSL_SERVER_VERSION);

	a.cipher.length=3;
	a.cipher.type=V_ASN1_OCTET_STRING;
	a.cipher.data=buf;
	buf[0]=in->cipher->c1;
	buf[1]=in->cipher->c2;
	buf[2]=in->cipher->c3;

	a.master_key.length=in->master_key_length;
	a.master_key.type=V_ASN1_OCTET_STRING;
	a.master_key.data=in->master_key;

	a.session_id.length=in->session_id_length;
	a.session_id.type=V_ASN1_OCTET_STRING;
	a.session_id.data=in->session_id;

	a.key_arg.length=in->key_arg_length;
	a.key_arg.type=V_ASN1_OCTET_STRING;
	a.key_arg.data=in->key_arg;

	M_ASN1_I2D_len(&(a.version),		i2d_ASN1_INTEGER);
	M_ASN1_I2D_len(&(a.ssl_version),	i2d_ASN1_INTEGER);
	M_ASN1_I2D_len(&(a.cipher),		i2d_ASN1_OCTET_STRING);
	M_ASN1_I2D_len(&(a.session_id),		i2d_ASN1_OCTET_STRING);
	M_ASN1_I2D_len(&(a.master_key),		i2d_ASN1_OCTET_STRING);
	if (in->key_arg_length > 0)
		M_ASN1_I2D_len_IMP_opt(&(a.key_arg),i2d_ASN1_OCTET_STRING);

	M_ASN1_I2D_seq_total();

	M_ASN1_I2D_put(&(a.version),		i2d_ASN1_INTEGER);
	M_ASN1_I2D_put(&(a.ssl_version),	i2d_ASN1_INTEGER);
	M_ASN1_I2D_put(&(a.cipher),		i2d_ASN1_OCTET_STRING);
	M_ASN1_I2D_put(&(a.session_id),		i2d_ASN1_OCTET_STRING);
	M_ASN1_I2D_put(&(a.master_key),		i2d_ASN1_OCTET_STRING);
	if (in->key_arg_length > 0)
		M_ASN1_I2D_put_IMP_opt(&(a.key_arg),i2d_ASN1_OCTET_STRING,0);

	M_ASN1_I2D_finish();
	}

SSL_SESSION *d2i_SSL_SESSION(a,pp,length)
SSL_SESSION **a;
unsigned char **pp;
long length;
	{
	CIPHER cipher,*cp;
	int version,ssl_version=0,i;
	ASN1_INTEGER ai,*aip;
	ASN1_OCTET_STRING os,*osp;
	M_ASN1_D2I_vars(a,SSL_SESSION *,SSL_SESSION_new);

	aip= &ai;
	osp= &os;

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();

	ai.data=NULL; ai.length=0;
	M_ASN1_D2I_get(aip,d2i_ASN1_INTEGER);
	version=0;
	for (i=0; i<ai.length; i++)
		version=version*256+ai.data[i];

	/* we don't care about the version right now :-) */
	M_ASN1_D2I_get(aip,d2i_ASN1_INTEGER);
	for (i=0; i<ai.length; i++)
		ssl_version=ssl_version*256+ai.data[i];

	os.data=NULL; os.length=0;
	M_ASN1_D2I_get(osp,d2i_ASN1_OCTET_STRING);
	if (os.length != 3)
		{
		c.error=SSL_R_CIPHER_CODE_TOO_LONG;
		goto err;
		}
	cipher.c1=os.data[0];
	cipher.c2=os.data[1];
	cipher.c3=os.data[2];
	cp=cp=(CIPHER *)OBJ_bsearch((char *)&cipher,(char *)ssl_ciphers,
                        NUM_CIPHERS,sizeof(CIPHER),(int (*)())ssl_cipher_cmp);
	if (cp == NULL)
		{
		free(os.data);
		c.error=SSL_R_UNSUPORTED_CIPHER;
		goto err;
		}
	else
		{
		ret->num_ciphers=1;
		ret->ciphers[0]=cp;
		ret->cipher=cp;
		}

	M_ASN1_D2I_get(osp,d2i_ASN1_OCTET_STRING);
	ret->session_id_length=os.length;
	ret->session_id=os.data;
	os.data=NULL; os.length=0;

	M_ASN1_D2I_get(osp,d2i_ASN1_OCTET_STRING);
	ret->master_key_length=os.length;
	ret->master_key=os.data;
	os.data=NULL; os.length=0;

	M_ASN1_D2I_get_IMP_opt(osp,d2i_ASN1_OCTET_STRING,0,V_ASN1_OCTET_STRING);
	ret->key_arg_length=os.length;
	ret->key_arg=os.data;
	os.data=NULL; os.length=0;

	ret->timeout=100;
	ret->time=time(NULL);

	M_ASN1_D2I_Finish(a,SSL_SESSION_free,SSL_F_D2I_SSL_SESSION);
	}

