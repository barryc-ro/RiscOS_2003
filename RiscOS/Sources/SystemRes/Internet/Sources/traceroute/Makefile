
TARGET=         TraceRoute


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
o.TraceRoute:	c.TraceRoute
o.TraceRoute:	TCPIPLibs:sys.h.param
o.TraceRoute:	TCPIPLibs:sys.h.types
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:machine.h.endian
o.TraceRoute:	TCPIPLibs:machine.h.ansi
o.TraceRoute:	TCPIPLibs:machine.h.types
o.TraceRoute:	TCPIPLibs:sys.h.syslimits
o.TraceRoute:	TCPIPLibs:sys.h.signal
o.TraceRoute:	TCPIPLibs:machine.h.signal
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:machine.h.param
o.TraceRoute:	TCPIPLibs:machine.h.limits
o.TraceRoute:	TCPIPLibs:sys.h.time
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:sys.h.socket
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:sys.h.file
o.TraceRoute:	TCPIPLibs:sys.h.fcntl
o.TraceRoute:	TCPIPLibs:sys.h.types
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:sys.h.unistd
o.TraceRoute:	TCPIPLibs:sys.h.ioctl
o.TraceRoute:	TCPIPLibs:sys.h.ttycom
o.TraceRoute:	TCPIPLibs:sys.h.ioccom
o.TraceRoute:	TCPIPLibs:sys.h.ioccom
o.TraceRoute:	TCPIPLibs:sys.h.filio
o.TraceRoute:	TCPIPLibs:sys.h.ioccom
o.TraceRoute:	TCPIPLibs:sys.h.sockio
o.TraceRoute:	TCPIPLibs:sys.h.ioccom
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:netinet.h.in_systm
o.TraceRoute:	TCPIPLibs:netinet.h.in
o.TraceRoute:	TCPIPLibs:netinet.h.ip
o.TraceRoute:	TCPIPLibs:netinet.h.ip_icmp
o.TraceRoute:	TCPIPLibs:netinet.h.udp
o.TraceRoute:	TCPIPLibs:arpa.h.inet
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:h.netdb
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:sys.h.errno
o.TraceRoute:	C:h.kernel
o.TraceRoute:	TCPIPLibs:h.unistd
o.TraceRoute:	TCPIPLibs:sys.h.cdefs
o.TraceRoute:	TCPIPLibs:sys.h.types
o.TraceRoute:	TCPIPLibs:sys.h.unistd
o.TraceRoute:	TCPIPLibs:h.unixlib
o.TraceRoute:	TCPIPLibs:h.riscos
