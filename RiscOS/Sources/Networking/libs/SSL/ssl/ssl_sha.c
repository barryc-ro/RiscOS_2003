/* ssl/ssl_sha.c */
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
#include "sha.h"
#include "ssl_locl.h"
#include "ssl_sha.h"

void ssl_compute_sha_mac(s, md)
SSL *s;
unsigned char *md;
	{
	SHA_CTX c;
	unsigned char sequence[4],*p,*sec,*act;
	unsigned long seq,len;

	if (s->send)
		{
		seq=s->write_sequence;
		sec=s->write_key;
		len=s->wact_data_length;
		act=s->wact_data;
		}
	else
		{
		seq=s->read_sequence;
		sec=s->read_key;
		len=s->ract_data_length;
		act=s->ract_data;
		}
	p= &(sequence[0]);
	l2n(seq,p);
#ifdef MAC_DEBUG
	SSL_TRACE(SSL_ERR,"MAC generation send=%d\n",s->send);
	ssl_print_bytes(SSL_ERR,s->session->cipher->key_size,sec);
	SSL_TRACE(SSL_ERR,"\n");
	ssl_print_bytes(SSL_ERR,len,act);
	SSL_TRACE(SSL_ERR,"\n");
	ssl_print_bytes(SSL_ERR,4,sequence);
	SSL_TRACE(SSL_ERR,"\n");
#endif
	SHA_Init(&c);
	SHA_Update(&c,sec,s->session->cipher->key_size);
	SHA_Update(&c,act,len); 
	/* the above line also does the pad data */
	SHA_Update(&c,sequence,4); 
	SHA_Final(md,&c);
	}
