/* crypto/asn1/a_set.c */
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

/* ASN1err(ASN1_F_ASN1_TYPE_NEW,ERR_R_MALLOC_FAILURE);
 */

int i2d_ASN1_SET(a,pp,func,ex_tag,ex_class)
STACK *a;
unsigned char **pp;
int (*func)();
int ex_tag;
int ex_class;
	{
	int ret=0,r;
	int i;
	unsigned char *p;

	if (a == NULL) return(0);
	for (i=sk_num(a)-1; i>=0; i--)
		ret+=func(sk_value(a,i),NULL);
	r=ASN1_object_size(1,ret,ex_tag);
	if (pp == NULL) return(r);

	p=*pp;
	ASN1_put_object(&p,1,ret,ex_tag,ex_class);
	for (i=0; i<sk_num(a); i++)
		func(sk_value(a,i),&p);

	*pp=p;
	return(r);
	}

STACK *d2i_ASN1_SET(a,pp,length,func,ex_tag,ex_class)
STACK **a;
unsigned char **pp;
long length;
char *(*func)();
int ex_tag;
int ex_class;
	{
	ASN1_CTX c;
	STACK *ret=NULL;

	if ((a == NULL) || ((*a) == NULL))
		{ if ((ret=sk_new(NULL)) == NULL) goto err; }
	else
		ret=(*a);

	c.p= *pp;
	c.max=(length == 0)?0:(c.p+length);

	c.inf=ASN1_get_object(&c.p,&c.slen,&c.tag,&c.xclass,c.max-c.p);
	if (c.inf == 0xff) goto err;
	if (ex_class != c.xclass)
		{
		ASN1err(ASN1_F_D2I_ASN1_SET,ASN1_R_BAD_CLASS);
		goto err;
		}
	if (ex_tag != c.tag)
		{
		ASN1err(ASN1_F_D2I_ASN1_SET,ASN1_R_BAD_TAG);
		goto err;
		}
	if ((c.slen+c.p) > c.max)
		{
		ASN1err(ASN1_F_D2I_ASN1_SET,ASN1_R_LENGTH_ERROR);
		goto err;
		}
	if (c.inf == 0x21)
		c.slen=length+ *pp-c.p;
	c.max=c.p+c.slen;

	while (c.p < c.max)
		{
		char *s;

		if (M_ASN1_D2I_end_sequence()) break;
		if ((s=func(NULL,&c.p,c.slen,c.max-c.p)) == NULL) goto err;
		if (!sk_push(ret,s)) goto err;
		}
	if (a != NULL) (*a)=ret;
	*pp=c.p;
	return(ret);
err:
	if ((ret != NULL) && (a != NULL) && (*a != ret)) sk_free(ret);
	return(NULL);
	}
