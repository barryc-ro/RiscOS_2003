/* ssl/ssl_trc.c */
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

/* ssl_trc    - generic portable trace routines to make Eric's life easier
 *
 * 04-Jul-95 eay    added stuff so it builds with gcc -Wall
 * 29-Apr-95 tjh    added ssl_fprintf
 * 21-Nov-94 tjh    original coding
 *
 */

#include <stdio.h>
#include "ssl_locl.h"
#include <sys/types.h>
#include "ssl_trc.h"

static char *ssl_trace_prog="SSL";
static int ssl_trace_pid=0;
static int ssl_trace_count=0;
/*static int ssl_trace_enabled=1; */
static int ssl_trace_autoflush=1;
static int ssl_trace_no_header=1;

void SSL_TRACE ( VAR_PLIST( FILE *, fp ) )
    VAR_ALIST
{
    VAR_BDEFN(args, FILE *, fp);
    char *format;
#if defined(THREADS) && defined(sun)
    struct tm data;
#endif
    struct  tm	    *tm_time;
    time_t  time_now = 0;
     
    VAR_INIT(args, FILE *, fp);
    VAR_ARG(args, char *, format);

    /* NULL file pointer is one way to disable the trace */
    if (fp==NULL)
	return;

    time_now = time ( (time_t *)0 );
#if defined(THREADS) && defined(sun)
    tm_time = localtime_r(&time_now,&data);
#else
    tm_time = localtime(&time_now);
#endif

    /* 0 means it isn't set or is no longer right */
    if (ssl_trace_pid==0)
	ssl_trace_pid=42; /* getpid(); EAY */

    ssl_trace_count++;

    /* slip a nice header on the front of whatever we are tracing 
     * that makes it easy to extra and sort into time-sequenced data
     */
    if (!ssl_trace_no_header)
      fprintf(fp,"%s: [%06d/%09d] %d%02d%02d %02d%02d%02d ",
		      ssl_trace_prog,
		      ssl_trace_pid, ssl_trace_count,
		      tm_time->tm_year,
		      tm_time->tm_mon+1,
		      tm_time->tm_mday,
		      tm_time->tm_hour,
		      tm_time->tm_min,
		      tm_time->tm_sec
		      );

    vfprintf(fp,format,args);

    if (ssl_trace_autoflush)
	fflush(fp);

    VAR_END( args );
    return;
}


#undef TRUNCATE

#ifndef TRACEWRITE
#define TRACEWRITE(X,Y,Z)   write((X),(Y),(Z)) /* hmm... */
#endif

/*
 *
 * 20-Apr-93 tjh     put - at 8th posn
 * xx-xxx-86 tjh     original coding
 *
 */
#define WIDTH 16

int
ssl_ddt_dump(s,len)
char *s;
int len;
{
#ifndef WIN16
	int i,j,rows,trunc;

        trunc=0;
#ifdef TRUNCATE
        for(;(len>0) && (s[len-1]==' ')||(s[len-1]=='\0');len--) 
	    trunc++;
#endif

	rows=(len/WIDTH);
	if ((rows*WIDTH)<len)
	  rows++;
	for(i=0;i<rows;i++) {
	  printf("%04x - ",i*WIDTH);
	  for(j=0;j<WIDTH;j++) {
		if (((i*WIDTH)+j)>=len)
			printf("   ");
		else
			printf("%02x%c",(char)*((char *)(s)+i*WIDTH+j)&0xff,j==7?'-':' ');
	  }
	  printf("  ");
	  for(j=0;j<WIDTH;j++) {
	    char ch;

	    if (((i*WIDTH)+j)>=len)
	      break;
	    ch=((char)*((char *)(s)+i*WIDTH+j)&0xff);
/*
	    printf("%c",isprint(ch)?ch:'.');
*/
	    printf("%c",((ch>=' ')&&(ch<='~'))?ch:'.');
	  }
	  printf("\n");
	}

#ifdef TRUNCATE
	if (trunc!=0) 
	    printf("%04x - <SPACES/NULS>\n",len+trunc);
#endif
#endif /* WIN16 */
  return(0);
}

int
ssl_ddt_dump_fd(fd,s,len)
int fd;
char *s;
int len;
{
  char buf[160+1],tmp[20];
  int i,j,rows,trunc;
  unsigned char ch;

  trunc=0;

#ifdef TRUNCATE
  for(;(len>0) && (s[len-1]==' ')||(s[len-1]=='\0');len--) 
    trunc++;
#endif

  rows=(len/WIDTH);
  if ((rows*WIDTH)<len)
    rows++;
  for(i=0;i<rows;i++) {
    buf[0]='\0';	/* start with empty string */
    sprintf(tmp,"%04x - ",i*WIDTH);
    strcpy(buf,tmp);
    for(j=0;j<WIDTH;j++) {
      if (((i*WIDTH)+j)>=len) {
	strcat(buf,"   ");
      } else {
        ch=((unsigned char)*((char *)(s)+i*WIDTH+j)) & 0xff;
	sprintf(tmp,"%02x%c",ch,j==7?'-':' ');
        strcat(buf,tmp);
      }
    }
    strcat(buf,"  ");
    for(j=0;j<WIDTH;j++) {
      if (((i*WIDTH)+j)>=len)
	break;
      ch=((unsigned char)*((char *)(s)+i*WIDTH+j)) & 0xff;
      sprintf(tmp,"%c",((ch>=' ')&&(ch<='~'))?ch:'.');
      strcat(buf,tmp);
    }
    strcat(buf,"\n");
    /* if this is the last call then update the ddt_dump thing so that
     * we will move the selection point in the debug window 
     */
    TRACEWRITE(fd,(char *)buf,strlen(buf));
  }
#ifdef TRUNCATE
  if (trunc!=0) {
    sprintf(buf,"%04x - <SPACES/NULS>\n",len+trunc);
    TRACEWRITE(fd,(char *)buf,strlen(buf));
  }
#endif
  return 0;
}


int SSL_fprintf ( VAR_PLIST( SSL *, ssl_con ) )
    VAR_ALIST
{
    VAR_BDEFN(args, SSL *, ssl_con);
    FILE *fp;
    char *format;
    char hugebuf[1024*10];        /* 10k in one chunk is the limit */

    VAR_INIT(args, SSL *, ssl_con);
    VAR_ARG(args, FILE *, fp);
    VAR_ARG(args, char *, format);

    if (ssl_con==NULL)
        return vfprintf(fp,format,args);

    hugebuf[0]='\0';

#if defined(sun) && !defined(VAR_ANSI) /**/
/* #ifdef sun */
    _doprnt(hugebuf,format,args);
#else /* !sun */
    vsprintf(hugebuf,format,args);
#endif /* sun */

#ifndef LOCAL_TEST
    SSL_write(ssl_con,hugebuf,(unsigned int)strlen(hugebuf));
#endif /* !LOCAL_TEST */

    VAR_END( args );
    return 0;
}


#ifdef LOCAL_TEST

#include <stdio.h>

#define SERVER_PROTOCOL "HTTP/1.0"
#define LF 10

SSL *ssl_con;

void begin_http_header(FILE *fd, char *msg) {
    SSL_fprintf(ssl_con,fd,"%s %s%c",SERVER_PROTOCOL,msg,LF);
}

int
main(argc,argv)
int argc;
char *argv[];
{
    ssl_con=NULL;

    SSL_TRACE(stderr,"Hello there\n");
    SSL_TRACE(stderr,"Hello %s %d there %x\n","tjh",1,16);
    SSL_TRACE(NULL,"Should not see this\n");
    SSL_TRACE(stderr,"Should not see this (twice - only once)\n");

    begin_http_header(stderr,"302 Found");


    fflush(stderr);

    exit(0);

}

#endif

