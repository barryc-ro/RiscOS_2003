/* lib/rsa/rsa_err.c */
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
#include "rsa.h"

/* BEGIN ERROR CODES */
static ERR_STRING_DATA RSA_str_functs[]=
	{
{ERR_PACK(0,RSA_F_RSA_GENERATE_KEY,0),	"RSA_generate_key"},
{ERR_PACK(0,RSA_F_RSA_NEW,0),	"RSA_new"},
{ERR_PACK(0,RSA_F_RSA_PRINT,0),	"RSA_print"},
{ERR_PACK(0,RSA_F_RSA_PRINT_FP,0),	"RSA_print_fp"},
{ERR_PACK(0,RSA_F_RSA_PRIVATE_DECRYPT,0),	"RSA_private_decrypt"},
{ERR_PACK(0,RSA_F_RSA_PRIVATE_ENCRYPT,0),	"RSA_private_encrypt"},
{ERR_PACK(0,RSA_F_RSA_PUBLIC_DECRYPT,0),	"RSA_public_decrypt"},
{ERR_PACK(0,RSA_F_RSA_PUBLIC_ENCRYPT,0),	"RSA_public_encrypt"},
{ERR_PACK(0,RSA_F_RSA_SIGN,0),	"RSA_sign"},
{ERR_PACK(0,RSA_F_RSA_VERIFY,0),	"RSA_verify"},
{0,NULL},
	};

static ERR_STRING_DATA RSA_str_reasons[]=
	{
{RSA_R_ALGORITHM_MISMATCH                ,"algorithm mismatch"},
{RSA_R_BAD_E_VALUE                       ,"bad e value"},
{RSA_R_BAD_FF_HEADER                     ,"bad ff header"},
{RSA_R_BAD_PAD_BYTE_COUNT                ,"bad pad byte count"},
{RSA_R_BAD_SIGNATURE                     ,"bad signature"},
{RSA_R_BLOCK_TYPE_IS_NOT_01              ,"block type is not 01"},
{RSA_R_BLOCK_TYPE_IS_NOT_02              ,"block type is not 02"},
{RSA_R_DATA_NOT_EQ_TO_MOD_LEN            ,"data not eq to mod len"},
{RSA_R_DATA_TO_LARGE_FOR_KEY_SIZE        ,"data to large for key size"},
{RSA_R_DIGEST_TOO_BIG_FOR_RSA_KEY        ,"digest too big for rsa key"},
{RSA_R_NULL_BEFORE_BLOCK_MISSING         ,"null before block missing"},
{RSA_R_THE_ASN1_OBJECT_IDENTIFIER_IS_NOT_KNOWN_FOR_THIS_MD,"the asn1 object identifier is not known for this md"},
{RSA_R_UNKNOWN_ALGORITHM_TYPE            ,"unknown algorithm type"},
{RSA_R_WRONG_SIGNATURE_LENGTH            ,"wrong signature length"},
{0,NULL},
	};

void ERR_load_RSA_strings()
	{
	static int init=1;

	if (init)
		{
		init=0;
		ERR_load_strings(ERR_LIB_RSA,RSA_str_functs);
		ERR_load_strings(ERR_LIB_RSA,RSA_str_reasons);
		}
	}
