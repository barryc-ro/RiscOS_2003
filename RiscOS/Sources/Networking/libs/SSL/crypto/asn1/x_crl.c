/* crypto/asn1/x_crl.c */
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
#include "x509.h"

/*
 * ASN1err(ASN1_F_D2I_X509_CRL,ASN1_R_LENGTH_MISMATCH);
 * ASN1err(ASN1_F_D2I_X509_CRL_INFO,ASN1_R_EXPECTING_A_SEQUENCE);
 * ASN1err(ASN1_F_D2I_X509_REVOKED,ASN1_R_LENGTH_MISMATCH);
 * ASN1err(ASN1_F_X509_CRL_NEW,ASN1_R_LENGTH_MISMATCH);
 * ASN1err(ASN1_F_X509_CRL_INFO_NEW,ASN1_R_EXPECTING_A_SEQUENCE);
 * ASN1err(ASN1_F_X509_REVOKED_NEW,ASN1_R_LENGTH_MISMATCH);
 */

#ifndef NOPROTO
static int X509_REVOKED_cmp(X509_REVOKED **a,X509_REVOKED **b);
static int X509_REVOKED_seq_cmp(X509_REVOKED **a,X509_REVOKED **b);
#else
static int X509_REVOKED_cmp();
static int X509_REVOKED_seq_cmp();
#endif

int i2d_X509_REVOKED(a,pp)
X509_REVOKED *a;
unsigned char **pp;
	{
	M_ASN1_I2D_vars(a);

	M_ASN1_I2D_len(a->serialNumber,i2d_ASN1_INTEGER);
	M_ASN1_I2D_len(a->revocationDate,i2d_ASN1_UTCTIME);

	M_ASN1_I2D_seq_total();

	M_ASN1_I2D_put(a->serialNumber,i2d_ASN1_INTEGER);
	M_ASN1_I2D_put(a->revocationDate,i2d_ASN1_UTCTIME);

	M_ASN1_I2D_finish();
	}

X509_REVOKED *d2i_X509_REVOKED(a,pp,length)
X509_REVOKED **a;
unsigned char **pp;
long length;
	{
	M_ASN1_D2I_vars(a,X509_REVOKED *,X509_REVOKED_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(ret->serialNumber,d2i_ASN1_INTEGER);
	M_ASN1_D2I_get(ret->revocationDate,d2i_ASN1_UTCTIME);
	M_ASN1_D2I_Finish(a,X509_REVOKED_free,ASN1_F_D2I_X509_REVOKED);
	}

int i2d_X509_CRL_INFO(a,pp)
X509_CRL_INFO *a;
unsigned char **pp;
	{
	M_ASN1_I2D_vars(a);

	if (sk_num(a->revoked) != 0)
		qsort((char *)a->revoked->data,sk_num(a->revoked),
			sizeof(X509_REVOKED *),(int (*)(P_CC_CC))X509_REVOKED_seq_cmp);
	M_ASN1_I2D_len(a->sig_alg,i2d_X509_ALGOR);
	M_ASN1_I2D_len(a->issuer,i2d_X509_NAME);
	M_ASN1_I2D_len(a->lastUpdate,i2d_ASN1_UTCTIME);
	M_ASN1_I2D_len(a->nextUpdate,i2d_ASN1_UTCTIME);
	M_ASN1_I2D_len_SEQ_opt(a->revoked,i2d_X509_REVOKED);

	M_ASN1_I2D_seq_total();

	M_ASN1_I2D_put(a->sig_alg,i2d_X509_ALGOR);
	M_ASN1_I2D_put(a->issuer,i2d_X509_NAME);
	M_ASN1_I2D_put(a->lastUpdate,i2d_ASN1_UTCTIME);
	M_ASN1_I2D_put(a->nextUpdate,i2d_ASN1_UTCTIME);
	M_ASN1_I2D_put_SEQ_opt(a->revoked,i2d_X509_REVOKED);

	M_ASN1_I2D_finish();
	}

X509_CRL_INFO *d2i_X509_CRL_INFO(a,pp,length)
X509_CRL_INFO **a;
unsigned char **pp;
long length;
	{
	int i;
	M_ASN1_D2I_vars(a,X509_CRL_INFO *,X509_CRL_INFO_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(ret->sig_alg,d2i_X509_ALGOR);
	M_ASN1_D2I_get(ret->issuer,d2i_X509_NAME);
	M_ASN1_D2I_get(ret->lastUpdate,d2i_ASN1_UTCTIME);
	M_ASN1_D2I_get(ret->nextUpdate,d2i_ASN1_UTCTIME);
	M_ASN1_D2I_get_seq_opt(ret->revoked,d2i_X509_REVOKED);

	if (ret->revoked != NULL)
		{
		for (i=0; i<sk_num(ret->revoked); i++)
			{
			((X509_REVOKED *)sk_value(ret->revoked,i))->sequence=i;
			}
		}

	M_ASN1_D2I_Finish(a,X509_CRL_INFO_free,ASN1_F_D2I_X509_CRL_INFO);
	}

int i2d_X509_CRL(a,pp)
X509_CRL *a;
unsigned char **pp;
	{
	M_ASN1_I2D_vars(a);

	M_ASN1_I2D_len(a->crl,i2d_X509_CRL_INFO);
	M_ASN1_I2D_len(a->sig_alg,i2d_X509_ALGOR);
	M_ASN1_I2D_len(a->signature,i2d_ASN1_BIT_STRING);

	M_ASN1_I2D_seq_total();

	M_ASN1_I2D_put(a->crl,i2d_X509_CRL_INFO);
	M_ASN1_I2D_put(a->sig_alg,i2d_X509_ALGOR);
	M_ASN1_I2D_put(a->signature,i2d_ASN1_BIT_STRING);

	M_ASN1_I2D_finish();
	}

X509_CRL *d2i_X509_CRL(a,pp,length)
X509_CRL **a;
unsigned char **pp;
long length;
	{
	M_ASN1_D2I_vars(a,X509_CRL *,X509_CRL_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(ret->crl,d2i_X509_CRL_INFO);
	M_ASN1_D2I_get(ret->sig_alg,d2i_X509_ALGOR);
	M_ASN1_D2I_get(ret->signature,d2i_ASN1_BIT_STRING);

	M_ASN1_D2I_Finish(a,X509_CRL_free,ASN1_F_D2I_X509_CRL);
	}


X509_REVOKED *X509_REVOKED_new()
	{
	X509_REVOKED *ret=NULL;

	M_ASN1_New_Malloc(ret,X509_REVOKED);
	M_ASN1_New(ret->serialNumber,ASN1_INTEGER_new);
	M_ASN1_New(ret->revocationDate,ASN1_UTCTIME_new);
	return(ret);
	M_ASN1_New_Error(ASN1_F_X509_REVOKED_NEW);
	}

X509_CRL_INFO *X509_CRL_INFO_new()
	{
	X509_CRL_INFO *ret=NULL;

	M_ASN1_New_Malloc(ret,X509_CRL_INFO);
	M_ASN1_New(ret->sig_alg,X509_ALGOR_new);
	M_ASN1_New(ret->issuer,X509_NAME_new);
	M_ASN1_New(ret->lastUpdate,ASN1_UTCTIME_new);
	M_ASN1_New(ret->nextUpdate,ASN1_UTCTIME_new);
	M_ASN1_New(ret->revoked,sk_new_null);
	ret->revoked->comp=(int (*)())X509_REVOKED_cmp;
	return(ret);
	M_ASN1_New_Error(ASN1_F_X509_CRL_INFO_NEW);
	}

X509_CRL *X509_CRL_new()
	{
	X509_CRL *ret=NULL;

	M_ASN1_New_Malloc(ret,X509_CRL);
	ret->references=1;
	M_ASN1_New(ret->crl,X509_CRL_INFO_new);
	M_ASN1_New(ret->sig_alg,X509_ALGOR_new);
	M_ASN1_New(ret->signature,ASN1_BIT_STRING_new);
	return(ret);
	M_ASN1_New_Error(ASN1_F_X509_CRL_NEW);
	}

void X509_REVOKED_free(a)
X509_REVOKED *a;
	{
	if (a == NULL) return;
	ASN1_INTEGER_free(a->serialNumber);
	ASN1_UTCTIME_free(a->revocationDate);
	free(a);
	}

void X509_CRL_INFO_free(a)
X509_CRL_INFO *a;
	{
	if (a == NULL) return;
	X509_ALGOR_free(a->sig_alg);
	X509_NAME_free(a->issuer);
	ASN1_UTCTIME_free(a->lastUpdate);
	ASN1_UTCTIME_free(a->nextUpdate);
	sk_pop_free(a->revoked,X509_REVOKED_free);
	free(a);
	}

void X509_CRL_free(a)
X509_CRL *a;
	{
	if (a == NULL) return;

	CRYPTO_w_lock(CRYPTO_LOCK_X509_CRL);
	if (--a->references > 0)
		{
		CRYPTO_w_unlock(CRYPTO_LOCK_X509_CRL);
		return;
		}
	CRYPTO_w_unlock(CRYPTO_LOCK_X509_CRL);

	X509_CRL_INFO_free(a->crl);
	X509_ALGOR_free(a->sig_alg);
	ASN1_BIT_STRING_free(a->signature);
	free(a);
	}

static int X509_REVOKED_cmp(a,b)
X509_REVOKED **a,**b;
	{
	return(ASN1_BIT_STRING_cmp((*a)->serialNumber,(*b)->serialNumber));
	}

static int X509_REVOKED_seq_cmp(a,b)
X509_REVOKED **a,**b;
	{
	return((*a)->sequence-(*b)->sequence);
	}
