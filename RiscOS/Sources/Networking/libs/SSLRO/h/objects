/* crypto/objects/objects.h */
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

#ifndef HEADER_OBJECTS_H
#define HEADER_OBJECTS_H

#ifdef  __cplusplus
extern "C" {
#endif

#define SN_undef			"UNDEF"
#define LN_undef			"undefined"
#define NID_undef			0

#define SN_Algorithm			"Algorithm"
#define LN_algorithm			"algorithm"
#define NID				38
#define OBJ_algorithm			1L,3L,14L,3L,2L

#define LN_rsadsi			"rsadsi"
#define NID_rsadsi			1
#define OBJ_rsadsi			1L,2L,840L,113549L

#define LN_pkcs				"pkcs"
#define NID_pkcs			2
#define OBJ_pkcs			OBJ_rsadsi,1L

#define SN_md2				"MD2"
#define LN_md2				"md2"
#define NID_md2				3
#define OBJ_md2				OBJ_rsadsi,2L,2L

#define SN_md5				"MD5"
#define LN_md5				"md5"
#define NID_md5				4
#define OBJ_md5				OBJ_rsadsi,2L,5L

#define SN_rc4				"RC4"
#define LN_rc4				"rc4"
#define NID_rc4				5
#define OBJ_rc4				OBJ_rsadsi,3L,4L

#define LN_rsaEncryption		"rsaEncryption"
#define NID_rsaEncryption		6
#define OBJ_rsaEncryption		OBJ_pkcs,1L,1L

#define SN_md2withRSAEncryption		"RSA-MD2"
#define LN_md2withRSAEncryption		"md2withRSAEncryption"
#define NID_md2withRSAEncryption	7
#define OBJ_md2withRSAEncryption	OBJ_pkcs,1L,2L

#define SN_md5withRSAEncryption		"RSA-MD5"
#define LN_md5withRSAEncryption		"md5withRSAEncryption"
#define NID_md5withRSAEncryption	8
#define OBJ_md5withRSAEncryption	OBJ_pkcs,1L,4L

#define LN_pbeWithMD2AndDES_CBC		"pbeWithMD2AndDES-CBC"
#define NID_pbeWithMD2AndDES_CBC	9
#define OBJ_pbeWithMD2AndDES_CBC	OBJ_pkcs,5L,1L

#define LN_pbeWithMD5AndDES_CBC		"pbeWithMD5AndDES-CBC"
#define NID_pbeWithMD5AndDES_CBC	10
#define OBJ_pbeWithMD5AndDES_CBC	OBJ_pkcs,5L,3L

#define LN_X500				"X500"
#define NID_X500			11
#define OBJ_X500			2L,5L

#define LN_X509				"X509"
#define NID_X509			12
#define OBJ_X509			OBJ_X500,4L

#define SN_commonName			"CN"
#define LN_commonName			"commonName"
#define NID_commonName			13
#define OBJ_commonName			OBJ_X509,3L

#define SN_countryName			"C"
#define LN_countryName			"countryName"
#define NID_countryName			14
#define OBJ_countryName			OBJ_X509,6L

#define SN_localityName			"L"
#define LN_localityName			"localityName"
#define NID_localityName		15
#define OBJ_localityName		OBJ_X509,7L

/* Postal Address? PA */

#define SN_stateOrProvinceName		"SP"
#define LN_stateOrProvinceName		"stateOrProvinceName"
#define NID_stateOrProvinceName		16
#define OBJ_stateOrProvinceName		OBJ_X509,8L

#define SN_organizationName		"O"
#define LN_organizationName		"organizationName"
#define NID_organizationName		17
#define OBJ_organizationName		OBJ_X509,10L

#define SN_organizationalUnitName	"OU"
#define LN_organizationalUnitName	"organizationalUnitName"
#define NID_organizationalUnitName	18
#define OBJ_organizationalUnitName	OBJ_X509,11L

#define SN_rsa				"RSA"
#define LN_rsa				"rsa"
#define NID_rsa				19
#define OBJ_rsa				OBJ_X500,8L,1L,1L

#define LN_pkcs7			"pkcs7"
#define NID_pkcs7			20
#define OBJ_pkcs7			OBJ_pkcs,7L

#define LN_pkcs7_data			"pkcs7-data"
#define NID_pkcs7_data			21
#define OBJ_pkcs7_data			OBJ_pkcs7,1L

#define LN_pkcs7_signed			"pkcs7-signedData"
#define NID_pkcs7_signed		22
#define OBJ_pkcs7_signed		OBJ_pkcs7,2L

#define LN_pkcs7_enveloped		"pkcs7-envelopedData"
#define NID_pkcs7_enveloped		23
#define OBJ_pkcs7_enveloped		OBJ_pkcs7,3L

#define LN_pkcs7_signedAndEnveloped	"pkcs7-signedAndEnvelopedData"
#define NID_pkcs7_signedAndEnveloped	24
#define OBJ_pkcs7_signedAndEnveloped	OBJ_pkcs7,4L

#define LN_pkcs7_digest			"pkcs7-digestData"
#define NID_pkcs7_digest		25
#define OBJ_pkcs7_digest		OBJ_pkcs7,5L

#define LN_pkcs7_encrypted		"pkcs7-encryptedData"
#define NID_pkcs7_encrypted		26
#define OBJ_pkcs7_encrypted		OBJ_pkcs7,6L

#define LN_pkcs3			"pkcs3"
#define NID_pkcs3			27
#define OBJ_pkcs3			OBJ_pkcs,3L

#define LN_dhKeyAgreement		"dhKeyAgreement"
#define NID_dhKeyAgreement		28
#define OBJ_dhKeyAgreement		OBJ_pkcs3,1L

#define SN_des_ecb			"DES-ECB"
#define LN_des_ecb			"des-ecb"
#define NID_des_ecb			29
#define OBJ_des_ecb			OBJ_algorithm,6L

#define SN_des_cfb			"DES-CFB"
#define LN_des_cfb			"des-cfb"
#define NID_des_cfb			30
#define OBJ_des_cfb			OBJ_algorithm,9L

#define SN_des_cbc			"DES-CBC"
#define LN_des_cbc			"des-cbc"
#define NID_des_cbc			31
#define OBJ_des_cbc			OBJ_algorithm,7L

#define SN_des_ede			"DES-EDE"
#define LN_des_ede			"des-ede"
#define NID_des_ede			32

#define SN_des_ede3			"DES-EDE3"
#define LN_des_ede3			"des-ede3"
#define NID_des_ede3			33
#define OBJ_des_ede3			OBJ_rsadsi,3L,17L

#define SN_idea_cbc			"IDEA-CBC"
#define LN_idea_cbc			"idea-cbc"
#define NID_idea_cbc			34

#define SN_idea_cfb			"IDEA-CFB"
#define LN_idea_cfb			"idea-cfb"
#define NID_idea_cfb			35

#define SN_idea_ecb			"IDEA-ECB"
#define LN_idea_ecb			"idea-ecb"
#define NID_idea_ecb			36

#define SN_rc2_cbc			"RC2-CBC"
#define LN_rc2_cbc			"rc2-cbc"
#define NID_rc2_cbc			37
#define OBJ_rc2_cbc			OBJ_rsadsi,3L,2L

#define SN_rc2_ecb			"RC2-ECB"
#define LN_rc2_ecb			"rc2-ecb"
#define NID_rc2_ecb			38

#define SN_rc2_cfb			"RC2-CFB"
#define LN_rc2_cfb			"rc2-cfb"
#define NID_rc2_cfb			39

#define SN_rc2_ofb			"RC2-OFB"
#define LN_rc2_ofb			"rc2-ofb"
#define NID_rc2_ofb			40

#define SN_sha				"SHA"
#define LN_sha				"sha"
#define NID_sha				41
#define OBJ_sha				OBJ_algorithm,18L

#define SN_shaWithRSAEncryption		"RSA-SHA"
#define LN_shaWithRSAEncryption		"shaWithRSAEncryption"
#define NID_shaWithRSAEncryption	42
#define OBJ_shawithRSAEncryption	OBJ_algorithm,15L

#define SN_des_ede_cbc			"DES-EDE-CBC"
#define LN_des_ede_cbc			"des-ede-cbc"
#define NID_des_ede_cbc			43

#define SN_des_ede3_cbc			"DES-EDE3-CBC"
#define LN_des_ede3_cbc			"des-ede3-cbc"
#define NID_des_ede3_cbc		44
#define OBJ_des_ede3_cbc		OBJ_rsadsi,3L,7L

#define SN_des_ofb			"DES-OFB"
#define LN_des_ofb			"des-ofb"
#define NID_des_ofb			45
#define OBJ_des_ofb			OBJ_algorithm,8L

#define SN_idea_ofb			"IDEA-OFB"
#define LN_idea_ofb			"idea-ofb"
#define NID_idea_ofb			46

#define LN_pkcs9			"pkcs9"
#define NID_pkcs9			47
#define OBJ_pkcs9			OBJ_pkcs,9L

#define SN_pkcs9_emailAddress		"Email"
#define LN_pkcs9_emailAddress		"emailAddress"
#define NID_pkcs9_emailAddress		48
#define OBJ_pkcs9_emailAddress		OBJ_pkcs9,1L

#define LN_pkcs9_unstructuredName	"unstructuredName"
#define NID_pkcs9_unstructuredName	49
#define OBJ_pkcs9_unstructuredName	OBJ_pkcs9,2L

#define LN_pkcs9_contentType		"contentType"
#define NID_pkcs9_contentType		50
#define OBJ_pkcs9_contentType		OBJ_pkcs9,3L

#define LN_pkcs9_messageDigest		"messageDigest"
#define NID_pkcs9_messageDigest		51
#define OBJ_pkcs9_messageDigest		OBJ_pkcs9,4L

#define LN_pkcs9_signingTime		"signingTime"
#define NID_pkcs9_signingTime		52
#define OBJ_pkcs9_signingTime		OBJ_pkcs9,5L

#define LN_pkcs9_countersignature	"countersignature"
#define NID_pkcs9_countersignature	53
#define OBJ_pkcs9_countersignature	OBJ_pkcs9,6L

#define LN_pkcs9_challengePassword	"challengePassword"
#define NID_pkcs9_challengePassword	54
#define OBJ_pkcs9_challengePassword	OBJ_pkcs9,7L

#define LN_pkcs9_unstructuredAddress	"unstructuredAddress"
#define NID_pkcs9_unstructuredAddress	55
#define OBJ_pkcs9_unstructuredAddress	OBJ_pkcs9,8L

#define LN_pkcs9_extCertAttributes	"extendedCertificateAttributes"
#define NID_pkcs9_extCertAttributes	56
#define OBJ_pkcs9_extCertAttributes	OBJ_pkcs9,9L

#define SN_netscape			"Netscape"
#define LN_netscape			"Netscape Communications Corp."
#define NID_netscape			57
/* #define OBJ_netscape			2L,16L,840L,113730L */
#define OBJ_netscape			2L,16L,840L,11370L

#define LN_netscape_gif2		"ns-gif-2"
#define NID_netscape_gif2		58
#define OBJ_netscape_gif2		2L,16L,840L,11370L,1L,2L

#define LN_netscape_gif3		"ns-gif-3"
#define NID_netscape_gif3		59
#define OBJ_netscape_gif3		2L,16L,840L,11370L,1L,3L

#define SN_des_ede_cfb			"DES-EDE-CFB"
#define LN_des_ede_cfb			"des-ede-cfb"
#define NID_des_ede_cfb			60

#define SN_des_ede3_cfb			"DES-EDE3-CFB"
#define LN_des_ede3_cfb			"des-ede3-cfb"
#define NID_des_ede3_cfb		61

#define SN_des_ede_ofb			"DES-EDE-OFB"
#define LN_des_ede_ofb			"des-ede-ofb"
#define NID_des_ede_ofb			62

#define SN_des_ede3_ofb			"DES-EDE3-OFB"
#define LN_des_ede3_ofb			"des-ede3-ofb"
#define NID_des_ede3_ofb		63

/* I'm not sure about the object ID */
#define SN_sha1				"SHA1"
#define LN_sha1				"sha1"
#define NID_sha1			64
#define OBJ_sha1			OBJ_algorithm,3L,2L,26L

#define SN_sha1WithRSAEncryption	"RSA-SHA1"
#define LN_sha1WithRSAEncryption	"sha1WithRSAEncryption"
#define NID_sha1WithRSAEncryption	65

#define SN_dsaWithSHA			"DSS-SHA"
#define LN_dsaWithSHA			"shaWithDSSEncryption"
#define NID_dsaWithSHA			66
#define OBJ_dsaWithSHA			OBJ_algorithm,12L

#define SN_dss				"DSS"
#define LN_dss				"dssEncryption"
#define NID_dss				67
#define OBJ_dss				OBJ_algorithm,13L

/* proposed by microsoft to RSA */
#define LN_pbeWithSHA1AndRC2_CBC	"pbeWithSHA1AndRC2-CBC"
#define NID_pbeWithSHA1AndRC2_CBC	68
#define OBJ_pbeWithSHA1AndRC2_CBC	OBJ_pkcs,5L,11L 

/* proposed by microsoft to RSA */
#define LN_pbeWithSHA1AndRC4		"pbeWithSHA1AndRC4"
#define NID_pbeWithSHA1AndRC4		69
#define OBJ_pbeWithSHA1AndRC4		OBJ_pkcs,5L,12L 

#define SN_dsaWithSHA1			"DSS-SHA1"
#define LN_dsaWithSHA1			"sha1WithDSSEncryption"
#define NID_dsaWithSHA1			70

#include "asn1.h"

#ifndef NOPROTO

ASN1_OBJECT *	OBJ_dup(ASN1_OBJECT *o);
ASN1_OBJECT *	OBJ_nid2obj(int n);
char *		OBJ_nid2ln(int n);
char *		OBJ_nid2sn(int n);
int		OBJ_obj2nid(ASN1_OBJECT *o);
int		OBJ_txt2nid(char *s);
int		OBJ_ln2nid(char *s);
int		OBJ_sn2nid(char *s);
int		OBJ_cmp(ASN1_OBJECT *a,ASN1_OBJECT *b);
char *		OBJ_bsearch(char *key,char *base,int num,int size,int (*cmp)());

void		ERR_load_OBJ_strings(void );
#else

ASN1_OBJECT *	OBJ_dup();
ASN1_OBJECT *	OBJ_nid2obj();
char *		OBJ_nid2ln();
char *		OBJ_nid2sn();
int		OBJ_obj2nid();
int		OBJ_txt2nid();
int		OBJ_ln2nid();
int		OBJ_sn2nid();
int		OBJ_cmp();
char *		OBJ_bsearch();

void		ERR_load_OBJ_strings();

#endif

/* BEGIN ERROR CODES */
/* Error codes for the OBJ functions. */

/* Function codes. */
#define OBJ_F_OBJ_DUP					 100
#define OBJ_F_OBJ_NID2LN				 101
#define OBJ_F_OBJ_NID2OBJ				 102
#define OBJ_F_OBJ_NID2SN				 103

/* Reason codes. */
#define OBJ_R_NID_IS_OUT_OF_RANGE			 100
#define OBJ_R_UNKNOWN_NID				 101

#ifdef  __cplusplus
}
#endif
#endif

