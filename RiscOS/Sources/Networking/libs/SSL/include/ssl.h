/* ssl/ssl.h */
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

#ifndef HEADER_SSL_H
#define HEADER_SSL_H

#ifdef  __cplusplus
extern "C" {
#endif

/* Protocol Version Codes */
#define SSL_CLIENT_VERSION	0x0002
#define SSL_SERVER_VERSION	0x0002

/* SSLeay version number for ASN.1 encoding of the session information */
#define SSL_SESSION_ASN1_VERSION 0x0000

/* Protocol Message Codes */
#define SSL_MT_ERROR			0
#define SSL_MT_CLIENT_HELLO		1
#define SSL_MT_CLIENT_MASTER_KEY	2
#define SSL_MT_CLIENT_FINISHED		3
#define SSL_MT_SERVER_HELLO		4
#define SSL_MT_SERVER_VERIFY		5
#define SSL_MT_SERVER_FINISHED		6
#define SSL_MT_REQUEST_CERTIFICATE	7
#define SSL_MT_CLIENT_CERTIFICATE	8

/* Error Message Codes */
#define SSL_PE_NO_CIPHER		0x0001
#define SSL_PE_NO_CERTIFICATE		0x0002
#define SSL_PE_BAD_CERTIFICATE		0x0004
#define SSL_PE_UNSUPPORTED_CERTIFICATE_TYPE 0x0006

/* Cipher Kind Values */
#define SSL_CK_NULL_WITH_MD5			0x00,0x00,0x00 /* v3 */
#define SSL_CK_RC4_128_WITH_MD5			0x01,0x00,0x80
#define SSL_CK_RC4_128_EXPORT40_WITH_MD5	0x02,0x00,0x80
#define SSL_CK_RC2_128_CBC_WITH_MD5		0x03,0x00,0x80
#define SSL_CK_RC2_128_CBC_EXPORT40_WITH_MD5	0x04,0x00,0x80
#define SSL_CK_IDEA_128_CBC_WITH_MD5		0x05,0x00,0x80
#define SSL_CK_DES_64_CBC_WITH_MD5		0x06,0x00,0x40
#define SSL_CK_DES_64_CBC_WITH_SHA		0x06,0x01,0x40 /* v3 */
#define SSL_CK_DES_192_EDE3_CBC_WITH_MD5	0x07,0x00,0xc0
#define SSL_CK_DES_192_EDE3_CBC_WITH_SHA	0x07,0x01,0xc0 /* v3 */

#define SSL_CK_DES_64_CFB64_WITH_MD5_1		0xff,0x08,0x00 /* SSLeay */
#define SSL_CK_NULL				0xff,0x08,0x10 /* SSLeay */

/* text strings for the ciphers */
#define SSL_TXT_NULL_WITH_MD5			"NULL-MD5"
#define SSL_TXT_RC4_128_WITH_MD5		"RC4-MD5"
#define SSL_TXT_RC4_128_EXPORT40_WITH_MD5	"EXP-RC4-MD5"
#define SSL_TXT_RC2_128_CBC_WITH_MD5		"RC2-CBC-MD5"
#define SSL_TXT_RC2_128_CBC_EXPORT40_WITH_MD5	"EXP-RC2-CBC-MD5"
#define SSL_TXT_IDEA_128_CBC_WITH_MD5		"IDEA-CBC-MD5"
#define SSL_TXT_DES_64_CBC_WITH_MD5		"DES-CBC-MD5"
#define SSL_TXT_DES_64_CBC_WITH_SHA		"DES-CBC-SHA"
#define SSL_TXT_DES_192_EDE3_CBC_WITH_MD5	"DES-CBC3-MD5"
#define SSL_TXT_DES_192_EDE3_CBC_WITH_SHA	"DES-CBC3-SHA"

#define SSL_TXT_DES_64_CFB64_WITH_MD5_1		"DES-CFB-M1"
#define SSL_TXT_NULL				"NULL"

/* Certificate Type Codes */
#define SSL_CT_X509_CERTIFICATE			0x01

/* Authentication Type Code */
#define SSL_AT_MD5_WITH_RSA_ENCRYPTION		0x01

/* Upper/Lower Bounds */
#define SSL_MAX_MASTER_KEY_LENGTH_IN_BITS	256
#define SSL_MAX_SSL_SESSION_ID_LENGTH_IN_BYTES	16
#define SSL_MIN_RSA_MODULUS_LENGTH_IN_BYTES	64
#define SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER	(unsigned int)32767
#define SSL_MAX_RECORD_LENGTH_3_BYTE_HEADER	16383 /**/

#ifndef HEADER_X509_H
#include "x509.h"
#endif

#ifndef HEADER_SSL_LOCL_H
#define  SSL_SESSION	char
#define  CERT		char
#endif

#ifdef HEADER_X509_H
#define SSL_FILETYPE_PEM	X509_FILETYPE_PEM
#define SSL_FILETYPE_ASN1	X509_FILETYPE_ASN1
#else
#define SSL_FILETYPE_PEM	1
#define SSL_FILETYPE_ASN1	2
#endif

typedef struct ssl_ctx_st
	{
	int num_cipher_list;
	char **cipher_list;

	/* This should be called something other than 'cert' to stop
	 * me mixing it up with SSL->cert.  Tim won't let me because
	 * is is externally visable now :-(. */
#ifdef HEADER_X509_H
	CERTIFICATE_CTX *cert;
#else
	char *cert;
#endif
#ifdef HEADER_LHASH_H
	LHASH *sessions;	/* a set of SSL_SESSION's */
#else
	char *sessions;
#endif

	/* If this callback is not null, it will be called each
	 * time a session id is added to the cache.  If this function
	 * returns 1, it means that the callback will do a
	 * SSL_SESSION_free() when it has finished using it.  Otherwise,
	 * on 0, it means the callback has finished with it. */
#ifndef NOPROTO
	int (*new_session_cb)(SSL_SESSION *new);
#else
	int (*new_session_cb)();
#endif

#ifndef NOPROTO
	SSL_SESSION *(*get_session_cb)(unsigned char *key,int key_len);
#else
	SSL_SESSION *(*get_session_cb)();
#endif

	long sess_connect;	/* SSL new (expensive) connection */
	long sess_connect_good;	/* SSL new (expensive) connection */
	long sess_accept;	/* SSL connection failues */
	long sess_accept_good;	/* SSL connection failues */
	long sess_miss;		/* session lookup misses  */
	long sess_timeout;	/* session reuse attempt on timeouted session */
	long sess_hit;		/* session reuse actually done */
	long sess_cb_hit;	/* session-id that was not in the cache was
				 * passed back via the callback.  This
				 * indicates that the application is supplying
				 * session-id's from other processes -
				 * spooky :-) */

	int references;

	/* used to do 'reverse' ssl */
	int reverse;

	/* used by client (for 'reverse' ssl) */
	char *cipher;
	unsigned int challenge_length;
	unsigned char *challenge;
	unsigned int master_key_length;
	unsigned char *master_key;
	unsigned int key_arg_length;
	unsigned char *key_arg;
	/* used by server (for 'reverse' ssl) */
	unsigned int conn_id_length;
	unsigned char *conn_id;
	void (*info_callback)();
	} SSL_CTX;


#define SSL_CTX_sessions(ctx)		((ctx)->sessions)
/* You will need to include lhash.h to access the following #define */
#define SSL_CTX_sess_number(ctx)	((ctx)->sessions->num_items)
#define SSL_CTX_sess_connect(ctx)	((ctx)->sess_connect)
#define SSL_CTX_sess_connect_good(ctx)	((ctx)->sess_connect_good)
#define SSL_CTX_sess_accept(ctx)	((ctx)->sess_accept)
#define SSL_CTX_sess_accept_good(ctx)	((ctx)->sess_accept_good)
#define SSL_CTX_sess_hits(ctx)		((ctx)->sess_hit)
#define SSL_CTX_sess_cb_hits(ctx)	((ctx)->sess_cb_hit)
#define SSL_CTX_sess_misses(ctx)	((ctx)->sess_miss)
#define SSL_CTX_sess_timeouts(ctx)	((ctx)->sess_timeout)

#define SSL_CTX_sess_set_new_cb(ctx,cb)	((ctx)->new_session_cb=(cb))
#define SSL_CTX_sess_get_new_cb(ctx)	((ctx)->new_session_cb)
#define SSL_CTX_sess_set_get_cb(ctx,cb)	((ctx)->get_session_cb=(cb))
#define SSL_CTX_sess_get_get_cb(ctx)	((ctx)->get_session_cb)

#define SSL_CTX_set_info_callback(ctx,cb)	((ctx)->info_callback=(cb))
#define SSL_CTX_get_info_callback(ctx)		((ctx)->info_callback)

#define SSL_NOTHING	1
#define SSL_WRITING	2
#define SSL_READING	3
#define SSL_X509_LOOKUP	4

/* These will only be used when doing non-blocking IO */
#define SSL_want(s)		((s)->rwstate)
#define SSL_want_nothing(s)	((s)->rwstate == SSL_NOTHING)
#define SSL_want_read(s)	((s)->rwstate == SSL_READING)
#define SSL_want_write(s)	((s)->rwstate == SSL_WRITING)
#define SSL_want_x509_lookup(s)	((s)->rwstate == SSL_X509_LOOKUP)

typedef struct ssl_st
	{
	/* There are 2 BIO's even though they are normally both the
	 * same.  This is so data can be read and written to different
	 * handlers */

#ifdef HEADER_BUFFER_H
	BIO *rbio; /* used by SSL_read */
	BIO *wbio; /* used by SSL_write */
#else
	char *rbio; /* used by SSL_read */
	char *wbio; /* used by SSL_write */
#endif
	int version;	/* procol version */

	/* This holds a variable that indicates what we were doing
	 * when a 0 or -1 is returned.  This is needed for
	 * non-blocking IO so we know what request needs re-doing when
	 * in SSL_accept or SSL_connect */
	int rwstate;

	int state;	/* where we are */
	unsigned int state_conn_id_length;	/* tmp */
	unsigned int state_cert_type;		/* tmp */
	unsigned int state_cert_length;		/* tmp */
	unsigned int state_csl; 		/* tmp */

	unsigned int state_clear; 		/* tmp */
	unsigned int state_enc; 		/* tmp */

	unsigned char *state_ccl;		/* tmp */

	unsigned int state_cipher_spec_length;
	unsigned int state_session_id_length;

	unsigned int state_clen;
	unsigned int state_rlen;

	int rstate;	/* where we are when reading */

	unsigned char *init_buf;/* buffer used during init */
	int init_num;	/* amount read/written */
	int init_off;	/* amount read/written */

	int read_ahead;
	int escape;
	int three_byte_header;
	int send;		/* direction of packet */
	int clear_text;		/* clear text */
	int hit;		/* reusing a previous session */

	/* non-blocking io info, used to make sure the same args were passwd */
	unsigned int wnum;	/* number of bytes sent so far */
	int wpend_tot;
	char *wpend_buf;

	int wpend_off;		/* offset to data to write */
	int wpend_len;  	/* number of bytes passwd to write */
	int wpend_ret;  	/* number of bytes to return to caller */

	int rpend_off;		/* offset to read position */
	int rpend_len;		/* number of bytes to read */

	/* buffer raw data */
	int rbuf_left;
	int rbuf_offs;
	unsigned char *rbuf;
	unsigned char *wbuf;

	/* used internally by ssl_read to talk to read_n */
	unsigned char *packet;
	unsigned int packet_length;

	/* set by read_n */
	unsigned int length;
	unsigned int padding;
	unsigned int ract_data_length; /* Set when things are encrypted. */
	unsigned int wact_data_length; /* Set when things are decrypted. */
	unsigned char *ract_data;
	unsigned char *wact_data;
	unsigned char *mac_data;
	unsigned char *pad_data;

	/* crypto */
	int num_cipher_list;
	char **cipher_list;	/* char ** is malloced and the rest is
				 * on malloced block pointed to by
				 * cipher_list[0] */
	char *crypt_state;	/* cryptographic state */
	unsigned char *read_key;
	unsigned char *write_key;

	/* session info */

	CERT *cert;
	/* This can also be in the session once a session is established */
	SSL_SESSION *session;

	unsigned int challenge_length;
	unsigned char *challenge;
	unsigned int conn_id_length;
	unsigned char *conn_id;
	unsigned int key_material_length;
	unsigned char *key_material;

	/* packet specs */
	/* send is true for send; false for recieve */

	unsigned char *write_ptr;	/* used to point to the start due to
					 * 2/3 byte header. */

	unsigned long read_sequence;
	unsigned long write_sequence;

	/* special stuff */
	int trust_level;	/* not used yet */

	/* The following has been moved into the SSL_SESSION */
	/* X509 *peer; */

	int verify_mode;	/* 0 don't care about verify failure.
				 * 1 fail if verify fails */
	int (*verify_callback)(); /* fail if callback returns 0 */
	void (*info_callback)(); /* optional informational callback */

	SSL_CTX *ctx;
	/* set this flag to 1 and a sleep(1) is put into all SSL_read()
	 * and SSL_write() calls, good for nbio debuging :-) */
	int debug;
	} SSL;

/* The following are the possible values for ssl->state are are
 * used to indicate where we are upto in the SSL conection establishment.
 * The macros that follow are about the only things you should need to use
 * and even then, only when using non-blocking IO.
 * It can also be useful to work out where you were when the connection
 * failed */

#define SSL_state(a)			((a)->state)
#define SSL_ST_CONNECT			0x0100
#define SSL_ST_ACCEPT			0x0200
#define SSL_ST_INIT			(SSL_ST_CONNECT|SSL_ST_ACCEPT)
#define SSL_ST_BEFORE			0x01
#define SSL_ST_OK			0x03

/* SSL info callback functions */
#define SSL_set_info_callback(ssl,cb)	((ssl)->info_callback=(cb))
#define SSL_get_info_callback(ssl)	((ssl)->info_callback)

#define SSL_CB_ACCEPT_LOOP		(SSL_ST_ACCEPT|0x01)
#define SSL_CB_ACCEPT_EXIT		(SSL_ST_ACCEPT|0x02)
#define SSL_CB_CONNECT_LOOP		(SSL_ST_CONNECT|0x01)
#define SSL_CB_CONNECT_EXIT		(SSL_ST_CONNECT|0x02)

/* Define the initial state
 * These are used as flags to be checked by SSL_in_connect_init() and
 * SSL_in_accept_init() */
#define SSL_set_connect_state(a)	((a)->state=SSL_ST_CONNECT)
#define SSL_set_accept_state(a)		((a)->state=SSL_ST_ACCEPT)

/* Is the SSL_connection established? */
#define SSL_is_init_finished(a)		((a)->state == SSL_ST_OK)
#define SSL_in_init(a)			((a)->state&SSL_ST_INIT)
#define SSL_in_connect_init(a)		((a)->state&SSL_ST_CONNECT)
#define SSL_in_accept_init(a)		((a)->state&SSL_ST_ACCEPT)

/* client */
#define SSL_ST_SEND_CLIENT_HELLO_A		(0x10|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_HELLO_B		(0x11|SSL_ST_CONNECT)
#define SSL_ST_GET_SERVER_HELLO_A		(0x20|SSL_ST_CONNECT)
#define SSL_ST_GET_SERVER_HELLO_B		(0x21|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_MASTER_KEY_A		(0x30|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_MASTER_KEY_B		(0x31|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_FINISHED_A		(0x40|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_FINISHED_B		(0x41|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_CERTIFICATE_A	(0x50|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_CERTIFICATE_B	(0x51|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_CERTIFICATE_C	(0x52|SSL_ST_CONNECT)
#define SSL_ST_SEND_CLIENT_CERTIFICATE_D	(0x53|SSL_ST_CONNECT)
#define SSL_ST_GET_SERVER_VERIFY_A		(0x60|SSL_ST_CONNECT)
#define SSL_ST_GET_SERVER_VERIFY_B		(0x61|SSL_ST_CONNECT)
#define SSL_ST_GET_SERVER_FINISHED_A		(0x70|SSL_ST_CONNECT)
#define SSL_ST_GET_SERVER_FINISHED_B		(0x71|SSL_ST_CONNECT)
#define SSL_ST_CLIENT_START_ENCRYPTION		(0x80|SSL_ST_CONNECT)
#define SSL_ST_X509_GET_CLIENT_CERTIFICATE	(0x90|SSL_ST_CONNECT)
/* server */
#define SSL_ST_GET_CLIENT_HELLO_A		(0x10|SSL_ST_ACCEPT)
#define SSL_ST_GET_CLIENT_HELLO_B		(0x11|SSL_ST_ACCEPT)
#define SSL_ST_SEND_SERVER_HELLO_A		(0x20|SSL_ST_ACCEPT)
#define SSL_ST_SEND_SERVER_HELLO_B		(0x21|SSL_ST_ACCEPT)
#define SSL_ST_GET_CLIENT_MASTER_KEY_A		(0x30|SSL_ST_ACCEPT)
#define SSL_ST_GET_CLIENT_MASTER_KEY_B		(0x31|SSL_ST_ACCEPT)
#define SSL_ST_SEND_SERVER_VERIFY_A		(0x40|SSL_ST_ACCEPT)
#define SSL_ST_SEND_SERVER_VERIFY_B		(0x41|SSL_ST_ACCEPT)
#define SSL_ST_GET_CLIENT_FINISHED_A		(0x50|SSL_ST_ACCEPT)
#define SSL_ST_GET_CLIENT_FINISHED_B		(0x51|SSL_ST_ACCEPT)
#define SSL_ST_SEND_SERVER_FINISHED_A		(0x60|SSL_ST_ACCEPT)
#define SSL_ST_SEND_SERVER_FINISHED_B		(0x61|SSL_ST_ACCEPT)
#define SSL_ST_SEND_REQUEST_CERTIFICATE_A	(0x70|SSL_ST_ACCEPT)
#define SSL_ST_SEND_REQUEST_CERTIFICATE_B	(0x71|SSL_ST_ACCEPT)
#define SSL_ST_SEND_REQUEST_CERTIFICATE_C	(0x72|SSL_ST_ACCEPT)
#define SSL_ST_SEND_REQUEST_CERTIFICATE_D	(0x73|SSL_ST_ACCEPT)
#define SSL_ST_SERVER_START_ENCRYPTION		(0x80|SSL_ST_ACCEPT)
#define SSL_ST_X509_GET_SERVER_CERTIFICATE	(0x90|SSL_ST_ACCEPT)

/* The following 2 states are kept in ssl->rstate when reads fail,
 * you should not need these */
#define SSL_ST_READ_HEADER			0xF0
#define SSL_ST_READ_BODY			0xF1

#define SSL_VERIFY_NONE			0x00
#define SSL_VERIFY_PEER			0x01
#define SSL_VERIFY_FAIL_IF_NO_PEER_CERT	0x02
#define SSL_VERIFY_CLIENT_ONCE		0x04

#define SSL_RWERR_BAD_WRITE_RETRY	(-2)
#define SSL_RWERR_BAD_MAC_DECODE	(-3)
#define SSL_RWERR_INTERNAL_ERROR	(-4) /* should not get this one */

#define SSL_set_default_verify_paths(ctx) \
		X509_set_default_verify_paths((ctx)->cert)
#define SSL_load_verify_locations(ctx,CAfile,CApath) \
		X509_load_verify_locations((ctx)->cert,(CAfile),(CApath))

#define SSL_get_session(s)	((s)->session)
#define SSL_get_certificate(s)	((s)->session->cert)
#define SSL_get_SSL_CTX(s)	((s)->ctx)

/* this is for backward compatablility */
#define SSL_get_pref_cipher(s,n)	SSL_get_cipher_list(s,n)
#define SSL_set_pref_cipher(c,n)	SSL_set_cipher_list(c,n)

#ifndef NOPROTO
#ifdef HEADER_BUFFER_H
BIO_METHOD *BIO_f_ssl(void);
#endif
int	SSL_CTX_set_cipher_list(SSL_CTX *,char *str);
SSL_CTX *SSL_CTX_new(void );
void	SSL_CTX_free(SSL_CTX *);
int	SSL_accept(SSL *s);
void	SSL_clear(SSL *s);
int	SSL_connect(SSL *s);
void	SSL_debug(char *file);
void	SSL_flush_sessions(SSL_CTX *ctx,long tm);
void	SSL_free(SSL *s);
char *	SSL_get_cipher(SSL *s);
int	SSL_get_fd(SSL *s);
char *	SSL_get_cipher_list(SSL *s, int n);
char *	SSL_get_shared_ciphers(SSL *s, char *buf, int len);
int	SSL_get_read_ahead(SSL * s);
SSL *	SSL_new(SSL_CTX *);
int	SSL_pending(SSL *s);
int	SSL_read(SSL *s, char *buf, unsigned int len);
int	SSL_set_fd(SSL *s, int fd);
int	SSL_set_rfd(SSL *s, int fd);
int	SSL_set_wfd(SSL *s, int fd);
#ifdef HEADER_BUFFER_H
void	SSL_set_bio(SSL *s, BIO *rbio,BIO *wbio);
BIO *	SSL_get_rbio(SSL *s);
BIO *	SSL_get_wbio(SSL *s);
#else
void	SSL_set_bio(SSL *s, char *rbio,char *wbio);
char *	SSL_get_rbio(SSL *s);
char *	SSL_get_wbio(SSL *s);
#endif
int	SSL_set_cipher_list(SSL *s, char *str);
void	SSL_set_read_ahead(SSL * s, int yes);
void	SSL_set_verify(SSL *s, int mode, int (*callback) ());
int	SSL_use_RSAPrivateKey(SSL *ssl, RSA *rsa);
int	SSL_use_RSAPrivateKey_ASN1(SSL *ssl, unsigned char *d, long len);
int	SSL_use_RSAPrivateKey_file(SSL *ssl, char *file, int type);
int	SSL_use_PrivateKey(SSL *ssl, EVP_PKEY *pkey);
int	SSL_use_PrivateKey_ASN1(int pk,SSL *ssl, unsigned char *d, long len);
int	SSL_use_PrivateKey_file(SSL *ssl, char *file, int type);
int	SSL_use_certificate(SSL *ssl, X509 *x);
int	SSL_use_certificate_ASN1(SSL *ssl, int len, unsigned char *d);
int	SSL_use_certificate_file(SSL *ssl, char *file, int type);
int	SSL_write(SSL *s, const char *buf, unsigned int len);
void	ERR_load_SSL_strings(void );
void	SSL_load_error_strings(void );
char * 	SSL_state_string(SSL *s);
char * 	SSL_rstate_string(SSL *s);
char * 	SSL_state_string_long(SSL *s);
char * 	SSL_rstate_string_long(SSL *s);
long	SSL_get_time(SSL_SESSION *s);
long	SSL_set_time(SSL_SESSION *s, long t);
long	SSL_get_timeout(SSL_SESSION *s);
long	SSL_set_timeout(SSL_SESSION *s, long t);
void	SSL_copy_session_id(SSL *to,SSL *from);

int     SSL_errno( void );

SSL_SESSION *SSL_SESSION_new(void);
#ifndef WIN16
int	SSL_SESSION_print_fp(FILE *fp,SSL_SESSION *ses);
#endif
#ifdef HEADER_BUFFER_H
int	SSL_SESSION_print(BIO *fp,SSL_SESSION *ses);
#else
int	SSL_SESSION_print(char *fp,SSL_SESSION *ses);
#endif
void	SSL_SESSION_free(SSL_SESSION *ses);
int	i2d_SSL_SESSION(SSL_SESSION *in,unsigned char **pp);
int	SSL_set_session(SSL *to, SSL_SESSION *session);
int	SSL_add_session(SSL_CTX *s, SSL_SESSION *c);
void	SSL_remove_session(SSL_CTX *,SSL_SESSION *c);
SSL_SESSION *d2i_SSL_SESSION(SSL_SESSION **a,unsigned char **pp,long length);

#ifdef HEADER_X509_H
X509 *	SSL_get_peer_certificate(SSL *s);
#else
char *	SSL_get_peer_certificate(SSL *s);
#endif

#else

#ifdef HEADER_BUFFER_H
BIO_METHOD *BIO_f_ssl();
#endif

int	SSL_CTX_set_cipher_list();
SSL_CTX *SSL_CTX_new();
void	SSL_CTX_free();
int	SSL_accept();
void	SSL_clear();
int	SSL_connect();
void	SSL_debug();
void	SSL_flush_sessions();
void	SSL_free();
char *	SSL_get_cipher();
int	SSL_get_fd();
char *	SSL_get_cipher_list();
int	SSL_get_read_ahead();
SSL *	SSL_new();
int	SSL_pending();
int	SSL_read();
int	SSL_set_fd();
void	SSL_set_bio();
#ifdef HEADER_BUFFER_H
BIO *	SSL_get_rbio();
BIO *	SSL_get_wbio();
#else
char *	SSL_get_rbio();
char *	SSL_get_wbio();
#endif
int	SSL_set_cipher_list();
char *	SSL_get_shared_ciphers();
void	SSL_set_read_ahead();
void	SSL_set_verify();
int	SSL_use_RSAPrivateKey();
int	SSL_use_RSAPrivateKey_ASN1();
int	SSL_use_RSAPrivateKey_file();
int	SSL_use_PrivateKey();
int	SSL_use_PrivateKey_ASN1();
int	SSL_use_PrivateKey_file();
int	SSL_use_certificate();
int	SSL_use_certificate_ASN1();
int	SSL_use_certificate_file();
int	SSL_write();
void	ERR_load_SSL_strings();
void	SSL_load_error_strings();
char * 	SSL_state_string();
char * 	SSL_rstate_string();
char * 	SSL_state_string_long();
char * 	SSL_rstate_string_long();
void	SSL_copy_session_id();

long	SSL_get_time();
long	SSL_set_time();
long	SSL_get_timeout();
long	SSL_set_timeout();

void	SSL_SESSION_free();
int	i2d_SSL_SESSION();
int	SSL_set_session();
int	SSL_add_session();
void	SSL_remove_session();
SSL_SESSION *SSL_SESSION_new();
#ifndef WIN16
int	SSL_SESSION_print_fp();
#endif
int	SSL_SESSION_print();
SSL_SESSION *d2i_SSL_SESSION();

#ifdef HEADER_X509_H
X509 *	SSL_get_peer_certificate();
#else
char *	SSL_get_peer_certificate();
#endif
#endif

/* tjh added these two dudes to enable external control
 * of debug and trace logging
 */
extern FILE *SSL_ERR;
extern FILE *SSL_LOG;

/* BEGIN ERROR CODES */
/* Error codes for the SSL functions. */

/* Function codes. */
#define SSL_F_CLIENT_CERTIFICATE			 100
#define SSL_F_CLIENT_HELLO				 101
#define SSL_F_CLIENT_MASTER_KEY				 102
#define SSL_F_D2I_SSL_SESSION				 103
#define SSL_F_GET_CLIENT_FINISHED			 104
#define SSL_F_GET_CLIENT_HELLO				 105
#define SSL_F_GET_CLIENT_MASTER_KEY			 106
#define SSL_F_GET_SERVER_FINISHED			 107
#define SSL_F_GET_SERVER_HELLO				 108
#define SSL_F_GET_SERVER_VERIFY				 109
#define SSL_F_I2D_SSL_SESSION				 110
#define SSL_F_READ_N					 111
#define SSL_F_REQUEST_CERTIFICATE			 112
#define SSL_F_SERVER_HELLO				 113
#define SSL_F_SSL_ACCEPT				 114
#define SSL_F_SSL_CERT_NEW				 115
#define SSL_F_SSL_CONNECT				 116
#define SSL_F_SSL_ENC_DES_CBC_INIT			 117
#define SSL_F_SSL_ENC_DES_CFB_INIT			 118
#define SSL_F_SSL_ENC_DES_EDE3_CBC_INIT			 119
#define SSL_F_SSL_ENC_IDEA_CBC_INIT			 120
#define SSL_F_SSL_ENC_NULL_INIT				 121
#define SSL_F_SSL_ENC_RC2_CBC_INIT			 122
#define SSL_F_SSL_ENC_RC4_INIT				 123
#define SSL_F_SSL_GET_NEW_SESSION			 124
#define SSL_F_SSL_MAKE_CIPHER_LIST			 125
#define SSL_F_SSL_NEW					 126
#define SSL_F_SSL_READ					 127
#define SSL_F_SSL_RSA_PRIVATE_DECRYPT			 128
#define SSL_F_SSL_RSA_PUBLIC_ENCRYPT			 129
#define SSL_F_SSL_SESSION_NEW				 130
#define SSL_F_SSL_SESSION_PRINT_FP			 131
#define SSL_F_SSL_SET_CERTIFICATE			 132
#define SSL_F_SSL_SET_FD				 133
#define SSL_F_SSL_SET_RFD				 134
#define SSL_F_SSL_SET_WFD				 135
#define SSL_F_SSL_STARTUP				 136
#define SSL_F_SSL_USE_CERTIFICATE			 137
#define SSL_F_SSL_USE_CERTIFICATE_ASN1			 138
#define SSL_F_SSL_USE_CERTIFICATE_FILE			 139
#define SSL_F_SSL_USE_PRIVATEKEY			 140
#define SSL_F_SSL_USE_PRIVATEKEY_ASN1			 141
#define SSL_F_SSL_USE_PRIVATEKEY_FILE			 142
#define SSL_F_SSL_USE_RSAPRIVATEKEY			 143
#define SSL_F_SSL_USE_RSAPRIVATEKEY_ASN1		 144
#define SSL_F_SSL_USE_RSAPRIVATEKEY_FILE		 145
#define SSL_F_WRITE_PENDING				 146

/* Reason codes. */
#define SSL_R_BAD_AUTHENTICATION_TYPE			 100
#define SSL_R_BAD_CHECKSUM				 101
#define SSL_R_BAD_MAC_DECODE				 102
#define SSL_R_BAD_RESPONSE_ARGUMENT			 103
#define SSL_R_BAD_SSL_FILETYPE				 104
#define SSL_R_BAD_SSL_SESSION_ID_LENGTH			 105
#define SSL_R_BAD_STATE					 106
#define SSL_R_BAD_WRITE_RETRY				 107
#define SSL_R_CHALLENGE_IS_DIFFERENT			 108
#define SSL_R_CIPHER_CODE_TOO_LONG			 109
#define SSL_R_CIPHER_TABLE_SRC_ERROR			 110
#define SSL_R_CONECTION_ID_IS_DIFFERENT			 111
#define SSL_R_INVALID_CHALLENGE_LENGTH			 112
#define SSL_R_NO_CERTIFICATE_SET			 113
#define SSL_R_NO_CERTIFICATE_SPECIFIED			 114
#define SSL_R_NO_CIPHER_LIST				 115
#define SSL_R_NO_CIPHER_MATCH				 116
#define SSL_R_NO_CIPHER_WE_TRUST			 117
#define SSL_R_NO_PRIVATEKEY				 118
#define SSL_R_NO_PUBLICKEY				 119
#define SSL_R_NO_READ_METHOD_SET			 120
#define SSL_R_NO_WRITE_METHOD_SET			 121
#define SSL_R_NULL_SSL_CTX				 122
#define SSL_R_PEER_DID_NOT_RETURN_A_CERTIFICATE		 123
#define SSL_R_PEER_ERROR				 124
#define SSL_R_PEER_ERROR_CERTIFICATE			 125
#define SSL_R_PEER_ERROR_NO_CIPHER			 126
#define SSL_R_PEER_ERROR_UNSUPPORTED_CERTIFICATE_TYPE	 127
#define SSL_R_PERR_ERROR_NO_CERTIFICATE			 128
#define SSL_R_PUBLIC_KEY_ENCRYPT_ERROR			 129
#define SSL_R_PUBLIC_KEY_IS_NOT_RSA			 130
#define SSL_R_PUBLIC_KEY_NO_RSA				 131
#define SSL_R_READ_WRONG_PACKET_TYPE			 132
#define SSL_R_REVERSE_KEY_ARG_LENGTH_IS_WRONG		 133
#define SSL_R_REVERSE_MASTER_KEY_LENGTH_IS_WRONG	 134
#define SSL_R_REVERSE_SSL_SESSION_ID_LENGTH_IS_WRONG	 135
#define SSL_R_SHORT_READ				 136
#define SSL_R_SSL_SESSION_ID_IS_DIFFERENT		 137
#define SSL_R_UNABLE_TO_EXTRACT_PUBLIC_KEY		 138
#define SSL_R_UNDEFINED_INIT_STATE			 139
#define SSL_R_UNKNOWN_REMOTE_ERROR_TYPE			 140
#define SSL_R_UNKNOWN_STATE				 141
#define SSL_R_UNSUPORTED_CIPHER				 142
#define SSL_R_WRONG_PUBLIC_KEY_TYPE			 143
#define SSL_R_X509_LIB					 144

#ifdef  __cplusplus
}
#endif
#endif

