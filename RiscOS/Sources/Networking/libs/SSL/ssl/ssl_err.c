/* lib/ssl/ssl_err.c */
/* Copyright (C) 1995-1996 Eric Young (eay@mincom.oz.au)
 * All rights reserved.
 * 
 * This file is part of an SSL implementation written
 * by Eric Young (eay@mincom.oz.au).
 * The implementation was written so as to conform with Netscapes SS
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
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIA
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
#include "err.h"
#include "ssl.h"

/* BEGIN ERROR CODES */
static ERR_STRING_DATA SSL_str_functs[]=
	{
{ERR_PACK(0,SSL_F_CLIENT_CERTIFICATE,0),	"CLIENT_CERTIFICATE"},
{ERR_PACK(0,SSL_F_CLIENT_HELLO,0),	"CLIENT_HELLO"},
{ERR_PACK(0,SSL_F_CLIENT_MASTER_KEY,0),	"CLIENT_MASTER_KEY"},
{ERR_PACK(0,SSL_F_D2I_SSL_SESSION,0),	"d2i_SSL_SESSION"},
{ERR_PACK(0,SSL_F_GET_CLIENT_FINISHED,0),	"GET_CLIENT_FINISHED"},
{ERR_PACK(0,SSL_F_GET_CLIENT_HELLO,0),	"GET_CLIENT_HELLO"},
{ERR_PACK(0,SSL_F_GET_CLIENT_MASTER_KEY,0),	"GET_CLIENT_MASTER_KEY"},
{ERR_PACK(0,SSL_F_GET_SERVER_FINISHED,0),	"GET_SERVER_FINISHED"},
{ERR_PACK(0,SSL_F_GET_SERVER_HELLO,0),	"GET_SERVER_HELLO"},
{ERR_PACK(0,SSL_F_GET_SERVER_VERIFY,0),	"GET_SERVER_VERIFY"},
{ERR_PACK(0,SSL_F_I2D_SSL_SESSION,0),	"i2d_SSL_SESSION"},
{ERR_PACK(0,SSL_F_READ_N,0),	"READ_N"},
{ERR_PACK(0,SSL_F_REQUEST_CERTIFICATE,0),	"REQUEST_CERTIFICATE"},
{ERR_PACK(0,SSL_F_SERVER_HELLO,0),	"SERVER_HELLO"},
{ERR_PACK(0,SSL_F_SSL_ACCEPT,0),	"SSL_accept"},
{ERR_PACK(0,SSL_F_SSL_CERT_NEW,0),	"SSL_CERT_NEW"},
{ERR_PACK(0,SSL_F_SSL_CONNECT,0),	"SSL_connect"},
{ERR_PACK(0,SSL_F_SSL_ENC_DES_CBC_INIT,0),	"SSL_ENC_DES_CBC_INIT"},
{ERR_PACK(0,SSL_F_SSL_ENC_DES_CFB_INIT,0),	"SSL_ENC_DES_CFB_INIT"},
{ERR_PACK(0,SSL_F_SSL_ENC_DES_EDE3_CBC_INIT,0),	"SSL_ENC_DES_EDE3_CBC_INIT"},
{ERR_PACK(0,SSL_F_SSL_ENC_IDEA_CBC_INIT,0),	"SSL_ENC_IDEA_CBC_INIT"},
{ERR_PACK(0,SSL_F_SSL_ENC_NULL_INIT,0),	"SSL_ENC_NULL_INIT"},
{ERR_PACK(0,SSL_F_SSL_ENC_RC2_CBC_INIT,0),	"SSL_ENC_RC2_CBC_INIT"},
{ERR_PACK(0,SSL_F_SSL_ENC_RC4_INIT,0),	"SSL_ENC_RC4_INIT"},
{ERR_PACK(0,SSL_F_SSL_GET_NEW_SESSION,0),	"SSL_GET_NEW_SESSION"},
{ERR_PACK(0,SSL_F_SSL_MAKE_CIPHER_LIST,0),	"SSL_MAKE_CIPHER_LIST"},
{ERR_PACK(0,SSL_F_SSL_NEW,0),	"SSL_new"},
{ERR_PACK(0,SSL_F_SSL_READ,0),	"SSL_read"},
{ERR_PACK(0,SSL_F_SSL_RSA_PRIVATE_DECRYPT,0),	"SSL_RSA_PRIVATE_DECRYPT"},
{ERR_PACK(0,SSL_F_SSL_RSA_PUBLIC_ENCRYPT,0),	"SSL_RSA_PUBLIC_ENCRYPT"},
{ERR_PACK(0,SSL_F_SSL_SESSION_NEW,0),	"SSL_SESSION_new"},
{ERR_PACK(0,SSL_F_SSL_SESSION_PRINT_FP,0),	"SSL_SESSION_print_fp"},
{ERR_PACK(0,SSL_F_SSL_SET_CERTIFICATE,0),	"SSL_SET_CERTIFICATE"},
{ERR_PACK(0,SSL_F_SSL_SET_FD,0),	"SSL_set_fd"},
{ERR_PACK(0,SSL_F_SSL_SET_RFD,0),	"SSL_SET_RFD"},
{ERR_PACK(0,SSL_F_SSL_SET_WFD,0),	"SSL_SET_WFD"},
{ERR_PACK(0,SSL_F_SSL_STARTUP,0),	"SSL_STARTUP"},
{ERR_PACK(0,SSL_F_SSL_USE_CERTIFICATE,0),	"SSL_use_certificate"},
{ERR_PACK(0,SSL_F_SSL_USE_CERTIFICATE_ASN1,0),	"SSL_use_certificate_ASN1"},
{ERR_PACK(0,SSL_F_SSL_USE_CERTIFICATE_FILE,0),	"SSL_use_certificate_file"},
{ERR_PACK(0,SSL_F_SSL_USE_PRIVATEKEY,0),	"SSL_use_PrivateKey"},
{ERR_PACK(0,SSL_F_SSL_USE_PRIVATEKEY_ASN1,0),	"SSL_use_PrivateKey_ASN1"},
{ERR_PACK(0,SSL_F_SSL_USE_PRIVATEKEY_FILE,0),	"SSL_use_PrivateKey_file"},
{ERR_PACK(0,SSL_F_SSL_USE_RSAPRIVATEKEY,0),	"SSL_use_RSAPrivateKey"},
{ERR_PACK(0,SSL_F_SSL_USE_RSAPRIVATEKEY_ASN1,0),	"SSL_use_RSAPrivateKey_ASN1"},
{ERR_PACK(0,SSL_F_SSL_USE_RSAPRIVATEKEY_FILE,0),	"SSL_use_RSAPrivateKey_file"},
{ERR_PACK(0,SSL_F_WRITE_PENDING,0),	"WRITE_PENDING"},
{0,NULL},
	};

static ERR_STRING_DATA SSL_str_reasons[]=
	{
{SSL_R_BAD_AUTHENTICATION_TYPE           ,"bad authentication type"},
{SSL_R_BAD_CHECKSUM                      ,"bad checksum"},
{SSL_R_BAD_MAC_DECODE                    ,"bad mac decode"},
{SSL_R_BAD_RESPONSE_ARGUMENT             ,"bad response argument"},
{SSL_R_BAD_SSL_FILETYPE                  ,"bad ssl filetype"},
{SSL_R_BAD_SSL_SESSION_ID_LENGTH         ,"bad ssl session id length"},
{SSL_R_BAD_STATE                         ,"bad state"},
{SSL_R_BAD_WRITE_RETRY                   ,"bad write retry"},
{SSL_R_CHALLENGE_IS_DIFFERENT            ,"challenge is different"},
{SSL_R_CIPHER_CODE_TOO_LONG              ,"cipher code too long"},
{SSL_R_CIPHER_TABLE_SRC_ERROR            ,"cipher table src error"},
{SSL_R_CONECTION_ID_IS_DIFFERENT         ,"conection id is different"},
{SSL_R_INVALID_CHALLENGE_LENGTH          ,"invalid challenge length"},
{SSL_R_NO_CERTIFICATE_SET                ,"no certificate set"},
{SSL_R_NO_CERTIFICATE_SPECIFIED          ,"no certificate specified"},
{SSL_R_NO_CIPHER_LIST                    ,"no cipher list"},
{SSL_R_NO_CIPHER_MATCH                   ,"no cipher match"},
{SSL_R_NO_CIPHER_WE_TRUST                ,"no cipher we trust"},
{SSL_R_NO_PRIVATEKEY                     ,"no privatekey"},
{SSL_R_NO_PUBLICKEY                      ,"no publickey"},
{SSL_R_NO_READ_METHOD_SET                ,"no read method set"},
{SSL_R_NO_WRITE_METHOD_SET               ,"no write method set"},
{SSL_R_NULL_SSL_CTX                      ,"null ssl ctx"},
{SSL_R_PEER_DID_NOT_RETURN_A_CERTIFICATE ,"peer did not return a certificate"},
{SSL_R_PEER_ERROR                        ,"peer error"},
{SSL_R_PEER_ERROR_CERTIFICATE            ,"peer error certificate"},
{SSL_R_PEER_ERROR_NO_CIPHER              ,"peer error no cipher"},
{SSL_R_PEER_ERROR_UNSUPPORTED_CERTIFICATE_TYPE,"peer error unsupported certificate type"},
{SSL_R_PERR_ERROR_NO_CERTIFICATE         ,"perr error no certificate"},
{SSL_R_PUBLIC_KEY_ENCRYPT_ERROR          ,"public key encrypt error"},
{SSL_R_PUBLIC_KEY_IS_NOT_RSA             ,"public key is not rsa"},
{SSL_R_PUBLIC_KEY_NO_RSA                 ,"public key no rsa"},
{SSL_R_READ_WRONG_PACKET_TYPE            ,"read wrong packet type"},
{SSL_R_REVERSE_KEY_ARG_LENGTH_IS_WRONG   ,"reverse key arg length is wrong"},
{SSL_R_REVERSE_MASTER_KEY_LENGTH_IS_WRONG,"reverse master key length is wrong"},
{SSL_R_REVERSE_SSL_SESSION_ID_LENGTH_IS_WRONG,"reverse ssl session id length is wrong"},
{SSL_R_SHORT_READ                        ,"short read"},
{SSL_R_SSL_SESSION_ID_IS_DIFFERENT       ,"ssl session id is different"},
{SSL_R_UNABLE_TO_EXTRACT_PUBLIC_KEY      ,"unable to extract public key"},
{SSL_R_UNDEFINED_INIT_STATE              ,"undefined init state"},
{SSL_R_UNKNOWN_REMOTE_ERROR_TYPE         ,"unknown remote error type"},
{SSL_R_UNKNOWN_STATE                     ,"unknown state"},
{SSL_R_UNSUPORTED_CIPHER                 ,"unsuported cipher"},
{SSL_R_WRONG_PUBLIC_KEY_TYPE             ,"wrong public key type"},
{SSL_R_X509_LIB                          ,"x509 lib"},
{0,NULL},
	};

void ERR_load_SSL_strings()
	{
	static int init=1;

	if (init)
		{
		init=0;
		ERR_load_strings(ERR_LIB_SSL,SSL_str_functs);
		ERR_load_strings(ERR_LIB_SSL,SSL_str_reasons);
		}
	}
