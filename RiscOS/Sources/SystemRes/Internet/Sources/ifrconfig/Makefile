
TARGET=         IfRConfig


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
o.IfRConfig:	c.IfRConfig
o.IfRConfig:	TCPIPLibs:sys.h.types
o.IfRConfig:	TCPIPLibs:sys.h.cdefs
o.IfRConfig:	TCPIPLibs:machine.h.endian
o.IfRConfig:	TCPIPLibs:machine.h.ansi
o.IfRConfig:	TCPIPLibs:machine.h.types
o.IfRConfig:	TCPIPLibs:sys.h.socket
o.IfRConfig:	TCPIPLibs:sys.h.cdefs
o.IfRConfig:	TCPIPLibs:sys.h.ioctl
o.IfRConfig:	TCPIPLibs:sys.h.ttycom
o.IfRConfig:	TCPIPLibs:sys.h.ioccom
o.IfRConfig:	TCPIPLibs:sys.h.ioccom
o.IfRConfig:	TCPIPLibs:sys.h.filio
o.IfRConfig:	TCPIPLibs:sys.h.ioccom
o.IfRConfig:	TCPIPLibs:sys.h.sockio
o.IfRConfig:	TCPIPLibs:sys.h.ioccom
o.IfRConfig:	TCPIPLibs:sys.h.cdefs
o.IfRConfig:	TCPIPLibs:net.h.if
o.IfRConfig:	TCPIPLibs:sys.h.socket
o.IfRConfig:	TCPIPLibs:sys.h.time
o.IfRConfig:	TCPIPLibs:sys.h.cdefs
o.IfRConfig:	TCPIPLibs:net.h.if_arp
o.IfRConfig:	TCPIPLibs:sys.h.socket
o.IfRConfig:	TCPIPLibs:netinet.h.in
o.IfRConfig:	TCPIPLibs:sys.h.errno
o.IfRConfig:	C:h.kernel
o.IfRConfig:	TCPIPLibs:h.netdb
o.IfRConfig:	TCPIPLibs:sys.h.cdefs
o.IfRConfig:	TCPIPLibs:h.riscos
o.IfRConfig:	TCPIPLibs:h.inetlib
o.IfRConfig:	TCPIPLibs:sys.h.types
o.IfRConfig:	TCPIPLibs:netinet.h.in
o.IfRConfig:	TCPIPLibs:h.socklib
o.IfRConfig:	TCPIPLibs:sys.h.types
o.IfRConfig:	TCPIPLibs:sys.h.socket
o.IfRConfig:	TCPIPLibs:sys.h.stat
o.IfRConfig:	TCPIPLibs:sys.h.time
o.IfRConfig:	TCPIPLibs:sys.h.cdefs
o.IfRConfig:	TCPIPLibs:sys.h.time
o.IfRConfig:	TCPIPLibs:sys.h.uio
o.IfRConfig:	TCPIPLibs:sys.h.cdefs
o.IfRConfig:	C:h.kernel
o.IfRConfig:	TCPIPLibs:h.unixlib
o.IfRConfig:	C:h.kernel
o.IfRConfig:	C:h.swis
