/* crypto/asn1/d2i_dh.c */
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
#include "bn.h"
#include "dh.h"
#include "objects.h"
#include "asn1_mac.h"

/*
 * ASN1err(ASN1_F_D2I_DHPARAMS,ASN1_R_LENGTH_MISMATCH);
 * ASN1err(ASN1_F_I2D_DHPARAMS,ASN1_R_UNKNOWN_ATTRIBUTE_TYPE);
 */

DH *d2i_DHparams(a,pp,length)
DH **a;
unsigned char **pp;
long length;
	{
	int i=ASN1_R_ERROR_STACK;
	ASN1_BIT_STRING *bs=NULL;
	long v=0;
	M_ASN1_D2I_vars(a,DH *,DH_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(bs,d2i_ASN1_INTEGER);
	if ((ret->p=BN_bin2bn(bs->data,bs->length,ret->p)) == NULL) goto err_bn;
	M_ASN1_D2I_get(bs,d2i_ASN1_INTEGER);
	if ((ret->g=BN_bin2bn(bs->data,bs->length,ret->g)) == NULL) goto err_bn;

	if (!M_ASN1_D2I_end_sequence())
		{
		M_ASN1_D2I_get(bs,d2i_ASN1_INTEGER);
		for (i=0; i<bs->length; i++)
			v=(v<<8)|(bs->data[i]);
		ret->length=(int)v;
		}

	ASN1_BIT_STRING_free(bs);

	M_ASN1_D2I_Finish_2(a);

err_bn:
	i=ERR_R_BN_LIB;
err:
	ASN1err(ASN1_F_D2I_DHPARAMS,i);
	if (ret != NULL) DH_free(ret);
	if (bs != NULL) ASN1_BIT_STRING_free(bs);
	return(NULL);
	}

