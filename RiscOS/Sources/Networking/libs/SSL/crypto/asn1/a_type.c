/* crypto/asn1/a_type.c */
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
#include "asn1_mac.h"

/* ASN1err(ASN1_F_ASN1_TYPE_NEW,ASN1_R_ERROR_STACK);
 * ASN1err(ASN1_F_D2I_ASN1_BYTES,ASN1_R_ERROR_STACK);
 * ASN1err(ASN1_F_D2I_ASN1_BYTES,ASN1_R_WRONG_TAG);
 */

int i2d_ASN1_TYPE(a,pp)
ASN1_TYPE *a;
unsigned char **pp;
	{
	int r=0;

	if (a == NULL) return(0);

	switch (a->type)
		{
	case V_ASN1_NULL:
		if (pp != NULL)
			ASN1_put_object(pp,0,0,V_ASN1_NULL,V_ASN1_UNIVERSAL);
		r=2;
		break;
	case V_ASN1_INTEGER:
	case V_ASN1_NEG_INTEGER:
		r=i2d_ASN1_INTEGER(a->value.integer,pp);
		break;
	case V_ASN1_BIT_STRING:
		r=i2d_ASN1_BIT_STRING(a->value.bit_string,pp);
		break;
	case V_ASN1_OCTET_STRING:
		r=i2d_ASN1_OCTET_STRING(a->value.octet_string,pp);
		break;
	case V_ASN1_OBJECT:
		r=i2d_ASN1_OBJECT(a->value.object,pp);
		break;
	case V_ASN1_PRINTABLESTRING:
		r=i2d_ASN1_PRINTABLESTRING(a->value.printablestring,pp);
		break;
	case V_ASN1_T61STRING:
		r=i2d_ASN1_T61STRING(a->value.t61string,pp);
		break;
	case V_ASN1_IA5STRING:
		r=i2d_ASN1_IA5STRING(a->value.ia5string,pp);
		break;
	case V_ASN1_UTCTIME:
		r=i2d_ASN1_UTCTIME(a->value.utctime,pp);
		break;
	case V_ASN1_SET:
	case V_ASN1_SEQUENCE:
		r=i2d_ASN1_bytes(a->value.set,pp,a->type,V_ASN1_UNIVERSAL);
		break;
		}
	return(r);
	}

ASN1_TYPE *d2i_ASN1_TYPE(a,pp,length)
ASN1_TYPE **a;
unsigned char **pp;
long length;
	{
	ASN1_TYPE *ret=NULL;
	unsigned char *q,*p,*max;
	int inf,tag,xclass;
	long len;

	if ((a == NULL) || ((*a) == NULL))
		{
		if ((ret=ASN1_TYPE_new()) == NULL) goto err;
		}
	else
		ret=(*a);

	p= *pp;
	q=p;
	max=(p+length);

	inf=ASN1_get_object(&q,&len,&tag,&xclass,length);
	if (inf == 0xff) goto err;
	
	if (ret->value.ptr != NULL)
		ASN1_TYPE_free(ret);
	ret->value.ptr=NULL;
	switch (tag)
		{
	case V_ASN1_NULL:
		p=q;
		ret->value.ptr=NULL;
		break;
	case V_ASN1_INTEGER:
		if ((ret->value.integer=
			d2i_ASN1_INTEGER(NULL,&p,max-p)) == NULL)
			goto err;
		break;
	case V_ASN1_BIT_STRING:
		if ((ret->value.bit_string=
			d2i_ASN1_BIT_STRING(NULL,&p,max-p)) == NULL)
			goto err;
		break;
	case V_ASN1_OCTET_STRING:
		if ((ret->value.octet_string=
			d2i_ASN1_OCTET_STRING(NULL,&p,max-p)) == NULL)
			goto err;
		break;
	case V_ASN1_OBJECT:
		if ((ret->value.object=
			d2i_ASN1_OBJECT(NULL,&p,max-p)) == NULL)
			goto err;
		break;
	case V_ASN1_PRINTABLESTRING:
		if ((ret->value.printablestring=
			d2i_ASN1_PRINTABLESTRING(NULL,&p,max-p)) == NULL)
			goto err;
		break;
	case V_ASN1_T61STRING:
		if ((ret->value.t61string=
			d2i_ASN1_T61STRING(NULL,&p,max-p)) == NULL)
			goto err;
		break;
	case V_ASN1_IA5STRING:
		if ((ret->value.ia5string=
			d2i_ASN1_IA5STRING(NULL,&p,max-p)) == NULL)
			goto err;
		break;
	case V_ASN1_UTCTIME:
		if ((ret->value.utctime=
			d2i_ASN1_UTCTIME(NULL,&p,max-p)) == NULL)
			goto err;
		break;
	case V_ASN1_SET:
	case V_ASN1_SEQUENCE:
		if ((ret->value.set=
			d2i_ASN1_bytes(NULL,&p,max-p,tag,V_ASN1_UNIVERSAL))
			== NULL)
			goto err;
		break;
	default:
		ASN1err(ASN1_F_D2I_ASN1_TYPE,ASN1_R_BAD_TYPE);
		goto err;
		}

	ret->type=tag;
	if (a != NULL) (*a)=ret;
	*pp=p;
	return(ret);
err:
	if ((ret != NULL) && (a != NULL) && (*a != ret)) ASN1_TYPE_free(ret);
	return(NULL);
	}

ASN1_TYPE *ASN1_TYPE_new()
	{
	ASN1_TYPE *ret=NULL;

	M_ASN1_New_Malloc(ret,ASN1_TYPE);
	ret->type=-1;
	ret->value.ptr=NULL;
	return(ret);
	M_ASN1_New_Error(ASN1_F_ASN1_TYPE_NEW);
	}

void ASN1_TYPE_free(a)
ASN1_TYPE *a;
	{
	if (a == NULL) return;

	if (a->value.ptr != NULL)
		{
		switch (a->type)
			{
		case V_ASN1_INTEGER:
		case V_ASN1_NEG_INTEGER:
			ASN1_INTEGER_free(a->value.integer);
			break;
		case V_ASN1_BIT_STRING:
			ASN1_BIT_STRING_free(a->value.bit_string);
			break;
		case V_ASN1_OCTET_STRING:
			ASN1_OCTET_STRING_free(a->value.octet_string);
			break;
		case V_ASN1_OBJECT:
			ASN1_OBJECT_free(a->value.object);
			break;
		case V_ASN1_PRINTABLESTRING:
			ASN1_PRINTABLESTRING_free(a->value.printablestring);
			break;
		case V_ASN1_T61STRING:
			ASN1_T61STRING_free(a->value.t61string);
			break;
		case V_ASN1_IA5STRING:
			ASN1_IA5STRING_free(a->value.ia5string);
			break;
		case V_ASN1_UTCTIME:
			ASN1_UTCTIME_free(a->value.utctime);
			break;
		case V_ASN1_SET:
		case V_ASN1_SEQUENCE:
			ASN1_BIT_STRING_free(a->value.set);
		default:
			/* MEMORY LEAK */
			break;
			}
		}
	free((char *)a);
	}

int i2d_ASN1_bytes(a, pp, tag, xclass)
ASN1_BIT_STRING *a;
unsigned char **pp;
int tag;
int xclass;
	{
	int ret,r;
	unsigned char *p;

	if (a == NULL)  return(0);
	ret=a->length;
	r=ASN1_object_size(0,ret,tag);
	if (pp == NULL) return(r);
	p= *pp;

	ASN1_put_object(&p,0,ret,tag,xclass);
	memcpy(p,a->data,a->length);
	p+=a->length;
	*pp= p;
	return(r);
	}

ASN1_BIT_STRING *d2i_ASN1_bytes(a, pp, length, Ptag, Pclass)
ASN1_OCTET_STRING **a;
unsigned char **pp;
long length;
int Ptag;
int Pclass;
	{
	ASN1_BIT_STRING *ret=NULL;
	unsigned char *p,*s;
	long len;
	int inf,tag,xclass;
	int i=0;

	if ((a == NULL) || ((*a) == NULL))
		{
		if ((ret=ASN1_BIT_STRING_new()) == NULL) return(NULL);
		}
	else
		ret=(*a);

	p= *pp;
	inf=ASN1_get_object(&p,&len,&tag,&xclass,length);
	if (inf == 0xff) goto err;

	if (tag != Ptag)
		{
		i=ASN1_R_WRONG_TAG;
		goto err;
		}
	if (len != 0)
		{
		if (ret->length < len)
			{
			if (ret->data != NULL) free(ret->data);
			s=(unsigned char *)malloc((int)len);
			if (s == NULL)
				{
				i=ERR_R_MALLOC_FAILURE;
				goto err;
				}
			}
		else
			s=ret->data;
		memcpy(s,p,(int)len);
		p+=len;
		}
	else
		{
		s=NULL;
		if (ret->data != NULL) free(ret->data);
		}

	ret->length=(int)len;
	ret->data=s;
	ret->type=Ptag;
	if (a != NULL) (*a)=ret;
	*pp=p;
	return(ret);
err:
	if ((ret != NULL) && (a != NULL) && (*a != ret))
		ASN1_BIT_STRING_free(ret);
	ASN1err(ASN1_F_D2I_ASN1_BYTES,i);
	return(NULL);
	}

