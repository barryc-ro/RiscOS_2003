/* crypto/x509/x509_req.c */
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
#include "asn1.h"
#include "x509.h"
#include "objects.h"
#include "buffer.h"
#include "pem.h"

X509_REQ *X509_to_X509_REQ(x,pkey)
X509 *x;
EVP_PKEY *pkey;
	{
	X509_REQ *ret;
	X509_REQ_INFO *ri;
	X509_NAME *n;
	ASN1_OBJECT *obj;
	int i;
	unsigned char *s,*p;

	ret=X509_REQ_new();
	if (ret == NULL)
		{
		X509err(X509_F_X509_TO_X509_REQ,ERR_R_MALLOC_FAILURE);
		goto err;
		}

	ri=ret->req_info;

	ri->version->length=1;
	ri->version->data=(unsigned char *)malloc(1);
	if (ri->version->data == NULL) goto err;
	ri->version->data[0]=0; /* version == 0 */

	n=X509_NAME_dup(x->cert_info->subject);
	if (n == NULL) goto err;

	X509_NAME_free(ri->subject); 
	ri->subject=n;

	if (pkey->type == EVP_PKEY_RSA)
		obj=OBJ_nid2obj(NID_rsaEncryption);
	else if (pkey->type == EVP_PKEY_DSA)
		obj=OBJ_nid2obj(NID_dss);
	else
		{
		X509err(X509_F_X509_TO_X509_REQ,X509_R_BAD_PUBLIC_KEY_TYPE);
		goto err;
		}

	if (obj == NULL) goto err;
	ASN1_OBJECT_free(ri->pubkey->algor->algorithm);
	ri->pubkey->algor->algorithm=obj;

	i=i2d_PublicKey(pkey,NULL);
	s=(unsigned char *)malloc((unsigned int)i+1);
	if (s == NULL)
		{
		X509err(X509_F_X509_TO_X509_REQ,ERR_R_MALLOC_FAILURE);
		goto err;
		}
	p=s;
	i2d_PublicKey(pkey,&p);
	ri->pubkey->public_key->length=i;
	ri->pubkey->public_key->data=s;

	if (!X509_REQ_sign(ret,pkey,EVP_md5()))
		goto err;
	return(ret);
err:
	X509_REQ_free(ret);
	return(NULL);
	}

EVP_PKEY *X509_REQ_extract_key(req)
X509_REQ *req;
	{
	X509_REQ_INFO *ri;
	EVP_PKEY *pkey=NULL;
	long j;
	unsigned char *p;
	int type;

	ri=req->req_info;

	type=OBJ_obj2nid(ri->pubkey->algor->algorithm);
	p=ri->pubkey->public_key->data;
	j=ri->pubkey->public_key->length;
	pkey=d2i_PublicKey(type,NULL,&p,(long)j);
	return(pkey);
	}

