/* crypto/asn1/a_d2i_fp.c */
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
#include "asn1_mac.h"

#define READ_CHUNK   2048

#ifndef WIN16
char *ASN1_d2i_fp(xnew,d2i,in,x)
char *(*xnew)();
char *(*d2i)();
FILE *in;
unsigned char **x;
        {
        BIO *b;
        char *ret;

        if ((b=BIO_new(BIO_s_file())) == NULL)
		{
		ASN1err(ASN1_F_ASN1_D2I_FP,ERR_R_BUF_LIB);
                return(NULL);
		}
        BIO_set_fp(b,in,BIO_NOCLOSE);
        ret=ASN1_d2i_bio(xnew,d2i,b,x);
        BIO_free(b);
        return(ret);
        }
#endif

char *ASN1_d2i_bio(xnew,d2i,in,x)
char *(*xnew)();
char *(*d2i)();
BIO *in;
unsigned char **x;
	{
	BUF_MEM *b;
	unsigned char *p;
	int i,lenbuf=0;
	char *ret=NULL;

	b=BUF_MEM_new();
	if (b == NULL)
		{
		ASN1err(ASN1_F_ASN1_D2I_BIO,ERR_R_MALLOC_FAILURE);
		return(0);
		}
	if (!BUF_MEM_grow(b,READ_CHUNK))
		{
		ASN1err(ASN1_F_ASN1_D2I_BIO,ERR_R_MALLOC_FAILURE);
		goto err;
		}
	
	for (;;)
		{
		i=BIO_read(in,&(b->data[lenbuf]),READ_CHUNK);
		if (i <= 0) break;
		lenbuf+=i;
		if (!BUF_MEM_grow(b,lenbuf+READ_CHUNK))
			{
			ASN1err(ASN1_F_ASN1_D2I_BIO,ERR_R_MALLOC_FAILURE);
			goto err;
			}
		}
	if (lenbuf < 10)
		{
		ASN1err(ASN1_F_ASN1_D2I_BIO,ASN1_R_NOT_ENOUGH_DATA);
		goto err;
		}
	p=(unsigned char *)b->data;
	ret=d2i(x,&p,lenbuf);
err:
	BUF_MEM_free(b);
	return(ret);
	}
