# Makefile for pppd
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date		Name	Description
# ----		----	-----------
# 1998-01-19	BAL	Created
#

#
# Generic options:
#
MKDIR =		cdir
AS =		objasm
CC =		${MEMCHECKCC} cc
CMHG =		cmhg
CP =		copy
LD =		link
RM =		remove
MODSQZ =	modsqz
WIPE =		-wipe

ASMFLAGS =	-depend !Depend ${THROWBACK}
CFLAGS =	-depend !Depend ${THROWBACK} ${INCLUDES} ${CBLOCKDRIVERS} -strict -Wp -ffah ${CCEXTRA} 
CPFLAGS =	~cfr~v
LDFLAGS =	-aof
WFLAGS =	~c~v

DFLAGS_COMMON = -DCOMPAT_INET4 -DDEBUGMAIN -DDEBUGCHAP -DNO_DRAND48 -DVJC
DFLAGS =	${DFLAGS_COMMON} -DNDEBUG
DDFLAGS =       ${DFLAGS_COMMON} -DDEBUG -DEBUGALL -DDEBUG_TML -DTrace_TRACE

#
# Include files
#
INCLUDES =	-ITCPIPLibs:,C:

#
# Program specific options:
#
TARGET =	o.pppd
DEBUGTARGET =	od.pppd


OBJS =\
 o.auth\
 o.ccp\
 o.chap\
 o.dea_crypt\
 o.dea_data\
 o.fsm\
 o.ipcp\
 o.lcp\
 o.magic\
 o.main\
 o.md5\
 o.minixlib\
 o.options\
 o.sys-riscos\
 o.upap\


OBJSD =\
 od.auth\
 od.ccp\
 od.chap\
 od.dea_crypt\
 od.dea_data\
 od.fsm\
 od.ipcp\
 od.lcp\
 od.magic\
 od.main\
 od.md5\
 od.minixlib\
 od.options\
 od.sys-riscos\
 od.upap\


OBJSCP =\
 cp.auth\
 cp.ccp\
 cp.chap\
 cp.dea_crypt\
 cp.fsm\
 cp.ipcp\
 cp.lcp\
 cp.magic\
 cp.main\
 cp.md5\
 cp.minixlib\
 cp.options\
 cp.sys-riscos\
 cp.upap\


#
# Rule patterns
#
.SUFFIXES:	.o .od .cp .c .s
.c.o:;		${CC} ${CFLAGS} ${DFLAGS}       -c -zM -zps1 -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DDFLAGS}      -c -zM -zps1 -fn -o $@ $<
.c.cp:;		${CC} ${CFLAGS} ${DFLAGS}       -c -C -E $< >> $@
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.s.od:;		$(AS) ${ASMFLAGS} -o $@ $<

#
# Main rules:
#
ram: ${TARGET}

rom: ${TARGET}

debug: ${DEBUGTARGET}

analyse: ${OBJSCP}

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
	${WIPE} cp.* ${WFLAGS}

#
# Target 
#
${TARGET}: ${OBJS}
	${LD} ${LDFLAGS} -o $@ ${OBJS}

#
# Debug target
#
${DEBUGTARGET}: ${OBJSD}
	${LD} ${LDFLAGS} -o $@ ${OBJSD}

# Dynamic dependencies:
o.chap:	c.chap
o.chap:	TCPIPLibs:sys.h.types
o.chap:	TCPIPLibs:sys.h.cdefs
o.chap:	TCPIPLibs:machine.h.endian
o.chap:	TCPIPLibs:machine.h.ansi
o.chap:	TCPIPLibs:machine.h.types
o.chap:	TCPIPLibs:sys.h.syslog
o.chap:	TCPIPLibs:machine.h.ansi
o.chap:	TCPIPLibs:sys.h.cdefs
o.chap:	TCPIPLibs:netinet.h.in_systm
o.chap:	h.ppp
o.chap:	h.pppd
o.chap:	TCPIPLibs:sys.h.param
o.chap:	TCPIPLibs:sys.h.types
o.chap:	TCPIPLibs:sys.h.syslimits
o.chap:	TCPIPLibs:sys.h.signal
o.chap:	TCPIPLibs:machine.h.signal
o.chap:	TCPIPLibs:sys.h.cdefs
o.chap:	TCPIPLibs:machine.h.param
o.chap:	TCPIPLibs:machine.h.limits
o.chap:	^.h.if_ppp
o.chap:	TCPIPLibs:sys.h.dcistructs
o.chap:	TCPIPLibs:sys.h.socket
o.chap:	TCPIPLibs:sys.h.cdefs
o.chap:	TCPIPLibs:net.h.if
o.chap:	TCPIPLibs:sys.h.socket
o.chap:	TCPIPLibs:sys.h.time
o.chap:	TCPIPLibs:sys.h.cdefs
o.chap:	TCPIPLibs:net.h.if_arp
o.chap:	TCPIPLibs:sys.h.socket
o.chap:	^.h.pppmodule
o.chap:	C:h.kernel
o.chap:	TCPIPLibs:netinet.h.in
o.chap:	TCPIPLibs:netinet.h.ip
o.chap:	^.h.slcompress
o.chap:	h.minixlib
o.chap:	h.riscos_ppp
o.chap:	h.auth
o.chap:	h.chap
o.chap:	h.md5
o.chap:	h.main
o.dea_crypt:	c.dea_crypt
o.dea_crypt:	h.dea_crypt
o.dea_data: s.dea_data
o.ipcp:	c.ipcp
o.ipcp:	TCPIPLibs:sys.h.syslog
o.ipcp:	TCPIPLibs:machine.h.ansi
o.ipcp:	TCPIPLibs:sys.h.cdefs
o.ipcp:	TCPIPLibs:h.socklib
o.ipcp:	TCPIPLibs:sys.h.types
o.ipcp:	TCPIPLibs:sys.h.cdefs
o.ipcp:	TCPIPLibs:machine.h.endian
o.ipcp:	TCPIPLibs:machine.h.ansi
o.ipcp:	TCPIPLibs:machine.h.types
o.ipcp:	TCPIPLibs:sys.h.socket
o.ipcp:	TCPIPLibs:sys.h.cdefs
o.ipcp:	TCPIPLibs:sys.h.stat
o.ipcp:	TCPIPLibs:sys.h.time
o.ipcp:	TCPIPLibs:sys.h.cdefs
o.ipcp:	TCPIPLibs:sys.h.cdefs
o.ipcp:	TCPIPLibs:sys.h.time
o.ipcp:	TCPIPLibs:sys.h.uio
o.ipcp:	TCPIPLibs:sys.h.cdefs
o.ipcp:	C:h.kernel
o.ipcp:	TCPIPLibs:h.inetlib
o.ipcp:	TCPIPLibs:sys.h.types
o.ipcp:	TCPIPLibs:netinet.h.in
o.ipcp:	TCPIPLibs:netinet.h.in_systm
o.ipcp:	h.pppd
o.ipcp:	TCPIPLibs:sys.h.param
o.ipcp:	TCPIPLibs:sys.h.types
o.ipcp:	TCPIPLibs:sys.h.syslimits
o.ipcp:	TCPIPLibs:sys.h.signal
o.ipcp:	TCPIPLibs:machine.h.signal
o.ipcp:	TCPIPLibs:sys.h.cdefs
o.ipcp:	TCPIPLibs:machine.h.param
o.ipcp:	TCPIPLibs:machine.h.limits
o.ipcp:	^.h.if_ppp
o.ipcp:	^.h.ppp
o.ipcp:	TCPIPLibs:sys.h.dcistructs
o.ipcp:	TCPIPLibs:sys.h.socket
o.ipcp:	TCPIPLibs:net.h.if
o.ipcp:	TCPIPLibs:sys.h.socket
o.ipcp:	TCPIPLibs:sys.h.time
o.ipcp:	TCPIPLibs:net.h.if_arp
o.ipcp:	TCPIPLibs:sys.h.socket
o.ipcp:	^.h.pppmodule
o.ipcp:	TCPIPLibs:netinet.h.in
o.ipcp:	TCPIPLibs:netinet.h.ip
o.ipcp:	^.h.slcompress
o.ipcp:	h.ppp
o.ipcp:	h.minixlib
o.ipcp:	h.riscos_ppp
o.ipcp:	h.auth
o.ipcp:	h.fsm
o.ipcp:	h.main
o.ipcp:	h.ipcp
o.ipcp:	h.pathnames
o.magic:	c.magic
o.magic:	TCPIPLibs:sys.h.types
o.magic:	TCPIPLibs:sys.h.cdefs
o.magic:	TCPIPLibs:machine.h.endian
o.magic:	TCPIPLibs:machine.h.ansi
o.magic:	TCPIPLibs:machine.h.types
o.magic:	C:h.kernel
o.magic:	C:h.swis
o.magic:	h.magic
o.minixlib:	c.minixlib
o.minixlib:	C:h.kernel
o.minixlib:	C:h.swis
o.minixlib:	TCPIPLibs:h.errno
o.minixlib:	TCPIPLibs:sys.h.errno
o.minixlib:	C:h.kernel
o.minixlib:	TCPIPLibs:sys.h.signal
o.minixlib:	TCPIPLibs:machine.h.signal
o.minixlib:	TCPIPLibs:sys.h.cdefs
o.minixlib:	TCPIPLibs:sys.h.types
o.minixlib:	TCPIPLibs:sys.h.cdefs
o.minixlib:	TCPIPLibs:machine.h.endian
o.minixlib:	TCPIPLibs:machine.h.ansi
o.minixlib:	TCPIPLibs:machine.h.types
o.minixlib:	TCPIPLibs:sys.h.syslog
o.minixlib:	TCPIPLibs:machine.h.ansi
o.minixlib:	TCPIPLibs:sys.h.cdefs
o.minixlib:	h.main
o.minixlib:	h.minixlib
o.minixlib:	h.riscos_ppp
o.minixlib:	^.h.pppmodule
o.minixlib:	^.h.svcprint
o.main:	c.main
o.main:	TCPIPLibs:sys.h.signal
o.main:	TCPIPLibs:machine.h.signal
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:sys.h.types
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:machine.h.endian
o.main:	TCPIPLibs:machine.h.ansi
o.main:	TCPIPLibs:machine.h.types
o.main:	TCPIPLibs:netinet.h.in_systm
o.main:	^.h.ppphdr
o.main:	C:h.kernel
o.main:	TCPIPLibs:h.errno
o.main:	TCPIPLibs:sys.h.errno
o.main:	C:h.kernel
o.main:	TCPIPLibs:h.socklib
o.main:	TCPIPLibs:sys.h.types
o.main:	TCPIPLibs:sys.h.socket
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:sys.h.stat
o.main:	TCPIPLibs:sys.h.time
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:sys.h.time
o.main:	TCPIPLibs:sys.h.uio
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	C:h.kernel
o.main:	TCPIPLibs:h.inetlib
o.main:	TCPIPLibs:sys.h.types
o.main:	TCPIPLibs:netinet.h.in
o.main:	TCPIPLibs:sys.h.syslog
o.main:	TCPIPLibs:machine.h.ansi
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:h.utmp
o.main:	TCPIPLibs:net.h.if
o.main:	TCPIPLibs:sys.h.socket
o.main:	TCPIPLibs:sys.h.time
o.main:	TCPIPLibs:net.h.if_arp
o.main:	TCPIPLibs:sys.h.socket
o.main:	^.h.blockdrive
o.main:	^.h.if_ppp
o.main:	^.h.ppp
o.main:	TCPIPLibs:sys.h.dcistructs
o.main:	TCPIPLibs:sys.h.socket
o.main:	TCPIPLibs:net.h.if
o.main:	^.h.pppmodule
o.main:	TCPIPLibs:netinet.h.in
o.main:	TCPIPLibs:netinet.h.ip
o.main:	^.h.slcompress
o.main:	h.callout
o.main:	h.ppp
o.main:	h.magic
o.main:	h.fsm
o.main:	h.auth
o.main:	h.lcp
o.main:	h.ipcp
o.main:	h.upap
o.main:	h.chap
o.main:	h.ccp
o.main:	h.options
o.main:	h.minixlib
o.main:	h.riscos_ppp
o.main:	h.pppd
o.main:	TCPIPLibs:sys.h.param
o.main:	TCPIPLibs:sys.h.types
o.main:	TCPIPLibs:sys.h.syslimits
o.main:	TCPIPLibs:sys.h.signal
o.main:	TCPIPLibs:machine.h.param
o.main:	TCPIPLibs:machine.h.limits
o.main:	h.pathnames
o.main:	h.patchlevel
o.main:	C:h.swis
o.main:	^.h.pppd
o.main:	h.main
o.main:	C:h.dialler
o.fsm:	c.fsm
o.fsm:	TCPIPLibs:sys.h.types
o.fsm:	TCPIPLibs:sys.h.cdefs
o.fsm:	TCPIPLibs:machine.h.endian
o.fsm:	TCPIPLibs:machine.h.ansi
o.fsm:	TCPIPLibs:machine.h.types
o.fsm:	TCPIPLibs:sys.h.syslog
o.fsm:	TCPIPLibs:machine.h.ansi
o.fsm:	TCPIPLibs:sys.h.cdefs
o.fsm:	TCPIPLibs:netinet.h.in_systm
o.fsm:	h.ppp
o.fsm:	h.pppd
o.fsm:	TCPIPLibs:sys.h.param
o.fsm:	TCPIPLibs:sys.h.types
o.fsm:	TCPIPLibs:sys.h.syslimits
o.fsm:	TCPIPLibs:sys.h.signal
o.fsm:	TCPIPLibs:machine.h.signal
o.fsm:	TCPIPLibs:sys.h.cdefs
o.fsm:	TCPIPLibs:machine.h.param
o.fsm:	TCPIPLibs:machine.h.limits
o.fsm:	^.h.if_ppp
o.fsm:	TCPIPLibs:sys.h.dcistructs
o.fsm:	TCPIPLibs:sys.h.socket
o.fsm:	TCPIPLibs:sys.h.cdefs
o.fsm:	TCPIPLibs:net.h.if
o.fsm:	TCPIPLibs:sys.h.socket
o.fsm:	TCPIPLibs:sys.h.time
o.fsm:	TCPIPLibs:sys.h.cdefs
o.fsm:	TCPIPLibs:net.h.if_arp
o.fsm:	TCPIPLibs:sys.h.socket
o.fsm:	^.h.pppmodule
o.fsm:	C:h.kernel
o.fsm:	TCPIPLibs:netinet.h.in
o.fsm:	TCPIPLibs:netinet.h.ip
o.fsm:	^.h.slcompress
o.fsm:	h.minixlib
o.fsm:	h.riscos_ppp
o.fsm:	h.fsm
o.auth:	c.auth
o.auth:	TCPIPLibs:sys.h.syslog
o.auth:	TCPIPLibs:machine.h.ansi
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	TCPIPLibs:h.pwd
o.auth:	TCPIPLibs:sys.h.types
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	TCPIPLibs:machine.h.endian
o.auth:	TCPIPLibs:machine.h.ansi
o.auth:	TCPIPLibs:machine.h.types
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	TCPIPLibs:sys.h.types
o.auth:	TCPIPLibs:sys.h.stat
o.auth:	TCPIPLibs:sys.h.time
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	TCPIPLibs:h.inetlib
o.auth:	TCPIPLibs:sys.h.types
o.auth:	TCPIPLibs:netinet.h.in
o.auth:	TCPIPLibs:netinet.h.in_systm
o.auth:	TCPIPLibs:h.netdb
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	TCPIPLibs:netinet.h.in
o.auth:	TCPIPLibs:arpa.h.inet
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	h.ppp
o.auth:	h.pppd
o.auth:	TCPIPLibs:sys.h.param
o.auth:	TCPIPLibs:sys.h.types
o.auth:	TCPIPLibs:sys.h.syslimits
o.auth:	TCPIPLibs:sys.h.signal
o.auth:	TCPIPLibs:machine.h.signal
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	TCPIPLibs:machine.h.param
o.auth:	TCPIPLibs:machine.h.limits
o.auth:	^.h.if_ppp
o.auth:	TCPIPLibs:sys.h.dcistructs
o.auth:	TCPIPLibs:sys.h.socket
o.auth:	TCPIPLibs:sys.h.cdefs
o.auth:	TCPIPLibs:net.h.if
o.auth:	TCPIPLibs:sys.h.socket
o.auth:	TCPIPLibs:sys.h.time
o.auth:	TCPIPLibs:net.h.if_arp
o.auth:	TCPIPLibs:sys.h.socket
o.auth:	^.h.pppmodule
o.auth:	C:h.kernel
o.auth:	TCPIPLibs:netinet.h.in
o.auth:	TCPIPLibs:netinet.h.ip
o.auth:	^.h.slcompress
o.auth:	h.fsm
o.auth:	h.lcp
o.auth:	h.upap
o.auth:	h.chap
o.auth:	h.ipcp
o.auth:	h.ccp
o.auth:	h.pathnames
o.auth:	h.options
o.auth:	h.auth
o.auth:	h.main
o.auth:	h.minixlib
o.auth:	h.riscos_ppp
o.auth:	h.dea_crypt
o.ccp:	c.ccp
o.ccp:	TCPIPLibs:sys.h.syslog
o.ccp:	TCPIPLibs:machine.h.ansi
o.ccp:	TCPIPLibs:sys.h.cdefs
o.ccp:	TCPIPLibs:h.socklib
o.ccp:	TCPIPLibs:sys.h.types
o.ccp:	TCPIPLibs:sys.h.cdefs
o.ccp:	TCPIPLibs:machine.h.endian
o.ccp:	TCPIPLibs:machine.h.ansi
o.ccp:	TCPIPLibs:machine.h.types
o.ccp:	TCPIPLibs:sys.h.socket
o.ccp:	TCPIPLibs:sys.h.cdefs
o.ccp:	TCPIPLibs:sys.h.stat
o.ccp:	TCPIPLibs:sys.h.time
o.ccp:	TCPIPLibs:sys.h.cdefs
o.ccp:	TCPIPLibs:sys.h.cdefs
o.ccp:	TCPIPLibs:sys.h.time
o.ccp:	TCPIPLibs:sys.h.uio
o.ccp:	TCPIPLibs:sys.h.cdefs
o.ccp:	C:h.kernel
o.ccp:	TCPIPLibs:h.inetlib
o.ccp:	TCPIPLibs:sys.h.types
o.ccp:	TCPIPLibs:netinet.h.in
o.ccp:	TCPIPLibs:netinet.h.in_systm
o.ccp:	h.pppd
o.ccp:	TCPIPLibs:sys.h.param
o.ccp:	TCPIPLibs:sys.h.types
o.ccp:	TCPIPLibs:sys.h.syslimits
o.ccp:	TCPIPLibs:sys.h.signal
o.ccp:	TCPIPLibs:machine.h.signal
o.ccp:	TCPIPLibs:sys.h.cdefs
o.ccp:	TCPIPLibs:machine.h.param
o.ccp:	TCPIPLibs:machine.h.limits
o.ccp:	^.h.if_ppp
o.ccp:	^.h.ppp
o.ccp:	TCPIPLibs:sys.h.dcistructs
o.ccp:	TCPIPLibs:sys.h.socket
o.ccp:	TCPIPLibs:net.h.if
o.ccp:	TCPIPLibs:sys.h.socket
o.ccp:	TCPIPLibs:sys.h.time
o.ccp:	TCPIPLibs:net.h.if_arp
o.ccp:	TCPIPLibs:sys.h.socket
o.ccp:	^.h.pppmodule
o.ccp:	TCPIPLibs:netinet.h.in
o.ccp:	TCPIPLibs:netinet.h.ip
o.ccp:	^.h.slcompress
o.ccp:	h.ppp
o.ccp:	h.minixlib
o.ccp:	h.riscos_ppp
o.ccp:	h.auth
o.ccp:	h.fsm
o.ccp:	h.main
o.ccp:	h.ccp
o.ccp:	^.h.ppp-comp
o.lcp:	c.lcp
o.lcp:	TCPIPLibs:sys.h.syslog
o.lcp:	TCPIPLibs:machine.h.ansi
o.lcp:	TCPIPLibs:sys.h.cdefs
o.lcp:	TCPIPLibs:sys.h.ioctl
o.lcp:	TCPIPLibs:sys.h.ttycom
o.lcp:	TCPIPLibs:sys.h.ioccom
o.lcp:	TCPIPLibs:sys.h.ioccom
o.lcp:	TCPIPLibs:sys.h.filio
o.lcp:	TCPIPLibs:sys.h.ioccom
o.lcp:	TCPIPLibs:sys.h.sockio
o.lcp:	TCPIPLibs:sys.h.ioccom
o.lcp:	TCPIPLibs:sys.h.cdefs
o.lcp:	TCPIPLibs:sys.h.types
o.lcp:	TCPIPLibs:sys.h.cdefs
o.lcp:	TCPIPLibs:machine.h.endian
o.lcp:	TCPIPLibs:machine.h.ansi
o.lcp:	TCPIPLibs:machine.h.types
o.lcp:	TCPIPLibs:sys.h.socket
o.lcp:	TCPIPLibs:sys.h.cdefs
o.lcp:	TCPIPLibs:sys.h.time
o.lcp:	TCPIPLibs:sys.h.cdefs
o.lcp:	TCPIPLibs:netinet.h.in
o.lcp:	TCPIPLibs:netinet.h.in_systm
o.lcp:	h.pppd
o.lcp:	TCPIPLibs:sys.h.param
o.lcp:	TCPIPLibs:sys.h.types
o.lcp:	TCPIPLibs:sys.h.syslimits
o.lcp:	TCPIPLibs:sys.h.signal
o.lcp:	TCPIPLibs:machine.h.signal
o.lcp:	TCPIPLibs:sys.h.cdefs
o.lcp:	TCPIPLibs:machine.h.param
o.lcp:	TCPIPLibs:machine.h.limits
o.lcp:	^.h.if_ppp
o.lcp:	^.h.ppp
o.lcp:	TCPIPLibs:sys.h.dcistructs
o.lcp:	TCPIPLibs:sys.h.socket
o.lcp:	TCPIPLibs:net.h.if
o.lcp:	TCPIPLibs:sys.h.socket
o.lcp:	TCPIPLibs:sys.h.time
o.lcp:	TCPIPLibs:net.h.if_arp
o.lcp:	TCPIPLibs:sys.h.socket
o.lcp:	^.h.pppmodule
o.lcp:	C:h.kernel
o.lcp:	TCPIPLibs:netinet.h.in
o.lcp:	TCPIPLibs:netinet.h.ip
o.lcp:	^.h.slcompress
o.lcp:	h.ppp
o.lcp:	h.minixlib
o.lcp:	h.riscos_ppp
o.lcp:	h.fsm
o.lcp:	h.lcp
o.lcp:	h.magic
o.lcp:	h.main
o.lcp:	h.chap
o.lcp:	h.auth
o.lcp:	h.upap
o.lcp:	h.ipcp
o.lcp:	h.ccp
