/* crypto/x509/x509_obj.c */
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
#include "lhash.h"
#include "objects.h"
#include "x509.h"
#include "buffer.h"

char *X509_NAME_oneline(a)
X509_NAME *a;
	{
	X509_NAME_ENTRY *ne;
	unsigned int i;
	int n,lold,l,l1,l2,num,j;
	char *s,*p;
	unsigned char *q;
	BUF_MEM *b=NULL;
	static char hex[17]="0123456789ABCDEF";

	if (a == NULL) return("NO X509_NAME");
	if ((b=BUF_MEM_new()) == NULL) goto err;
	if (!BUF_MEM_grow(b,200)) goto err;

	b->data[0]='\0';
	l=0;
	for (i=0; (int)i<sk_num(a); i++)
		{
		ne=(X509_NAME_ENTRY *)sk_value(a,i);
		n=OBJ_obj2nid(ne->object);
		if (n == NID_undef)
			s="UNKNOWN";
		else
			{
			s=OBJ_nid2sn(n);
			if (s == NULL) s="UNKNOWN2";
			}
		l1=strlen(s);

		num=ne->value->length;
		q=ne->value->data;

		for (l2=j=0; j<num; j++)
			{
			l2++;
			if ((q[j] < ' ') || (q[j] > '~')) l2+=3;
			}
#ifdef undef
		l2=ne->value->length;
#endif
		lold=l;
		l+=1+l1+1+l2;
		if (!BUF_MEM_grow(b,l+1)) goto err;
		p=&(b->data[lold]);
		*(p++)='/';
		memcpy(p,s,(unsigned int)l1); p+=l1;
		*(p++)='=';

		q=ne->value->data;
		for (j=0; j<num; j++)
			{
			n=q[j];
			if ((n < ' ') || (n > '~'))
				{
				*(p++)='\\';
				*(p++)='x';
				*(p++)=hex[(n>>4)&0x0f];
				*(p++)=hex[n&0x0f];
				}
			else
				*(p++)=n;
			}
#ifdef undef
		if (ne->value->data != NULL)
			memcpy(p,ne->value->data,(unsigned int)l2+1);
#endif
		*p='\0';
		}
	p=b->data;
	free(b);
	return(p);
err:
	X509err(X509_F_X509_NAME_ONELINE,ERR_R_MALLOC_FAILURE);
	if (b != NULL) BUF_MEM_free(b);
	return(NULL);
	}
