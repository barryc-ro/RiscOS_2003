/* ssl/ssl_srvr.c */
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
#include "buffer.h"
#include "rand.h"
#include "objects.h"
#include "ssl_locl.h"
#include "envelope.h"

#ifndef NOPROTO
static int get_client_master_key(SSL *s);
static int get_client_hello(SSL *s);
static int server_hello(SSL *s); 
static int get_client_finished(SSL *s);
static int server_verify(SSL *s);
static int server_finish(SSL *s);
static int request_certificate(SSL *s);
static int ssl_cipher_pstrcmp(CIPHER **ap,CIPHER **bp);
#else
static int get_client_master_key();
static int get_client_hello();
static int server_hello(); 
static int get_client_finished();
static int server_verify();
static int server_finish();
static int request_certificate();
static int ssl_cipher_pstrcmp();
#endif

#define BREAK	break

int SSL_accept(s)
SSL *s;
	{
	unsigned long l=time(NULL);
	BUF_MEM *buf=NULL;
	int ret= -1;
	void (*cb)()=NULL;

	RAND_seed((unsigned char *)&l,sizeof(l));
	ERR_clear_error();
	errno=0;

	if (s->info_callback != NULL)
		cb=s->info_callback;
	else if (s->ctx->info_callback != NULL)
		cb=s->ctx->info_callback;

	/* init things to blank */
	if (!SSL_in_init(s)) SSL_clear(s);

	if (((s->session == NULL) || (s->session->cert == NULL)) &&
		(s->cert == NULL))
		{
		SSLerr(SSL_F_SSL_ACCEPT,SSL_R_NO_CERTIFICATE_SET);
		return(-1);
		}

	errno=0;
	for (;;)
		{
		int state;

#ifdef SSL_DEBUG
		SSL_TRACE(SSL_ERR,"state=%02X %d %s\n",
			s->state,s->init_num,ssl_state_string(s));
#endif
		state=s->state;

		switch (s->state)
			{
		case SSL_ST_BEFORE:
		case SSL_ST_ACCEPT:
			if ((buf=BUF_MEM_new()) == NULL)
				{ ret=-1; goto end; }
			if (!BUF_MEM_grow(buf,(int)
				SSL_MAX_RECORD_LENGTH_3_BYTE_HEADER))
				{ ret=-1; goto end; }
			if (s->init_buf) free(s->init_buf);
			s->init_buf=(unsigned char *)buf->data;
			free(buf);
			s->init_num=0;
			s->state=SSL_ST_GET_CLIENT_HELLO_A;
			s->ctx->sess_accept++;
			BREAK;

		case SSL_ST_GET_CLIENT_HELLO_A:
		case SSL_ST_GET_CLIENT_HELLO_B:
			ret=get_client_hello(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_SEND_SERVER_HELLO_A;
			BREAK;

		case SSL_ST_SEND_SERVER_HELLO_A:
		case SSL_ST_SEND_SERVER_HELLO_B:
			ret=server_hello(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			if (!s->hit)
				{
				s->state=SSL_ST_GET_CLIENT_MASTER_KEY_A;
				BREAK;
				}
			else
				{
				s->state=SSL_ST_SERVER_START_ENCRYPTION;
				BREAK;
				}
		case SSL_ST_GET_CLIENT_MASTER_KEY_A:
		case SSL_ST_GET_CLIENT_MASTER_KEY_B:
			ret=get_client_master_key(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_SERVER_START_ENCRYPTION;
			BREAK;

		case SSL_ST_SERVER_START_ENCRYPTION:
			/* Ok we how have sent all the stuff needed to
			 * start encrypting, the next packet back will
			 * be encrypted. */
			if (!s->session->cipher->crypt_init(s,0))
				{
				ret=-1;
				goto end;
				}
			s->clear_text=0;
			s->state=SSL_ST_SEND_SERVER_VERIFY_A;
			BREAK;

		case SSL_ST_SEND_SERVER_VERIFY_A:
		case SSL_ST_SEND_SERVER_VERIFY_B:
			ret=server_verify(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_GET_CLIENT_FINISHED_A;
			BREAK;

		case SSL_ST_GET_CLIENT_FINISHED_A:
		case SSL_ST_GET_CLIENT_FINISHED_B:
			ret=get_client_finished(s);
			if (ret <= 0)
				goto end;
			s->init_num=0;
			s->state=SSL_ST_SEND_REQUEST_CERTIFICATE_A;
			BREAK;

		case SSL_ST_SEND_REQUEST_CERTIFICATE_A:
		case SSL_ST_SEND_REQUEST_CERTIFICATE_B:
		case SSL_ST_SEND_REQUEST_CERTIFICATE_C:
		case SSL_ST_SEND_REQUEST_CERTIFICATE_D:
			/* don't do a 'request certificate' if we
			 * don't want to, or we already have one, or
			 * we once want to ever get one once. */
			if (!(s->verify_mode & SSL_VERIFY_PEER)
				|| (s->session->peer != NULL)
				|| (s->verify_mode & SSL_VERIFY_CLIENT_ONCE))
				{
				s->state=SSL_ST_SEND_SERVER_FINISHED_A;
				break;
				}
			else
				{
				ret=request_certificate(s);
				if (ret <= 0) goto end;
				s->init_num=0;
				s->state=SSL_ST_SEND_SERVER_FINISHED_A;
				}
			BREAK;

		case SSL_ST_SEND_SERVER_FINISHED_A:
		case SSL_ST_SEND_SERVER_FINISHED_B:
			ret=server_finish(s);
			if (ret <= 0) goto end;
			s->init_num=0;
			s->state=SSL_ST_OK;
			break;

		case SSL_ST_OK:
			free(s->init_buf);
			s->init_buf=NULL;
			s->init_num=0;
			ERR_clear_error();

			if (SSL_add_session(s->ctx,s->session) &&
				(s->ctx->new_session_cb != NULL))
				{
				CS_BEGIN;
				s->session->references++;
				CS_END;
				if (!s->ctx->new_session_cb(s->session))
					{
					CS_BEGIN;
					s->session->references--;
					CS_END;
					}
				}

			s->ctx->sess_accept_good++;
			ret=1;
			goto end;
			BREAK;

		default:
			SSLerr(SSL_F_SSL_ACCEPT,SSL_R_UNKNOWN_STATE);
			ret=-1;
			goto end;
			BREAK;
			}
		
		if ((cb != NULL) && (s->state != state))
			cb(s,SSL_CB_ACCEPT_LOOP,1);
		}
end:
	if (cb != NULL)
		cb(s,SSL_CB_ACCEPT_EXIT,ret);
	return(ret);
	}

static int get_client_master_key(s)
SSL *s;
	{
	int i,j,n,keya;
	unsigned char *p;
	CIPHER *cp,c;

	p=s->init_buf;
	if (s->state == SSL_ST_GET_CLIENT_MASTER_KEY_A)
		{
		i=SSL_read(s,(char *)&(p[s->init_num]),10-s->init_num);

		if (i < (10-s->init_num))
			return(ssl_part_read(s,SSL_F_GET_CLIENT_MASTER_KEY,i));
		if (*(p++) != SSL_MT_CLIENT_MASTER_KEY)
			{
			if (p[-1] != SSL_MT_ERROR)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_GET_CLIENT_MASTER_KEY,SSL_R_READ_WRONG_PACKET_TYPE);
				}
			else
				SSLerr(SSL_F_GET_CLIENT_MASTER_KEY,
					SSL_R_PEER_ERROR);
			return(-1);
			}

		c.c1=p[0];
		c.c2=p[1];
		c.c3=p[2];
		cp=(CIPHER *)OBJ_bsearch((char *)&c,(char *)ssl_ciphers,
			NUM_CIPHERS,sizeof(CIPHER),(int (*)())ssl_cipher_cmp);
		if ((cp == NULL) || (!cp->valid))
			{
			ssl_return_error(s);
			SSLerr(SSL_F_GET_CLIENT_MASTER_KEY,
				SSL_R_NO_CIPHER_MATCH);
			return(-1);
			}
		s->session->cipher= cp;
		p+=3;
		n2s(p,i); s->state_clear=i;
		n2s(p,i); s->state_enc=i;
		n2s(p,i); s->session->key_arg_length=i;
		s->state=SSL_ST_GET_CLIENT_MASTER_KEY_B;
		s->init_num=0;
		}

	/* SSL_ST_GET_CLIENT_MASTER_KEY_B */
	p=s->init_buf;
	keya=s->session->key_arg_length;
	n=s->state_clear+s->state_enc+keya - s->init_num;
	i=SSL_read(s,(char *)&(p[s->init_num]),(unsigned int)n);
	if (i != n) return(ssl_part_read(s,SSL_F_GET_CLIENT_MASTER_KEY,i));

	/* do key_arg before we unpack the crypted key. */
	if (keya > 0)
		{
		s->session->key_arg=(unsigned char *)malloc((unsigned int)keya);
		if (s->session->key_arg == NULL)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_GET_CLIENT_MASTER_KEY,
				ERR_R_MALLOC_FAILURE);
			return(-1);
			}
		}

	memcpy(s->session->key_arg,&(p[s->state_clear+s->state_enc]),
		(unsigned int)keya);

	if (s->session->cert->privatekey == NULL)
		{
		ssl_return_error(s);
		SSLerr(SSL_F_GET_CLIENT_MASTER_KEY,SSL_R_NO_PRIVATEKEY);
		return(-1);
		}
	i=s->cert->private_decrypt(s->cert,s->state_enc,
		&(p[s->state_clear]),&(p[s->state_clear]));
	j=s->session->cipher->enc_bits/8;
	/* fix so we will not accept clear ciphers when using
	 * non-export ciphers */
	if (	((j == 0) && (i != (int)(s->session->cipher->key_size))) ||
		((j != 0) && (i != j)))
		{
		SSL_TRACE(SSL_ERR,"i=%d enc_bits=%d\n",i,
			s->session->cipher->enc_bits/8);
		ssl_return_error(s);
		return(-1);
		}
	i+=s->state_clear;
	s->session->master_key_length=i;
	s->session->master_key=(unsigned char *)malloc((unsigned int)i+1);
	memcpy(s->session->master_key,p,(unsigned int)i);
	return(1);
	}

static int get_client_hello(s)
SSL *s;
	{
	int i,j,n;
	unsigned char *p;
	CIPHER **cipher;

	p=s->init_buf;
	if (s->state == SSL_ST_GET_CLIENT_HELLO_A)
		{
		i=SSL_read(s,(char *)&(p[s->init_num]),9-s->init_num);
		if (i < (9-s->init_num)) 
			return(ssl_part_read(s,SSL_F_GET_CLIENT_HELLO,i));
	
		if (*(p++) != SSL_MT_CLIENT_HELLO)
			{
			if (p[-1] != SSL_MT_ERROR)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_GET_CLIENT_HELLO,SSL_R_READ_WRONG_PACKET_TYPE);
				}
			else
				SSLerr(SSL_F_GET_CLIENT_HELLO,SSL_R_PEER_ERROR);
			return(-1);
			}
		n2s(p,i);
		if (i < s->version) s->version=i;
		n2s(p,i); s->state_cipher_spec_length=i;
		n2s(p,i); s->state_session_id_length=i;
		n2s(p,i); s->challenge_length=i;
		if (	(i < MIN_CHALLENGE_LENGTH) ||
			(i > MAX_CHALLENGE_LENGTH))
			{
			SSLerr(SSL_F_GET_CLIENT_HELLO,SSL_R_INVALID_CHALLENGE_LENGTH);
			return(-1);
			}
		s->state=SSL_ST_GET_CLIENT_HELLO_B;
		s->init_num=0;
		}

	/* SSL_ST_GET_CLIENT_HELLO_B */
	p=s->init_buf;
	n=s->state_cipher_spec_length+s->challenge_length+
		s->state_session_id_length-s->init_num;
	i=SSL_read(s,(char *)&(p[s->init_num]),(unsigned int)n);
	if (i != n) return(ssl_part_read(s,SSL_F_GET_CLIENT_HELLO,i));

	/* get session-id before cipher stuff so we can get out session
	 * structure if it is cached */
	/* session-id */
	if ((s->state_session_id_length != 0) && 
		(s->state_session_id_length != SSL_SESSION_ID_LENGTH))
		{
		ssl_return_error(s);
		SSLerr(SSL_F_GET_CLIENT_HELLO,SSL_R_BAD_SSL_SESSION_ID_LENGTH);
		return(-1);
		}

	if (s->state_session_id_length == 0)
		{
		if (!ssl_get_new_session(s,1))
			{
			ssl_return_error(s);
			return(-1);
			}
		}
	else
		{
		i=ssl_get_prev_session(s,s->state_session_id_length,
			&(p[s->state_cipher_spec_length]));
		if (i == 1)
			{ /* previous session */
			s->hit=1;
			}
		else if (i == -1)
			{
			ssl_return_error(s);
			return(-1);
			}
		else
			{
			if (s->cert == NULL)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_GET_CLIENT_HELLO,SSL_R_NO_CERTIFICATE_SET);
				return(-1);
				}

			if (!ssl_get_new_session(s,1))
				{
				ssl_return_error(s);
				return(-1);
				}
			}
		}

	if (!s->hit)
		{
		CIPHER cc,*cp,**cpp;
		CIPHER *pref[NUM_CIPHERS],*good[NUM_CIPHERS];
		int num_ciphers;
		char *str;

		cipher= &(s->session->ciphers[0]);

		/* First up, get a list of valid ciphers */
		num_ciphers=0;
		for (i=0; i<NUM_CIPHERS; i++)
			{
			if (ssl_ciphers[i].valid)
				pref[num_ciphers++]= &(ssl_ciphers[i]);
			good[i]=NULL;
			}

		/* sort by string name */
		qsort((char *)&(pref[0]),num_ciphers,sizeof(CIPHER *),
			(int (*)(P_CC_CC))ssl_cipher_pstrcmp);

		/* now for each preference, copy across */
		i=0;
		cp=&cc;
		for (;;)
			{
			str=SSL_get_cipher_list(s,i++);
			if (str == NULL) break;
			cc.name=str;
			cpp=(CIPHER **)OBJ_bsearch((char *)&cp,
				(char *)&(pref[0]),
				num_ciphers,sizeof(CIPHER *),
				(int (*)(P_CC_CC))ssl_cipher_pstrcmp);
			if (cpp != NULL) good[cpp- &(pref[0])]= *cpp;
			}
		/* have a 'holy' list of ciphers, pack it down */
		for (i=j=0; i<num_ciphers; i++)
			if (good[i] != NULL) pref[j++]=good[i];

		/* re-order it by cipher if */
		num_ciphers=j;
		qsort((char *)&(pref[0]),num_ciphers,sizeof(CIPHER *),
			(int (*)(P_CC_CC))ssl_cipher_ptr_cmp);

		/* pref now has 'num_ciphers' valid ciphers in it. */

		cp= &cc;
		for (j=i=0; i<(int)s->state_cipher_spec_length; i+=3)
			{
			cc.c1=p[i];
			cc.c2=p[i+1];
			cc.c3=p[i+2];

			cpp=(CIPHER **)OBJ_bsearch((char *)&cp,
				(char *)&(pref[0]),num_ciphers,sizeof(CIPHER *),
				(int (*)())ssl_cipher_ptr_cmp);

			/* If the cipher is valid, mark it as so */
			if ((cpp != NULL) && (*cpp)->valid)
				{
				*(cipher++)= *cpp;
				j++;
				}
			if (j == NUM_CIPHERS)
				{
				/* SOMETHING IS WRONG IF WE GET THIS IF THIS
				 * IS NOT THE LAST ENTRY */
				break;
				}
			}

		/* Sort them by the 3 cipher numers, this is how we
		 * look things up */
		s->session->num_ciphers=j;
		qsort((char *)&(s->session->ciphers[0]),j,sizeof(CIPHER *),
			(int (*)(P_CC_CC))ssl_cipher_ptr_cmp);

		}
	p+=s->state_cipher_spec_length;
	/* done cipher selection */

	/* session id extracted already */
	p+=s->state_session_id_length;

	/* challenge */
	s->challenge=(unsigned char *)malloc((unsigned int)s->challenge_length);
	if (s->challenge == NULL)
		{
		ssl_return_error(s);
		SSLerr(SSL_F_GET_CLIENT_HELLO,ERR_R_MALLOC_FAILURE);
		return(-1);
		}
	memcpy(s->challenge,p,(unsigned int)s->challenge_length);
	return(1);
	}

static int server_hello(s)
SSL *s;
	{
	unsigned char *p,*d;
	int n,hit,i;
	CIPHER **cipher;

	p=s->init_buf;
	if (s->state == SSL_ST_SEND_SERVER_HELLO_A)
		{
		d=p+11;
		*(p++)=SSL_MT_SERVER_HELLO;		/* type */
		hit=s->hit;
		*(p++)=(unsigned char)hit;
		if (!hit)
			{			/* else add cert to session */
			if (s->session->cert != NULL)
				ssl_cert_free(s->session->cert);
			s->session->cert=s->cert;		
			CS_BEGIN;
			s->cert->references++;
			CS_END;
			}

		if (s->session->cert == NULL)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_SERVER_HELLO,SSL_R_NO_CERTIFICATE_SPECIFIED);
			return(-1);
			}

		if (hit)
			{
			*(p++)=0;		/* no certificate type */
			s2n(s->version,p);	/* version */
			s2n(0,p);		/* cert len */
			s2n(0,p);		/* ciphers len */
			}
		else
			{
			*(p++)=s->cert->cert_type; /* put certificate type */
			s2n(s->version,p);	/* version */
			n=i2d_X509(s->cert->x509,NULL);
			s2n(n,p);		/* certificate length */
			i2d_X509(s->cert->x509,&d);
			n=0;
			
			/* lets send out the ciphers we like in the
			 * prefered order */
			cipher= &(s->session->ciphers[0]);
			for (i=0; i<s->session->num_ciphers; i++)
				{
				*(d++)=cipher[i]->c1;
				*(d++)=cipher[i]->c2;
				*(d++)=cipher[i]->c3;
				n+=3;
				}
			s2n(n,p);		/* add cipher length */
			}

		/* make and send conn_id */
		s2n(CONECTION_ID_LENGTH,p);	/* add conn_id length */
		s->conn_id=(unsigned char *)malloc(CONECTION_ID_LENGTH);
		if (s->conn_id == NULL)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_SERVER_HELLO,ERR_R_MALLOC_FAILURE);
			return(-1);
			}
		s->conn_id_length=CONECTION_ID_LENGTH;
		if (s->ctx->reverse)
			{
			if (s->ctx->conn_id_length != s->conn_id_length)
				{
				SSLerr(SSL_F_SERVER_HELLO,SSL_R_REVERSE_SSL_SESSION_ID_LENGTH_IS_WRONG);
				return(-1);
				}
			memcpy(s->conn_id,s->ctx->conn_id,
				(int)s->conn_id_length);
			}
		else
			RAND_bytes(s->conn_id,(int)s->conn_id_length);
		memcpy(d,s->conn_id,CONECTION_ID_LENGTH);
		d+=CONECTION_ID_LENGTH;

		s->state=SSL_ST_SEND_SERVER_HELLO_B;
		s->init_num=d-s->init_buf;
		s->init_off=0;
		}
	/* SSL_ST_SEND_SERVER_HELLO_B */
	return(ssl_do_write(s));
	}

static int get_client_finished(s)
SSL *s;
	{
	unsigned char *p;
	int i;

	p=s->init_buf;
	if (s->state == SSL_ST_GET_CLIENT_FINISHED_A)
		{
		i=SSL_read(s,(char *)&(p[s->init_num]),1-s->init_num);
		if (i < 1-s->init_num)
			return(ssl_part_read(s,SSL_F_GET_CLIENT_FINISHED,i));

		if (*p != SSL_MT_CLIENT_FINISHED)
			{
			if (*p != SSL_MT_ERROR)
				{
				ssl_return_error(s);
				SSLerr(SSL_F_GET_CLIENT_FINISHED,SSL_R_READ_WRONG_PACKET_TYPE);
				}
			else
				SSLerr(SSL_F_GET_CLIENT_FINISHED,SSL_R_PEER_ERROR);
			return(-1);
			}
		s->init_num=0;
		s->state=SSL_ST_GET_CLIENT_FINISHED_B;
		}

	/* SSL_ST_GET_CLIENT_FINISHED_B */
	i=SSL_read(s,(char *)&(p[s->init_num]),
		(unsigned int)s->conn_id_length-s->init_num);
	if (i < (int)s->conn_id_length-s->init_num)
		{
		return(ssl_part_read(s,SSL_F_GET_CLIENT_FINISHED,i));
#ifdef undef /* hmmm..... NBIO */
		ssl_return_error(s);
		SSLerr(SSL_F_GET_CLIENT_FINISHED,SSL_R_SHORT_READ);
		return(-1);
#endif
		}
	if (memcmp(p,s->conn_id,(unsigned int)s->conn_id_length) != 0)
		{
		ssl_return_error(s);
		SSLerr(SSL_F_GET_CLIENT_FINISHED,SSL_R_CONECTION_ID_IS_DIFFERENT);
		return(-1);
		}
	return(1);
	}

static int server_verify(s)
SSL *s;
	{
	unsigned char *p;

	if (s->state == SSL_ST_SEND_SERVER_VERIFY_A)
		{
		p=s->init_buf;
		*(p++)=SSL_MT_SERVER_VERIFY;
		memcpy(p,s->challenge,(unsigned int)s->challenge_length);
		/* p+=s->challenge_length; */

		s->state=SSL_ST_SEND_SERVER_VERIFY_B;
		s->init_num=s->challenge_length+1;
		s->init_off=0;
		}
	return(ssl_do_write(s));
	}

static int server_finish(s)
SSL *s;
	{
	unsigned char *p;

	if (s->state == SSL_ST_SEND_SERVER_FINISHED_A)
		{
		p=s->init_buf;
		*(p++)=SSL_MT_SERVER_FINISHED;

		memcpy(p,s->session->session_id,
			(unsigned int)s->session->session_id_length);
		/* p+=s->session->session_id_length; */

		s->state=SSL_ST_SEND_SERVER_FINISHED_B;
		s->init_num=s->session->session_id_length+1;
		s->init_off=0;
		}

	/* SSL_ST_SEND_SERVER_FINISHED_B */
	return(ssl_do_write(s));
	}

/* send the request and check the response */
static int request_certificate(s)
SSL *s;
	{
	unsigned char *p,*p2,*buf2;
	unsigned char *ccd;
	int i,j,ctype,ret=-1;
	X509 *x509=NULL;

	if (s->state_ccl == NULL)
		{
		s->state_ccl=(unsigned char *)malloc(MAX_CERT_CHALLENGE_LENGTH);
		if (s->state_ccl == NULL)
			{
			SSLerr(SSL_F_REQUEST_CERTIFICATE,ERR_R_MALLOC_FAILURE);
			goto end;
			}
		}
	ccd=s->state_ccl;
	if (s->state == SSL_ST_SEND_REQUEST_CERTIFICATE_A)
		{
		p=s->init_buf;
		*(p++)=SSL_MT_REQUEST_CERTIFICATE;
		*(p++)=SSL_AT_MD5_WITH_RSA_ENCRYPTION;
		RAND_bytes(ccd,MIN_CERT_CHALLENGE_LENGTH);
		memcpy(p,ccd,MIN_CERT_CHALLENGE_LENGTH);

		s->state=SSL_ST_SEND_REQUEST_CERTIFICATE_B;
		s->init_num=MIN_CERT_CHALLENGE_LENGTH+2;
		s->init_off=0;
		}

	if (s->state == SSL_ST_SEND_REQUEST_CERTIFICATE_B)
		{
		i=ssl_do_write(s);
		if (i <= 0)
			{
			ret=i;
			goto end;
			}

		s->init_num=0;
		s->state=SSL_ST_SEND_REQUEST_CERTIFICATE_C;
		}

	if (s->state == SSL_ST_SEND_REQUEST_CERTIFICATE_C)
		{
		p=s->init_buf;
		i=SSL_read(s,(char *)&(p[s->init_num]),6-s->init_num);
		if (i < 3)
			{
			ret=ssl_part_read(s,SSL_F_REQUEST_CERTIFICATE,i);
			goto end;
			}

		if ((*p == SSL_MT_ERROR) && (i >= 3))
			{
			n2s(p,i);
			if (s->verify_mode & SSL_VERIFY_FAIL_IF_NO_PEER_CERT)
				{
				SSLerr(SSL_F_REQUEST_CERTIFICATE,SSL_R_PEER_DID_NOT_RETURN_A_CERTIFICATE);
				goto end;
				}
			ret=1;
			goto end;
			}
		if ((*(p++) != SSL_MT_CLIENT_CERTIFICATE) || (i < 6))
			{
			ssl_return_error(s);
			SSLerr(SSL_F_REQUEST_CERTIFICATE,SSL_R_SHORT_READ);
			goto end;
			}
		/* ok we have a response */
		/* certificate type, there is only one right now. */
		ctype= *(p++);
		if (ctype != SSL_AT_MD5_WITH_RSA_ENCRYPTION)
			{
			ssl_return_error(s);
			SSLerr(SSL_F_REQUEST_CERTIFICATE,SSL_R_BAD_RESPONSE_ARGUMENT);
			goto end;
			}
		n2s(p,i); s->state_clen=i;
		n2s(p,i); s->state_rlen=i;
		s->state=SSL_ST_SEND_REQUEST_CERTIFICATE_D;
		s->init_num=0;
		}

	/* SSL_ST_SEND_REQUEST_CERTIFICATE_D */
	p=s->init_buf;
	j=s->state_clen+s->state_rlen-s->init_num;
	i=SSL_read(s,(char *)&(p[s->init_num]),(unsigned int)j);
	if (i < j) 
		{
		ret=ssl_part_read(s,SSL_F_REQUEST_CERTIFICATE,i);
		goto end;
		}

	x509=(X509 *)d2i_X509(NULL,&p,(long)s->state_clen);
	if (x509 == NULL)
		{
		SSLerr(SSL_F_REQUEST_CERTIFICATE,ERR_R_X509_LIB);
		goto end;
		}
	i=X509_cert_verify(s->ctx->cert,x509,s->verify_callback);

	if (i)	/* we like the packet, now check the chksum */
		{
		EVP_MD_CTX ctx;
		EVP_PKEY *pkey=NULL;

		EVP_VerifyInit(&ctx,EVP_md5());
		EVP_VerifyUpdate(&ctx,s->key_material,
			(unsigned int)s->key_material_length);
		EVP_VerifyUpdate(&ctx,ccd,MIN_CERT_CHALLENGE_LENGTH);

		i=i2d_X509(s->session->cert->x509,NULL);
		buf2=(unsigned char *)malloc((unsigned int)i);
		if (buf2 == NULL)
			{
			SSLerr(SSL_F_REQUEST_CERTIFICATE,ERR_R_MALLOC_FAILURE);
			goto end;
			}
		p2=buf2;
		i=i2d_X509(s->session->cert->x509,&p2);
		EVP_VerifyUpdate(&ctx,buf2,(unsigned int)i);
		free(buf2);

		pkey=X509_extract_key(x509);
		if (pkey == NULL) goto end;
		i=EVP_VerifyFinal(&ctx,p,s->state_rlen,pkey);
		memset(&ctx,0,sizeof(ctx));
		EVP_PKEY_free(pkey);

		if (i) 
			{
			if (s->session->peer != NULL)
				X509_free(s->session->peer);
			s->session->peer=x509;
			CS_BEGIN
			x509->references++;
			CS_END
			ret=1;
			goto end;
			}
		else
			SSLerr(SSL_F_REQUEST_CERTIFICATE,SSL_R_BAD_CHECKSUM);
		}
end:
	if (x509 != NULL) X509_free(x509);
	return(ret);
	}

static int ssl_cipher_pstrcmp(ap,bp)
CIPHER **ap,**bp;
	{
	return(strcmp((*ap)->name,(*bp)->name));
	}
