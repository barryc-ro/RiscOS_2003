
TARGET=         Pong


INCLUDES=	C:,TCPIPLibs:
CC=		cc
RM=             remove
WFLAGS=         fr~c~v
WIPE=           -wipe

CFLAGS=		-pcc -I$(INCLUDES) -c ${THROWBACK} -depend !Depend

OBJS=		o.${TARGET}

LIBS=		C:o.Stubs TCPIPLibs:o.Unixlib TCPIPLibs:o.Inetlib \
		TCPIPLibs:o.Socklib5

.SUFFIXES:	.c .o
.c.o:;		$(CC) $(CFLAGS) -o $@ $<

${TARGET}:	$(OBJS) $(LIBS)
		Link -o $@ $(OBJS) $(LIBS)
		Squeeze $@

clean:;		${RM} ${TARGET}
		${WIPE} o.* ${WFLAGS}
	
# Dynamic dependencies:
o.Pong:	c.Pong
o.Pong:	TCPIPLibs:h.riscos
o.Pong:	TCPIPLibs:sys.h.errno
o.Pong:	C:h.kernel
o.Pong:	TCPIPLibs:h.netdb
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	TCPIPLibs:sys.h.types
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	TCPIPLibs:machine.h.endian
o.Pong:	TCPIPLibs:machine.h.ansi
o.Pong:	TCPIPLibs:machine.h.types
o.Pong:	TCPIPLibs:sys.h.time
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	TCPIPLibs:sys.h.param
o.Pong:	TCPIPLibs:sys.h.types
o.Pong:	TCPIPLibs:sys.h.syslimits
o.Pong:	TCPIPLibs:sys.h.signal
o.Pong:	TCPIPLibs:machine.h.signal
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	TCPIPLibs:machine.h.param
o.Pong:	TCPIPLibs:machine.h.limits
o.Pong:	TCPIPLibs:sys.h.socket
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	TCPIPLibs:sys.h.file
o.Pong:	TCPIPLibs:sys.h.fcntl
o.Pong:	TCPIPLibs:sys.h.types
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	TCPIPLibs:sys.h.unistd
o.Pong:	TCPIPLibs:sys.h.ioctl
o.Pong:	TCPIPLibs:sys.h.ttycom
o.Pong:	TCPIPLibs:sys.h.ioccom
o.Pong:	TCPIPLibs:sys.h.ioccom
o.Pong:	TCPIPLibs:sys.h.filio
o.Pong:	TCPIPLibs:sys.h.ioccom
o.Pong:	TCPIPLibs:sys.h.sockio
o.Pong:	TCPIPLibs:sys.h.ioccom
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	TCPIPLibs:net.h.if
o.Pong:	TCPIPLibs:sys.h.socket
o.Pong:	TCPIPLibs:sys.h.time
o.Pong:	TCPIPLibs:net.h.if_arp
o.Pong:	TCPIPLibs:sys.h.socket
o.Pong:	TCPIPLibs:netinet.h.in
o.Pong:	TCPIPLibs:netinet.h.in_systm
o.Pong:	TCPIPLibs:netinet.h.ip
o.Pong:	TCPIPLibs:netinet.h.ip_icmp
o.Pong:	TCPIPLibs:netinet.h.udp
o.Pong:	TCPIPLibs:h.inetlib
o.Pong:	TCPIPLibs:sys.h.types
o.Pong:	TCPIPLibs:netinet.h.in
o.Pong:	TCPIPLibs:h.socklib
o.Pong:	TCPIPLibs:sys.h.types
o.Pong:	TCPIPLibs:sys.h.socket
o.Pong:	TCPIPLibs:sys.h.stat
o.Pong:	TCPIPLibs:sys.h.time
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	TCPIPLibs:sys.h.time
o.Pong:	TCPIPLibs:sys.h.uio
o.Pong:	TCPIPLibs:sys.h.cdefs
o.Pong:	C:h.kernel
o.Pong:	TCPIPLibs:h.unixlib
