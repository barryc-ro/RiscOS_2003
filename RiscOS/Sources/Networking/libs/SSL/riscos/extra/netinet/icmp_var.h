/*
 * $Header$
 * $Source$
 *
 * Copyright (c) 1988 Acorn Computers Ltd., Cambridge, England
 *
 * $Desc$
 * $Log$
 * Revision 1.3  88/06/17  20:25:51  beta
 * Acorn Unix initial beta version
 * 
 */
/* @(#)icmp_var.h       1.1 87/06/30 3.2/4.3NFSSRC +KDR BSD fixes 7.2 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *      @(#)icmp_var.h  7.2 (Berkeley) 1/13/87
 */

/*
 * Variables related to this implementation
 * of the internet control message protocol.
 */
struct  icmpstat {
/* statistics related to icmp packets generated */
        int     icps_error;             /* # of calls to icmp_error */
        int     icps_oldshort;          /* no error 'cuz old ip too short */
        int     icps_oldicmp;           /* no error 'cuz old was icmp */
        int     icps_outhist[ICMP_MAXTYPE + 1];
/* statistics related to input messages processed */
        int     icps_badcode;           /* icmp_code out of range */
        int     icps_tooshort;          /* packet < ICMP_MINLEN */
        int     icps_checksum;          /* bad checksum */
        int     icps_badlen;            /* calculated bound mismatch */
        int     icps_reflect;           /* number of responses */
        int     icps_inhist[ICMP_MAXTYPE + 1];
};

#ifdef KERNEL
struct  icmpstat icmpstat;
#endif
