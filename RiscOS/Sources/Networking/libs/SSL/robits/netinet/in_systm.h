/*
 * $Header$
 * $Source$
 *
 * Copyright (c) 1988 Acorn Computers Ltd., Cambridge, England
 *
 * $Desc$
 * $Log$
 * Revision 1.3  88/06/17  20:26:31  beta
 * Acorn Unix initial beta version
 * 
 */
/* @(#)in_systm.h	1.1 87/06/30 3.2/4.3NFSSRC */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)in_systm.h	7.1 (Berkeley) 6/5/86
 */

#ifndef __netinet_in_systm_h
#define __netinet_in_systm_h

/*
 * Miscellaneous internetwork
 * definitions for kernel.
 */

/*
 * Network types.
 *
 * Internally the system keeps counters in the headers with the bytes
 * swapped so that VAX instructions will work on them.  It reverses
 * the bytes before transmission at each protocol level.  The n_ types
 * represent the types with the bytes in ``high-ender'' order.
 */
typedef u_short n_short;		/* short as received from the net */
typedef u_long	n_long;			/* long as received from the net */

typedef	u_long	n_time;			/* ms since 00:00 GMT, byte rev */

#ifdef KERNEL
n_time	iptime();
#endif

#endif /* !defined(__netinet_in_systm_h) */

/* EOF in_systm.h */
