/* ssl/ssl_sess.c */
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
#include "lhash.h"
#include "rand.h"
#include "ssl_locl.h"

SSL_SESSION *SSL_SESSION_new()
	{
	SSL_SESSION *ss;
	int i;

	ss=(SSL_SESSION *)malloc(sizeof(SSL_SESSION));
	if (ss == NULL)
		{
		SSLerr(SSL_F_SSL_SESSION_NEW,ERR_R_MALLOC_FAILURE);
		return(0);
		}
	ss->num_ciphers=0;
	for (i=0; i<NUM_CIPHERS; i++) ss->ciphers[i]=NULL;
	ss->cipher=NULL;
	ss->key_arg_length=0;
	ss->key_arg=NULL;
	ss->master_key_length=0;
	ss->master_key=NULL;
	ss->session_id_length=0;
	ss->session_id=NULL;

	ss->cert=NULL;
	ss->peer=NULL;
	ss->references=1;
	ss->timeout=60*5; /* minute timeout by default */
	ss->time=time(NULL);
	return(ss);
	}

int ssl_get_new_session(s, session)
SSL *s;
int session;
	{
	SSL_SESSION *ss=NULL;

	if ((ss=SSL_SESSION_new()) == NULL) return(0);

	if (s->session != NULL)
		{
		SSL_SESSION_free(s->session);
		s->session=NULL;
		}

	if (session)
		{
		ss->session_id_length=SSL_SESSION_ID_LENGTH;
		ss->session_id=(unsigned char *)malloc(SSL_SESSION_ID_LENGTH);
		if (ss->session_id == NULL)
			{
			SSLerr(SSL_F_SSL_GET_NEW_SESSION,ERR_R_MALLOC_FAILURE);
			SSL_SESSION_free(ss);
			return(0);
			}

		for (;;)
			{
			SSL_SESSION *r;

			RAND_bytes(ss->session_id,SSL_SESSION_ID_LENGTH);
			r=(SSL_SESSION *)lh_retrieve(s->ctx->sessions,
				(char *)ss);
			if (r == NULL) break;
			/* else - woops a session_id match */
			}
		}
	else
		{
		ss->session_id_length=0;
		}

	s->session=ss;
	return(1);
	}

int ssl_get_prev_session(s, len, session)
SSL *s;
int len;
unsigned char *session;
	{
	SSL_SESSION *ret,data;

	/* conn_init();*/
	data.session_id_length=len;
	data.session_id=session;
	if ((ret=(SSL_SESSION *)lh_retrieve(s->ctx->sessions,(char *)&data))
		== NULL)
		{
		s->ctx->sess_miss++;
		ret=NULL;
		if ((s->ctx->get_session_cb != NULL) &&
			((ret=s->ctx->get_session_cb(session,len)) != NULL))
			{
			s->ctx->sess_cb_hit++;
			/* The following should not return 1, otherwise,
			 * things are very strange */
			SSL_add_session(s->ctx,ret);
			}
		if (ret == NULL) return(0);
		}
#ifdef SSL_DEBUG
	SSL_TRACE(SSL_ERR,"GOT ONE\n");
#endif

	/* If a thread got the session, then 'swaped', and another got
	 * it and then due to a time-out decided to 'free' it we could
	 * be in trouble.  So I'll increment it now, then double decrement
	 * later - am I speaking rubbish?. */
	CS_BEGIN;
	ret->references++;
	CS_END;

	if ((long)(ret->time+ret->timeout) < (long)time(NULL)) /* timeout */
		{
#ifdef SSL_DEBUG
		SSL_TRACE(SSL_ERR,"TIMEOUT ON SSL_SESSION\n");
#endif
		s->ctx->sess_timeout++;
		SSL_remove_session(s->ctx,ret); /* remove it from the cache */
		SSL_SESSION_free(ret);		/* again to actually free it */
		return(0);
		}
	s->ctx->sess_hit++;

	/* ret->time=time(NULL); */ /* rezero timeout? */
	/* again, just leave the session 
	 * if it is the same session, we have just incremented and
	 * then decremented the reference count :-) */
	if (s->session != NULL)
		SSL_SESSION_free(s->session);
	s->session=ret;
	return(1);
	}

int SSL_add_session(ctx,c)
SSL_CTX *ctx;
SSL_SESSION *c;
	{
	SSL_SESSION *s;

	/* conn_init(); */
	CS_BEGIN;
	c->references++;
	CS_END;
	s=(SSL_SESSION *)lh_insert(ctx->sessions,(char *)c);
	/* If the same session if is being 're-added', free the old
	 * one when the last person stops using it.
	 * This will also work if it is alread in the cache.
	 * The references will go up and then down :-) */
	if (s != NULL)
		{
		SSL_SESSION_free(s);
		return(0);
		}
	else
		return(1);
	}

void SSL_remove_session(ctx,c)
SSL_CTX *ctx;
SSL_SESSION *c;
	{
	SSL_SESSION *r;

	if ((c->session_id != NULL) && (c != NULL))
		{
		r=(SSL_SESSION *)lh_delete(ctx->sessions,(char *)c);
		if (r == NULL)
			{
#ifdef SSL_DEBUG
			SSL_TRACE(SSL_ERR,"freeing a session that is not hashed\n");
#endif
			return;
			}
		else
			SSL_SESSION_free(r);
		}
	}

void SSL_SESSION_free(ss)
SSL_SESSION *ss;
	{
	CS_BEGIN;
	if (--ss->references > 0) return;
	CS_END;
	if (ss->key_arg != NULL)
		{
		memset(ss->key_arg,0,ss->key_arg_length);
		free(ss->key_arg);
		}
	if (ss->master_key != NULL)
		{
		memset(ss->master_key,0,ss->master_key_length);
		free(ss->master_key);
		}
	if (ss->session_id != NULL)
		{
		memset(ss->session_id,0,ss->session_id_length);
		free(ss->session_id);
		}
	if (ss->cert != NULL) ssl_cert_free(ss->cert);
	if (ss->peer != NULL) X509_free(ss->peer);
	free(ss);
	}

int SSL_set_session(s, session)
SSL *s;
SSL_SESSION *session;
	{
	int ret=0;

	CS_BEGIN;
	if (session != NULL)
		{
		if (s->session != NULL)
			SSL_SESSION_free(s->session);
		CS_BEGIN
		session->references++;
		CS_END
		s->session=session;
		ret=1;
		}
	CS_END;
	return(ret);
	}

long SSL_set_timeout(s,t)
SSL_SESSION *s;
long t;
	{
	if (s == NULL) return(0);
	s->timeout=t;
	return(1);
	}

long SSL_get_timeout(s)
SSL_SESSION *s;
	{
	if (s == NULL) return(0);
	return(s->timeout);
	}

long SSL_get_time(s)
SSL_SESSION *s;
	{
	if (s == NULL) return(0);
	return(s->time);
	}

long SSL_set_time(s,t)
SSL_SESSION *s;
long t;
	{
	if (s == NULL) return(0);
	s->time=t;
	return(t);
	}

typedef struct timeout_param_st
	{
	unsigned long time;
	LHASH *cache;
	} TIMEOUT_PARAM;

static void timeout(s,p)
SSL_SESSION *s;
TIMEOUT_PARAM *p;
	{
	if ((p->time == 0) || (p->time < (s->time+s->timeout))) /* timeout */
		{
		lh_delete(p->cache,(char *)s);
		SSL_SESSION_free(s);
		}
	}

void SSL_flush_sessions(s,t)
SSL_CTX *s;
long t;
	{
	unsigned long i;
	TIMEOUT_PARAM tp;

	tp.cache=SSL_CTX_sessions(s);
	if (tp.cache == NULL) return;
	tp.time=t;
	i=tp.cache->down_load;
	tp.cache->down_load=0;
	lh_doall_arg(tp.cache,(void (*)())timeout,(char *)&tp);
	tp.cache->down_load=i;
	}

