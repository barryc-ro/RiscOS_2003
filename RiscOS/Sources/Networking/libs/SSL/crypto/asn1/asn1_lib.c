/* crypto/asn1/asn1_lib.c */
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

#ifndef NOPROTO
static int asn1_get_length(unsigned char **pp,int *inf,long *rl,int max);
static void asn1_put_length(unsigned char **pp, int length);
#else
static int asn1_get_length();
static void asn1_put_length();
#endif

char *ASN1_version="ASN1 part of SSLeay 0.6.0 21-Jun-1996";

int ASN1_check_infinite_end(p,len)
unsigned char **p;
long len;
	{
	if (((*p)[0] == 0) && ((*p)[1] == 0))
		{
		(*p)+=2;
		return(1);
		}
	else if (len <= 0)
		return(1);
	return(0);
	}


int ASN1_get_object(pp, plength, ptag, pclass, omax)
unsigned char **pp;
long *plength;
int *ptag;
int *pclass;
long omax;
	{
	int i,ret;
	long l;
	unsigned char *p= *pp;
	int tag,xclass,inf;
	long max=omax;

	if (!max) goto err;
	ret=(*p&V_ASN1_CONSTRUCTED);
	xclass=(*p&V_ASN1_PRIVATE);
	i= *p&V_ASN1_PRIMATIVE_TAG;
	if (i == V_ASN1_PRIMATIVE_TAG)
		{		/* high-tag */
		p++;
		if (--max == 0) goto err;
		l=0;
		while (*p&0x80)
			{
			l<<=7L;
			l|= *(p++)&0x7f;
			if (--max == 0) goto err;
			}
		l<<=7L;
		l|= *(p++)&0x7f;
		tag=(int)l;
		}
	else
		{ /* constructed */
		tag=i;
		p++;
		if (--max == 0) goto err;
		}
	*ptag=tag;
	*pclass=xclass;
	if (!asn1_get_length(&p,&inf,plength,(int)max)) goto err;
	if ((p+ *plength) > (omax+ *pp))
		{
		ASN1err(ASN1_F_ASN1_GET_OBJECT,ASN1_R_TOO_LONG);
		return(0xff);
		}
	*pp=p;
	return(ret+inf);
err:
	ASN1err(ASN1_F_ASN1_GET_OBJECT,ASN1_R_HEADER_TOO_LONG);
	return(0xff);
	}

static int asn1_get_length(pp,inf,rl,max)
unsigned char **pp;
int *inf;
long *rl;
int max;
	{
	unsigned char *p= *pp;
	long ret=0;
	int i;

	if (max-- < 1) return(0);
	if (*p == 0x80)
		{
		*inf=1;
		ret=0;
		p++;
		}
	else
		{
		*inf=0;
		i= *p&0x7f;
		if (*(p++) & 0x80)
			{
			if (max-- == 0) return(0);
			while (i-- > 0)
				{
				ret<<=8L;
				ret|= *(p++);
				if (max-- == 0) return(0);
				}
			}
		else
			ret=i;
		}
	*pp=p;
	*rl=ret;
	return(1);
	}

/* class 0 is constructed
 * constructed == 2 for indefinitle length constructed */
void ASN1_put_object(pp,constructed,length,tag,xclass)
unsigned char **pp;
int constructed;
int length;
int tag;
int xclass;
	{
	unsigned char *p= *pp;
	int i;

	i=(constructed)?V_ASN1_CONSTRUCTED:0;
	i|=(xclass&V_ASN1_PRIVATE);
	if (tag < 31)
		*(p++)=i|(tag&V_ASN1_PRIMATIVE_TAG);
	else
		{
		*(p++)=i|V_ASN1_PRIMATIVE_TAG;
		while (tag > 0x3f)
			{
			*(p++)=(tag&0x3f)|0x80;
			tag>>=7;
			}
		*(p++)=(tag&0x3f);
		}
	if ((constructed == 2) && (length == 0))
		*(p++)=0x80; /* der_put_length would output 0 instead */
	else
		asn1_put_length(&p,length);
	*pp=p;
	}

static void asn1_put_length(pp, length)
unsigned char **pp;
int length;
	{
	unsigned char *p= *pp;
	int i,l;
	if (length <= 127)
		*(p++)=(unsigned char)length;
	else
		{
		l=length;
		for (i=0; l > 0; i++)
			l>>=8;
		*(p++)=i|0x80;
		l=i;
		while (i-- > 0)
			{
			p[i]=length&0xff;
			length>>=8;
			}
		p+=l;
		}
	*pp=p;
	}

int ASN1_object_size(constructed, length, tag)
int constructed;
int length;
int tag;
	{
	int ret;

	ret=length;
	ret++;
	if (tag >= 31)
		{
		while (tag > 0)
			{
			tag>>=7;
			ret++;
			}
		}
	if ((length == 0) && (constructed == 2))
		ret+=2;
	ret++;
	if (length > 127)
		{
		while (length > 0)
			{
			length>>=8;
			ret++;
			}
		}
	return(ret);
	}

int asn1_Finish(c)
ASN1_CTX *c;
	{
	if ((c->inf == 0x21) && (!c->eos))
		{
		if (!ASN1_check_infinite_end(&c->p,c->slen))
			{
			c->error=ASN1_R_MISSING_EOS;
			return(0);
			}
		}
	if (	((c->slen != 0) && !(c->inf & 1)) ||
		((c->slen < 0) && (c->inf & 1)))
		{
		c->error=ASN1_R_LENGTH_MISMATCH;
		return(0);
		}
	return(1);
	}

int asn1_GetSequence(c,length)
ASN1_CTX *c;
long *length;
	{
	unsigned char *q;

	q=c->p;
	c->inf=ASN1_get_object(&(c->p),&(c->slen),&(c->tag),&(c->xclass),
		*length);
	if (c->inf == 0xff)
		{
		c->error=ASN1_R_BAD_GET_OBJECT;
		return(0);
		}
	if (c->tag != V_ASN1_SEQUENCE)
		{
		c->error=ASN1_R_EXPECTING_A_SEQUENCE;
		return(0);
		}
	(*length)-=(c->p-q);
	if (c->max && (*length < 0))
		{
		c->error=ASN1_R_LENGTH_MISMATCH;
		return(0);
		}
	if (c->inf == 0x21)
		c->slen= *length+ *(c->pp)-c->p;
	c->eos=0;
	return(1);
	}

