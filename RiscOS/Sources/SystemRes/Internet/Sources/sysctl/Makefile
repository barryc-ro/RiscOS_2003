
TARGET=         SysCtl


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

PathConf:	PathConf.o
		Link -o $@ PathConf.o ${LIBS}
	
# Dynamic dependencies:
o.PathConf:	c.PathConf
o.PathConf:	TCPIPLibs:sys.h.param
o.PathConf:	TCPIPLibs:sys.h.types
o.PathConf:	TCPIPLibs:sys.h.cdefs
o.PathConf:	TCPIPLibs:machine.h.endian
o.PathConf:	TCPIPLibs:machine.h.ansi
o.PathConf:	TCPIPLibs:machine.h.types
o.PathConf:	TCPIPLibs:sys.h.syslimits
o.PathConf:	TCPIPLibs:sys.h.signal
o.PathConf:	TCPIPLibs:machine.h.signal
o.PathConf:	TCPIPLibs:sys.h.cdefs
o.PathConf:	TCPIPLibs:machine.h.param
o.PathConf:	TCPIPLibs:machine.h.limits
o.PathConf:	TCPIPLibs:sys.h.sysctl
o.PathConf:	TCPIPLibs:sys.h.unistd
o.PathConf:	TCPIPLibs:sys.h.errno
o.PathConf:	C:h.kernel
o.SysCtl:	c.SysCtl
o.SysCtl:	TCPIPLibs:sys.h.param
o.SysCtl:	TCPIPLibs:sys.h.types
o.SysCtl:	TCPIPLibs:sys.h.cdefs
o.SysCtl:	TCPIPLibs:machine.h.endian
o.SysCtl:	TCPIPLibs:machine.h.ansi
o.SysCtl:	TCPIPLibs:machine.h.types
o.SysCtl:	TCPIPLibs:sys.h.syslimits
o.SysCtl:	TCPIPLibs:sys.h.signal
o.SysCtl:	TCPIPLibs:machine.h.signal
o.SysCtl:	TCPIPLibs:sys.h.cdefs
o.SysCtl:	TCPIPLibs:machine.h.param
o.SysCtl:	TCPIPLibs:machine.h.limits
o.SysCtl:	TCPIPLibs:sys.h.stat
o.SysCtl:	TCPIPLibs:sys.h.time
o.SysCtl:	TCPIPLibs:sys.h.cdefs
o.SysCtl:	TCPIPLibs:sys.h.cdefs
o.SysCtl:	TCPIPLibs:sys.h.sysctl
o.SysCtl:	TCPIPLibs:sys.h.socket
o.SysCtl:	TCPIPLibs:sys.h.cdefs
o.SysCtl:	TCPIPLibs:netinet.h.in
o.SysCtl:	TCPIPLibs:netinet.h.in_systm
o.SysCtl:	TCPIPLibs:netinet.h.ip
o.SysCtl:	TCPIPLibs:netinet.h.ip_icmp
o.SysCtl:	TCPIPLibs:netinet.h.icmp_var
o.SysCtl:	TCPIPLibs:netinet.h.ip_var
o.SysCtl:	TCPIPLibs:netinet.h.udp
o.SysCtl:	TCPIPLibs:netinet.h.udp_var
o.SysCtl:	TCPIPLibs:netinet.h.tcp
o.SysCtl:	TCPIPLibs:netinet.h.tcp_seq
o.SysCtl:	TCPIPLibs:netinet.h.tcp_timer
o.SysCtl:	TCPIPLibs:netinet.h.tcp_var
o.SysCtl:	TCPIPLibs:netinet.h.igmp_var
o.SysCtl:	TCPIPLibs:sys.h.errno
o.SysCtl:	C:h.kernel
o.SysCtl:	TCPIPLibs:h.riscos
