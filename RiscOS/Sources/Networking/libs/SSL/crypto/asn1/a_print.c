/* crypto/asn1/a_print.c */
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
#include "asn1.h"

/* ASN1err(ASN1_F_D2I_ASN1_PRINT_TYPE,ASN1_R_WRONG_PRINTABLE_TYPE);
 * ASN1err(ASN1_F_D2I_ASN1_PRINT_TYPE,ASN1_R_TAG_VALUE_TOO_HIGH);
 */

static unsigned long tag2bit[32]={
0,	0,	0,	0,	/* tags  0 -  3 */
0,	0,	0,	0, 	/* tags  4 -  7 */
0,	0,	0,	0, 	/* tags  8 - 11 */
0,	0,	0,	0, 	/* tags 12 - 15 */
0,	0,	B_ASN1_NUMERICSTRING,B_ASN1_PRINTABLESTRING,
B_ASN1_T61STRING,B_ASN1_VIDEOTEXSTRING,B_ASN1_IA5STRING,0,
0,B_ASN1_GRAPHICSTRING,B_ASN1_ISO64STRING,B_ASN1_GENERALSTRING,
B_ASN1_UNIVERSALSTRING,0,0,0,
	};

int i2d_ASN1_IA5STRING(a,pp)
ASN1_BIT_STRING *a;
unsigned char **pp;
	{ return(M_i2d_ASN1_IA5STRING(a,pp)); }

ASN1_IA5STRING *d2i_ASN1_IA5STRING(a,pp,l)
ASN1_BIT_STRING **a;
unsigned char **pp;
long l;
	{ return(M_d2i_ASN1_IA5STRING(a,pp,l)); }

ASN1_T61STRING *d2i_ASN1_T61STRING(a,pp,l)
ASN1_BIT_STRING **a;
unsigned char **pp;
long l;
	{ return(M_d2i_ASN1_T61STRING(a,pp,l)); }

ASN1_PRINTABLESTRING *d2i_ASN1_PRINTABLESTRING(a,pp,l)
ASN1_BIT_STRING **a;
unsigned char **pp;
long l;
	{ return(M_d2i_ASN1_PRINTABLESTRING(a,pp,l)); }

int i2d_ASN1_PRINTABLE(a,pp)
ASN1_BIT_STRING *a;
unsigned char **pp;
	{ return(M_i2d_ASN1_PRINTABLE(a,pp)); }

ASN1_BIT_STRING *d2i_ASN1_PRINTABLE(a,pp,l)
ASN1_BIT_STRING **a;
unsigned char **pp;
long l;
	{ return(M_d2i_ASN1_PRINTABLE(a,pp,l)); }

/* type is a 'bitmap' of acceptable string types to be accepted.
 */
ASN1_BIT_STRING *d2i_asn1_print_type(a, pp, length, type)
ASN1_BIT_STRING **a;
unsigned char **pp;
long length;
int type;
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

	if (tag >= 32)
		{
		i=ASN1_R_TAG_VALUE_TOO_HIGH;;
		goto err;
		}
#ifdef undef
	if (((type != 0) && (tag != type)) ||
		((type == 0) &&
			!((tag == V_ASN1_PRINTABLESTRING) ||
			  (tag == V_ASN1_T61STRING) ||
			  (tag == V_ASN1_IA5STRING))))
		{
		i=ASN1_R_EXPECTING_A_PRINTABLE;
		goto err;
		}
#endif
	if (!(tag2bit[tag] & type))
		{
		i=ASN1_R_WRONG_PRINTABLE_TYPE;
		goto err;
		}
	if (len != 0)
		{
		s=(unsigned char *)malloc((int)len+1);
		if (s == NULL)
			{
			i=ERR_R_MALLOC_FAILURE;
			goto err;
			}
		memcpy(s,p,(int)len);
		s[len]='\0';
		p+=len;
		}
	else
		s=NULL;

	if (ret->data != NULL) free(ret->data);
	ret->length=(int)len;
	ret->data=s;
	ret->type=tag;
	if (a != NULL) (*a)=ret;
	*pp=p;
	return(ret);
err:
	ASN1err(ASN1_F_D2I_ASN1_PRINT_TYPE,i);
	if ((ret != NULL) && (a != NULL) && (*a != ret))
		ASN1_BIT_STRING_free(ret);
	return(NULL);
	}

int ASN1_PRINTABLE_type(s)
unsigned char *s;
	{
	int c;
	int ia5=0;
	int t61=0;

	while (*s)
		{
		c= *(s++);
		if (!(	((c >= 'a') && (c <= 'z')) ||
			((c >= 'A') && (c <= 'Z')) ||
			(c == ' ') ||
			((c >= '0') && (c <= '9')) ||
			(c == ' ') || (c == '\'') ||
			(c == '(') || (c == ')') ||
			(c == '+') || (c == ',') ||
			(c == '-') || (c == '.') ||
			(c == '/') || (c == ':') ||
			(c == '=') || (c == '?')))
			ia5=1;
		if (c&0x80)
			t61=1;
		}
	if (t61) return(V_ASN1_T61STRING);
	if (ia5) return(V_ASN1_IA5STRING);
	return(V_ASN1_PRINTABLESTRING);
	}
