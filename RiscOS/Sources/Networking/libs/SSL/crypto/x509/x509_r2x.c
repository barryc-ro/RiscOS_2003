/* crypto/x509/x509_r2x.c */
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
#include "rsa.h"
#include "envelope.h"
#include "asn1.h"
#include "x509.h"
#include "objects.h"
#include "buffer.h"
#include "pem.h"

X509 *X509_REQ_to_X509(r,days,pkey)
X509_REQ *r;
int days;
EVP_PKEY *pkey;
	{
	X509 *ret=NULL;
	int er=1;
	X509_REQ_INFO *ri=NULL;
	X509_CINF *xi=NULL;

	if ((ret=X509_new()) == NULL)
		{
		X509err(X509_F_X509_REQ_TO_X509,ERR_R_MALLOC_FAILURE);
		goto err;
		}

	/* duplicate the request */
	ri=(X509_REQ_INFO *)ASN1_dup(i2d_X509_REQ_INFO,
		(char *(*)())d2i_X509_REQ_INFO,(char *)r->req_info);
	if (ri == NULL) goto err;

	xi=ret->cert_info;

	if (sk_num(ri->attributes) != 0)
		{
		if ((xi->version=ASN1_INTEGER_new()) == NULL) goto err;
		if (!ASN1_INTEGER_set(xi->version,2)) goto err;
/*		xi->extensions=ri->attributes; <- bad, should not ever be done
		ri->attributes=NULL; */
		}

	X509_NAME_free(xi->subject); 
	xi->subject=ri->subject;
	ri->subject=NULL;
	X509_NAME_free(xi->issuer); 
	xi->issuer=X509_NAME_dup(xi->subject);
	if (xi->issuer == NULL) goto err;

	X509_gmtime_adj(xi->validity->notBefore,0);
	X509_gmtime_adj(xi->validity->notAfter,(long)60*60*24*days);

	xi->key=ri->pubkey;
	ri->pubkey=NULL;

	if (!X509_sign(ret,pkey,EVP_md5()))
		goto err;
	er=0;
err:
	if (er)
		{
		X509_free(ret);
		X509_REQ_INFO_free(ri);
		return(NULL);
		}
	return(ret);
	}

