/* apps/s_time.c */
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

/*-----------------------------------------
   cntime - SSL client connection timer program
   Written and donated by Larry Streepy <streepy@healthcare.com>
  -----------------------------------------*/
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
#include <stdlib.h>
#include <string.h>

#include "x509.h"
#include "ssl.h"
#include "pem.h"
#define USE_SOCKETS
#include "apps.h"
#include "s_apps.h"
#include "err.h"
#ifdef WIN32_STUFF
#include "winmain.h"
#include "wintext.h"
#endif

#ifndef MSDOS
#define TIMES
#endif

#ifndef VMS
#ifndef _IRIX
#include <time.h>
#endif
#ifdef TIMES
#include <sys/types.h>
#include <sys/times.h>
#endif
#else /* VMS */
#include <types.h>
struct tms {
	time_t tms_utime;
	time_t tms_stime;
	time_t tms_uchild;	/* I dunno...  */
	time_t tms_uchildsys;	/* so these names are a guess :-) */
	}
#endif
#ifndef TIMES
#include <sys/timeb.h>
#endif

#ifdef _AIX
#include <sys/select.h>
#endif

#ifdef sun
#include <limits.h>
#include <sys/param.h>
#endif

/* The following if from times(3) man page.  It may need to be changed
*/
#ifndef HZ
#ifndef CLK_TCK
#ifndef VMS
#define HZ      100.0
#else /* VMS */
#define HZ      100.0
#endif
#else /* CLK_TCK */
#define HZ ((double)CLK_TCK)
#endif
#endif

#undef PROG
#define PROG s_time_main

#define ioctl ioctlsocket

#define SSL_HOST_NAME	"localhost"

/*#define TEST_CERT "client.pem" */ /* no default cert. */

#undef BUFSIZZ
#define BUFSIZZ 1024*10

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#undef SECONDS
#define SECONDS	30
extern int verify_depth;
extern int verify_error;

#ifndef NOPROTO
static void usage(void);
static int parseArgs( int argc, char **argv );
static SSL *doConnection( SSL *scon );
#else
static void usage();
static int parseArgs();
static SSL *doConnection();
#endif


/***********************************************************************
 * Static data declarations
 */

static int port=PORT;
static char *host=SSL_HOST_NAME;
static char *t_cert_file=NULL;
static char *t_key_file=NULL;
static char *CApath=NULL;
static char *CAfile=NULL;
static char *cipher=NULL;
static int verify = SSL_VERIFY_NONE;
static int maxTime = SECONDS;
static SSL_CTX *ctx=NULL;

#ifdef FIONBIO
static int nbio=0;
#endif

#ifdef WIN32
static int exitNow = 0;		/* Set when it's time to exit main */
#endif

/***********************************************************************
 * usage - display usage message
 */
static void usage()
{
	static char umsg[] = "\
-time arg     - max number of seconds to collect data, default %d\n\
-verify arg   - turn on peer certificate verification, arg == depth\n\
-cert arg     - certificate file to use, PEM format assumed\n\
-key arg      - RSA file to use, PEM format assumed, in cert file if\n\
                not specified but cert fill is.\n\
-CApath arg   - PEM format directory of CA's\n\
-CAfile arg   - PEM format file of CA's\n\
-cipher       - prefered cipher to use, ':' seperated list of the following\n\
                NULL-MD5     RC4-MD5      EXP-RC4-MD5 \n\
                IDEA-CBC-MD5 RC2-CBC-MD5  EXP-RC2-CBC-MD5\n\
                DES-CBC-MD5  DES-CBC-SHA  DES-CBC3-MD5\n\
                DES-CBC3-SHA DES-CFB-M1\n\n";

	printf( "usage: client <args>\n\n" );

	printf("-host arg     - host or IP to connect to (default is %s)\n",
			SSL_HOST_NAME);
	printf("-port arg     - port to connect to (default is %d)\n",PORT);
#ifdef FIONBIO
	printf("-nbio         - Run with non-blocking IO\n");
#endif
	printf( umsg,SECONDS );
}

/***********************************************************************
 * parseArgs - Parse command line arguments and initialize data
 *
 * Returns 0 if ok, -1 on bad args
 */
static int parseArgs(argc,argv)
int argc;
char **argv;
{
    int badop = 0;

    verify_depth=0;
    verify_error=VERIFY_OK;
#ifdef FIONBIO
    nbio=0;
#endif

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

    argc--;
    argv++;

    while (argc >= 1) {
	if( strcmp(*argv,"-host") == 0) {

	    if (--argc < 1) goto bad;
	    host= *(++argv);

	} else if( strcmp(*argv,"-port") == 0) {

	    if (--argc < 1) goto bad;
	    port=atoi(*(++argv));
	    if (port == 0) goto bad;

	} else if( strcmp(*argv,"-verify") == 0) {

	    verify=SSL_VERIFY_PEER;
	    if (--argc < 1) goto bad;
	    verify_depth=atoi(*(++argv));
	    fprintf(stderr,"verify depth is %d\n",verify_depth);

	} else if( strcmp(*argv,"-cert") == 0) {

	    if (--argc < 1) goto bad;
	    t_cert_file= *(++argv);

	} else if( strcmp(*argv,"-key") == 0) {

	    if (--argc < 1) goto bad;
	    t_key_file= *(++argv);

	} else if( strcmp(*argv,"-CApath") == 0) {

	    if (--argc < 1) goto bad;
	    CApath= *(++argv);

	} else if( strcmp(*argv,"-CAfile") == 0) {

	    if (--argc < 1) goto bad;
	    CAfile= *(++argv);

	} else if( strcmp(*argv,"-cipher") == 0) {

	    if (--argc < 1) goto bad;
	    cipher= *(++argv);
	}
#ifdef FIONBIO
	else if(strcmp(*argv,"-nbio") == 0) {
	    nbio=1;
	}
#endif
	else if( strcmp(*argv,"-time") == 0) {

	    if (--argc < 1) goto bad;
	    maxTime= atoi(*(++argv));
	}
	else {
	    fprintf(stderr,"unknown option %s\n",*argv);
	    badop=1;
	    break;
	}

	argc--;
	argv++;
    }

    if(badop) {
bad:
		usage();
		return -1;
    }

	return 0;			/* Valid args */
}

/***********************************************************************
 * TIME - time functions
 */
#define START	0
#define STOP	1

static double Time_F(s)
int s;
	{
	double ret;
#ifdef TIMES
	static struct tms tstart,tend;

	if(s == START) {
		times(&tstart);
		return(0);
	} else {
		times(&tend);
		ret=((double)(tend.tms_utime-tstart.tms_utime))/HZ;
		return((ret == 0.0)?1e-6:ret);
	}
#else /* !times() */
	static struct timeb tstart,tend;
	long i;

	if(s == START) {
		ftime(&tstart);
		return(0);
	} else {
		ftime(&tend);
		i=(long)tend.millitm-(long)tstart.millitm;
		ret=((double)(tend.time-tstart.time))+((double)i)/1000.0;
		return((ret == 0.0)?1e-6:ret);
	}
#endif
}

/***********************************************************************
 * MAIN - main processing area for client
 *			real name depends on MONOLITH
 */
int
MAIN(argc,argv)
int argc;
char **argv;
{
	double totalTime = 0.0;
	int nConn = 0;
	SSL *scon, *savecon;
	long finishtime=0;

	if ((ctx=SSL_CTX_new()) == NULL) return(1);

	/* parse the command line arguments */
	if( parseArgs( argc, argv ) < 0 ) {
		return 1;
	}

	SSL_load_error_strings();

	if ((!SSL_load_verify_locations(ctx,CAfile,CApath)) ||
		(!SSL_set_default_verify_paths(ctx))) {
		fprintf(stderr,"error seting default verify locations\n");
		ERR_print_errors(bio_err);
		EXIT(1);
	}

	if (cipher == NULL)
		cipher = getenv("SSL_CIPHER");

	if( cipher == NULL ) {
		fprintf( stderr, "No CIPHER specified\n" );
/*		EXIT(1); */
	}

	printf( "Collecting connection statistics for %d seconds\n", maxTime );

	/* Loop and time how long it takes to make connections */

	finishtime=(long)time(NULL)+SECONDS;
	for (;;)
		{
		if (finishtime < time(NULL)) break;
#ifdef WIN32_STUFF

		if( flushWinMsgs(0) == -1 ) {
			EXIT(1);
		}

		if( waitingToDie || exitNow )		/* we're dead */
			EXIT(0);
#endif

		Time_F(START);
		
		if( (scon = doConnection( NULL )) == NULL ) {
			EXIT(1);
		}

		SHUTDOWN(SSL_get_fd(scon));
		SSL_free( scon );

		totalTime += Time_F(STOP); /* Add the time for this iteration */
		nConn += 1;
		fputc( '#', stdout ); fflush(stdout);
	}

	printf( "\n\n%d connections in %.2fs; %.2f connections/user sec\n",
			nConn, totalTime, ((double)nConn/totalTime) );
	printf( "%d connections in %ld real seconds\n",nConn,
		time(NULL)-finishtime+SECONDS);

	/* Now loop and time connections using the same session id over and over */

	printf( "\n\nNow timing with session id reuse.\n" );

	/* Get an SSL object so we can reuse the session id */
	if( (savecon = doConnection( NULL )) == NULL ) {
		fprintf( stderr, "Unable to get connection\n" );
		EXIT(1);
	}
	nConn = 0;
	totalTime = 0.0;

	finishtime=time(NULL)+SECONDS;

	for (;;)
		{
		if (finishtime < time(NULL)) break;

#ifdef WIN32_STUFF
		if( flushWinMsgs(0) == -1 ) {
			EXIT(1);
		}

		if( waitingToDie || exitNow )	/* we're dead */
			EXIT(0);
#endif

		Time_F(START);
		
	 	if( (scon = doConnection( savecon )) == NULL ) {
			EXIT(1);
		}

		SHUTDOWN(SSL_get_fd(scon));
		SSL_free( scon );

	
		totalTime += Time_F(STOP); /* Add the time for this iteration*/
		nConn += 1;
		fputc( '#', stdout ); fflush(stdout);
	}

	SSL_free( savecon );

	printf( "\n\n%d connections in %.2fs; %.2f connections/sec\n",
			nConn, totalTime, ((double)nConn/totalTime) );
	printf( "%d connections in %ld real seconds\n",nConn,
		time(NULL)-finishtime+SECONDS);

	EXIT(0);
}

/***********************************************************************
 * doConnection - make a connection
 * Args:
 *		scon	= earlier ssl connection for session id, or NULL
 * Returns:
 *		SSL *	= the connection pointer.
 */
static SSL *
doConnection(scon)
SSL *scon;
	{
	SSL *serverCon;
	int s;
	int width, i;
	fd_set readfds;

	if(init_client(&s,host,port) == 0) {
		perror( "couldn't connect" );
		/* SHUTDOWN(s); */
		return NULL;
	}

	serverCon=(SSL *)SSL_new(ctx);
	SSL_set_fd(serverCon,s);

	if( scon != NULL )
		SSL_set_session(serverCon,SSL_get_session(scon));

/*	SSL_set_verify(serverCon,verify,verify_callback);*/
	SSL_CTX_set_cipher_list(ctx,cipher);

	if(!set_cert_stuff(serverCon,t_cert_file,t_key_file)) {
		SHUTDOWN(SSL_get_fd(serverCon));
		return NULL;
	}

	/* ok, lets connect */
	do	{
		width=SSL_get_fd(serverCon)+1;
		for(;;) {
			i=SSL_connect(serverCon);
			if (should_retry(i))
				{
				fprintf(stderr,"DELAY\n");

				FD_ZERO(&readfds);
				FD_SET(SSL_get_fd(serverCon),&readfds);
				select(width,&readfds,NULL,NULL,NULL);
				continue;
				}
			break;
			}
		if(i <= 0)
			{
			fprintf(stderr,"ERROR\n");
			if (verify_error != VERIFY_OK)
				fprintf(stderr,"verify error:%s\n",
						X509_cert_verify_error_string(verify_error));
			else
				ERR_print_errors(bio_err);
			return NULL;
			}
		} while (0);
													
	SHUTDOWN(SSL_get_fd(serverCon));

	return serverCon;
	}


