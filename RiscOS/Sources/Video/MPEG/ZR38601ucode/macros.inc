/* ident "@(#)macros.inc:/dsp/audiocod/A10_step_C0/global_inc/SCCS/s.macros.inc 1.7 05/26/97" */
/**************************************************************************/
/*                                                                        */
/*    Copyright (c) 1995 ZORAN Corporation, All Rights Reserved           */
/*    THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ZORAN CORPORATION    */
/*                                                                        */
/*
 *    File : SCCS/s.macros.inc
 *    Type : C File
 *    Module : macros.inc
 *    Sccs Identification (SID) : 1.7
 *    Modification Time : 05/26/97 10:50:24
 *
 *    Purpose    : 
 *    Programmer : 
 *                                                                        */
/*                                                                        */
/**************************************************************************/

/*	File: usr_mac.inc		$Revision$ */

/****************************************************************************
;	Unpublished work.  Copyright 1992 Dolby Laboratories, Inc.
;	All Rights Reserved.
;
;	Include:	usr_mac
;		Common macros
;
;	History:
;		sv	8/17/92		Ported to Zoran A1
;***************************************************************************/

// #include "com_def.inc"


#define do_1(a,b)		/* 1 delay do loop macro */\
	move	a,lc;		/* set up loop count */\
	loop	b		/* initiate loop */

#define jeq_1(a)		/* 1 delay conditional branch */\
	dbeq	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jeq_2(a)		/* 2 delay conditional branch */\
	dbeq	a		/* delayed conditional branch */

#define jge_1(a)		/* 1 delay conditional branch */\
	dbge	a;		/* delayed conditional branch */\
	nop									/* one delay state */

#define jge_2(a)		/* 2 delay conditional branch */\
	dbge	a		/* delayed conditional branch */

#define jgt_1(a)		/* 1 delay conditional branch */\
	dbgt	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jgt_2(a)		/* 2 delay conditional branch */\
	dbgt	a		/* delayed conditional branch */

#define jle_1(a)		/* 1 delay conditional branch */\
	dble	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jle_2(a)		/* 2 delay conditional branch */\
	dble	a		/* delayed conditional branch */

#define jlt_1(a)		/* 1 delay conditional branch */\
	dblt	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jlt_2(a)		/* 2 delay conditional branch */\
	dblt	a		/* delayed conditional branch */

#define jne_1(a)		/* 1 delay conditional branch */\
	dbne	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jne_2(a)		/* 2 delay conditional branch */\
	dbne	a		/* delayed conditional branch */

#define jc_1(a)			/* 1 delay conditional branch */\
	dbc		a;	/* delayed conditional branch */\
	nop			/* one delay state */

#define jc_2(a)			/* 2 delay conditional branch */\
	dbc		a	/* delayed conditional branch */

#define jnc_1(a)		/* 1 delay conditional branch */\
	dbnc	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jnc_2(a)		/* 2 delay conditional branch */\
	dbnc	a		/* delayed conditional branch */

#define jsv_1(a)		/* 1 delay conditional branch */\
	dbsv	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jsv_2(a)		/* 2 delay conditional branch */\
	dbsv	a		/* delayed conditional branch */

#define jnsv_1(a)		/* 1 delay conditional branch */\
	dbnsv	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jnsv_2(a)		/* 2 delay conditional branch */\
	dbnsv	a		/* delayed conditional branch */

#define jss_1(a)		/* 1 delay conditional branch */\
	dbss	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jss_2(a)		/* 2 delay conditional branch */\
	dbss	a		/* delayed conditional branch */

#define jnss_1(a)		/* 1 delay conditional branch */\
	dbnss	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jnss_2(a)		/* 2 delay conditional branch */\
	dbnss	a		/* delayed conditional branch */

#define jsl_1(a)		/* 1 delay conditional branch */\
	dbsl	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jsl_2(a)		/* 2 delay conditional branch */\
	dbsl	a		/* delayed conditional branch */

#define jnsl_1(a)		/* 1 delay conditional branch */\
	dbnsl	a;		/* delayed conditional branch */\
	nop			/* one delay state */

#define jnsl_2(a)		/* 2 delay conditional branch */\
	dbnsl	a		/* delayed conditional branch */

#define jmp_1(a)		/* 1 delay branch */\
	db		a;	/* delayed branch */\
	nop			/* one delay state */

#define jmp_2(a)		/* 2 delay branch */\
	db		a	/* delayed branch */

#define rti_1			/* 1 delay return from interrupt */\
	move	(sp)+,status;	/* pop status flags */\
	move	(sp)+,pc;	/* pop program counter */\
	move	#1,ie		/* enable interrupts */

#define rts_1			/* 1 delay return from subroutine */\
	move	(sp)+,pc;	/* pop program counter */\
	nop			/* one delay state */

#define rts_2			/* 2 delay return from subroutine */\
	move	(sp)+,pc	/* pop program counter */

#define move_m(value,memptr)	/* set memory element with value */\
	move	value,d0;	/* get value */\
	move	d0m,memptr	/* save to memory */

#define move_m1(value,memptr)	/* set memory element with value */\
        move    value,d3;	/* get value */\
        move    d3,memptr	/* save to memory */

#define inc_m(memptr)		/* increment memory element */\
	move	memptr,d0;	/* get memory element */\
	inc		d0;	/* increment */\
	move	d0m,memptr	/* save to memory */

#define dec_m(memptr)		/* decrement memory element */\
	move	memptr,d0;	/* get memory element */\
	dec		d0;	/* decrement */\
	move	d0m,memptr	/* save to memory */

#define add_m(value,memptr)	/* add to memory element by value */\
	move	memptr,d0;	/* get memory element */\
	move	value,d1;	/* get add amount */\
	add		d1,d0;	/* add to memory element */\
	move	d0m,memptr	/* save to memory */

#define tst_m(memptr)		/* compare memory element to zero */\
	move	memptr,d0;	/* get memory element */\
	cmpz	d0		/* compare to zero */

#define cmp_m(value,memptr)	/* compare memory element to value */\
	move	memptr,d0;	/* get memory element */\
	move	value,d1;	/* get value */\
	cmp		d1,d0	/* compare memory element to value */

#define pt_1(base,index1,reg,off)	/* point to 1-dim array */\
	move	base,reg;	/* get array base */\
	move	index1,off;	/* get first dimension index */\
	nop;			\
	move	(reg)+off	/* adjust pointer */

#define pt_1r(base,index1,reg,off,dreg)	/* lookup to 1-dim array */\
	move	base,reg;	/* get array base */\
	move	index1,off;	/* get first dimension index */\
	nop;			\
	move	(reg+off),dreg	/* look-up entry */

#if 1
#ifdef AX
#define cmpi_m(value,memptr)	/* compare memory element to value */\
	move	memptr,d0;	/* get memory element */\
	cmpi	#value,d0	/* compare memory element to value */

#define pt_2(base,index1,index2,size2,reg,off) /* point to 2-dim array */\
	clr	d1 clrb	#3,mode; /* set up for integer multiply */\
	move	index1,d0;	/* get first dimension index */\
	muli	#size2,d0;	/* compute first offset */\
	move	index2,d1l;	/* get second dimension index */\
	add	d1,d0 setb #3,mode;	/* add second offset */\
	move	base,reg;	/* get array base */\
	move	d0l,off;	/* set up offset */\
	nop;			\
	move	(reg)+off	/* adjust pointer */
#else
#define pt_2(base,index1,index2,size2,reg,off) /* point to 2-dim array */\
	clrb	#3,mode;	/*move	#0,ms;*//* set up for integer multiply */\
	move	index1,d1;	/* get first dimension index */\
	move	#size2,d0;	/* get first dimension spacing */\
	mul	d1,d0 #0,d1;	/* compute first offset */\
	move	index2,d1l;	/* get second dimension index */\
	add	d1,d0 setb #3,mode; /*#1,ms;*/	/* add second offset */\
	move	base,reg;	/* get array base */\
	move	d0l,off;	/* set up offset */\
	nop;			\
	move	(reg)+off	/* adjust pointer */
#endif
#else
#define pt_2(base,index1,index2,size2,reg,off) /* point to 2-dim array */\
	clrb	#3,mode; /*move	#0,ms;*/	/* set up for integer multiply */\
	move	index1,d1;	/* get first dimension index */\
	move	#size2,d0;	/* get first dimension spacing */\
	mul	d1,d0,d0;	/* compute first offset */\
	move	d0l,i6;		/* set up first offset */\
	move	i6,d0;		/* !!! vf */ \
	move	index2,d1;	/* get second dimension index */\
	add  	d1,d0 setb #3,mode; /*#1,ms;*/	/* add second offset */\
	move	base,reg;	/* get array base */\
	move	d0m,off;	/* set up offset */\
	nop;			\
	move	(reg)+off	/* adjust pointer */
#endif

// Note: do (ZR38000)/rpt are not interruptible; use #size with caution
#ifdef AX
#define cp_memory(source,dest,size)	/* point to 2-dim array */\
	move	source,a0;	/* setup source ptr */\
	move	dest,a1;	/* setup destination ptr */\
	rpt size; move	(a0)+,d0 d0,(a1)+	/* copy src to dest */
#define cp_mem(source,dest,size,cplp)	/* point to 2-dim array */\
	move	source,a0;	/* setup source ptr */\
	move	dest,a1;	/* setup destination ptr */\
	rpt size; move	(a0)+,d0 d0,(a1)+	/* copy src to dest */
#else
#define cp_mem(source,dest,size,cplp)	/* point to 2-dim array */\
	move	source,a0;		/* setup source ptr */\
	do_1(	size,cplp);		\
	move	dest,a1;		/* setup destination ptr */\
		move	(a0)+,d0;	/* copy src to reg */\
		move	d0m,(a1)+	/* copy to dest */\

#endif

#ifdef AX
#define alfld(source,mask,sft,dest)	/* extract and align field */\
	move	source,dest;		/* get input */\
	andi	mask,dest;		/* isolate field */\
	lshi	sft,dest		/* shift in place */
#else
#define alfld(source,mask,sft,dest)	/* extract and align field */\
	move	source,dest;		/* get input */\
	move	mask,d1;		/* setup mask */\
	and		d1,dest sft,d1;	/* isolate field, setup shift num */\
	lsh		d1,dest		/* shift in place */
#endif

#ifndef AX
#define	clrbit(n,reg)			/* clear bit n in register reg */\
	move	#1,d0;			/* set lsb mask */\
	move	n,d1;			/* set bit number */ \
	lsh		d1,d0;		/* setup positive bit mask */\
	neg		d0 reg,d1;	/* negative mask, copy reg */\
	and		d1,d0;		/* clear bit */\
	move	d0,reg			/* copy result */

#define	setbit(n,reg)			/* set bit n in register reg */\
	move	#1,d0;			/* set lsb mask */\
	move	n,d1;			/* set bit number */ \
	lsh		d1,d0;		/* setup bit mask */\
	move	reg,d1;			/* copy reg */\
	or		d1,d0;		/* set bit */\
	move	d0,reg			/* copy result */


#define	enableintab			/* enable int A, B, dfifo */\
	move	#0x32000,d0;		/* interrupt enable mask */\
	move	#0,ie;			/* disable ints */ \
	move	imr,d1;			\
	or		d1,d0;		/* disable serial I/O ints */\
	move	d0m,imr;		/* update mode reg */ \
	move	#1,ie			/* enable ints */

#define	disableintab			/* disable int A, B, dfifo */\
	move	#0xCDFFF,d0;		/* interrupt disable mask */\
	move	#0,ie;			/* disable ints */ \
	move	imr,d1;			\
	and		d1,d0;		/* disable serial I/O ints */\
	move	d0m,imr;		/* update mode reg */ \
	move	#1,ie			/* enable ints */
#else
#define	clrbit(n,reg)			/* clear bit n in register reg */\
	clrb	n,reg

#define	setbit(n,reg)			/* set bit n in register reg */\
	setb	n,reg

#define	tstfld(src,msk)			/* test bit field in source */\
	move	#msk,d1;		/* set mask */\
	move	src,d0;			/* setup source op */\
	and		d0,d1		/* test field */



#define	enableintb			/* enable int B */\
	setb	#16,mode		/* set BM field */ 

#define	enableintab			/* enable int A, B */\
	setb	#17,mode;		/* set AM field */ \
	setb	#16,mode		/* set BM field */ 

#define	disableintab			/* disable int A, B */\
	clrb	#17,mode;		/* clr AM field */ \
	clrb	#16,mode		/* clr BM field */ 

#define	enableinta			/* enable int A */ \
	setb	#17,mode		/* set AM field */ 

#define	disableinta			/* disable int A */\
	clrb	#17,mode		/* clr AM field */ 

#define	enableintfifo		/* enable DFFIFO int */\
	clrb	#HSPM_bit,imr	/* set HSPM field */ 

#define	disableintfifo		/* disable DFIFO int */\
	setb	#HSPM_bit,imr	/* clear HSPM field */


#endif

/////// MPEG MACROS /////////////////////////////////////////////////////////
// Perform error detection if protection_bit is set
//
#define	CRC_ERROR_CHECK \
	move	(PROTECTION_BIT),d4; \
	cmpz	d4  #1,i2; \
	dbne  No_protection; \
/*1*/	move	#Nb_bit_alloc, a1; /* same as above */ \
/*2*/	move	#Bit_alloc, a2; /* bit allocation for each subband */ \
	move	#1,i1;		/* stride for nb */ \
	jsr  CRC_calc; /* a1 - # of bits; i2 - stride */ \
	CRC_ERROR_CHECK_II; \
No_protection:

#define	CRC_ERROR_CHECK_II \
	move	(CRC_CHECK),d4; /* check CRC */ \
	move	#4,d2; \
	lsh	d2,d4  d1m,d2; /* align values */ \
	cmp	d2,d4; /* sets Z flag if error */ \
	jne_2( Exit); /* CRC error: set error status and exit */ \
	move	#2,d4; \
	move	d4,(dec_status) 

#define ReqDr_bit       3       // #bit in AVS_mask
#define DREQ_EN(reg) \
        move    (AVS_mask),reg;  \
        tstb    #ReqDr_bit,reg
