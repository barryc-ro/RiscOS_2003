/* crypto/asn1/a_object.c */
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
#include "asn1.h"
#include "objects.h"

/* ASN1err(ASN1_F_ASN1_OBJECT_NEW,ASN1_R_EXPECTING_AN_OBJECT); 
 * ASN1err(ASN1_F_D2I_ASN1_OBJECT,ASN1_R_BAD_OBJECT_HEADER); 
 * ASN1err(ASN1_F_I2A_ASN1_OBJECT,ASN1_R_BAD_OBJECT_HEADER);
 */

int i2d_ASN1_OBJECT(a, pp)
ASN1_OBJECT *a;
unsigned char **pp;
	{
	unsigned char *p;

	if ((a == NULL) || (a->data == NULL)) return(0);

	if (pp == NULL)
		return(ASN1_object_size(0,a->length,V_ASN1_OBJECT));

	p= *pp;
	ASN1_put_object(&p,0,a->length,V_ASN1_OBJECT,V_ASN1_UNIVERSAL);
	memcpy(p,a->data,a->length);
	p+=a->length;

	*pp=p;
	return(a->length);
	}

int i2a_ASN1_OBJECT(bp,a)
BIO *bp;
ASN1_OBJECT *a;
	{
	char buf[32];
	int j,i,n=0,len,nid,reason=ERR_R_BUF_LIB;
	unsigned long l;
	unsigned char *p;
	char *s;

	if ((a == NULL) || (a->data == NULL))
		{
		return(BIO_write(bp,"NULL",4));
		}

	nid=OBJ_obj2nid(a);
	if (nid == NID_undef)
		{
		len=a->length;
		p=a->data;
		if (len >= 1)
			{
			sprintf(buf,"%d %d",(unsigned int)p[0]/40,
				(unsigned int)p[0]%40);
			j=strlen(buf);
			if (BIO_write(bp,buf,j) != j) goto err;
			n+=((((int)(p[0]%40)) > 9)?2:1)+2;
			}
		p++;
		l=0;
		for (i=1; i<len; i++)
			{
			l=(l<<7L)|(*p&0x7f);
			if (!(*(p++) & 0x80))
				{
				sprintf(buf," %ld",l);
				j=strlen(buf);
				if (BIO_write(bp,buf,j) != j) goto err;
				n++;
				if (l == 0)
					n++;
				else
					{
					while (l)
						{
						l/=10;
						n++;
						}
					}
				l=0;
				}
			}
		}
	else
		{
		s=(char *)OBJ_nid2ln(nid);
		if (s == NULL)
			s=(char *)OBJ_nid2sn(nid);
		j=strlen(s);
		if (BIO_write(bp,s,j) != j) goto err;
		n=j;
		}
	return(n);
err:
	ASN1err(ASN1_F_I2A_ASN1_OBJECT,reason);
	return(-1);
	}

ASN1_OBJECT *d2i_ASN1_OBJECT(a, pp, length)
ASN1_OBJECT **a;
unsigned char **pp;
long length; 
	{
	ASN1_OBJECT *ret=NULL;
	unsigned char *p;
	long len;
	int tag,xclass;
	int inf,i;

	/* only the ASN1_OBJECTs from the 'table' will have values
	 * for ->sn or ->ln */
	if ((a == NULL) || ((*a) == NULL) ||
		((*a)->sn != NULL) || ((*a)->ln != NULL))
		{
		if ((ret=ASN1_OBJECT_new()) == NULL) return(NULL);
		}
	else	ret=(*a);

	p= *pp;

	inf=ASN1_get_object(&p,&len,&tag,&xclass,length);
	if (inf == 0xff)
		{
		i=ASN1_R_BAD_OBJECT_HEADER;
		goto err;
		}

	if (tag != V_ASN1_OBJECT)
		{
		i=ASN1_R_EXPECTING_AN_OBJECT;
		goto err;
		}
	ret->data=(unsigned char *)malloc((int)len);
	if (ret->data == NULL)
		{ i=ERR_R_MALLOC_FAILURE; goto err; }
	memcpy(ret->data,p,(int)len);
	ret->length=(int)len;
	ret->sn=NULL;
	ret->ln=NULL;
	p+=len;

	if (a != NULL) (*a)=ret;
	*pp=p;
	return(ret);
err:
	ASN1err(ASN1_F_D2I_ASN1_OBJECT,i);
	if ((ret != NULL) && (a != NULL) && (*a != ret))
		ASN1_OBJECT_free(ret);
	return(NULL);
	}

ASN1_OBJECT *ASN1_OBJECT_new()
	{
	ASN1_OBJECT *ret;

	ret=(ASN1_OBJECT *)malloc(sizeof(ASN1_OBJECT));
	if (ret == NULL)
		{
		ASN1err(ASN1_F_ASN1_OBJECT_NEW,ERR_R_MALLOC_FAILURE);
		return(NULL);
		}
	ret->length=0;
	ret->data=NULL;
	ret->nid=0;
	ret->sn=NULL;
	ret->ln=NULL;
	return(ret);
	}

void ASN1_OBJECT_free(a)
ASN1_OBJECT *a;
	{
	if (a == NULL) return;
	if ((a->sn != NULL) || (a->ln != NULL)) return;
	if (a->data != NULL) free(a->data);
	free(a);
	}

