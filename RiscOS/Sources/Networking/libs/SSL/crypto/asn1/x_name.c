/* crypto/asn1/x_name.c */
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
#include "objects.h"
#include "asn1_mac.h"

/*
 * ASN1err(ASN1_F_D2I_X509_NAME,ASN1_R_LENGTH_MISMATCH);
 * ASN1err(ASN1_F_X509_NAME_NEW,ASN1_R_UNKNOWN_ATTRIBUTE_TYPE);
 * ASN1err(ASN1_F_D2I_X509_NAME_ENTRY,ASN1_R_LENGTH_MISMATCH);
 * ASN1err(ASN1_F_X509_NAME_ENTRY_NEW,ASN1_R_UNKNOWN_ATTRIBUTE_TYPE);
 */

int i2d_X509_NAME_ENTRY(a,pp)
X509_NAME_ENTRY *a;
unsigned char **pp;
	{
	M_ASN1_I2D_vars(a);

	M_ASN1_I2D_len(a->object,i2d_ASN1_OBJECT);
	M_ASN1_I2D_len(a->value,i2d_ASN1_PRINTABLE);

	M_ASN1_I2D_seq_total();

	M_ASN1_I2D_put(a->object,i2d_ASN1_OBJECT);
	M_ASN1_I2D_put(a->value,i2d_ASN1_PRINTABLE);

	M_ASN1_I2D_finish();
	}

X509_NAME_ENTRY *d2i_X509_NAME_ENTRY(a,pp,length)
X509_NAME_ENTRY **a;
unsigned char **pp;
long length;
	{
	M_ASN1_D2I_vars(a,X509_NAME_ENTRY *,X509_NAME_ENTRY_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(ret->object,d2i_ASN1_OBJECT);
	M_ASN1_D2I_get(ret->value,d2i_ASN1_PRINTABLE);
	ret->set=0;
	M_ASN1_D2I_Finish(a,X509_NAME_ENTRY_free,ASN1_F_D2I_X509_NAME_ENTRY);
	}

int i2d_X509_NAME(a,pp)
X509_NAME *a;
unsigned char **pp;
	{
	X509_NAME_ENTRY *ne,*fe=NULL;
	int set=0,r,ret=0;
	int i;
	unsigned char *p;

	if (a == NULL) return(0);
	for (i=0; i<sk_num(a); i++)
		{
		ne=(X509_NAME_ENTRY *)sk_value(a,i);
		if (fe == NULL)
			{
			fe=ne;
			fe->size=0;
			}

		if (ne->set != set)
			{
			ret+=ASN1_object_size(1,fe->size,V_ASN1_SET);
			fe=ne;
			fe->size=0;
			set=ne->set;
			}
		fe->size+=i2d_X509_NAME_ENTRY(ne,NULL);
		}

	ret+=ASN1_object_size(1,fe->size,V_ASN1_SET);

	M_ASN1_I2D_seq_total();

	set=-1;
	for (i=0; i<sk_num(a); i++)
		{
		ne=(X509_NAME_ENTRY *)sk_value(a,i);
		if (set != ne->set)
			{
			set=ne->set;
			ASN1_put_object(&p,1,ne->size,
				V_ASN1_SET,V_ASN1_UNIVERSAL);
			}
		i2d_X509_NAME_ENTRY(ne,&p);
		}

	M_ASN1_I2D_finish();
	}

X509_NAME *d2i_X509_NAME(a,pp,length)
X509_NAME **a;
unsigned char **pp;
long length;
	{
	int set=0;
	int index=0;
	M_ASN1_D2I_vars(a,X509_NAME *,X509_NAME_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	for (;;)
		{
		if (M_ASN1_D2I_end_sequence()) break;
		M_ASN1_D2I_get_set(ret,d2i_X509_NAME_ENTRY);
		for (; index < sk_num(ret); index++)
			{
			((X509_NAME_ENTRY *)sk_value(ret,index))->set=set;
			}
		set++;
		}
	M_ASN1_D2I_Finish(a,X509_NAME_free,ASN1_F_D2I_X509_NAME);
	}

X509_NAME *X509_NAME_new()
	{
	X509_NAME *ret=NULL;

	if ((ret=sk_new(NULL)) == NULL) goto err2;
	return(ret);
	M_ASN1_New_Error(ASN1_F_X509_NAME_NEW);
	}

X509_NAME_ENTRY *X509_NAME_ENTRY_new()
	{
	X509_NAME_ENTRY *ret=NULL;

	M_ASN1_New_Malloc(ret,X509_NAME_ENTRY);
/*	M_ASN1_New(ret->object,ASN1_OBJECT_new);*/
	ret->object=NULL;
	ret->set=0;
	M_ASN1_New(ret->value,ASN1_BIT_STRING_new);
	return(ret);
	M_ASN1_New_Error(ASN1_F_X509_NAME_ENTRY_NEW);
	}

void X509_NAME_free(sk)
X509_NAME *sk;
	{
	sk_pop_free(sk,X509_NAME_ENTRY_free);
	}

void X509_NAME_ENTRY_free(a)
X509_NAME_ENTRY *a;
	{
	if (a == NULL) return;
	ASN1_OBJECT_free(a->object);
	ASN1_BIT_STRING_free(a->value);
	free(a);
	}

