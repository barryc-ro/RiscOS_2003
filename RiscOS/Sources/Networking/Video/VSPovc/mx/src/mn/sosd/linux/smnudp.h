/* Copyright (c) 1994 by Oracle Corporation.  All Rights Reserved.
 *
 * smnudp.h - OMN UDP Network Interface for SunOS 4.1.3
 *
 * REVISONS
 * jolkin    04/20/94  Creation of Version 2.0
 */

#ifndef SMNUDP_ORACLE
#define SMNUDP_ORACLE

/*
 * smnudpOpen - Opens a UDP Network Interface
 *
 * DESCRIPTION
 * smnudpOpn() creates and returns an NIO for UDP.  The UDP socket opened
 * may be bound to a particular IP name if the name parameter is non-null.
 * If intr is TRUE, then the UDP NIO is created so as to generate interrupts
 * when data is received.  Otherwise, the NIO operates synchronously.
 */
mnnio *smnudpOpen(CONST char *name, boolean intr);

/*
 * smnudpPa - Name to Physical Address conversion
 *
 * DESCRIPTION
 * smnudpPa() determines the physical address for a given name.  The
 * physical address is written to *pa.
 *
 * The format of the name may be one of the following:
 *   UDP:<ip-addr>:<prtno>
 *   UDP:<hostname>:<prtno>
 *
 * where <ip-addr> is a dotted decimal form of an IP address, and
 * <hostname> is the name given to an IP node.  If the name is not
 * valid, a -1 is returned; otherwise, a 0 is returned.  The initial
 * "UDP:" prefix must be specified.
 */
sb4 smnudpPa(mnnpa *pa, CONST char *name);

#endif /* SMNUDP_ORACLE */
