/* crypto/asn1/x_x509.c */
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
#include "asn1_mac.h"

/*
 * ASN1err(ASN1_F_D2I_X509,ASN1_R_LENGTH_MISMATCH);
 * ASN1err(ASN1_F_X509_NEW,ASN1_R_BAD_GET_OBJECT);
 */

static ASN1_METHOD meth={
	(int (*)())  i2d_X509,
	(char *(*)())d2i_X509,
	(char *(*)())X509_new,
	(void (*)()) X509_free};

ASN1_METHOD *ASN1_X509_meth()
	{
	return(&meth);
	}

int i2d_X509(a,pp)
X509 *a;
unsigned char **pp;
	{
	M_ASN1_I2D_vars(a);

	M_ASN1_I2D_len(a->cert_info,	i2d_X509_CINF);
	M_ASN1_I2D_len(a->sig_alg,	i2d_X509_ALGOR);
	M_ASN1_I2D_len(a->signature,	i2d_ASN1_BIT_STRING);

	M_ASN1_I2D_seq_total();

	M_ASN1_I2D_put(a->cert_info,	i2d_X509_CINF);
	M_ASN1_I2D_put(a->sig_alg,	i2d_X509_ALGOR);
	M_ASN1_I2D_put(a->signature,	i2d_ASN1_BIT_STRING);

	M_ASN1_I2D_finish();
	}

X509 *d2i_X509(a,pp,length)
X509 **a;
unsigned char **pp;
long length;
	{
	M_ASN1_D2I_vars(a,X509 *,X509_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(ret->cert_info,d2i_X509_CINF);
	M_ASN1_D2I_get(ret->sig_alg,d2i_X509_ALGOR);
	M_ASN1_D2I_get(ret->signature,d2i_ASN1_BIT_STRING);
	M_ASN1_D2I_Finish(a,X509_free,ASN1_F_D2I_X509);
	}

X509 *X509_new()
	{
	X509 *ret=NULL;

	M_ASN1_New_Malloc(ret,X509);
	ret->references=1;
	M_ASN1_New(ret->cert_info,X509_CINF_new);
	M_ASN1_New(ret->sig_alg,X509_ALGOR_new);
	M_ASN1_New(ret->signature,ASN1_BIT_STRING_new);
	return(ret);
	M_ASN1_New_Error(ASN1_F_X509_NEW);
	}

void X509_free(a)
X509 *a;
	{
	if (a == NULL) return;

	CRYPTO_w_lock(CRYPTO_LOCK_X509);
	if (--a->references > 0)
		{
		CRYPTO_w_unlock(CRYPTO_LOCK_X509);
		return;
		}
	CRYPTO_w_unlock(CRYPTO_LOCK_X509);

	X509_CINF_free(a->cert_info);
	X509_ALGOR_free(a->sig_alg);
	ASN1_BIT_STRING_free(a->signature);
	free(a);
	}

