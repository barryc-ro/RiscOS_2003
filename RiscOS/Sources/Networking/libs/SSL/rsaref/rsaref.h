/* rsaref/rsaref.h */
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

#ifndef HEADER_RSAREF_H
#define HEADER_RSAREF_H

/* RSAeuro */
/*#define  RSAref_MAX_BITS		2048*/

/* RSAref */
#define  RSAref_MAX_BITS		1024

#define RSAref_MIN_BITS		508
#define RSAref_MAX_LEN		((RSAref_MAX_BITS+7)/8)
#define RSAref_MAX_PBITS	(RSAref_MAX_BITS+1)/2
#define RSAref_MAX_PLEN		((RSAref_MAX_PBITS+7)/8)

typedef struct RSArefPublicKey_st
	{
	unsigned int bits;
	unsigned char m[RSAref_MAX_LEN];
	unsigned char e[RSAref_MAX_LEN];
	} RSArefPublicKey;

typedef struct RSArefPrivateKey_st
	{
	unsigned int bits;
	unsigned char m[RSAref_MAX_LEN];
	unsigned char e[RSAref_MAX_LEN];
	unsigned char d[RSAref_MAX_LEN];
	unsigned char prime[2][RSAref_MAX_PLEN];/* p & q */
	unsigned char pexp[2][RSAref_MAX_PLEN];	/* dmp1 & dmq1 */
	unsigned char coef[RSAref_MAX_PLEN];	/* iqmp */
	} RSArefPrivateKey;

typedef struct RSARandomState_st
	{
	unsigned int needed;
	unsigned char state[16];
	unsigned int outputnum;
	unsigned char output[16];
	} RSARandomState;

#define RE_CONTENT_ENCODING 0x0400
#define RE_DATA 0x0401
#define RE_DIGEST_ALGORITHM 0x0402
#define RE_ENCODING 0x0403
#define RE_KEY 0x0404
#define RE_KEY_ENCODING 0x0405
#define RE_LEN 0x0406
#define RE_MODULUS_LEN 0x0407
#define RE_NEED_RANDOM 0x0408
#define RE_PRIVATE_KEY 0x0409
#define RE_PUBLIC_KEY 0x040a
#define RE_SIGNATURE 0x040b
#define RE_SIGNATURE_ENCODING 0x040c
#define RE_ENCRYPTION_ALGORITHM 0x040d

#ifndef NOPROTO
int RSAPrivateDecrypt(unsigned char *to, int *outlen, unsigned char *from,
	int len, RSArefPrivateKey *RSAkey);
int RSAPrivateEncrypt(unsigned char *to, int *outlen, unsigned char *from,
	int len, RSArefPrivateKey *RSAkey);
int RSAPublicDecrypt(unsigned char *to, int *outlen, unsigned char *from,
	int len, RSArefPublicKey *RSAkey);
int RSAPublicEncrypt(unsigned char *to, int *outlen, unsigned char *from,
	int len, RSArefPublicKey *RSAkey,RSARandomState *rnd);
int R_RandomInit(RSARandomState *rnd);
int R_GetRandomBytesNeeded(unsigned int *,RSARandomState *rnd);
int R_RandomUpdate(RSARandomState *rnd, unsigned char *data, unsigned int n);
int R_RandomFinal(RSARandomState *rnd);
#else
int RSAPrivateDecrypt();
int RSAPrivateEncrypt();
int RSAPublicDecrypt();
int RSAPublicEncrypt();
int R_RandomInit();
int R_GetRandomBytesNeeded();
int R_RandomUpdate();
int R_RandomFinal();
#endif

/* BEGIN ERROR CODES */
/* Error codes for the RSAREF functions. */

/* Function codes. */
#define RSAREF_F_RSAREF_BN2BIN				 100
#define RSAREF_F_RSA_BN2BIN				 101
#define RSAREF_F_RSA_PRIVATE_DECRYPT			 102
#define RSAREF_F_RSA_PRIVATE_ENCRYPT			 103
#define RSAREF_F_RSA_PUBLIC_DECRYPT			 104
#define RSAREF_F_RSA_PUBLIC_ENCRYPT			 105

/* Reason codes. */

#ifdef  __cplusplus
}
#endif
#endif

