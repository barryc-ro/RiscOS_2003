# Makefile for PPP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date		Name	Description
# ----		----	-----------
# 1998-01-19	BAL	Created
#

#
# Paths
#
EXP_HDR	=	<export$dir>

#
# Generic options:
#
MKDIR =		cdir
AS =		objasm
CC =		cc
CMHG =		cmhg
CP =		copy
LD =		<boot$dir>.library.link
RM =		remove
MODSQZ =	modsqz
WIPE =		-wipe

ASMFLAGS =	-depend !Depend ${THROWBACK} -predefine "BlockDrivers SETL ${ASMBLOCKDRIVERS}"
CFLAGS =	-depend !Depend ${THROWBACK} ${INCLUDES} ${CBLOCKDRIVERS} -strict -wP -ffah 
CMHGFLAGS =	-depend !Depend ${THROWBACK} ${INCLUDES} ${CBLOCKDRIVERS} -p
CPFLAGS =	~cfr~v
WFLAGS =	~c~v

DFLAGS_COMMON = -DINET -DCOMPAT_INET4 -DVJC
DFLAGS =	${DFLAGS_COMMON} -DNDEBUG
DDFLAGS =       ${DFLAGS_COMMON} -DDEBUG -DDEBUG_TML -DTrace_TRACE

#
# Libraries
#
CLIB =		CLIB:o.stubs
RLIB =		RISCOSLIB:o.risc_oslib
RSTUBS =	RISCOSLIB:o.rstubs
ROMSTUBS =	RISCOSLIB:o.romstubs
ROMCSTUBS =	RISCOSLIB:o.romcstubs
ABSSYM  =	RISC_OSLib:o.c_AbsSym

INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm

LIBS =\
 ${INETLIB}\
 ${SOCKLIB}\
 ${UNIXLIB}
		

LIBSD =\
 ${INETLIB}\
 ${SOCKLIB}\
 ${UNIXLIB}\
 <Lib$Dir>.svcprint.o.svcprint\
 <Lib$Dir>.Trace.o.T_M_PL


#
# Include files
#
INCLUDES =	-ITCPIPLibs:,C:

#
# Program specific options:
#
COMPONENT =	PPP
RAMTARGET =	rm.PPP
ROMTARGET =	aof.PPP
DEBUGTARGET =	rm.PPPD
EXPORTS =	${EXP_DIR}.PPP


#
# Export Paths for Messages module
#
#RESDIR = 	<resource$dir>.Resources2.PPP


OBJS =\
 o.blockdrive\
 o.if_ppp\
 o.mbuf\
 o.ppp_io\
 o.ppphdr\
 o.pppmodule\
 o.serial\
 o.slcompress\


OBJSD =\
 od.blockdrive\
 od.if_ppp\
 od.mbuf\
 od.ppp_io\
 od.ppphdr\
 od.pppmodule\
 od.serial\
 od.slcompress\


OBJSCP =\
 cp.if_ppp\
 cp.mbuf\
 o.ppphdr\
 cp.pppmodule\
 cp.serial\
 cp.slcompress\


HDRS =	h.ppphdr

#
# Rule patterns
#
.SUFFIXES:	.o .od .cp .h .s .c .cmhg
.c.o:;		${CC} ${CFLAGS} ${DFLAGS}       -c -zM -zps1 -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DDFLAGS}      -c -zM -zps1 -fn -o $@ $<
.c.cp:;		${CC} ${CFLAGS} ${DFLAGS}       -c -C -E $< >> $@
.s.o:;		${AS} ${ASMFLAGS}               -o $@ $<
.s.od:;		${AS} ${ASMFLAGS}               -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} ${DFLAGS}  -o $@ $<
.cmhg.od:;	${CMHG} ${CMHGFLAGS} ${DDFLAGS} -o $@ $<
.cmhg.h:;	${CMHG} ${CMHGFLAGS} ${DFLAGS}  -d $@ $<

#
# Main rules:
#
ram: ${RAMTARGET}
        @echo ${COMPONENT}: all complete

rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

debug: ${DEBUGTARGET}
        @echo ${COMPONENT}: debug build complete

analyse: pppd ${OBJSCP} ${HDRS}
	@echo ${COMPONENT}: analysis build complete

export:
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        @echo ${COMPONENT}: resource files copied

clean:	pppd
	${RM} ${HDRS}
	${WIPE} o.* ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
	${WIPE} cp.* ${WFLAGS}
	${WIPE} rm.* ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${WIPE} map ${WFLAGS}
	${RM} ${RAMTARGET}
	${RM} ${ROMTARGET}
	${RM} ${DEBUGTARGET}
	@echo ${COMPONENT}: cleaned

pppd:
        Dir pppd
        amu ${MFLAGS}
        Up

#
# Target 
#
${RAMTARGET}: pppd ${OBJS} ${HDRS} ${CLIB}
	${LD} -rmf -o $@ pppd.o.pppd ${OBJS} ${LIBS} ${CLIB}
	${MODSQZ} $@

#
# ROM Target (re-linked at ROM Image build time)
#
${ROMTARGET}: pppd ${OBJS} ${HDRS} ${ROMCSTUBS}
	${LD} -o $@ -aof pppd.o.pppd ${OBJS} ${LIBS} ${ROMCSTUBS}

#
# Debug target
#
${DEBUGTARGET}: pppd ${OBJSD} ${HDRS} ${CLIB}
	${LD} -rmf -o $@ pppd.od.pppd ${OBJSD} ${LIBSD} ${CLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


# Dynamic dependencies:
od.blockdrive: s.blockdrive
od.if_ppp:	c.if_ppp
od.if_ppp:	TCPIPLibs:sys.h.dcistructs
od.if_ppp:	h.ppp
od.if_ppp:	TCPIPLibs:sys.h.errno
od.if_ppp:	C:h.kernel
od.if_ppp:	TCPIPLibs:sys.h.types
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	TCPIPLibs:machine.h.endian
od.if_ppp:	TCPIPLibs:machine.h.ansi
od.if_ppp:	TCPIPLibs:machine.h.types
od.if_ppp:	TCPIPLibs:sys.h.mbuf
od.if_ppp:	TCPIPLibs:sys.h.malloc
od.if_ppp:	TCPIPLibs:sys.h.socket
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	TCPIPLibs:sys.h.ioctl
od.if_ppp:	TCPIPLibs:sys.h.ttycom
od.if_ppp:	TCPIPLibs:sys.h.ioccom
od.if_ppp:	TCPIPLibs:sys.h.ioccom
od.if_ppp:	TCPIPLibs:sys.h.filio
od.if_ppp:	TCPIPLibs:sys.h.ioccom
od.if_ppp:	TCPIPLibs:sys.h.sockio
od.if_ppp:	TCPIPLibs:sys.h.ioccom
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	TCPIPLibs:sys.h.signal
od.if_ppp:	TCPIPLibs:machine.h.signal
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	TCPIPLibs:sys.h.tty
od.if_ppp:	TCPIPLibs:sys.h.termios
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	TCPIPLibs:sys.h.ttycom
od.if_ppp:	TCPIPLibs:sys.h.ttydefaults
od.if_ppp:	TCPIPLibs:sys.h.select
od.if_ppp:	TCPIPLibs:sys.h.conf
od.if_ppp:	TCPIPLibs:sys.h.syslog
od.if_ppp:	TCPIPLibs:machine.h.ansi
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	TCPIPLibs:net.h.if
od.if_ppp:	TCPIPLibs:sys.h.socket
od.if_ppp:	TCPIPLibs:sys.h.time
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	TCPIPLibs:net.h.if_arp
od.if_ppp:	TCPIPLibs:sys.h.socket
od.if_ppp:	TCPIPLibs:net.h.netisr
od.if_ppp:	TCPIPLibs:net.h.route
od.if_ppp:	TCPIPLibs:net.h.radix
od.if_ppp:	TCPIPLibs:netinet.h.in
od.if_ppp:	TCPIPLibs:netinet.h.if_ether
od.if_ppp:	TCPIPLibs:netinet.h.in_systm
od.if_ppp:	TCPIPLibs:netinet.h.in_var
od.if_ppp:	TCPIPLibs:sys.h.queue
od.if_ppp:	TCPIPLibs:netinet.h.ip
od.if_ppp:	TCPIPLibs:netinet.h.tcp
od.if_ppp:	TCPIPLibs:h.socklib
od.if_ppp:	TCPIPLibs:sys.h.types
od.if_ppp:	TCPIPLibs:sys.h.socket
od.if_ppp:	TCPIPLibs:sys.h.stat
od.if_ppp:	TCPIPLibs:sys.h.time
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	TCPIPLibs:sys.h.time
od.if_ppp:	TCPIPLibs:sys.h.uio
od.if_ppp:	TCPIPLibs:sys.h.cdefs
od.if_ppp:	C:h.kernel
od.if_ppp:	h.mbuf
od.if_ppp:	C:h.swis
od.if_ppp:	h.pppd
od.if_ppp:	h.ppp_io
od.if_ppp:	h.ppphdr
od.if_ppp:	h.pppmodule
od.if_ppp:	h.svcprint
od.if_ppp:	h.ioctl
od.if_ppp:	C:h.kernel
od.if_ppp:	h.if_ppp
od.if_ppp:	TCPIPLibs:sys.h.socket
od.if_ppp:	TCPIPLibs:net.h.if
od.if_ppp:	TCPIPLibs:netinet.h.in
od.if_ppp:	TCPIPLibs:netinet.h.ip
od.if_ppp:	h.slcompress
od.if_ppp:	h.serial
od.if_ppp:	C:h.dialler
od.if_ppp:	pppd.h.minixlib
od.if_ppp:	pppd.h.riscos_ppp
od.if_ppp:	pppd.h.main
od.mbuf:	c.mbuf
od.mbuf:	C:h.kernel
od.mbuf:	C:h.swis
od.mbuf:	TCPIPLibs:sys.h.param
od.mbuf:	TCPIPLibs:sys.h.types
od.mbuf:	TCPIPLibs:sys.h.cdefs
od.mbuf:	TCPIPLibs:machine.h.endian
od.mbuf:	TCPIPLibs:machine.h.ansi
od.mbuf:	TCPIPLibs:machine.h.types
od.mbuf:	TCPIPLibs:sys.h.syslimits
od.mbuf:	TCPIPLibs:sys.h.signal
od.mbuf:	TCPIPLibs:machine.h.signal
od.mbuf:	TCPIPLibs:sys.h.cdefs
od.mbuf:	TCPIPLibs:machine.h.param
od.mbuf:	TCPIPLibs:machine.h.limits
od.mbuf:	TCPIPLibs:sys.h.errno
od.mbuf:	C:h.kernel
od.mbuf:	TCPIPLibs:sys.h.mbuf
od.mbuf:	TCPIPLibs:sys.h.malloc
od.mbuf:	TCPIPLibs:net.h.if
od.mbuf:	TCPIPLibs:sys.h.socket
od.mbuf:	TCPIPLibs:sys.h.cdefs
od.mbuf:	TCPIPLibs:sys.h.time
od.mbuf:	TCPIPLibs:sys.h.cdefs
od.mbuf:	TCPIPLibs:net.h.if_arp
od.mbuf:	TCPIPLibs:sys.h.socket
od.mbuf:	TCPIPLibs:netinet.h.in
od.mbuf:	TCPIPLibs:netinet.h.if_ether
od.mbuf:	h.mbuf
od.ppp_io: s.ppp_io
od.ppp_io: Hdr.CVars
od.serial:	c.serial
od.serial:	TCPIPLibs:h.errno
od.serial:	TCPIPLibs:sys.h.errno
od.serial:	C:h.kernel
od.serial:	TCPIPLibs:sys.h.types
od.serial:	TCPIPLibs:sys.h.cdefs
od.serial:	TCPIPLibs:machine.h.endian
od.serial:	TCPIPLibs:machine.h.ansi
od.serial:	TCPIPLibs:machine.h.types
od.serial:	TCPIPLibs:sys.h.dcistructs
od.serial:	TCPIPLibs:net.h.if
od.serial:	TCPIPLibs:sys.h.socket
od.serial:	TCPIPLibs:sys.h.cdefs
od.serial:	TCPIPLibs:sys.h.time
od.serial:	TCPIPLibs:sys.h.cdefs
od.serial:	TCPIPLibs:net.h.if_arp
od.serial:	TCPIPLibs:sys.h.socket
od.serial:	TCPIPLibs:netinet.h.in_systm
od.serial:	TCPIPLibs:netinet.h.in
od.serial:	TCPIPLibs:netinet.h.if_ether
od.serial:	TCPIPLibs:sys.h.signal
od.serial:	TCPIPLibs:machine.h.signal
od.serial:	TCPIPLibs:sys.h.cdefs
od.serial:	TCPIPLibs:sys.h.syslog
od.serial:	TCPIPLibs:machine.h.ansi
od.serial:	TCPIPLibs:sys.h.cdefs
od.serial:	C:h.kernel
od.serial:	C:h.swis
od.serial:	h.ppp_io
od.serial:	h.pppmodule
od.serial:	h.svcprint
od.serial:	h.ioctl
od.serial:	C:h.kernel
od.serial:	h.if_ppp
od.serial:	h.ppp
od.serial:	TCPIPLibs:sys.h.socket
od.serial:	TCPIPLibs:net.h.if
od.serial:	TCPIPLibs:netinet.h.in
od.serial:	TCPIPLibs:netinet.h.ip
od.serial:	h.slcompress
od.serial:	h.ppphdr
od.serial:	pppd.h.minixlib
od.serial:	pppd.h.riscos_ppp
od.serial:	pppd.h.main
od.serial:	h.serial
od.serial:	C:h.dialler
od.slcompress:	c.slcompress
od.slcompress:	TCPIPLibs:sys.h.types
od.slcompress:	TCPIPLibs:sys.h.cdefs
od.slcompress:	TCPIPLibs:machine.h.endian
od.slcompress:	TCPIPLibs:machine.h.ansi
od.slcompress:	TCPIPLibs:machine.h.types
od.slcompress:	TCPIPLibs:sys.h.param
od.slcompress:	TCPIPLibs:sys.h.types
od.slcompress:	TCPIPLibs:sys.h.syslimits
od.slcompress:	TCPIPLibs:sys.h.signal
od.slcompress:	TCPIPLibs:machine.h.signal
od.slcompress:	TCPIPLibs:sys.h.cdefs
od.slcompress:	TCPIPLibs:machine.h.param
od.slcompress:	TCPIPLibs:machine.h.limits
od.slcompress:	TCPIPLibs:sys.h.mbuf
od.slcompress:	TCPIPLibs:sys.h.malloc
od.slcompress:	TCPIPLibs:sys.h.socketvar
od.slcompress:	TCPIPLibs:sys.h.stat
od.slcompress:	TCPIPLibs:sys.h.time
od.slcompress:	TCPIPLibs:sys.h.cdefs
od.slcompress:	TCPIPLibs:sys.h.cdefs
od.slcompress:	TCPIPLibs:sys.h.select
od.slcompress:	TCPIPLibs:h.inetlib
od.slcompress:	TCPIPLibs:sys.h.types
od.slcompress:	TCPIPLibs:netinet.h.in
od.slcompress:	TCPIPLibs:netinet.h.in_systm
od.slcompress:	TCPIPLibs:netinet.h.ip
od.slcompress:	TCPIPLibs:netinet.h.tcp
od.slcompress:	C:h.kernel
od.slcompress:	h.mbuf
od.slcompress:	h.slcompress
od.pppmodule:	c.pppmodule
od.pppmodule:	TCPIPLibs:sys.h.types
od.pppmodule:	TCPIPLibs:sys.h.cdefs
od.pppmodule:	TCPIPLibs:machine.h.endian
od.pppmodule:	TCPIPLibs:machine.h.ansi
od.pppmodule:	TCPIPLibs:machine.h.types
od.pppmodule:	TCPIPLibs:sys.h.dcistructs
od.pppmodule:	TCPIPLibs:sys.h.errno
od.pppmodule:	C:h.kernel
od.pppmodule:	TCPIPLibs:sys.h.mbuf
od.pppmodule:	TCPIPLibs:sys.h.malloc
od.pppmodule:	TCPIPLibs:sys.h.signal
od.pppmodule:	TCPIPLibs:machine.h.signal
od.pppmodule:	TCPIPLibs:sys.h.cdefs
od.pppmodule:	TCPIPLibs:sys.h.ioctl
od.pppmodule:	TCPIPLibs:sys.h.ttycom
od.pppmodule:	TCPIPLibs:sys.h.ioccom
od.pppmodule:	TCPIPLibs:sys.h.ioccom
od.pppmodule:	TCPIPLibs:sys.h.filio
od.pppmodule:	TCPIPLibs:sys.h.ioccom
od.pppmodule:	TCPIPLibs:sys.h.sockio
od.pppmodule:	TCPIPLibs:sys.h.ioccom
od.pppmodule:	TCPIPLibs:sys.h.cdefs
od.pppmodule:	TCPIPLibs:sys.h.syslog
od.pppmodule:	TCPIPLibs:machine.h.ansi
od.pppmodule:	TCPIPLibs:sys.h.cdefs
od.pppmodule:	pppd.h.minixlib
od.pppmodule:	pppd.h.riscos_ppp
od.pppmodule:	TCPIPLibs:net.h.if
od.pppmodule:	TCPIPLibs:sys.h.socket
od.pppmodule:	TCPIPLibs:sys.h.cdefs
od.pppmodule:	TCPIPLibs:sys.h.time
od.pppmodule:	TCPIPLibs:sys.h.cdefs
od.pppmodule:	TCPIPLibs:net.h.if_arp
od.pppmodule:	TCPIPLibs:sys.h.socket
od.pppmodule:	TCPIPLibs:netinet.h.in
od.pppmodule:	TCPIPLibs:netinet.h.in_systm
od.pppmodule:	TCPIPLibs:netinet.h.if_ether
od.pppmodule:	C:h.kernel
od.pppmodule:	C:h.swis
od.pppmodule:	h.ppp
od.pppmodule:	h.pppd
od.pppmodule:	h.pppmodule
od.pppmodule:	h.svcprint
od.pppmodule:	h.serial
od.pppmodule:	h.if_ppp
od.pppmodule:	TCPIPLibs:sys.h.socket
od.pppmodule:	TCPIPLibs:net.h.if
od.pppmodule:	TCPIPLibs:netinet.h.in
od.pppmodule:	TCPIPLibs:netinet.h.ip
od.pppmodule:	h.slcompress
od.pppmodule:	pppd.h.pppd
od.pppmodule:	TCPIPLibs:sys.h.param
od.pppmodule:	TCPIPLibs:sys.h.types
od.pppmodule:	TCPIPLibs:sys.h.syslimits
od.pppmodule:	TCPIPLibs:sys.h.signal
od.pppmodule:	TCPIPLibs:machine.h.param
od.pppmodule:	TCPIPLibs:machine.h.limits
od.pppmodule:	h.ioctl
od.pppmodule:	C:h.kernel
od.pppmodule:	pppd.h.main
od.pppmodule:	h.ppp_io
od.pppmodule:	h.ppphdr
od.pppmodule:	h.mbuf
od.pppmodule:	C:h.dialler
od.pppmodule:	h.svcprint
od.pppmodule:	C:Trace.h.Trace
o.blockdrive: s.blockdrive
o.if_ppp:	c.if_ppp
o.if_ppp:	TCPIPLibs:sys.h.dcistructs
o.if_ppp:	h.ppp
o.if_ppp:	TCPIPLibs:sys.h.errno
o.if_ppp:	C:h.kernel
o.if_ppp:	TCPIPLibs:sys.h.types
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	TCPIPLibs:machine.h.endian
o.if_ppp:	TCPIPLibs:machine.h.ansi
o.if_ppp:	TCPIPLibs:machine.h.types
o.if_ppp:	TCPIPLibs:sys.h.mbuf
o.if_ppp:	TCPIPLibs:sys.h.malloc
o.if_ppp:	TCPIPLibs:sys.h.socket
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	TCPIPLibs:sys.h.ioctl
o.if_ppp:	TCPIPLibs:sys.h.ttycom
o.if_ppp:	TCPIPLibs:sys.h.ioccom
o.if_ppp:	TCPIPLibs:sys.h.ioccom
o.if_ppp:	TCPIPLibs:sys.h.filio
o.if_ppp:	TCPIPLibs:sys.h.ioccom
o.if_ppp:	TCPIPLibs:sys.h.sockio
o.if_ppp:	TCPIPLibs:sys.h.ioccom
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	TCPIPLibs:sys.h.signal
o.if_ppp:	TCPIPLibs:machine.h.signal
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	TCPIPLibs:sys.h.tty
o.if_ppp:	TCPIPLibs:sys.h.termios
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	TCPIPLibs:sys.h.ttycom
o.if_ppp:	TCPIPLibs:sys.h.ttydefaults
o.if_ppp:	TCPIPLibs:sys.h.select
o.if_ppp:	TCPIPLibs:sys.h.conf
o.if_ppp:	TCPIPLibs:sys.h.syslog
o.if_ppp:	TCPIPLibs:machine.h.ansi
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	TCPIPLibs:net.h.if
o.if_ppp:	TCPIPLibs:sys.h.socket
o.if_ppp:	TCPIPLibs:sys.h.time
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	TCPIPLibs:net.h.if_arp
o.if_ppp:	TCPIPLibs:sys.h.socket
o.if_ppp:	TCPIPLibs:net.h.netisr
o.if_ppp:	TCPIPLibs:net.h.route
o.if_ppp:	TCPIPLibs:net.h.radix
o.if_ppp:	TCPIPLibs:netinet.h.in
o.if_ppp:	TCPIPLibs:netinet.h.if_ether
o.if_ppp:	TCPIPLibs:netinet.h.in_systm
o.if_ppp:	TCPIPLibs:netinet.h.in_var
o.if_ppp:	TCPIPLibs:sys.h.queue
o.if_ppp:	TCPIPLibs:netinet.h.ip
o.if_ppp:	TCPIPLibs:netinet.h.tcp
o.if_ppp:	TCPIPLibs:h.socklib
o.if_ppp:	TCPIPLibs:sys.h.types
o.if_ppp:	TCPIPLibs:sys.h.socket
o.if_ppp:	TCPIPLibs:sys.h.stat
o.if_ppp:	TCPIPLibs:sys.h.time
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	TCPIPLibs:sys.h.time
o.if_ppp:	TCPIPLibs:sys.h.uio
o.if_ppp:	TCPIPLibs:sys.h.cdefs
o.if_ppp:	C:h.kernel
o.if_ppp:	h.mbuf
o.if_ppp:	C:h.swis
o.if_ppp:	h.pppd
o.if_ppp:	h.ppp_io
o.if_ppp:	h.ppphdr
o.if_ppp:	h.pppmodule
o.if_ppp:	h.ioctl
o.if_ppp:	C:h.kernel
o.if_ppp:	h.if_ppp
o.if_ppp:	TCPIPLibs:sys.h.socket
o.if_ppp:	TCPIPLibs:net.h.if
o.if_ppp:	TCPIPLibs:netinet.h.in
o.if_ppp:	TCPIPLibs:netinet.h.ip
o.if_ppp:	h.slcompress
o.if_ppp:	h.serial
o.if_ppp:	C:h.dialler
o.if_ppp:	pppd.h.minixlib
o.if_ppp:	pppd.h.riscos_ppp
o.if_ppp:	pppd.h.main
o.mbuf:	c.mbuf
o.mbuf:	C:h.kernel
o.mbuf:	C:h.swis
o.mbuf:	TCPIPLibs:sys.h.param
o.mbuf:	TCPIPLibs:sys.h.types
o.mbuf:	TCPIPLibs:sys.h.cdefs
o.mbuf:	TCPIPLibs:machine.h.endian
o.mbuf:	TCPIPLibs:machine.h.ansi
o.mbuf:	TCPIPLibs:machine.h.types
o.mbuf:	TCPIPLibs:sys.h.syslimits
o.mbuf:	TCPIPLibs:sys.h.signal
o.mbuf:	TCPIPLibs:machine.h.signal
o.mbuf:	TCPIPLibs:sys.h.cdefs
o.mbuf:	TCPIPLibs:machine.h.param
o.mbuf:	TCPIPLibs:machine.h.limits
o.mbuf:	TCPIPLibs:sys.h.errno
o.mbuf:	C:h.kernel
o.mbuf:	TCPIPLibs:sys.h.mbuf
o.mbuf:	TCPIPLibs:sys.h.malloc
o.mbuf:	TCPIPLibs:net.h.if
o.mbuf:	TCPIPLibs:sys.h.socket
o.mbuf:	TCPIPLibs:sys.h.cdefs
o.mbuf:	TCPIPLibs:sys.h.time
o.mbuf:	TCPIPLibs:sys.h.cdefs
o.mbuf:	TCPIPLibs:net.h.if_arp
o.mbuf:	TCPIPLibs:sys.h.socket
o.mbuf:	TCPIPLibs:netinet.h.in
o.mbuf:	TCPIPLibs:netinet.h.if_ether
o.mbuf:	h.mbuf
o.ppp_io: s.ppp_io
o.ppp_io: Hdr.CVars
o.ppphdr:	cmhg.ppphdr
o.pppmodule:	c.pppmodule
o.pppmodule:	TCPIPLibs:sys.h.types
o.pppmodule:	TCPIPLibs:sys.h.cdefs
o.pppmodule:	TCPIPLibs:machine.h.endian
o.pppmodule:	TCPIPLibs:machine.h.ansi
o.pppmodule:	TCPIPLibs:machine.h.types
o.pppmodule:	TCPIPLibs:sys.h.dcistructs
o.pppmodule:	TCPIPLibs:sys.h.errno
o.pppmodule:	C:h.kernel
o.pppmodule:	TCPIPLibs:sys.h.mbuf
o.pppmodule:	TCPIPLibs:sys.h.malloc
o.pppmodule:	TCPIPLibs:sys.h.signal
o.pppmodule:	TCPIPLibs:machine.h.signal
o.pppmodule:	TCPIPLibs:sys.h.cdefs
o.pppmodule:	TCPIPLibs:sys.h.ioctl
o.pppmodule:	TCPIPLibs:sys.h.ttycom
o.pppmodule:	TCPIPLibs:sys.h.ioccom
o.pppmodule:	TCPIPLibs:sys.h.ioccom
o.pppmodule:	TCPIPLibs:sys.h.filio
o.pppmodule:	TCPIPLibs:sys.h.ioccom
o.pppmodule:	TCPIPLibs:sys.h.sockio
o.pppmodule:	TCPIPLibs:sys.h.ioccom
o.pppmodule:	TCPIPLibs:sys.h.cdefs
o.pppmodule:	TCPIPLibs:sys.h.syslog
o.pppmodule:	TCPIPLibs:machine.h.ansi
o.pppmodule:	TCPIPLibs:sys.h.cdefs
o.pppmodule:	pppd.h.minixlib
o.pppmodule:	pppd.h.riscos_ppp
o.pppmodule:	TCPIPLibs:net.h.if
o.pppmodule:	TCPIPLibs:sys.h.socket
o.pppmodule:	TCPIPLibs:sys.h.cdefs
o.pppmodule:	TCPIPLibs:sys.h.time
o.pppmodule:	TCPIPLibs:sys.h.cdefs
o.pppmodule:	TCPIPLibs:net.h.if_arp
o.pppmodule:	TCPIPLibs:sys.h.socket
o.pppmodule:	TCPIPLibs:netinet.h.in
o.pppmodule:	TCPIPLibs:netinet.h.in_systm
o.pppmodule:	TCPIPLibs:netinet.h.if_ether
o.pppmodule:	C:h.kernel
o.pppmodule:	C:h.swis
o.pppmodule:	h.ppp
o.pppmodule:	h.pppd
o.pppmodule:	h.pppmodule
o.pppmodule:	h.serial
o.pppmodule:	h.if_ppp
o.pppmodule:	TCPIPLibs:sys.h.socket
o.pppmodule:	TCPIPLibs:net.h.if
o.pppmodule:	TCPIPLibs:netinet.h.in
o.pppmodule:	TCPIPLibs:netinet.h.ip
o.pppmodule:	h.slcompress
o.pppmodule:	pppd.h.pppd
o.pppmodule:	TCPIPLibs:sys.h.param
o.pppmodule:	TCPIPLibs:sys.h.types
o.pppmodule:	TCPIPLibs:sys.h.syslimits
o.pppmodule:	TCPIPLibs:sys.h.signal
o.pppmodule:	TCPIPLibs:machine.h.param
o.pppmodule:	TCPIPLibs:machine.h.limits
o.pppmodule:	h.ioctl
o.pppmodule:	C:h.kernel
o.pppmodule:	pppd.h.main
o.pppmodule:	h.ppp_io
o.pppmodule:	h.ppphdr
o.pppmodule:	h.mbuf
o.pppmodule:	C:h.dialler
o.pppmodule:	h.svcprint
o.serial:	c.serial
o.serial:	TCPIPLibs:h.errno
o.serial:	TCPIPLibs:sys.h.errno
o.serial:	C:h.kernel
o.serial:	TCPIPLibs:sys.h.types
o.serial:	TCPIPLibs:sys.h.cdefs
o.serial:	TCPIPLibs:machine.h.endian
o.serial:	TCPIPLibs:machine.h.ansi
o.serial:	TCPIPLibs:machine.h.types
o.serial:	TCPIPLibs:sys.h.dcistructs
o.serial:	TCPIPLibs:net.h.if
o.serial:	TCPIPLibs:sys.h.socket
o.serial:	TCPIPLibs:sys.h.cdefs
o.serial:	TCPIPLibs:sys.h.time
o.serial:	TCPIPLibs:sys.h.cdefs
o.serial:	TCPIPLibs:net.h.if_arp
o.serial:	TCPIPLibs:sys.h.socket
o.serial:	TCPIPLibs:netinet.h.in_systm
o.serial:	TCPIPLibs:netinet.h.in
o.serial:	TCPIPLibs:netinet.h.if_ether
o.serial:	TCPIPLibs:sys.h.signal
o.serial:	TCPIPLibs:machine.h.signal
o.serial:	TCPIPLibs:sys.h.cdefs
o.serial:	TCPIPLibs:sys.h.syslog
o.serial:	TCPIPLibs:machine.h.ansi
o.serial:	TCPIPLibs:sys.h.cdefs
o.serial:	C:h.kernel
o.serial:	C:h.swis
o.serial:	h.ppp_io
o.serial:	h.pppmodule
o.serial:	h.ioctl
o.serial:	C:h.kernel
o.serial:	h.if_ppp
o.serial:	h.ppp
o.serial:	TCPIPLibs:sys.h.socket
o.serial:	TCPIPLibs:net.h.if
o.serial:	TCPIPLibs:netinet.h.in
o.serial:	TCPIPLibs:netinet.h.ip
o.serial:	h.slcompress
o.serial:	h.ppphdr
o.serial:	pppd.h.minixlib
o.serial:	pppd.h.riscos_ppp
o.serial:	pppd.h.main
o.serial:	h.serial
o.serial:	C:h.dialler
o.slcompress:	c.slcompress
o.slcompress:	TCPIPLibs:sys.h.types
o.slcompress:	TCPIPLibs:sys.h.cdefs
o.slcompress:	TCPIPLibs:machine.h.endian
o.slcompress:	TCPIPLibs:machine.h.ansi
o.slcompress:	TCPIPLibs:machine.h.types
o.slcompress:	TCPIPLibs:sys.h.param
o.slcompress:	TCPIPLibs:sys.h.types
o.slcompress:	TCPIPLibs:sys.h.syslimits
o.slcompress:	TCPIPLibs:sys.h.signal
o.slcompress:	TCPIPLibs:machine.h.signal
o.slcompress:	TCPIPLibs:sys.h.cdefs
o.slcompress:	TCPIPLibs:machine.h.param
o.slcompress:	TCPIPLibs:machine.h.limits
o.slcompress:	TCPIPLibs:sys.h.mbuf
o.slcompress:	TCPIPLibs:sys.h.malloc
o.slcompress:	TCPIPLibs:sys.h.socketvar
o.slcompress:	TCPIPLibs:sys.h.stat
o.slcompress:	TCPIPLibs:sys.h.time
o.slcompress:	TCPIPLibs:sys.h.cdefs
o.slcompress:	TCPIPLibs:sys.h.cdefs
o.slcompress:	TCPIPLibs:sys.h.select
o.slcompress:	TCPIPLibs:h.inetlib
o.slcompress:	TCPIPLibs:sys.h.types
o.slcompress:	TCPIPLibs:netinet.h.in
o.slcompress:	TCPIPLibs:netinet.h.in_systm
o.slcompress:	TCPIPLibs:netinet.h.ip
o.slcompress:	TCPIPLibs:netinet.h.tcp
o.slcompress:	C:h.kernel
o.slcompress:	h.mbuf
o.slcompress:	h.slcompress
o.ppphdr:	cmhg.ppphdr
