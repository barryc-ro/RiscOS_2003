/* crypto/asn1/t_pkey.c */
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
#include "dh.h"
#include "rsa.h"
#include "dsa.h"

/* DHerr(DH_F_DHPARAMS_PRINT,ERR_R_MALLOC_FAILURE);
 */

#ifndef NOPROTO
static int print(BIO *fp,char *str,BIGNUM *num,
		unsigned char *buf,int off);
#else
static int print();
#endif

#ifndef WIN16
int RSA_print_fp(fp,x,off)
FILE *fp;
RSA *x;
int off;
        {
        BIO *b;
        int ret;

        if ((b=BIO_new(BIO_s_file())) == NULL)
		{
		X509err(RSA_F_RSA_PRINT_FP,ERR_R_BUF_LIB);
                return(0);
		}
        BIO_set_fp(b,fp,BIO_NOCLOSE);
        ret=RSA_print(b,x,off);
        BIO_free(b);
        return(ret);
        }
#endif

int RSA_print(bp,x,off)
BIO *bp;
RSA *x;
int off;
	{
	char str[128],*s;
	unsigned char *m=NULL;
	int i,ret=0;

	i=RSA_size(x);
	m=(unsigned char *)malloc((unsigned int)i+10);
	if (m == NULL)
		{
		RSAerr(RSA_F_RSA_PRINT,ERR_R_MALLOC_FAILURE);
		goto err;
		}

	if (off)
		{
		if (off > 128) off=128;
		memset(str,' ',off);
		}
	if (x->d != NULL)
		{
		if (BIO_write(bp,str,off) <= 0) return(0);
		sprintf(str,"Private-Key: (%d bit)\n",BN_num_bits(x->n));
		}

	s=(x->d == NULL)?"Modulus:":"modulus:";
	if (!print(bp,s,x->n,m,off)) goto err;
	s=(x->d == NULL)?"Exponent:":"publicExponent:";
	if (!print(bp,s,x->e,m,off)) goto err;
	if (!print(bp,"privateExponent:",x->d,m,off)) goto err;
	if (!print(bp,"prime1:",x->p,m,off)) goto err;
	if (!print(bp,"prime2:",x->q,m,off)) goto err;
	if (!print(bp,"exponent1:",x->dmp1,m,off)) goto err;
	if (!print(bp,"exponent2:",x->dmq1,m,off)) goto err;
	if (!print(bp,"coefficient:",x->iqmp,m,off)) goto err;
	ret=1;
err:
	if (m != NULL) free(m);
	return(ret);
	}

int DSA_print(bp,x,off)
BIO *bp;
DSA *x;
int off;
	{
	char str[128];
	unsigned char *m=NULL;
	int i,ret=0;

	/* larger than needed but what the hell :-) */
	i=BN_num_bytes(x->g)*2;
	m=(unsigned char *)malloc((unsigned int)i+10);
	if (m == NULL)
		{
		DSAerr(DSA_F_DSA_PRINT,ERR_R_MALLOC_FAILURE);
		goto err;
		}

	if (off)
		{
		if (off > 128) off=128;
		memset(str,' ',off);
		}
	if (x->x != NULL)
		{
		if (BIO_write(bp,str,off) <= 0) return(0);
		sprintf(str,"Private-Key: (%d bit)\n",BN_num_bits(x->g));
		}

	if (!print(bp,"X:",x->x,m,off)) goto err;
	if (!print(bp,"Y:",x->y,m,off)) goto err;
	if (!print(bp,"P:",x->p,m,off)) goto err;
	if (!print(bp,"Q:",x->q,m,off)) goto err;
	if (!print(bp,"G:",x->g,m,off)) goto err;
	ret=1;
err:
	if (m != NULL) free(m);
	return(ret);
	}

static int print(bp,number,num,buf,off)
BIO *bp;
char *number;
BIGNUM *num;
unsigned char *buf;
int off;
	{
	int n,i;
	char *neg;
	char str[128];

	if (num == NULL) return(1);
	neg=(num->neg)?"-":"";
	if (off)
		{
		if (off > 128) off=128;
		memset(str,' ',off);
		if (BIO_write(bp,str,off) <= 0) return(0);
		}

	if (BN_num_bytes(num) <= BN_BYTES)
		{
		sprintf(str,"%s %s%lu (%s0x%lx)\n",number,neg,
			(unsigned long)num->d[0],neg,(unsigned long)num->d[0]);
		if (BIO_puts(bp,str) <= 0) return(0);
		}
	else
		{
		buf[0]=0;
		sprintf(str,"%s%s",number,(neg[0] == '-')?" (Negative)":"");
		if (BIO_puts(bp,str) <= 0) return(0);
		n=BN_bn2bin(num,&buf[1]);
	
		if (buf[1] & 0x80)
			n++;
		else	buf++;

		for (i=0; i<n; i++)
			{
			if ((i%15) == 0)
				{
				str[0]='\n';
				memset(&(str[1]),' ',off+4);
				if (BIO_write(bp,str,off+1+4) <= 0) return(0);
				}
			sprintf(str,"%02x%s",buf[i],((i+1) == n)?"":":");
			if (BIO_puts(bp,str) <= 0) return(0);
			}
		if (BIO_puts(bp,"\n") <= 0) return(0);
		}
	return(1);
	}

#ifndef WIN16
int DHparams_print_fp(fp,x)
FILE *fp;
DH *x;
        {
        BIO *b;
        int ret;

        if ((b=BIO_new(BIO_s_file())) == NULL)
		{
		DHerr(DH_F_DHPARAMS_PRINT_FP,ERR_R_BUF_LIB);
                return(0);
		}
        BIO_set_fp(b,fp,BIO_NOCLOSE);
        ret=DHparams_print(b, x);
        BIO_free(b);
        return(ret);
        }
#endif

int DHparams_print(bp,x)
BIO *bp;
DH *x;
	{
	unsigned char *m=NULL;
	char buf[128];
	int reason=ERR_R_BUF_LIB,i,ret=0;

	i=BN_num_bytes(x->p);
	m=(unsigned char *)malloc((unsigned int)i+10);
	if (m == NULL)
		{
		reason=ERR_R_MALLOC_FAILURE;
		goto err;
		}

	sprintf(buf,"Diffie-Hellman-Parameters: (%d bit)\n",BN_num_bits(x->p));
	if (BIO_puts(bp,buf) <= 0) goto err;
	if (!print(bp,"prime:",x->p,m,4)) goto err;
	if (!print(bp,"generator:",x->g,m,4)) goto err;
	if (x->length != 0)
		{
		sprintf(buf,"    recomented-private-length: %d bits\n",
			(int)x->length);
		if (BIO_puts(bp,buf) <= 0) goto err;
		}
	ret=1;
err:
	if (m != NULL) free(m);
	DHerr(DH_F_DHPARAMS_PRINT,reason);
	return(ret);
	}
