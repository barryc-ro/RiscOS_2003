/* crypto/rand/randfile.c */
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
#include "cryptlib.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "rand.h"

#ifdef RISCOS
#include "kernel.h"
#endif

#define BUFSIZE	1024
#define RAND_DATA 1024

/* #define RFILE ".rand" - defined in ../../e_os.h */

int RAND_load_file(file,bytes)
char *file;
long bytes;
	{
	unsigned char buf[BUFSIZE];
#ifdef RISCOS
        _kernel_osfile_block sb;
#else
	struct stat sb;
#endif
	int i,ret=0;
	FILE *in;

	if (file == NULL) return(0);

#ifdef RISCOS
        i = ((_kernel_osfile( 5, file, &sb ) & (~2)) == 1) ? 1 : -1;
#else
	i=stat(file,&sb);
#endif
	/* If the state fails, put some crap in anyway */
	RAND_seed((unsigned char *)&sb,sizeof(sb));
	ret+=sizeof(sb);
	if (i < 0) return(0);
	if (bytes <= 0) return(ret);

	in=fopen(file,"r");
	if (in == NULL) goto err;
	for (;;)
		{
		i=fread(buf,1,BUFSIZE,in);
		if (i <= 0) break;
		/* even if BUSIZE != i, use the full array */
		RAND_seed(buf,BUFSIZE);
		ret+=i;
		if (i > bytes) break;
		bytes-=i;
		}
	fclose(in);
	memset(buf,0,BUFSIZE);
err:
	return(ret);
	}

int RAND_write_file(file)
char *file;
	{
	unsigned char buf[BUFSIZE];
	int i,ret=0;
	FILE *out;
	int n;

	out=fopen(file,"w");
	if (out == NULL) goto err;
	n=RAND_DATA;
	for (;;)
		{
		i=(n > BUFSIZE)?BUFSIZE:n;
		n-=BUFSIZE;
		RAND_bytes(buf,i);
		i=fwrite(buf,1,i,out);
		if (i <= 0)
			{
			ret=0;
			break;
			}
		ret+=i;
		if (n <= 0) break;
		}
	fclose(out);
	memset(buf,0,BUFSIZE);
	chmod(file,0600);
err:
	return(ret);
	}

char *RAND_file_name(buf,size)
char *buf;
int size;
	{
	char *s;
	char *ret=NULL;

	s=getenv("RANDFILE");
	if (s != NULL)
		{
		strncpy(buf,s,size-1);
		buf[size-1]='\0';
		ret=buf;
		}
	else
		{
		s=getenv("HOME");
		if (s == NULL) return(RFILE);
		if (((int)(strlen(s)+strlen(RFILE)+2)) > size)
			return(RFILE);
		strcpy(buf,s);
		strcat(buf,"/");
		strcat(buf,RFILE);
		ret=buf;
		}
	return(ret);
	}
