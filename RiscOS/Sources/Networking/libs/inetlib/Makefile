# -*-Text-*-
#
# $Header$
# $Source$
#
# Copyright (c) 1995 Acorn Computers Ltd., Cambridge, England
#
# Makefile for inetlib.a
#
# :RCS Log discontinued:
# Revision 1.3  95/06/01  15:37:14  kwelton
# Added rresvport(), remade dependencies.
# 
# Revision 1.2  95/04/20  12:40:17  kwelton
# All library functions are now prototyped in inetlib.h.
# 
# Revision 1.1  95/04/18  16:48:06  kwelton
# Initial revision
# 
#
LEVEL=		../..
INCLUDES=	INetInc:,C:

CC=		cc
AS=		objasm
LD=		link
LIB=		libfile

CMHG=		cmhg

CFLAGS=		-depend !Depend -pcc -zps0 -I${INCLUDES} -fah -Fn -DINET
CFLAGSZM=	-depend !Depend -pcc -zps1 -I${INCLUDES} -ffah -DINET -zM

CCZM=		${CC} ${CFLAGSZM}

RMFLAGS=	-f
RM=		rm ${RMFLAGS}

CFILES=		gethent.c gethnad.c getnbyad.c getnbyna.c getnent.c \
		getprent.c getprnam.c getproto.c getsbyna.c getsbypo.c \
		getsent.c htonl.c inet_addr.c inet_lnaof.c inet_maddr.c \
		inet_nof.c inet_ntoa.c inet_nwork.c rresvport.c

OFILES=		gethent.o gethnad.o getnbyad.o getnbyna.o getnent.o \
		getprent.o getprnam.o getproto.o getsbyna.o getsbypo.o \
		getsent.o htonl.o inet_addr.o inet_lnaof.o inet_maddr.o \
		inet_nof.o inet_ntoa.o inet_nwork.o rresvport.o

ZFILES=		z.gethent z.gethnad z.getnbyad z.getnbyna z.getnent \
		z.getprent z.getprnam z.getproto z.getsbyna z.getsbypo \
		z.getsent z.htonl z.inet_addr z.inet_lnaof z.inet_maddr \
		z.inet_nof z.inet_ntoa z.inet_nwork z.rresvport

.SUFFIXES:	.z .o .c .s

.c.z:;       ${CCZM} -c -o $@ $<
.c.o:;       ${CC} ${CFLAGS} -c -o $@ $<

all:	inetlib inetlibzm 

inetlib:	${OFILES} Makefile
	${LIB} -c inetlib ${OFILES}
	${LIB} -o inetlib

inetlibzm:	${ZFILES} Makefile
	${LIB} -c inetlibzm ${ZFILES}
	${LIB} -o inetlibzm

clean:
	-Wipe o.* ~C~V
	-Wipe z.* ~C~V
	Remove inetlib
	Remove inetlibzm


# Dynamic dependencies:
o.gethent:	c.gethent
o.gethent:	INetInc:h.netdb
o.gethent:	INetInc:sys.h.types
o.gethent:	INetInc:sys.h.socket
o.gethent:	h.inetlib
o.gethent:	INetInc:sys.h.types
o.gethent:	INetInc:netinet.h.in
o.gethnad:	c.gethnad
o.gethnad:	C:h.kernel
o.gethnad:	C:h.swis
o.gethnad:	INetInc:h.netdb
o.gethnad:	h.inetlib
o.gethnad:	INetInc:sys.h.types
o.gethnad:	INetInc:netinet.h.in
o.getnbyad:	c.getnbyad
o.getnbyad:	INetInc:h.netdb
o.getnbyad:	h.inetlib
o.getnbyad:	INetInc:sys.h.types
o.getnbyad:	INetInc:netinet.h.in
o.getnbyna:	c.getnbyna
o.getnbyna:	INetInc:h.netdb
o.getnbyna:	h.inetlib
o.getnbyna:	INetInc:sys.h.types
o.getnbyna:	INetInc:netinet.h.in
o.getnent:	c.getnent
o.getnent:	INetInc:h.netdb
o.getnent:	INetInc:sys.h.types
o.getnent:	INetInc:sys.h.socket
o.getnent:	h.inetlib
o.getnent:	INetInc:sys.h.types
o.getnent:	INetInc:netinet.h.in
o.getprent:	c.getprent
o.getprent:	INetInc:h.netdb
o.getprent:	INetInc:sys.h.types
o.getprent:	INetInc:sys.h.socket
o.getprent:	h.inetlib
o.getprent:	INetInc:sys.h.types
o.getprent:	INetInc:netinet.h.in
o.getprnam:	c.getprnam
o.getprnam:	INetInc:h.netdb
o.getprnam:	h.inetlib
o.getprnam:	INetInc:sys.h.types
o.getprnam:	INetInc:netinet.h.in
o.getproto:	c.getproto
o.getproto:	INetInc:h.netdb
o.getproto:	h.inetlib
o.getproto:	INetInc:sys.h.types
o.getproto:	INetInc:netinet.h.in
o.getsbyna:	c.getsbyna
o.getsbyna:	INetInc:h.netdb
o.getsbyna:	h.inetlib
o.getsbyna:	INetInc:sys.h.types
o.getsbyna:	INetInc:netinet.h.in
o.getsbypo:	c.getsbypo
o.getsbypo:	INetInc:h.netdb
o.getsbypo:	h.inetlib
o.getsbypo:	INetInc:sys.h.types
o.getsbypo:	INetInc:netinet.h.in
o.getsent:	c.getsent
o.getsent:	INetInc:h.netdb
o.getsent:	INetInc:sys.h.types
o.getsent:	INetInc:sys.h.socket
o.getsent:	h.inetlib
o.getsent:	INetInc:sys.h.types
o.getsent:	INetInc:netinet.h.in
o.htonl:	c.htonl
o.htonl:	INetInc:sys.h.types
o.htonl:	h.inetlib
o.htonl:	INetInc:sys.h.types
o.htonl:	INetInc:netinet.h.in
o.inet_addr:	c.inet_addr
o.inet_addr:	INetInc:sys.h.types
o.inet_addr:	INetInc:netinet.h.in
o.inet_addr:	h.inetlib
o.inet_addr:	INetInc:sys.h.types
o.inet_addr:	INetInc:netinet.h.in
o.inet_lnaof:	c.inet_lnaof
o.inet_lnaof:	INetInc:sys.h.types
o.inet_lnaof:	INetInc:netinet.h.in
o.inet_lnaof:	h.inetlib
o.inet_lnaof:	INetInc:sys.h.types
o.inet_lnaof:	INetInc:netinet.h.in
o.inet_maddr:	c.inet_maddr
o.inet_maddr:	INetInc:sys.h.types
o.inet_maddr:	INetInc:netinet.h.in
o.inet_maddr:	h.inetlib
o.inet_maddr:	INetInc:sys.h.types
o.inet_maddr:	INetInc:netinet.h.in
o.inet_nof:	c.inet_nof
o.inet_nof:	INetInc:sys.h.types
o.inet_nof:	INetInc:netinet.h.in
o.inet_nof:	h.inetlib
o.inet_nof:	INetInc:sys.h.types
o.inet_nof:	INetInc:netinet.h.in
o.inet_ntoa:	c.inet_ntoa
o.inet_ntoa:	INetInc:sys.h.types
o.inet_ntoa:	INetInc:netinet.h.in
o.inet_ntoa:	h.inetlib
o.inet_ntoa:	INetInc:sys.h.types
o.inet_ntoa:	INetInc:netinet.h.in
o.inet_nwork:	c.inet_nwork
o.inet_nwork:	INetInc:sys.h.types
o.inet_nwork:	h.inetlib
o.inet_nwork:	INetInc:sys.h.types
o.inet_nwork:	INetInc:netinet.h.in
o.rresvport:	c.rresvport
o.rresvport:	INetInc:sys.h.errno
o.rresvport:	INetInc:sys.^.h.errno
o.rresvport:	C:h.kernel
o.rresvport:	INetInc:sys.h.types
o.rresvport:	INetInc:sys.h.socket
o.rresvport:	INetInc:netinet.h.in
o.rresvport:	h.inetlib
o.rresvport:	INetInc:sys.h.types
o.rresvport:	INetInc:netinet.h.in
z.gethent:	c.gethent
z.gethent:	INetInc:h.netdb
z.gethent:	INetInc:sys.h.types
z.gethent:	INetInc:sys.h.socket
z.gethent:	h.inetlib
z.gethent:	INetInc:sys.h.types
z.gethent:	INetInc:netinet.h.in
z.gethnad:	c.gethnad
z.gethnad:	C:h.kernel
z.gethnad:	C:h.swis
z.gethnad:	INetInc:h.netdb
z.gethnad:	h.inetlib
z.gethnad:	INetInc:sys.h.types
z.gethnad:	INetInc:netinet.h.in
z.getnbyad:	c.getnbyad
z.getnbyad:	INetInc:h.netdb
z.getnbyad:	h.inetlib
z.getnbyad:	INetInc:sys.h.types
z.getnbyad:	INetInc:netinet.h.in
z.getnbyna:	c.getnbyna
z.getnbyna:	INetInc:h.netdb
z.getnbyna:	h.inetlib
z.getnbyna:	INetInc:sys.h.types
z.getnbyna:	INetInc:netinet.h.in
z.getnent:	c.getnent
z.getnent:	INetInc:h.netdb
z.getnent:	INetInc:sys.h.types
z.getnent:	INetInc:sys.h.socket
z.getnent:	h.inetlib
z.getnent:	INetInc:sys.h.types
z.getnent:	INetInc:netinet.h.in
z.getprent:	c.getprent
z.getprent:	INetInc:h.netdb
z.getprent:	INetInc:sys.h.types
z.getprent:	INetInc:sys.h.socket
z.getprent:	h.inetlib
z.getprent:	INetInc:sys.h.types
z.getprent:	INetInc:netinet.h.in
z.getprnam:	c.getprnam
z.getprnam:	INetInc:h.netdb
z.getprnam:	h.inetlib
z.getprnam:	INetInc:sys.h.types
z.getprnam:	INetInc:netinet.h.in
z.getproto:	c.getproto
z.getproto:	INetInc:h.netdb
z.getproto:	h.inetlib
z.getproto:	INetInc:sys.h.types
z.getproto:	INetInc:netinet.h.in
z.getsbyna:	c.getsbyna
z.getsbyna:	INetInc:h.netdb
z.getsbyna:	h.inetlib
z.getsbyna:	INetInc:sys.h.types
z.getsbyna:	INetInc:netinet.h.in
z.getsbypo:	c.getsbypo
z.getsbypo:	INetInc:h.netdb
z.getsbypo:	h.inetlib
z.getsbypo:	INetInc:sys.h.types
z.getsbypo:	INetInc:netinet.h.in
z.getsent:	c.getsent
z.getsent:	INetInc:h.netdb
z.getsent:	INetInc:sys.h.types
z.getsent:	INetInc:sys.h.socket
z.getsent:	h.inetlib
z.getsent:	INetInc:sys.h.types
z.getsent:	INetInc:netinet.h.in
z.htonl:	c.htonl
z.htonl:	INetInc:sys.h.types
z.htonl:	h.inetlib
z.htonl:	INetInc:sys.h.types
z.htonl:	INetInc:netinet.h.in
z.inet_addr:	c.inet_addr
z.inet_addr:	INetInc:sys.h.types
z.inet_addr:	INetInc:netinet.h.in
z.inet_addr:	h.inetlib
z.inet_addr:	INetInc:sys.h.types
z.inet_addr:	INetInc:netinet.h.in
z.inet_lnaof:	c.inet_lnaof
z.inet_lnaof:	INetInc:sys.h.types
z.inet_lnaof:	INetInc:netinet.h.in
z.inet_lnaof:	h.inetlib
z.inet_lnaof:	INetInc:sys.h.types
z.inet_lnaof:	INetInc:netinet.h.in
z.inet_maddr:	c.inet_maddr
z.inet_maddr:	INetInc:sys.h.types
z.inet_maddr:	INetInc:netinet.h.in
z.inet_maddr:	h.inetlib
z.inet_maddr:	INetInc:sys.h.types
z.inet_maddr:	INetInc:netinet.h.in
z.inet_nof:	c.inet_nof
z.inet_nof:	INetInc:sys.h.types
z.inet_nof:	INetInc:netinet.h.in
z.inet_nof:	h.inetlib
z.inet_nof:	INetInc:sys.h.types
z.inet_nof:	INetInc:netinet.h.in
z.inet_ntoa:	c.inet_ntoa
z.inet_ntoa:	INetInc:sys.h.types
z.inet_ntoa:	INetInc:netinet.h.in
z.inet_ntoa:	h.inetlib
z.inet_ntoa:	INetInc:sys.h.types
z.inet_ntoa:	INetInc:netinet.h.in
z.inet_nwork:	c.inet_nwork
z.inet_nwork:	INetInc:sys.h.types
z.inet_nwork:	h.inetlib
z.inet_nwork:	INetInc:sys.h.types
z.inet_nwork:	INetInc:netinet.h.in
z.rresvport:	c.rresvport
z.rresvport:	INetInc:sys.h.errno
z.rresvport:	INetInc:sys.^.h.errno
z.rresvport:	C:h.kernel
z.rresvport:	INetInc:sys.h.types
z.rresvport:	INetInc:sys.h.socket
z.rresvport:	INetInc:netinet.h.in
z.rresvport:	h.inetlib
z.rresvport:	INetInc:sys.h.types
z.rresvport:	INetInc:netinet.h.in
