/* lib/x509/x509_err.c */
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

#include <stdio.h>
#include "err.h"
#include "x509.h"

/* BEGIN ERROR CODES */
static ERR_STRING_DATA X509_str_functs[]=
	{
{ERR_PACK(0,X509_F_INIT_CERTS,0),	"INIT_CERTS"},
{ERR_PACK(0,X509_F_X509_ADD_CERT,0),	"X509_add_cert"},
{ERR_PACK(0,X509_F_X509_ADD_CERT_DIR,0),	"X509_add_cert_dir"},
{ERR_PACK(0,X509_F_X509_ADD_CERT_FILE,0),	"X509_add_cert_file"},
{ERR_PACK(0,X509_F_X509_EXTRACT_KEY,0),	"X509_extract_key"},
{ERR_PACK(0,X509_F_X509_GET_CERT,0),	"X509_get_cert"},
{ERR_PACK(0,X509_F_X509_NAME_ONELINE,0),	"X509_NAME_oneline"},
{ERR_PACK(0,X509_F_X509_NAME_PRINT,0),	"X509_NAME_print"},
{ERR_PACK(0,X509_F_X509_PRINT_FP,0),	"X509_print_fp"},
{ERR_PACK(0,X509_F_X509_REQ_PRINT,0),	"X509_REQ_print"},
{ERR_PACK(0,X509_F_X509_REQ_PRINT_FP,0),	"X509_REQ_print_fp"},
{ERR_PACK(0,X509_F_X509_REQ_TO_X509,0),	"X509_REQ_to_X509"},
{ERR_PACK(0,X509_F_X509_SET_DEFAULT_VERIFY_PATHS,0),	"X509_set_default_verify_paths"},
{ERR_PACK(0,X509_F_X509_TO_X509_REQ,0),	"X509_to_X509_REQ"},
{0,NULL},
	};

static ERR_STRING_DATA X509_str_reasons[]=
	{
{X509_R_BAD_PUBLIC_KEY_TYPE              ,"bad public key type"},
{X509_R_BAD_X509_FILETYPE                ,"bad x509 filetype"},
{X509_R_CERT_ALREADY_IN_HASH_TABLE       ,"cert already in hash table"},
{X509_R_ERR_ASN1_LIB                     ,"err asn1 lib"},
{X509_R_LOADING_DEFAULTS                 ,"loading defaults"},
{X509_R_LOADING_DIR_ENV_VAR              ,"loading dir env var"},
{X509_R_LOADING_FILE_ENV_VAR             ,"loading file env var"},
{0,NULL},
	};

void ERR_load_X509_strings()
	{
	static int init=1;

	if (init)
		{
		init=0;
		ERR_load_strings(ERR_LIB_X509,X509_str_functs);
		ERR_load_strings(ERR_LIB_X509,X509_str_reasons);
		}
	}
