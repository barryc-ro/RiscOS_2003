/* crypto/asn1/t_req.c */
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
#include "bn.h"
#include "rsa.h"
#include "objects.h"
#include "x509.h"

#ifndef WIN16
int X509_REQ_print_fp(fp,x)
FILE *fp;
X509_REQ *x;
        {
        BIO *b;
        int ret;

        if ((b=BIO_new(BIO_s_file())) == NULL)
		{
		X509err(X509_F_X509_REQ_PRINT_FP,ERR_R_BUF_LIB);
                return(0);
		}
        BIO_set_fp(b,fp,BIO_NOCLOSE);
        ret=X509_REQ_print(b, x);
        BIO_free(b);
        return(ret);
        }
#endif

int X509_REQ_print(bp,x)
BIO *bp;
X509_REQ *x;
	{
	unsigned long l;
	int i,n;
	char *m=NULL,*s,*neg;
	X509_REQ_INFO *ri;
	STACK *sk;
	RSA *rsa;
	char str[128];

	ri=x->req_info;
	sprintf(str,"Certificate Request:\n");
	if (BIO_puts(bp,str) <= 0) goto err;
	sprintf(str,"%4sData:\n","");
	if (BIO_puts(bp,str) <= 0) goto err;

	neg=(ri->version->type == V_ASN1_NEG_INTEGER)?"-":"";
	l=0;
	for (i=0; i<ri->version->length; i++)
		{ l<<=8; l+=ri->version->data[i]; }
	sprintf(str,"%8sVersion: %s%lu (%s0x%lx)\n","",neg,l,neg,l);
	if (BIO_puts(bp,str) <= 0) goto err;
	sprintf(str,"%8sSubject: ","");
	if (BIO_puts(bp,str) <= 0) goto err;

	X509_NAME_print(bp,ri->subject,16);
	sprintf(str,"\n%8sSubject Public Key Info:\n","");
	if (BIO_puts(bp,str) <= 0) goto err;
	i=OBJ_obj2nid(ri->pubkey->algor->algorithm);
	sprintf(str,"%12sPublic Key Algorithm: %s\n","",
		(i == NID_undef)?"UNKNOWN":OBJ_nid2ln(i));
	if (BIO_puts(bp,str) <= 0) goto err;

	rsa=RSA_new();
	s=(char *)ri->pubkey->public_key->data;
	i=ri->pubkey->public_key->length;
	d2i_RSAPublicKey(&rsa,(unsigned char **)&s,(long)i);
	i=RSA_size(rsa);
	m=s=(char *)malloc((unsigned int)i+10);
	s[0]=0;
	n=BN_bn2bin(rsa->n,(unsigned char *)&(s[1]));
	if (s[1] & 0x80)
		n++;
	else	s++;
	
	sprintf(str,"%12sPublic Key: (%d bit)\n","",BN_num_bits(rsa->n));
	if (BIO_puts(bp,str) <= 0) goto err;
	neg=(rsa->n->neg)?" (Negative)":"";
	sprintf(str,"%16sModulus:%s","",neg);
	if (BIO_puts(bp,str) <= 0) goto err;

	for (i=0; i<n; i++)
		{
		if ((i%15) == 0)
			{
			sprintf(str,"\n%20s","");
			if (BIO_puts(bp,str) <= 0) goto err;
			}
		sprintf(str,"%02x%s",(unsigned char)s[i],((i+1) == n)?"":":");
		if (BIO_puts(bp,str) <= 0) goto err;
		}
	free(m);
	if (BIO_puts(bp,"\n") <= 0) goto err;
	neg=(rsa->e->neg)?"-":"";
	
	l=0;
	for (i=0; i<rsa->e->top; i++)
		{
		l<<=BN_BITS4;
		l<<=BN_BITS4;
		l+=rsa->e->d[0];
		}
	sprintf(str,"%16sExponent: %s%lu (%s0x%lx)\n","",neg,l,neg,l);
	if (BIO_puts(bp,str) <= 0) goto err;
	RSA_free(rsa);

	/* may not be */
	sprintf(str,"%8sAttributes:\n","");
	if (BIO_puts(bp,str) <= 0) goto err;

	sk=x->req_info->attributes;
	if ((sk == NULL) || (sk_num(sk) == 0))
		{
		if (!x->req_info->req_kludge)
			{
			sprintf(str,"%12sa0:00\n","");
			if (BIO_puts(bp,str) <= 0) goto err;
			}
		}
	else
		{
		for (i=0; i<sk_num(sk); i++)
			{
			X509_ATTRIBUTE *a;
			ASN1_BIT_STRING *bs;
			ASN1_TYPE *t;
			int j,k;

			a=(X509_ATTRIBUTE *)sk_value(sk,i);
			sprintf(str,"%12s","");
			if (BIO_puts(bp,str) <= 0) goto err;
			if ((j=i2a_ASN1_OBJECT(bp,a->object)) > 0)
			if (a->set)
				{
				if (BIO_puts(bp,"\n") <= 0) goto err;
				continue;
				}
			t=a->value.single;
			bs=t->value.bit_string;
			for (j=25-j; j>0; j--)
				if (BIO_write(bp," ",1) != 1) goto err;
			k=t->type;
			if (BIO_puts(bp,":") <= 0) goto err;
			if (	(k == V_ASN1_PRINTABLESTRING) ||
				(k == V_ASN1_T61STRING) ||
				(k == V_ASN1_IA5STRING))
				{
				if (BIO_write(bp,(char *)bs->data,bs->length)
					!= bs->length)
					goto err;
				BIO_puts(bp,"\n");
				}
			else
				{
				BIO_puts(bp,"unable to print attribute\n");
				}
			}
		}

	i=OBJ_obj2nid(x->sig_alg->algorithm);
	sprintf(str,"%4sSignature Algorithm: %s","",
		(i == NID_undef)?"UNKNOWN":OBJ_nid2ln(i));
	if (BIO_puts(bp,str) <= 0) goto err;

	n=x->signature->length;
	s=(char *)x->signature->data;
	for (i=0; i<n; i++)
		{
		if ((i%18) == 0)
			{
			sprintf(str,"\n%8s","");
			if (BIO_puts(bp,str) <= 0) goto err;
			}
		sprintf(str,"%02x%s",(unsigned char)s[i],((i+1) == n)?"":":");
		if (BIO_puts(bp,str) <= 0) goto err;
		}
	if (BIO_puts(bp,"\n") <= 0) goto err;
	return(1);
err:
	X509err(X509_F_X509_REQ_PRINT,ERR_R_BUF_LIB);
	return(0);
	}
