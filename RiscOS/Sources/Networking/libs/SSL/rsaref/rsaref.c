/* rsaref/rsaref.c */
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
#include "bn.h"
#include "rsa.h"
#include "rsaref.h"
#include "rand.h"

/* 
 * RSAREFerr(RSA_private_decrypt,RE_CONTENT_ENCODING);
 * RSAREFerr(RSA_private_decrypt,RE_DATA);
 * RSAREFerr(RSA_private_decrypt,RE_DIGEST_ALGORITHM);
 * RSAREFerr(RSA_private_decrypt,RE_ENCODING);
 * RSAREFerr(RSA_private_decrypt,RE_KEY);
 * RSAREFerr(RSA_private_decrypt,RE_KEY_ENCODING);
 * RSAREFerr(RSA_private_decrypt,RE_LEN);
 * RSAREFerr(RSA_private_decrypt,RE_MODULUS_LEN);
 * RSAREFerr(RSA_private_decrypt,RE_NEED_RANDOM);
 * RSAREFerr(RSA_private_decrypt,RE_PRIVATE_KEY);
 * RSAREFerr(RSA_private_decrypt,RE_PUBLIC_KEY);
 * RSAREFerr(RSA_private_decrypt,RE_SIGNATURE);
 * RSAREFerr(RSA_private_decrypt,RE_SIGNATURE_ENCODING);
 * RSAREFerr(RSA_private_decrypt,RE_ENCRYPTION_ALGORITHM);
 * RSAREFerr(RSAREF_F_RSAREF_BN2BIN,ERR_R_BN_LIB);
 */

#ifndef NOPROTO
static int RSAref_bn2bin(BIGNUM * from, unsigned char* to, int max);
#ifdef undef
static BIGNUM* RSAref_bin2bn(unsigned char* from, BIGNUM * to, int max);
#endif
static int RSAref_Public_eay2ref(RSA * from, RSArefPublicKey * to);
static int RSAref_Private_eay2ref(RSA * from, RSArefPrivateKey * to);
#else
static int RSAref_bn2bin();
#ifdef undef
static BIGNUM* RSAref_bin2bn();
#endif
static int RSAref_Public_eay2ref();
static int RSAref_Private_eay2ref();
#endif

static int RSAref_bn2bin(from,to,max)
BIGNUM *from;
unsigned char *to; /* [max] */
int max;
	{
	int i;

	i=BN_num_bytes(from);
	if (i > max)
		{
		RSAREFerr(RSAREF_F_RSAREF_BN2BIN,RE_LEN);
		return(0);
		}

	memset(to,0,(unsigned int)max);
	if (!BN_bn2bin(from,&(to[max-i])))
		return(0);
	return(1);
	}

#ifdef undef
static BIGNUM *RSAref_bin2bn(from,to,max)
unsigned char *from; /* [max] */
BIGNUM *to;
int max;
	{
	int i;
	BIGNUM *ret;

	for (i=0; i<max; i++)
		if (from[i]) break;

	ret=BN_bin2bn(&(from[i]),max-i,to);
	return(ret);
	}

static int RSAref_Public_ref2eay(from,to)
RSArefPublicKey *from;
RSA *to;
	{
	to->n=RSAref_bin2bn(from->m,NULL,RSAref_MAX_LEN);
	to->e=RSAref_bin2bn(from->e,NULL,RSAref_MAX_LEN);
	if ((to->n == NULL) || (to->e == NULL)) return(0);
	return(1);
	}
#endif

static int RSAref_Public_eay2ref(from,to)
RSA *from;
RSArefPublicKey *to;
	{
	to->bits=BN_num_bits(from->n);
	if (!RSAref_bn2bin(from->n,to->m,RSAref_MAX_LEN)) return(0);
	if (!RSAref_bn2bin(from->e,to->e,RSAref_MAX_LEN)) return(0);
	return(1);
	}

#ifdef undef
static int RSAref_Private_ref2eay(from,to)
RSArefPrivateKey *from;
RSA *to;
	{
	if ((to->n=RSAref_bin2bn(from->m,NULL,RSAref_MAX_LEN)) == NULL)
		return(0);
	if ((to->e=RSAref_bin2bn(from->e,NULL,RSAref_MAX_LEN)) == NULL)
		return(0);
	if ((to->d=RSAref_bin2bn(from->d,NULL,RSAref_MAX_LEN)) == NULL)
		return(0);
	if ((to->p=RSAref_bin2bn(from->prime[0],NULL,RSAref_MAX_PLEN)) == NULL)
		return(0);
	if ((to->q=RSAref_bin2bn(from->prime[1],NULL,RSAref_MAX_PLEN)) == NULL)
		return(0);
	if ((to->dmp1=RSAref_bin2bn(from->pexp[0],NULL,RSAref_MAX_PLEN))
		== NULL)
		return(0);
	if ((to->dmq1=RSAref_bin2bn(from->pexp[1],NULL,RSAref_MAX_PLEN))
		== NULL)
		return(0);
	if ((to->iqmp=RSAref_bin2bn(from->coef,NULL,RSAref_MAX_PLEN)) == NULL)
		return(0);
	return(1);
	}
#endif

static int RSAref_Private_eay2ref(from,to)
RSA *from;
RSArefPrivateKey *to;
	{
	to->bits=BN_num_bits(from->n);
	if (!RSAref_bn2bin(from->n,to->m,RSAref_MAX_LEN)) return(0);
	if (!RSAref_bn2bin(from->e,to->e,RSAref_MAX_LEN)) return(0);
	if (!RSAref_bn2bin(from->d,to->d,RSAref_MAX_LEN)) return(0);
	if (!RSAref_bn2bin(from->p,to->prime[0],RSAref_MAX_PLEN)) return(0);
	if (!RSAref_bn2bin(from->q,to->prime[1],RSAref_MAX_PLEN)) return(0);
	if (!RSAref_bn2bin(from->dmp1,to->pexp[0],RSAref_MAX_PLEN)) return(0);
	if (!RSAref_bn2bin(from->dmq1,to->pexp[1],RSAref_MAX_PLEN)) return(0);
	if (!RSAref_bn2bin(from->iqmp,to->coef,RSAref_MAX_PLEN)) return(0);
	return(1);
	}

int RSA_private_decrypt(len,from,to,rsa)
int len;
unsigned char *from,*to;
RSA *rsa;
	{
	int i,outlen= -1;
	RSArefPrivateKey RSAkey;

	if (!RSAref_Private_eay2ref(rsa,&RSAkey))
		goto err;
	if ((i=RSAPrivateDecrypt(to,&outlen,from,len,&RSAkey)) != 0)
		{
		RSAREFerr(RSAREF_F_RSA_PRIVATE_DECRYPT,i);
		outlen= -1;
		}
err:
	memset(&RSAkey,0,sizeof(RSAkey));
	return(outlen);
	}

int RSA_private_encrypt(len,from,to,rsa)
int len;
unsigned char *from,*to;
RSA *rsa;
	{
	int i,outlen= -1;
	RSArefPrivateKey RSAkey;

	if (!RSAref_Private_eay2ref(rsa,&RSAkey))
		goto err;
	if ((i=RSAPrivateEncrypt(to,&outlen,from,len,&RSAkey)) != 0)
		{
		RSAREFerr(RSAREF_F_RSA_PRIVATE_ENCRYPT,i);
		outlen= -1;
		}
err:
	memset(&RSAkey,0,sizeof(RSAkey));
	return(outlen);
	}

int RSA_public_decrypt(len,from,to,rsa)
int len;
unsigned char *from,*to;
RSA *rsa;
	{
	int i,outlen= -1;
	RSArefPublicKey RSAkey;

	if (!RSAref_Public_eay2ref(rsa,&RSAkey))
		goto err;
	if ((i=RSAPublicDecrypt(to,&outlen,from,len,&RSAkey)) != 0)
		{
		RSAREFerr(RSAREF_F_RSA_PUBLIC_DECRYPT,i);
		outlen=-1;
		}
err:
	memset(&RSAkey,0,sizeof(RSAkey));
	return(outlen);
	}

int RSA_public_encrypt(len,from,to,rsa)
int len;
unsigned char *from,*to;
RSA *rsa;
	{
	int outlen= -1;
	int i;
	RSArefPublicKey RSAkey;
	RSARandomState rnd;
	unsigned char buf[16];

	R_RandomInit(&rnd);
	R_GetRandomBytesNeeded((unsigned int *)&i,&rnd);
	while (i > 0)
		{
		RAND_bytes(buf,16);
		R_RandomUpdate(&rnd,buf,(unsigned int)((i>16)?16:i));
		i-=16;
		}

	if (!RSAref_Public_eay2ref(rsa,&RSAkey))
		goto err;
	if ((i=RSAPublicEncrypt(to,&outlen,from,len,&RSAkey,&rnd)) != 0)
		{
		RSAREFerr(RSAREF_F_RSA_PUBLIC_DECRYPT,i);
		outlen=-1;
		goto err;
		}
err:
	memset(&RSAkey,0,sizeof(RSAkey));
	R_RandomFinal(&rnd);
	memset(&rnd,0,sizeof(rnd));
	return(outlen);
	}

