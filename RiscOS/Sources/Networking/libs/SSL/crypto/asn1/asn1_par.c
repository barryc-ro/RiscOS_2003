/* crypto/asn1/asn1_par.c */
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
#include "x509.h"

#ifndef NOPROTO
static int asn1_print_info(BIO *bp, int tag, int xclass,int constructed);
static int asn1_parse2(BIO *bp, unsigned char **pp, long length,
	int offset, int depth);
#else
static int asn1_print_info();
static int asn1_parse2();
#endif

static int asn1_print_info(bp, tag, xclass, constructed)
BIO *bp;
int tag;
int xclass;
int constructed;
	{
	static char *fmt="%-18s";
	static char *fmt2="%2d %-15s";
	char *p,str[128],*p2=NULL;

	if (constructed & V_ASN1_CONSTRUCTED)
		p="cons: ";
	else
		p="prim: ";
	if (BIO_puts(bp,p) <= 0) goto err;
	p=str;
	if ((xclass & V_ASN1_PRIVATE) == V_ASN1_PRIVATE)
		sprintf(str,"priv [ %d ] ",tag);
	else if ((xclass & V_ASN1_CONTEXT_SPECIFIC) == V_ASN1_CONTEXT_SPECIFIC)
		sprintf(str,"cont [ %d ]",tag);
	else if ((xclass & V_ASN1_APPLICATION) == V_ASN1_APPLICATION)
		sprintf(str,"appl [ %d ]",tag);
	else if ((tag == V_ASN1_EOC) /* && (xclass == V_ASN1_UNIVERSAL) */)
		p="EOC";
	else if (tag == V_ASN1_INTEGER)
		p="INTEGER";
	else if (tag == V_ASN1_BIT_STRING)
		p="BIT STRING";
	else if (tag == V_ASN1_OCTET_STRING)
		p="OCTET STRING";
	else if (tag == V_ASN1_NULL)
		p="NULL";
	else if (tag == V_ASN1_OBJECT)
		p="OBJECT";
	else if (tag == V_ASN1_SEQUENCE)
		p="SEQUENCE";
	else if (tag == V_ASN1_SET)
		p="SET";
	else if (tag == V_ASN1_PRINTABLESTRING)
		p="PRINTABLESTRING";
	else if (tag == V_ASN1_T61STRING)
		p="T61STRING";
	else if (tag == V_ASN1_IA5STRING)
		p="IA5STRING";
	else if (tag == V_ASN1_UTCTIME)
		p="UTCTIME";

	/* extras */
	else if (tag == V_ASN1_NUMERICSTRING)
		p="NUMERICSTRING";
	else if (tag == V_ASN1_VIDEOTEXSTRING)
		p="VIDEOTEXSTRING";
	else if (tag == V_ASN1_GENERALIZEDTIME)
		p="GENERALIZEDTIME";
	else if (tag == V_ASN1_GRAPHICSTRING)
		p="GRAPHICSTRING";
	else if (tag == V_ASN1_ISO64STRING)
		p="ISO64STRING";
	else if (tag == V_ASN1_GENERALSTRING)
		p="GENERALSTRING";
	else if (tag == V_ASN1_UNIVERSALSTRING)
		p="UNIVERSALSTRING";

	else
		p2="(unknown)";
		
	if (p2 != NULL)
		sprintf(str,fmt2,tag,p2);
	else
		sprintf(str,fmt,p);
	if (BIO_puts(bp,str) <= 0) goto err;
	return(1);
err:
	return(0);
	}

int ASN1_parse(bp, pp, len)
BIO *bp;
unsigned char *pp;
long len;
	{
	return(asn1_parse2(bp,&pp,len,0,0));
	}

static int asn1_parse2(bp, pp, length, offset, depth)
BIO *bp;
unsigned char **pp;
long length;
int offset;
int depth;
	{
	char str[128];
	unsigned char *p,*ep,*tot,*op,*opp;
	long len;
	int tag,xclass,ret=0;
	int nl,hl,j,r;
	ASN1_OBJECT *o=NULL;

	p= *pp;
	tot=p+length;
	op=p-1;
	while ((p < tot) && (op < p))
		{
		op=p;
		j=ASN1_get_object(&p,&len,&tag,&xclass,length);
#ifdef LINT
		j=j;
#endif
		if (j == 0xff)
			{
			if (BIO_puts(bp,"Error in encoding\n") <= 0) goto end;
			ret=0;
			goto end;
			}
		hl=(p-op);
		/* if j == 0x21 it is a constructed indefinite length object */
		sprintf(str,"%5ld:",(long)offset+(long)(op- *pp));
		if (BIO_puts(bp,str) <= 0) goto end;

		if (j != 0x21)
			{
			sprintf(str,"d=%-2d hl=%ld l=%4ld ",depth,(long)hl,len);
			if (BIO_puts(bp,str) <= 0) goto end;
			}
		else
			{
			sprintf(str,"d=%-2d hl=%ld l=inf  ",depth,(long)hl);
			if (BIO_puts(bp,str) <= 0) goto end;
			}
		if (!asn1_print_info(bp,tag,xclass,j)) goto end;
		if (j & V_ASN1_CONSTRUCTED)
			{
			ep=p+len;
			if (BIO_puts(bp,"\n") <= 0) goto end;
			if (len > length)
				{
				sprintf(str,"length is greater than %ld\n",
					length);
				if (BIO_puts(bp,str) <= 0) goto end;
				ret=0;
				goto end;
				}
			if ((j == 0x21) && (len == 0))
				{
				for (;;)
					{
					r=asn1_parse2(bp,&p,(long)(tot-p),
						offset+(p - *pp),depth+1);
					if (r == 0) { ret=0; goto end; }
					if ((r == 2) || (p >= tot)) break;
					}
				}
			else
				while (p < ep)
					{
					r=asn1_parse2(bp,&p,(long)len,
						offset+(p - *pp),depth+1);
					if (r == 0) { ret=0; goto end; }
					}
			}
		else if (xclass != 0)
			{
			p+=len;
			if (BIO_puts(bp,"\n") <= 0) goto end;
			}
		else
			{
			nl=0;
			if (	(tag == V_ASN1_PRINTABLESTRING) ||
				(tag == V_ASN1_T61STRING) ||
				(tag == V_ASN1_IA5STRING) ||
				(tag == V_ASN1_UTCTIME))
				{
				if (BIO_puts(bp,":") <= 0) goto end;
				if (BIO_write(bp,(char *)p,(int)len)
					!= (int)len)
					goto end;
				}
			else if (tag == V_ASN1_OBJECT)
				{
				opp=op;
				if (d2i_ASN1_OBJECT(&o,&opp,len+hl) != NULL)
					{
					if (BIO_puts(bp,":") <= 0) goto end;
					i2a_ASN1_OBJECT(bp,o);
					}
				else
					{
					if (BIO_puts(bp,":BAD OBJECT") <= 0)
						goto end;
					}
				}
			else if (tag == V_ASN1_INTEGER)
				{
				ASN1_INTEGER *bs;
				int i;

				opp=op;
				bs=d2i_ASN1_INTEGER(NULL,&opp,len+hl);
				if (bs != NULL)
					{
					if (BIO_puts(bp,":") <= 0) goto end;
					if (bs->type == V_ASN1_NEG_INTEGER)
						if (BIO_puts(bp,"-") <= 0)
							goto end;
					for (i=0; i<bs->length; i++)
						{
						sprintf(str,"%02X",bs->data[i]);
						if (BIO_puts(bp,str) <= 0)
							goto end;
						}
					if (bs->length == 0)
						{
						if (BIO_puts(bp,"00") <= 0)
							goto end;
						}
					}
				else
					{
					if (BIO_puts(bp,"BAD INTEGER") <= 0)
						goto end;
					}
				ASN1_INTEGER_free(bs);
				}

			if (!nl) 
				{
				if (BIO_puts(bp,"\n") <= 0) goto end;
				}
			p+=len;
			if ((tag == V_ASN1_EOC) && (xclass == 0))
				{
				ret=2; /* End of sequence */
				goto end;
				}
			}
		}
	ret=1;
end:
	if (o != NULL) ASN1_OBJECT_free(o);
	*pp=p;
	return(ret);
	}
