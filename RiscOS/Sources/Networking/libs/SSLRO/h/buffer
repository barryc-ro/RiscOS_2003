/* crypto/buffer/buffer.h */
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

#ifndef HEADER_BUFFER_H
#define HEADER_BUFFER_H

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct buf_mem_st
	{
	int length;
	char *data;
	int max;
	} BUF_MEM;

#define BIO_TYPE_NONE		0
#define BIO_TYPE_MEM		1
#define BIO_TYPE_FILE		2
#define BIO_TYPE_FILENAME	3
#define BIO_TYPE_FD		4
#define BIO_TYPE_SOCKET		5
#define BIO_TYPE_NULL		6
#define BIO_TYPE_SSL		7
#define BIO_TYPE_MD		8	/* pasive filter */
#define BIO_TYPE_BUFFER		9	/* filter */
#define BIO_TYPE_ENCRYPT	10	/* filter */
#define BIO_TYPE_BASE64		11	/* filter */

/* BIO_FILENAME_READ|BIO_CLOSE to open and close on free. */
#define BIO_NOCLOSE		0
#define BIO_CLOSE		1

#define BIO_CTRL_RESET		1  /* opt - rewind/zero etc */
#define BIO_CTRL_EOF		2  /* opt - are we at the eof */
#define BIO_CTRL_INFO		3  /* opt - extra tit-bits */
#define BIO_CTRL_SET		4  /* man - set the 'IO' type */
#define BIO_CTRL_GET		5  /* man - get the 'IO' type */
#define BIO_CTRL_PUSH		6  /* man - set the set down stream filter */
#define BIO_CTRL_POP		7  /* man - set the get down stream filter */
#define BIO_CTRL_GET_CLOSE	8  /* man - set the 'close' on free */
#define BIO_CTRL_SET_CLOSE	9  /* man - set the 'close' on free */
#define BIO_CTRL_PENDING	10  /* opt - is their more data buffered */
#define BIO_CTRL_FLUSH		11  /* opt - 'flush' buffered output */
#define BIO_CTRL_SHOULD_RETRY	12 /* opt - should we retry the failed op */
#define BIO_CTRL_RETRY_TYPE	13 /* opt - r/w for retry */

#define BIO_CTRL_SET_FILENAME	30	/* BIO_s_file special */

#define BIO_FP_READ		2
#define BIO_FP_WRITE		4
#define BIO_FP_APPEND		8

#define BIO_FLAGS_READ		0x01
#define BIO_FLAGS_WRITE		0x02
#define BIO_FLAGS_RW		(BIO_FLAGS_READ|BIO_FLAGS_WRITE)
#define BIO_FLAGS_SHOULD_RETRY	0x04

#define BIO_should_read(a)	((a)->flags & BIO_FLAGS_READ)
#define BIO_should_write(a)	((a)->flags & BIO_FLAGS_WRITE)
#define BIO_retry_type(a)	((a)->flags & BIO_FLAGS_RW)
#define BIO_should_retry(a)	((a)->flags & BIO_FLAGS_SHOULD_RETRY)

#define BIO_CB_FREE	0x01
#define BIO_CB_READ	0x02
#define BIO_CB_WRITE	0x03
#define BIO_CB_PUTS	0x04
#define BIO_CB_GETS	0x05
#define BIO_CB_CTRL	0x06

#define BIO_CB_RETURN	0x80
#define BIO_CB_return(a) ((a)|BIO_CB_RETURN))
#define BIO_cb_pre(a)	(!((a)&BIO_CB_RETURN))
#define BIO_cb_post(a)	((a)&BIO_CB_RETURN)

#define BIO_set_callback(b,cb)		((b)->callback=(cb))
#define BIO_set_callback_arg(b,arg)	((b)->cb_arg=(char *)(arg))
#define BIO_get_callback(b)		((b)->callback)
#define BIO_method_name(b)		((b)->method->name)
#define BIO_method_type(b)		((b)->method->type)

#ifndef WIN16
typedef struct bio_method_st
	{
	int type;
	char *name;
	int (*bwrite)();
	int (*bread)();
	int (*bputs)();
	int (*bgets)();
	long (*ctrl)();
	int (*create)();
	int (*destroy)();
	} BIO_METHOD;
#else
typedef struct bio_method_st
	{
	int type;
	char *name;
	int (_far *bwrite)();
	int (_far *bread)();
	int (_far *bputs)();
	int (_far *bgets)();
	long (_far *ctrl)();
	int (_far *create)();
	int (_far *destroy)();
	} BIO_METHOD;
#endif

typedef struct bio_st
	{
	BIO_METHOD *method;
#ifndef NOPROTO
	/* bio, mode, argp, argi, argl, ret */
	long (*callback)(struct bio_st *,int,char *,int, long,long);
#else
	long (*callback)();
#endif
	char *cb_arg; /* first argument for the callback */

	int init;
	int shutdown;
	int flags;	/* extra storage */
	int num;
	char *ptr;
	} BIO;

typedef struct bio_f_buffer_ctx_struct
	{
	BIO *bio;		/* the bio for IO */
	int buf_size;		/* how big is the buffer */

	char *ibuf;		/* the char array */
	int ibuf_len;		/* how many bytes are in it */
	int ibuf_off;		/* write/read offset */

	char *obuf;		/* the char array */
	int obuf_len;		/* how many bytes are in it */
	int obuf_off;		/* write/read offset */
	} BIO_F_BUFFER_CTX;

#define BIO_set_fd(b,fdp,c)	BIO_ctrl(b,BIO_CTRL_SET,c,(char *)fdp)
#define BIO_get_fd(b,c)		BIO_ctrl(b,BIO_CTRL_GET,0,(char *)c)

#define BIO_set_fp(b,fp,c)	BIO_ctrl(b,BIO_CTRL_SET,c,(char *)fp)
#define BIO_get_fp(b,fpp)	BIO_ctrl(b,BIO_CTRL_GET,0,(char **)fpp)

#define BIO_read_filename(b,name) BIO_ctrl(b,BIO_CTRL_SET_FILENAME, \
		BIO_CLOSE|BIO_FP_READ,name)
#define BIO_write_filename(b,name) BIO_ctrl(b,BIO_CTRL_SET_FILENAME, \
		BIO_CLOSE|BIO_FP_WRITE,name)
#define BIO_append_filename(b,name) BIO_ctrl(b,BIO_CTRL_SET_FILENAME, \
		BIO_CLOSE|BIO_FP_APPEND,name)

#define BIO_set_ssl(b,ssl,c)	BIO_ctrl(b,BIO_CTRL_SET,c,(char *)ssl)
#define BIO_get_ssl(b,sslp)	BIO_ctrl(b,BIO_CTRL_GET,0,(char **)sslp)

#define BIO_set_bio(b,bio)	BIO_ctrl(b,BIO_CTRL_SET,0,(char *)bio)
/* #define BIO_set_md(b,md)	BIO_ctrl(b,BIO_CTRL_SET,1,(char *)md) */

#define BIO_reset(b)		BIO_ctrl(b,BIO_CTRL_RESET,0,NULL)
#define BIO_eof(b)		BIO_ctrl(b,BIO_CTRL_EOF,0,NULL)
#define BIO_set_close(b,c)	BIO_ctrl(b,BIO_CTRL_SET_CLOSE,(c),NULL)
#define BIO_get_close(b)	BIO_ctrl(b,BIO_CTRL_GET_CLOSE,0,NULL)
#define BIO_pending(b)		BIO_ctrl(b,BIO_CTRL_PENDING,0,NULL)
#define BIO_flush(b)		BIO_ctrl(b,BIO_CTRL_FLUSH,0,NULL)
#define BIO_push(b,bio)		BIO_ctrl(b,BIO_CTRL_PUSH,0,bio)
/* the next 2 have been done as macros */
/* #define BIO_should_retry(b)	BIO_ctrl(b,BIO_CTRL_SHOULD_RETRY,0,NULL) */
/* #define BIO_retry_type(b)	BIO_ctrl(b,BIO_CTRL_RETRY_TYPE,0,NULL) */

#ifndef NOPROTO
BUF_MEM *BUF_MEM_new(void);
void	BUF_MEM_free(BUF_MEM *a);
int	BUF_MEM_grow(BUF_MEM *str, int len);

BIO *	BIO_new(BIO_METHOD *type);
int	BIO_free(BIO *a);
int	BIO_read(BIO *b, char *data, int len);
int	BIO_gets(BIO *bp,char *buf, int size);
int	BIO_write(BIO *b, char *data, int len);
int	BIO_puts(BIO *bp,char *buf);
int	BIO_puts(BIO *bp,char *buf);
long	BIO_ctrl(BIO *bp,int cmd,long larg,char *parg);

#ifndef WIN16
long BIO_debug_callback(BIO *bio,int cmd,char *argp,int argi,
	long argl,long ret);
#else
long _far _loadds BIO_debug_callback(BIO *bio,int cmd,char *argp,int argi,
	long argl,long ret);
#endif

#ifndef WIN16
BIO_METHOD *BIO_s_file(void);
#else
BIO_METHOD *BIO_s_file_internal_w16(void);
#endif

BIO_METHOD *BIO_s_mem(void);
BIO_METHOD *BIO_s_socket(void);
BIO_METHOD *BIO_s_fd(void);
BIO_METHOD *BIO_s_null(void);
BIO_METHOD *BIO_f_buffer(void);

void	ERR_load_BUF_strings(void );

#else

BUF_MEM *BUF_MEM_new();
void	BUF_MEM_free();
int	BUF_MEM_grow();

#ifndef WIN16
long BIO_debug_callback();
#else
long _far _loadds BIO_debug_callback();
#endif

BIO *	BIO_new();
int	BIO_free();
int	BIO_read();
int	BIO_gets();
int	BIO_write();
int	BIO_puts();
int	BIO_puts();
long	BIO_ctrl();

#ifndef WIN16
BIO_METHOD *BIO_s_file();
#else
BIO_METHOD *BIO_s_file_internal_w16();
#endif

BIO_METHOD *BIO_s_mem();
BIO_METHOD *BIO_s_socket();
BIO_METHOD *BIO_s_fd();
BIO_METHOD *BIO_s_null();
BIO_METHOD *BIO_f_buffer();

void	ERR_load_BUF_strings();
#endif

/* BEGIN ERROR CODES */
/* Error codes for the BUF functions. */

/* Function codes. */
#define BUF_F_BIO_CTRL					 100
#define BUF_F_BIO_GETS					 101
#define BUF_F_BIO_NEW					 102
#define BUF_F_BIO_PUTS					 103
#define BUF_F_BIO_READ					 104
#define BUF_F_BIO_WRITE					 105
#define BUF_F_BUF_MEM_GROW				 106
#define BUF_F_BUF_MEM_NEW				 107
#define BUF_F_FILE_CTRL					 108
#define BUF_F_MEM_WRITE					 109

/* Reason codes. */
#define BUF_R_BAD_FOPEN_MODE				 100
#define BUF_R_NULL_PARAMETER				 101
#define BUF_R_UNINITALISED				 102
#define BUF_R_UNSUPPORTED_METHOD			 103

#ifdef  __cplusplus
}
#endif
#endif

