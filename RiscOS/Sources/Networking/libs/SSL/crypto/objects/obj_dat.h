/* lib/obj/obj_dat.h */
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

/* THIS FILE IS GENERATED FROM Objects.h by obj_dat.pl via the
 * following command:
 * perl obj_dat.pl < objects.h > obj_dat.h
 */

#define NUM_NID 71
#define NUM_SN 42
#define NUM_LN 71
#define NUM_OBJ 54

static unsigned char lvalues[379]={
0x2A,0x86,0x48,0x86,0xF7,0x0D,               /* [  0] OBJ_rsadsi */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,          /* [  6] OBJ_pkcs */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x02,0x02,     /* [ 13] OBJ_md2 */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x02,0x05,     /* [ 21] OBJ_md5 */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x03,0x04,     /* [ 29] OBJ_rc4 */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x01,/* [ 37] OBJ_rsaEncryption */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x02,/* [ 46] OBJ_md2withRSAEncryption */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x04,/* [ 55] OBJ_md5withRSAEncryption */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x05,0x01,/* [ 64] OBJ_pbeWithMD2AndDES_CBC */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x05,0x03,/* [ 73] OBJ_pbeWithMD5AndDES_CBC */
0x55,                                        /* [ 82] OBJ_X500 */
0x55,0x04,                                   /* [ 83] OBJ_X509 */
0x55,0x04,0x03,                              /* [ 85] OBJ_commonName */
0x55,0x04,0x06,                              /* [ 88] OBJ_countryName */
0x55,0x04,0x07,                              /* [ 91] OBJ_localityName */
0x55,0x04,0x08,                              /* [ 94] OBJ_stateOrProvinceName */
0x55,0x04,0x0A,                              /* [ 97] OBJ_organizationName */
0x55,0x04,0x0B,                              /* [100] OBJ_organizationalUnitName */
0x55,0x08,0x01,0x01,                         /* [103] OBJ_rsa */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,     /* [107] OBJ_pkcs7 */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x01,/* [115] OBJ_pkcs7_data */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x02,/* [124] OBJ_pkcs7_signed */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x03,/* [133] OBJ_pkcs7_enveloped */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x04,/* [142] OBJ_pkcs7_signedAndEnveloped */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x05,/* [151] OBJ_pkcs7_digest */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x06,/* [160] OBJ_pkcs7_encrypted */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x03,     /* [169] OBJ_pkcs3 */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x03,0x01,/* [177] OBJ_dhKeyAgreement */
0x2B,0x0E,0x03,0x02,0x06,                    /* [186] OBJ_des_ecb */
0x2B,0x0E,0x03,0x02,0x09,                    /* [191] OBJ_des_cfb */
0x2B,0x0E,0x03,0x02,0x07,                    /* [196] OBJ_des_cbc */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x03,0x11,     /* [201] OBJ_des_ede3 */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x03,0x02,     /* [209] OBJ_rc2_cbc */
0x2B,0x0E,0x03,0x02,0x12,                    /* [217] OBJ_sha */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x03,0x07,     /* [222] OBJ_des_ede3_cbc */
0x2B,0x0E,0x03,0x02,0x08,                    /* [230] OBJ_des_ofb */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,     /* [235] OBJ_pkcs9 */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x01,/* [243] OBJ_pkcs9_emailAddress */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x02,/* [252] OBJ_pkcs9_unstructuredName */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x03,/* [261] OBJ_pkcs9_contentType */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x04,/* [270] OBJ_pkcs9_messageDigest */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x05,/* [279] OBJ_pkcs9_signingTime */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x06,/* [288] OBJ_pkcs9_countersignature */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x07,/* [297] OBJ_pkcs9_challengePassword */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x08,/* [306] OBJ_pkcs9_unstructuredAddress */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x09,/* [315] OBJ_pkcs9_extCertAttributes */
0x60,0x86,0x48,0xD8,0x6A,                    /* [324] OBJ_netscape */
0x60,0x86,0x48,0xD8,0x6A,0x01,0x02,          /* [329] OBJ_netscape_gif2 */
0x60,0x86,0x48,0xD8,0x6A,0x01,0x03,          /* [336] OBJ_netscape_gif3 */
0x2B,0x0E,0x03,0x02,0x03,0x02,0x1A,          /* [343] OBJ_sha1 */
0x2B,0x0E,0x03,0x02,0x0C,                    /* [350] OBJ_dsaWithSHA */
0x2B,0x0E,0x03,0x02,0x0D,                    /* [355] OBJ_dss */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x05,0x0B,/* [360] OBJ_pbeWithSHA1AndRC2_CBC */
0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x05,0x0C,/* [369] OBJ_pbeWithSHA1AndRC4 */
};

static ASN1_OBJECT nid_objs[NUM_NID]={
{"UNDEF","undefined",NID_undef,0,(unsigned char*)-1},
{"rsadsi","rsadsi",NID_rsadsi,6,(unsigned char*)0},
{"pkcs","pkcs",NID_pkcs,7,(unsigned char*)6},
{"MD2","md2",NID_md2,8,(unsigned char*)13},
{"MD5","md5",NID_md5,8,(unsigned char*)21},
{"RC4","rc4",NID_rc4,8,(unsigned char*)29},
{"rsaEncryption","rsaEncryption",NID_rsaEncryption,9,(unsigned char*)37},
{"RSA-MD2","md2withRSAEncryption",NID_md2withRSAEncryption,9,
	(unsigned char*)46},
{"RSA-MD5","md5withRSAEncryption",NID_md5withRSAEncryption,9,
	(unsigned char*)55},
{"pbeWithMD2AndDES-CBC","pbeWithMD2AndDES-CBC",
	NID_pbeWithMD2AndDES_CBC,9,(unsigned char*)64},
{"pbeWithMD5AndDES-CBC","pbeWithMD5AndDES-CBC",
	NID_pbeWithMD5AndDES_CBC,9,(unsigned char*)73},
{"X500","X500",NID_X500,1,(unsigned char*)82},
{"X509","X509",NID_X509,2,(unsigned char*)83},
{"CN","commonName",NID_commonName,3,(unsigned char*)85},
{"C","countryName",NID_countryName,3,(unsigned char*)88},
{"L","localityName",NID_localityName,3,(unsigned char*)91},
{"SP","stateOrProvinceName",NID_stateOrProvinceName,3,(unsigned char*)94},
{"O","organizationName",NID_organizationName,3,(unsigned char*)97},
{"OU","organizationalUnitName",NID_organizationalUnitName,3,
	(unsigned char*)100},
{"RSA","rsa",NID_rsa,4,(unsigned char*)103},
{"pkcs7","pkcs7",NID_pkcs7,8,(unsigned char*)107},
{"pkcs7-data","pkcs7-data",NID_pkcs7_data,9,(unsigned char*)115},
{"pkcs7-signedData","pkcs7-signedData",NID_pkcs7_signed,9,
	(unsigned char*)124},
{"pkcs7-envelopedData","pkcs7-envelopedData",NID_pkcs7_enveloped,9,
	(unsigned char*)133},
{"pkcs7-signedAndEnvelopedData","pkcs7-signedAndEnvelopedData",
	NID_pkcs7_signedAndEnveloped,9,(unsigned char*)142},
{"pkcs7-digestData","pkcs7-digestData",NID_pkcs7_digest,9,
	(unsigned char*)151},
{"pkcs7-encryptedData","pkcs7-encryptedData",NID_pkcs7_encrypted,9,
	(unsigned char*)160},
{"pkcs3","pkcs3",NID_pkcs3,8,(unsigned char*)169},
{"dhKeyAgreement","dhKeyAgreement",NID_dhKeyAgreement,9,
	(unsigned char*)177},
{"DES-ECB","des-ecb",NID_des_ecb,5,(unsigned char*)186},
{"DES-CFB","des-cfb",NID_des_cfb,5,(unsigned char*)191},
{"DES-CBC","des-cbc",NID_des_cbc,5,(unsigned char*)196},
{"DES-EDE","des-ede",NID_des_ede,0,(unsigned char*)-1},
{"DES-EDE3","des-ede3",NID_des_ede3,8,(unsigned char*)201},
{"IDEA-CBC","idea-cbc",NID_idea_cbc,0,(unsigned char*)-1},
{"IDEA-CFB","idea-cfb",NID_idea_cfb,0,(unsigned char*)-1},
{"IDEA-ECB","idea-ecb",NID_idea_ecb,0,(unsigned char*)-1},
{"RC2-CBC","rc2-cbc",NID_rc2_cbc,8,(unsigned char*)209},
{"RC2-ECB","rc2-ecb",NID_rc2_ecb,0,(unsigned char*)-1},
{"RC2-CFB","rc2-cfb",NID_rc2_cfb,0,(unsigned char*)-1},
{"RC2-OFB","rc2-ofb",NID_rc2_ofb,0,(unsigned char*)-1},
{"SHA","sha",NID_sha,5,(unsigned char*)217},
{"RSA-SHA","shaWithRSAEncryption",NID_shaWithRSAEncryption,0,
	(unsigned char*)-1},
{"DES-EDE-CBC","des-ede-cbc",NID_des_ede_cbc,0,(unsigned char*)-1},
{"DES-EDE3-CBC","des-ede3-cbc",NID_des_ede3_cbc,8,(unsigned char*)222},
{"DES-OFB","des-ofb",NID_des_ofb,5,(unsigned char*)230},
{"IDEA-OFB","idea-ofb",NID_idea_ofb,0,(unsigned char*)-1},
{"pkcs9","pkcs9",NID_pkcs9,8,(unsigned char*)235},
{"Email","emailAddress",NID_pkcs9_emailAddress,9,(unsigned char*)243},
{"unstructuredName","unstructuredName",NID_pkcs9_unstructuredName,9,
	(unsigned char*)252},
{"contentType","contentType",NID_pkcs9_contentType,9,(unsigned char*)261},
{"messageDigest","messageDigest",NID_pkcs9_messageDigest,9,
	(unsigned char*)270},
{"signingTime","signingTime",NID_pkcs9_signingTime,9,(unsigned char*)279},
{"countersignature","countersignature",NID_pkcs9_countersignature,9,
	(unsigned char*)288},
{"challengePassword","challengePassword",NID_pkcs9_challengePassword,
	9,(unsigned char*)297},
{"unstructuredAddress","unstructuredAddress",
	NID_pkcs9_unstructuredAddress,9,(unsigned char*)306},
{"extendedCertificateAttributes","extendedCertificateAttributes",
	NID_pkcs9_extCertAttributes,9,(unsigned char*)315},
{"Netscape","Netscape Communications Corp.",NID_netscape,5,
	(unsigned char*)324},
{"ns-gif-2","ns-gif-2",NID_netscape_gif2,7,(unsigned char*)329},
{"ns-gif-3","ns-gif-3",NID_netscape_gif3,7,(unsigned char*)336},
{"DES-EDE-CFB","des-ede-cfb",NID_des_ede_cfb,0,(unsigned char*)-1},
{"DES-EDE3-CFB","des-ede3-cfb",NID_des_ede3_cfb,0,(unsigned char*)-1},
{"DES-EDE-OFB","des-ede-ofb",NID_des_ede_ofb,0,(unsigned char*)-1},
{"DES-EDE3-OFB","des-ede3-ofb",NID_des_ede3_ofb,0,(unsigned char*)-1},
{"SHA1","sha1",NID_sha1,7,(unsigned char*)343},
{"RSA-SHA1","sha1WithRSAEncryption",NID_sha1WithRSAEncryption,0,
	(unsigned char*)-1},
{"DSS-SHA","shaWithDSSEncryption",NID_dsaWithSHA,5,(unsigned char*)350},
{"DSS","dssEncryption",NID_dss,5,(unsigned char*)355},
{"pbeWithSHA1AndRC2-CBC","pbeWithSHA1AndRC2-CBC",
	NID_pbeWithSHA1AndRC2_CBC,9,(unsigned char*)360},
{"pbeWithSHA1AndRC4","pbeWithSHA1AndRC4",NID_pbeWithSHA1AndRC4,9,
	(unsigned char*)369},
{"DSS-SHA1","sha1WithDSSEncryption",NID_dsaWithSHA1,0,(unsigned char*)-1},
};

static ASN1_OBJECT *sn_objs[NUM_SN]={
(ASN1_OBJECT*)14,/* "C" */
(ASN1_OBJECT*)13,/* "CN" */
(ASN1_OBJECT*)31,/* "DES-CBC" */
(ASN1_OBJECT*)30,/* "DES-CFB" */
(ASN1_OBJECT*)29,/* "DES-ECB" */
(ASN1_OBJECT*)32,/* "DES-EDE" */
(ASN1_OBJECT*)43,/* "DES-EDE-CBC" */
(ASN1_OBJECT*)60,/* "DES-EDE-CFB" */
(ASN1_OBJECT*)62,/* "DES-EDE-OFB" */
(ASN1_OBJECT*)33,/* "DES-EDE3" */
(ASN1_OBJECT*)44,/* "DES-EDE3-CBC" */
(ASN1_OBJECT*)61,/* "DES-EDE3-CFB" */
(ASN1_OBJECT*)63,/* "DES-EDE3-OFB" */
(ASN1_OBJECT*)45,/* "DES-OFB" */
(ASN1_OBJECT*)67,/* "DSS" */
(ASN1_OBJECT*)66,/* "DSS-SHA" */
(ASN1_OBJECT*)70,/* "DSS-SHA1" */
(ASN1_OBJECT*)48,/* "Email" */
(ASN1_OBJECT*)34,/* "IDEA-CBC" */
(ASN1_OBJECT*)35,/* "IDEA-CFB" */
(ASN1_OBJECT*)36,/* "IDEA-ECB" */
(ASN1_OBJECT*)46,/* "IDEA-OFB" */
(ASN1_OBJECT*)15,/* "L" */
(ASN1_OBJECT*) 3,/* "MD2" */
(ASN1_OBJECT*) 4,/* "MD5" */
(ASN1_OBJECT*)57,/* "Netscape" */
(ASN1_OBJECT*)17,/* "O" */
(ASN1_OBJECT*)18,/* "OU" */
(ASN1_OBJECT*)37,/* "RC2-CBC" */
(ASN1_OBJECT*)39,/* "RC2-CFB" */
(ASN1_OBJECT*)38,/* "RC2-ECB" */
(ASN1_OBJECT*)40,/* "RC2-OFB" */
(ASN1_OBJECT*) 5,/* "RC4" */
(ASN1_OBJECT*)19,/* "RSA" */
(ASN1_OBJECT*) 7,/* "RSA-MD2" */
(ASN1_OBJECT*) 8,/* "RSA-MD5" */
(ASN1_OBJECT*)42,/* "RSA-SHA" */
(ASN1_OBJECT*)65,/* "RSA-SHA1" */
(ASN1_OBJECT*)41,/* "SHA" */
(ASN1_OBJECT*)64,/* "SHA1" */
(ASN1_OBJECT*)16,/* "SP" */
(ASN1_OBJECT*) 0,/* "UNDEF" */
};

static ASN1_OBJECT *ln_objs[NUM_LN]={
(ASN1_OBJECT*)57,/* "Netscape Communications Corp." */
(ASN1_OBJECT*)11,/* "X500" */
(ASN1_OBJECT*)12,/* "X509" */
(ASN1_OBJECT*)54,/* "challengePassword" */
(ASN1_OBJECT*)13,/* "commonName" */
(ASN1_OBJECT*)50,/* "contentType" */
(ASN1_OBJECT*)53,/* "countersignature" */
(ASN1_OBJECT*)14,/* "countryName" */
(ASN1_OBJECT*)31,/* "des-cbc" */
(ASN1_OBJECT*)30,/* "des-cfb" */
(ASN1_OBJECT*)29,/* "des-ecb" */
(ASN1_OBJECT*)32,/* "des-ede" */
(ASN1_OBJECT*)43,/* "des-ede-cbc" */
(ASN1_OBJECT*)60,/* "des-ede-cfb" */
(ASN1_OBJECT*)62,/* "des-ede-ofb" */
(ASN1_OBJECT*)33,/* "des-ede3" */
(ASN1_OBJECT*)44,/* "des-ede3-cbc" */
(ASN1_OBJECT*)61,/* "des-ede3-cfb" */
(ASN1_OBJECT*)63,/* "des-ede3-ofb" */
(ASN1_OBJECT*)45,/* "des-ofb" */
(ASN1_OBJECT*)28,/* "dhKeyAgreement" */
(ASN1_OBJECT*)67,/* "dssEncryption" */
(ASN1_OBJECT*)48,/* "emailAddress" */
(ASN1_OBJECT*)56,/* "extendedCertificateAttributes" */
(ASN1_OBJECT*)34,/* "idea-cbc" */
(ASN1_OBJECT*)35,/* "idea-cfb" */
(ASN1_OBJECT*)36,/* "idea-ecb" */
(ASN1_OBJECT*)46,/* "idea-ofb" */
(ASN1_OBJECT*)15,/* "localityName" */
(ASN1_OBJECT*) 3,/* "md2" */
(ASN1_OBJECT*) 7,/* "md2withRSAEncryption" */
(ASN1_OBJECT*) 4,/* "md5" */
(ASN1_OBJECT*) 8,/* "md5withRSAEncryption" */
(ASN1_OBJECT*)51,/* "messageDigest" */
(ASN1_OBJECT*)58,/* "ns-gif-2" */
(ASN1_OBJECT*)59,/* "ns-gif-3" */
(ASN1_OBJECT*)17,/* "organizationName" */
(ASN1_OBJECT*)18,/* "organizationalUnitName" */
(ASN1_OBJECT*) 9,/* "pbeWithMD2AndDES-CBC" */
(ASN1_OBJECT*)10,/* "pbeWithMD5AndDES-CBC" */
(ASN1_OBJECT*)68,/* "pbeWithSHA1AndRC2-CBC" */
(ASN1_OBJECT*)69,/* "pbeWithSHA1AndRC4" */
(ASN1_OBJECT*) 2,/* "pkcs" */
(ASN1_OBJECT*)27,/* "pkcs3" */
(ASN1_OBJECT*)20,/* "pkcs7" */
(ASN1_OBJECT*)21,/* "pkcs7-data" */
(ASN1_OBJECT*)25,/* "pkcs7-digestData" */
(ASN1_OBJECT*)26,/* "pkcs7-encryptedData" */
(ASN1_OBJECT*)23,/* "pkcs7-envelopedData" */
(ASN1_OBJECT*)24,/* "pkcs7-signedAndEnvelopedData" */
(ASN1_OBJECT*)22,/* "pkcs7-signedData" */
(ASN1_OBJECT*)47,/* "pkcs9" */
(ASN1_OBJECT*)37,/* "rc2-cbc" */
(ASN1_OBJECT*)39,/* "rc2-cfb" */
(ASN1_OBJECT*)38,/* "rc2-ecb" */
(ASN1_OBJECT*)40,/* "rc2-ofb" */
(ASN1_OBJECT*) 5,/* "rc4" */
(ASN1_OBJECT*)19,/* "rsa" */
(ASN1_OBJECT*) 6,/* "rsaEncryption" */
(ASN1_OBJECT*) 1,/* "rsadsi" */
(ASN1_OBJECT*)41,/* "sha" */
(ASN1_OBJECT*)64,/* "sha1" */
(ASN1_OBJECT*)70,/* "sha1WithDSSEncryption" */
(ASN1_OBJECT*)65,/* "sha1WithRSAEncryption" */
(ASN1_OBJECT*)66,/* "shaWithDSSEncryption" */
(ASN1_OBJECT*)42,/* "shaWithRSAEncryption" */
(ASN1_OBJECT*)52,/* "signingTime" */
(ASN1_OBJECT*)16,/* "stateOrProvinceName" */
(ASN1_OBJECT*) 0,/* "undefined" */
(ASN1_OBJECT*)55,/* "unstructuredAddress" */
(ASN1_OBJECT*)49,/* "unstructuredName" */
};

static ASN1_OBJECT *obj_objs[NUM_OBJ]={
(ASN1_OBJECT*)11,/* OBJ_X500                         2 5 */
(ASN1_OBJECT*)12,/* OBJ_X509                         2 5 4 */
(ASN1_OBJECT*)13,/* OBJ_commonName                   2 5 4 3 */
(ASN1_OBJECT*)14,/* OBJ_countryName                  2 5 4 6 */
(ASN1_OBJECT*)15,/* OBJ_localityName                 2 5 4 7 */
(ASN1_OBJECT*)16,/* OBJ_stateOrProvinceName          2 5 4 8 */
(ASN1_OBJECT*)17,/* OBJ_organizationName             2 5 4 10 */
(ASN1_OBJECT*)18,/* OBJ_organizationalUnitName       2 5 4 11 */
(ASN1_OBJECT*)19,/* OBJ_rsa                          2 5 8 1 1 */
(ASN1_OBJECT*)29,/* OBJ_des_ecb                      1 3 14 3 2 6 */
(ASN1_OBJECT*)31,/* OBJ_des_cbc                      1 3 14 3 2 7 */
(ASN1_OBJECT*)45,/* OBJ_des_ofb                      1 3 14 3 2 8 */
(ASN1_OBJECT*)30,/* OBJ_des_cfb                      1 3 14 3 2 9 */
(ASN1_OBJECT*)66,/* OBJ_dsaWithSHA                   1 3 14 3 2 12 */
(ASN1_OBJECT*)67,/* OBJ_dss                          1 3 14 3 2 13 */
(ASN1_OBJECT*)41,/* OBJ_sha                          1 3 14 3 2 18 */
(ASN1_OBJECT*)57,/* OBJ_netscape                     2 16 840 11370 */
(ASN1_OBJECT*) 1,/* OBJ_rsadsi                       1 2 840 113549 */
(ASN1_OBJECT*) 2,/* OBJ_pkcs                         1 2 840 113549 1 */
(ASN1_OBJECT*)64,/* OBJ_sha1                         1 3 14 3 2 3 2 26 */
(ASN1_OBJECT*)58,/* OBJ_netscape_gif2                2 16 840 11370 1 2 */
(ASN1_OBJECT*)59,/* OBJ_netscape_gif3                2 16 840 11370 1 3 */
(ASN1_OBJECT*)27,/* OBJ_pkcs3                        1 2 840 113549 1 3 */
(ASN1_OBJECT*)20,/* OBJ_pkcs7                        1 2 840 113549 1 7 */
(ASN1_OBJECT*)47,/* OBJ_pkcs9                        1 2 840 113549 1 9 */
(ASN1_OBJECT*) 3,/* OBJ_md2                          1 2 840 113549 2 2 */
(ASN1_OBJECT*) 4,/* OBJ_md5                          1 2 840 113549 2 5 */
(ASN1_OBJECT*)37,/* OBJ_rc2_cbc                      1 2 840 113549 3 2 */
(ASN1_OBJECT*) 5,/* OBJ_rc4                          1 2 840 113549 3 4 */
(ASN1_OBJECT*)44,/* OBJ_des_ede3_cbc                 1 2 840 113549 3 7 */
(ASN1_OBJECT*)33,/* OBJ_des_ede3                     1 2 840 113549 3 17 */
(ASN1_OBJECT*) 6,/* OBJ_rsaEncryption                1 2 840 113549 1 1 1 */
(ASN1_OBJECT*) 7,/* OBJ_md2withRSAEncryption         1 2 840 113549 1 1 2 */
(ASN1_OBJECT*) 8,/* OBJ_md5withRSAEncryption         1 2 840 113549 1 1 4 */
(ASN1_OBJECT*)28,/* OBJ_dhKeyAgreement               1 2 840 113549 1 3 1 */
(ASN1_OBJECT*) 9,/* OBJ_pbeWithMD2AndDES_CBC         1 2 840 113549 1 5 1 */
(ASN1_OBJECT*)10,/* OBJ_pbeWithMD5AndDES_CBC         1 2 840 113549 1 5 3 */
(ASN1_OBJECT*)68,/* OBJ_pbeWithSHA1AndRC2_CBC        1 2 840 113549 1 5 11  */
(ASN1_OBJECT*)69,/* OBJ_pbeWithSHA1AndRC4            1 2 840 113549 1 5 12  */
(ASN1_OBJECT*)21,/* OBJ_pkcs7_data                   1 2 840 113549 1 7 1 */
(ASN1_OBJECT*)22,/* OBJ_pkcs7_signed                 1 2 840 113549 1 7 2 */
(ASN1_OBJECT*)23,/* OBJ_pkcs7_enveloped              1 2 840 113549 1 7 3 */
(ASN1_OBJECT*)24,/* OBJ_pkcs7_signedAndEnveloped     1 2 840 113549 1 7 4 */
(ASN1_OBJECT*)25,/* OBJ_pkcs7_digest                 1 2 840 113549 1 7 5 */
(ASN1_OBJECT*)26,/* OBJ_pkcs7_encrypted              1 2 840 113549 1 7 6 */
(ASN1_OBJECT*)48,/* OBJ_pkcs9_emailAddress           1 2 840 113549 1 9 1 */
(ASN1_OBJECT*)49,/* OBJ_pkcs9_unstructuredName       1 2 840 113549 1 9 2 */
(ASN1_OBJECT*)50,/* OBJ_pkcs9_contentType            1 2 840 113549 1 9 3 */
(ASN1_OBJECT*)51,/* OBJ_pkcs9_messageDigest          1 2 840 113549 1 9 4 */
(ASN1_OBJECT*)52,/* OBJ_pkcs9_signingTime            1 2 840 113549 1 9 5 */
(ASN1_OBJECT*)53,/* OBJ_pkcs9_countersignature       1 2 840 113549 1 9 6 */
(ASN1_OBJECT*)54,/* OBJ_pkcs9_challengePassword      1 2 840 113549 1 9 7 */
(ASN1_OBJECT*)55,/* OBJ_pkcs9_unstructuredAddress    1 2 840 113549 1 9 8 */
(ASN1_OBJECT*)56,/* OBJ_pkcs9_extCertAttributes      1 2 840 113549 1 9 9 */
};

