/*
 * $Header$
 * $Source$
 *
 * Copyright (c) 1988 Acorn Computers Ltd., Cambridge, England
 *
 * $Desc$
 * $Log$
 * Revision 1.3  88/06/17  20:27:25  beta
 * Acorn Unix initial beta version
 * 
 */
/* @(#)tcp_debug.h	1.1 87/07/01 3.2/4.3NFSSRC */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)tcp_debug.h	7.1 (Berkeley) 6/5/86
 */

struct	tcp_debug {
	n_time	td_time;
	short	td_act;
	short	td_ostate;
	caddr_t	td_tcb;
	struct	tcpiphdr td_ti;
#ifndef arm
	short	td_req;
#else   arm
	int	td_req; /* For debugging this stores a char* */
#endif  arm
	struct	tcpcb td_cb;
};

#define	TA_INPUT 	0
#define	TA_OUTPUT	1
#define	TA_USER		2
#define	TA_RESPOND	3
#define	TA_DROP		4
#define TA_DEBUG        5  /* KDR special just for debugging */

#ifdef TANAMES
char	*tanames[] =
    { "input", "output", "user", "respond", "drop", "debug" };
#endif

#define	TCP_NDEBUG 100
#ifdef  KERNEL
struct	tcp_debug tcp_debug[TCP_NDEBUG];
int	tcp_debx;
#endif  KERNEL
