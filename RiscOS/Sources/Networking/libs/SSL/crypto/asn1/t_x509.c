/* crypto/asn1/t_x509.c */
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
#include "bn.h"
#include "rsa.h"
#include "dsa.h"
#include "objects.h"
#include "x509.h"

#ifndef WIN16
int X509_print_fp(fp,x)
FILE *fp;
X509 *x;
        {
        BIO *b;
        int ret;

        if ((b=BIO_new(BIO_s_file())) == NULL)
		{
		X509err(X509_F_X509_PRINT_FP,ERR_R_BUF_LIB);
                return(0);
		}
        BIO_set_fp(b,fp,BIO_NOCLOSE);
        ret=X509_print(b, x);
        BIO_free(b);
        return(ret);
        }
#endif

int X509_print(bp,x)
BIO *bp;
X509 *x;
	{
	char str[128];
	unsigned long l;
	int ret=0,i,n,len;
	char *m=NULL,*s;
	X509_CINF *ci;
	ASN1_BIT_STRING *bs;
	RSA *rsa=NULL;
	DSA *dsa=NULL;
	char *neg;

	ci=x->cert_info;
	if (BIO_puts(bp,"Certificate:\n") <= 0) goto err;
	if (BIO_puts(bp,"    Data:\n") <= 0) goto err;
	bs=ci->version;
	if ((bs == NULL) || (bs->data == NULL))
		l=0;
	else
		for (l=i=0; i<bs->length; i++)
			{ l<<=8; l+=bs->data[i]; }

	sprintf(str,"%8sVersion: %lu (0x%lx)\n","",l,l);
	if (BIO_puts(bp,str) <= 0) goto err;
	if (BIO_puts(bp,"        Serial Number:") <= 0) goto err;

	bs=ci->serialNumber;
	if (bs->length <= 4)
		{
		neg=(ci->serialNumber->type == V_ASN1_NEG_INTEGER)?"-":"";
		for (l=i=0; i<bs->length; i++)
			{ l<<=8; l+=bs->data[i]; }
		sprintf(str," %s%lu (%s0x%lx)\n",neg,l,neg,l);
		if (BIO_puts(bp,str) <= 0) goto err;
		}
	else
		{
		neg=(ci->serialNumber->type == V_ASN1_NEG_INTEGER)?" (Negative)":"";
		sprintf(str,"\n%12s%s","",neg);
		if (BIO_puts(bp,str) <= 0) goto err;

		for (i=0; i<bs->length; i++)
			{
			sprintf(str,"%02x%c",bs->data[i],
				((i+1 == bs->length)?'\n':':'));
			if (BIO_puts(bp,str) <= 0) goto err;
			}
		}

	i=OBJ_obj2nid(ci->signature->algorithm);
	sprintf(str,"%8sSignature Algorithm: %s\n","",
		(i == NID_undef)?"UNKNOWN":OBJ_nid2ln(i));
	if (BIO_puts(bp,str) <= 0) goto err;

	if (BIO_puts(bp,"        Issuer: ") <= 0) goto err;
	if (!X509_NAME_print(bp,ci->issuer,16)) goto err;
	if (BIO_puts(bp,"\n        Validity\n") <= 0) goto err;
	if (BIO_puts(bp,"            Not Before: ") <= 0) goto err;
	if (!ASN1_UTCTIME_print(bp,ci->validity->notBefore)) goto err;
	if (BIO_puts(bp,"\n            Not After : ") <= 0) goto err;
	if (!ASN1_UTCTIME_print(bp,ci->validity->notAfter)) goto err;
	if (BIO_puts(bp,"\n        Subject: ") <= 0) goto err;
	if (!X509_NAME_print(bp,ci->subject,16)) goto err;
	if (BIO_puts(bp,"\n        Subject Public Key Info:\n") <= 0) goto err;
	i=OBJ_obj2nid(ci->key->algor->algorithm);
	sprintf(str,"%12sPublic Key Algorithm: %s\n","",
		(i == NID_undef)?"UNKNOWN":OBJ_nid2ln(i));
	if (BIO_puts(bp,str) <= 0) goto err;

	s=(char *)ci->key->public_key->data;
	len=ci->key->public_key->length;
	switch (i)
		{
	case NID_rsaEncryption:
	case NID_rsa:
		if (d2i_RSAPublicKey(&rsa,(unsigned char **)&s, len) == NULL)
			goto err;
		RSA_print(bp,rsa,16);
		break;
	case NID_dss:
	case NID_dsaWithSHA:
	case NID_dsaWithSHA1:
		if (d2i_DSAPublicKey(&dsa,(unsigned char **)&s, len) == NULL)
			goto err;
		DSA_print(bp,dsa,16);
		break;
	default:
		sprintf(str,"%16sUnable to print this type of key\n","");
		BIO_puts(bp,str);
		}

	i=OBJ_obj2nid(x->sig_alg->algorithm);
	sprintf(str,"%4sSignature Algorithm: %s","",
		(i == NID_undef)?"UNKNOWN":OBJ_nid2ln(i));
	if (BIO_puts(bp,str) <= 0) goto err;

	n=x->signature->length;
	s=(char *)x->signature->data;
	for (i=0; i<n; i++)
		{
		if ((i%18) == 0)
			if (BIO_puts(bp,"\n        ") <= 0) goto err;
		sprintf(str,"%02x%s",(unsigned char)s[i],((i+1) == n)?"":":");
		if (BIO_puts(bp,str) <= 0) goto err;
		}
	if (BIO_write(bp,"\n",1) != 1) goto err;
	ret=1;
err:
	if (rsa != NULL) RSA_free(rsa);
	if (dsa != NULL) DSA_free(dsa);
	if (m != NULL) free(m);
	return(ret);
	}

int ASN1_UTCTIME_print(bp,v)
BIO *bp;
ASN1_UTCTIME *v;
	{
	int gmt=0;
	char str[128];
	static char *mon[12]={
		"Jan","Feb","Mar","Apr","May","Jun",
		"Jul","Aug","Sep","Oct","Nov","Dec"};
	int i;
	int y=0,M=0,d=0,h=0,m=0,s=0;

	i=strlen(v);
	if (i < 10) goto err;
	if (v[i-1] == 'Z') gmt=1;
	for (i=0; i<10; i++)
		if ((v[i] > '9') || (v[i] < '0')) goto err;
	y= (v[0]-'0')*10+(v[1]-'0');
	if (y < 70) y+=100;
	M= (v[2]-'0')*10+(v[3]-'0');
	if ((M > 12) || (M < 1)) goto err;
	d= (v[4]-'0')*10+(v[5]-'0');
	h= (v[6]-'0')*10+(v[7]-'0');
	m=  (v[8]-'0')*10+(v[9]-'0');
	if (	(v[10] >= '0') && (v[10] <= '9') &&
		(v[11] >= '0') && (v[11] <= '9'))
		s=  (v[10]-'0')*10+(v[11]-'0');

	sprintf(str,"%s %2d %02d:%02d:%02d %d%s",
		mon[M-1],d,h,m,s,y+1900,(gmt)?" GMT":"");
	if (BIO_puts(bp,str) <= 0)
		return(0);
	else
		return(1);
err:
	BIO_puts(bp,"Bad time value");
	return(1);
	}

int X509_NAME_print(bp,name,obase)
BIO *bp;
X509_NAME *name;
int obase;
	{
	char *m=NULL,*s,*c;
	int ret=0,l,ll,i,first=1;

	ll=80-2-obase;

	s=m=X509_NAME_oneline(name);
	if (s == NULL)
		{
		X509err(X509_F_X509_NAME_PRINT,ERR_R_X509_LIB);
		return(0);
		}
	s++; /* skip the first slash */

	l=ll;
	c=s;
	for (;;)
		{
		if ((*s == '/') || (*s == '\0'))
			{
			if ((l <= 0) && !first)
				{
				first=0;
				if (BIO_write(bp,"\n",1) != 1) goto err;
				for (i=0; i<obase; i++)
					{
					if (BIO_write(bp," ",1) != 1) goto err;
					}
				l=ll;
				}
			i=s-c;
			if (BIO_write(bp,c,i) != i) goto err;
			c+=i;
			c++;
			if (*s != '\0')
				{
				if (BIO_write(bp,", ",2) != 2) goto err;
				}
			l--;
			}
		if (*s == '\0') break;
		s++;
		l--;
		}
	
	ret=1;
	if (0)
		{
err:
		X509err(X509_F_X509_NAME_PRINT,ERR_R_BUF_LIB);
		}
	if (m != NULL) free(m);
	return(ret);
	}

