/* crypto/evp/e_names.c */
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

#include <stdio.h>
#include "cryptlib.h"
#include "envelope.h"
#include "objects.h"

EVP_CIPHER *EVP_get_cipherbyname(name)
char *name;
	{
	if (	(strcmp(name,SN_des_ecb) == 0) ||
		(strcmp(name,LN_des_ecb) == 0))
		return(     EVP_des_ecb());
	if (	(strcmp(name,SN_des_cfb) == 0) ||
		(strcmp(name,LN_des_cfb) == 0))
		return(     EVP_des_cfb());
	if (	(strcmp(name,SN_des_ede_cfb) == 0) ||
		(strcmp(name,LN_des_ede_cfb) == 0))
		return(	    EVP_des_ede_cfb());
	if (	(strcmp(name,SN_des_ede3_cfb) == 0) ||
		(strcmp(name,LN_des_ede3_cfb) == 0))
		return(     EVP_des_ede3_cfb());
	if (	(strcmp(name,SN_des_ofb) == 0) ||
		(strcmp(name,LN_des_ofb) == 0))
		return(     EVP_des_ofb());
	if (	(strcmp(name,SN_des_ede_ofb) == 0) ||
		(strcmp(name,LN_des_ede_ofb) == 0))
		return(     EVP_des_ede_ofb());
	if (	(strcmp(name,SN_des_ede3_ofb) == 0) ||
		(strcmp(name,LN_des_ede3_ofb) == 0))
		return(     EVP_des_ede3_ofb());
	if (	(strcmp(name,SN_des_cbc) == 0) ||
		(strcmp(name,LN_des_cbc) == 0) ||
		(strcmp(name,"DES") == 0) ||
		(strcmp(name,"des") == 0))
		return(     EVP_des_cbc());
	if (	(strcmp(name,SN_des_ede_cbc) == 0) ||
		(strcmp(name,LN_des_ede_cbc) == 0))
		return(     EVP_des_ede_cbc());
	if (	(strcmp(name,SN_des_ede3_cbc) == 0) ||
		(strcmp(name,LN_des_ede3_cbc) == 0) ||
		(strcmp(name,"DES3") == 0) ||
		(strcmp(name,"des3") == 0))
		return(     EVP_des_ede3_cbc());
	if (	(strcmp(name,SN_des_ede) == 0) ||
		(strcmp(name,LN_des_ede) == 0))
		return(     EVP_des_ede());
	if (	(strcmp(name,SN_des_ede3) == 0) ||
		(strcmp(name,LN_des_ede3) == 0))
		return(     EVP_des_ede3());
#ifndef NO_RC4
	if (	(strcmp(name,SN_rc4) == 0) ||
		(strcmp(name,LN_rc4) == 0))
		return(     EVP_rc4());
#endif
#ifndef NO_IDEA
	if (	(strcmp(name,SN_idea_ecb) == 0) ||
		(strcmp(name,LN_idea_ecb) == 0))
		return(     EVP_idea_ecb());
	if (	(strcmp(name,SN_idea_cfb) == 0) ||
		(strcmp(name,LN_idea_cfb) == 0))
		return(     EVP_idea_cfb());
	if (	(strcmp(name,SN_idea_ofb) == 0) ||
		(strcmp(name,LN_idea_ofb) == 0))
		return(     EVP_idea_ofb());
	if (	(strcmp(name,SN_idea_cbc) == 0) ||
		(strcmp(name,LN_idea_cbc) == 0) ||
		(strcmp(name,"IDEA") == 0) ||
		(strcmp(name,"idea") == 0))
		return(     EVP_idea_cbc());
#endif
#ifndef NO_RC2
	if (	(strcmp(name,SN_rc2_ecb) == 0) ||
		(strcmp(name,LN_rc2_ecb) == 0))
		return(     EVP_rc2_ecb());
	if (	(strcmp(name,SN_rc2_cfb) == 0) ||
		(strcmp(name,LN_rc2_cfb) == 0))
		return(     EVP_rc2_cfb());
	if (	(strcmp(name,SN_rc2_ofb) == 0) ||
		(strcmp(name,LN_rc2_ofb) == 0))
		return(     EVP_rc2_ofb());
	if (	(strcmp(name,SN_rc2_cbc) == 0) ||
		(strcmp(name,LN_rc2_cbc) == 0) ||
		(strcmp(name,"RC2") == 0) ||
		(strcmp(name,"rc2") == 0))
		return(     EVP_rc2_cbc());
#endif
	return(NULL);
	}

