/* crypto/asn1/a_utctm.c */
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
#include <time.h>
#include "cryptlib.h"
#include "asn1.h"

/* ASN1err(ASN1_F_ASN1_UTCTIME_NEW,ASN1_R_UTCTIME_TOO_LONG);
 * ASN1err(ASN1_F_D2I_ASN1_UTCTIME,ASN1_R_EXPECTING_A_UTCTIME);
 */

int i2d_ASN1_UTCTIME(a,pp)
ASN1_UTCTIME *a;
unsigned char **pp;
	{
	int ret,r;
	unsigned char *p;

	if (a == NULL) return(0);
	ret=strlen((char *)a);
	r=ASN1_object_size(0,ret,V_ASN1_UTCTIME);
	if (pp == NULL) return(r);
	p= *pp;

	ASN1_put_object(&p,0,ret,V_ASN1_UTCTIME,V_ASN1_UNIVERSAL);
	memcpy(p,a,(unsigned int)ret);
	p+=ret;
	*pp=p;
	return(r);
	}


ASN1_UTCTIME *d2i_ASN1_UTCTIME(a, pp, length)
ASN1_UTCTIME **a;
unsigned char **pp;
long length;
	{
	ASN1_UTCTIME *ret=NULL;
	unsigned char *p;
	long len;
	int inf,tag,xclass;
	int i;

	if ((a == NULL) || ((*a) == NULL))
		{
		if ((ret=ASN1_UTCTIME_new()) == NULL) return(NULL);
		}
	else
		ret=(*a);

	p= *pp;
	inf=ASN1_get_object(&p,&len,&tag,&xclass,length);
	if (inf == 0xff)
		{
		i=ASN1_R_BAD_OBJECT_HEADER;
		goto err;
		}

	if (tag != V_ASN1_UTCTIME)
		{
		i=ASN1_R_EXPECTING_A_UTCTIME;
		goto err;
		}
	if (len > 31) { i=ASN1_R_UTCTIME_TOO_LONG; goto err; }

	memcpy(ret,p,(int)len);
	ret[len]='\0';
	p+=len;
	if (a != NULL) (*a)=ret;
	*pp=p;
	return(ret);
err:
	ASN1err(ASN1_F_D2I_ASN1_UTCTIME,i);
	if ((ret != NULL) && (a != NULL) && (*a != ret)) ASN1_UTCTIME_free(ret);
	return(NULL);
	}

ASN1_UTCTIME *ASN1_UTCTIME_new()
	{
	ASN1_UTCTIME *ret;

	ret=(ASN1_UTCTIME *)malloc(/*sizeof(ASN1_UTCTIME)*/ 32);
	if (ret == NULL)
		{
		ASN1err(ASN1_F_ASN1_UTCTIME_NEW,ERR_R_MALLOC_FAILURE);
		return(NULL);
		}
	return(ret);
	}

ASN1_UTCTIME *ASN1_UTCTIME_dup(a)
ASN1_UTCTIME *a;
	{
	ASN1_UTCTIME *ret;

	ret=(ASN1_UTCTIME *)malloc(/*sizeof(ASN1_UTCTIME)*/ 32);
	if (ret == NULL)
		{
		ASN1err(ASN1_F_ASN1_UTCTIME_DUP,ERR_R_MALLOC_FAILURE);
		return(NULL);
		}
	strcpy(ret,a);
	return(ret);
	}

void ASN1_UTCTIME_free(a)
ASN1_UTCTIME *a;
	{
	if (a != NULL) free(a);
	}

int ASN1_UTCTIME_check(a)
ASN1_UTCTIME *a;
	{
	static int min[8]={ 0, 1, 1, 0, 0, 0, 0, 0};
	static int max[8]={99,12,31,23,59,59,12,59};
	int n,i,l;

	l=strlen(a);
	if (l < 11) return(0);
	for (i=0; i<6; i++)
		{
		if ((i == 5) && ((*a == 'Z') || (*a == '+') || (*a == '-')))
			{ i++; break; }
		if ((*a < '0') || (*a > '9')) return(0);
		n= *a-'0';
		a++;
		if ((*a < '0') || (*a > '9')) return(0);
		n=(n*10)+ *a-'0';
		a++;
		if ((n < min[i]) || (n > max[i])) return(0);
		}
	if (*a == 'Z')
		a++;
	else if ((*a == '+') || (*a == '-'))
		{
		a++;
		for (i=6; i<8; i++)
			{
			if ((*a < '0') || (*a > '9')) return(0);
			n= *a-'0';
			a++;
			if ((*a < '0') || (*a > '9')) return(0);
			n=(n*10)+ *a-'0';
			a++;
			if ((n < min[i]) || (n > max[i])) return(0);
			if (*a != '\'') return(0);
			a++;
			}
		}
	return(*a == '\0');
	}

ASN1_UTCTIME *ASN1_UTCTIME_set(s, t)
ASN1_UTCTIME *s;
long t;
	{
	struct tm *ts;
#if defined(THREADS) && defined(sun)
	struct tm data;
#endif
#if defined(RISCOS)
        const time_t ctt = (const time_t)t;
#endif

	if (s == NULL)
		s=ASN1_UTCTIME_new();
	if (s == NULL) return(NULL);
#if defined(THREADS) && defined(sun)
	ts=gmtime_r(&t,&data);
#else
#ifdef RISCOS
        ts = gmtime( &ctt );
#else
	ts=gmtime(&t);
#endif
#endif
	sprintf(s,"%02d%02d%02d%02d%02d%02dZ",ts->tm_year%100,
		ts->tm_mon+1,ts->tm_mday,ts->tm_hour,ts->tm_min,ts->tm_sec);
	return(s);
	}
