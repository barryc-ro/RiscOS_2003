/* crypto/evp/evp_enc.c */
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
#include "envelope.h"

char *EVP_version="EVP part of SSLeay 0.6.0 21-Jun-1996";

void EVP_CipherInit(ctx,data,key,iv,enc)
EVP_CIPHER_CTX *ctx;
EVP_CIPHER *data;
unsigned char *key;
unsigned char *iv;
int enc;
	{
	if (enc)
		EVP_EncryptInit(ctx,data,key,iv);
	else	
		EVP_DecryptInit(ctx,data,key,iv);
	}

void EVP_CipherUpdate(ctx,out,outl,in,inl)
EVP_CIPHER_CTX *ctx;
unsigned char *out;
int *outl;
unsigned char *in;
int inl;
	{
	if (ctx->encrypt)
		EVP_EncryptUpdate(ctx,out,outl,in,inl);
	else	EVP_DecryptUpdate(ctx,out,outl,in,inl);
	}

int EVP_CipherFinal(ctx,out,outl)
EVP_CIPHER_CTX *ctx;
unsigned char *out;
int *outl;
	{
	if (ctx->encrypt)
		{
		EVP_EncryptFinal(ctx,out,outl);
		return(1);
		}
	else	return(EVP_DecryptFinal(ctx,out,outl));
	}

void EVP_EncryptInit(ctx,cipher,key,iv)
EVP_CIPHER_CTX *ctx;
EVP_CIPHER *cipher;
unsigned char *key;
unsigned char *iv;
	{
	if (cipher != NULL)
		ctx->cipher=cipher;
	cipher->enc_init(ctx,key,iv);
	ctx->encrypt=1;
	ctx->buf_len=0;
	}

void EVP_DecryptInit(ctx,cipher,key,iv)
EVP_CIPHER_CTX *ctx;
EVP_CIPHER *cipher;
unsigned char *key;
unsigned char *iv;
	{
	if (cipher != NULL)
		ctx->cipher=cipher;
	cipher->dec_init(ctx,key,iv);
	ctx->encrypt=0;
	ctx->buf_len=0;
	}


void EVP_EncryptUpdate(ctx,out,outl,in,inl)
EVP_CIPHER_CTX *ctx;
unsigned char *out;
int *outl;
unsigned char *in;
int inl;
	{
	int i,j,bl;

	i=ctx->buf_len;
	bl=ctx->cipher->block_size;
	*outl=0;
	if ((inl == 0) && (i != bl)) return;
	if (i != 0)
		{
		if (i+inl < bl)
			{
			memcpy(&(ctx->buf[i]),in,inl);
			ctx->buf_len+=inl;
			return;
			}
		else
			{
			j=bl-i;
			if (j != 0) memcpy(&(ctx->buf[i]),in,j);
			ctx->cipher->do_cipher(ctx,out,ctx->buf,bl);
			inl-=j;
			in+=j;
			out+=bl;
			*outl+=bl;
			}
		}
	i=inl%bl; /* how much is left */
	inl-=i;
	if (inl > 0)
		{
		ctx->cipher->do_cipher(ctx,out,in,inl);
		*outl+=inl;
		}

	if (i != 0)
		memcpy(ctx->buf,&(in[inl]),i);
	ctx->buf_len=i;
	}

void EVP_EncryptFinal(ctx,out,outl)
EVP_CIPHER_CTX *ctx;
unsigned char *out;
int *outl;
	{
	int i,n,b,bl;

	b=ctx->cipher->block_size;
	if (b == 1)
		{
		*outl=0;
		return;
		}
	bl=ctx->buf_len;
	n=b-bl;
	for (i=bl; i<b; i++)
		ctx->buf[i]=n;
	ctx->cipher->do_cipher(ctx,out,ctx->buf,b);
	*outl=b;
	}

void EVP_DecryptUpdate(ctx,out,outl,in,inl)
EVP_CIPHER_CTX *ctx;
unsigned char *out;
int *outl;
unsigned char *in;
int inl;
	{
	int b,bl,n;
	int keep_last=0;

	if (inl == 0) return;

	b=ctx->cipher->block_size;
	if (b > 1)
		{
		/* Is the input a multiple of the block size? */
		bl=ctx->buf_len;
		n=inl+bl;
		if (n%b == 0)
			{
			if (inl < b) /* must be 'just one' buff */
				{
				memcpy(&(ctx->buf[bl]),in,inl);
				ctx->buf_len=b;
				*outl=0;
				return;
				}
			keep_last=1;
			inl-=b; /* don't do the last block */
			}
		}
	EVP_EncryptUpdate(ctx,out,outl,in,inl);

	/* if we have 'decrypted' a multiple of block size, make sure
	 * we have a copy of this last block */
	if (keep_last)
		{
		memcpy(&(ctx->buf[0]),&(in[inl]),b);
#ifdef DEBUG
		if (ctx->buf_len != 0)
			{
			abort();
			}
#endif
		ctx->buf_len=b;
		}
	}

int EVP_DecryptFinal(ctx,out,outl)
EVP_CIPHER_CTX *ctx;
unsigned char *out;
int *outl;
	{
	int i,b;
	int n;

	*outl=0;
	b=ctx->cipher->block_size;
	if (b > 1)
		{
		if (ctx->buf_len != b)
			{
			EVPerr(EVP_F_EVP_DECRYPTFINAL,EVP_R_WRONG_FINAL_BLOCK_LENGTH);
			return(0);
			}
		EVP_EncryptUpdate(ctx,ctx->buf,&n,ctx->buf,0);
		if (n != b)
			return(0);
		n=ctx->buf[b-1];
		if (n > b)
			{
			EVPerr(EVP_F_EVP_DECRYPTFINAL,EVP_R_BAD_DECRYPT);
			return(0);
			}
		for (i=0; i<n; i++)
			{
			if (ctx->buf[--b] != n)
				{
				EVPerr(EVP_F_EVP_DECRYPTFINAL,EVP_R_BAD_DECRYPT);
				return(0);
				}
			}
		n=ctx->cipher->block_size-n;
		for (i=0; i<n; i++)
			out[i]=ctx->buf[i];
		*outl=n;
		}
	else
		*outl=0;
	return(1);
	}

