/*
 * $Header$
 * $Source$
 *
 * Copyright (c) 1988 Acorn Computers Ltd., Cambridge, England
 *
 * $Desc$
 * $Log$
 * Revision 1.4  88/10/04  20:48:39  keith
 * Add a new ICMP PEEK type, to allow remote spying on kernel version
 * 
 * Revision 1.3  88/06/17  20:27:13  beta
 * Acorn Unix initial beta version
 * 
 */
/*
 *    ../netinet/peek.h  Defined the values of the memtype in peek requests
 */

#define ICMP_PEEK_VERSION 0 /* Peek at a remote machine's kernel version */
#define ICMP_PEEK_MEM  1
#define ICMP_PEEK_KMEM 2
