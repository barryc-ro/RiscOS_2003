/* crypto/x509/x509.h */
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

#ifndef HEADER_X509_H
#define HEADER_X509_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stack.h>
#include <asn1.h>

/*
#include "rsa.h"
#include "dsa.h"
#include "dh.h"
*/
#include "envelope.h"

#define X509_FILETYPE_PEM                       1
#define X509_FILETYPE_ASN1                      2

typedef struct X509_algor_st
	{
	ASN1_OBJECT *algorithm;
	ASN1_TYPE *parameter;
	} X509_ALGOR;

typedef struct X509_val_st
	{
	ASN1_UTCTIME *notBefore;
	ASN1_UTCTIME *notAfter;
	} X509_VAL;

typedef struct X509_pubkey_st
	{
	X509_ALGOR *algor;
	ASN1_BIT_STRING *public_key;
	} X509_PUBKEY;

typedef struct X509_sig_st
	{
	X509_ALGOR *algor;
	ASN1_BIT_STRING *digest;
	} X509_SIG;

typedef struct X509_name_entry_st
	{
	ASN1_OBJECT *object;
	ASN1_BIT_STRING *value;
	int set;
	int size; 	/* temp variable */
	} X509_NAME_ENTRY;

typedef struct X509_extension_st
	{
	ASN1_OBJECT *object;
	int critical;
	ASN1_OCTET_STRING *value;
	} X509_EXTENSION;

typedef struct X509_extension_method_st
	{
	int object;
	ASN1_METHOD *method;
	} X509_EXTENSION_METHOD;


#define X509_NAME	STACK /* of X509_NAME_ENTRY */

typedef struct X509_req_info_st
	{
	ASN1_INTEGER *version;
	X509_NAME *subject;
	X509_PUBKEY *pubkey;
	/*  d=2 hl=2 l=  0 cons: cont: 00 */
	STACK /* X509_ATTRIBUTE */ *attributes; /* [ 0 ] */
	int req_kludge;
	} X509_REQ_INFO;

typedef struct X509_req_st
	{
	X509_REQ_INFO *req_info;
	X509_ALGOR *sig_alg;
	ASN1_BIT_STRING *signature;
	int references;
	} X509_REQ;

typedef struct x509_cinf_st
	{
	ASN1_INTEGER *version;		/* [ 0 ] default of v1 */
	ASN1_INTEGER *serialNumber;
	X509_ALGOR *signature;
	X509_NAME *issuer;
	X509_VAL *validity;
	X509_NAME *subject;
	X509_PUBKEY *key;
	ASN1_BIT_STRING *issuerUID;		/* [ 1 ] optional in v2 */
	ASN1_BIT_STRING *subjectUID;		/* [ 2 ] optional in v2 */
	STACK /* X509_EXTENSION */ *extensions;	/* [ 3 ] optional in v3 */
	} X509_CINF;

typedef struct x509_st
	{
	X509_CINF *cert_info;
	X509_ALGOR *sig_alg;
	ASN1_BIT_STRING *signature;
	int references;
	} X509;

typedef struct X509_revoked_st
	{
	ASN1_INTEGER *serialNumber;
	ASN1_UTCTIME *revocationDate;
	int sequence; /* loaded sequence */
	} X509_REVOKED;

typedef struct X509_crl_info_st
	{
	X509_ALGOR *sig_alg;
	X509_NAME *issuer;
	ASN1_UTCTIME *lastUpdate;
	ASN1_UTCTIME *nextUpdate;
	STACK /* X509_REVOKED */ *revoked;
	} X509_CRL_INFO;

typedef struct X509_crl_st
	{
	/* actual signature */
	X509_CRL_INFO *crl;
	X509_ALGOR *sig_alg;
	ASN1_BIT_STRING *signature;
	int references;
	} X509_CRL;

/* a sequence of these are used */
typedef struct x509_attributes_st
	{
	/* Use the above macros on the type to get the 'set' order.  */
	ASN1_OBJECT *object;
	int nid;
	int set;
	union	{
		char		*ptr;
/* n */		STACK 		*set;
/* 1 */		ASN1_TYPE	*single;
		} value;
	} X509_ATTRIBUTE;

typedef struct private_key_st
	{
	int version;
	/* The PKCS#8 data types */
	X509_ALGOR *enc_algor;
	ASN1_OCTET_STRING *enc_pkey;	/* encrypted pub key */

	/* When decrypted, the following will not be NULL */
	EVP_PKEY *dec_pkey;

	/* used to encrypt and decrypt */
	int key_length;
	char *key_data;
	int key_free;	/* true if we should auto free key_data */

	/* expanded version of 'enc_algor' */
	EVP_CIPHER_INFO cipher;

	int references;
	} X509_PKEY;

#ifdef HEADER_ENVELOPE_H
typedef struct X509_info_st
	{
	X509 *x509;
	X509_CRL *crl;
	X509_PKEY *x_pkey;

	EVP_CIPHER_INFO enc_cipher;
	int enc_len;
	char *enc_data;

	int references;
	} X509_INFO;
#endif

/* The next 2 structures and their 8 routines were sent to me by
 * Pat Richard <patr@x509.com> and are used to manipulate
 * Netscapes spki strucutres - usefull if you are writing a CA web page
 */
typedef struct Netscape_spkac_st
	{
	X509_PUBKEY *pubkey;
	ASN1_IA5STRING *challenge;	/* challenge sent in atlas >= PR2 */
	} NETSCAPE_SPKAC;

typedef struct Netscape_spki_st
	{
	NETSCAPE_SPKAC *spkac;	/* signed public key and challenge */
	X509_ALGOR *sig_algor;
	ASN1_BIT_STRING *signature;
	} NETSCAPE_SPKI;

#ifndef HEADER_BN_H
#define BIGNUM 		char
#endif

#ifndef HEADER_ENVELOPE_H
#define EVP_MD		char
#endif

typedef struct CBCParameter_st
	{
	unsigned char iv[8];
	} CBC_PARAM;

typedef struct CFBParameter_st
	{
	unsigned char iv[8];
	int num_bits;		/* I only support 64 bits */
	} CFB_PARAM;

typedef struct AUTH_CTX_st
	{
	int num;	/* number of paths */
	int max;	/* num paths allocated */
	char **paths;
	} AUTH_CTX;

/* Outer object */
typedef struct X509_AUTH_st
	{
	int num_dirs;
	char **dirs;
	int *dirs_type;
	int num_dirs_alloced;

	int init;
	int cache; 	/* if true, stash any hits */
#ifdef HEADER_LHASH_H
	LHASH *certs;	/* cached certs; */ 
#else
	char *certs;
#endif
	int num_paths;	/* number of paths to files or directories */
	int num_alloced;
	char **paths;	/* the list of paths or directories */
	int *path_type;
	int (*get)();	/* get the certificate corresponding to
			 * passed subject */
	int (*verify)();/* Verify callback for if all else fails */
	} CERTIFICATE_CTX;

#define	VERIFY_OK					 0
#define	VERIFY_ROOT_OK					 1
#define	VERIFY_ERR_UNABLE_TO_GET_ISSUER			11
#define	VERIFY_ERR_UNABLE_TO_DECRYPT_SIGNATURE		12
#define	VERIFY_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY	13
#define	VERIFY_ERR_UNABLE_TO_UNPACK_SIGNATURE		14
#define	VERIFY_ERR_UNKNOWN_X509_SIG_ALGORITHM		15
#define	VERIFY_ERR_SIG_DIGEST_LENGTH_WRONG		16
#define	VERIFY_ERR_SIGNATURE_FAILURE			17
#define	VERIFY_ERR_CERT_NOT_YET_VALID			18
#define	VERIFY_ERR_CERT_HAS_EXPIRED			19
#define	VERIFY_ERR_ERROR_IN_NOT_BEFORE_FIELD		20
#define	VERIFY_ERR_ERROR_IN_NOT_AFTER_FIELD		21
#define	VERIFY_ERR_OUT_OF_MEM				22
#define	VERIFY_ERR_DEPTH_ZERO_SELF_SIGNED_CERT		23

#define X509_verify(a,r) ASN1_verify(i2d_X509_CINF,a->sig_alg,\
	a->signature,(char *)a->cert_info,r)

#define X509_REQ_verify(a,r) ASN1_verify(i2d_X509_REQ_INFO,a->sig_alg,\
	a->signature,(char *)a->req_info,r)

#define X509_CRL_verify(a,r) ASN1_verify(i2d_X509_CRL_INFO,a->sig_alg,\
	a->signature,(char *)a->crl,r)

#define NETSCAPE_SPKI_verify(a,r) ASN1_sign(i2d_NETSCAPE_SPKAC,a->sig_algor,\
		a->signature, (char *)a->spkac,r)

#define X509_sign(x,pkey,md) \
	ASN1_sign(i2d_X509_CINF, x->cert_info->signature, x->sig_alg, \
		x->signature, (char *)x->cert_info,pkey,md)

#define X509_REQ_sign(x,pkey,md) \
	ASN1_sign(i2d_X509_REQ_INFO,x->sig_alg, NULL, \
		x->signature, (char *)x->req_info,pkey,md)

#define X509_CRL_sign(x,pkey,md) \
	ASN1_sign(i2d_X509_CRL_INFO,x->crl->sig_alg,x->sig_alg, \
		x->signature, (char *)x->crl,pkey,md)

#define NETSCAPE_SPKI_sign(x,pkey,md) \
	ASN1_sign(i2d_NETSCAPE_SPKAC, x->sig_algor,NULL, \
		x->signature, (char *)x->spkac,pkey,md)

#define X509_dup(x509) (X509 *)ASN1_dup(i2d_X509, \
		(char *(*)())d2i_X509,(char *)x509)
#define d2i_X509_fp(fp,x509) (X509 *)ASN1_d2i_fp((char *(*)())X509_new, \
		(char *(*)())d2i_X509, (fp),(unsigned char **)(x509))
#define i2d_X509_fp(fp,x509) ASN1_i2d_fp(i2d_X509,fp,(unsigned char *)x509)
#define d2i_X509_bio(bp,x509) (X509 *)ASN1_d2i_bio((char *(*)())X509_new, \
		(char *(*)())d2i_X509, (bp),(unsigned char **)(x509))
#define i2d_X509_bio(bp,x509) ASN1_i2d_bio(i2d_X509,bp,(unsigned char *)x509)

#define X509_CRL_dup(crl) (X509_CRL *)ASN1_dup(i2d_X509_CRL, \
		(char *(*)())d2i_X509_CRL,(char *)crl)
#define d2i_X509_CRL_fp(fp,crl) (X509_CRL *)ASN1_d2i_fp((char *(*)()) \
		X509_CRL_new,(char *(*)())d2i_X509_CRL, (fp),\
		(unsigned char **)(crl))
#define i2d_X509_CRL_fp(fp,crl) ASN1_i2d_fp(i2d_X509_CRL,fp,\
		(unsigned char *)crl)
#define d2i_X509_CRL_bio(bp,crl) (X509_CRL *)ASN1_d2i_bio((char *(*)()) \
		X509_CRL_new,(char *(*)())d2i_X509_CRL, (bp),\
		(unsigned char **)(crl))
#define i2d_X509_CRL_bio(bp,crl) ASN1_i2d_bio(i2d_X509_CRL,bp,\
		(unsigned char *)crl)

#define PKCS7_dup(p7) (PKCS7 *)ASN1_dup(i2d_PKCS7, \
		(char *(*)())d2i_PKCS7,(char *)p7)
#define d2i_PKCS7_fp(fp,p7) (PKCS7 *)ASN1_d2i_fp((char *(*)()) \
		PKCS7_new,(char *(*)())d2i_PKCS7, (fp),\
		(unsigned char **)(p7))
#define i2d_PKCS7_fp(fp,p7) ASN1_i2d_fp(i2d_PKCS7,fp,\
		(unsigned char *)p7)
#define d2i_PKCS7_bio(bp,p7) (PKCS7 *)ASN1_d2i_bio((char *(*)()) \
		PKCS7_new,(char *(*)())d2i_PKCS7, (bp),\
		(unsigned char **)(p7))
#define i2d_PKCS7_bio(bp,p7) ASN1_i2d_bio(i2d_PKCS7,bp,\
		(unsigned char *)p7)

#define X509_REQ_dup(req) (X509_REQ *)ASN1_dup(i2d_X509_REQ, \
		(char *(*)())d2i_X509_REQ,(char *)req)
#define d2i_X509_REQ_fp(fp,req) (X509_REQ *)ASN1_d2i_fp((char *(*)())\
		X509_REQ_new, (char *(*)())d2i_X509_REQ, (fp),\
		(unsigned char **)(req))
#define i2d_X509_REQ_fp(fp,req) ASN1_i2d_fp(i2d_X509_REQ,fp,\
		(unsigned char *)req)
#define d2i_X509_REQ_bio(bp,req) (X509_REQ *)ASN1_d2i_bio((char *(*)())\
		X509_REQ_new, (char *(*)())d2i_X509_REQ, (bp),\
		(unsigned char **)(req))
#define i2d_X509_REQ_bio(bp,req) ASN1_i2d_bio(i2d_X509_REQ,bp,\
		(unsigned char *)req)

#define RSAPublicKey_dup(rsa) (RSA *)ASN1_dup(i2d_RSAPublikKey, \
		(char *(*)())d2i_RSAPublicKey,(char *)rsa)
#define RSAPrivateKey_dup(rsa) (RSA *)ASN1_dup(i2d_RSAPrivateKey, \
		(char *(*)())d2i_RSAPrivateKey,(char *)rsa)
#define d2i_RSAPrivateKey_fp(fp,rsa) (RSA *)ASN1_d2i_fp((char *(*)())\
		RSA_new,(char *(*)())d2i_RSAPrivateKey, (fp), \
		(unsigned char **)(rsa))
#define i2d_RSAPrivateKey_fp(fp,rsa) ASN1_i2d_fp(i2d_RSAPrivateKey,fp, \
		(unsigned char *)rsa)
#define d2i_RSAPrivateKey_bio(bp,rsa) (RSA *)ASN1_d2i_bio((char *(*)())\
		RSA_new,(char *(*)())d2i_RSAPrivateKey, (bp), \
		(unsigned char **)(rsa))
#define i2d_RSAPrivateKey_bio(bp,rsa) ASN1_i2d_bio(i2d_RSAPrivateKey,bp, \
		(unsigned char *)rsa)

#define X509_NAME_dup(xn) (X509_NAME *)ASN1_dup(i2d_X509_NAME, \
		(char *(*)())d2i_X509_NAME,(char *)xn)
#define X509_NAME_ENTRY_dup(ne) (X509_NAME_ENTRY *)ASN1_dup( \
		i2d_X509_NAME_ENTRY, (char *(*)())d2i_X509_NAME_ENTRY,\
		(char *)ne)

#define X509_EXTENSION_dup(ex) (X509_EXTENSION *)ASN1_dup( \
		(int (*)())i2d_X509_EXTENSION, \
		(char *(*)())d2i_X509_EXTENSION,(char *)ex)

#ifndef NOPROTO
X509 *		X509_get_cert(CERTIFICATE_CTX *ctx,X509_NAME * name,
		X509 *tmp_x509);
int  		X509_add_cert(CERTIFICATE_CTX *ctx,X509 *);

char *		X509_cert_verify_error_string(int n);
int 		X509_add_cert_file(CERTIFICATE_CTX *c,char *file, int type);
int		X509_cmp_current_time(char *s);
char *		X509_gmtime_adj(char *s, long adj);
int		X509_add_cert_dir (CERTIFICATE_CTX *c,char *dir, int type);
int		X509_load_verify_locations (CERTIFICATE_CTX *ctx,
			char *file_env, char *dir_env);
int		X509_set_default_verify_paths(CERTIFICATE_CTX *cts);
char *		X509_get_default_cert_area(void );
char *		X509_get_default_cert_dir(void );
char *		X509_get_default_cert_file(void );
char *		X509_get_default_cert_dir_env(void );
char *		X509_get_default_cert_file_env(void );
char *		X509_get_default_private_dir(void );
X509_REQ *	X509_to_X509_REQ(X509 *x, EVP_PKEY *pkey);
X509 *		X509_REQ_to_X509(X509_REQ *r, int days,EVP_PKEY *pkey);
int		X509_cert_verify(CERTIFICATE_CTX *ctx,X509 *xs, int (*cb)()); 

void ERR_load_X509_strings(void );

CERTIFICATE_CTX *CERTIFICATE_CTX_new(void );
void CERTIFICATE_CTX_free(CERTIFICATE_CTX *c);

X509_ALGOR *	X509_ALGOR_new(void );
void		X509_ALGOR_free(X509_ALGOR *a);
int		i2d_X509_ALGOR(X509_ALGOR *a,unsigned char **pp);
X509_ALGOR *	d2i_X509_ALGOR(X509_ALGOR **a,unsigned char **pp,
			long length);

X509_VAL *	X509_VAL_new(void );
void		X509_VAL_free(X509_VAL *a);
int		i2d_X509_VAL(X509_VAL *a,unsigned char **pp);
X509_VAL *	d2i_X509_VAL(X509_VAL **a,unsigned char **pp,
			long length);

X509_PUBKEY *	X509_PUBKEY_new(void );
void		X509_PUBKEY_free(X509_PUBKEY *a);
int		i2d_X509_PUBKEY(X509_PUBKEY *a,unsigned char **pp);
X509_PUBKEY *	d2i_X509_PUBKEY(X509_PUBKEY **a,unsigned char **pp,
			long length);

X509_SIG *	X509_SIG_new(void );
void		X509_SIG_free(X509_SIG *a);
int		i2d_X509_SIG(X509_SIG *a,unsigned char **pp);
X509_SIG *	d2i_X509_SIG(X509_SIG **a,unsigned char **pp,long length);

X509_REQ_INFO *X509_REQ_INFO_new(void);
void		X509_REQ_INFO_free(X509_REQ_INFO *a);
int		i2d_X509_REQ_INFO(X509_REQ_INFO *a,unsigned char **pp);
X509_REQ_INFO *d2i_X509_REQ_INFO(X509_REQ_INFO **a,unsigned char **pp,
			long length);

X509_REQ *	X509_REQ_new(void);
void		X509_REQ_free(X509_REQ *a);
int		i2d_X509_REQ(X509_REQ *a,unsigned char **pp);
X509_REQ *	d2i_X509_REQ(X509_REQ **a,unsigned char **pp,long length);

X509_ATTRIBUTE *X509_ATTRIBUTE_new(void );
void		X509_ATTRIBUTE_free(X509_ATTRIBUTE *a);
int		i2d_X509_ATTRIBUTE(X509_ATTRIBUTE *a,unsigned char **pp);
X509_ATTRIBUTE *d2i_X509_ATTRIBUTE(X509_ATTRIBUTE **a,unsigned char **pp,
			long length);

X509_NAME_ENTRY *X509_NAME_ENTRY_new(void);
void		X509_NAME_ENTRY_free(X509_NAME_ENTRY *a);
int		i2d_X509_NAME_ENTRY(X509_NAME_ENTRY *a,unsigned char **pp);
X509_NAME_ENTRY *d2i_X509_NAME_ENTRY(X509_NAME_ENTRY **a,unsigned char **pp,
			long length);

X509_NAME *	X509_NAME_new(void);
void		X509_NAME_free(X509_NAME *a);
int		i2d_X509_NAME(X509_NAME *a,unsigned char **pp);
X509_NAME *	d2i_X509_NAME(X509_NAME **a,unsigned char **pp,long length);

X509_CINF *	X509_CINF_new(void);
void		X509_CINF_free(X509_CINF *a);
int		i2d_X509_CINF(X509_CINF *a,unsigned char **pp);
X509_CINF *	d2i_X509_CINF(X509_CINF **a,unsigned char **pp,long length);

X509 *		X509_new(void);
void		X509_free(X509 *a);
int		i2d_X509(X509 *a,unsigned char **pp);
X509 *		d2i_X509(X509 **a,unsigned char **pp,long length);

X509_REVOKED *	X509_REVOKED_new(void);
void		X509_REVOKED_free(X509_REVOKED *a);
int		i2d_X509_REVOKED(X509_REVOKED *a,unsigned char **pp);
X509_REVOKED *	d2i_X509_REVOKED(X509_REVOKED **a,unsigned char **pp,long length);

X509_CRL_INFO *X509_CRL_INFO_new(void);
void		X509_CRL_INFO_free(X509_CRL_INFO *a);
int		i2d_X509_CRL_INFO(X509_CRL_INFO *a,unsigned char **pp);
X509_CRL_INFO *d2i_X509_CRL_INFO(X509_CRL_INFO **a,unsigned char **pp,
			long length);

X509_CRL *	X509_CRL_new(void);
void		X509_CRL_free(X509_CRL *a);
int		i2d_X509_CRL(X509_CRL *a,unsigned char **pp);
X509_CRL *	d2i_X509_CRL(X509_CRL **a,unsigned char **pp,long length);

X509_PKEY *	X509_PKEY_new();
void		X509_PKEY_free(X509_PKEY *a);
int		i2d_X509_PKEY(X509_PKEY *a,unsigned char **pp);
X509_PKEY *	d2i_X509_PKEY(X509_PKEY **a,unsigned char **pp,long length);

NETSCAPE_SPKI *	NETSCAPE_SPKI_new(void );
void		NETSCAPE_SPKI_free(NETSCAPE_SPKI *a);
int		i2d_NETSCAPE_SPKI(NETSCAPE_SPKI *a,unsigned char **pp);
NETSCAPE_SPKI *	d2i_NETSCAPE_SPKI(NETSCAPE_SPKI **a,unsigned char **pp,
			long length);

NETSCAPE_SPKAC *NETSCAPE_SPKAC_new(void );
void		NETSCAPE_SPKAC_free(NETSCAPE_SPKAC *a);
int		i2d_NETSCAPE_SPKAC(NETSCAPE_SPKAC *a,unsigned char **pp);
NETSCAPE_SPKAC *d2i_NETSCAPE_SPKAC(NETSCAPE_SPKAC **a,unsigned char **pp,
		long length);

X509_EXTENSION *X509_EXTENSION_new(void );
void		X509_EXTENSION_free(X509_EXTENSION *a);
int		i2d_X509_EXTENSION(X509_EXTENSION *a,unsigned char **pp);
X509_EXTENSION *d2i_X509_EXTENSION(X509_EXTENSION **a,unsigned char **pp,
			long length);

#ifdef HEADER_ENVELOPE_H
X509_INFO *	X509_INFO_new(void);
void		X509_INFO_free(X509_INFO *a);
#endif

char *		X509_NAME_oneline(X509_NAME *a);

int ASN1_verify(int (*i2d)(), X509_ALGOR *algor1,
	ASN1_BIT_STRING *signature,char *data,EVP_PKEY *pkey);

int ASN1_sign(int (*i2d)(), X509_ALGOR *algor1, X509_ALGOR *algor2,
	ASN1_BIT_STRING *signature,
	char *data,EVP_PKEY *pkey, EVP_MD *type);

EVP_PKEY *	X509_REQ_extract_key(X509_REQ *req);
EVP_PKEY *	X509_extract_key(X509 *x509);

int		X509_issuer_and_serial_cmp(X509 *a, X509 *b);
unsigned long	X509_issuer_and_serial_hash(X509 *a);

X509_NAME *	X509_get_issuer_name(X509 *a);
int		X509_issuer_name_cmp(X509 *a, X509 *b);
unsigned long	X509_issuer_name_hash(X509 *a);

X509_NAME *	X509_get_subject_name(X509 *a);
int		X509_subject_name_cmp(X509 *a,X509 *b);
unsigned long	X509_subject_name_hash(X509 *x);

int		X509_name_cmp (X509_NAME *a, X509_NAME *b);
unsigned long	X509_name_hash(X509_NAME *x);

#ifndef WIN16
int		X509_print_fp(FILE *bp,X509 *x);
int		X509_REQ_print_fp(FILE *bp,X509_REQ *req);
#endif

#ifdef HEADER_BUFFER_H
int		X509_NAME_print(BIO *bp, X509_NAME *name, int obase);
int		X509_print(BIO *bp,X509 *x);
int		X509_REQ_print(BIO *bp,X509_REQ *req);
#else
int		X509_NAME_print(char *bp, X509_NAME *name, int obase);
int		X509_print(char *bp,X509 *x);
int		X509_REQ_print(char *bp,X509_REQ *req);
#endif

#else

X509 *		X509_get_cert();
int  		X509_add_cert();
char *		X509_cert_verify_error_string();
int 		X509_add_cert_file();
int		X509_cmp_current_time();
char *		X509_gmtime_adj();
int		X509_add_cert_dir ();
int		X509_load_verify_locations ();
int		X509_set_default_verify_paths();
char *		X509_get_default_cert_area();
char *		X509_get_default_cert_dir();
char *		X509_get_default_cert_file();
char *		X509_get_default_cert_dir_env();
char *		X509_get_default_cert_file_env();
char *		X509_get_default_private_dir();
X509_REQ *	X509_to_X509_REQ();
X509 *		X509_REQ_to_X509();
int		X509_cert_verify();
void		ERR_load_X509_strings();
CERTIFICATE_CTX *CERTIFICATE_CTX_new();
void		CERTIFICATE_CTX_free();
X509_ALGOR *	X509_ALGOR_new();
void		X509_ALGOR_free();
int		i2d_X509_ALGOR();
X509_ALGOR *	d2i_X509_ALGOR();
X509_VAL *	X509_VAL_new();
void		X509_VAL_free();
int		i2d_X509_VAL();
X509_VAL *	d2i_X509_VAL();
X509_PUBKEY *	X509_PUBKEY_new();
void		X509_PUBKEY_free();
int		i2d_X509_PUBKEY();
X509_PUBKEY *	d2i_X509_PUBKEY();
X509_SIG *	X509_SIG_new();
void		X509_SIG_free();
int		i2d_X509_SIG();
X509_SIG *	d2i_X509_SIG();
X509_REQ_INFO *X509_REQ_INFO_new();
void		X509_REQ_INFO_free();
int		i2d_X509_REQ_INFO();
X509_REQ_INFO *d2i_X509_REQ_INFO();
X509_REQ *	X509_REQ_new();
void		X509_REQ_free();
int		i2d_X509_REQ();
X509_REQ *	d2i_X509_REQ();
X509_ATTRIBUTE *X509_ATTRIBUTE_new();
void		X509_ATTRIBUTE_free();
int		i2d_X509_ATTRIBUTE();
X509_ATTRIBUTE *d2i_X509_ATTRIBUTE();
X509_NAME_ENTRY *X509_NAME_ENTRY_new();
void		X509_NAME_ENTRY_free();
int		i2d_X509_NAME_ENTRY();
X509_NAME_ENTRY *d2i_X509_NAME_ENTRY();
X509_NAME *	X509_NAME_new();
void		X509_NAME_free();
int		i2d_X509_NAME();
X509_NAME *	d2i_X509_NAME();
X509_CINF *	X509_CINF_new();
void		X509_CINF_free();
int		i2d_X509_CINF();
X509_CINF *	d2i_X509_CINF();
X509 *		X509_new();
void		X509_free();
int		i2d_X509();
X509 *		d2i_X509();
X509_REVOKED *	X509_REVOKED_new();
void		X509_REVOKED_free();
int		i2d_X509_REVOKED();
X509_REVOKED *	d2i_X509_REVOKED();
X509_CRL_INFO *X509_CRL_INFO_new();
void		X509_CRL_INFO_free();
int		i2d_X509_CRL_INFO();
X509_CRL_INFO *d2i_X509_CRL_INFO();
X509_CRL *	X509_CRL_new();
void		X509_CRL_free();
int		i2d_X509_CRL();
X509_CRL *	d2i_X509_CRL();
X509_PKEY *	X509_PKEY_new();
void		X509_PKEY_free();
int		i2d_X509_PKEY();
X509_PKEY *	d2i_X509_PKEY();
#ifdef HEADER_ENVELOPE_H
X509_INFO *	X509_INFO_new();
void		X509_INFO_free();
#endif
char *		X509_NAME_oneline();
int		ASN1_sign();
int		ASN1_verify();
EVP_PKEY *	X509_REQ_extract_key();
EVP_PKEY *	X509_extract_key();
int		X509_issuer_and_serial_cmp();
unsigned long	X509_issuer_and_serial_hash();
X509_NAME *	X509_get_issuer_name();
int		X509_issuer_name_cmp();
unsigned long	X509_issuer_name_hash();
X509_NAME *	X509_get_subject_name();
int		X509_subject_name_cmp();
unsigned long	X509_subject_name_hash();
int		X509_name_cmp();
unsigned long	X509_name_hash();

#ifndef WIN16
int		X509_print_fp();
int		X509_REQ_print_fp();
#endif

int		X509_NAME_print();
int		X509_print();
int		X509_REQ_print();
NETSCAPE_SPKAC *NETSCAPE_SPKAC_new();
void		NETSCAPE_SPKAC_free();
int		i2d_NETSCAPE_SPKAC();
NETSCAPE_SPKAC *d2i_NETSCAPE_SPKAC();
NETSCAPE_SPKI *NETSCAPE_SPKI_new();
void		NETSCAPE_SPKI_free();
int		i2d_NETSCAPE_SPKI();
NETSCAPE_SPKI *d2i_NETSCAPE_SPKI();

X509_EXTENSION *X509_EXTENSION_new();
void		X509_EXTENSION_free();
int		i2d_X509_EXTENSION();
X509_EXTENSION *d2i_X509_EXTENSION();

#endif
    
/* BEGIN ERROR CODES */
/* Error codes for the X509 functions. */

/* Function codes. */
#define X509_F_INIT_CERTS				 100
#define X509_F_X509_ADD_CERT				 101
#define X509_F_X509_ADD_CERT_DIR			 102
#define X509_F_X509_ADD_CERT_FILE			 103
#define X509_F_X509_EXTRACT_KEY				 104
#define X509_F_X509_GET_CERT				 105
#define X509_F_X509_NAME_ONELINE			 106
#define X509_F_X509_NAME_PRINT				 107
#define X509_F_X509_PRINT_FP				 108
#define X509_F_X509_REQ_PRINT				 109
#define X509_F_X509_REQ_PRINT_FP			 110
#define X509_F_X509_REQ_TO_X509				 111
#define X509_F_X509_SET_DEFAULT_VERIFY_PATHS		 112
#define X509_F_X509_TO_X509_REQ				 113

/* Reason codes. */
#define X509_R_BAD_PUBLIC_KEY_TYPE			 100
#define X509_R_BAD_X509_FILETYPE			 101
#define X509_R_CERT_ALREADY_IN_HASH_TABLE		 102
#define X509_R_ERR_ASN1_LIB				 103
#define X509_R_LOADING_DEFAULTS				 104
#define X509_R_LOADING_DIR_ENV_VAR			 105
#define X509_R_LOADING_FILE_ENV_VAR			 106

#ifdef  __cplusplus
}
#endif
#endif

