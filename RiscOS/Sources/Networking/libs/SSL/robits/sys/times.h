/*
 * $Header$
 * $Source$
 *
 * Copyright (c) 1988 Acorn Computers Ltd., Cambridge, England
 *
 * $Desc$
 * $Log$
 * Revision 1.3  88/06/17  20:21:58  beta
 * Acorn Unix initial beta version
 * 
 */
/* @(#)times.h	1.2 87/05/15 3.2/4.3NFSSRC */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)times.h	7.1 (Berkeley) 6/4/86
 */

/*
 * Structure returned by times()
 */
struct tms {
	time_t	tms_utime;		/* user time */
	time_t	tms_stime;		/* system time */
	time_t	tms_cutime;		/* user time, children */
	time_t	tms_cstime;		/* system time, children */
};
