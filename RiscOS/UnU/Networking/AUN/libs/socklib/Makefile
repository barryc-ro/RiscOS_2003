# -*-Text-*-
#
# $Header$
# $Source$
#
# Copyright (c) 1994 Acorn Computers Ltd., Cambridge, England
#
# Makefile for socklib.a
#
# $Log$
# Revision 1.1  95/03/08  11:38:13  kwelton
# Initial revision
# 
#

INCLUDES=       InetInc:

CC=             cc
AS=             objasm
LD=             link
LIB=            libfile

CMHG=           cmhg

AFLAGS=         -I<Hdr$Dir>.Global.,<Hdr$Dir>.Interface. -Stamp -quit
CFLAGS=         -pcc -zps1 -I${INCLUDES},. -DINET

CCZM=           ${CC} ${CFLAGS} -zM -ff

RM=             remove
WFLAGS=         ~c~v
WIPE=           -wipe

CFILES=         accept.c getstablesize.c select.c socket.c socketwrite.c \
                bind.c listen.c send.c socketclose.c socketwritev.c \
                connect.c makecall.c sendmsg.c socketioctl.c \
                getpeername.c recv.c sendto.c socketread.c getsockname.c \
                recvfrom.c setsockopt.c socketreadv.c getsockopt.c \
                recvmsg.c shutdown.c socketstat.c

OFILES=         accept.o getstablesize.o select.o socket.o socketwrite.o \
                bind.o listen.o send.o socketclose.o socketwritev.o \
                connect.o makecall.o sendmsg.o socketioctl.o \
                getpeername.o recv.o sendto.o socketread.o getsockname.o \
                recvfrom.o setsockopt.o socketreadv.o getsockopt.o \
                recvmsg.o shutdown.o socketstat.o

ZFILES=         z.accept z.getstablesize z.select z.socket z.socketwrite \
                z.bind z.listen z.send z.socketclose z.socketwritev \
                z.connect z.makecall z.sendmsg z.socketioctl \
                z.getpeername z.recv z.sendto z.socketread z.getsockname \
                z.recvfrom z.setsockopt z.socketreadv z.getsockopt \
                z.recvmsg z.shutdown z.socketstat

.SUFFIXES:      .z .o .c .s

.c.o:   ;       ${CC} ${CFLAGS} -depend !Depend -c -o $@ $<
.c.z:   ;       ${CCZM} -depend !Depend -c -o $@ $<

all:            socklib socklibzm

socklib:        ${OFILES} Makefile
        ${LIB} -c socklib ${OFILES}
        ${LIB} -o socklib

socklibzm:      ${ZFILES} Makefile
        ${LIB} -c socklibzm ${ZFILES}
        ${LIB} -o socklibzm

clean:
        ${RM} socklib
        ${RM} socklibz
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} z.* ${WFLAGS}
        @echo cleaned

# Dynamic dependencies:
o.getsockopt:	c.getsockopt
o.getsockopt:	h.stubs
o.getsockopt:	C:h.kernel
o.getsockopt:	InetInc:sys.h.types
o.getsockopt:	InetInc:sys.h.uio
o.getsockopt:	InetInc:sys.h.errno
o.getsockopt:	InetInc:sys.^.h.errno
o.getsockopt:	C:h.kernel
o.getsockopt:	InetInc:sys.h.socket
o.getsockopt:	InetInc:sys.h.socketvar
o.getsockopt:	InetInc:sys.h.dcistructs
o.recvmsg:	c.recvmsg
o.recvmsg:	h.stubs
o.recvmsg:	C:h.kernel
o.recvmsg:	InetInc:sys.h.types
o.recvmsg:	InetInc:sys.h.uio
o.recvmsg:	InetInc:sys.h.errno
o.recvmsg:	InetInc:sys.^.h.errno
o.recvmsg:	C:h.kernel
o.recvmsg:	InetInc:sys.h.socket
o.recvmsg:	InetInc:sys.h.socketvar
o.recvmsg:	InetInc:sys.h.dcistructs
o.shutdown:	c.shutdown
o.shutdown:	h.stubs
o.shutdown:	C:h.kernel
o.shutdown:	InetInc:sys.h.types
o.shutdown:	InetInc:sys.h.uio
o.shutdown:	InetInc:sys.h.errno
o.shutdown:	InetInc:sys.^.h.errno
o.shutdown:	C:h.kernel
o.shutdown:	InetInc:sys.h.socket
o.shutdown:	InetInc:sys.h.socketvar
o.shutdown:	InetInc:sys.h.dcistructs
o.socketstat:	c.socketstat
o.socketstat:	h.stubs
o.socketstat:	C:h.kernel
o.socketstat:	InetInc:sys.h.types
o.socketstat:	InetInc:sys.h.uio
o.socketstat:	InetInc:sys.h.errno
o.socketstat:	InetInc:sys.^.h.errno
o.socketstat:	C:h.kernel
o.socketstat:	InetInc:sys.h.socket
o.socketstat:	InetInc:sys.h.socketvar
o.socketstat:	InetInc:sys.h.dcistructs
z.getsockopt:	c.getsockopt
z.getsockopt:	h.stubs
z.getsockopt:	C:h.kernel
z.getsockopt:	InetInc:sys.h.types
z.getsockopt:	InetInc:sys.h.uio
z.getsockopt:	InetInc:sys.h.errno
z.getsockopt:	InetInc:sys.^.h.errno
z.getsockopt:	C:h.kernel
z.getsockopt:	InetInc:sys.h.socket
z.getsockopt:	InetInc:sys.h.socketvar
z.getsockopt:	InetInc:sys.h.dcistructs
z.recvmsg:	c.recvmsg
z.recvmsg:	h.stubs
z.recvmsg:	C:h.kernel
z.recvmsg:	InetInc:sys.h.types
z.recvmsg:	InetInc:sys.h.uio
z.recvmsg:	InetInc:sys.h.errno
z.recvmsg:	InetInc:sys.^.h.errno
z.recvmsg:	C:h.kernel
z.recvmsg:	InetInc:sys.h.socket
z.recvmsg:	InetInc:sys.h.socketvar
z.recvmsg:	InetInc:sys.h.dcistructs
z.shutdown:	c.shutdown
z.shutdown:	h.stubs
z.shutdown:	C:h.kernel
z.shutdown:	InetInc:sys.h.types
z.shutdown:	InetInc:sys.h.uio
z.shutdown:	InetInc:sys.h.errno
z.shutdown:	InetInc:sys.^.h.errno
z.shutdown:	C:h.kernel
z.shutdown:	InetInc:sys.h.socket
z.shutdown:	InetInc:sys.h.socketvar
z.shutdown:	InetInc:sys.h.dcistructs
z.socketstat:	c.socketstat
z.socketstat:	h.stubs
z.socketstat:	C:h.kernel
z.socketstat:	InetInc:sys.h.types
z.socketstat:	InetInc:sys.h.uio
z.socketstat:	InetInc:sys.h.errno
z.socketstat:	InetInc:sys.^.h.errno
z.socketstat:	C:h.kernel
z.socketstat:	InetInc:sys.h.socket
z.socketstat:	InetInc:sys.h.socketvar
z.socketstat:	InetInc:sys.h.dcistructs
