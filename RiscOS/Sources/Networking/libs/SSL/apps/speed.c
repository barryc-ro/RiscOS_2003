/* apps/speed.c */
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

/* most of this code has been pilfered from my libdes speed.c program */

#undef SECONDS
#define SECONDS		3
#define RSA_SECONDS	10

/* 11-Sep-92 Andrew Daviel   Support for Silicon Graphics IRIX added */
/* 06-Apr-92 Luke Brennan    Support for VMS and add extra signal calls */

#undef PROG
#define PROG speed_main

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "apps.h"
#include "crypto.h"
#include "err.h"

#if !defined(MSDOS) && !defined(RISCOS)
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

#ifndef RISCOS
#ifndef TIMES
#include <sys/timeb.h>
#endif
#endif

#ifdef sun
#include <limits.h>
#include <sys/param.h>
#endif

#include "des.h"
#include "md2.h"
#include "md5.h"
#include "sha.h"
#ifndef NO_RC4
#include "rc4.h"
#endif
#ifndef NO_RC2
#include "rc2.h"
#endif
#ifndef NO_IDEA
#include "idea.h"
#endif
#include "rsa.h"
#include "x509.h"
#include "./testcert.h"

/* The following if from times(3) man page.  It may need to be changed */
#ifndef HZ
#ifndef CLK_TCK
#ifndef VMS
#define HZ	100.0
#else /* VMS */
#define HZ	100.0
#endif
#else /* CLK_TCK */
#define HZ ((double)CLK_TCK)
#endif
#endif

#undef BUFSIZE
#define BUFSIZE	((long)1024*8)
long run=0;

#ifndef NOPROTO
static double Time_F(int s);
static void print_message(char *s,long num,int length);
static void rsa_print_message(long num,int bits);
#else
static double Time_F();
static void print_message();
static void rsa_print_message();
#endif

#ifdef SIGALRM
#if defined(__STDC__) || defined(sgi)
#define SIGRETTYPE void
#else
#define SIGRETTYPE int
#endif

#ifndef NOPROTO
static SIGRETTYPE sig_done(int sig);
#else
static SIGRETTYPE sig_done();
#endif

static SIGRETTYPE sig_done(sig)
int sig;
	{
	signal(SIGALRM,sig_done);
	run=0;
#ifdef LINT
	sig=sig;
#endif
	}
#endif

#define START	0
#define STOP	1

#define MONOTIME (*(unsigned int*)0x10C)

static double Time_F(s)
int s;
	{
	double ret;
#ifdef TIMES
	static struct tms tstart,tend;

	if (s == START)
		{
		times(&tstart);
		return(0);
		}
	else
		{
		times(&tend);
		ret=((double)(tend.tms_utime-tstart.tms_utime))/HZ;
		return((ret < 0.001)?0.001:ret);
		}
#else /* !times() */
#ifdef RISCOS
        static unsigned int monotime;

        if ( s == START )
            monotime = MONOTIME;
        else
        {
            ret = (double)(0.01*(unsigned int)(MONOTIME-monotime));
            return ( ret < 0.01 ) ? 0.01 : ret;
        }
        return 0;
#else
	static struct timeb tstart,tend;
	long i;

	if (s == START)
		{
		ftime(&tstart);
		return(0);
		}
	else
		{
		ftime(&tend);
		i=(long)tend.millitm-(long)tstart.millitm;
		ret=((double)(tend.time-tstart.time))+((double)i)/1000.0;
		return((ret < 0.001)?0.001:ret);
		}
#endif
#endif
	}

int MAIN(argc,argv)
int argc;
char **argv;
	{
	unsigned char *buf=NULL;
	int ret=1;
#define ALGOR_NUM	12
#define SIZE_NUM	5
#define RSA_NUM		4
	long count;
	int i,j,k;
	unsigned char md2[MD2_DIGEST_LENGTH];
	unsigned char md5[MD5_DIGEST_LENGTH];
	unsigned char sha[SHA_DIGEST_LENGTH];
#ifndef NO_RC4
	RC4_KEY rc4_ks;
#endif
#ifndef NO_RC2
	RC2_KEY rc2_ks;
#endif
#ifndef NO_IDEA
	IDEA_KEY_SCHEDULE idea_ks;
#endif
	static unsigned char key16[16]=
		{0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0,
		 0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0,0x12};
	int cfb_state=0;
	des_cblock iv;
	static des_cblock key ={0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0};
	static des_cblock key2={0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0,0x12};
	static des_cblock key3={0x56,0x78,0x9a,0xbc,0xde,0xf0,0x12,0x34};
	des_key_schedule sch,sch2,sch3;
#define	D_MD2		0
#define	D_MD5		1
#define	D_SHA		2
#define	D_SHA1		3
#define	D_RC4		4
#define	D_CFB_DES	5
#define	D_CBC_DES	6
#define	D_EDE3_DES	7
#define	D_CFB_IDEA	8
#define	D_CBC_IDEA	9
#define	D_CFB_RC2	10
#define	D_CBC_RC2	11
	double d,results[ALGOR_NUM][SIZE_NUM];
	static int lengths[SIZE_NUM]={8,64,256,1024,8192};
	long c[ALGOR_NUM][SIZE_NUM];
	static char *names[ALGOR_NUM]={
		"md2","md5","sha","sha1","rc4",
		"des cfb","des cbc","des ede3","idea cfb","idea cbc",
		"rc2 cfb","rc2 cbc"};
#define	R_RSA_512	0
#define	R_RSA_1024	1
#define	R_RSA_2048	2
#define	R_RSA_4096	3
	RSA *rsa_key[RSA_NUM];
	long rsa_c[RSA_NUM];
	double rsa_results[RSA_NUM];
	static unsigned int rsa_bits[RSA_NUM]={512,1024,2048,4096};
	static unsigned char *rsa_data[RSA_NUM]=
		{test512,test1024,test2048,test4096};
	static int rsa_data_length[RSA_NUM]={
		sizeof(test512),sizeof(test1024),
		sizeof(test2048),sizeof(test4096)};
	unsigned char *p;
	int doit[ALGOR_NUM];
	int rsa_doit[RSA_NUM];
	int pr_header=0;

	apps_startup();

	if (bio_err == NULL)
		if ((bio_err=BIO_new(BIO_s_file())) != NULL)
			BIO_set_fp(bio_err,stderr,BIO_NOCLOSE);

	for (i=0; i<RSA_NUM; i++)
		rsa_key[i]=NULL;

	if ((buf=(unsigned char *)malloc(BUFSIZE)) == NULL)
		{
		fprintf(stderr,"out of memory\n");
		goto end;
		}

	memset(c,0,sizeof(c));
	memset(iv,0,sizeof(iv));

	for (i=0; i<ALGOR_NUM; i++)
		doit[i]=0;
	for (i=0; i<RSA_NUM; i++)
		rsa_doit[i]=0;

	j=0;
	argc--;
	argv++;
	while (argc)
		{
		if	(strcmp(*argv,"md2") == 0) doit[D_MD2]=1;
		else if (strcmp(*argv,"md5") == 0) doit[D_MD5]=1;
		else if (strcmp(*argv,"sha") == 0) doit[D_SHA]=1;
		else if (strcmp(*argv,"sha1") == 0) doit[D_SHA1]=1;
#ifndef NO_RC4
		else if (strcmp(*argv,"rc4") == 0) doit[D_RC4]=1;
#endif
		else if (strcmp(*argv,"des-cfb") == 0) doit[D_CFB_DES]=1;
		else if (strcmp(*argv,"des-cbc") == 0) doit[D_CBC_DES]=1;
		else if (strcmp(*argv,"des-ede3") == 0) doit[D_EDE3_DES]=1;
		else if (strcmp(*argv,"rsa512") == 0) rsa_doit[R_RSA_512]=2;
		else if (strcmp(*argv,"rsa1024") == 0) rsa_doit[R_RSA_1024]=2;
		else if (strcmp(*argv,"rsa2048") == 0) rsa_doit[R_RSA_2048]=2;
		else if (strcmp(*argv,"rsa4096") == 0) rsa_doit[R_RSA_4096]=2;
#ifndef NO_RC2
		else if (strcmp(*argv,"rc2-cfb") == 0) doit[D_CFB_RC2]=1;
		else if (strcmp(*argv,"rc2-cbc") == 0) doit[D_CBC_RC2]=1;
		else if (strcmp(*argv,"rc2") == 0)
			{
			doit[D_CFB_RC2]=1;
			doit[D_CBC_RC2]=1;
			}
#endif
#ifndef NO_IDEA
		else if (strcmp(*argv,"idea-cfb") == 0) doit[D_CFB_IDEA]=1;
		else if (strcmp(*argv,"idea-cbc") == 0) doit[D_CBC_IDEA]=1;
		else if (strcmp(*argv,"idea") == 0)
			{
			doit[D_CFB_IDEA]=1;
			doit[D_CBC_IDEA]=1;
			}
#endif
		else if (strcmp(*argv,"des") == 0)
			{
			doit[D_CFB_DES]=1;
			doit[D_CBC_DES]=1;
			doit[D_EDE3_DES]=1;
			}
		else if (strcmp(*argv,"rsa") == 0)
			{
			rsa_doit[R_RSA_512]=1;
			rsa_doit[R_RSA_1024]=1;
			rsa_doit[R_RSA_2048]=1;
			rsa_doit[R_RSA_4096]=1;
			}
		else
			{
			fprintf(stderr,"bad value, pick one of\n");
			fprintf(stderr,"md2      md5      sha      sha1\n");
#ifndef NO_IDEA
			fprintf(stderr,"idea-cfb idea-cbc ");
#endif
#ifndef NO_RC2
			fprintf(stderr,"rc2-cfb  rc2-cbc");
#endif
#if !(defined(NO_IDEA) && defined(NO_RC2))
			fprintf(stderr,"\n");
#endif
			fprintf(stderr,"des-cfb  des-cbc  des-ede3 ");
#ifndef NO_RC4
			fprintf(stderr,"rc4");
#endif
			fprintf(stderr,"\nrsa512   rsa1024  rsa2048  rsa4096\n");
			fprintf(stderr,"idea     rc2      des      rsa\n");
			goto end;
			}
		argc--;
		argv++;
		j++;
		}

	if (j == 0)
		{
		for (i=0; i<ALGOR_NUM; i++)
			doit[i]=1;
		for (i=0; i<RSA_NUM; i++)
			rsa_doit[i]=1;
		}
	for (i=0; i<ALGOR_NUM; i++)
		if (doit[i]) pr_header++;

#ifndef TIMES
	fprintf(stderr,"To get the most accurate results, try to run this\n");
	fprintf(stderr,"program when this computer is idle.\n");
#endif

	for (i=0; i<RSA_NUM; i++)
		{
		p=rsa_data[i];
		rsa_key[i]=d2i_RSAPrivateKey(NULL,&p,rsa_data_length[i]);
		if (rsa_key[i] == NULL)
			{
			fprintf(stderr,"internal error loading RSA key number %d\n",i);
			goto end;
			}
		}
	des_set_key((C_Block *)key,sch);
	des_set_key((C_Block *)key2,sch2);
	des_set_key((C_Block *)key3,sch3);
#ifndef NO_IDEA
	idea_set_encrypt_key(key16,&idea_ks);
#endif
#ifndef NO_RC4
	RC4_set_key(&rc4_ks,16,key16);
#endif
#ifndef NO_RC2
	RC2_set_key(&rc2_ks,16,key16);
#endif

#ifndef SIGALRM
	fprintf(stderr,"First we calculate the approximate speed ...\n");
	count=10;
	do	{
		long i;
		count*=2;
		Time_F(START);
		for (i=count; i; i--)
			des_ecb_encrypt((C_Block *)buf,(C_Block *)buf,
				&(sch[0]),DES_ENCRYPT);
		d=Time_F(STOP);
		} while (d <3);
	c[D_MD2][0]=count/10;
	c[D_MD5][0]=count;
	c[D_SHA][0]=count;
	c[D_SHA1][0]=count;
	c[D_RC4][0]=count*5;
	c[D_CFB_DES][0]=count;
	c[D_CBC_DES][0]=count;
	c[D_EDE3_DES][0]=count/3;
	c[D_CBC_IDEA][0]=count;
	c[D_CFB_IDEA][0]=count;
	c[D_CBC_RC2][0]=count;
	c[D_CFB_RC2][0]=count;

	for (i=1; i<SIZE_NUM; i++)
		{
		c[D_MD2][i]=c[D_MD2][0]*4*lengths[0]/lengths[i];
		c[D_MD5][i]=c[D_MD5][0]*4*lengths[0]/lengths[i];
		c[D_SHA][i]=c[D_SHA][0]*4*lengths[0]/lengths[i];
		c[D_SHA1][i]=c[D_SHA1][0]*4*lengths[0]/lengths[i];
		}
	for (i=1; i<SIZE_NUM; i++)
		{
		long l0,l1;

		l0=(long)lengths[i-1];
		l1=(long)lengths[i];
		c[D_RC4][i]=c[D_RC4][i-1]*l0/l1;
		c[D_CFB_DES][i]=c[D_CFB_DES][i-1]*l0/l1;
		c[D_CBC_DES][i]=c[D_CBC_DES][i-1]*l0/l1;
		c[D_EDE3_DES][i]=c[D_EDE3_DES][i-1]*l0/l1;
		c[D_CBC_IDEA][i]=c[D_CBC_IDEA][i-1]*l0/l1;
		c[D_CFB_IDEA][i]=c[D_CFB_IDEA][i-1]*l0/l1;
		c[D_CBC_RC2][i]=c[D_CBC_RC2][i-1]*l0/l1;
		c[D_CFB_RC2][i]=c[D_CFB_RC2][i-1]*l0/l1;
		}
	rsa_c[R_RSA_512]=count/2000;
	for (i=1; i<RSA_NUM; i++)
		{
		rsa_c[i]=rsa_c[i-1]/8;
		if ((rsa_doit[i] <= 1) && (rsa_c[i] == 0))
			rsa_doit[i]=0;
		else
			{ if (rsa_c[i] == 0) rsa_c[i]=1; }
		}
#define COND(d)	(count != (d))
#define COUNT(d) (d)
#else
#define COND(c)	(run)
#define COUNT(d) (count)
	signal(SIGALRM,sig_done);
#endif

	if (doit[D_MD2])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_MD2],c[D_MD2][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_MD2][j]); count++)
				MD2(buf,(unsigned long)lengths[j],&(md2[0]));
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_MD2],d);
			results[D_MD2][j]=((double)count)/d*lengths[j];
			}
		}

	if (doit[D_MD5])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_MD5],c[D_MD5][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_MD5][j]); count++)
				MD5(buf,(unsigned long)lengths[j],&(md5[0]));
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_MD5],d);
			results[D_MD5][j]=((double)count)/d*lengths[j];
			}
		}

	if (doit[D_SHA])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_SHA],c[D_SHA][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_SHA][j]); count++)
				SHA(buf,(unsigned long)lengths[j],&(sha[0]));
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_SHA],d);
			results[D_SHA][j]=((double)count)/d*lengths[j];
			}
		}

	if (doit[D_SHA1])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_SHA1],c[D_SHA1][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_SHA1][j]); count++)
				SHA1(buf,(unsigned long)lengths[j],&(sha[0]));
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_SHA1],d);
			results[D_SHA1][j]=((double)count)/d*lengths[j];
			}
		}

#ifndef NO_RC4
	if (doit[D_RC4])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_RC4],c[D_RC4][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_RC4][j]); count++)
				RC4(&rc4_ks,(unsigned int)lengths[j],
					buf,buf);
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_RC4],d);
			results[D_RC4][j]=((double)count)/d*lengths[j];
			}
		}
#endif

	if (doit[D_CFB_DES])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_CFB_DES],c[D_CFB_DES][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_CFB_DES][j]); count++)
				des_cfb64_encrypt(buf,buf,(long)lengths[j],sch,
					(C_Block *)&(iv[0]),&cfb_state,DES_ENCRYPT);
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_CFB_DES],d);
			results[D_CFB_DES][j]=((double)count)/d*lengths[j];
			}
		}

	if (doit[D_CBC_DES])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_CBC_DES],c[D_CBC_DES][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_CBC_DES][j]); count++)
				des_ncbc_encrypt((C_Block *)buf,
					(C_Block *)buf,
					(long)lengths[j],sch,
					(C_Block *)&(iv[0]),DES_ENCRYPT);
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_CBC_DES],d);
			results[D_CBC_DES][j]=((double)count)/d*lengths[j];
			}
		}

	if (doit[D_EDE3_DES])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_EDE3_DES],c[D_EDE3_DES][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_EDE3_DES][j]); count++)
				des_ede3_cbc_encrypt((C_Block *)buf,
					(C_Block *)buf,
					(long)lengths[j],sch,sch2,sch3,
					(C_Block *)&(iv[0]),DES_ENCRYPT);
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_EDE3_DES],d);
			results[D_EDE3_DES][j]=((double)count)/d*lengths[j];
			}
		}

#ifndef NO_IDEA
	if (doit[D_CFB_IDEA])
		{
		int v=0;

		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_CFB_IDEA],c[D_CFB_IDEA][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_CFB_IDEA][j]); count++)
				idea_cfb64_encrypt(buf,buf,
					(unsigned long)lengths[j],&idea_ks,
					(unsigned char *)&(iv[0]),
					&v,IDEA_ENCRYPT);
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_CFB_IDEA],d);
			results[D_CFB_IDEA][j]=((double)count)/d*lengths[j];
			}
		}

	if (doit[D_CBC_IDEA])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_CBC_IDEA],c[D_CBC_IDEA][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_CBC_IDEA][j]); count++)
				idea_cbc_encrypt(buf,buf,
					(unsigned long)lengths[j],&idea_ks,
					(unsigned char *)&(iv[0]),IDEA_ENCRYPT);
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_CBC_IDEA],d);
			results[D_CBC_IDEA][j]=((double)count)/d*lengths[j];
			}
		}
#endif
#ifndef NO_RC2
	if (doit[D_CFB_RC2])
		{
		int v=0;

		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_CFB_RC2],c[D_CFB_RC2][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_CFB_RC2][j]); count++)
				RC2_cfb64_encrypt(buf,buf,
					(unsigned long)lengths[j],&rc2_ks,
					(unsigned char *)&(iv[0]),
					&v,RC2_ENCRYPT);
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_CFB_RC2],d);
			results[D_CFB_RC2][j]=((double)count)/d*lengths[j];
			}
		}

	if (doit[D_CBC_RC2])
		{
		for (j=0; j<SIZE_NUM; j++)
			{
			print_message(names[D_CBC_RC2],c[D_CBC_RC2][j],lengths[j]);
			Time_F(START);
			for (count=0,run=1; COND(c[D_CBC_RC2][j]); count++)
				RC2_cbc_encrypt(buf,buf,
					(unsigned long)lengths[j],&rc2_ks,
					(unsigned char *)&(iv[0]),RC2_ENCRYPT);
			d=Time_F(STOP);
			fprintf(stderr,"%ld %s's in %.2fs\n",
				count,names[D_CBC_RC2],d);
			results[D_CBC_RC2][j]=((double)count)/d*lengths[j];
			}
		}
#endif

	for (j=0; j<RSA_NUM; j++)
		{
		if (!rsa_doit[j]) continue;
		rsa_print_message(rsa_c[j],rsa_bits[j]);
		Time_F(START);
		for (count=0,run=1; COND(rsa_c[j]); count++)
			{
			if (RSA_private_encrypt(16,buf,buf,rsa_key[j]) < 0)
				{
				fprintf(stderr,"RSA private encrypt failure\n");
				ERR_print_errors(bio_err);
				count=1;
				break;
				}
			}
		d=Time_F(STOP);
		fprintf(stderr,"%ld %d bit RSA's in %.2fs\n",
			count,rsa_bits[j],d);
		rsa_results[j]=d/(double)count;
		if (count <= 1)
			{
			/* if longer than 10s, don't do any more */
			for (j++; j<RSA_NUM; j++)
				rsa_doit[j]=0;
			}
		}

	fprintf(stdout,"%s\n",SSLeay_version(SSLEAY_VERSION));
        fprintf(stdout,"%s\n",SSLeay_version(SSLEAY_BUILT_ON));
	fprintf(stdout,"%s\n",SSLeay_version(SSLEAY_OPTIONS));
	fprintf(stdout,"%s\n",SSLeay_version(SSLEAY_CFLAGS));

	if (pr_header)
		{
		fprintf(stdout,"The 'numbers' are in 1000s of bytes per second processed.\n");
		fprintf(stdout,"type     ");
		for (j=0;  j<SIZE_NUM; j++)
			fprintf(stdout,"%7d bytes",lengths[j]);
		fprintf(stdout,"\n");
		}

	for (k=0; k<ALGOR_NUM; k++)
		{
		if (!doit[k]) continue;
		fprintf(stdout,"%-9s",names[k]);
		for (j=0; j<SIZE_NUM; j++)
			{
			if (results[k][j] > 10000)
				fprintf(stdout," %11.2fk",results[k][j]/1000.0);
			else
				fprintf(stdout," %11.2f ",results[k][j]);
			}
		fprintf(stdout,"\n");
		}
	for (k=0; k<RSA_NUM; k++)
		{
		if (!rsa_doit[k]) continue;
		fprintf(stdout,"rsa %4d bits %7.3fs",
			rsa_bits[k],rsa_results[k]);
		fprintf(stdout,"\n");
		}
	ret=0;
end:
	if (buf != NULL) free(buf);
	for (i=0; i<RSA_NUM; i++)
		if (rsa_key[i] != NULL)
			RSA_free(rsa_key[i]);
	EXIT(ret);
	}

static void print_message(s,num,length)
char *s;
long num;
int length;
	{
#ifdef SIGALRM
	fprintf(stderr,"Doing %s for %ds on %d size blocks: ",s,SECONDS,length);
	fflush(stderr);
	alarm(SECONDS);
#else
	fprintf(stderr,"Doing %s %ld times on %d size blocks: ",s,num,length);
	fflush(stderr);
#endif
#ifdef LINT
	num=num;
#endif
	}

static void rsa_print_message(num,bits)
long num;
int bits;
	{
#ifdef SIGALRM
	fprintf(stderr,"Doing %d bit rsa's for %ds: ",bits,RSA_SECONDS);
	fflush(stderr);
	alarm(RSA_SECONDS);
#else
	fprintf(stderr,"Doing %ld %d bit rsa's: ",num,bits);
	fflush(stderr);
#endif
#ifdef LINT
	num=num;
#endif
	}
