/* crypto/x509/x509_cmp.c */
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
#include <sys/types.h>
#include <sys/stat.h>
#include "cryptlib.h"
#include "md5.h"
#include "objects.h"
#include "x509.h"

int X509_issuer_and_serial_cmp(a,b)
X509 *a;
X509 *b;
	{
	int i;
	X509_CINF *ai,*bi;

	ai=a->cert_info;
	bi=b->cert_info;
	i=ASN1_BIT_STRING_cmp(ai->serialNumber,bi->serialNumber);
	if (i) return(i);
	return(X509_name_cmp(ai->issuer,bi->issuer));
	}

unsigned long X509_issuer_and_serial_hash(a)
X509 *a;
	{
	unsigned long ret=0;
	MD5_CTX ctx;
	unsigned char md[16];
	char *str;

	str=X509_NAME_oneline(a->cert_info->issuer);
	if (str == NULL) return(0);
	ret=strlen(str);
	MD5_Init(&ctx);
	MD5_Update(&ctx,(unsigned char *)str,ret);
	free(str);
	MD5_Update(&ctx,(unsigned char *)a->cert_info->serialNumber->data,
		(unsigned long)a->cert_info->serialNumber->length);
	MD5_Final(&(md[0]),&ctx);
	ret=(	((unsigned long)md[0]     )|((unsigned long)md[1]<<8L)|
		((unsigned long)md[2]<<16L)|((unsigned long)md[3]<<24L)
		)&0xffffffffL;
	return(ret);
	}
	
int X509_issuer_name_cmp(a, b)
X509 *a;
X509 *b;
	{
	return(X509_name_cmp(a->cert_info->issuer,b->cert_info->issuer));
	}

int X509_subject_name_cmp(a, b)
X509 *a;
X509 *b;
	{
	return(X509_name_cmp(a->cert_info->subject,b->cert_info->subject));
	}

X509_NAME *X509_get_issuer_name(a)
X509 *a;
	{
	return(a->cert_info->issuer);
	}

unsigned long X509_issuer_name_hash(x)
X509 *x;
	{
	return(X509_name_hash(x->cert_info->issuer));
	}

X509_NAME *X509_get_subject_name(a)
X509 *a;
	{
	return(a->cert_info->subject);
	}

ASN1_INTEGER *X509_get_serial_numberf(a)
X509 *a;
	{
	return(a->cert_info->serialNumber);
	}

unsigned long X509_subject_name_hash(x)
X509 *x;
	{
	return(X509_name_hash(x->cert_info->subject));
	}

int X509_name_cmp(a, b)
X509_NAME *a;
X509_NAME *b;
	{
	int i,j;
	X509_NAME_ENTRY *na,*nb;

	if (sk_num(a) != sk_num(b))
		return(sk_num(a)-sk_num(b));
	for (i=sk_num(a)-1; i>=0; i--)
		{
		na=(X509_NAME_ENTRY *)sk_value(a,i);
		nb=(X509_NAME_ENTRY *)sk_value(b,i);
		j=na->value->length-nb->value->length;
		if (j) return(j);
		j=memcmp(na->value->data,nb->value->data,
			na->value->length);
		if (j) return(j);
		j=na->set-nb->set;
		if (j) return(j);
		}

	/* We will check the object types after checking the values
	 * since the values will more often be different than the object
	 * types. */
	for (i=sk_num(a)-1; i>=0; i--)
		{
		na=(X509_NAME_ENTRY *)sk_value(a,i);
		nb=(X509_NAME_ENTRY *)sk_value(b,i);
		j=OBJ_cmp(na->object,nb->object);
		if (j) return(j);
		}
	return(0);
	}

/* I should do a DER encoding of the name and then hash it. */
unsigned long X509_name_hash(x)
X509_NAME *x;
	{
	unsigned long ret=0;
	unsigned char md[16];
	char *str;

	str=X509_NAME_oneline(x);
	if (str == NULL) return(0);
	ret=strlen(str);
	MD5((unsigned char *)str,ret,&(md[0]));
	free(str);
	ret=(	((unsigned long)md[0]     )|((unsigned long)md[1]<<8L)|
		((unsigned long)md[2]<<16L)|((unsigned long)md[3]<<24L)
		)&0xffffffffL;
	return(ret);
	}
