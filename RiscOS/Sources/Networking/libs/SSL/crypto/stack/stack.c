/* crypto/stack/stack.c */
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

/* Code for stacks
 * Author - Eric Young v 1.0
 * 1.1 eay - Take from netdb and added to SSLeay
 *
 * 1.0 eay - First version 29/07/92
 */
#include <stdio.h>
#include "cryptlib.h"
#include "stack.h"

#undef MIN_NODES
#define MIN_NODES	4

char *STACK_version="STACK part of SSLeay 0.6.0 21-Jun-1996";

#ifndef NOPROTO
#define	FP_ICC	(int (*)(const void *,const void *))
#else
#define FP_ICC
#endif

#include <errno.h>
STACK *sk_new(c)
int (*c)();
	{
	STACK *ret;
	int i;

	if ((ret=(STACK *)malloc(sizeof(STACK))) == NULL)
		goto err0;
	if ((ret->data=(char **)malloc(sizeof(char *)*MIN_NODES)) == NULL)
		goto err1;
	for (i=0; i<MIN_NODES; i++)
		ret->data[i]=NULL;
	ret->comp=c;
	ret->num_alloc=MIN_NODES;
	ret->num=0;
	ret->sorted=0;
	return(ret);
err1:
	free((char *)ret);
err0:
	return(NULL);
	}

int sk_insert(st,data,loc)
STACK *st;
char *data;
int loc;
	{
	char **s;

	if (st->num_alloc <= st->num+1)
		{
		s=(char **)realloc((char *)st->data,
			(unsigned int)sizeof(char *)*st->num_alloc*2);
		if (s == NULL)
			return(0);
		st->data=s;
		st->num_alloc*=2;
		}
	if ((loc >= (int)st->num) || (loc < 0))
		st->data[st->num]=data;
	else
		{
		memcpy( (char *)&(st->data[loc+1]),
			(char *)&(st->data[loc]),
			sizeof(char *)*(st->num-loc));
		st->data[loc]=data;
		}
	st->num++;
	st->sorted=0;
	return(st->num);
	}

char *sk_delete_ptr(st,p)
STACK *st;
char *p;
	{
	int i;

	for (i=0; i<st->num; i++)
		if (st->data[i] == p)
			return(sk_delete(st,i));
	return(NULL);
	}

char *sk_delete(st,loc)
STACK *st;
int loc;
	{
	char *ret;

	if ((st->num == 0) || (loc < 0) || (loc >= st->num)) return(NULL);

	ret=st->data[loc];
	if (loc != st->num-1)
		memcpy( &(st->data[loc]),
			&(st->data[loc+1]),
			sizeof(char *)*(st->num-loc-1));
	st->num--;
	return(ret);
	}

int sk_find(st,data)
STACK *st;
char *data;
	{
	char **r;
	int i;
	int (*comp_func)();

	if (st->comp == NULL)
		{
		for (i=0; i<st->num; i++)
			if (st->data[i] == data)
				return(i);
		}
	comp_func=(int (*)())st->comp;
	if (!st->sorted)
		{
		qsort((char *)st->data,st->num,sizeof(char *),FP_ICC comp_func);
		st->sorted=1;
		}
	if (data == NULL) return(-1);
	r=(char **)bsearch(&data,(char *)st->data,
		st->num,sizeof(char *),FP_ICC comp_func);
	if (r == NULL) return(-1);
	i=(int)(r-st->data);
	for ( ; i>0; i--)
		if ((*st->comp)(&(st->data[i-1]),&data) <= 0)
			break;
	return(i);
	}


int sk_push(st,data)
STACK *st;
char *data;
	{
	return(sk_insert(st,data,st->num));
	}

int sk_unshift(st,data)
STACK *st;
char *data;
	{
	return(sk_insert(st,data,0));
	}

char *sk_shift(st)
STACK *st;
	{
	if (st == NULL) return(NULL);
	if (st->num <= 0) return(NULL);
	return(sk_delete(st,0));
	}

char *sk_pop(st)
STACK *st;
	{
	if (st == NULL) return(NULL);
	if (st->num <= 0) return(NULL);
	return(sk_delete(st,st->num-1));
	}

void sk_pop_free(st,func)
STACK *st;
void (*func)();
	{
	int i;

	if (st == NULL) return;
	for (i=0; i<st->num; i++)
		if (st->data[i] != NULL)
			func(st->data[i]);
	sk_free(st);
	}

void sk_free(st)
STACK *st;
	{
	if (st == NULL) return;
	if (st->data != NULL) free((char *)st->data);
	free((char *)st);
	}

