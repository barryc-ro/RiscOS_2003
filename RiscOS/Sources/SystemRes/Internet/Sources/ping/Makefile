
TARGET=         Ping


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
o.Ping:	c.Ping
o.Ping:	TCPIPLibs:sys.h.param
o.Ping:	TCPIPLibs:sys.h.types
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:machine.h.endian
o.Ping:	TCPIPLibs:machine.h.ansi
o.Ping:	TCPIPLibs:machine.h.types
o.Ping:	TCPIPLibs:sys.h.syslimits
o.Ping:	TCPIPLibs:sys.h.signal
o.Ping:	TCPIPLibs:machine.h.signal
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:machine.h.param
o.Ping:	TCPIPLibs:machine.h.limits
o.Ping:	TCPIPLibs:sys.h.socket
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:sys.h.file
o.Ping:	TCPIPLibs:sys.h.fcntl
o.Ping:	TCPIPLibs:sys.h.types
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:sys.h.unistd
o.Ping:	TCPIPLibs:sys.h.time
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:sys.h.signal
o.Ping:	TCPIPLibs:sys.h.ioctl
o.Ping:	TCPIPLibs:sys.h.ttycom
o.Ping:	TCPIPLibs:sys.h.ioccom
o.Ping:	TCPIPLibs:sys.h.ioccom
o.Ping:	TCPIPLibs:sys.h.filio
o.Ping:	TCPIPLibs:sys.h.ioccom
o.Ping:	TCPIPLibs:sys.h.sockio
o.Ping:	TCPIPLibs:sys.h.ioccom
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:netinet.h.in_systm
o.Ping:	TCPIPLibs:netinet.h.in
o.Ping:	TCPIPLibs:netinet.h.ip
o.Ping:	TCPIPLibs:netinet.h.ip_icmp
o.Ping:	TCPIPLibs:netinet.h.ip_var
o.Ping:	TCPIPLibs:arpa.h.inet
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:h.netdb
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:h.unistd
o.Ping:	TCPIPLibs:sys.h.cdefs
o.Ping:	TCPIPLibs:sys.h.types
o.Ping:	TCPIPLibs:sys.h.unistd
o.Ping:	TCPIPLibs:sys.h.errno
o.Ping:	C:h.kernel
o.Ping:	TCPIPLibs:h.riscos
o.Ping:	TCPIPLibs:h.unixlib
