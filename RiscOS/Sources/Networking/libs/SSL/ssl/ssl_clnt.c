/* ssl/ssl_clnt.c */
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
#include "rand.h"
#include "buffer.h"
#include "objects.h"
#include "ssl_locl.h"
#include "envelope.h"

#ifndef NOPROTO
static int get_server_finished(SSL *s);
static int get_server_verify(SSL *s);
static int get_server_hello(SSL *s);
static int client_hello(SSL *s);
static int client_master_key(SSL *s);
static int client_finished(SSL *s);
static int client_certificate(SSL *s);
static int choose_cipher(SSL *s);
#else
static int get_server_finished();
static int get_server_verify();
static int get_server_hello();
static int client_hello();
static int client_master_key();
static int client_finished();
static int client_certificate();
static int choose_cipher();
#endif

#define BREAK	break

int SSL_connect(s)
SSL *s;
	{
	unsigned long l=time(NULL);
	BUF_MEM *buf=NULL;
	int ret= -1;
	void (*cb)()=NULL;
#ifdef SSL_DEBUG
	fprintf( stderr, "Now in ssl_connect\n" );
#endif

	RAND_seed((unsigned char *)&l,sizeof(l));
	ERR_clear_error();
	errno=0;

	if (s->info_callback != NULL)
		cb=s->info_callback;
	else if (s->ctx->info_callback != NULL)
		cb=s->ctx->info_callback;

	/* init things to blank */
	if (!SSL_in_init(s)) SSL_clear(s);
#ifdef SSL_DEBUG
	fprintf( stderr, "Starting main loop...\n" );
#endif

	for (;;)
		{
		int state;

#ifdef SSL_DEBUG
		SSL_TRACE(SSL_ERR,"state=%02X %d %s\n",
			s->state,s->init_num,SSL_state_string(s));
#endif

		state=s->state;

		switch (s->state)
			{
		case SSL_ST_BEFORE:
		case SSL_ST_CONNECT:
			if ((buf=BUF_MEM_new()) == NULL)
				{
				ret=-1;
				goto end;
				}
			if (!BUF_MEM_grow(buf,
				SSL_MAX_RECORD_LENGTH_3_BYTE_HEADER))
				{
				ret=-1;
				goto end;
				}
			if (s->init_buf) free(s->init_buf);
			s->init_buf=(unsigned char *)buf->data;
			free(buf);
			s->init_num=0;
			s->state=SSL_ST_SEND_CLIENT_HELLO_A;
			s->ctx->sess_connect++;
			BREAK;

		case SSL_ST_SEND_CLIENT_HELLO_A:
		case SSL_ST_SEND_CLIENT_HELLO_B:
			ret=client_hello(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_GET_SERVER_HELLO_A;
			BREAK;

		case SSL_ST_GET_SERVER_HELLO_A:
		case SSL_ST_GET_SERVER_HELLO_B:
			ret=get_server_hello(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			if (!s->hit) /* new session */
				{
				s->state=SSL_ST_SEND_CLIENT_MASTER_KEY_A;
				BREAK;
				}
			else
				{
				s->state=SSL_ST_CLIENT_START_ENCRYPTION;
				break;
				}

		case SSL_ST_SEND_CLIENT_MASTER_KEY_A:
		case SSL_ST_SEND_CLIENT_MASTER_KEY_B:
			ret=client_master_key(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_CLIENT_START_ENCRYPTION;
			break;

		case SSL_ST_CLIENT_START_ENCRYPTION:
			/* Ok, we now have all the stuff needed to
			 * start encrypting, so lets fire it up :-) */
			if (!s->session->cipher->crypt_init(s,1))
				{
				ret=-1;
				goto end;
				}
			s->clear_text=0;
			s->state=SSL_ST_SEND_CLIENT_FINISHED_A;
			break;

		case SSL_ST_SEND_CLIENT_FINISHED_A:
		case SSL_ST_SEND_CLIENT_FINISHED_B:
			ret=client_finished(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_GET_SERVER_VERIFY_A;
			break;

		case SSL_ST_GET_SERVER_VERIFY_A:
		case SSL_ST_GET_SERVER_VERIFY_B:
			ret=get_server_verify(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_GET_SERVER_FINISHED_A;
			break;

		case SSL_ST_GET_SERVER_FINISHED_A:
		case SSL_ST_GET_SERVER_FINISHED_B:
			ret=get_server_finished(s);
			if (ret <= 0) goto end;
			break;

		case SSL_ST_SEND_CLIENT_CERTIFICATE_A:
		case SSL_ST_SEND_CLIENT_CERTIFICATE_B:
		case SSL_ST_SEND_CLIENT_CERTIFICATE_C:
		case SSL_ST_SEND_CLIENT_CERTIFICATE_D:
		case SSL_ST_X509_GET_CLIENT_CERTIFICATE:
			ret=client_certificate(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_GET_SERVER_FINISHED_A;
			break;

		case SSL_ST_OK:
			free(s->init_buf);
			s->init_buf=NULL;
			s->init_num=0;
			ERR_clear_error();

			if (SSL_add_session(s->ctx,s->session) &&
				(s->ctx->new_session_cb != NULL))
				{
				CRYPTO_w_lock(CRYPTO_LOCK_SSL_SESSION);
				s->session->references++;
				CRYPTO_w_unlock(CRYPTO_LOCK_SSL_SESSION);

				if (!s->ctx->new_session_cb(s->session))
					{
					CRYPTO_w_lock(CRYPTO_LOCK_SSL_SESSION);
					s->session->references--;
					CRYPTO_w_unlock(CRYPTO_LOCK_SSL_SESSION);
					}
				}
			s->ctx->sess_connect_good++;
			ret=1;
			goto end;
			break;
		default:
			SSLerr(SSL_F_SSL_CONNECT,SSL_R_UNKNOWN_STATE);
			return(-1);
			/* break; */
			}

		if ((cb != NULL) && (s->state != state))
			cb(s,SSL_CB_CONNECT_LOOP,1);
		}
end:
	if (cb != NULL)
		cb(s,SSL_CB_CONNECT_EXIT,ret);
	return(ret);
	}

static int get_server_hello(s)
SSL *s;
	{
	unsigned char *buf;
	unsigned char *p;
	CIPHER **cipher;
	CIPHER *tmp_c[NUM_CIPHERS];
	int i,j,num_c,nnum_c;

	buf=s->init_buf;
	p=buf;
	if (s->state == SSL_ST_GET_SERVER_HELLO_A)
		{
		i=SSL_read(s,(char *)&(buf[s->init_num]),11-s->init_num);
		if (i < (11-s->init_num))
			return(ssl_part_read(s,SSL_F_GET_SERVER_HELLO,i));

		if (*(p++) != SSL_MT_SERVER_HELLO)
			{
			if (p[-1] != SSL_MT_ERROR)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_GET_SERVER_HELLO,
					SSL_R_READ_WRONG_PACKET_TYPE);
				}
			else
				SSLerr(SSL_F_GET_SERVER_HELLO,
					SSL_R_PEER_ERROR);
			return(-1);
			}
		s->hit=(*(p++))?1:0;
		s->state_cert_type= *(p++);
		n2s(p,i);
		if (i < s->version) s->version=i;
		n2s(p,i); s->state_cert_length=i;
		n2s(p,i); s->state_csl=i;
		n2s(p,i); s->state_conn_id_length=i;
		s->state=SSL_ST_GET_SERVER_HELLO_B;
		s->init_num=0;
		}

	/* SSL_ST_GET_SERVER_HELLO_B */
	j=s->state_cert_length+s->state_csl+s->state_conn_id_length
		- s->init_num;
	i=SSL_read(s,(char *)&(buf[s->init_num]),j);
	if (i != j) return(ssl_part_read(s,SSL_F_GET_SERVER_HELLO,i));

	/* things are looking good */

	p=buf;
	if (s->hit)
		{
		if ((s->state_cert_length != 0) ||
			/*(s->state_cert_type != 0) || */ (s->state_csl != 0))
			{
			/*return(-1);*/
			/* ignore these errors :-(
			 * SSLref does return a non-zero cert_type */
			}
		}
	else
		{
		if (s->session->session_id != NULL) free(s->session->session_id);
		s->session->session_id=NULL;
		s->session->session_id_length=0;

		if (ssl_set_certificate(s,s->state_cert_type,
			s->state_cert_length,p) <= 0)
			{
			ssl_return_error(s);
			return(-1);
			}
		p+=s->state_cert_length;

		if (s->state_csl == 0)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_GET_SERVER_HELLO,SSL_R_NO_CIPHER_LIST);
			return(-1);
			}

		/* what we will do is copy the ciphers we are happy to
		 * use to a temp area and then check the server ones
		 * are put those that are in the session structure again. */
		cipher= &(s->session->ciphers[0]);
		memcpy(&(tmp_c[0]),cipher,sizeof(CIPHER *)*NUM_CIPHERS);
		num_c=s->session->num_ciphers;
		nnum_c=0;
		for (i=0; i<(int)s->state_csl; i+=3)
			{
			CIPHER c,*cp= &c,**cpp;

			c.c1=p[i];
			c.c2=p[i+1];
			c.c3=p[i+2];

			cpp=(CIPHER **)OBJ_bsearch((char *)&cp,
				(char *)&(cipher[0]),num_c,sizeof(CIPHER *),
				(int (*)())ssl_cipher_ptr_cmp);
			if ((cpp != NULL) && ((*cpp)->valid))
				{
				s->session->ciphers[nnum_c++]= *cpp;
				}
			if (nnum_c == NUM_CIPHERS) break;
			}
		p+=s->state_csl;
		s->session->num_ciphers=nnum_c;

		if (nnum_c == 0)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_GET_SERVER_HELLO,SSL_R_NO_CIPHER_MATCH);
			return(-1);
			}
		qsort((char *)&(cipher[0]),nnum_c,sizeof(CIPHER *),
			(int (*)(P_CC_CC))ssl_cipher_ptr_cmp);

		/* we now have the sorted cipher in s->session->ciphers; */

		/* pick a cipher - which one? */
		if (!choose_cipher(s))
			{
			ssl_return_error(s);
			SSLerr(SSL_F_GET_SERVER_HELLO,SSL_R_NO_CIPHER_WE_TRUST);
			return(-1);
			}
		}

	if ((s->session != NULL) && (s->session->peer != NULL))
		X509_free(s->session->peer);
	s->session->peer=s->session->cert->x509;

	/* hmmm, can we have the problem of the other session with this
	 * cert, free's it before we increment the reference count. */
	CRYPTO_w_lock(CRYPTO_LOCK_X509);
	s->session->peer->references++;
	CRYPTO_w_lock(CRYPTO_LOCK_X509);

	/* get conn_id */
	if (s->conn_id_length != s->state_conn_id_length)
		{
		if (s->conn_id) free(s->conn_id);
		s->conn_id=(unsigned char *)malloc(s->state_conn_id_length);
		if (s->conn_id == NULL)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_GET_SERVER_HELLO,ERR_R_MALLOC_FAILURE);
			return(-1);
			}
		}
	s->conn_id_length=s->state_conn_id_length;
	memcpy(s->conn_id,p,s->state_conn_id_length);
	return(1);
	}

static int client_hello(s)
SSL *s;
	{
	unsigned char *buf;
	unsigned char *p,*d;
	CIPHER **cipher;
	int i,n,j;

	buf=s->init_buf;
	if (s->state == SSL_ST_SEND_CLIENT_HELLO_A)
		{
		if (s->session == NULL)
			if (!ssl_get_new_session(s,0))
				{
				ssl_return_error(s);
				return(-1);
				}
		/* else use the pre-loaded session */

		cipher= &(s->session->ciphers[0]);
		p=buf;					/* header */
		d=p+9;					/* data section */
		*(p++)=SSL_MT_CLIENT_HELLO;		/* type */
		s2n(SSL_CLIENT_VERSION,p);		/* version */
		n=j=0;
		for (i=0; i<NUM_CIPHERS; i++)
			{
			if (ssl_ciphers[i].valid)
				{
				cipher[j++]= &(ssl_ciphers[i]);
				*(d++)=ssl_ciphers[i].c1;
				*(d++)=ssl_ciphers[i].c2;
				*(d++)=ssl_ciphers[i].c3;
				n+=3;
				}
			}
		s->session->num_ciphers=j;
		s2n(n,p);			/* cipher spec num bytes */

		if (s->session->session_id != NULL)
			{
			i=s->session->session_id_length;
			s2n(i,p);		/* session id length */
			memcpy(d,s->session->session_id,(unsigned int)i);
			d+=i;
			}
		else
			{
			s2n(0,p);
			}

		s->challenge_length=CHALLENGE_LENGTH;
		s2n(CHALLENGE_LENGTH,p);		/* challenge length */
		s->challenge=(unsigned char *)malloc(CHALLENGE_LENGTH);
		if (s->challenge == NULL)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_CLIENT_HELLO,ERR_R_MALLOC_FAILURE);
			return(-1);
			}
		if (s->ctx->reverse)
			{
			memcpy(s->challenge,s->ctx->challenge,
				s->ctx->challenge_length);
			}
		else
			/*challenge id data*/
			RAND_bytes(s->challenge,CHALLENGE_LENGTH);
		memcpy(d,s->challenge,CHALLENGE_LENGTH);
		d+=CHALLENGE_LENGTH;

		s->state=SSL_ST_SEND_CLIENT_HELLO_B;
		s->init_num=d-buf;
		s->init_off=0;
		}
	/* SSL_ST_SEND_CLIENT_HELLO_B */
	return(ssl_do_write(s));
	}

#ifdef SSL_DEBUG
#define ptrace fprintf
#else
#define ptrace 1?0:fprintf
#endif

static int client_master_key(s)
SSL *s;
	{
	unsigned char *buf;
	unsigned char *p,*d;
	int clear,enc,karg,i;
#ifdef SSL_DEBUG
        fprintf( stderr, "In client_master_key\n" );
#endif
	buf=s->init_buf;
	if (s->state == SSL_ST_SEND_CLIENT_MASTER_KEY_A)
		{
		p=buf;
		d=p+10;
		*(p++)=SSL_MT_CLIENT_MASTER_KEY;/* type */
		*(p++)=s->session->cipher->c1;	/* cipher type - byte 1 */
		*(p++)=s->session->cipher->c2;	/* cipher type - byte 2 */
		*(p++)=s->session->cipher->c3;	/* cipher type - byte 3 */

		/* make a master key */
		i=s->session->master_key_length=(unsigned char)
			s->session->cipher->key_size;

		s->session->master_key=(unsigned char *)malloc((unsigned int)i+1);
		if (s->session->master_key == NULL)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_CLIENT_MASTER_KEY,ERR_R_MALLOC_FAILURE);
			return(-1);
			}
		if (s->ctx->reverse)
			{
			if ((int)s->ctx->master_key_length != i)
				{
				SSLerr(SSL_F_CLIENT_MASTER_KEY,SSL_R_REVERSE_MASTER_KEY_LENGTH_IS_WRONG);
				return(-1);
				}
			memcpy(s->session->master_key,s->ctx->master_key,i);
			}
		else
			RAND_bytes(s->session->master_key,i);

                ptrace( stderr, "making key_arg data\n" );
		/* make key_arg data */
		i=s->session->key_arg_length=s->session->cipher->key_arg_size;
		if (i == 0)
			s->session->key_arg=NULL;
		else
			{
			s->session->key_arg=(unsigned char *)
				malloc((unsigned int)i);
			if (s->session->key_arg == NULL)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_CLIENT_MASTER_KEY,ERR_R_MALLOC_FAILURE);
				return(-1);
				}
			if (s->ctx->reverse)
				{
				if ((int)s->ctx->key_arg_length != i)
					{
					SSLerr(SSL_F_CLIENT_MASTER_KEY,SSL_R_REVERSE_KEY_ARG_LENGTH_IS_WRONG);
					return(-1);
					}
				memcpy(s->session->key_arg,s->ctx->key_arg,i);
				}
			else
				RAND_bytes(s->session->key_arg,i);
			}

		i=s->session->cipher->key_size*8;
		enc=s->session->cipher->enc_bits;
		if (enc == 0) enc=i;
		if (i < enc)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_CLIENT_MASTER_KEY,SSL_R_CIPHER_TABLE_SRC_ERROR);
			return(-1);
			}
		clear=i-enc;
		/* bytes or bits? */
		clear/=8; /* clear */
		s2n(clear,p);
		memcpy(d,s->session->master_key,(unsigned int)clear);
		d+=clear;

                ptrace( stderr, "public_encrypt = %p\n",
                                s->session->cert->public_encrypt );

		enc/=8;
		enc=s->session->cert->public_encrypt(s->session->cert,enc,
			&(s->session->master_key[clear]),d);
                ptrace( stderr, "after public_encrypt\n" );

		if (enc <= 0)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_CLIENT_MASTER_KEY,SSL_R_PUBLIC_KEY_ENCRYPT_ERROR);
			return(-1);
			}
		s2n(enc,p);
		d+=enc;
		karg=s->session->cipher->key_arg_size;
		s2n(karg,p); /* key arg size */
		memcpy(d,s->session->key_arg,(unsigned int)karg);
		d+=karg;

		s->state=SSL_ST_SEND_CLIENT_MASTER_KEY_B;
		s->init_num=d-buf;
		s->init_off=0;
		}

        ptrace( stderr, "ssl_do_write\n" );

	/* SSL_ST_SEND_CLIENT_MASTER_KEY_B */
	return(ssl_do_write(s));
	}

static int client_finished(s)
SSL *s;
	{
	unsigned char *p;

	if (s->state == SSL_ST_SEND_CLIENT_FINISHED_A)
		{
		p=s->init_buf;
		*(p++)=SSL_MT_CLIENT_FINISHED;
		memcpy(p,s->conn_id,(unsigned int)s->conn_id_length);

		s->state=SSL_ST_SEND_CLIENT_FINISHED_B;
		s->init_num=s->conn_id_length+1;
		s->init_off=0;
		}
	return(ssl_do_write(s));
	}

/* read the data and then respond */
static int client_certificate(s)
SSL *s;
	{
	unsigned char *buf;
	unsigned char *p,*d;
	int i;
	unsigned int n;
	int cert_ch_len=0;
	unsigned char *cert_ch;

	buf=s->init_buf;
	cert_ch= &(buf[2]);

	/* We have a cert associated with out SSL, so attach it to
	 * the session if it does not have one */

	if (s->state == SSL_ST_SEND_CLIENT_CERTIFICATE_A)
		{
		i=SSL_read(s,(char *)&(buf[s->init_num]),
			MAX_CERT_CHALLENGE_LENGTH+1-s->init_num);
		if (i<(MIN_CERT_CHALLENGE_LENGTH+1-s->init_num))
			return(ssl_part_read(s,SSL_F_CLIENT_CERTIFICATE,i));

		/* type=buf[0]; */
		/* type eq x509 */
		if (buf[1] != SSL_AT_MD5_WITH_RSA_ENCRYPTION)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_CLIENT_CERTIFICATE,SSL_R_BAD_AUTHENTICATION_TYPE);
			return(-1);
			}
		cert_ch_len=i-1;

		if ((s->cert == NULL) || (s->cert->x509 == NULL) ||
			(s->cert->privatekey == NULL))
			{
			s->state=SSL_ST_X509_GET_CLIENT_CERTIFICATE;
			}
		else
			s->state=SSL_ST_SEND_CLIENT_CERTIFICATE_C;
		}

	if (s->state == SSL_ST_X509_GET_CLIENT_CERTIFICATE)
		{
		/* If we get an error we need to
		 * ssl->rwstate=SSL_X509_LOOKUP;
		 * return(error);
		 * We should then be retried when things are ok and we
		 * can get a cert or not */
		if (0) /* we get a good client cert */
			{
			/*s->cert=NULL; */ /* ! NULL */
			s->cert->x509=NULL; /* new cert */
			s->cert->privatekey=NULL; /* new pkey */
			/* This is where we need to call the application
			 * to give us a client certificate.
			 * This need to support blocking */
			s->state=SSL_ST_SEND_CLIENT_CERTIFICATE_C;
			}
		else
			{
			/* We have no client certificate to respond with
			 * so send the correct error message back */
			s->state=SSL_ST_SEND_CLIENT_CERTIFICATE_B;
			p=buf;
			*(p++)=SSL_MT_ERROR;
			s2n(SSL_PE_NO_CERTIFICATE,p);
			s->init_off=0;
			s->init_num=3;
			/* Write is done at the end */
			}
		}

	if (s->state == SSL_ST_SEND_CLIENT_CERTIFICATE_B)
		{
		return(ssl_do_write(s));
		}

	if (s->state == SSL_ST_SEND_CLIENT_CERTIFICATE_C)
		{
		EVP_MD_CTX ctx;

		/* ok, now we calculate the checksum
		 * do it first so we can reuse buf :-) */
		p=buf;
		EVP_SignInit(&ctx,EVP_md5());
		EVP_SignUpdate(&ctx,s->key_material,
			(unsigned int)s->key_material_length);
		EVP_SignUpdate(&ctx,cert_ch,(unsigned int)cert_ch_len);
		n=i2d_X509(s->session->cert->x509,&p);
		EVP_SignUpdate(&ctx,buf,(unsigned int)n);

		p=buf;
		d=p+6;
		*(p++)=SSL_MT_CLIENT_CERTIFICATE;
		*(p++)=SSL_CT_X509_CERTIFICATE;
		n=i2d_X509(s->cert->x509,&d);
		s2n(n,p);

		if (!EVP_SignFinal(&ctx,d,&n,s->cert->privatekey))
			{
			/* this is not good.  If things have failed it
			 * means there so something wrong with the key.
			 * We will contiune with a 0 length signature
			 */
			}
		memset(&ctx,0,sizeof(ctx));
		s2n(n,p);
		d+=n;

		s->state=SSL_ST_SEND_CLIENT_CERTIFICATE_D;
		s->init_num=d-buf;
		s->init_off=0;
		}
	/* if (s->state == SSL_ST_SEND_CLIENT_CERTIFICATE_D) */
	return(ssl_do_write(s));
	}

static int get_server_verify(s)
SSL *s;
	{
	unsigned char *p;
	int i;

	p=s->init_buf;
	if (s->state == SSL_ST_GET_SERVER_VERIFY_A)
		{
		i=SSL_read(s,(char *)&(p[s->init_num]),1-s->init_num);
		if (i < (1-s->init_num))
			return(ssl_part_read(s,SSL_F_GET_SERVER_VERIFY,i));

		s->state= SSL_ST_GET_SERVER_VERIFY_B;
		s->init_num=0;
		if (*p != SSL_MT_SERVER_VERIFY)
			{
			if (p[0] != SSL_MT_ERROR)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_GET_SERVER_VERIFY,
					SSL_R_READ_WRONG_PACKET_TYPE);
				}
			else
				SSLerr(SSL_F_GET_SERVER_VERIFY,
					SSL_R_PEER_ERROR);
			return(-1);
			}
		}

	p=s->init_buf;
	i=SSL_read(s,(char *)&(p[s->init_num]),
		(unsigned int)s->challenge_length-s->init_num);
	if (i < ((int)s->challenge_length-s->init_num))
		return(ssl_part_read(s,SSL_F_GET_SERVER_VERIFY,i));
	if (memcmp(p,s->challenge,(unsigned int)s->challenge_length) != 0)
		{
		ssl_return_error(s);
		SSLerr(SSL_F_GET_SERVER_VERIFY,SSL_R_CHALLENGE_IS_DIFFERENT);
		return(-1);
		}
	return(1);
	}

static int get_server_finished(s)
SSL *s;
	{
	unsigned char *buf;
	unsigned char *p;
	int i;

	buf=s->init_buf;
	p=buf;
	if (s->state == SSL_ST_GET_SERVER_FINISHED_A)
		{
		i=SSL_read(s,(char *)&(buf[s->init_num]),1-s->init_num);
		if (i < (1-s->init_num))
			return(ssl_part_read(s,SSL_F_GET_SERVER_FINISHED,i));
		s->init_num=i;
		if (*p == SSL_MT_REQUEST_CERTIFICATE)
			{
			s->state=SSL_ST_SEND_CLIENT_CERTIFICATE_A;
			return(1);
			}
		else if (*p != SSL_MT_SERVER_FINISHED)
			{
			if (p[0] != SSL_MT_ERROR)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_GET_SERVER_FINISHED,SSL_R_READ_WRONG_PACKET_TYPE);
				}
			else
				SSLerr(SSL_F_GET_SERVER_FINISHED,SSL_R_PEER_ERROR);
			return(-1);
			}
		s->state=SSL_ST_OK;
		s->init_num=0;
		}

	i=SSL_read(s,(char *)&(buf[s->init_num]),
		SSL_SESSION_ID_LENGTH-s->init_num);
	if (i < (SSL_SESSION_ID_LENGTH-s->init_num))
		return(ssl_part_read(s,SSL_F_GET_SERVER_FINISHED,i));

	if (!s->hit) /* new session */
		{
		if ((s->session->session_id == NULL) ||
			(s->session->session_id_length <
				SSL_SESSION_ID_LENGTH))
			{
			if (s->session->session_id)
				free(s->session->session_id);
			s->session->session_id=(unsigned char *)
				malloc(SSL_SESSION_ID_LENGTH);
			if (s->session->session_id == NULL)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_GET_SERVER_FINISHED,
					ERR_R_MALLOC_FAILURE);
				return(-1);
				}
			}
		s->session->session_id_length=SSL_SESSION_ID_LENGTH;
		memcpy(s->session->session_id,p,SSL_SESSION_ID_LENGTH);
		}
	else
		{
		if (memcmp(buf,s->session->session_id,
			(unsigned int)s->session->session_id_length) != 0)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_GET_SERVER_FINISHED,SSL_R_SSL_SESSION_ID_IS_DIFFERENT);
			return(-1);
			}
		}
	return(1);
	}

static int choose_cipher(s)
SSL *s;
	{
	int j,i;
	char *p;

	if (s->ctx->reverse)
		{
		for (j=0; j < s->session->num_ciphers; j++)
			{
			if (strcmp(s->ctx->cipher,
				s->session->ciphers[j]->name) == 0)
				{
				s->session->cipher=s->session->ciphers[j];
				return(1);
				}
			}
		return(0);
		}
	i=0;
	for (;;)
		{
		p=SSL_get_cipher_list(s,i++);
		if (p == NULL)
			{
			s->session->cipher=NULL;
			return(0);
			}
		for (j=0; j < s->session->num_ciphers; j++)
			{
			if (strcmp(p,s->session->ciphers[j]->name) == 0)
				{
				s->session->cipher=s->session->ciphers[j];
				return(1);
				}
			}
		}
	}

