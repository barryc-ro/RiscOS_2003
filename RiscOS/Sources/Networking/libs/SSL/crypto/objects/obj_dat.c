/* crypto/objects/obj_dat.c */
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
#include "lhash.h"
#include "asn1.h"
#include "objects.h"

/* obj_dat.h is generated from objects.h by obj_dat.pl */
#include "obj_dat.h"

#ifdef RISCOS_ZM
void riscos_OBJ_FoulAndGrodyHack( void )
{
    int i;

    /* Patch up the nids */
    for ( i=0; i < NUM_NID; i++ )
    {
        int j = (int) nid_objs[i].data;
        if ( j > -1 )
            nid_objs[i].data = lvalues + j;
        else
            nid_objs[i].data = NULL;
    }

    /* Patch up the sn's */
    for ( i=0; i < NUM_SN; i++ )
    {
        int j = (int) sn_objs[i];

        sn_objs[i] = nid_objs + j;
    }

    /* Patch up the ln's */
    for ( i=0; i < NUM_LN; i++ )
    {
        int j = (int) ln_objs[i];

        ln_objs[i] = nid_objs + j;
    }

    /* Patch up the obj's */
    for ( i=0; i < NUM_OBJ; i++ )
    {
        int j = (int) obj_objs[i];

        obj_objs[i] = nid_objs + j;
    }
}
#endif

#ifndef NOPROTO
static int sn_cmp(ASN1_OBJECT **a, ASN1_OBJECT **b);
static int ln_cmp(ASN1_OBJECT **a, ASN1_OBJECT **b);
static int obj_cmp(ASN1_OBJECT **a, ASN1_OBJECT **b);
#else
static int sn_cmp();
static int ln_cmp();
static int obj_cmp();
#endif

static int sn_cmp(ap,bp)
ASN1_OBJECT **ap;
ASN1_OBJECT **bp;
	{ return(strcmp((*ap)->sn,(*bp)->sn)); }

static int ln_cmp(ap,bp)
ASN1_OBJECT **ap;
ASN1_OBJECT **bp;
	{ return(strcmp((*ap)->ln,(*bp)->ln)); }

ASN1_OBJECT *OBJ_nid2obj(n)
int n;
	{
	if ((n < 0) || (n >= NUM_NID))
		{
		OBJerr(OBJ_F_OBJ_NID2OBJ,OBJ_R_NID_IS_OUT_OF_RANGE);
		return(NULL);
		}
	if ((n != NID_undef) && (nid_objs[n].nid == NID_undef))
		{
		OBJerr(OBJ_F_OBJ_NID2OBJ,OBJ_R_UNKNOWN_NID);
		return(NULL);
		}
	return((ASN1_OBJECT *)&(nid_objs[n]));
	}

char *OBJ_nid2sn(n)
int n;
	{
	if ((n < 0) || (n >= NUM_NID))
		{
		OBJerr(OBJ_F_OBJ_NID2SN,OBJ_R_NID_IS_OUT_OF_RANGE);
		return(NULL);
		}
	if ((n != NID_undef) && (nid_objs[n].nid == NID_undef))
		{
		OBJerr(OBJ_F_OBJ_NID2SN,OBJ_R_UNKNOWN_NID);
		return(NULL);
		}
	return(nid_objs[n].sn);
	}

char *OBJ_nid2ln(n)
int n;
	{
	if ((n < 0) || (n >= NUM_NID))
		{
		OBJerr(OBJ_F_OBJ_NID2LN,OBJ_R_NID_IS_OUT_OF_RANGE);
		return(NULL);
		}
	if ((n != NID_undef) && (nid_objs[n].nid == NID_undef))
		{
		OBJerr(OBJ_F_OBJ_NID2LN,OBJ_R_UNKNOWN_NID);
		return(NULL);
		}
	return(nid_objs[n].ln);
	}

int OBJ_obj2nid(a)
ASN1_OBJECT *a;
	{
	ASN1_OBJECT **op;

	if (a == NULL)
		return(NID_undef);
	op=(ASN1_OBJECT **)OBJ_bsearch((char *)&a,(char *)obj_objs,NUM_OBJ,
		sizeof(ASN1_OBJECT *),(int (*)())obj_cmp);
	if (op == NULL)
		return(NID_undef);
	return((*op)->nid);
	}

int OBJ_txt2nid(s)
char *s;
	{
	int ret;

	ret=OBJ_sn2nid(s);
	if (ret == NID_undef)
		return(OBJ_ln2nid(s));
	else
		return(ret);
	}

int OBJ_ln2nid(s)
char *s;
	{
	ASN1_OBJECT o,*oo=&o,**op;

	o.ln=s;
	op=(ASN1_OBJECT **)OBJ_bsearch((char *)&oo,(char *)ln_objs,NUM_LN,
		sizeof(ASN1_OBJECT *),(int (*)())ln_cmp);
	if (op == NULL) return(NID_undef);
	return((*op)->nid);
	}

int OBJ_sn2nid(s)
char *s;
	{
	ASN1_OBJECT o,*oo=&o,**op;

	o.sn=s;
	op=(ASN1_OBJECT **)OBJ_bsearch((char *)&oo,(char *)sn_objs,NUM_SN,
		sizeof(ASN1_OBJECT *),(int (*)())sn_cmp);
	if (op == NULL) return(NID_undef);
	return((*op)->nid);
	}

static int obj_cmp(ap, bp)
ASN1_OBJECT **ap;
ASN1_OBJECT **bp;
	{
	int j;
	ASN1_OBJECT *a= *ap;
	ASN1_OBJECT *b= *bp;

	j=(a->length - b->length);
        if (j) return(j);
	return(memcmp(a->data,b->data,a->length));
        }

char *OBJ_bsearch(key,base,num,size,cmp)
char *key;
char *base;
int num;
int size;
int (*cmp)();
	{
	int l,h,i,c;
	char *p;

	if (num == 0) return(NULL);
	l=0;
	h=num;
	while (l < h)
		{
		i=(l+h)/2;
		p= &(base[i*size]);
		c=(*cmp)(key,p);
		if (c < 0)
			h=i;
		else if (c > 0)
			l=i+1;
		else
			return(p);
		}
	return(NULL);
	}
