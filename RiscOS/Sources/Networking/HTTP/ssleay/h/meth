/* crypto/meth/meth.h */
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

#ifndef HEADER_X509METH_H
#define HEADER_X509METH_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "stack.h"

#define METH_FREE       0
#define METH_INIT       1
#define METH_SHUTDOWN   2
#define METH_CONTROL    3
#define METH_GET        4
#define METH_NUMBER     5

#define METH_MAX_NUMER  10


#define METH_X509_BY_ALIAS		2
#define METH_X509_CA_BY_SUBJECT		3 /* X509_subject_name_cmp */
#define METH_X509_BY_SUBJECT		4 /* X509_subject_name_cmp */
#define METH_X509_BY_ISSUER_AND_SERIAL	5 /* X509_issuer_and_serial_cmp */
#define METH_X509_BY_ISSUER		6 /* X509_issuer_name_cmp */
#define METH_X509_NUMBER		7

#define METH_POSTFIX_BY_SUBJECT                 ".sub"
#define METH_POSTFIX_BY_SUBJECT_CRL             ".crl"
#define METH_POSTFIX_BY_ISSUER                  ".iss"
#define METH_POSTFIX_BY_ISSUER_RSA              ".key"
#define METH_POSTFIX_BY_ISSUER_AND_SERIAL       ".ias"

#define METH_TYPE_FILE		1
#define METH_TYPE_DIR		2
#define METH_TYPE_INET		3
#define METH_TYPE_X509		4
#define METH_TYPE_RSA		5
#define METH_TYPE_X509_INF0	6
#define METH_TYPE_CALLBACK	7

#define METH_CONTROL_DUMP	1
#define METH_CONTROL_FLUSH	2

#define METH_TYPE(t)		((t)&0x0f)
#define METH_SUB_TYPE(t)	((t)>>4)
#define METH_JOIN_TYPES(t,s)	(((t)&0x0f)|((s)<<4))

typedef struct method_st
	{
	char *name;
	int num_func;
	int (*func[METH_MAX_NUMER])();
	} METHOD;

#define METH_FLAG_ACTIVE        0x0001
	
typedef struct method_ctx_st
	{
	int error;
	int flags;
	METHOD *method;
	STACK *arg_type;	/* Type info about the arguments */
	STACK *args;		/* The arguments */
	char *state;		/* extra stuff */
	} METHOD_CTX;

#define METH_free(c)	meth_call((c),METH_FREE,NULL,NULL)
#define METH_init(c)	meth_call((c),METH_JOIN_TYPES(METH_INIT,0), \
				NULL,&((c)->state))
#define METH_shutdown(c) meth_call((c),METH_JOIN_TYPES(METH_SHUTDOWN,0), \
				NULL,NULL)
#define METH_control(c,t,a) meth_call((c),METH_JOIN_TYPES(METH_CONTROL,(t)), \
				(char *)(a),NULL)
#define METH_get(c,t,w,r) meth_call((c),METH_JOIN_TYPES(METH_GET,(t)), \
				(char *)(w),(char **)(r))


#define X509_by_alias(c,w,r) meth_call((c), \
		METH_JOIN_TYPES(METH_X509_BY_ALIAS,0), \
		(char *)(w),(char **)(r))
#define X509_by_ca_subject(c,w,r) meth_call((c), \
		METH_JOIN_TYPES(METH_X509_BY_CA_SUBJECT,0) \
		(char *)(w),(char **)(r))
#define X509_by_subject(c,w,r) meth_call((c), \
		METH_JOIN_TYPES(METH_X509_BY_SUBJECT,0) \
		(char *)(w),(char **)(r))
#define X509_by_issuer_and_serial(c,w,r) meth_call((c), \
		METH_JOIN_TYPES(METH_X509_BY_ISSUER_AND_SERIAL,0),\
		(char *)(w),(char **)(r))
#define X509_by_issuer(c,w,r) meth_call((c),\
		METH_JOIN_TYPES(METH_X509_BY_ISSUER,0),(char *)(w), \
		(char **)(r))

#define METH_arg(c,t,a)		meth_arg((c),(t),(char *)(a))
#define METH_push(c,t,a)	meth_arg((c),(t),(char *)(a))
#define METH_new(m)		meth_new(m)

#ifndef NOPROTO

METHOD_CTX *	meth_new(METHOD *meth);
int		meth_call(METHOD_CTX *ctx,int type,char *arg,char **ret);
int 		meth_arg(METHOD_CTX *ctx,int type,char *arg);

void ERR_load_METH_strings(void );

METHOD *x509_lookup(void );
METHOD *x509_by_file(void );
METHOD *x509_by_dir(void );

#else

METHOD_CTX *	meth_new();
int		meth_call();
int 		meth_arg();

void ERR_load_METH_strings();

METHOD *x509_lookup();
METHOD *x509_by_file();
METHOD *x509_by_dir();

#endif

/* BEGIN ERROR CODES */
/* Error codes for the METH functions. */

/* Function codes. */
#define METH_F_METH_CALL				 100
#define METH_F_METH_NEW					 101
#define METH_F_X509_METHOD_INIT				 102

/* Reason codes. */
#define METH_R_FUNCTION_NUMBER_TO_LARGE			 100
#define METH_R_INVALID_FUNCTION				 101
#define METH_R_NO_METH_STRUCTURE_PASSED			 102

#ifdef  __cplusplus
}
#endif
#endif

