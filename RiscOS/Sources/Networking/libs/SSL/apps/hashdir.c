/* apps/hashdir.c */
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
#include <string.h>
#include <time.h>
#include "apps.h"
#include "err.h"
#include "rsa.h"
#include "meth.h"
#include "x509.h"
#include "pem.h"

#undef PROG
#define PROG	hashdir_main

/* This program reads a set of PEM input files and then outputs
 * according to the mode */

/* -write	write the files
 * -link	symlink the files
 * -alias	generate alias file
 * -sub		hash by subject
 * -iss		hash by issuer
 * -ias		hash by issuer and serial
 * -text	print the details for each entry
 */

int MAIN(argc, argv)
int argc;
char **argv;
	{
	int ret=1;
	int i,badops=0;
	int print=0,write=0,link=0,text=0;
	int sub=0,iss=0,alias=0,ias=0;
	char *prog;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	prog=argv[0];
	argc--;
	argv++;
	while (argc >= 1)
		{
		if	(*argv[0] != '-')
			break;
		else if	(strcmp(*argv,"-write") == 0)
			write=1;
		else if (strcmp(*argv,"-link") == 0)
			link=1;
		else if (strcmp(*argv,"-alias") == 0)
			{
			alias=alias;
			sub=1;
			}
		else if (strcmp(*argv,"-sub") == 0)
			sub=1;
		else if (strcmp(*argv,"-iss") == 0)
			iss=1;
		else if (strcmp(*argv,"-ias") == 0)
			ias=1;
		else if (strcmp(*argv,"-text") == 0)
			text=1;
		else if (strcmp(*argv,"-print") == 0)
			print=1;
		else
			{
			fprintf(stderr,"unknown option %s\n",*argv);
			badops=1;
			break;
			}
		argc--;
		argv++;
		}

	if (badops)
		{
		fprintf(stderr,"%s [options] <filese\n",prog);
		fprintf(stderr,"where options are\n");
		fprintf(stderr," -write	 rewrite the files\n");
		fprintf(stderr," -link	 symlink the files\n");
		fprintf(stderr," -text	 print shell commands\n");
		fprintf(stderr," -print	 print details\n");
		fprintf(stderr,"\n");
		fprintf(stderr," -alias	 generate aliases (does -sub)\n");
		fprintf(stderr," -sub	 do for 'subject' lookup\n");
		fprintf(stderr," -iss	 do for 'issuer' lookup\n");
		fprintf(stderr," -ias	 do for 'issuer and serial' lookup\n");
		goto end;
		}

	ERR_load_crypto_strings();

	for (i=0; i<argc; i++)
		{
		printf("%s\n",argv[i]);
		}
	ret=0;
end:
	EXIT(ret);
	}
