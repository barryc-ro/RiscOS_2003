/* crypto/error/err.h */
/* Copyright (C) 1995-1996 Eric Young (eay@mincom.oz.au)
 * All rights reserved.
 * 
 * This file is part of an SSL implementation written
 * by Eric Young (eay@mincom.oz.au).
 * The implementation was written so as to conform with Netscapes SS
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
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIA
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

#ifndef HEADER_ERR_H
#define HEADER_ERR_H

#ifdef  __cplusplus
extern "C" {
#endif

/* The following is a bit of a trick to help the object files only contain
 * the 'name of the file' string once.  Since 'err.h' is protected by the
 * HEADER_ERR_H stuff, this should be included only once per file. */

#define ERR_file_name	__FILE__

#include <errno.h>

/* library */
#define ERR_LIB_NONE		1
#define ERR_LIB_SYS		2
#define ERR_LIB_BN		3
#define ERR_LIB_RSA		4
#define ERR_LIB_DH		5
#define ERR_LIB_EVP		6
#define ERR_LIB_BUF		7
#define ERR_LIB_OBJ		8
#define ERR_LIB_PEM		9
#define ERR_LIB_DSA		10
#define ERR_LIB_X509		11
#define ERR_LIB_METH		12
#define ERR_LIB_ASN1		13
#define ERR_LIB_CONF		14
#define ERR_LIB_SSL		20
#define ERR_LIB_RSAREF		30

#define SYSerr(f,r)	ERR_put_error(ERR_LIB_SYS,f,r,ERR_file_name,__LINE__)
#define BNerr(f,r)	ERR_put_error(ERR_LIB_BN,f,r,ERR_file_name,__LINE__)
#define RSAerr(f,r)	ERR_put_error(ERR_LIB_RSA,f,r,ERR_file_name,__LINE__)
#define DHerr(f,r)	ERR_put_error(ERR_LIB_DH,f,r,ERR_file_name,__LINE__)
#define EVPerr(f,r)	ERR_put_error(ERR_LIB_EVP,f,r,ERR_file_name,__LINE__)
#define BUFerr(f,r)	ERR_put_error(ERR_LIB_BUF,f,r,ERR_file_name,__LINE__)
#define OBJerr(f,r)	ERR_put_error(ERR_LIB_OBJ,f,r,ERR_file_name,__LINE__)
#define PEMerr(f,r)	ERR_put_error(ERR_LIB_PEM,f,r,ERR_file_name,__LINE__)
#define DSAerr(f,r)	ERR_put_error(ERR_LIB_DSA,f,r,ERR_file_name,__LINE__)
#define X509err(f,r)	ERR_put_error(ERR_LIB_X509,f,r,ERR_file_name,__LINE__)
#define METHerr(f,r)	ERR_put_error(ERR_LIB_METH,f,r,ERR_file_name,__LINE__)
#define ASN1err(f,r)	ERR_put_error(ERR_LIB_ASN1,f,r,ERR_file_name,__LINE__)
#define CONFerr(f,r)	ERR_put_error(ERR_LIB_CONF,f,r,ERR_file_name,__LINE__)
#define SSLerr(f,r)	ERR_put_error(ERR_LIB_SSL,f,r,ERR_file_name,__LINE__)
#define RSAREFerr(f,r)	ERR_put_error(ERR_LIB_RSAREF,f,r,ERR_file_name,__LINE__)

/* Borland C seems too stupid to be able to shift and do longs in
 * the pre-processor :-( */
#define ERR_PACK(l,f,r)		(((((unsigned long)l)&0xffL)*0x1000000)| \
				((((unsigned long)f)&0xfffL)*0x1000)| \
				((((unsigned long)r)&0xfffL)))
#define ERR_GET_LIB(l)		(int)((((unsigned long)l)>>24L)&0xffL)
#define ERR_GET_FUNC(l)		(int)((((unsigned long)l)>>12L)&0xfffL)
#define ERR_GET_REASON(l)	(int)((l)&0xfffL)
#define ERR_FATAL_ERROR(l)	(int)((l)&ERR_R_FATAL)

/* fuctions are local */
#define ERR_F_FOPEN		1

#define ERR_R_FATAL		32	
/* reasons */
#define ERR_R_SYS_LIB	ERR_LIB_SYS
#define ERR_R_BN_LIB	ERR_LIB_BN
#define ERR_R_RSA_LIB	ERR_LIB_RSA
#define ERR_R_DSA_LIB	ERR_LIB_DSA
#define ERR_R_DH_LIB	ERR_LIB_DH
#define ERR_R_EVP_LIB	ERR_LIB_EVP
#define ERR_R_BUF_LIB	ERR_LIB_BUF
#define ERR_R_OBJ_LIB	ERR_LIB_OBJ
#define ERR_R_PEM_LIB	ERR_LIB_PEM
#define ERR_R_X509_LIB	ERR_LIB_X509
#define ERR_R_METH_LIB	ERR_LIB_METH
#define ERR_R_ASN1_LIB	ERR_LIB_ASN1
#define ERR_R_CONF_LIB	ERR_LIB_CONF
#define ERR_R_SSL_LIB	ERR_LIB_SSL

/* fatal error */
#define	ERR_R_MALLOC_FAILURE			(1|ERR_R_FATAL)
#define	ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED	(2|ERR_R_FATAL)

typedef struct ERR_string_data_st
	{
	unsigned long error;
	char *string;
	} ERR_STRING_DATA;

#ifndef NOPROTO
void ERR_put_error(int lib, int func,int reason,char *file,int line);
unsigned long ERR_get_error(void );
unsigned long ERR_get_error_line(char **file,int *line);
unsigned long ERR_peek_error(void );
unsigned long ERR_peek_error_line(char **file,int *line);
void ERR_clear_error(void );
char *ERR_error_string(unsigned long e,char *buf);
char *ERR_lib_error_string(unsigned long e);
char *ERR_func_error_string(unsigned long e);
char *ERR_reason_error_string(unsigned long e);
#ifndef WIN16
void ERR_print_errors_fp(FILE *fp);
#endif
#ifdef HEADER_BUFFER_H
void ERR_print_errors(BIO *bp);
#else
void ERR_print_errors(char *bp);
#endif
void ERR_load_strings(int lib,ERR_STRING_DATA str[]);
void ERR_load_ERR_strings(void );
void ERR_load_crypto_strings(void );
void ERR_free_strings(void );

#ifdef HEADER_LHASH_H
LHASH *err_data(void );
#else
char *err_data(void );
#endif

#else

void ERR_put_error();
unsigned long ERR_get_error();
unsigned long ERR_get_error_line();
unsigned long ERR_peek_error();
unsigned long ERR_peek_error_line();
void ERR_clear_error();
char *ERR_error_string();
char *ERR_lib_error_string();
char *ERR_func_error_string();
char *ERR_reason_error_string();
#ifndef WIN16
void ERR_print_errors_fp();
#endif
void ERR_print_errors();
void ERR_load_strings();
void ERR_load_ERR_strings();
void ERR_load_crypto_strings();
void ERR_free_strings();

#ifdef HEADER_LHASH_H
LHASH *err_data();
#else
char *err_data();
#endif

#endif

#ifdef  __cplusplus
}
#endif

#endif
