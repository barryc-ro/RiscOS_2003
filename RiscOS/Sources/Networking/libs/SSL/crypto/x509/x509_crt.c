/* crypto/x509/x509_crt.c */
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
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cryptlib.h"
#include "bn.h"
#include "lhash.h"
#include "buffer.h"
#include "x509.h"
#include "pem.h"

#ifndef NOPROTO
static int init_certs(CERTIFICATE_CTX *ctx);
#else
static int init_certs();
#endif

extern FILE *log_file;
#ifdef SSL_DEBUG
#define pprintf fprintf
#else
#define pprintf 1?0:fprintf
#endif

static int init_certs(ctx)
CERTIFICATE_CTX *ctx;
	{
	/* hold everyone who wants to init, there can be only one...
	 * bad things will happen if the init fails */

	CRYPTO_w_lock(CRYPTO_LOCK_CERTIFICATE_CTX);

	if (ctx->init <= 0)
		{
		CRYPTO_w_unlock(CRYPTO_LOCK_CERTIFICATE_CTX);
		return(1);
		}
	ctx->init=0;

	ctx->certs=lh_new(X509_subject_name_hash,X509_subject_name_cmp);

	CRYPTO_w_unlock(CRYPTO_LOCK_CERTIFICATE_CTX);

	if (ctx->certs == NULL)
		{
		X509err(X509_F_INIT_CERTS,ERR_R_MALLOC_FAILURE);
		return(0);
		}
	return(1);
	}

X509 *X509_get_cert(ctx,name,tmp_x)
CERTIFICATE_CTX *ctx;
X509_NAME *name;
X509 *tmp_x;
	{
	X509 *x,*xs,*ret=NULL;
	int i,j,k;
	unsigned long h;
	BUF_MEM *b=NULL;
	X509_NAME *tmp_n;
#ifndef RISCOS
	struct stat st;
#endif

	if ((ctx->init > 0) && (!init_certs(ctx)))
		return(NULL);
	if (name == NULL) return(NULL);
	if (tmp_x == NULL)
		{
		xs=(X509 *)X509_new();
		if (xs == NULL)
			{
			X509err(X509_F_X509_GET_CERT,ERR_R_MALLOC_FAILURE);
			return(NULL);
			}
		}
	else
		xs=tmp_x;
	tmp_n=xs->cert_info->subject;
	xs->cert_info->subject=name;

	CRYPTO_r_lock(CRYPTO_LOCK_CERTIFICATE_CTX);

	x=(X509 *)lh_retrieve(ctx->certs,(char *)xs);

	CRYPTO_r_unlock(CRYPTO_LOCK_CERTIFICATE_CTX);

	if (x != NULL)
		{
		ret=x;
		goto finish;
		}

	b=BUF_MEM_new();
	if ((b == NULL) || !BUF_MEM_grow(b,256))
		{
		X509err(X509_F_X509_GET_CERT,ERR_R_MALLOC_FAILURE);
		goto finish;
		}
	h=X509_subject_name_hash(xs);
	for (i=0; i<ctx->num_dirs; i++)
		{
		j=strlen(ctx->dirs[i])+1+8+6+1;
		if (!BUF_MEM_grow(b,j))
			{
			X509err(X509_F_X509_GET_CERT,ERR_R_MALLOC_FAILURE);
			goto finish;
			}
		k=0;
		for (;;)
			{
#ifdef RISCOS
			sprintf(b->data,"%s.%08lx/%c",ctx->dirs[i],h,k+65);
			pprintf( log_file, "searching for certificate at %s ...",
			         b->data );
#else
			sprintf(b->data,"%s/%08lx.%d",ctx->dirs[i],h,k);
#endif
			k++;
#ifdef RISCOS
			if (!riscos_FileExists(b->data))
			{
			  pprintf( log_file, " not found\n" );
			  break;
			}
#else
			if (!stat(b->data,&st) < 0)
				break;
#endif
                        pprintf( log_file, " found\n" );
			/* found one. */
			if ((X509_add_cert_file(ctx,b->data,
				ctx->dirs_type[i])) == 0)
				break;
			}

#ifdef RISCOS
                {
                    /* check the file <dir>.!CertMap for mappings between
                       hash values and human-readable filenames */
                    FILE *f;

                    sprintf( b->data, "%s.!CertMap", ctx->dirs[i] );
                    f = fopen( b->data, "r" );
                    if ( f )
                    {
                        char line[80];
                        int found = 0;

                        pprintf( log_file, "searching file %s for mapping ... ",
                                 b->data );

                        while ( !feof( f ) )
                        {
                            if ( fgets( line, 80, f )
                                 && *line != '#' && *line != 0 )
                            {
                                int x;
                                char leafname[40];
                                if ( sscanf( line, "%x %s", &x, leafname ) ==2 )
                                {
                                    if ( x==h )
                                    {
                                        sprintf( b->data, "%s.%s",
                                                 ctx->dirs[i], leafname );
                                        X509_add_cert_file( ctx, b->data,
                                                            ctx->dirs_type[i] );
                                        found = 1;
                                        pprintf( log_file, "found : %s\n",
                                                 leafname );
                                    }
                                }
                            }
                        }
                        if ( !found )
                            pprintf( log_file, "not found\n" );
                        fclose( f );
                    }
                    else
                        pprintf( log_file, "mapping file %s not found\n",
                                 b->data );
                }
#endif

		CRYPTO_r_lock(CRYPTO_LOCK_CERTIFICATE_CTX);

		x=(X509 *)lh_retrieve(ctx->certs,(char *)xs);

		CRYPTO_r_unlock(CRYPTO_LOCK_CERTIFICATE_CTX);

		if (x != NULL)
			{
			ret=x;
			goto finish;
			}
		}
finish:
	xs->cert_info->subject=tmp_n;
	if (b != NULL) BUF_MEM_free(b);
	if (tmp_x == NULL) X509_free(xs);
	return(ret);
	}

int X509_add_cert(ctx,x)
CERTIFICATE_CTX *ctx;
X509 *x;
	{
	X509 *r;
	int ret=1;

	if ((ctx->init) && (!init_certs(ctx))) return(0);
	if (x == NULL) return(0);

	CRYPTO_w_lock(CRYPTO_LOCK_CERTIFICATE_CTX);

	r=(X509 *)lh_insert(ctx->certs,(char *)x);
	if (r != NULL)
		{ /* oops, put it back */
		lh_delete(ctx->certs,(char *)x);
		lh_insert(ctx->certs,(char *)r);
		X509err(X509_F_X509_ADD_CERT,X509_R_CERT_ALREADY_IN_HASH_TABLE);
		ret=0;
		}

	CRYPTO_w_lock(CRYPTO_LOCK_CERTIFICATE_CTX);

	return(ret);
	}
