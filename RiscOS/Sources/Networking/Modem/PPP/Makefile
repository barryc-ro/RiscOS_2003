# Makefile for PPP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 16-Jul-96  SMC          Created
#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm -tb
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

#
# Include files
#
INCLUDES=	TCPIPLibs:,C:

INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm

#
# Program specific options:
#
COMPONENT = PPP
TARGET = rm.PPP
ROMTARGET = aof.PPP

# To compile a version of PPP which uses the blockdrivers uncomment the next
# line, ensure that o.blockdrive is in the list of object files and change
# ASMFLAGS so that the symbol BlockDrivers is predefined to {TRUE}
# 
# The Makefile in pppd must also be changed

BLOCKDRIVERS=	-DBLOCKDRIVERS


# You may want to link with TinyStubs. if you do, set LD to tlink and
# uncomment this TINYSTUBS, and comment out the STUBS

TINYSTUBS=	#-DTINYSTUBS
STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

DEBUG=		#-DDEBUG

COMPAT=		-DCOMPAT_INET4

CFLAGS=		-ffah -zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -Wp -depend !Depend -c -DVJC ${TINYSTUBS}\
		${BLOCKDRIVERS}

ASMFLAGS=       -PreDefine "BlockDrivers SETL {TRUE}"

OBJS=		o.if_ppp o.slcompress o.pppmodule \
		o.serial o.mbuf o.ppphdr o.ppp_io o.blockdrive \
		${SOCKLIB} ${INETLIB} ${UNIXLIB} \
		#o.svcprint

#
# Rule patterns
#
.SUFFIXES:	.o .s .c
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<

all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export:
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
		$(WIPE) o.*      $(WFLAGS)
		$(WIPE) pppd.o.* $(WFLAGS)
                $(WIPE) rm.*     $(WFLAGS)
		$(RM) link/sym

#
# Target 
#
${TARGET}:	$(OBJS) pppd
		$(LD) -rmf -s link/sym -o rm.PPPu $(OBJS) $(STUBS) pppd.o.pppd
		ModSqz rm.PPPu $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} pppd
        ${LD} -s link/sym -o $@ -aof ${OBJS} ${ROMCSTUBS} pppd.o.pppd

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

#
# Some rules that need explicitly declaring
#
pppd:
        Dir pppd
        Amu
        Up

s.blockdrive::	Internal.Driver
	
o.ppphdr:	cmhg.ppphdr
		cmhg -p ${BLOCKDRIVERS} -depend !Depend \
		cmhg.ppphdr o.ppphdr h.ppphdr


# Dynamic dependencies:
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
o.if_ppp:	h.blockdrive
o.if_ppp:	h.if_ppp
o.if_ppp:	TCPIPLibs:sys.h.socket
o.if_ppp:	TCPIPLibs:net.h.if
o.if_ppp:	TCPIPLibs:netinet.h.in
o.if_ppp:	TCPIPLibs:netinet.h.ip
o.if_ppp:	h.slcompress
o.if_ppp:	h.serial
o.if_ppp:	h.dialler
o.if_ppp:	C:h.kernel
o.if_ppp:	pppd.h.minixlib
o.if_ppp:	pppd.h.riscos_ppp
o.if_ppp:	pppd.h.main
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
o.pppmodule:	h.blockdrive
o.pppmodule:	pppd.h.main
o.pppmodule:	h.ppp_io
o.pppmodule:	h.ppphdr
o.pppmodule:	h.mbuf
o.pppmodule:	h.dialler
o.pppmodule:	C:h.kernel
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
o.serial:	h.blockdrive
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
o.serial:	h.dialler
o.serial:	C:h.kernel
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
o.ppphdr:	cmhg.ppphdr
