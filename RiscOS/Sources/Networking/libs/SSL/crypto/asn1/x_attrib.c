/* crypto/asn1/x_attrib.c */
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
 * ASN1err(ASN1_F_D2I_X509_ATTRIBUTE,ASN1_R_LENGTH_MISMATCH);
 * ASN1err(ASN1_F_X509_ATTRIBUTE_NEW,ASN1_R_UNKNOWN_ATTRIBUTE_TYPE);
 * ASN1err(ASN1_F_I2D_X509_ATTRIBUTE,ASN1_R_UNKNOWN_ATTRIBUTE_TYPE);
 */

int i2d_X509_ATTRIBUTE(a,pp)
X509_ATTRIBUTE *a;
unsigned char **pp;
	{
	int k=0;
	int r=0,ret=0;
	unsigned char **p=NULL;

	if (a == NULL) return(0);

	p=NULL;
	for (;;)
		{
		if (k)
			{
			r=ASN1_object_size(1,ret,V_ASN1_SEQUENCE);
			if (pp == NULL) return(r);
			p=pp;
			ASN1_put_object(p,1,ret,V_ASN1_SEQUENCE,
				V_ASN1_UNIVERSAL);
			}

		ret+=i2d_ASN1_OBJECT(a->object,p);
		if (a->set)
			ret+=i2d_ASN1_SET(a->value.set,p,i2d_ASN1_TYPE,
				V_ASN1_SET,V_ASN1_UNIVERSAL);
		else
			ret+=i2d_ASN1_TYPE(a->value.single,p);
		if (k++) return(r);
		}
	}

X509_ATTRIBUTE *d2i_X509_ATTRIBUTE(a,pp,length)
X509_ATTRIBUTE **a;
unsigned char **pp;
long length;
	{
	M_ASN1_D2I_vars(a,X509_ATTRIBUTE *,X509_ATTRIBUTE_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(ret->object,d2i_ASN1_OBJECT);
	ret->nid=OBJ_obj2nid(ret->object);
	if (M_ASN1_next == (V_ASN1_CONSTRUCTED|V_ASN1_UNIVERSAL|V_ASN1_SET))
		{
		ret->set=1;
		M_ASN1_D2I_get_set(ret->value.set,d2i_ASN1_TYPE);
		}
	else
		{
		ret->set=0;
		M_ASN1_D2I_get(ret->value.single,d2i_ASN1_TYPE);
		}

	M_ASN1_D2I_Finish(a,X509_ATTRIBUTE_free,ASN1_F_D2I_X509_ATTRIBUTE);
	}

X509_ATTRIBUTE *X509_ATTRIBUTE_new()
	{
	X509_ATTRIBUTE *ret=NULL;

	M_ASN1_New_Malloc(ret,X509_ATTRIBUTE);
	M_ASN1_New(ret->object,ASN1_OBJECT_new);
	ret->nid=0;
	ret->set=0;
	ret->value.ptr=NULL;
	return(ret);
	M_ASN1_New_Error(ASN1_F_X509_ATTRIBUTE_NEW);
	}
	
void X509_ATTRIBUTE_free(a)
X509_ATTRIBUTE *a;
	{
	if (a == NULL) return;
	ASN1_OBJECT_free(a->object);
	if (a->set)
		sk_pop_free(a->value.set,ASN1_TYPE_free);
	else
		ASN1_TYPE_free(a->value.single);
	free(a);
	}

