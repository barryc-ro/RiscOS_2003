/* crypto/evp/p_s.c */
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
#include "rsa.h"
#include "envelope.h"
#include "objects.h"
#include "x509.h"

#ifdef undef
void EVP_SignInit(ctx,type)
EVP_MD_CTX *ctx;
EVP_MD *type;
	{
	EVP_DigestInit(ctx,type);
	}

void EVP_SignUpdate(ctx,data,count)
EVP_MD_CTX *ctx;
unsigned char *data;
unsigned int count;
	{
	EVP_DigestUpdate(ctx,data,count);
	}
#endif

int EVP_SignFinal(ctx,sigret,siglen,rsa)
EVP_MD_CTX *ctx;
unsigned char *sigret;
unsigned int *siglen;
RSA *rsa;
	{
	unsigned char m[EVP_MAX_MD_SIZE];
	unsigned int m_len;
	int i,j,ret=1;
	X509_SIG sig;
        X509_ALGOR algor;
	ASN1_TYPE parameter;
	ASN1_OCTET_STRING digest;
	unsigned char *p,*s;

	*siglen=0;
	EVP_DigestFinal(ctx,&(m[0]),&m_len);

	sig.algor= &algor;
	sig.algor->algorithm=OBJ_nid2obj(ctx->digest->type);
	if (sig.algor->algorithm == NULL)
		{
		EVPerr(EVP_F_EVP_SIGNFINAL,EVP_R_UNKNOWN_ALGORITHM_TYPE);
		return(0);
		}
	if (sig.algor->algorithm->length == 0)
		{
		EVPerr(EVP_F_EVP_SIGNFINAL,EVP_R_THE_ASN1_OBJECT_IDENTIFIER_IS_NOT_KNOWN_FOR_THIS_MD);
		return(0);
		}
	parameter.type=V_ASN1_NULL;
	parameter.value.ptr=NULL;
	sig.algor->parameter= &parameter;

	sig.digest= &digest;
	sig.digest->data=m;
	sig.digest->length=m_len;

	i=i2d_X509_SIG(&sig,NULL);
	j=RSA_size(rsa);
	if ((i-RSA_PKCS1_PADDING) > j)
		{
		EVPerr(EVP_F_EVP_SIGNFINAL,EVP_R_DIGEST_TOO_BIG_FOR_RSA_KEY);
		return(0);
		}
	s=(unsigned char *)malloc((unsigned int)j+1);
	if (s == NULL)
		{ EVPerr(EVP_F_EVP_SIGNFINAL,ERR_R_MALLOC_FAILURE); return(0); }
	p=s;
	i2d_X509_SIG(&sig,&p);
	i=RSA_private_encrypt(i,s,sigret,rsa);
	if (i <= 0)
		ret=0;
	else
		*siglen=i;

	memset(s,0,(unsigned int)j+1);
	free(s);
	return(ret);
	}
