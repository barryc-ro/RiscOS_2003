/* ssl/ssl_stat.c */
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
#include "ssl_locl.h"

char *SSL_state_string_long(s)
SSL *s;
	{
	char *str;

	switch (s->state)
		{
case SSL_ST_BEFORE: str="before SSL initalisation"; break;
case SSL_ST_CLIENT_START_ENCRYPTION: str="client start encryption"; break;
case SSL_ST_SERVER_START_ENCRYPTION: str="server start encryption"; break;
case SSL_ST_OK: str="SSL negotiation finished successfully"; break;
case SSL_ST_SEND_CLIENT_HELLO_A: str="send client hello A"; break;
case SSL_ST_SEND_CLIENT_HELLO_B: str="send client hello B"; break;
case SSL_ST_GET_SERVER_HELLO_A: str="get server hello A"; break;
case SSL_ST_GET_SERVER_HELLO_B: str="get server hello B"; break;
case SSL_ST_SEND_CLIENT_MASTER_KEY_A: str="send client master key A"; break;
case SSL_ST_SEND_CLIENT_MASTER_KEY_B: str="send client master key B"; break;
case SSL_ST_SEND_CLIENT_FINISHED_A: str="send client finished A"; break;
case SSL_ST_SEND_CLIENT_FINISHED_B: str="send client finished B"; break;
case SSL_ST_SEND_CLIENT_CERTIFICATE_A: str="send client certificate A"; break;
case SSL_ST_SEND_CLIENT_CERTIFICATE_B: str="send client certificate B"; break;
case SSL_ST_SEND_CLIENT_CERTIFICATE_C: str="send client certificate C"; break;
case SSL_ST_SEND_CLIENT_CERTIFICATE_D: str="send client certificate D"; break;
case SSL_ST_GET_SERVER_VERIFY_A: str="get server verify A"; break;
case SSL_ST_GET_SERVER_VERIFY_B: str="get server verify B"; break;
case SSL_ST_GET_SERVER_FINISHED_A: str="get server finished A"; break;
case SSL_ST_GET_SERVER_FINISHED_B: str="get server finished B"; break;
case SSL_ST_GET_CLIENT_HELLO_A: str="get client hello A"; break;
case SSL_ST_GET_CLIENT_HELLO_B: str="get client hello B"; break;
case SSL_ST_SEND_SERVER_HELLO_A: str="send server hello A"; break;
case SSL_ST_SEND_SERVER_HELLO_B: str="send server hello B"; break;
case SSL_ST_GET_CLIENT_MASTER_KEY_A: str="get client master key A"; break;
case SSL_ST_GET_CLIENT_MASTER_KEY_B: str="get client master key B"; break;
case SSL_ST_SEND_SERVER_VERIFY_A: str="send server verify A"; break;
case SSL_ST_SEND_SERVER_VERIFY_B: str="send server verify B"; break;
case SSL_ST_GET_CLIENT_FINISHED_A: str="get client finished A"; break;
case SSL_ST_GET_CLIENT_FINISHED_B: str="get client finished B"; break;
case SSL_ST_SEND_SERVER_FINISHED_A: str="send server finished A"; break;
case SSL_ST_SEND_SERVER_FINISHED_B: str="send server finished B"; break;
case SSL_ST_SEND_REQUEST_CERTIFICATE_A: str="send request certificate A"; break;
case SSL_ST_SEND_REQUEST_CERTIFICATE_B: str="send request certificate B"; break;
case SSL_ST_SEND_REQUEST_CERTIFICATE_C: str="send request certificate C"; break;
case SSL_ST_SEND_REQUEST_CERTIFICATE_D: str="send request certificate D"; break;
case SSL_ST_X509_GET_SERVER_CERTIFICATE: str="X509 get server certificate"; break;
case SSL_ST_X509_GET_CLIENT_CERTIFICATE: str="X509 get client certificate"; break;
default:	str="unknown state"; break;
		}
	return(str);
	}

char *SSL_rstate_string_long(s)
SSL *s;
	{
	char *str;

	switch (s->rstate)
		{
	case SSL_ST_READ_HEADER: str="read header"; break;
	case SSL_ST_READ_BODY: str="read body"; break;
	default: str="unknown"; break;
		}
	return(str);
	}

char *SSL_state_string(s)
SSL *s;
	{
	char *str;

	switch (s->state)
		{
case SSL_ST_BEFORE:			str="PINIT"; break;
case SSL_ST_CLIENT_START_ENCRYPTION:	str="CSENC"; break;
case SSL_ST_SERVER_START_ENCRYPTION:	str="SSENC"; break;
case SSL_ST_OK:			 	str="SSLOK"; break;
case SSL_ST_SEND_CLIENT_HELLO_A:	str="SCH_A"; break;
case SSL_ST_SEND_CLIENT_HELLO_B:	str="SCH_B"; break;
case SSL_ST_GET_SERVER_HELLO_A:		str="GSH_A"; break;
case SSL_ST_GET_SERVER_HELLO_B:		str="GSH_B"; break;
case SSL_ST_SEND_CLIENT_MASTER_KEY_A:	str="SCMKA"; break;
case SSL_ST_SEND_CLIENT_MASTER_KEY_B:	str="SCMKB"; break;
case SSL_ST_SEND_CLIENT_FINISHED_A:	str="SCF_A"; break;
case SSL_ST_SEND_CLIENT_FINISHED_B:	str="SCF_B"; break;
case SSL_ST_SEND_CLIENT_CERTIFICATE_A:	str="SCC_A"; break;
case SSL_ST_SEND_CLIENT_CERTIFICATE_B:	str="SCC_B"; break;
case SSL_ST_SEND_CLIENT_CERTIFICATE_C:	str="SCC_C"; break;
case SSL_ST_SEND_CLIENT_CERTIFICATE_D:	str="SCC_D"; break;
case SSL_ST_GET_SERVER_VERIFY_A:	str="GSV_A"; break;
case SSL_ST_GET_SERVER_VERIFY_B:	str="GSV_B"; break;
case SSL_ST_GET_SERVER_FINISHED_A:	str="GSF_A"; break;
case SSL_ST_GET_SERVER_FINISHED_B:	str="GSF_B"; break;
case SSL_ST_GET_CLIENT_HELLO_A:		str="GCH_A"; break;
case SSL_ST_GET_CLIENT_HELLO_B:		str="GCH_B"; break;
case SSL_ST_SEND_SERVER_HELLO_A:	str="SSH_A"; break;
case SSL_ST_SEND_SERVER_HELLO_B:	str="SSH_B"; break;
case SSL_ST_GET_CLIENT_MASTER_KEY_A:	str="GCMKA"; break;
case SSL_ST_GET_CLIENT_MASTER_KEY_B:	str="GCMKA"; break;
case SSL_ST_SEND_SERVER_VERIFY_A:	str="SSV_A"; break;
case SSL_ST_SEND_SERVER_VERIFY_B:	str="SSV_B"; break;
case SSL_ST_GET_CLIENT_FINISHED_A:	str="GCF_A"; break;
case SSL_ST_GET_CLIENT_FINISHED_B:	str="GCF_B"; break;
case SSL_ST_SEND_SERVER_FINISHED_A:	str="SSF_A"; break;
case SSL_ST_SEND_SERVER_FINISHED_B:	str="SSF_B"; break;
case SSL_ST_SEND_REQUEST_CERTIFICATE_A: str="SRC_A"; break;
case SSL_ST_SEND_REQUEST_CERTIFICATE_B: str="SRC_B"; break;
case SSL_ST_SEND_REQUEST_CERTIFICATE_C: str="SRC_C"; break;
case SSL_ST_SEND_REQUEST_CERTIFICATE_D: str="SRC_D"; break;
case SSL_ST_X509_GET_SERVER_CERTIFICATE:str="X9GSC"; break;
case SSL_ST_X509_GET_CLIENT_CERTIFICATE:str="X9GCC"; break;
default:				str="UNKWN"; break;
		}
	return(str);
	}

char *SSL_rstate_string(s)
SSL *s;
	{
	char *str;

	switch (s->rstate)
		{
	case SSL_ST_READ_HEADER:str="RH"; break;
	case SSL_ST_READ_BODY:	str="RB"; break;
	default: str="unknown"; break;
		}
	return(str);
	}
