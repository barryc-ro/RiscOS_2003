
TARGET=         ARP


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
o.ARP:	c.ARP
o.ARP:	TCPIPLibs:sys.h.param
o.ARP:	TCPIPLibs:sys.h.types
o.ARP:	TCPIPLibs:sys.h.cdefs
o.ARP:	TCPIPLibs:machine.h.endian
o.ARP:	TCPIPLibs:machine.h.ansi
o.ARP:	TCPIPLibs:machine.h.types
o.ARP:	TCPIPLibs:sys.h.syslimits
o.ARP:	TCPIPLibs:sys.h.signal
o.ARP:	TCPIPLibs:machine.h.signal
o.ARP:	TCPIPLibs:sys.h.cdefs
o.ARP:	TCPIPLibs:machine.h.param
o.ARP:	TCPIPLibs:machine.h.limits
o.ARP:	TCPIPLibs:sys.h.file
o.ARP:	TCPIPLibs:sys.h.fcntl
o.ARP:	TCPIPLibs:sys.h.types
o.ARP:	TCPIPLibs:sys.h.cdefs
o.ARP:	TCPIPLibs:sys.h.unistd
o.ARP:	TCPIPLibs:sys.h.socket
o.ARP:	TCPIPLibs:sys.h.cdefs
o.ARP:	TCPIPLibs:sys.h.sysctl
o.ARP:	TCPIPLibs:net.h.if
o.ARP:	TCPIPLibs:sys.h.socket
o.ARP:	TCPIPLibs:sys.h.time
o.ARP:	TCPIPLibs:sys.h.cdefs
o.ARP:	TCPIPLibs:net.h.if_arp
o.ARP:	TCPIPLibs:sys.h.socket
o.ARP:	TCPIPLibs:net.h.if_dl
o.ARP:	TCPIPLibs:sys.h.cdefs
o.ARP:	TCPIPLibs:net.h.if_types
o.ARP:	TCPIPLibs:net.h.route
o.ARP:	TCPIPLibs:net.h.radix
o.ARP:	TCPIPLibs:netinet.h.in
o.ARP:	TCPIPLibs:netinet.h.if_ether
o.ARP:	TCPIPLibs:arpa.h.inet
o.ARP:	TCPIPLibs:sys.h.cdefs
o.ARP:	TCPIPLibs:h.netdb
o.ARP:	TCPIPLibs:sys.h.cdefs
o.ARP:	TCPIPLibs:sys.h.errno
o.ARP:	C:h.kernel
o.ARP:	TCPIPLibs:h.unixlib
o.ARP:	TCPIPLibs:h.riscos
o.ARP:	TCPIPLibs:h.nlist
o.ARP:	TCPIPLibs:sys.h.cdefs
