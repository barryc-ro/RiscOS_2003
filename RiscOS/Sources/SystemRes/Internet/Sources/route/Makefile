
TARGET=         Route


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
o.Route:	c.Route
o.Route:	TCPIPLibs:sys.h.param
o.Route:	TCPIPLibs:sys.h.types
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:machine.h.endian
o.Route:	TCPIPLibs:machine.h.ansi
o.Route:	TCPIPLibs:machine.h.types
o.Route:	TCPIPLibs:sys.h.syslimits
o.Route:	TCPIPLibs:sys.h.signal
o.Route:	TCPIPLibs:machine.h.signal
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:machine.h.param
o.Route:	TCPIPLibs:machine.h.limits
o.Route:	TCPIPLibs:sys.h.file
o.Route:	TCPIPLibs:sys.h.fcntl
o.Route:	TCPIPLibs:sys.h.types
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:sys.h.unistd
o.Route:	TCPIPLibs:sys.h.socket
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:sys.h.ioctl
o.Route:	TCPIPLibs:sys.h.ttycom
o.Route:	TCPIPLibs:sys.h.ioccom
o.Route:	TCPIPLibs:sys.h.ioccom
o.Route:	TCPIPLibs:sys.h.filio
o.Route:	TCPIPLibs:sys.h.ioccom
o.Route:	TCPIPLibs:sys.h.sockio
o.Route:	TCPIPLibs:sys.h.ioccom
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:sys.h.mbuf
o.Route:	TCPIPLibs:sys.h.sysctl
o.Route:	TCPIPLibs:net.h.if
o.Route:	TCPIPLibs:sys.h.socket
o.Route:	TCPIPLibs:sys.h.time
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:net.h.if_arp
o.Route:	TCPIPLibs:sys.h.socket
o.Route:	TCPIPLibs:net.h.route
o.Route:	TCPIPLibs:net.h.radix
o.Route:	TCPIPLibs:net.h.if_dl
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:netinet.h.in
o.Route:	TCPIPLibs:netns.h.ns
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:netiso.h.iso
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:netccitt.h.x25
o.Route:	TCPIPLibs:arpa.h.inet
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:h.netdb
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:sys.h.errno
o.Route:	C:h.kernel
o.Route:	C:h.swis
o.Route:	TCPIPLibs:h.unixlib
o.Route:	TCPIPLibs:h.unistd
o.Route:	TCPIPLibs:sys.h.cdefs
o.Route:	TCPIPLibs:sys.h.types
o.Route:	TCPIPLibs:sys.h.unistd
o.Route:	TCPIPLibs:h.riscos
o.Route:	h.keywords
