/* crypto/pkcs7/pkcs7.h */
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


#ifndef HEADER_PKCS7_H
#define HEADER_PKCS7_H

#ifdef  __cplusplus
extern "C" {
#endif

/*
Encryption_ID		DES-CBC
Digest_ID		MD5
Digest_Encryption_ID	rsaEncryption
Key_Encryption_ID	rsaEncryption
*/

typedef struct pkcs7_issuer_and_serial_st
	{
	X509_NAME *issuer;
	ASN1_INTEGER *serial;
	} PKCS7_ISSUER_AND_SERIAL;

typedef struct pkcs7_signer_info_st
	{
	ASN1_INTEGER 			*version;	/* version 1 */
	PKCS7_ISSUER_AND_SERIAL		*issuer_and_serial;
	ASN1_OBJECT			*digest_alg;
	STACK /* X509_ATTRIBUTE */	*auth_attr;	/* [ 0 ] */
	X509_ALGOR			*digest_enc_alg;
	ASN1_OCTET_STRING		*enc_digest;
	STACK /* X509_ATTRIBUTE */	*unauth_attr;	/* [ 1 ] */
	} PKCS7_SIGNER_INFO;

typedef struct pkcs7_recip_info_st
	{
	ASN1_INTEGER			*version;	/* version 0 */
	PKCS7_ISSUER_AND_SERIAL		*issuer_and_serial;
	X509_ALGOR			*key_enc_algor;
	ASN1_OCTET_STRING		*enc_key;
	} PKCS7_RECIP_INFO;

typedef struct pkcs7_signed_st
	{
	ASN1_INTEGER			*version;	/* version 1 */
	STACK /* ASN1_OBJECT's */	*md_algs;	/* md used */
	struct pkcs7_st			*contents;
	STACK /* X509 */		*cert;		/* [ 0 ] */
	STACK /* X509_CRL */		*crl;		/* [ 1 ] */
	STACK /* PKCS7_SIGNER_INFO */	*signer_info;
	} PKCS7_SIGNED;

typedef struct pkcs7_enc_content_st
	{
	ASN1_OBJECT			*content_type;
	X509_ALGOR			*algorithm;
	ASN1_OCTET_STRING		*enc_data;	/* [ 0 ] */
	} PKCS7_ENC_CONTENT;

typedef struct pkcs7_enveloped_st
	{
	ASN1_INTEGER			*version;	/* version 0 */
	STACK /* PKCS7_RECIP_INFO */	*recipientinfo;
	PKCS7_ENC_CONTENT		*enc_data;
	} PKCS7_ENVELOPE;
	
typedef struct pkcs7_signedandenveloped_st
	{
	ASN1_INTEGER			*version;	/* version 1 */
	STACK /* PKCS7_RECIP_INFO */	*recipientinfo;
	STACK /* ASN1_OBJECT's */	*md_algs;	/* md used */
	PKCS7_ENC_CONTENT		*enc_data;
	STACK /* X509 */		*cert;		/* [ 0 ] */
	STACK /* X509_CRL */		*crl;		/* [ 1 ] */
	STACK /* PKCS7_SIGNER_INFO */	*signer_info;
	} PKCS7_SIGN_ENVELOPE;

typedef struct pkcs7_digest_st
	{
	ASN1_INTEGER			*version;	/* version 0 */
	ASN1_OBJECT			*md;		/* md used */
	struct pkcs7_st 		*contents;
	ASN1_OCTET_STRING		*digest;
	} PKCS7_DIGEST;

typedef struct pkcs7_encrypted_st
	{
	ASN1_INTEGER			*version;	/* version 0 */
	PKCS7_ENC_CONTENT		*enc_data;
	} PKCS7_ENCRYPT;

typedef struct pkcs7_st
	{
	/* The following is non NULL if it contains ASN1 encoding of
	 * this structure */
	unsigned char *asn1;
	long length;

	ASN1_OBJECT *type;
	/* content as defined by the type */
	/* all encryption/message digests are applied to the 'contents',
	 * leaving out the 'type' field. */
	union	{
		char *ptr;

		/* NID_pkcs7_data */
		ASN1_OCTET_STRING *data;

		/* NID_pkcs7_signed */
		PKCS7_SIGNED *sign;

		/* NID_pkcs7_enveloped */
		PKCS7_ENVELOPE *enveloped;

		/* NID_pkcs7_signedAndEnveloped */
		PKCS7_SIGN_ENVELOPE *signed_and_enveloped;

		/* NID_pkcs7_digest */
		PKCS7_DIGEST *digest;

		/* NID_pkcs7_encrypted */
		PKCS7_ENCRYPT *encrypted;
		} d;
	} PKCS7;

#ifndef NOPROTO
PKCS7_ISSUER_AND_SERIAL *PKCS7_ISSUER_AND_SERIAL_new(void );
void			PKCS7_ISSUER_AND_SERIAL_free(
				PKCS7_ISSUER_AND_SERIAL *a);
int 			i2d_PKCS7_ISSUER_AND_SERIAL(
				PKCS7_ISSUER_AND_SERIAL *a,unsigned char **pp);
PKCS7_ISSUER_AND_SERIAL *d2i_PKCS7_ISSUER_AND_SERIAL(
				PKCS7_ISSUER_AND_SERIAL **a,
				unsigned char **pp, long length);


PKCS7_SIGNER_INFO	*PKCS7_SIGNER_INFO_new(void);
void			PKCS7_SIGNER_INFO_free(PKCS7_SIGNER_INFO *a);
int 			i2d_PKCS7_SIGNER_INFO(PKCS7_SIGNER_INFO *a,
				unsigned char **pp);
PKCS7_SIGNER_INFO	*d2i_PKCS7_SIGNER_INFO(PKCS7_SIGNER_INFO **a,
				unsigned char **pp,long length);

PKCS7_RECIP_INFO	*PKCS7_RECIP_INFO_new(void);
void			PKCS7_RECIP_INFO_free(PKCS7_RECIP_INFO *a);
int 			i2d_PKCS7_RECIP_INFO(PKCS7_RECIP_INFO *a,
				unsigned char **pp);
PKCS7_RECIP_INFO	*d2i_PKCS7_RECIP_INFO(PKCS7_RECIP_INFO **a,
				unsigned char **pp,long length);

PKCS7_SIGNED		*PKCS7_SIGNED_new(void);
void			PKCS7_SIGNED_free(PKCS7_SIGNED *a);
int 			i2d_PKCS7_SIGNED(PKCS7_SIGNED *a,
				unsigned char **pp);
PKCS7_SIGNED		*d2i_PKCS7_SIGNED(PKCS7_SIGNED **a,
				unsigned char **pp,long length);

PKCS7_ENC_CONTENT	*PKCS7_ENC_CONTENT_new(void);
void			PKCS7_ENC_CONTENT_free(PKCS7_ENC_CONTENT *a);
int 			i2d_PKCS7_ENC_CONTENT(PKCS7_ENC_CONTENT *a,
				unsigned char **pp);
PKCS7_ENC_CONTENT	*d2i_PKCS7_ENC_CONTENT(PKCS7_ENC_CONTENT **a,
				unsigned char **pp,long length);

PKCS7_ENVELOPE		*PKCS7_ENVELOPE_new(void);
void			PKCS7_ENVELOPE_free(PKCS7_ENVELOPE *a);
int 			i2d_PKCS7_ENVELOPE(PKCS7_ENVELOPE *a,
				unsigned char **pp);
PKCS7_ENVELOPE		*d2i_PKCS7_ENVELOPE(PKCS7_ENVELOPE **a,
				unsigned char **pp,long length);

PKCS7_SIGN_ENVELOPE	*PKCS7_SIGN_ENVELOPE_new(void);
void			PKCS7_SIGN_ENVELOPE_free(PKCS7_SIGN_ENVELOPE *a);
int 			i2d_PKCS7_SIGN_ENVELOPE(PKCS7_SIGN_ENVELOPE *a,
				unsigned char **pp);
PKCS7_SIGN_ENVELOPE	*d2i_PKCS7_SIGN_ENVELOPE(PKCS7_SIGN_ENVELOPE **a,
				unsigned char **pp,long length);

PKCS7_DIGEST		*PKCS7_DIGEST_new(void);
void			PKCS7_DIGEST_free(PKCS7_DIGEST *a);
int 			i2d_PKCS7_DIGEST(PKCS7_DIGEST *a,
				unsigned char **pp);
PKCS7_DIGEST		*d2i_PKCS7_DIGEST(PKCS7_DIGEST **a,
				unsigned char **pp,long length);

PKCS7_ENCRYPT		*PKCS7_ENCRYPT_new(void);
void			PKCS7_ENCRYPT_free(PKCS7_ENCRYPT *a);
int 			i2d_PKCS7_ENCRYPT(PKCS7_ENCRYPT *a,
				unsigned char **pp);
PKCS7_ENCRYPT		*d2i_PKCS7_ENCRYPT(PKCS7_ENCRYPT **a,
				unsigned char **pp,long length);

PKCS7			*PKCS7_new(void);
void			PKCS7_free(PKCS7 *a);
int 			i2d_PKCS7(PKCS7 *a,
				unsigned char **pp);
PKCS7			*d2i_PKCS7(PKCS7 **a,
				unsigned char **pp,long length);

#else

PKCS7_ISSUER_AND_SERIAL *PKCS7_ISSUER_AND_SERIAL_new();
void			PKCS7_ISSUER_AND_SERIAL_free();
int 			i2d_PKCS7_ISSUER_AND_SERIAL();
PKCS7_ISSUER_AND_SERIAL *d2i_PKCS7_ISSUER_AND_SERIAL();
PKCS7_SIGNER_INFO	*PKCS7_SIGNER_INFO_new();
void			PKCS7_SIGNER_INFO_free();
int 			i2d_PKCS7_SIGNER_INFO();
PKCS7_SIGNER_INFO	*d2i_PKCS7_SIGNER_INFO();
PKCS7_RECIP_INFO	*PKCS7_RECIP_INFO_new();
void			PKCS7_RECIP_INFO_free();
int 			i2d_PKCS7_RECIP_INFO();
PKCS7_RECIP_INFO	*d2i_PKCS7_RECIP_INFO();
PKCS7_SIGNED		*PKCS7_SIGNED_new();
void			PKCS7_SIGNED_free();
int 			i2d_PKCS7_SIGNED();
PKCS7_SIGNED		*d2i_PKCS7_SIGNED();
PKCS7_ENC_CONTENT	*PKCS7_ENC_CONTENT_new();
void			PKCS7_ENC_CONTENT_free();
int 			i2d_PKCS7_ENC_CONTENT();
PKCS7_ENC_CONTENT	*d2i_PKCS7_ENC_CONTENT();
PKCS7_ENVELOPE		*PKCS7_ENVELOPE_new();
void			PKCS7_ENVELOPE_free();
int 			i2d_PKCS7_ENVELOPE();
PKCS7_ENVELOPE		*d2i_PKCS7_ENVELOPE();
PKCS7_SIGN_ENVELOPE	*PKCS7_SIGN_ENVELOPE_new();
void			PKCS7_SIGN_ENVELOPE_free();
int 			i2d_PKCS7_SIGN_ENVELOPE();
PKCS7_SIGN_ENVELOPE	*d2i_PKCS7_SIGN_ENVELOPE();
PKCS7_DIGEST		*PKCS7_DIGEST_new();
void			PKCS7_DIGEST_free();
int 			i2d_PKCS7_DIGEST();
PKCS7_DIGEST		*d2i_PKCS7_DIGEST();
PKCS7_ENCRYPT		*PKCS7_ENCRYPT_new();
void			PKCS7_ENCRYPT_free();
int 			i2d_PKCS7_ENCRYPT();
PKCS7_ENCRYPT		*d2i_PKCS7_ENCRYPT();
PKCS7			*PKCS7_new();
void			PKCS7_free();
int 			i2d_PKCS7();
PKCS7			*d2i_PKCS7();

#endif

#ifdef  __cplusplus
}
#endif

#endif
