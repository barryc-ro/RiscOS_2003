/* apps/enc.c */
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
#include "apps.h"
#include "buffer.h"
#include "err.h"
#include "envelope.h"
#include "objects.h"
#include "x509.h"
#include "md5.h"
#include "pem.h"

#ifndef NOPROTO
int set_hex(char *in,unsigned char *out,int size);
#else
int set_hex();
#endif

#undef SIZE
#undef BSIZE
#undef PROG

#define SIZE	(512)
#define BSIZE	(8*1024)
#define	PROG	enc_main

int MAIN(argc,argv)
int argc;
char **argv;
	{
	char *strbuf=NULL;
	EVP_ENCODE_CTX basein,baseout;
	EVP_CIPHER_CTX p;
	unsigned char *buff=NULL,*buff2=NULL,*btmp,*bufsize=NULL;
	int bsize=BSIZE,verbose=0;
	int num_in=0,num_out=0;
	int ret=1,inl,outl,donesome=0;
	unsigned char key[24],iv[MD5_DIGEST_LENGTH];
	char *str=NULL;
	char *hkey=NULL,*hiv=NULL;
	int enc=1,printkey=0,i,base64=0,done=0;
	EVP_CIPHER *cipher=NULL,*c;
	char *inf=NULL,*outf=NULL;
	BIO *in=NULL,*out=NULL;
#define PROG_NAME_SIZE  16
        char pname[PROG_NAME_SIZE];


	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	/* first check the program name */
        program_name(argv[0],pname,PROG_NAME_SIZE);
	if (strcmp(pname,"base64") == 0)
		base64=1;

	cipher=EVP_get_cipherbyname(pname);

	argc--;
	argv++;
	while (argc >= 1)
		{
		if	(strcmp(*argv,"-e") == 0)
			enc=1;
		else if (strcmp(*argv,"-in") == 0)
			{
			if (--argc < 1) goto bad;
			inf= *(++argv);
			}
		else if (strcmp(*argv,"-out") == 0)
			{
			if (--argc < 1) goto bad;
			outf= *(++argv);
			}
		else if	(strcmp(*argv,"-d") == 0)
			enc=0;
		else if	(strcmp(*argv,"-p") == 0)
			printkey=1;
		else if	(strcmp(*argv,"-v") == 0)
			verbose=1;
		else if	(strcmp(*argv,"-P") == 0)
			printkey=2;
		else if	(strcmp(*argv,"-a") == 0)
			base64=1;
		else if	(strcmp(*argv,"-base64") == 0)
			base64=1;
		else if (strcmp(*argv,"-bufsize") == 0)
			{
			if (--argc < 1) goto bad;
			bufsize=(unsigned char *)*(++argv);
			}
		else if (strcmp(*argv,"-k") == 0)
			{
			if (--argc < 1) goto bad;
			str=*(++argv);
			}
		else if (strcmp(*argv,"-K") == 0)
			{
			if (--argc < 1) goto bad;
			hkey= *(++argv);
			}
		else if (strcmp(*argv,"-iv") == 0)
			{
			if (--argc < 1) goto bad;
			hiv= *(++argv);
			}
		else if	((argv[0][0] == '-') &&
			((c=EVP_get_cipherbyname(&(argv[0][1]))) != NULL))
			{
			cipher=c;
			}
		else if (strcmp(*argv,"-none") == 0)
			cipher=NULL;
		else
			{
			fprintf(stderr,"unknown option '%s'\n",*argv);
bad:
			fprintf(stderr,"options are\n");
			fprintf(stderr,"%-14s input file\n","-in <file>");
			fprintf(stderr,"%-14s output fileencrypt\n","-out <file>");
			fprintf(stderr,"%-14s encrypt\n","-e");
			fprintf(stderr,"%-14s decrypt\n","-d");
			fprintf(stderr,"%-14s base64 encode/decode, depending on encryption flag\n","-a/-base64");
			fprintf(stderr,"%-14s key is the next argument\n","-k");
			fprintf(stderr,"%-14s key/iv in hex is the next argument\n","-K/-iv");
			fprintf(stderr,"%-14s print the iv/key (then exit if -P)\n","-[pP]");
			fprintf(stderr,"%-14s buffer size\n","-bufsize <n>");

			fprintf(stderr,"Cipher Types\n");
			fprintf(stderr,"des     : 56 bit key DES encryption\n");
			fprintf(stderr,"des_ede :112 bit key ede DES encryption\n");
			fprintf(stderr,"des_ede3:168 bit key ede DES encryption\n");
#ifndef NO_IDEA
			fprintf(stderr,"idea    :128 bit key IDEA encryption\n");
#endif
#ifndef NO_RC4
			fprintf(stderr,"rc2     :128 bit key RC2 encryption\n");
#endif
#ifndef NO_RC4
			fprintf(stderr," -%-5s :128 bit key RC4 encryption\n",
				LN_rc4);
#endif

			fprintf(stderr," -%-12s -%-12s -%-12s -%-12s",
				LN_des_ecb,LN_des_cbc,LN_des_cfb,LN_des_ofb);
			fprintf(stderr," -%-4s (%s)\n",
				"des", LN_des_cbc);

			fprintf(stderr," -%-12s -%-12s -%-12s -%-12s",
				LN_des_ede,LN_des_ede_cbc,
				LN_des_ede_cfb,LN_des_ede_ofb);
			fprintf(stderr," -none\n");


			fprintf(stderr," -%-12s -%-12s -%-12s -%-12s",
				LN_des_ede3,LN_des_ede3_cbc,
				LN_des_ede3_cfb,LN_des_ede3_ofb);
			fprintf(stderr," -%-4s (%s)\n",
				"des3", LN_des_ede3_cbc);

#ifndef NO_IDEA
			fprintf(stderr," -%-12s -%-12s -%-12s -%-12s",
				LN_idea_ecb, LN_idea_cbc,
				LN_idea_cfb, LN_idea_ofb);
			fprintf(stderr," -%-4s (%s)\n","idea",LN_idea_cbc);
#endif
#ifndef NO_RC2
			fprintf(stderr," -%-12s -%-12s -%-12s -%-12s",
				LN_rc2_ecb, LN_rc2_cbc,
				LN_rc2_cfb, LN_rc2_ofb);
			fprintf(stderr," -%-4s (%s)\n","rc2", LN_rc2_cbc);
#endif
			goto end;
			}
		argc--;
		argv++;
		}

	if (bufsize != NULL)
		{
		int i;
		unsigned long n;

		for (n=0; *bufsize; bufsize++)
			{
			i=*bufsize;
			if ((i <= '9') && (i >= '0'))
				n=n*10+'i'-'0';
			else if (i == 'k')
				{
				n*=1024;
				bufsize++;
				break;
				}
			}
		if (*bufsize != '\0')
			{
			fprintf(stderr,"invalid 'bufsize' specified.\n");
			goto end;
			}

		/* It must be large enough for a base64 encoded line */
		if (n < 80) n=80;

		bsize=(int)n;
		if (verbose) fprintf(stderr,"bufsize=%d\n",bsize);
		}

	strbuf=malloc(SIZE);
	buff=(unsigned char *)malloc(EVP_ENCODE_LENGTH(bsize));
	buff2=(unsigned char *)malloc(EVP_ENCODE_LENGTH(bsize));
	if ((buff == NULL) || (buff2 == NULL) || (strbuf == NULL))
		{
		fprintf(stderr,"malloc failure\n");
		goto end;
		}

	in=BIO_new(BIO_s_file());
	out=BIO_new(BIO_s_file());
	if ((in == NULL) || (out == NULL))
		{
		ERR_print_errors(bio_err);
		goto end;
		}

	if (inf == NULL)
		BIO_set_fp(in,stdin,BIO_NOCLOSE);
	else
		{
		if (BIO_read_filename(in,inf) <= 0)
			{
			perror(inf);
			goto end;
			}
		}

	if ((str == NULL) && (cipher != NULL) && (hkey == NULL))
		{
		for (;;)
			{
			char buf[200];

			sprintf(buf,"enter %s %s password:",
				OBJ_nid2ln(cipher->type),
				(enc)?"encryption":"decryption");
			strbuf[0]='\0';
			i=EVP_read_pw_string((char *)strbuf,SIZE,buf,enc);
			if (i == 0)
				{
				if (strbuf[0] == '\0')
					{
					ret=1;
					goto end;
					}
				str=strbuf;
				break;
				}
			if (i < 0)
				{
				fprintf(stderr,"bad password read\n");
				goto end;
				}
			}
		}

	if (cipher != NULL)
		{
		if (str != NULL)
			{
			EVP_BytesToKey(cipher,EVP_md5(),NULL,
				(unsigned char *)str,
				strlen(str),1,key,iv);
			/* zero the complete buffer or the string
			 * passed from the command line
			 * bug picked up by
			 * Larry J. Hughes Jr. <hughes@indiana.edu> */
			if (str == strbuf)
				memset(str,0,SIZE);
			else
				memset(str,0,strlen(str));
			}
		if ((hiv != NULL) && !set_hex(hiv,iv,8))
			{
			fprintf(stderr,"invalid hex iv value\n");
			goto end;
			}
		if ((hkey != NULL) && !set_hex(hkey,key,24))
			{
			fprintf(stderr,"invalid hex key value\n");
			goto end;
			}
		EVP_CipherInit(&p,cipher,key,iv,enc);

		if (printkey)
			{
			if (cipher->key_len > 0)
				{
				printf("key=");
				for (i=0; i<cipher->key_len; i++)
					printf("%02X",key[i]);
				printf("\n");
				}
			if (cipher->iv_len > 0)
				{
				printf("iv =");
				for (i=0; i<cipher->iv_len; i++)
					printf("%02X",iv[i]);
				printf("\n");
				}
			if (printkey == 2)
				{
				ret=0;
				goto end;
				}
			}
		}


	if (outf == NULL)
		BIO_set_fp(out,stdout,BIO_NOCLOSE);
	else
		{
		if (BIO_write_filename(out,outf) <= 0)
			{
			perror(outf);
			goto end;
			}
		}

	if (base64)
		{
		if (enc)
			EVP_EncodeInit(&baseout);
		else
			EVP_DecodeInit(&basein);
		}

	for (;;)
		{
		/* While we have not started base64 decoding,
		 * read a line at a time until we hit a valid line */
		if (base64 && !enc && !donesome)
			{
			buff[0]='\0';
			inl=BIO_gets(in,(char *)buff,bsize);
			if (buff[0] == '\0') break;
			}
		else
			inl=BIO_read(in,(char *)buff,bsize);

		if (inl == 0) break;
		num_in+=inl;
		outl=inl;

		if (base64 && !enc)
			{
			i=EVP_DecodeUpdate(&basein,buff2,&outl,buff,outl);
			if ((i <= 0) && !donesome)
				{
				EVP_DecodeInit(&basein);
				continue;
				}
			donesome=1;
			if (i <= 0) done=1;
			btmp=buff2; buff2=buff; buff=btmp;
			}

		if (cipher != NULL)
			{
			EVP_CipherUpdate(&p,buff2,&outl,buff,outl);
			btmp=buff2; buff2=buff; buff=btmp;
			}

		if (base64 && enc)
			{
			EVP_EncodeUpdate(&baseout,buff2,&outl,buff,outl);
			btmp=buff2; buff2=buff; buff=btmp;
			}

		num_out+=outl;
		if (outl != 0)
			BIO_write(out,(char *)buff,outl);
		outl=0;
		if (done) break;
		}

	/* Finish off any pending base64 input */
	if (base64 && !enc)
		{
		EVP_DecodeFinal(&basein,buff2,&outl);
		btmp=buff2; buff2=buff; buff=btmp;

		if ((cipher != NULL) && (outl != 0))
			{
			EVP_CipherUpdate(&p,buff2,&outl,buff,outl);
			btmp=buff2; buff2=buff; buff=btmp;
			}
		num_out+=outl;
		if (outl != 0)
			BIO_write(out,(char *)buff,outl);
		outl=0;
		}

	/* Finish off any pending cipher input */
	if (cipher != NULL)
		{
		if (!EVP_CipherFinal(&p,buff,&outl))
			{
			fprintf(stderr,"error in EVP_CipherFinal\n");
			goto end;
			}
		memset(&p,0,sizeof(p));
		if (base64 && enc)
			{
			EVP_EncodeUpdate(&baseout,buff2,&outl,buff,outl);
			btmp=buff2; buff2=buff; buff=btmp;
			num_out+=outl;
			if (outl != 0)
				BIO_write(out,(char *)buff,outl);
			outl=0;
			}
		}

	/* Finish off any pending base64 output */
	if (base64 && enc)
		{
		EVP_EncodeFinal(&baseout,buff2,&outl);
		btmp=buff2; buff2=buff; buff=btmp;
		}

	/* Finish off any pending output */
	num_out+=outl;
	if (outl != 0)
		BIO_write(out,(char *)buff,outl);

	ret=0;
	if (verbose)
		{
		fprintf(stderr,"bytes read   :%8d\n",num_in);
		fprintf(stderr,"bytes written:%8d\n",num_out);
		}
end:
	if (strbuf != NULL) free(strbuf);
	if (buff != NULL) free(buff);
	if (buff2 != NULL) free(buff2);
	if (in != NULL) BIO_free(in);
	if (out != NULL) BIO_free(out);
	EXIT(ret);
	}

int set_hex(in,out,size)
char *in;
unsigned char *out;
int size;
	{
	int i,n;
	unsigned char j;

	n=strlen(in);
	if (n > (size*2))
		{
		fprintf(stderr,"hex string is too long\n");
		return(0);
		}
	memset(out,0,size);
	for (i=0; i<n; i++)
		{
		j=(unsigned char)*in;
		*(in++)='\0';
		if (j == 0) break;
		if ((j >= '0') && (j <= '9'))
			j-='0';
		else if ((j >= 'A') && (j <= 'F'))
			j=j-'A'+10;
		else if ((j >= 'a') && (j <= 'f'))
			j=j-'a'+10;
		else
			{
			fprintf(stderr,"non-hex digit\n");
			return(0);
			}
		if (i&1)
			out[i/2]|=j;
		else
			out[i/2]=(j<<4);
		}
	return(1);
	}
