/* crypto/rc2/rc2.h */
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

#ifndef HEADER_RC2_H
#define HEADER_RC2_H

#ifdef  __cplusplus
extern "C" {
#endif

#define RC2_ENCRYPT	1
#define RC2_DECRYPT	0

/* I need to put in a mod for the alpha - eay */
#define RC2_INT unsigned long

#define RC2_BLOCK	8
#define RC2_KEY_LENGTH	16

typedef struct rc2_key_st
	{
	RC2_INT data[64];
	} RC2_KEY;

#ifndef NOPROTO
 
void RC2_set_key(RC2_KEY *key, int len, unsigned char *data);
void RC2_ecb_encrypt(unsigned char *in,unsigned char *out,RC2_KEY *key,
	int encrypt);
void RC2_encrypt(unsigned long *data,RC2_KEY *key,int encrypt);
void RC2_cbc_encrypt(unsigned char *in, unsigned char *out, long length,
	RC2_KEY *ks, unsigned char *iv, int encrypt);
void RC2_cfb64_encrypt(unsigned char *in, unsigned char *out, long length,
	RC2_KEY *schedule, unsigned char *ivec, int *num, int encrypt);
void RC2_ofb64_encrypt(unsigned char *in, unsigned char *out, long length,
	RC2_KEY *schedule, unsigned char *ivec, int *num);

#else

void RC2_set_key();
void RC2_ecb_encrypt();
void RC2_encrypt();
void RC2_cbc_encrypt();
void RC2_cfb64_encrypt();
void RC2_ofb64_encrypt();

#endif

#ifdef  __cplusplus
}
#endif

#endif
