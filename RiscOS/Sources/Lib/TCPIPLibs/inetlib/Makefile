#
# Copyright (c) 1995 Acorn Computers Ltd., Cambridge, England
#
# Makefile for inetlib.a
#
# Makefile,v
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
LIBDIR = <Lib$Dir>

#
# Generic options:
#
MKDIR   = cdir
WIPE    = -wipe
RM      = -remove
CP      = copy
CC	= cc
AS	= objasm
LD	= link
LIB	= libfile
CMHG	= cmhg

WFLAGS  = ~c~v
CPFLAGS = ~cflnr~v

INCLUDES=	TCPIPLibs:,C:

CFLAGS=		-depend !Depend -Wap -zps0 -I${INCLUDES} -fah -Fn -DINET -DCOMPAT_INET4
CFLAGSZM=	-depend !Depend -Wap -zps1 -I${INCLUDES} -ffah -DINET -DCOMPAT_INET4 -zM

CCZM=		${CC} ${CFLAGSZM}

CFILES=		gethent.c gethnad.c getnbyad.c getnbyna.c getnent.c \
		getprent.c getprnam.c getproto.c getsbyna.c getsbypo.c \
		getsent.c htonl.c inet_addr.c inet_lnaof.c inet_maddr.c \
		inet_nof.c inet_ntoa.c inet_nwork.c rresvport.c \
		linkaddr.c ns_addr.c ns_ntoa.c iso_addr.c

OFILES=		gethent.o gethnad.o getnbyad.o getnbyna.o getnent.o \
		getprent.o getprnam.o getproto.o getsbyna.o getsbypo.o \
		getsent.o htonl.o inet_addr.o inet_lnaof.o inet_maddr.o \
		inet_nof.o inet_ntoa.o inet_nwork.o rresvport.o \
		linkaddr.o ns_addr.o ns_ntoa.o iso_addr.o

ZFILES=		z.gethent z.gethnad z.getnbyad z.getnbyna z.getnent \
		z.getprent z.getprnam z.getproto z.getsbyna z.getsbypo \
		z.getsent z.htonl z.inet_addr z.inet_lnaof z.inet_maddr \
		z.inet_nof z.inet_ntoa z.inet_nwork z.rresvport \
		z.linkaddr z.ns_addr z.ns_ntoa z.iso_addr

.SUFFIXES:	.z .o .c .s

.c.z:;       ${CCZM} -c -o $@ $<
.c.o:;       ${CC} ${CFLAGS} -c -o $@ $<

COMPONENT = inetlib
LIBDIR = <Lib$Dir>

all:	inetlib inetlibzm

inetlib:	${OFILES} local_dirs
	${LIB} -c inetlib ${OFILES}
	${LIB} -o inetlib

inetlibzm:	${ZFILES} local_dirs
	${LIB} -c inetlibzm ${ZFILES}
	${LIB} -o inetlibzm

export: export_${PHASE}

export_hdrs:
	${CP} h.inetlib	TCPIPLibs:h.inetlib	${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: inetlib inetlibzm dirs
	${CP} inetlib	TCPIPLibs:o.inetlib	${CPFLAGS}
	${CP} inetlibzm	TCPIPLibs:o.inetlibzm	${CPFLAGS}

dirs:
	${MKDIR} $(LIBDIR)
	${MKDIR} $(LIBDIR).TCPIPLibs
	${MKDIR} $(LIBDIR).TCPIPLibs.o

local_dirs:
        ${MKDIR} o
        ${MKDIR} z
        Create local_dirs

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} z.* ${WFLAGS}
	${RM} local_dirs
	${RM} inetlib
	${RM} inetlibzm


# Dynamic dependencies:
o.gethent:	c.gethent
o.gethent:	TCPIPLibs:h.netdb
o.gethent:	TCPIPLibs:sys.h.cdefs
o.gethent:	TCPIPLibs:sys.h.types
o.gethent:	TCPIPLibs:sys.h.cdefs
o.gethent:	TCPIPLibs:machine.h.endian
o.gethent:	TCPIPLibs:machine.h.ansi
o.gethent:	TCPIPLibs:machine.h.types
o.gethent:	TCPIPLibs:sys.h.socket
o.gethent:	TCPIPLibs:sys.h.cdefs
o.gethent:	h.inetlib
o.gethent:	TCPIPLibs:sys.h.types
o.gethent:	TCPIPLibs:netinet.h.in
o.gethnad:	c.gethnad
o.gethnad:	C:h.kernel
o.gethnad:	C:h.swis
o.gethnad:	TCPIPLibs:h.netdb
o.gethnad:	TCPIPLibs:sys.h.cdefs
o.gethnad:	h.inetlib
o.gethnad:	TCPIPLibs:sys.h.types
o.gethnad:	TCPIPLibs:sys.h.cdefs
o.gethnad:	TCPIPLibs:machine.h.endian
o.gethnad:	TCPIPLibs:machine.h.ansi
o.gethnad:	TCPIPLibs:machine.h.types
o.gethnad:	TCPIPLibs:netinet.h.in
o.getnbyad:	c.getnbyad
o.getnbyad:	TCPIPLibs:h.netdb
o.getnbyad:	TCPIPLibs:sys.h.cdefs
o.getnbyad:	h.inetlib
o.getnbyad:	TCPIPLibs:sys.h.types
o.getnbyad:	TCPIPLibs:sys.h.cdefs
o.getnbyad:	TCPIPLibs:machine.h.endian
o.getnbyad:	TCPIPLibs:machine.h.ansi
o.getnbyad:	TCPIPLibs:machine.h.types
o.getnbyad:	TCPIPLibs:netinet.h.in
o.getnbyna:	c.getnbyna
o.getnbyna:	TCPIPLibs:h.netdb
o.getnbyna:	TCPIPLibs:sys.h.cdefs
o.getnbyna:	h.inetlib
o.getnbyna:	TCPIPLibs:sys.h.types
o.getnbyna:	TCPIPLibs:sys.h.cdefs
o.getnbyna:	TCPIPLibs:machine.h.endian
o.getnbyna:	TCPIPLibs:machine.h.ansi
o.getnbyna:	TCPIPLibs:machine.h.types
o.getnbyna:	TCPIPLibs:netinet.h.in
o.getnent:	c.getnent
o.getnent:	TCPIPLibs:h.netdb
o.getnent:	TCPIPLibs:sys.h.cdefs
o.getnent:	TCPIPLibs:sys.h.types
o.getnent:	TCPIPLibs:sys.h.cdefs
o.getnent:	TCPIPLibs:machine.h.endian
o.getnent:	TCPIPLibs:machine.h.ansi
o.getnent:	TCPIPLibs:machine.h.types
o.getnent:	TCPIPLibs:sys.h.socket
o.getnent:	TCPIPLibs:sys.h.cdefs
o.getnent:	h.inetlib
o.getnent:	TCPIPLibs:sys.h.types
o.getnent:	TCPIPLibs:netinet.h.in
o.getprent:	c.getprent
o.getprent:	TCPIPLibs:h.netdb
o.getprent:	TCPIPLibs:sys.h.cdefs
o.getprent:	TCPIPLibs:sys.h.types
o.getprent:	TCPIPLibs:sys.h.cdefs
o.getprent:	TCPIPLibs:machine.h.endian
o.getprent:	TCPIPLibs:machine.h.ansi
o.getprent:	TCPIPLibs:machine.h.types
o.getprent:	TCPIPLibs:sys.h.socket
o.getprent:	TCPIPLibs:sys.h.cdefs
o.getprent:	h.inetlib
o.getprent:	TCPIPLibs:sys.h.types
o.getprent:	TCPIPLibs:netinet.h.in
o.getprnam:	c.getprnam
o.getprnam:	TCPIPLibs:h.netdb
o.getprnam:	TCPIPLibs:sys.h.cdefs
o.getprnam:	h.inetlib
o.getprnam:	TCPIPLibs:sys.h.types
o.getprnam:	TCPIPLibs:sys.h.cdefs
o.getprnam:	TCPIPLibs:machine.h.endian
o.getprnam:	TCPIPLibs:machine.h.ansi
o.getprnam:	TCPIPLibs:machine.h.types
o.getprnam:	TCPIPLibs:netinet.h.in
o.getproto:	c.getproto
o.getproto:	TCPIPLibs:h.netdb
o.getproto:	TCPIPLibs:sys.h.cdefs
o.getproto:	h.inetlib
o.getproto:	TCPIPLibs:sys.h.types
o.getproto:	TCPIPLibs:sys.h.cdefs
o.getproto:	TCPIPLibs:machine.h.endian
o.getproto:	TCPIPLibs:machine.h.ansi
o.getproto:	TCPIPLibs:machine.h.types
o.getproto:	TCPIPLibs:netinet.h.in
o.getsbyna:	c.getsbyna
o.getsbyna:	TCPIPLibs:h.netdb
o.getsbyna:	TCPIPLibs:sys.h.cdefs
o.getsbyna:	h.inetlib
o.getsbyna:	TCPIPLibs:sys.h.types
o.getsbyna:	TCPIPLibs:sys.h.cdefs
o.getsbyna:	TCPIPLibs:machine.h.endian
o.getsbyna:	TCPIPLibs:machine.h.ansi
o.getsbyna:	TCPIPLibs:machine.h.types
o.getsbyna:	TCPIPLibs:netinet.h.in
o.getsbypo:	c.getsbypo
o.getsbypo:	TCPIPLibs:h.netdb
o.getsbypo:	TCPIPLibs:sys.h.cdefs
o.getsbypo:	h.inetlib
o.getsbypo:	TCPIPLibs:sys.h.types
o.getsbypo:	TCPIPLibs:sys.h.cdefs
o.getsbypo:	TCPIPLibs:machine.h.endian
o.getsbypo:	TCPIPLibs:machine.h.ansi
o.getsbypo:	TCPIPLibs:machine.h.types
o.getsbypo:	TCPIPLibs:netinet.h.in
o.getsent:	c.getsent
o.getsent:	TCPIPLibs:h.netdb
o.getsent:	TCPIPLibs:sys.h.cdefs
o.getsent:	TCPIPLibs:sys.h.types
o.getsent:	TCPIPLibs:sys.h.cdefs
o.getsent:	TCPIPLibs:machine.h.endian
o.getsent:	TCPIPLibs:machine.h.ansi
o.getsent:	TCPIPLibs:machine.h.types
o.getsent:	TCPIPLibs:sys.h.socket
o.getsent:	TCPIPLibs:sys.h.cdefs
o.getsent:	h.inetlib
o.getsent:	TCPIPLibs:sys.h.types
o.getsent:	TCPIPLibs:netinet.h.in
o.htonl:	c.htonl
o.htonl:	TCPIPLibs:sys.h.types
o.htonl:	TCPIPLibs:sys.h.cdefs
o.htonl:	TCPIPLibs:machine.h.endian
o.htonl:	TCPIPLibs:machine.h.ansi
o.htonl:	TCPIPLibs:machine.h.types
o.htonl:	h.inetlib
o.htonl:	TCPIPLibs:sys.h.types
o.htonl:	TCPIPLibs:netinet.h.in
o.inet_addr:	c.inet_addr
o.inet_addr:	TCPIPLibs:sys.h.param
o.inet_addr:	TCPIPLibs:sys.h.types
o.inet_addr:	TCPIPLibs:sys.h.cdefs
o.inet_addr:	TCPIPLibs:machine.h.endian
o.inet_addr:	TCPIPLibs:machine.h.ansi
o.inet_addr:	TCPIPLibs:machine.h.types
o.inet_addr:	TCPIPLibs:sys.h.syslimits
o.inet_addr:	TCPIPLibs:sys.h.signal
o.inet_addr:	TCPIPLibs:machine.h.signal
o.inet_addr:	TCPIPLibs:sys.h.cdefs
o.inet_addr:	TCPIPLibs:machine.h.param
o.inet_addr:	TCPIPLibs:machine.h.limits
o.inet_addr:	TCPIPLibs:netinet.h.in
o.inet_addr:	TCPIPLibs:arpa.h.inet
o.inet_addr:	TCPIPLibs:sys.h.cdefs
o.inet_addr:	h.inetlib
o.inet_addr:	TCPIPLibs:sys.h.types
o.inet_addr:	TCPIPLibs:netinet.h.in
o.inet_lnaof:	c.inet_lnaof
o.inet_lnaof:	TCPIPLibs:sys.h.types
o.inet_lnaof:	TCPIPLibs:sys.h.cdefs
o.inet_lnaof:	TCPIPLibs:machine.h.endian
o.inet_lnaof:	TCPIPLibs:machine.h.ansi
o.inet_lnaof:	TCPIPLibs:machine.h.types
o.inet_lnaof:	TCPIPLibs:netinet.h.in
o.inet_lnaof:	h.inetlib
o.inet_lnaof:	TCPIPLibs:sys.h.types
o.inet_lnaof:	TCPIPLibs:netinet.h.in
o.inet_maddr:	c.inet_maddr
o.inet_maddr:	TCPIPLibs:sys.h.param
o.inet_maddr:	TCPIPLibs:sys.h.types
o.inet_maddr:	TCPIPLibs:sys.h.cdefs
o.inet_maddr:	TCPIPLibs:machine.h.endian
o.inet_maddr:	TCPIPLibs:machine.h.ansi
o.inet_maddr:	TCPIPLibs:machine.h.types
o.inet_maddr:	TCPIPLibs:sys.h.syslimits
o.inet_maddr:	TCPIPLibs:sys.h.signal
o.inet_maddr:	TCPIPLibs:machine.h.signal
o.inet_maddr:	TCPIPLibs:sys.h.cdefs
o.inet_maddr:	TCPIPLibs:machine.h.param
o.inet_maddr:	TCPIPLibs:machine.h.limits
o.inet_maddr:	TCPIPLibs:netinet.h.in
o.inet_maddr:	TCPIPLibs:arpa.h.inet
o.inet_maddr:	TCPIPLibs:sys.h.cdefs
o.inet_maddr:	h.inetlib
o.inet_maddr:	TCPIPLibs:sys.h.types
o.inet_maddr:	TCPIPLibs:netinet.h.in
o.inet_nof:	c.inet_nof
o.inet_nof:	TCPIPLibs:sys.h.types
o.inet_nof:	TCPIPLibs:sys.h.cdefs
o.inet_nof:	TCPIPLibs:machine.h.endian
o.inet_nof:	TCPIPLibs:machine.h.ansi
o.inet_nof:	TCPIPLibs:machine.h.types
o.inet_nof:	TCPIPLibs:netinet.h.in
o.inet_nof:	h.inetlib
o.inet_nof:	TCPIPLibs:sys.h.types
o.inet_nof:	TCPIPLibs:netinet.h.in
o.inet_ntoa:	c.inet_ntoa
o.inet_ntoa:	TCPIPLibs:sys.h.types
o.inet_ntoa:	TCPIPLibs:sys.h.cdefs
o.inet_ntoa:	TCPIPLibs:machine.h.endian
o.inet_ntoa:	TCPIPLibs:machine.h.ansi
o.inet_ntoa:	TCPIPLibs:machine.h.types
o.inet_ntoa:	TCPIPLibs:netinet.h.in
o.inet_ntoa:	h.inetlib
o.inet_ntoa:	TCPIPLibs:sys.h.types
o.inet_ntoa:	TCPIPLibs:netinet.h.in
o.inet_nwork:	c.inet_nwork
o.inet_nwork:	TCPIPLibs:sys.h.types
o.inet_nwork:	TCPIPLibs:sys.h.cdefs
o.inet_nwork:	TCPIPLibs:machine.h.endian
o.inet_nwork:	TCPIPLibs:machine.h.ansi
o.inet_nwork:	TCPIPLibs:machine.h.types
o.inet_nwork:	h.inetlib
o.inet_nwork:	TCPIPLibs:sys.h.types
o.inet_nwork:	TCPIPLibs:netinet.h.in
o.rresvport:	c.rresvport
o.rresvport:	TCPIPLibs:sys.h.errno
o.rresvport:	C:h.kernel
o.rresvport:	TCPIPLibs:sys.h.types
o.rresvport:	TCPIPLibs:sys.h.cdefs
o.rresvport:	TCPIPLibs:machine.h.endian
o.rresvport:	TCPIPLibs:machine.h.ansi
o.rresvport:	TCPIPLibs:machine.h.types
o.rresvport:	TCPIPLibs:sys.h.socket
o.rresvport:	TCPIPLibs:sys.h.cdefs
o.rresvport:	TCPIPLibs:netinet.h.in
o.rresvport:	h.inetlib
o.rresvport:	TCPIPLibs:sys.h.types
o.rresvport:	TCPIPLibs:netinet.h.in
o.linkaddr:	c.linkaddr
o.linkaddr:	TCPIPLibs:sys.h.types
o.linkaddr:	TCPIPLibs:sys.h.cdefs
o.linkaddr:	TCPIPLibs:machine.h.endian
o.linkaddr:	TCPIPLibs:machine.h.ansi
o.linkaddr:	TCPIPLibs:machine.h.types
o.linkaddr:	TCPIPLibs:sys.h.socket
o.linkaddr:	TCPIPLibs:sys.h.cdefs
o.linkaddr:	TCPIPLibs:net.h.if_dl
o.linkaddr:	TCPIPLibs:sys.h.cdefs
o.linkaddr:	h.inetlib
o.linkaddr:	TCPIPLibs:sys.h.types
o.linkaddr:	TCPIPLibs:netinet.h.in
o.ns_addr:	c.ns_addr
o.ns_addr:	TCPIPLibs:sys.h.param
o.ns_addr:	TCPIPLibs:sys.h.types
o.ns_addr:	TCPIPLibs:sys.h.cdefs
o.ns_addr:	TCPIPLibs:machine.h.endian
o.ns_addr:	TCPIPLibs:machine.h.ansi
o.ns_addr:	TCPIPLibs:machine.h.types
o.ns_addr:	TCPIPLibs:sys.h.syslimits
o.ns_addr:	TCPIPLibs:sys.h.signal
o.ns_addr:	TCPIPLibs:machine.h.signal
o.ns_addr:	TCPIPLibs:sys.h.cdefs
o.ns_addr:	TCPIPLibs:machine.h.param
o.ns_addr:	TCPIPLibs:machine.h.limits
o.ns_addr:	TCPIPLibs:netns.h.ns
o.ns_addr:	TCPIPLibs:sys.h.cdefs
o.ns_addr:	h.inetlib
o.ns_addr:	TCPIPLibs:sys.h.types
o.ns_addr:	TCPIPLibs:netinet.h.in
o.ns_ntoa:	c.ns_ntoa
o.ns_ntoa:	TCPIPLibs:sys.h.param
o.ns_ntoa:	TCPIPLibs:sys.h.types
o.ns_ntoa:	TCPIPLibs:sys.h.cdefs
o.ns_ntoa:	TCPIPLibs:machine.h.endian
o.ns_ntoa:	TCPIPLibs:machine.h.ansi
o.ns_ntoa:	TCPIPLibs:machine.h.types
o.ns_ntoa:	TCPIPLibs:sys.h.syslimits
o.ns_ntoa:	TCPIPLibs:sys.h.signal
o.ns_ntoa:	TCPIPLibs:machine.h.signal
o.ns_ntoa:	TCPIPLibs:sys.h.cdefs
o.ns_ntoa:	TCPIPLibs:machine.h.param
o.ns_ntoa:	TCPIPLibs:machine.h.limits
o.ns_ntoa:	TCPIPLibs:netns.h.ns
o.ns_ntoa:	TCPIPLibs:sys.h.cdefs
o.iso_addr:	c.iso_addr
o.iso_addr:	TCPIPLibs:sys.h.types
o.iso_addr:	TCPIPLibs:sys.h.cdefs
o.iso_addr:	TCPIPLibs:machine.h.endian
o.iso_addr:	TCPIPLibs:machine.h.ansi
o.iso_addr:	TCPIPLibs:machine.h.types
o.iso_addr:	TCPIPLibs:netiso.h.iso
o.iso_addr:	TCPIPLibs:netinet.h.in
o.iso_addr:	TCPIPLibs:sys.h.cdefs
o.iso_addr:	h.inetlib
o.iso_addr:	TCPIPLibs:sys.h.types
o.iso_addr:	TCPIPLibs:netinet.h.in
z.gethent:	c.gethent
z.gethent:	TCPIPLibs:h.netdb
z.gethent:	TCPIPLibs:sys.h.cdefs
z.gethent:	TCPIPLibs:sys.h.types
z.gethent:	TCPIPLibs:sys.h.cdefs
z.gethent:	TCPIPLibs:machine.h.endian
z.gethent:	TCPIPLibs:machine.h.ansi
z.gethent:	TCPIPLibs:machine.h.types
z.gethent:	TCPIPLibs:sys.h.socket
z.gethent:	TCPIPLibs:sys.h.cdefs
z.gethent:	h.inetlib
z.gethent:	TCPIPLibs:sys.h.types
z.gethent:	TCPIPLibs:netinet.h.in
z.gethnad:	c.gethnad
z.gethnad:	C:h.kernel
z.gethnad:	C:h.swis
z.gethnad:	TCPIPLibs:h.netdb
z.gethnad:	TCPIPLibs:sys.h.cdefs
z.gethnad:	h.inetlib
z.gethnad:	TCPIPLibs:sys.h.types
z.gethnad:	TCPIPLibs:sys.h.cdefs
z.gethnad:	TCPIPLibs:machine.h.endian
z.gethnad:	TCPIPLibs:machine.h.ansi
z.gethnad:	TCPIPLibs:machine.h.types
z.gethnad:	TCPIPLibs:netinet.h.in
z.getnbyad:	c.getnbyad
z.getnbyad:	TCPIPLibs:h.netdb
z.getnbyad:	TCPIPLibs:sys.h.cdefs
z.getnbyad:	h.inetlib
z.getnbyad:	TCPIPLibs:sys.h.types
z.getnbyad:	TCPIPLibs:sys.h.cdefs
z.getnbyad:	TCPIPLibs:machine.h.endian
z.getnbyad:	TCPIPLibs:machine.h.ansi
z.getnbyad:	TCPIPLibs:machine.h.types
z.getnbyad:	TCPIPLibs:netinet.h.in
z.getnbyna:	c.getnbyna
z.getnbyna:	TCPIPLibs:h.netdb
z.getnbyna:	TCPIPLibs:sys.h.cdefs
z.getnbyna:	h.inetlib
z.getnbyna:	TCPIPLibs:sys.h.types
z.getnbyna:	TCPIPLibs:sys.h.cdefs
z.getnbyna:	TCPIPLibs:machine.h.endian
z.getnbyna:	TCPIPLibs:machine.h.ansi
z.getnbyna:	TCPIPLibs:machine.h.types
z.getnbyna:	TCPIPLibs:netinet.h.in
z.getnent:	c.getnent
z.getnent:	TCPIPLibs:h.netdb
z.getnent:	TCPIPLibs:sys.h.cdefs
z.getnent:	TCPIPLibs:sys.h.types
z.getnent:	TCPIPLibs:sys.h.cdefs
z.getnent:	TCPIPLibs:machine.h.endian
z.getnent:	TCPIPLibs:machine.h.ansi
z.getnent:	TCPIPLibs:machine.h.types
z.getnent:	TCPIPLibs:sys.h.socket
z.getnent:	TCPIPLibs:sys.h.cdefs
z.getnent:	h.inetlib
z.getnent:	TCPIPLibs:sys.h.types
z.getnent:	TCPIPLibs:netinet.h.in
z.getprent:	c.getprent
z.getprent:	TCPIPLibs:h.netdb
z.getprent:	TCPIPLibs:sys.h.cdefs
z.getprent:	TCPIPLibs:sys.h.types
z.getprent:	TCPIPLibs:sys.h.cdefs
z.getprent:	TCPIPLibs:machine.h.endian
z.getprent:	TCPIPLibs:machine.h.ansi
z.getprent:	TCPIPLibs:machine.h.types
z.getprent:	TCPIPLibs:sys.h.socket
z.getprent:	TCPIPLibs:sys.h.cdefs
z.getprent:	h.inetlib
z.getprent:	TCPIPLibs:sys.h.types
z.getprent:	TCPIPLibs:netinet.h.in
z.getprnam:	c.getprnam
z.getprnam:	TCPIPLibs:h.netdb
z.getprnam:	TCPIPLibs:sys.h.cdefs
z.getprnam:	h.inetlib
z.getprnam:	TCPIPLibs:sys.h.types
z.getprnam:	TCPIPLibs:sys.h.cdefs
z.getprnam:	TCPIPLibs:machine.h.endian
z.getprnam:	TCPIPLibs:machine.h.ansi
z.getprnam:	TCPIPLibs:machine.h.types
z.getprnam:	TCPIPLibs:netinet.h.in
z.getproto:	c.getproto
z.getproto:	TCPIPLibs:h.netdb
z.getproto:	TCPIPLibs:sys.h.cdefs
z.getproto:	h.inetlib
z.getproto:	TCPIPLibs:sys.h.types
z.getproto:	TCPIPLibs:sys.h.cdefs
z.getproto:	TCPIPLibs:machine.h.endian
z.getproto:	TCPIPLibs:machine.h.ansi
z.getproto:	TCPIPLibs:machine.h.types
z.getproto:	TCPIPLibs:netinet.h.in
z.getsbyna:	c.getsbyna
z.getsbyna:	TCPIPLibs:h.netdb
z.getsbyna:	TCPIPLibs:sys.h.cdefs
z.getsbyna:	h.inetlib
z.getsbyna:	TCPIPLibs:sys.h.types
z.getsbyna:	TCPIPLibs:sys.h.cdefs
z.getsbyna:	TCPIPLibs:machine.h.endian
z.getsbyna:	TCPIPLibs:machine.h.ansi
z.getsbyna:	TCPIPLibs:machine.h.types
z.getsbyna:	TCPIPLibs:netinet.h.in
z.getsbypo:	c.getsbypo
z.getsbypo:	TCPIPLibs:h.netdb
z.getsbypo:	TCPIPLibs:sys.h.cdefs
z.getsbypo:	h.inetlib
z.getsbypo:	TCPIPLibs:sys.h.types
z.getsbypo:	TCPIPLibs:sys.h.cdefs
z.getsbypo:	TCPIPLibs:machine.h.endian
z.getsbypo:	TCPIPLibs:machine.h.ansi
z.getsbypo:	TCPIPLibs:machine.h.types
z.getsbypo:	TCPIPLibs:netinet.h.in
z.getsent:	c.getsent
z.getsent:	TCPIPLibs:h.netdb
z.getsent:	TCPIPLibs:sys.h.cdefs
z.getsent:	TCPIPLibs:sys.h.types
z.getsent:	TCPIPLibs:sys.h.cdefs
z.getsent:	TCPIPLibs:machine.h.endian
z.getsent:	TCPIPLibs:machine.h.ansi
z.getsent:	TCPIPLibs:machine.h.types
z.getsent:	TCPIPLibs:sys.h.socket
z.getsent:	TCPIPLibs:sys.h.cdefs
z.getsent:	h.inetlib
z.getsent:	TCPIPLibs:sys.h.types
z.getsent:	TCPIPLibs:netinet.h.in
z.htonl:	c.htonl
z.htonl:	TCPIPLibs:sys.h.types
z.htonl:	TCPIPLibs:sys.h.cdefs
z.htonl:	TCPIPLibs:machine.h.endian
z.htonl:	TCPIPLibs:machine.h.ansi
z.htonl:	TCPIPLibs:machine.h.types
z.htonl:	h.inetlib
z.htonl:	TCPIPLibs:sys.h.types
z.htonl:	TCPIPLibs:netinet.h.in
z.inet_addr:	c.inet_addr
z.inet_addr:	TCPIPLibs:sys.h.param
z.inet_addr:	TCPIPLibs:sys.h.types
z.inet_addr:	TCPIPLibs:sys.h.cdefs
z.inet_addr:	TCPIPLibs:machine.h.endian
z.inet_addr:	TCPIPLibs:machine.h.ansi
z.inet_addr:	TCPIPLibs:machine.h.types
z.inet_addr:	TCPIPLibs:sys.h.syslimits
z.inet_addr:	TCPIPLibs:sys.h.signal
z.inet_addr:	TCPIPLibs:machine.h.signal
z.inet_addr:	TCPIPLibs:sys.h.cdefs
z.inet_addr:	TCPIPLibs:machine.h.param
z.inet_addr:	TCPIPLibs:machine.h.limits
z.inet_addr:	TCPIPLibs:netinet.h.in
z.inet_addr:	TCPIPLibs:arpa.h.inet
z.inet_addr:	TCPIPLibs:sys.h.cdefs
z.inet_addr:	h.inetlib
z.inet_addr:	TCPIPLibs:sys.h.types
z.inet_addr:	TCPIPLibs:netinet.h.in
z.inet_lnaof:	c.inet_lnaof
z.inet_lnaof:	TCPIPLibs:sys.h.types
z.inet_lnaof:	TCPIPLibs:sys.h.cdefs
z.inet_lnaof:	TCPIPLibs:machine.h.endian
z.inet_lnaof:	TCPIPLibs:machine.h.ansi
z.inet_lnaof:	TCPIPLibs:machine.h.types
z.inet_lnaof:	TCPIPLibs:netinet.h.in
z.inet_lnaof:	h.inetlib
z.inet_lnaof:	TCPIPLibs:sys.h.types
z.inet_lnaof:	TCPIPLibs:netinet.h.in
z.inet_maddr:	c.inet_maddr
z.inet_maddr:	TCPIPLibs:sys.h.param
z.inet_maddr:	TCPIPLibs:sys.h.types
z.inet_maddr:	TCPIPLibs:sys.h.cdefs
z.inet_maddr:	TCPIPLibs:machine.h.endian
z.inet_maddr:	TCPIPLibs:machine.h.ansi
z.inet_maddr:	TCPIPLibs:machine.h.types
z.inet_maddr:	TCPIPLibs:sys.h.syslimits
z.inet_maddr:	TCPIPLibs:sys.h.signal
z.inet_maddr:	TCPIPLibs:machine.h.signal
z.inet_maddr:	TCPIPLibs:sys.h.cdefs
z.inet_maddr:	TCPIPLibs:machine.h.param
z.inet_maddr:	TCPIPLibs:machine.h.limits
z.inet_maddr:	TCPIPLibs:netinet.h.in
z.inet_maddr:	TCPIPLibs:arpa.h.inet
z.inet_maddr:	TCPIPLibs:sys.h.cdefs
z.inet_maddr:	h.inetlib
z.inet_maddr:	TCPIPLibs:sys.h.types
z.inet_maddr:	TCPIPLibs:netinet.h.in
z.inet_nof:	c.inet_nof
z.inet_nof:	TCPIPLibs:sys.h.types
z.inet_nof:	TCPIPLibs:sys.h.cdefs
z.inet_nof:	TCPIPLibs:machine.h.endian
z.inet_nof:	TCPIPLibs:machine.h.ansi
z.inet_nof:	TCPIPLibs:machine.h.types
z.inet_nof:	TCPIPLibs:netinet.h.in
z.inet_nof:	h.inetlib
z.inet_nof:	TCPIPLibs:sys.h.types
z.inet_nof:	TCPIPLibs:netinet.h.in
z.inet_ntoa:	c.inet_ntoa
z.inet_ntoa:	TCPIPLibs:sys.h.types
z.inet_ntoa:	TCPIPLibs:sys.h.cdefs
z.inet_ntoa:	TCPIPLibs:machine.h.endian
z.inet_ntoa:	TCPIPLibs:machine.h.ansi
z.inet_ntoa:	TCPIPLibs:machine.h.types
z.inet_ntoa:	TCPIPLibs:netinet.h.in
z.inet_ntoa:	h.inetlib
z.inet_ntoa:	TCPIPLibs:sys.h.types
z.inet_ntoa:	TCPIPLibs:netinet.h.in
z.inet_nwork:	c.inet_nwork
z.inet_nwork:	TCPIPLibs:sys.h.types
z.inet_nwork:	TCPIPLibs:sys.h.cdefs
z.inet_nwork:	TCPIPLibs:machine.h.endian
z.inet_nwork:	TCPIPLibs:machine.h.ansi
z.inet_nwork:	TCPIPLibs:machine.h.types
z.inet_nwork:	h.inetlib
z.inet_nwork:	TCPIPLibs:sys.h.types
z.inet_nwork:	TCPIPLibs:netinet.h.in
z.rresvport:	c.rresvport
z.rresvport:	TCPIPLibs:sys.h.errno
z.rresvport:	C:h.kernel
z.rresvport:	TCPIPLibs:sys.h.types
z.rresvport:	TCPIPLibs:sys.h.cdefs
z.rresvport:	TCPIPLibs:machine.h.endian
z.rresvport:	TCPIPLibs:machine.h.ansi
z.rresvport:	TCPIPLibs:machine.h.types
z.rresvport:	TCPIPLibs:sys.h.socket
z.rresvport:	TCPIPLibs:sys.h.cdefs
z.rresvport:	TCPIPLibs:netinet.h.in
z.rresvport:	h.inetlib
z.rresvport:	TCPIPLibs:sys.h.types
z.rresvport:	TCPIPLibs:netinet.h.in
z.linkaddr:	c.linkaddr
z.linkaddr:	TCPIPLibs:sys.h.types
z.linkaddr:	TCPIPLibs:sys.h.cdefs
z.linkaddr:	TCPIPLibs:machine.h.endian
z.linkaddr:	TCPIPLibs:machine.h.ansi
z.linkaddr:	TCPIPLibs:machine.h.types
z.linkaddr:	TCPIPLibs:sys.h.socket
z.linkaddr:	TCPIPLibs:sys.h.cdefs
z.linkaddr:	TCPIPLibs:net.h.if_dl
z.linkaddr:	TCPIPLibs:sys.h.cdefs
z.linkaddr:	h.inetlib
z.linkaddr:	TCPIPLibs:sys.h.types
z.linkaddr:	TCPIPLibs:netinet.h.in
z.ns_addr:	c.ns_addr
z.ns_addr:	TCPIPLibs:sys.h.param
z.ns_addr:	TCPIPLibs:sys.h.types
z.ns_addr:	TCPIPLibs:sys.h.cdefs
z.ns_addr:	TCPIPLibs:machine.h.endian
z.ns_addr:	TCPIPLibs:machine.h.ansi
z.ns_addr:	TCPIPLibs:machine.h.types
z.ns_addr:	TCPIPLibs:sys.h.syslimits
z.ns_addr:	TCPIPLibs:sys.h.signal
z.ns_addr:	TCPIPLibs:machine.h.signal
z.ns_addr:	TCPIPLibs:sys.h.cdefs
z.ns_addr:	TCPIPLibs:machine.h.param
z.ns_addr:	TCPIPLibs:machine.h.limits
z.ns_addr:	TCPIPLibs:netns.h.ns
z.ns_addr:	TCPIPLibs:sys.h.cdefs
z.ns_addr:	h.inetlib
z.ns_addr:	TCPIPLibs:sys.h.types
z.ns_addr:	TCPIPLibs:netinet.h.in
z.ns_ntoa:	c.ns_ntoa
z.ns_ntoa:	TCPIPLibs:sys.h.param
z.ns_ntoa:	TCPIPLibs:sys.h.types
z.ns_ntoa:	TCPIPLibs:sys.h.cdefs
z.ns_ntoa:	TCPIPLibs:machine.h.endian
z.ns_ntoa:	TCPIPLibs:machine.h.ansi
z.ns_ntoa:	TCPIPLibs:machine.h.types
z.ns_ntoa:	TCPIPLibs:sys.h.syslimits
z.ns_ntoa:	TCPIPLibs:sys.h.signal
z.ns_ntoa:	TCPIPLibs:machine.h.signal
z.ns_ntoa:	TCPIPLibs:sys.h.cdefs
z.ns_ntoa:	TCPIPLibs:machine.h.param
z.ns_ntoa:	TCPIPLibs:machine.h.limits
z.ns_ntoa:	TCPIPLibs:netns.h.ns
z.ns_ntoa:	TCPIPLibs:sys.h.cdefs
z.iso_addr:	c.iso_addr
z.iso_addr:	TCPIPLibs:sys.h.types
z.iso_addr:	TCPIPLibs:sys.h.cdefs
z.iso_addr:	TCPIPLibs:machine.h.endian
z.iso_addr:	TCPIPLibs:machine.h.ansi
z.iso_addr:	TCPIPLibs:machine.h.types
z.iso_addr:	TCPIPLibs:netiso.h.iso
z.iso_addr:	TCPIPLibs:netinet.h.in
z.iso_addr:	TCPIPLibs:sys.h.cdefs
z.iso_addr:	h.inetlib
z.iso_addr:	TCPIPLibs:sys.h.types
z.iso_addr:	TCPIPLibs:netinet.h.in
