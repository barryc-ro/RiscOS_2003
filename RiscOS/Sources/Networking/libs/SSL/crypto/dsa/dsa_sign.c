/* crypto/dsa/dsa_sign.c */
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

/* lib/dsa/dsa_sign.c */
/* Copyright (C) 1995 Eric Young (eay@mincom.oz.au)
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

/* Origional version from Steven Schoch <schoch@sheba.arc.nasa.gov> */

#include <stdio.h>
#include "cryptlib.h"
#include "bn.h"
#include "dsa.h"
#include "rand.h"
#include "asn1.h"

/* data has already been hashed (probably with SHA or SHA-1). */
/*	DSAerr(DSA_F_DSA_SIGN,DSA_R_DATA_TO_LARGE_FOR_KEY_SIZE); */

int DSA_sign(type,dgst,dlen,sig,siglen,dsa)
int type;
unsigned char *dgst;
int dlen;
unsigned char *sig;	/* out */
unsigned int *siglen;	/* out */
DSA *dsa;
	{
	BIGNUM *m=NULL;
	BIGNUM *k=NULL,*kinv=NULL,*r=NULL,*s=NULL;
	BN_CTX *ctx;
	unsigned char *p;
	int i,len=0,ret=0,reason=ERR_R_BN_LIB;
        ASN1_BIT_STRING rbs,sbs;
	MS_STATIC unsigned char rbuf[50]; /* assuming r is 20 bytes +extra */
	MS_STATIC unsigned char sbuf[50]; /* assuming s is 20 bytes +extra */

	ctx=BN_CTX_new();
	if (ctx == NULL) goto err;

	i=BN_num_bytes(dsa->q);	/* should be 20 */
	if ((dlen > i) || (dlen > 50))
		{
		reason=DSA_R_DATA_TO_LARGE_FOR_KEY_SIZE;
		goto err;
		}
	
	m=BN_new();
	r=BN_new();
	s=BN_new();
	k=BN_new();
	if (m == NULL || r == NULL || s == NULL || k == NULL) goto err;

	if (BN_bin2bn(dgst,dlen,m) == NULL) goto err;

	/* Get random k */
	if (!BN_rand(k, 160, 1, 0)) goto err;

	/* Compute r = (g^k mod p) mod q */
	if (!BN_mod_exp(s,dsa->g,k,dsa->p,ctx)) goto err;	/* s is temp */
	if (!BN_mod(r,s,dsa->q,ctx)) goto err;

	/* Compute  s = inv(k) (m + xr) mod q */
	if (!BN_mul(s, dsa->x, r)) goto err;	/* s = xr */
	if (!BN_add(s, s, m)) goto err;		/* s = m + xr */
	if ((kinv=BN_mod_inverse(k,dsa->q,ctx)) == NULL) goto err;
	if (!BN_mod_mul(s,s,kinv,dsa->q,ctx)) goto err;

	/*
	 * Now create a ASN.1 sequence of the integers R and S.
	 */
	rbs.data=rbuf;
	sbs.data=sbuf;
	rbs.length=BN_bn2bin(r,rbs.data);
	sbs.length=BN_bn2bin(s,sbs.data);

	len =i2d_ASN1_INTEGER(&rbs,NULL);
	len+=i2d_ASN1_INTEGER(&sbs,NULL);

	p=sig;
	ASN1_put_object(&p,1,len,V_ASN1_SEQUENCE,V_ASN1_UNIVERSAL);
	i2d_ASN1_INTEGER(&rbs,&p);
	i2d_ASN1_INTEGER(&sbs,&p);
	*siglen=(p-sig);
	ret=1;
err:
	if (!ret) DSAerr(DSA_F_DSA_SIGN,reason);
		
	if (ctx != NULL) BN_CTX_free(ctx);
	if (m != NULL) BN_free(m);
	if (r != NULL) BN_free(r);
	if (s != NULL) BN_free(s);
	if (k != NULL) BN_free(k);
	if (kinv != NULL) BN_free(kinv);
	return(ret);
	}
