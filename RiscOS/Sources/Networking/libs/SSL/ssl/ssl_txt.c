/* ssl/ssl_txt.c */
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
#include "buffer.h"
#include "ssl_locl.h"

#ifndef WIN16
int SSL_SESSION_print_fp(fp, x)
FILE *fp;
SSL_SESSION *x;
        {
        BIO *b;
        int ret;

        if ((b=BIO_new(BIO_s_file())) == NULL)
		{
		PEMerr(SSL_F_SSL_SESSION_PRINT_FP,ERR_R_BUF_LIB);
                return(0);
		}
        BIO_set_fp(b,fp,BIO_NOCLOSE);
        ret=SSL_SESSION_print(b,x);
        BIO_free(b);
        return(ret);
        }
#endif

int SSL_SESSION_print(bp,x)
BIO *bp;
SSL_SESSION *x;
	{
	int i;
	char str[128];

	if (BIO_puts(bp,"SSL-Session:\n") <= 0) goto err;
	sprintf(str,"    Cipher    : %s\n",x->cipher->name);
	if (BIO_puts(bp,str) <= 0) goto err;
	if (BIO_puts(bp,"    Session-ID: ") <= 0) goto err;
	for (i=0; i<(int)x->session_id_length; i++)
		{
		sprintf(str,"%02X",x->session_id[i]);
		if (BIO_puts(bp,str) <= 0) goto err;
		}
	if (BIO_puts(bp,"\n    Master-Key: ") <= 0) goto err;
	for (i=0; i<(int)x->master_key_length; i++)
		{
		sprintf(str,"%02X",x->master_key[i]);
		if (BIO_puts(bp,str) <= 0) goto err;
		}
	if (BIO_puts(bp,"\n    Key-Arg   : ") <= 0) goto err;
	if (x->key_arg_length == 0)
		{
		if (BIO_puts(bp,"None") <= 0) goto err;
		}
	else
		for (i=0; i<(int)x->key_arg_length; i++)
			{
			sprintf(str,"%02X",x->key_arg[i]);
			if (BIO_puts(bp,str) <= 0) goto err;
			}
	if (BIO_puts(bp,"\n") <= 0) goto err;
	return(1);
err:
	return(0);
	}

