/* bugs/sgiccbug.c */
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

/* sgibug.c */
/* bug found by Eric Young (eay@mincom.oz.au) May 95 */

#include <stdio.h>

/* This compiler bug it present on IRIX 5.3, 5.1 and 4.0.5 (these are
 * the only versions of IRIX I have access to.
 * defining FIXBUG removes the bug.
 */
 
/* Compare the output from
 * cc sgiccbug.c; ./a.out
 * and
 * cc -O sgiccbug.c; ./a.out
 */

static unsigned long a[4]={0x01234567,0x89ABCDEF,0xFEDCBA98,0x76543210};
static unsigned long b[4]={0x89ABCDEF,0xFEDCBA98,0x76543210,0x01234567};
static unsigned long c[4]={0x77777778,0x8ACF1357,0x88888888,0x7530ECA9};

main()
	{
	unsigned long r[4];
	sub(r,a,b);
	fprintf(stderr,"input a= %08X %08X %08X %08X\n",a[3],a[2],a[1],a[0]);
	fprintf(stderr,"input b= %08X %08X %08X %08X\n",b[3],b[2],b[1],b[0]);
	fprintf(stderr,"output = %08X %08X %08X %08X\n",r[3],r[2],r[1],r[0]);
	fprintf(stderr,"correct= %08X %08X %08X %08X\n",c[3],c[2],c[1],c[0]);
	}

int sub(r,a,b)
unsigned long *r,*a,*b;
	{
	register unsigned long t1,t2,*ap,*bp,*rp;
	int i,carry;
#ifdef FIXBUG
	unsigned long dummy;
#endif

	ap=a;
	bp=b;
	rp=r;
	carry=0;
	for (i=0; i<4; i++)
		{
		t1= *(ap++);
		t2= *(bp++);
		t1=(t1-t2);
#ifdef FIXBUG
		dummy=t1;
#endif
		*(rp++)=t1&0xffffffff;
		}
	}
