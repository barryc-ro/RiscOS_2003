
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 * Copyright (c) 1991 Acorn Computers Ltd., Cambridge, England
 */

/*
 * Structures returned by network
 * data base library.  All addresses
 * are supplied in host order, and
 * returned in network order (suitable
 * for use in system calls).
 */
struct  hostent {
        char    *h_name;        /* official name of host */
        char    **h_aliases;    /* alias list */
        int     h_addrtype;     /* host address type */
        int     h_length;       /* length of address */
        char    **h_addr_list;  /* list of addresses returned */
#define h_addr  h_addr_list[0]  /* address, for backward compatiblity */
};

/*
 * Assumption here is that a network number
 * fits in 32 bits -- probably a poor one.
 */
struct  netent {
        char            *n_name;        /* official name of net */
        char            **n_aliases;    /* alias list */
        int             n_addrtype;     /* net address type */
        unsigned long   n_net;          /* network # */
};

struct  servent {
        char    *s_name;        /* official service name */
        char    **s_aliases;    /* alias list */
        int     s_port;         /* port # */
        char    *s_proto;       /* protocol to use */
};

struct  protoent {
        char    *p_name;        /* official protocol name */
        char    **p_aliases;    /* alias list */
        int     p_proto;        /* protocol # */
};

struct hostent  *gethostbyname(char *),
                *gethostbyaddr(char *, int, int),
                *gethostent(void);
void sethostent(int), endhostent(void);

struct netent   *getnetbyname(char *),
                *getnetbyaddr(long, int),
                *getnetent(void);
void setnetent(int), endnetent(void);
struct servent  *getservbyname(char *, char*),
                *getservbyport(int, char *),
                *getservent(void);
void setservent(int), endservent(void);
struct protoent *getprotobyname(char *),
                *getprotobynumber(int),
                *getprotoent(void);
void setprotoent(int), endprotoent(void);

