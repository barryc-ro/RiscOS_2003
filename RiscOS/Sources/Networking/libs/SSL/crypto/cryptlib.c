/* crypto/cryptlib.c */
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
#include "md2.h"
#include "rc4.h"
#include "des.h"
#include "idea.h"
#include "crypto.h"
#include "date.h"

#ifdef NO_IDEA
#define idea_options() ""
#endif

char *SSLeay_version(t)
int t;
	{
	if (t == SSLEAY_VERSION)
		return("SSLeay 0.6.0 21-Jun-1996");
	if (t == SSLEAY_OPTIONS)
		{
		static char buf[100];

		sprintf(buf,"options:%s %s %s %s %s",BN_options(),MD2_options(),
			RC4_options(),des_options(),idea_options());
		return(buf);
		}
	if (t == SSLEAY_BUILT_ON)
		{
#ifdef DATE
		static char buf[sizeof(DATE)+10];

		sprintf(buf,"built on %s",DATE);
        	return(buf);
#else
		return("build date not available");
#endif
		}
	if (t == SSLEAY_CFLAGS)
		{
#ifdef CFLAGS
		static char buf[sizeof(CFLAGS)+10];

		sprintf(buf,"C flags:%s",CFLAGS);
		return(buf);
#else
		return("C flags not available");
#endif
		}
	return("not available");
	}

unsigned long SSLeay()
	{
	return(SSLEAY_VERSION_NUMBER);
	}

#ifndef NOPROTO
static void (MS_FAR *locking_callback)(int mode,int type)=NULL;
#else
static void (MS_FAR *locking_callback)()=NULL;
#endif

void (*CRYPTO_get_locking_callback())(P_I_I)
	{
	return(locking_callback);
	}

void CRYPTO_set_locking_callback(func)
void (*func)(P_I_I);
	{
	locking_callback=func;
	}

void CRYPTO_lock(mode,type)
int mode;
int type;
	{
	if (locking_callback != NULL)
		locking_callback(mode,type);
	}
