
TARGET=         MTrace


INCLUDES=	C:,TCPIPLibs:
CC=		cc
RM=             remove
WFLAGS=         fr~c~v
WIPE=           -wipe

CFLAGS=		-Wp -I$(INCLUDES) -c ${THROWBACK} -depend !Depend

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
o.MTrace:	c.MTrace
o.MTrace:	TCPIPLibs:h.err
o.MTrace:	TCPIPLibs:machine.h.ansi
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:h.netdb
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:h.unistd
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:sys.h.types
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:machine.h.endian
o.MTrace:	TCPIPLibs:machine.h.ansi
o.MTrace:	TCPIPLibs:machine.h.types
o.MTrace:	TCPIPLibs:sys.h.unistd
o.MTrace:	TCPIPLibs:sys.h._posix
o.MTrace:	TCPIPLibs:sys.h.param
o.MTrace:	TCPIPLibs:sys.h.types
o.MTrace:	TCPIPLibs:sys.h.syslimits
o.MTrace:	TCPIPLibs:sys.h.signal
o.MTrace:	TCPIPLibs:machine.h.signal
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:machine.h.param
o.MTrace:	TCPIPLibs:machine.h.limits
o.MTrace:	TCPIPLibs:sys.h.types
o.MTrace:	TCPIPLibs:sys.h.socket
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:sys.h.time
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:net.h.if
o.MTrace:	TCPIPLibs:sys.h.time
o.MTrace:	TCPIPLibs:netinet.h.in
o.MTrace:	TCPIPLibs:netinet.h.in_systm
o.MTrace:	TCPIPLibs:netinet.h.ip
o.MTrace:	TCPIPLibs:netinet.h.igmp
o.MTrace:	TCPIPLibs:sys.h.ioctl
o.MTrace:	TCPIPLibs:sys.h.ttycom
o.MTrace:	TCPIPLibs:sys.h.ioccom
o.MTrace:	TCPIPLibs:sys.h.ioccom
o.MTrace:	TCPIPLibs:sys.h.filio
o.MTrace:	TCPIPLibs:sys.h.ioccom
o.MTrace:	TCPIPLibs:sys.h.sockio
o.MTrace:	TCPIPLibs:sys.h.ioccom
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:arpa.h.inet
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:sys.h.errno
o.MTrace:	C:h.kernel
o.MTrace:	TCPIPLibs:sys.h.syslog
o.MTrace:	TCPIPLibs:machine.h.ansi
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:h.socklib
o.MTrace:	TCPIPLibs:sys.h.types
o.MTrace:	TCPIPLibs:sys.h.socket
o.MTrace:	TCPIPLibs:sys.h.stat
o.MTrace:	TCPIPLibs:sys.h.time
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	TCPIPLibs:sys.h.time
o.MTrace:	TCPIPLibs:sys.h.uio
o.MTrace:	TCPIPLibs:sys.h.cdefs
o.MTrace:	C:h.kernel
o.MTrace:	TCPIPLibs:h.unixlib
o.MTrace:	C:h.swis
o.MTrace:	h.mtrace
