/* ssl/ssl_lib.c */
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
#include "md5.h"
#include "ssl_locl.h"
#include "ssl_des.h"
#include "ssl_md5.h"
#include "ssl_sha.h"
#include "ssl_rc4.h"
#include "ssl_rc2.h"
#include "ssl_idea.h"
#include "ssl_rsa.h"
#include "ssl_null.h"

char *SSL_version="SSLeay 0.6.0 21-Jun-1996";

/* THIS ARRAY MUST BE KEPT ORDERED BY c1, c2 and c3.
 * basically the second last 'value' which is a #define for these 3
 * numbers */
CIPHER ssl_ciphers[NUM_CIPHERS]={
/* NULL_WITH_MD5 v3 */
	{	1,SSL_TXT_NULL_WITH_MD5,
	ssl_enc_null_init,(void (*)())free,
	ssl_enc_null, ssl_compute_md5_mac,
	8,MD5_MAC_SIZE,1,0,SSL_CK_NULL_WITH_MD5,0},

#ifndef NO_RC4
#ifndef EXPORT_ONLY
/* RC4_128_WITH_MD5 */
	{	1,SSL_TXT_RC4_128_WITH_MD5,
	ssl_enc_rc4_init,(void (*)())free,
	ssl_enc_rc4,ssl_compute_md5_mac,
	16,MD5_MAC_SIZE,1,0,SSL_CK_RC4_128_WITH_MD5,0},
#endif

/* RC4_128_EXPORT40_WITH_MD5 */
	{	1,SSL_TXT_RC4_128_EXPORT40_WITH_MD5,
	ssl_enc_rc4_init,(void (*)())free,
	ssl_enc_rc4,ssl_compute_md5_mac,
	16,MD5_MAC_SIZE,1,0, SSL_CK_RC4_128_EXPORT40_WITH_MD5,40},
#endif

#ifndef NO_RC2
#ifndef EXPORT_ONLY
/* RC2_128_CBC_WITH_MD5 */
	{	1,SSL_TXT_RC2_128_CBC_WITH_MD5,
	ssl_enc_rc2_cbc_init,(void (*)())free,
	ssl_enc_rc2_cbc,ssl_compute_md5_mac,
	16,MD5_MAC_SIZE,8,8,SSL_CK_RC2_128_CBC_WITH_MD5,0},
#endif

/* RC2_128_CBC_EXPORT40_WITH_MD5 */
	{	1,SSL_TXT_RC2_128_CBC_EXPORT40_WITH_MD5,
	ssl_enc_rc2_cbc_init,(void (*)())
	free,ssl_enc_rc2_cbc,ssl_compute_md5_mac,
	16,MD5_MAC_SIZE,8,8,SSL_CK_RC2_128_CBC_EXPORT40_WITH_MD5,40},
#endif

#ifndef EXPORT_ONLY
#ifndef NO_IDEA
/* IDEA_128_CBC_WITH_MD5 */
	{	1,SSL_TXT_IDEA_128_CBC_WITH_MD5,
	ssl_enc_idea_cbc_init,(void (*)())free,
	ssl_enc_idea_cbc,ssl_compute_md5_mac,
	16,MD5_MAC_SIZE,8,8, SSL_CK_IDEA_128_CBC_WITH_MD5,0},
#endif

/* DES_64_CBC_WITH_MD5 */
	{	1,SSL_TXT_DES_64_CBC_WITH_MD5,
	ssl_enc_des_cbc_init,(void (*)())free,
	ssl_enc_des_cbc,ssl_compute_md5_mac,
	8,MD5_MAC_SIZE,8,8,
	SSL_CK_DES_64_CBC_WITH_MD5,0},

#ifndef NO_SHA
/* DES_64_CBC_WITH_SHA */
	{	1,SSL_TXT_DES_64_CBC_WITH_SHA,
	ssl_enc_des_cbc_init,(void (*)())free,
	ssl_enc_des_cbc,ssl_compute_sha_mac,
	8,SHA_MAC_SIZE,8,8,
	SSL_CK_DES_64_CBC_WITH_SHA,0},
#endif

/* DES_192_EDE3_CBC_WITH_MD5 */
	{	1,SSL_TXT_DES_192_EDE3_CBC_WITH_MD5,
	ssl_enc_des_ede3_cbc_init,(void (*)())free,
	ssl_enc_des_ede3_cbc,ssl_compute_md5_mac,
	24,MD5_MAC_SIZE,8,8,
	SSL_CK_DES_192_EDE3_CBC_WITH_MD5,0},

#ifndef NO_SHA
/* DES_192_EDE3_CBC_WITH_SHA */
	{	1,SSL_TXT_DES_192_EDE3_CBC_WITH_SHA,
	ssl_enc_des_ede3_cbc_init,(void (*)())free,
	ssl_enc_des_ede3_cbc,ssl_compute_sha_mac,
	24,SHA_MAC_SIZE,8,8,
	SSL_CK_DES_192_EDE3_CBC_WITH_SHA,0},
#endif

/* DES_64_CFB64_WITH_MD5_1 SSLeay */
	{	1,SSL_TXT_DES_64_CFB64_WITH_MD5_1,
	ssl_enc_des_cfb_init,(void (*)())free,
	ssl_enc_des_cfb,ssl_compute_md5_mac_1,
	8,1,1,8,
	SSL_CK_DES_64_CFB64_WITH_MD5_1,0},
#endif

/* NULL SSLeay (testing) */
	{	0,SSL_TXT_NULL,
	ssl_enc_null_init,(void (*)())free,
	ssl_enc_null,ssl_compute_null_mac,
	0,0,1,0,SSL_CK_NULL,0},

/* end of list :-) */
	};

static char *pref_cipher[]={
#ifndef EXPORT_ONLY
#ifndef NO_RC4
	SSL_TXT_RC4_128_WITH_MD5,		/* Stream cipher but fast */
#endif
#ifndef NO_IDEA
	SSL_TXT_IDEA_128_CBC_WITH_MD5,		/* best? */
#endif
#ifndef NO_RC2
	SSL_TXT_RC2_128_CBC_WITH_MD5,		/* how good is RC2? */
#endif
	SSL_TXT_DES_64_CBC_WITH_MD5,		/* Small key */
#ifndef NO_SHA
	SSL_TXT_DES_64_CBC_WITH_SHA,		/* Small key, SHA */
#endif
	SSL_TXT_DES_192_EDE3_CBC_WITH_MD5,	/* paranoid but slow */
#ifndef NO_SHA
	SSL_TXT_DES_192_EDE3_CBC_WITH_SHA,	/* paranoid but slow, SHA */
#endif
#endif

#ifndef NO_RC4
	SSL_TXT_RC4_128_EXPORT40_WITH_MD5,	/* Smaller key easy to break */
#endif
#ifndef NO_RC2
	SSL_TXT_RC2_128_CBC_EXPORT40_WITH_MD5,/* Slow and Small key :-( */
#endif
	NULL
	};

#define NUM_OF(x) (sizeof(x) / sizeof(char *))
static int num_pref_cipher=NUM_OF(pref_cipher);

#if !defined(WIN16) && !defined(_DLL) && !defined(RISCOS_ZM)
FILE *SSL_LOG=stderr;
FILE *SSL_ERR=stderr;
#else
FILE *SSL_LOG=NULL;
FILE *SSL_ERR=NULL;
#endif

#ifdef RISCOS_ZM
void riscos_SSLLib_Init(void)
{
    SSL_LOG = stderr;
    SSL_ERR = stderr;
}
#endif

#ifdef PKT_DEBUG
void SSL_debug(file)
char *file;
	{
	SSL_LOG=fopen(file,"w");
	if (SSL_LOG == NULL)
		{
		perror(file);
		abort();
		}
	/* this is naughty ... each *character* goes out as a individual
	 * system call --tjh
	setbuf(SSL_LOG,NULL);
	 */
	}
#else
void SSL_debug(file)
char *file;
	{
	SSL_LOG=fopen(file,"w");
	}
#endif

int SSL_errno( void )
{
    return errno;
}

void SSL_clear(s)
SSL *s;
	{
	s->version=SSL_SERVER_VERSION;
	s->rwstate=SSL_NOTHING;
	s->state=SSL_ST_BEFORE;
	s->rstate=SSL_ST_READ_HEADER;
	if (s->init_buf != NULL) free(s->init_buf);
	s->init_buf=NULL;

	if (s->state_ccl != NULL) free(s->state_ccl);
	s->state_ccl=NULL;
	s->read_ahead=0;
	s->wnum=0;
	s->wpend_tot=0;
	s->wpend_off=0;
	s->wpend_len=0;
	s->rpend_off=0;
	s->rpend_len=0;

	s->rbuf_left=0;
	s->rbuf_offs=0;

	s->packet=s->rbuf;
	s->packet_length=0;

	s->escape=0;
	s->length=0;
	s->padding=0;
	s->ract_data_length=0;
	s->wact_data_length=0;
	s->ract_data=NULL;
	s->wact_data=NULL;
	s->mac_data=NULL;
	s->pad_data=NULL;

	if (s->crypt_state != NULL) free(s->crypt_state);
	s->crypt_state=NULL;
	s->read_key=NULL;
	s->write_key=NULL;

	s->challenge_length=0;
	if (s->challenge != NULL) free(s->challenge);
	s->challenge=NULL;

	s->conn_id_length=0;
	if (s->conn_id != NULL) free(s->conn_id);
	s->conn_id=NULL;

	s->key_material_length=0;
	if (s->key_material != NULL) free(s->key_material);
	s->key_material=NULL;

	s->send=0;
	s->clear_text=1;
	s->hit=0;
	s->write_ptr=NULL;

	s->read_sequence=0;
	s->write_sequence=0;
	s->trust_level=0;
	}

SSL *SSL_new(ctx)
SSL_CTX *ctx;
	{
	SSL *s;

	if (ctx == NULL)
		{
		SSLerr(SSL_F_SSL_NEW,SSL_R_NULL_SSL_CTX);
		return(NULL);
		}

	s=(SSL *)malloc(sizeof(SSL));
	if (s == NULL) goto err;

	s->rbio=NULL;
	s->wbio=NULL;
	s->init_buf=NULL;
	s->rbuf=(unsigned char *)malloc(SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER+2);
	if (s->rbuf == NULL) goto err;
	s->wbuf=(unsigned char *)malloc(SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER+2);
	if (s->wbuf == NULL) goto err;
	s->num_cipher_list=0;
	s->cipher_list=NULL;
	s->crypt_state=NULL;
	s->challenge=NULL;
	s->conn_id=NULL;
	s->key_material=NULL;
	s->session=NULL;
	s->cert=NULL;
	s->verify_mode=SSL_VERIFY_NONE;
	s->verify_callback=NULL;
	s->ctx=ctx;
	s->debug=0;
	s->info_callback=NULL;
	CS_BEGIN;
	CRYPTO_w_lock(CRYPTO_LOCK_SSL_CTX);
	ctx->references++;
	CRYPTO_w_unlock(CRYPTO_LOCK_SSL_CTX);
	CS_END;
	s->state_ccl=NULL;

	SSL_clear(s);
	return(s);
err:
	SSLerr(SSL_F_SSL_NEW,ERR_R_MALLOC_FAILURE);
	return(NULL);
	}

void SSL_free(s)
SSL *s;
	{
	if (s->rbio != NULL)
		BIO_free(s->rbio);
	if ((s->wbio != NULL) && (s->wbio != s->rbio))
		BIO_free(s->wbio);

	/* add extra stuff */
	free(s->rbuf);
	free(s->wbuf);
	if (s->cipher_list != NULL)
		{
		free(s->cipher_list[0]);
		free(s->cipher_list);
		}
	if (s->init_buf != NULL) free(s->init_buf);
	if ((s->session != NULL) && (s->session->cipher != NULL) &&
		(s->session->cipher->crypt_cleanup != NULL) &&
		(s->crypt_state != NULL))
		s->session->cipher->crypt_cleanup(s->crypt_state);
	if (s->session != NULL) SSL_SESSION_free(s->session);
	if (s->cert != NULL) ssl_cert_free(s->cert);
	if (s->challenge != NULL) free(s->challenge);
	if (s->conn_id != NULL) free(s->conn_id);
	if (s->key_material != NULL) free(s->key_material);
	if (s->state_ccl != NULL) free(s->state_ccl);
	/* free up if allocated */

	if (s->ctx) SSL_CTX_free(s->ctx);
	free((char *)s);
	}

void ssl_print_bytes(f, n, b)
FILE *f;
int n;
char *b;
	{
	int i;
	static char *h="0123456789abcdef";

	/* NULL means don't trace/log */
	if (f == NULL) return;

	fflush(f);
#if 0
	ssl_ddt_dump_fd(fileno(f),b,n);
#else

	for (i=0; i<n; i++)
		{
		fputc(h[(b[i]>>4)&0x0f],f);
		fputc(h[(b[i]   )&0x0f],f);
		fputc(' ',f);
		}
#endif

	}

void ssl_return_error(s)
SSL *s;
	{
	static char buf[1]={SSL_MT_ERROR};

	SSL_write(s,buf,1);
#ifdef ABORT_DEBUG
	abort(1);
#endif
	}

void SSL_set_bio(s, rbio,wbio)
SSL *s;
BIO *rbio;
BIO *wbio;
	{
	if ((s->rbio != NULL) && (s->rbio != rbio))
		BIO_free(s->rbio);
	if ((s->wbio != NULL) && (s->wbio != wbio) && (s->rbio != s->rbio))
		BIO_free(s->wbio);
	s->rbio=rbio;
	s->wbio=wbio;
	}

BIO *SSL_get_rbio(s)
SSL *s;
	{ return(s->rbio); }

BIO *SSL_get_wbio(s)
SSL *s;
	{ return(s->wbio); }

int SSL_get_fd(s)
SSL *s;
	{
	int ret=-1;
	BIO *b;

	b=SSL_get_rbio(s);
	if (b != NULL)
		BIO_get_fd(b,&ret);
	return(ret);
	}

int SSL_set_fd(s, fd)
SSL *s;
int fd;
	{
	int ret=0;
	BIO *bio=NULL;

	bio=BIO_new(BIO_s_socket());

	if (bio == NULL)
		{
		SSLerr(SSL_F_SSL_SET_FD,ERR_R_BUF_LIB);
		goto err;
		}
	BIO_set_fd(bio,&fd,BIO_NOCLOSE);
	if (s->rbio != NULL)
		BIO_free(s->rbio);
	if ((s->wbio != NULL) && (s->wbio != s->rbio))
		BIO_free(s->wbio);
	s->rbio=bio;
	s->wbio=bio;
	ret=1;
err:
	return(ret);
	}

int SSL_set_wfd(s, fd)
SSL *s;
int fd;
	{
	int ret=0;
	BIO *bio=NULL;

	bio=BIO_new(BIO_s_socket());

	if (bio == NULL)
		{ SSLerr(SSL_F_SSL_SET_WFD,ERR_R_BUF_LIB); goto err; }
	BIO_set_fd(bio,&fd,BIO_NOCLOSE);
	if (s->wbio != NULL) BIO_free(s->wbio);
	s->wbio=bio;
	ret=1;
err:
	return(ret);
	}

int SSL_set_rfd(s, fd)
SSL *s;
int fd;
	{
	int ret=0;
	BIO *bio=NULL;

	bio=BIO_new(BIO_s_socket());

	if (bio == NULL)
		{
		SSLerr(SSL_F_SSL_SET_RFD,ERR_R_BUF_LIB);
		goto err;
		}
	BIO_set_fd(bio,&fd,BIO_NOCLOSE);
	if (s->wbio != NULL) BIO_free(s->wbio);
	s->rbio=bio;
	ret=1;
err:
	return(ret);
	}

int SSL_set_cipher_list(s, str)
SSL *s;
char *str;
	{
	return(ssl_make_cipher_list(&s->cipher_list,&s->num_cipher_list,str));
	}

int ssl_make_cipher_list(rp,n,str)
char ***rp;
int *n;
char *str;
	{
	int i,j;
	char *p,**pp;

	if (str == NULL) return(1);
	if (rp == NULL) return(0);
	if (n == NULL) return(0);

	p=str;
	i=(*p == '\0')?0:1;
	for (; *p; p++)
		if (*p == ':') i++;
	pp=(char **)malloc(sizeof(char *)*(i+1));
	if (pp == NULL) goto err;
	pp[0]=(char *)malloc(strlen(str)+1);
	if (pp[0] == NULL) goto err;
	strcpy(pp[0],str);
	p=pp[0];
	j=1;
	for (;;)
		{
		while ((*p != ':') && (*p != '\0'))
			p++;
		if (*p == '\0') break;
		*p='\0';
		p++;
		pp[j++]=p;
		}
	pp[j]=NULL;
	if (*rp != NULL)
		{
		free(*rp[0]);
		free(*rp);
		}

	*rp=pp;
	*n=i;
	return(1);
err:
	SSLerr(SSL_F_SSL_MAKE_CIPHER_LIST,ERR_R_MALLOC_FAILURE);
	return(0);
	}

char *SSL_get_cipher_list(s, n)
SSL *s;
int n;
	{
	if (s->num_cipher_list > 0)
		{
		if (n < s->num_cipher_list)
			return(s->cipher_list[n]);
		}
	else if (s->ctx->num_cipher_list > 0)
		{
		if (n < s->ctx->num_cipher_list)
			return(s->ctx->cipher_list[n]);
		}
	else
		{
		if (n < num_pref_cipher)
			return(pref_cipher[n]);
		}
	return(NULL);
	}

char *SSL_get_cipher(s)
SSL *s;
	{
	if ((s->session != NULL) && (s->session->cipher != NULL))
		return(s->session->cipher->name);
	return(NULL);
	}

/* pdh */
int SSL_verified( SSL *s )
{
    if ( s->session )
        return s->session->verified;
    return 0;
}

char *SSL_get_shared_ciphers(s,buf,len)
SSL *s;
char *buf;
int len;
	{
	char *p,*cp;
	CIPHER **c;
	int i;

	if ((s->session == NULL) || (s->session->ciphers == NULL) ||
		(len < 2))
		return(NULL);

	p=buf;
	c= &(s->session->ciphers[0]);
	len--;
	for (i=0; i<s->session->num_ciphers; i++)
		{
		for (cp=c[i]->name; *cp; )
			{
			if (--len == 0)
				{
				*p='\0';
				return(buf);
				}
			else
				*(p++)= *(cp++);
			}
		*(p++)=':';
		}
	p[-1]='\0';
	return(buf);
	}

void SSL_set_verify(s, mode, callback)
SSL *s;
int mode;
int (*callback)();
	{
	s->verify_mode=mode;
	s->verify_callback=callback;
	}

void SSL_set_read_ahead(s, yes)
SSL *s;
int yes;
	{
	s->read_ahead=yes;
	}

int SSL_get_read_ahead(s)
SSL *s;
	{
	return(s->read_ahead);
	}

int SSL_pending(s)
SSL *s;
	{
	return(s->rbuf_left);
	}

X509 *SSL_get_peer_certificate(s)
SSL *s;
	{
	X509 *r;

	CRYPTO_r_lock(CRYPTO_LOCK_SSL_SESSION);
	CRYPTO_w_lock(CRYPTO_LOCK_SSL_CTX);

	if ((s == NULL) || (s->session == NULL))
		r=NULL;
	else
		r=s->session->peer;

	CRYPTO_r_unlock(CRYPTO_LOCK_SSL_SESSION);

	if (r == NULL)
		{
		CRYPTO_w_unlock(CRYPTO_LOCK_SSL_CTX);
		return(0);
		}

	r->references++;
	CRYPTO_w_unlock(CRYPTO_LOCK_SSL_CTX);

	return(r);
	}

int ssl_cipher_cmp(a,b)
CIPHER *a,*b;
	{
	int i;

	i=a->c1-b->c1;
	if (i) return(i);
	i=a->c2-b->c2;
	if (i) return(i);
	return(a->c3-b->c3);
	}

int ssl_cipher_ptr_cmp(ap,bp)
CIPHER **ap,**bp;
	{
	int i;
	CIPHER *a= *ap,*b= *bp;

	i=a->c1-b->c1;
	if (i) return(i);
	i=a->c2-b->c2;
	if (i) return(i);
	return(a->c3-b->c3);
	}

void ssl_generate_key_material(s)
SSL *s;
	{
	unsigned int i;
	MD5_CTX ctx;
	unsigned char *km;
	unsigned char c='0';

	km=s->key_material;
	for (i=0; i<s->key_material_length; i+=MD5_DIGEST_LENGTH)
		{
		MD5_Init(&ctx);

		MD5_Update(&ctx,s->session->master_key,s->session->master_key_length);
		MD5_Update(&ctx,(unsigned char *)&c,1);
		c++;
		MD5_Update(&ctx,s->challenge,s->challenge_length);
		MD5_Update(&ctx,s->conn_id,s->conn_id_length);
		MD5_Final(km,&ctx);
		km+=MD5_DIGEST_LENGTH;
		}
	}

/* Now in theory, since the calling process own 't' it should be safe to
 * modify.  We need to be able to read f without being hassled */
void SSL_copy_session_id(t,f)
SSL *t,*f;
	{
	CRYPTO_r_lock(CRYPTO_LOCK_SSL);

	SSL_set_session(t,SSL_get_session(f));

	CRYPTO_r_lock(CRYPTO_LOCK_X509);
	if (t->cert != NULL) ssl_cert_free(t->cert);
	if (f->cert != NULL) f->cert->references++;

	t->cert=f->cert;
	CRYPTO_r_unlock(CRYPTO_LOCK_X509);
	CRYPTO_r_unlock(CRYPTO_LOCK_SSL);
	}
