/* apps/s_cb.c */
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
#include <stdlib.h>
#define USE_SOCKETS
#define NON_MAIN
#include "apps.h"
#undef NON_MAIN
#undef USE_SOCKETS
#include "err.h"
#include "x509.h"
#include "ssl.h"
#include "s_apps.h"

int verify_depth=0;
int verify_error=VERIFY_OK;

/* should be X509 * but we can just have them as char *. */
int MS_CALLBACK verify_callback(ok, xs, xi, depth, error)
int ok;
X509 *xs;
X509 *xi;
int depth;
int error;
	{
	char *s;

	s=(char *)X509_NAME_oneline(X509_get_subject_name(xs));
	if (s == NULL)
		{
		ERR_print_errors(bio_err);
		return(0);
		}
	fprintf(stderr,"depth=%d %s\n",depth,s);
	free(s);
	if (error == VERIFY_ERR_UNABLE_TO_GET_ISSUER)
		{
		s=(char *)X509_NAME_oneline(X509_get_issuer_name(xs));
		if (s == NULL)
			{
			fprintf(stderr,"verify error\n");
			ERR_print_errors(bio_err);
			return(0);
			}
		fprintf(stderr,"issuer= %s\n",s);
		free(s);
		}
	if (!ok)
		{
		fprintf(stderr,"verify error:num=%d:%s\n",error,
			X509_cert_verify_error_string(error));
		if (verify_depth <= depth)
			{
			ok=1;
			verify_error=VERIFY_OK;
			}
		else
			{
			ok=0;
			verify_error=error;
			}
		}
	fprintf(stderr,"verify return:%d\n",ok);
	return(ok);
	}

int set_cert_stuff(con, cert_file, key_file)
SSL *con;
char *cert_file;
char *key_file;
	{
	if (cert_file != NULL)
		{
		if (SSL_use_certificate_file(con,cert_file,
			SSL_FILETYPE_PEM) <= 0)
			{
			fprintf(stderr,"unable to set certificate file\n");
			ERR_print_errors(bio_err);
			return(0);
			}
		if (key_file == NULL) key_file=cert_file;
		if (SSL_use_PrivateKey_file(con,key_file,
			SSL_FILETYPE_PEM) <= 0)
			{
			fprintf(stderr,"unable to set public key file\n");
			ERR_print_errors(bio_err);
			return(0);
			}
		}
	return(1);
	}

