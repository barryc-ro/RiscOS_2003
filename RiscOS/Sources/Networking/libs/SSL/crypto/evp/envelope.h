/* crypto/evp/envelope.h */
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

#ifndef HEADER_ENVELOPE_H
#define HEADER_ENVELOPE_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "md2.h"
#include "md5.h"
#include "sha.h"
#include "des.h"
#ifndef NO_RC4
#include "rc4.h"
#endif
#ifndef NO_RC2
#include "rc2.h"
#endif
#ifndef NO_IDEA
#include "idea.h"
#endif

#define EVP_RC2_KEY_SIZE	16
#define EVP_RC4_KEY_SIZE	16
#define EVP_MAX_MD_SIZE		20
#define EVP_MAX_KEY_LENGTH	24
#define EVP_MAX_IV_LENGTH	8

#include "rsa.h"
#include "dsa.h"
#include "dh.h"

#define EVP_PKEY_NONE	NID_undef
#define EVP_PKEY_RSA	NID_rsaEncryption
#define EVP_PKEY_RSA2	NID_rsa
#define EVP_PKEY_DSA	NID_dss
#define EVP_PKEY_DSA2	NID_dsaWithSHA
#define EVP_PKEY_DSA3	NID_dsaWithSHA1
#define EVP_PKEY_DH	NID_dhKeyAgreement

typedef struct evp_pkey_st
        {
        int type;
	int references;
        union   {
                char *ptr;
                RSA *rsa;
                DSA *dsa;
                DH *dh;
                } pkey;
#ifdef HEADER_STACK_H
        STACK /* X509_ATTRIBUTE */ *attributes; /* [ 0 ] */
#else
        char /* X509_ATTRIBUTE */ *attributes; /* [ 0 ] */
#endif
        } EVP_PKEY;

#ifndef EVP_MD
typedef struct env_md_st
	{
	int type;
	int pkey_type;
	int md_size;
	void (*init)();
	void (*update)();
	void (*final)();

	int required_pkey_type; /*EVP_PKEY_xxx */
	int (*sign)();
	int (*verify)();
	} EVP_MD;
#endif

typedef struct env_md_ctx_st
	{
	EVP_MD *digest;
	union	{
		unsigned char base[4];
		MD2_CTX md2;
		MD5_CTX md5;
		SHA_CTX sha;
		} md;
	} EVP_MD_CTX;

typedef struct evp_cipher_st
	{
	int type;
	int block_size;
	int key_len;
	int iv_len;
	void (*enc_init)();	/* init for encryption */
	void (*dec_init)();	/* init for decryption */
	void (*do_cipher)();	/* encrypt data */
	} EVP_CIPHER;

typedef struct evp_cipher_info_st
	{
	EVP_CIPHER *cipher;
	unsigned char iv[EVP_MAX_IV_LENGTH];
	} EVP_CIPHER_INFO;

typedef struct evp_cipher_ctx_st
	{
	EVP_CIPHER *cipher;
	int encrypt;		/* encrypt or decrypt */
	int buf_len;		/* number we have left */
	unsigned char buf[8];
	union	{
#ifndef NO_RC4
		struct
			{
			unsigned char key[EVP_RC4_KEY_SIZE];
			RC4_KEY ks;	/* working key */
			} rc4;
#endif
		struct
			{
			des_key_schedule ks;/* key schedule */
			} des_ecb;

		struct
			{
			C_Block oiv;	/* original iv */
			C_Block iv;	/* working iv */
			des_key_schedule ks;/* key schedule */
			} des_cbc;

		struct
			{
			C_Block oiv;	/* original iv */
			C_Block iv;	/* working iv */
			des_key_schedule ks;/* key schedule */
			des_key_schedule ks2;/* key schedule (for ede) */
			des_key_schedule ks3;/* key schedule (for ede3) */
			int num;	/* used by cfb mode */
			} des_cfb;

		struct
			{
			C_Block oiv;	/* original iv */
			C_Block iv;	/* working iv */
			des_key_schedule ks1;/* ksched 1 */
			des_key_schedule ks2;/* ksched 2 */
			des_key_schedule ks3;/* ksched 3 */
			} des_ede;
#ifndef NO_IDEA
		struct
			{
			IDEA_KEY_SCHEDULE ks;/* key schedule */
			} idea_ecb;
		struct
			{
			unsigned char oiv[8];/* original iv */
			unsigned char iv[8]; /* working iv */
			IDEA_KEY_SCHEDULE ks;/* key schedule */
			} idea_cbc;
		struct
			{
			unsigned char oiv[8];/* original iv */
			unsigned char iv[8]; /* working iv */
			IDEA_KEY_SCHEDULE ks;/* key schedule */
			int num;	/* used by cfb mode */
			} idea_cfb;
#endif
#ifndef NO_RC2
		struct
			{
			RC2_KEY ks;/* key schedule */
			} rc2_ecb;
		struct
			{
			unsigned char oiv[8];/* original iv */
			unsigned char iv[8]; /* working iv */
			RC2_KEY ks;/* key schedule */
			} rc2_cbc;
		struct
			{
			unsigned char oiv[8];/* original iv */
			unsigned char iv[8]; /* working iv */
			RC2_KEY ks;/* key schedule */
			int num;	/* used by cfb mode */
			} rc2_cfb;
#endif
		} c;
	} EVP_CIPHER_CTX;

typedef struct evp_Encode_Ctx_st
	{
	int num;        /* number saved in a partial encode/decode */
	int length;	/* The length is either the output line length
			 * (in input bytes) or the shortest input line
			 * length that is ok.  Once decoding begins,
			 * the length is adjusted up each time a longer
			 * line is decoded */
	unsigned char enc_data[80];     /* data to encode */
	} EVP_ENCODE_CTX;

#define EVP_ENCODE_LENGTH(l)    (((l+2)/3*4)+(l/48+1)*2)
#define EVP_DECODE_LENGTH(l)    ((l+3)/4*3)

#define EVP_SignInit(a,b)		EVP_DigestInit(a,b)
#define EVP_SignUpdate(a,b,c)		EVP_DigestUpdate(a,b,c)
#define	EVP_VerifyInit(a,b)		EVP_DigestInit(a,b)
#define	EVP_VerifyUpdate(a,b,c)		EVP_DigestUpdate(a,b,c)
#define EVP_OpenUpdate(a,b,c,d,e)	EVP_DecryptUpdate(a,b,c,d,e)
#define EVP_SealUpdate(a,b,c,d,e)	EVP_EncryptUpdate(a,b,c,d,e)	

#ifdef HEADER_BUFFER_H
typedef struct bio_f_md_ctx_struct
	{
	BIO *bio;		/* the bio for IO */
	EVP_MD_CTX md_ctx;	/* the digest to use */
	} BIO_F_MD_CTX;
#endif

#define BIO_B64_GET_DECODE	1
#define BIO_B64_PUT_ENCODE	1
#define BIO_B64_GET_ENCODE	2
#define BIO_B64_PUT_DECODE	2

#define BIO_set_md(b,md)	BIO_ctrl(b,BIO_CTRL_SET,1,(char *)md)

#ifndef NOPROTO

EVP_MD *EVP_get_MDbyname(char *name);

void	EVP_DigestInit(EVP_MD_CTX *ctx, EVP_MD *type);
void	EVP_DigestUpdate(EVP_MD_CTX *ctx,unsigned char *d,unsigned int cnt);
void	EVP_DigestFinal(EVP_MD_CTX *ctx,unsigned char *md,unsigned int *s);

int	EVP_read_pw_string(char *buf,int length,char *prompt,int verify);
void	EVP_set_pw_prompt(char *prompt);
char *	EVP_get_pw_prompt(void);

int	EVP_BytesToKey(EVP_CIPHER *type,EVP_MD *md,unsigned char *salt,
		unsigned char *data, int datal, int count,
		unsigned char *key,unsigned char *iv);

EVP_CIPHER *EVP_get_cipherbyname(char *name);

void	EVP_EncryptInit(EVP_CIPHER_CTX *ctx,EVP_CIPHER *type,
		unsigned char *key, unsigned char *iv);
void	EVP_EncryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out,
		int *outl, unsigned char *in, int inl);
void	EVP_EncryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl);

void	EVP_DecryptInit(EVP_CIPHER_CTX *ctx,EVP_CIPHER *type,
		unsigned char *key, unsigned char *iv);
void	EVP_DecryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out,
		int *outl, unsigned char *in, int inl);
int	EVP_DecryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);

void	EVP_CipherInit(EVP_CIPHER_CTX *ctx,EVP_CIPHER *type, unsigned char *key,
		unsigned char *iv,int enc);
void	EVP_CipherUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out,
		int *outl, unsigned char *in, int inl);
int	EVP_CipherFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);

int     EVP_SignFinal(EVP_MD_CTX *ctx,unsigned char *md,unsigned int *s,
		EVP_PKEY *pkey);

int	EVP_VerifyFinal(EVP_MD_CTX *ctx,unsigned char *sigbuf,
		unsigned int siglen,EVP_PKEY *pkey);

int	EVP_OpenInit(EVP_CIPHER_CTX *ctx,EVP_CIPHER *type,unsigned char *ek,
		int ekl,unsigned char *iv,EVP_PKEY *priv);
int	EVP_OpenFinal(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl);



int	EVP_SealInit(EVP_CIPHER_CTX *ctx, EVP_CIPHER *type, unsigned char **ek,
		int *ekl, unsigned char *iv,EVP_PKEY **pubk, int npubk);
void	EVP_SealFinal(EVP_CIPHER_CTX *ctx,unsigned char *out,int *outl);

void    EVP_EncodeInit(EVP_ENCODE_CTX *ctx);
void    EVP_EncodeUpdate(EVP_ENCODE_CTX *ctx,unsigned char *out,
		int *outl,unsigned char *in,int inl);
void    EVP_EncodeFinal(EVP_ENCODE_CTX *ctx,unsigned char *out,int *outl);
int     EVP_EncodeBlock(unsigned char *t, unsigned char *f, int n);

void    EVP_DecodeInit(EVP_ENCODE_CTX *ctx);
int     EVP_DecodeUpdate(EVP_ENCODE_CTX *ctx,unsigned char *out,int *outl,
		unsigned char *in, int inl);
int     EVP_DecodeFinal(EVP_ENCODE_CTX *ctx, unsigned
		char *out, int *outl);
int     EVP_DecodeBlock(unsigned char *t, unsigned
		char *f, int n);

void	ERR_load_EVP_strings(void );

#ifdef HEADER_BUFFER_H
BIO_METHOD *BIO_f_md(void);
#endif

EVP_MD *EVP_md2(void);
EVP_MD *EVP_md5(void);
EVP_MD *EVP_sha(void);
EVP_MD *EVP_sha1(void);
EVP_MD *EVP_dss(void);
EVP_MD *EVP_dss1(void);

EVP_CIPHER *EVP_des_ecb(void);
EVP_CIPHER *EVP_des_ede(void);
EVP_CIPHER *EVP_des_ede3(void);
EVP_CIPHER *EVP_des_cfb(void);
EVP_CIPHER *EVP_des_ede_cfb(void);
EVP_CIPHER *EVP_des_ede3_cfb(void);
EVP_CIPHER *EVP_des_ofb(void);
EVP_CIPHER *EVP_des_ede_ofb(void);
EVP_CIPHER *EVP_des_ede3_ofb(void);
EVP_CIPHER *EVP_des_cbc(void);
EVP_CIPHER *EVP_des_ede_cbc(void);
EVP_CIPHER *EVP_des_ede3_cbc(void);
EVP_CIPHER *EVP_rc4(void);
EVP_CIPHER *EVP_idea_ecb(void);
EVP_CIPHER *EVP_idea_cfb(void);
EVP_CIPHER *EVP_idea_ofb(void);
EVP_CIPHER *EVP_idea_cbc(void);
EVP_CIPHER *EVP_rc2_ecb(void);
EVP_CIPHER *EVP_rc2_cbc(void);
EVP_CIPHER *EVP_rc2_cfb(void);
EVP_CIPHER *EVP_rc2_ofb(void);

int		EVP_PKEY_size(EVP_PKEY *pkey);
EVP_PKEY *	EVP_PKEY_new(void);
void		EVP_PKEY_free(EVP_PKEY *pkey);
EVP_PKEY *	d2i_PublicKey(int type,EVP_PKEY **a, unsigned char **pp,
			long length);
int		i2d_PublicKey(EVP_PKEY *a, unsigned char **pp);

EVP_PKEY *	d2i_PrivateKey(int type,EVP_PKEY **a, unsigned char **pp,
			long length);
int     	i2d_PrivateKey(EVP_PKEY *a, unsigned char **pp);

#else

int		EVP_PKEY_size();
EVP_PKEY *	EVP_PKEY_new();
void		EVP_PKEY_free();
EVP_PKEY *	d2i_PublicKey();
int		i2d_PublicKey();
EVP_PKEY *	d2i_PrivateKey();
int     	i2d_PrivateKey();

EVP_MD *EVP_get_MDbyname();
void	EVP_DigestInit();
void	EVP_DigestUpdate();
void	EVP_DigestFinal();
int	EVP_read_pw_string();
void	EVP_set_pw_prompt();
char *	EVP_get_pw_prompt();
int	EVP_BytesToKey();
EVP_CIPHER *EVP_get_cipherbyname();
void	EVP_EncryptInit();
void	EVP_EncryptUpdate();
void	EVP_EncryptFinal();
void	EVP_DecryptInit();
void	EVP_DecryptUpdate();
int	EVP_DecryptFinal();
void	EVP_CipherInit();
void	EVP_CipherUpdate();
int	EVP_CipherFinal();
int     EVP_SignFinal();
int	EVP_VerifyFinal();
int	EVP_OpenInit();
int	EVP_OpenFinal();
int	EVP_SealInit();
void	EVP_SealFinal();
void    EVP_EncodeInit();
void    EVP_EncodeUpdate();
void    EVP_EncodeFinal();
int     EVP_EncodeBlock();
void    EVP_DecodeInit();
int     EVP_DecodeUpdate();
int     EVP_DecodeFinal();
int     EVP_DecodeBlock();

void	ERR_load_EVP_strings();

#ifdef HEADER_BUFFER_H
BIO_METHOD *BIO_f_md();
#endif

EVP_MD *EVP_md2();
EVP_MD *EVP_md5();
EVP_MD *EVP_sha();
EVP_MD *EVP_sha1();

EVP_CIPHER *EVP_des_ecb();
EVP_CIPHER *EVP_des_ede();
EVP_CIPHER *EVP_des_ede3();
EVP_CIPHER *EVP_des_cfb();
EVP_CIPHER *EVP_des_ede_cfb();
EVP_CIPHER *EVP_des_ede3_cfb();
EVP_CIPHER *EVP_des_ofb();
EVP_CIPHER *EVP_des_ede_ofb();
EVP_CIPHER *EVP_des_ede3_ofb();
EVP_CIPHER *EVP_des_cbc();
EVP_CIPHER *EVP_des_ede_cbc();
EVP_CIPHER *EVP_des_ede3_cbc();
EVP_CIPHER *EVP_rc4();
EVP_CIPHER *EVP_idea_ecb();
EVP_CIPHER *EVP_idea_cfb();
EVP_CIPHER *EVP_idea_ofb();
EVP_CIPHER *EVP_idea_cbc();
EVP_CIPHER *EVP_rc2_ecb();
EVP_CIPHER *EVP_rc2_cbc();
EVP_CIPHER *EVP_rc2_cfb();
EVP_CIPHER *EVP_rc2_ofb();

#endif

/* BEGIN ERROR CODES */
/* Error codes for the EVP functions. */

/* Function codes. */
#define EVP_F_D2I_PKEY					 100
#define EVP_F_EVP_DECRYPTFINAL				 101
#define EVP_F_EVP_OPENINIT				 102
#define EVP_F_EVP_PKEY_NEW				 103
#define EVP_F_EVP_SEALINIT				 104
#define EVP_F_EVP_SIGNFINAL				 105
#define EVP_F_EVP_VERIFYFINAL				 106

/* Reason codes. */
#define EVP_R_BAD_DECRYPT				 100
#define EVP_R_DIGEST_TOO_BIG_FOR_RSA_KEY		 101
#define EVP_R_IV_TOO_LARGE				 102
#define EVP_R_PUBLIC_KEY_NOT_RSA			 103
#define EVP_R_THE_ASN1_OBJECT_IDENTIFIER_IS_NOT_KNOWN_FOR_THIS_MD 104
#define EVP_R_UNKNOWN_ALGORITHM_TYPE			 105
#define EVP_R_UNSUPORTED_CIPHER				 106
#define EVP_R_WRONG_FINAL_BLOCK_LENGTH			 107
#define EVP_R_WRONG_PUBLIC_KEY_TYPE			 108

#ifdef  __cplusplus
}
#endif
#endif

