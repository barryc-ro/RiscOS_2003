/* crypto/asn1/d2i_pu.c */
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
#include "envelope.h"
#include "objects.h"
#include "x509.h"

EVP_PKEY *d2i_PublicKey(type,a,pp,length)
int type;
EVP_PKEY **a;
unsigned char **pp;
long length;
	{
	EVP_PKEY *ret;

	if ((a == NULL) || (*a == NULL))
		{
		if ((ret=EVP_PKEY_new()) == NULL)
			{
			ASN1err(ASN1_F_D2I_PUBLICKEY,ERR_R_EVP_LIB);
			return(NULL);
			}
		}
	else	ret= *a;

	switch (type)
		{
	case EVP_PKEY_RSA:
	case EVP_PKEY_RSA2:
		if ((ret->pkey.rsa=d2i_RSAPublicKey(NULL,pp,length)) == NULL)
			{
			ASN1err(ASN1_F_D2I_PUBLICKEY,ERR_R_ASN1_LIB);
			goto err;
			}
		break;
	case EVP_PKEY_DSA:
	case EVP_PKEY_DSA2:
	case EVP_PKEY_DSA3:
		if ((ret->pkey.dsa=d2i_DSAPublicKey(NULL,pp,length)) == NULL)
			{
			ASN1err(ASN1_F_D2I_PUBLICKEY,ERR_R_ASN1_LIB);
			goto err;
			}
		break;
	default:
		ASN1err(ASN1_F_D2I_PUBLICKEY,ASN1_R_UNKNOWN_PUBLIC_KEY_TYPE);
		goto err;
		break;
		}
	ret->type=type;
	if (a != NULL) (*a)=ret;
	return(ret);
err:
	if (ret != NULL)EVP_PKEY_free(ret);
	if ((a != NULL) && (*a == ret)) *a=NULL;
	return(NULL);
	}

