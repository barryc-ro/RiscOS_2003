/* -*-C-*-
 *
 * $Header$
 * $Source$
 *
 * Copyright (c) 1995 Acorn Computers Ltd., Cambridge, England
 *
 * $Log$
 * Revision 1.1  95/04/20  12:16:09  kwelton
 * Initial revision
 * 
 */
#include "sys/types.h"

#include "netinet/in.h"

/*
 * variable declarations
 */
extern int _host_stayopen;
extern int _net_stayopen;
extern int _proto_stayopen;
extern int _serv_stayopen;

/*
 * function prototypes
 */
extern void endhostent(void);
extern void endnetent(void);
extern void endprotoent(void);
extern void endservent(void);

extern struct hostent *gethostbyaddr(char *addr, int length, int type);
extern struct hostent *gethostbyname(char *nam);
extern struct hostent *gethostent(void);
extern struct netent *getnetbyaddr(long net, int type);
extern struct netent *getnetbyname(char *name);
extern struct netent *getnetent(void);
extern struct protoent *getprotobyname(char *name);
extern struct protoent *getprotobynumber(int proto);
extern struct protoent *getprotoent(void);
extern struct servent *getservbyname(char *name, char *proto);
extern struct servent *getservbyport(int port, char *proto);
extern struct servent *getservent(void);

extern u_long htonl(u_long x);
extern int htons(int x);

extern u_long inet_addr(char *cp);
extern int inet_lnaof(struct in_addr in);
extern struct in_addr inet_makeaddr(int net, int host);
extern u_long inet_network(char *cp);
extern int inet_netof(struct in_addr in);
extern char *inet_ntoa(struct in_addr in);

extern struct hostent *namisipadr(char *nam);
extern u_long ntohl(u_long x);
extern int ntohs(int x);

extern void sethostent(int f);
extern void setnetent(int f);
extern void setprotoent(int f);
extern void setservent(int f);

/* EOF inetlib.h */
