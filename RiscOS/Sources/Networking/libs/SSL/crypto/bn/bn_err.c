/* lib/bn/bn_err.c */
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
#include "bn.h"

/* BEGIN ERROR CODES */
static ERR_STRING_DATA BN_str_functs[]=
	{
{ERR_PACK(0,BN_F_BN_BN2ASCII,0),	"BN_bn2ascii"},
{ERR_PACK(0,BN_F_BN_CTX_NEW,0),	"BN_CTX_new"},
{ERR_PACK(0,BN_F_BN_DIV,0),	"BN_div"},
{ERR_PACK(0,BN_F_BN_EXPAND2,0),	"bn_expand2"},
{ERR_PACK(0,BN_F_BN_MOD_INVERSE,0),	"BN_mod_inverse"},
{ERR_PACK(0,BN_F_BN_MOD_MUL_RECIPROCAL,0),	"BN_mod_mul_reciprocal"},
{ERR_PACK(0,BN_F_BN_NEW,0),	"BN_new"},
{ERR_PACK(0,BN_F_BN_RAND,0),	"BN_rand"},
{0,NULL},
	};

static ERR_STRING_DATA BN_str_reasons[]=
	{
{BN_R_BAD_RECIPROCAL                     ,"bad reciprocal"},
{BN_R_DIV_BY_ZERO                        ,"div by zero"},
{BN_R_NO_INVERSE                         ,"no inverse"},
{0,NULL},
	};

void ERR_load_BN_strings()
	{
	static int init=1;

	if (init)
		{
		init=0;
		ERR_load_strings(ERR_LIB_BN,BN_str_functs);
		ERR_load_strings(ERR_LIB_BN,BN_str_reasons);
		}
	}
