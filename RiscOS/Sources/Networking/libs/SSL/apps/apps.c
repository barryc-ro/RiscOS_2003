/* apps/apps.c */
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
#include <sys/types.h>
#include <sys/stat.h>
#define NON_MAIN
#include "apps.h"
#undef NON_MAIN

#ifdef WIN16
#define APPS_WIN16
#include "../crypto/buffer/bss_file.c"
#endif

#ifdef undef /* never finished - probably never will be :-) */
int args_from_file(file,argc,argv)
char *file;
int *argc;
char **argv[];
	{
	FILE *fp;
	int num,i;
	unsigned int len;
	static char *buf=NULL;
	static char **arg=NULL;
	char *p;
	struct stat stbuf;

	if (stat(file,&stbuf) < 0) return(0);

	fp=fopen(file,"r");
	if (fp == NULL)
		return(0);

	*argc=0;
	*argv=NULL;

	len=(unsigned int)stbuf.st_size;
	if (buf != NULL) free(buf);
	buf=(char *)malloc(len+1);
	if (buf == NULL) return(0);

	len=fread(buf,1,len,fp);
	if (len <= 1) return(0);
	buf[len]='\0';

	i=0;
	for (p=buf; *p; p++)
		if (*p == '\n') i++;
	if (arg != NULL) free(arg);
	arg=(char **)malloc(sizeof(char *)*(i*2));

	*argv=arg;
	num=0;
	p=buf;
	for (;;)
		{
		if (!*p) break;
		if (*p == '#') /* comment line */
			{
			while (*p && (*p != '\n')) p++;
			continue;
			}
		/* else we have a line */
		*(arg++)=p;
		num++;
		while (*p && ((*p != ' ') && (*p != '\t') && (*p != '\n')))
			p++;
		if (!*p) break;
		if (*p == '\n')
			{
			*(p++)='\0';
			continue;
			}
		/* else it is a tab or space */
		p++;
		while (*p && ((*p == ' ') || (*p == '\t') || (*p == '\n')))
			p++;
		if (!*p) break;
		if (*p == '\n')
			{
			p++;
			continue;
			}
		*(arg++)=p++;
		num++;
		while (*p && (*p != '\n')) p++;
		if (!*p) break;
		/* else *p == '\n' */
		*(p++)='\0';
		}
	*argc=num;
	return(1);
	}
#endif

int str2fmt(s)
char *s;
	{
	if 	((*s == 'D') || (*s == 'd'))
		return(FORMAT_ASN1);
	else if ((*s == 'T') || (*s == 't'))
		return(FORMAT_TEXT);
	else if ((*s == 'P') || (*s == 'p'))
		return(FORMAT_PEM);
	else if ((*s == 'N') || (*s == 'n'))
		return(FORMAT_NETSCAPE);
	else
		return(FORMAT_UNDEF);
	}

#if defined(MSDOS) || defined(WIN32) || defined(WIN16)
void program_name(in,out,size)
char *in;
char *out;
int size;
	{
	int i,n;
	char *p=NULL;

	n=strlen(in);
	/* find the last '/', '\' or ':' */
	for (i=n-1; i>0; i--)
		{
		if ((in[i] == '/') || (in[i] == '\\') || (in[i] == ':'))
			{
			p= &(in[i+1]);
			break;
			}
		}
	if (p == NULL)
		p=in;
	n=strlen(p);
	/* strip off trailing .exe if present. */
	if ((n > 4) && (p[n-4] == '.') &&
		((p[n-3] == 'e') || (p[n-3] == 'E')) &&
		((p[n-2] == 'x') || (p[n-2] == 'X')) &&
		((p[n-1] == 'e') || (p[n-1] == 'E')))
		n-=4;
	if (n > size-1)
		n=size-1;

	for (i=0; i<n; i++)
		{
		if ((p[i] >= 'A') && (p[i] <= 'Z'))
			out[i]=p[i]-'A'+'a';
		else
			out[i]=p[i];
		}
	out[n]='\0';
	}
#else
void program_name(in,out,size)
char *in;
char *out;
int size;
	{
	char *p;

	p=strrchr(in,'/');
	if (p != NULL)
		p++;
	else
		p=in;
	strncpy(out,p,size-1);
	out[size-1]='\0';
	}
#endif
