/* ssl/ssl_pkt.c */
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
#include "errno.h"
#define USE_SOCKETS
#include "ssl_locl.h"
#include "ssl_trc.h"

/* SSLerr(SSL_F_GET_SERVER_HELLO,SSL_R_PEER_ERROR_NO_CIPHER);
 * SSLerr(SSL_F_GET_SERVER_HELLO,SSL_R_PERR_ERROR_NO_CERTIFICATE);
 * SSLerr(SSL_F_GET_SERVER_HELLO,SSL_R_PEER_ERROR_CERTIFICATE);
 * SSLerr(SSL_F_GET_SERVER_HELLO,SSL_R_PEER_ERROR_UNSUPPORTED_CERTIFICATE_TYPE);
 * SSLerr(SSL_F_GET_SERVER_HELLO,SSL_R_UNKNOWN_REMOTE_ERROR_TYPE);
 */

#ifndef NOPROTO
static int read_n(SSL *s,unsigned int n,unsigned int max,unsigned int extend);
static int do_ssl_write(SSL *s, const char *buf, unsigned int len);
static int write_pending(SSL *s, const char *buf, unsigned int len);
static int ssl_mt_error(int n);
#else
static int read_n();
static int do_ssl_write();
static int write_pending();
static int ssl_mt_error();
#endif

/* SSL_read -
 * This routine will return 0 to len bytes, decrypted etc if required.
 */
int SSL_read(s, buf, len)
SSL *s;
char *buf;
unsigned int len;
	{
	int n;
	unsigned char mac[MAX_MAC_SIZE];
	unsigned char *pp;
	int i;

	s->rwstate=SSL_NOTHING;
	s->send=0;
	if (s->ract_data_length != 0) /* read from buffer */
		{
		if (len > s->ract_data_length)
			n=s->ract_data_length;
		else
			n=len;

		memcpy(buf,s->ract_data,(unsigned int)n);
		s->ract_data_length-=n;
		s->ract_data+=n;
		if (s->ract_data_length == 0)
			s->rstate=SSL_ST_READ_HEADER;
		return(n);
		}

	if (s->rstate == SSL_ST_READ_HEADER)
		{
		n=read_n(s,2,SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER+2,0);

		if (n <= 0) return(n); /* error or non-blocking */
		/* part read stuff */

		s->rstate=SSL_ST_READ_BODY;
		pp=s->packet;
		/* Do header */
		s->padding=0;
		s->escape=0;
		s->length=((pp[0]<<8)|(pp[1]));
		if ((pp[0] & TWO_BYTE_BIT))		/* Two byte header? */
			{
			s->three_byte_header=0;
			s->length&=TWO_BYTE_MASK;	
			}
		else
			{
			s->three_byte_header=1;
			s->length&=THREE_BYTE_MASK;

			/* security escape */
			s->escape=((pp[0] & SEC_ESC_BIT))?1:0;
			}
		}

	if (s->rstate == SSL_ST_READ_BODY)
		{
		n=s->length+2+s->three_byte_header;
		if (n > (int)s->packet_length)
			{
			n-=s->packet_length;
			i=read_n(s,(unsigned int)n,(unsigned int)n,1);
			if (i <= 0) return(i); /* ERROR */
			}

		pp= &(s->packet[2]);
		s->rstate=SSL_ST_READ_HEADER;
		if (s->three_byte_header)
			s->padding= *(pp++);
		else	s->padding=0;

		/* Data portion */
		if (s->clear_text)
			{
			s->mac_data=pp;
			s->ract_data=pp;
			s->pad_data=NULL;
			}
		else
			{
			s->mac_data=pp;
			s->ract_data= &pp[s->session->cipher->mac_size];
			s->pad_data= &pp[s->session->cipher->mac_size+
				s->length-s->padding];
			}

		s->ract_data_length=s->length;
		/* added a check for length > max_size incase
		 * encryption was not turned on yet due to an error */
		if ((!s->clear_text) &&
			(s->length >= s->session->cipher->mac_size))
			{
			s->session->cipher->crypt(s);
			s->ract_data_length-=s->session->cipher->mac_size;
			s->session->cipher->hash(s,mac);
			s->ract_data_length-=s->padding;
			if (	(memcmp(mac,s->mac_data,
				(unsigned int)s->session->cipher->mac_size) != 0)||
				(s->length%s->session->cipher->block_size != 0))
				{
				SSLerr(SSL_F_SSL_READ,SSL_R_BAD_MAC_DECODE);
				return(SSL_RWERR_BAD_MAC_DECODE);
				}
			}
		INC32(s->read_sequence); /* expect next number */
		/* s->ract_data is now available for processing */
		return(SSL_read(s,buf,len));
		}
	else
		{
		SSLerr(SSL_F_SSL_READ,SSL_R_BAD_STATE);
			return(SSL_RWERR_INTERNAL_ERROR);
		}
	}

static int read_n(s, n, max, extend)
SSL *s;
unsigned int n;
unsigned int max;
unsigned int extend;
	{
	int i,off,new;

	/* if there is stuff still in the buffer from a previous read,
	 * and there is more than we want, take some. */
	if (s->rbuf_left >= (int)n)
		{
		if (extend)
			s->packet_length+=n;
		else
			{
			s->packet= &(s->rbuf[s->rbuf_offs]);
			s->packet_length=n;
			}
		s->rbuf_left-=n;
		s->rbuf_offs+=n;
		return(n);
		}

	if (!s->read_ahead) max=n;
	if (max > (unsigned int)(SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER+2))
		max=SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER+2;
	

	/* Else we want more than we have.
	 * First, if there is some left or we want to extend */
	off=0;
	if ((s->rbuf_left != 0) || ((s->packet_length != 0) && extend))
		{
		new=s->rbuf_left;
		if (extend)
			{
			off=s->packet_length;
			if (s->packet != s->rbuf)
				memcpy(s->rbuf,s->packet,(unsigned int)new+off);
			}
		else if (s->rbuf_offs != 0)
			{
			memcpy(s->rbuf,&(s->rbuf[s->rbuf_offs]),
				(unsigned int)new);
			s->rbuf_offs=0;
			}
		s->rbuf_left=0;
		}
	else
		new=0;

	/* off is the offset to start writing too.
	 * r->rbuf_offs is the 'unread data', now 0. 
	 * new is the number of new bytes so far
	 */
	s->packet=s->rbuf;
	while (new < (int)n)
		{
		errno=0;
		if (s->rbio != NULL)
			i=BIO_read(s->rbio,(char *)&(s->rbuf[off+new]),max-new);
		else
			{
			SSLerr(SSL_F_READ_N,SSL_R_NO_READ_METHOD_SET);
			i=-1;
			}
#ifdef PKT_DEBUG
		if (s->debug & 0x01) sleep(1);
#endif
		if (i <= 0)
			{
			s->rbuf_left+=new;
			return(i);
			}
		new+=i;
		}

	/* record unread data */
	if (new > (int)n)
		{
		s->rbuf_offs=n+off;
		s->rbuf_left=new-n;
		}
	else
		{
		s->rbuf_offs=0;
		s->rbuf_left=0;
		}
	if (extend)
		s->packet_length+=n;
	else
		s->packet_length=n;
	return(n);
	}

/* Ok, I'm just about to go over this to allow for non-blocking io */
int SSL_write(s, buf, len)
SSL *s;
const char *buf;
unsigned int len;
	{
	unsigned int n,tot;
	int i;

	s->rwstate=SSL_NOTHING;

	tot=s->wnum;
	s->wnum=0;

	n=(len-tot);
	for (;;)
		{
		i=do_ssl_write(s,&(buf[tot]),n);
		if (i <= 0)
			{
			s->wnum=tot;
			return(i);
			}
		if (i == (int)n) return(tot+i);

		n-=i;
		tot+=i;
		}
	}

static int write_pending(s,buf,len)
SSL *s;
const char *buf;
unsigned int len;
	{
	int i;

	/* s->wpend_len != 0 MUST be true. */

	/* check that they have given us the same buffer to
	 * write */
	if ((s->wpend_tot != (int)len) || (s->wpend_buf != buf))
		{
		SSLerr(SSL_F_WRITE_PENDING,SSL_R_BAD_WRITE_RETRY);
		return(SSL_RWERR_BAD_WRITE_RETRY);
		}
	for (;;)
		{
		errno=0;
		if (s->wbio != NULL)
			{
			i=BIO_write(s->wbio,(char *)&(s->write_ptr[s->wpend_off]),
				(unsigned int)s->wpend_len);
			}
		else
			{
			SSLerr(SSL_F_WRITE_PENDING,SSL_R_NO_WRITE_METHOD_SET);
			i=-1;
			}
#ifdef PKT_DEBUG
		if (s->debug & 0x01) sleep(1);
#endif
		if (i == s->wpend_len)
			{
			s->wpend_len=0;
			return(s->wpend_ret);
			}
		else if (i <= 0)
			return(i);
		s->wpend_off+=i;
		s->wpend_len-=i;
		}
	}

static int do_ssl_write(s, buf, len)
SSL *s;
const char *buf;
unsigned int len;
	{
	unsigned int j,k,olen,p,mac_size,bs;
	register unsigned char *pp;

	s->send=1;
	olen=len;

	/* first check if there is data from an encryption waiting to
	 * be sent - it must be sent because the other end is waiting.
	 * This will happen with non-blocking IO.  We print it and then
	 * return.
	 */
	if (s->wpend_len != 0) return(write_pending(s,buf,len));

	/* set mac_size to mac size */
	if (s->clear_text)
		mac_size=0;
	else
		mac_size=s->session->cipher->mac_size;

	/* lets set the pad p */
	if (s->clear_text)
		{
		if (len > SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER)
			len=SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER;
		p=0;
		s->three_byte_header=0;
		/* len=len; */
		}
	else
		{
		bs=s->session->cipher->block_size;
		j=len+mac_size;
		if (j > SSL_MAX_RECORD_LENGTH_3_BYTE_HEADER)
			{
			if (j > SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER)
				j=SSL_MAX_RECORD_LENGTH_2_BYTE_HEADER;
			/* set k to the max number of bytes with 2
			 * byte header */
			k=j-(j%bs);
			/* how many data bytes? */
			len=k-mac_size; 
			s->three_byte_header=0;
			p=0;
			}
		else if (bs <= 1)
			{
			/* len=len; */
			s->three_byte_header=0;
			p=0;
			}
		else /* 3 byte header */
			{
			/*len=len; */
			p=(j%bs);
			p=(p == 0)?0:(bs-p);
			s->three_byte_header=(p == 0)?0:1;
			}
		}
	/* mac_size is the number of MAC bytes
	 * len is the number of data bytes we are going to send
	 * p is the number of padding bytes
	 * if p == 0, it is a 2 byte header */

	s->length=len;
	s->padding=p;
	s->mac_data= &(s->wbuf[3]);
	s->wact_data= &(s->wbuf[3+mac_size]);
	/* we copy the data into s->wbuf */
	memcpy(s->wact_data,buf,len);
#ifdef PURIFY
	if (p)
		memset(&(s->wact_data[len]),0,p);
#endif

	if (!s->clear_text)
		{
		s->wact_data_length=len+p;
		s->session->cipher->hash(s,s->mac_data);
		s->length+=p+mac_size;
		s->session->cipher->crypt(s);
		}

	/* package up the header */
	s->wpend_len=s->length;
	if (s->three_byte_header) /* 3 byte header */
		{
		pp=s->mac_data;
		pp-=3;
		pp[0]=(s->length>>8)&(THREE_BYTE_MASK>>8);
		if (s->escape) pp[0]|=SEC_ESC_BIT;
		pp[1]=s->length&0xff;
		pp[2]=s->padding;
		s->wpend_len+=3;
		}
	else
		{
		pp=s->mac_data;
		pp-=2;
		pp[0]=((s->length>>8)&(TWO_BYTE_MASK>>8))|TWO_BYTE_BIT;
		pp[1]=s->length&0xff;
		s->wpend_len+=2;
		}
	s->write_ptr=pp;
	
	INC32(s->write_sequence); /* expect next number */

	/* lets try to actually write the data */
	s->wpend_tot=olen;
	s->wpend_buf=(char *)buf;

	s->wpend_ret=len;

	s->wpend_off=0;
	return(write_pending(s,buf,olen));
	}

int ssl_part_read(s,f,i)
SSL *s;
unsigned long f;
int i;
	{
	/* check for error */
	if ((s->init_num == 0) && (i >= 3))
		{
		if (s->init_buf[0] == SSL_MT_ERROR)
			{
			int i;

			i=(s->init_buf[1]<<8)|s->init_buf[2];
			SSLerr((int)f,ssl_mt_error(i));
			}
		}

	s->rwstate=SSL_READING;

	if (i < 0)
		{
		/* ssl_return_error(s); */
		/* for non-blocking io,
		 * this is not fatal */
#ifdef PKT_DEBUG
	        SSL_TRACE(SSL_ERR,"read return is -1, errno=%d\n",errno);
#endif

		return(-1);
		}
	else
		{
		s->init_num+=i;
		return(0);
		}
	}

int ssl_do_write(s)
SSL *s;
	{
	int ret;

	ret=SSL_write(s,
		(char *)&(s->init_buf[s->init_off]),(unsigned int)s->init_num);
	if (ret == s->init_num)
		return(1);
	s->rwstate=SSL_WRITING;
	if (ret < 0)
		return(-1);
	s->init_off+=ret;
	s->init_num-=ret;
	return(0);
	}

static int ssl_mt_error(n)
int n;
	{
	int ret;

	switch (n)
		{
	case SSL_PE_NO_CIPHER:
		ret=SSL_R_PEER_ERROR_NO_CIPHER;
		break;
	case SSL_PE_NO_CERTIFICATE:
		ret=SSL_R_PERR_ERROR_NO_CERTIFICATE;
		break;
	case SSL_PE_BAD_CERTIFICATE:
		ret=SSL_R_PEER_ERROR_CERTIFICATE;
		break;
	case SSL_PE_UNSUPPORTED_CERTIFICATE_TYPE:
		ret=SSL_R_PEER_ERROR_UNSUPPORTED_CERTIFICATE_TYPE;
		break;
	default:
		ret=SSL_R_UNKNOWN_REMOTE_ERROR_TYPE;
		break;
		}
	return(ret);
	}
