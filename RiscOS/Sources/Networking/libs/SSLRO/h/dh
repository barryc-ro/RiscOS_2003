/* crypto/dh/dh.h */
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

#ifndef HEADER_DH_H
#define HEADER_DH_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef HEADER_BN_H
#define BIGNUM 		char
#endif

typedef struct dh_param_st
	{
	/* This first argument is used to pick up errors when
	 * a DH is passed instead of a EVP_PKEY */
	int pad;
	int version;
	BIGNUM *p;
	BIGNUM *g;
	int length; /* optional */
	BIGNUM *public;
	BIGNUM *private;
	} DH;

#define DH_GENERATOR_2		2
/* #define DH_GENERATOR_3	3 */
#define DH_GENERATOR_5		5

/* DH_check error codes */
#define DH_CHECK_P_NOT_PRIME		0x01
#define DH_CHECK_P_NOT_STRONG_PRIME	0x02
#define DH_UNABLE_TO_CHECK_GENERATOR	0x04
#define DH_NOT_SUITABLE_GENERATOR	0x08

#define DHparams_dup(x) (DH *)ASN1_dup(i2d_DHparams,(char *(*)()d2i_DHparams, \
		(char *)(x)))
#define d2i_DHparams_fp(fp,x) (DH *)ASN1_d2i_fp((char *(*)())DH_new, \
		(char *(*)())d2i_DHparams,(fp),(unsigned char **)(x))
#define i2d_DHparams_fp(fp,x) ASN1_i2d_fp(i2d_DHparams,(fp), \
		(unsigned char *)(x))
#define d2i_DHparams_bio(bp,x) (DH *)ASN1_d2i_bio((char *(*)())DH_new, \
		(char *(*)())d2i_DHparams,(bp),(unsigned char **)(x))
#define i2d_DHparams_bio(bp,x) ASN1_i2d_bio(i2d_DHparams,(bp), \
		(unsigned char *)(x))

#ifndef NOPROTO
DH *	DH_new(void);
void	DH_free(DH *dh);
int	DH_size(DH *dh);
DH *	DH_generate_parameters(int prime_len,int generator,
		void (*callback)(int,int));
int	DH_check(DH *dh,int *codes);
int	DH_generate_key(DH *dh);
int	DH_compute_key(unsigned char *key,BIGNUM *public,DH *dh);
DH *	d2i_DHparams(DH **a,unsigned char **pp, long length);
int	i2d_DHparams(DH *a,unsigned char **pp);
#ifndef WIN16
int	DHparams_print_fp(FILE *fp, DH *x);
#endif
#ifdef HEADER_BUFFER_H
int	DHparams_print(BIO *bp, DH *x);
#else
int	DHparams_print(char *bp, DH *x);
#endif
void	ERR_load_DH_strings(void );

#else

DH *	DH_new();
void	DH_free();
int	DH_size();
DH *	DH_generate_parameters();
int	DH_check();
int	DH_generate_key();
int	DH_compute_key();
DH *	d2i_DHparams();
int	i2d_DHparams();
#ifndef WIN16
int	DHparams_print_fp();
#endif
int	DHparams_print();
void	ERR_load_DH_strings();

#endif

/* BEGIN ERROR CODES */
/* Error codes for the DH functions. */

/* Function codes. */
#define DH_F_DHPARAMS_PRINT				 100
#define DH_F_DHPARAMS_PRINT_FP				 101
#define DH_F_DH_COMPUTE_KEY				 102
#define DH_F_DH_GENERATE_KEY				 103
#define DH_F_DH_GENERATE_PARAMETERS			 104
#define DH_F_DH_NEW					 105

/* Reason codes. */
#define DH_R_NO_PRIVATE_VALUE				 100

#ifdef  __cplusplus
}
#endif
#endif

