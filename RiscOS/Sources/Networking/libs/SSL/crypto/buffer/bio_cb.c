/* crypto/buffer/bio_cb.c */
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
#include <string.h>
#include <stdlib.h>
#include "cryptlib.h"
#include "buffer.h"
#include "err.h"

long MS_CALLBACK BIO_debug_callback(bio,cmd,argp,argi,argl,ret)
BIO *bio;
int cmd;
char *argp;
int argi;
long argl;
long ret;
	{
	BIO *b;
	MS_STATIC char buf[256];
	char *p;
	long r=1;

	if (BIO_CB_RETURN & cmd)
		r=ret;

	sprintf(buf,"BIO[%08lX]:",(unsigned long)bio);
	p= &(buf[14]);
	switch (cmd)
		{
	case BIO_CB_FREE:
		sprintf(p,"free - %s\n",bio->method->name);
		break;
	case BIO_CB_READ:
		sprintf(p,"read(%d) - %s\n",argi,bio->method->name);
		break;
	case BIO_CB_WRITE:
		sprintf(p,"write(%d) - %s\n",argi,bio->method->name);
		break;
	case BIO_CB_PUTS:
		sprintf(p,"puts() - %s\n",bio->method->name);
		break;
	case BIO_CB_GETS:
		sprintf(p,"gets(%d) - %s\n",argi,bio->method->name);
		break;
	case BIO_CB_CTRL:
		sprintf(p,"ctrl(%d) - %s\n",argi,bio->method->name);
		break;
	case BIO_CB_RETURN|BIO_CB_READ:
		sprintf(p,"put  return %ld\n",ret);
		break;
	case BIO_CB_RETURN|BIO_CB_WRITE:
		sprintf(p,"get  return %ld\n",ret);
		break;
	case BIO_CB_RETURN|BIO_CB_GETS:
		sprintf(p,"gets return %ld\n",ret);
		break;
	case BIO_CB_RETURN|BIO_CB_PUTS:
		sprintf(p,"puts return %ld\n",ret);
		break;
	case BIO_CB_RETURN|BIO_CB_CTRL:
		sprintf(p,"ctrl return %ld\n",ret);
		break;
	default:
		sprintf(p,"bio callback - unknown type (%d)\n",cmd);
		break;
		}

	b=(BIO *)bio->cb_arg;
	if (b != NULL)
		BIO_write(b,buf,strlen(buf));
#ifndef WIN16
	else
		fputs(buf,stderr);
#endif
	return(r);
	}
