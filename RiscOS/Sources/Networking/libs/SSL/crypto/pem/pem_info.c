/* crypto/pem/pem_info.c */
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
#include "buffer.h"
#include "objects.h"
#include "rsa.h"
#include "dsa.h"
#include "envelope.h"
#include "x509.h"
#include "pem.h"

#ifndef WIN16
STACK *PEM_X509_INFO_read(fp,sk,cb)
FILE *fp;
STACK *sk;
int (*cb)();
	{
        BIO *b;
        STACK *ret;

        if ((b=BIO_new(BIO_s_file())) == NULL)
		{
		PEMerr(PEM_F_PEM_X509_INFO_READ,ERR_R_BUF_LIB);
                return(0);
		}
        BIO_set_fp(b,fp,BIO_NOCLOSE);
        ret=PEM_X509_INFO_read_bio(b,sk,cb);
        BIO_free(b);
        return(ret);
	}
#endif

STACK *PEM_X509_INFO_read_bio(bp,sk,cb)
BIO *bp;
STACK *sk;
int (*cb)();
	{
	X509_INFO *xi=NULL;
	char *name=NULL,*header=NULL,**pp;
	unsigned char *data=NULL,*p;
	long len,error=0;
	int ok=0;
	STACK *ret=NULL;
	unsigned int i,raw;
	char *(*d2i)();

	if (sk == NULL)
		{
		if ((ret=sk_new_null()) == NULL)
			{
			PEMerr(PEM_F_PEM_X509_INFO_READ_BIO,ERR_R_MALLOC_FAILURE);
			goto err;
			}
		}
	else
		ret=sk;

	if ((xi=X509_INFO_new()) == NULL) goto err;
	for (;;)
		{
		raw=0;
		i=PEM_read_bio(bp,&name,&header,&data,&len);
		if (i == 0)
			{
			error=ERR_GET_REASON(ERR_peek_error());
			if (error == PEM_R_NO_START_LINE)
				{
				ERR_clear_error();
				break;
				}
			goto err;
			}
start:
		if (	(strcmp(name,PEM_STRING_X509) == 0) ||
			(strcmp(name,PEM_STRING_X509_OLD) == 0))
			{
			d2i=(char *(*)())d2i_X509;
			pp=(char **)&(xi->x509);
			}
		else if (strcmp(name,PEM_STRING_X509_CRL) == 0)
			{
			d2i=(char *(*)())d2i_X509_CRL;
			pp=(char **)&(xi->crl);
			}
		else if (strcmp(name,PEM_STRING_RSA) == 0)
			{
			d2i=(char *(*)())d2i_RSAPrivateKey;
			if (xi->x_pkey != NULL) X509_PKEY_free(xi->x_pkey);
			xi->x_pkey=X509_PKEY_new();
			xi->x_pkey->dec_pkey->type=EVP_PKEY_RSA;
			pp=(char **)&(xi->x_pkey->dec_pkey->pkey.rsa);
			if ((int)strlen(header) > 10) /* assume encrypted */
				raw=1;
			}
		else if (strcmp(name,PEM_STRING_DSA) == 0)
			{
			d2i=(char *(*)())d2i_DSAPrivateKey;
			if (xi->x_pkey != NULL) X509_PKEY_free(xi->x_pkey);
			xi->x_pkey=X509_PKEY_new();
			xi->x_pkey->dec_pkey->type=EVP_PKEY_DSA;
			pp=(char **)&(xi->x_pkey->dec_pkey->pkey.dsa);
			if ((int)strlen(header) > 10) /* assume encrypted */
				raw=1;
			}
		else
			{
			d2i=NULL;
			pp=NULL;
			}

		if (d2i != NULL)
			{
			if ((*pp != NULL) || ((xi->enc_data != NULL)
				&& (xi->x_pkey != NULL)))
				{
				if (!sk_push(ret,(char *)xi)) goto err;
				if ((xi=X509_INFO_new()) == NULL) goto err;
				goto start;
				}
			if (!raw)
				{
				EVP_CIPHER_INFO cipher;

				if (!PEM_get_EVP_CIPHER_INFO(header,&cipher))
					goto err;
				if (!PEM_do_header(&cipher,data,&len,cb))
					goto err;
				p=data;
				if (d2i(pp,&p,len) == NULL)
					{
					PEMerr(PEM_F_PEM_X509_INFO_READ_BIO,ERR_R_ASN1_LIB);
					goto err;
					}
				}
			else
				{ /* encrypted RSA data */
				if (!PEM_get_EVP_CIPHER_INFO(header,
					&xi->enc_cipher)) goto err;
				xi->enc_data=(char *)data;
				xi->enc_len=(int)len;
				data=NULL;
				}
			}
		else	{
			/* unknown */
			}
		if (name != NULL) free(name);
		if (header != NULL) free(header);
		if (data != NULL) free(data);
		name=NULL;
		header=NULL;
		data=NULL;
		}
	if ((xi->x509 != NULL) || (xi->crl != NULL) ||
		(xi->x_pkey != NULL) || (xi->enc_data != NULL))
		{
		if (!sk_push(ret,(char *)xi)) goto err;
		xi=NULL;
		}
	ok=1;
err:
	if (xi != NULL) X509_INFO_free(xi);
	if (!ok)
		{
		for (i=0; ((int)i)<sk_num(ret); i++)
			{
			xi=(X509_INFO *)sk_value(ret,i);
			X509_INFO_free(xi);
			}
		if (ret != sk) sk_free(ret);
		}
		
	if (name != NULL) free(name);
	if (header != NULL) free(header);
	if (data != NULL) free(data);
	return(ret);
	}
