/* e_os.h */
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

#ifndef HEADER_E_OS_H
#define HEADER_E_OS_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef DEVRANDOM
#undef DEVRANDOM  /* set this to you 'random' device if you have one */
#endif

#if defined(NOCONST)
#define const
#endif

#if defined(WIN32) || defined(WIN16)
#ifndef WINDOWS
#define WINDOWS
#ifndef MSDOS
#define MSDOS
#endif
#endif
#endif

#ifdef VC_DEBUG
#define _DEBUG
#include <crtdbg.h>
#endif

/* The following is used becaue of the small stack in some
 * Microsoft operating systems */
#if defined(WIN16) || defined(MSDOS)
#define MS_STATIC	static
#else
#define MS_STATIC
#endif

#ifdef WIN16
#define MS_CALLBACK	_far _loadds
#define MS_FAR		_far
#else
#define MS_CALLBACK
#define MS_FAR
#endif

#if defined(WINDOWS) || defined(MSDOS)
#define RFILE	"rand.dat"
#else
#define RFILE	".rand"
#endif

#if defined(WINDOWS) || defined(MSDOS)

#ifdef WINDOWS
#include <windows.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#endif
#include <io.h>
#include <fcntl.h>

#define EXIT(n)		return(n);
#define LIST_SEPERATOR_CHAR ';'
#define X_OK	0
#define W_OK	2
#define R_OK	4
#define SSLEAY_CONF	"ssleay.cnf"
#define NUL_DEV		"nul"

#else /* The unix world */

#include <unistd.h>

#define SSLEAY_CONF	"ssleay.conf"
#ifdef RISCOS
#define LIST_SEPERATOR_CHAR ','
/* sorry to be a pedant Eric but it should be spelt separator */
#else
#define LIST_SEPERATOR_CHAR ':'
#endif
#ifndef MONOLITH
#define EXIT(n)		exit(n); return(n)
#else
#define EXIT(n)		return(n)
#endif
#define NUL_DEV		"/dev/null"

#endif

#ifdef USE_SOCKETS
#if defined(WINDOWS) || defined(MSDOS)
/* windows world */
#include <winsock.h>

#ifdef NO_SOCK
#define SSLeay_Write(a,b,c)     (-1)
#define SSLeay_Read(a,b,c)      (-1)
#else
#define SSLeay_Write(a,b,c)     send((a),(b),(c),0)
#define SSLeay_Read(a,b,c)      recv((a),(b),(c),0)
#endif

#define SHUTDOWN(fd)    { shutdown((fd),2); closesocket((fd)); }

#else

#ifndef MSDOS
/* unix world */
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/time.h> /* Needed under linux for FD_XXX */
#include <netinet/in.h>


#if defined(NeXT) || defined(_NEXT_SOURCE)
#include <sys/fcntl.h>
#include <sys/types.h>
#endif

#ifdef AIX
#include <sys/select.h>
#endif

#if defined(sun)
#include <sys/filio.h>
#else
#include <sys/ioctl.h>
#endif

#define SSLeay_Read(a,b,c)     read((a),(b),(c))
#define SSLeay_Write(a,b,c)    write((a),(b),(c))
#define SHUTDOWN(fd)    { shutdown((fd),2); close((fd)); }
#define INVALID_SOCKET	-1
#endif
#endif
#endif

#ifndef NOPROTO
#define P_CC_CC	const void *,const void *
#define P_I_I	int,int
#else
#define P_CC_CC
#define P_I_I
#endif

/* not used yet */
#define	CS_BEGIN
#define CS_END

/* do we need to do this for getenv.
 * Just define getenv for use under windows */

#if defined(MSDOS) || defined(_Windows)
#define LIST_SEPERATOR_CHAR ';'
#else
#ifdef RISCOS
#define LIST_SEPERATOR_CHAR ','
#else
#define LIST_SEPERATOR_CHAR ':'
#endif
#endif

#ifdef WIN16
/* How to do this needs to be thought out a bit more.... */
/*char *GETENV(char *);
#define Getenv	GETENV*/
#define Getenv	getenv
#else
#define Getenv getenv
#endif

#define DG_GCC_BUG	/* gcc < 2.6.3 on DGUX */

#ifdef sgi
#define IRIX_CC_BUG	/* all version of IRIX I've tested (4.* 5.*) */
#endif

#ifdef NO_MD2
#define MD2_Init MD2Init
#define MD2_Update MD2Update
#define MD2_Final MD2Final
#define MD2_DIGEST_LENGTH 16
#endif
#ifdef NO_MD5
#define MD5_Init MD5Init
#define MD5_Update MD5Update
#define MD5_Final MD5Final
#define MD5_DIGEST_LENGTH 16
#endif

#ifndef THREADS
#define CRYPTO_w_lock(type)
#define CRYPTO_w_unlock(type)
#define CRYPTO_r_lock(type)
#define CRYPTO_r_unlock(type)
#endif

#ifdef  __cplusplus
}
#endif

#endif
