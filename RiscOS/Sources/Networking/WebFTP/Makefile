# Makefile for webftp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR = <Cexport$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS} -wp -DCOMPAT_INET4
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
INETLIB   = <Lib$Dir>.InetLib.o.inetlibzm
SOCKLIB   = <Lib$Dir>.InetLib.o.socklibzm
UNIXLIB   = <Lib$Dir>.InetLib.o.unixlibzm
#
# Include files
#

INCLUDES  = -IC:,<Lib$Dir>.InetLib

DFLAGS = -DVERBOSEDEBUG=0 -DRISCOS -DDEBUG=0 -DPROG_IDENT="\"NCBrowser\""

#
# Program specific options:
#
COMPONENT = webftp
TARGET    = aof.${COMPONENT}
OBJS      =  webftp.o q.o files.o header.o support.o misc.o dirparse.o
EXPORTS   = h.webftp

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
	${CP} h.webftp ${EXP_HDR}.h.webftp ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT} 
	${CP} rm.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
#        ${MKDIR} ${RESDIR}.${COMPONENT}
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${INETLIB} ${SOCKLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${INETLIB} ${SOCKLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB} ${INETLIB} ${SOCKLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${INETLIB} ${SOCKLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.webftp:	c.webftp
o.webftp:	h.all
o.webftp:	h.tcplibs
o.webftp:	<Lib$Dir>.InetLib.sys.h.types
o.webftp:	<Lib$Dir>.InetLib.sys.h.cdefs
o.webftp:	<Lib$Dir>.InetLib.machine.h.endian
o.webftp:	<Lib$Dir>.InetLib.machine.h.ansi
o.webftp:	<Lib$Dir>.InetLib.machine.h.types
o.webftp:	<Lib$Dir>.InetLib.sys.h.socket
o.webftp:	<Lib$Dir>.InetLib.sys.h.cdefs
o.webftp:	<Lib$Dir>.InetLib.sys.h.errno
o.webftp:	C:h.kernel
o.webftp:	<Lib$Dir>.InetLib.net.h.if
o.webftp:	<Lib$Dir>.InetLib.sys.h.socket
o.webftp:	<Lib$Dir>.InetLib.sys.h.time
o.webftp:	<Lib$Dir>.InetLib.sys.h.cdefs
o.webftp:	<Lib$Dir>.InetLib.net.h.if_arp
o.webftp:	<Lib$Dir>.InetLib.sys.h.socket
o.webftp:	<Lib$Dir>.InetLib.netinet.h.in
o.webftp:	<Lib$Dir>.InetLib.sys.h.ioctl
o.webftp:	<Lib$Dir>.InetLib.sys.h.ttycom
o.webftp:	<Lib$Dir>.InetLib.sys.h.ioccom
o.webftp:	<Lib$Dir>.InetLib.sys.h.ioccom
o.webftp:	<Lib$Dir>.InetLib.sys.h.filio
o.webftp:	<Lib$Dir>.InetLib.sys.h.ioccom
o.webftp:	<Lib$Dir>.InetLib.sys.h.sockio
o.webftp:	<Lib$Dir>.InetLib.sys.h.ioccom
o.webftp:	<Lib$Dir>.InetLib.sys.h.cdefs
o.webftp:	<Lib$Dir>.InetLib.h.netdb
o.webftp:	<Lib$Dir>.InetLib.sys.h.cdefs
o.webftp:	<Lib$Dir>.InetLib.netns.h.ns
o.webftp:	<Lib$Dir>.InetLib.sys.h.cdefs
o.webftp:	<Lib$Dir>.InetLib.sys.h.time
o.webftp:	<Lib$Dir>.InetLib.h.socklib
o.webftp:	<Lib$Dir>.InetLib.sys.h.types
o.webftp:	<Lib$Dir>.InetLib.sys.h.socket
o.webftp:	<Lib$Dir>.InetLib.sys.h.stat
o.webftp:	<Lib$Dir>.InetLib.sys.h.time
o.webftp:	<Lib$Dir>.InetLib.sys.h.cdefs
o.webftp:	<Lib$Dir>.InetLib.sys.h.time
o.webftp:	<Lib$Dir>.InetLib.sys.h.uio
o.webftp:	<Lib$Dir>.InetLib.sys.h.cdefs
o.webftp:	C:h.kernel
o.webftp:	<Lib$Dir>.InetLib.h.inetlib
o.webftp:	<Lib$Dir>.InetLib.sys.h.types
o.webftp:	<Lib$Dir>.InetLib.netinet.h.in
o.webftp:	C:h.kernel
o.webftp:	C:h.swis
o.webftp:	h.files
o.webftp:	h.q
o.webftp:	h.status
o.webftp:	h.webftp
o.webftp:	h.pads
o.webftp:	h.privwebftp
o.webftp:	h.autofns
o.q:	c.q
o.q:	h.all
o.q:	h.tcplibs
o.q:	<Lib$Dir>.InetLib.sys.h.types
o.q:	<Lib$Dir>.InetLib.sys.h.cdefs
o.q:	<Lib$Dir>.InetLib.machine.h.endian
o.q:	<Lib$Dir>.InetLib.machine.h.ansi
o.q:	<Lib$Dir>.InetLib.machine.h.types
o.q:	<Lib$Dir>.InetLib.sys.h.socket
o.q:	<Lib$Dir>.InetLib.sys.h.cdefs
o.q:	<Lib$Dir>.InetLib.sys.h.errno
o.q:	C:h.kernel
o.q:	<Lib$Dir>.InetLib.net.h.if
o.q:	<Lib$Dir>.InetLib.sys.h.socket
o.q:	<Lib$Dir>.InetLib.sys.h.time
o.q:	<Lib$Dir>.InetLib.sys.h.cdefs
o.q:	<Lib$Dir>.InetLib.net.h.if_arp
o.q:	<Lib$Dir>.InetLib.sys.h.socket
o.q:	<Lib$Dir>.InetLib.netinet.h.in
o.q:	<Lib$Dir>.InetLib.sys.h.ioctl
o.q:	<Lib$Dir>.InetLib.sys.h.ttycom
o.q:	<Lib$Dir>.InetLib.sys.h.ioccom
o.q:	<Lib$Dir>.InetLib.sys.h.ioccom
o.q:	<Lib$Dir>.InetLib.sys.h.filio
o.q:	<Lib$Dir>.InetLib.sys.h.ioccom
o.q:	<Lib$Dir>.InetLib.sys.h.sockio
o.q:	<Lib$Dir>.InetLib.sys.h.ioccom
o.q:	<Lib$Dir>.InetLib.sys.h.cdefs
o.q:	<Lib$Dir>.InetLib.h.netdb
o.q:	<Lib$Dir>.InetLib.sys.h.cdefs
o.q:	<Lib$Dir>.InetLib.netns.h.ns
o.q:	<Lib$Dir>.InetLib.sys.h.cdefs
o.q:	<Lib$Dir>.InetLib.sys.h.time
o.q:	<Lib$Dir>.InetLib.h.socklib
o.q:	<Lib$Dir>.InetLib.sys.h.types
o.q:	<Lib$Dir>.InetLib.sys.h.socket
o.q:	<Lib$Dir>.InetLib.sys.h.stat
o.q:	<Lib$Dir>.InetLib.sys.h.time
o.q:	<Lib$Dir>.InetLib.sys.h.cdefs
o.q:	<Lib$Dir>.InetLib.sys.h.time
o.q:	<Lib$Dir>.InetLib.sys.h.uio
o.q:	<Lib$Dir>.InetLib.sys.h.cdefs
o.q:	C:h.kernel
o.q:	<Lib$Dir>.InetLib.h.inetlib
o.q:	<Lib$Dir>.InetLib.sys.h.types
o.q:	<Lib$Dir>.InetLib.netinet.h.in
o.q:	C:h.kernel
o.q:	C:h.swis
o.q:	h.files
o.q:	h.q
o.q:	h.status
o.q:	h.webftp
o.q:	h.pads
o.q:	h.privwebftp
o.q:	h.autofns
o.files:	c.files
o.files:	h.tcplibs
o.files:	<Lib$Dir>.InetLib.sys.h.types
o.files:	<Lib$Dir>.InetLib.sys.h.cdefs
o.files:	<Lib$Dir>.InetLib.machine.h.endian
o.files:	<Lib$Dir>.InetLib.machine.h.ansi
o.files:	<Lib$Dir>.InetLib.machine.h.types
o.files:	<Lib$Dir>.InetLib.sys.h.socket
o.files:	<Lib$Dir>.InetLib.sys.h.cdefs
o.files:	<Lib$Dir>.InetLib.sys.h.errno
o.files:	C:h.kernel
o.files:	<Lib$Dir>.InetLib.net.h.if
o.files:	<Lib$Dir>.InetLib.sys.h.socket
o.files:	<Lib$Dir>.InetLib.sys.h.time
o.files:	<Lib$Dir>.InetLib.sys.h.cdefs
o.files:	<Lib$Dir>.InetLib.net.h.if_arp
o.files:	<Lib$Dir>.InetLib.sys.h.socket
o.files:	<Lib$Dir>.InetLib.netinet.h.in
o.files:	<Lib$Dir>.InetLib.sys.h.ioctl
o.files:	<Lib$Dir>.InetLib.sys.h.ttycom
o.files:	<Lib$Dir>.InetLib.sys.h.ioccom
o.files:	<Lib$Dir>.InetLib.sys.h.ioccom
o.files:	<Lib$Dir>.InetLib.sys.h.filio
o.files:	<Lib$Dir>.InetLib.sys.h.ioccom
o.files:	<Lib$Dir>.InetLib.sys.h.sockio
o.files:	<Lib$Dir>.InetLib.sys.h.ioccom
o.files:	<Lib$Dir>.InetLib.sys.h.cdefs
o.files:	<Lib$Dir>.InetLib.h.netdb
o.files:	<Lib$Dir>.InetLib.sys.h.cdefs
o.files:	<Lib$Dir>.InetLib.netns.h.ns
o.files:	<Lib$Dir>.InetLib.sys.h.cdefs
o.files:	<Lib$Dir>.InetLib.sys.h.time
o.files:	<Lib$Dir>.InetLib.h.socklib
o.files:	<Lib$Dir>.InetLib.sys.h.types
o.files:	<Lib$Dir>.InetLib.sys.h.socket
o.files:	<Lib$Dir>.InetLib.sys.h.stat
o.files:	<Lib$Dir>.InetLib.sys.h.time
o.files:	<Lib$Dir>.InetLib.sys.h.cdefs
o.files:	<Lib$Dir>.InetLib.sys.h.time
o.files:	<Lib$Dir>.InetLib.sys.h.uio
o.files:	<Lib$Dir>.InetLib.sys.h.cdefs
o.files:	C:h.kernel
o.files:	<Lib$Dir>.InetLib.h.inetlib
o.files:	<Lib$Dir>.InetLib.sys.h.types
o.files:	<Lib$Dir>.InetLib.netinet.h.in
o.files:	C:h.kernel
o.files:	C:h.swis
o.files:	h.files
o.files:	h.q
o.files:	h.status
o.files:	h.webftp
o.files:	h.pads
o.files:	h.privwebftp
o.files:	h.autofns
o.support: s.support
o.support: hdr.swinames
o.support: hdr.regnames
o.misc:	c.misc
o.misc:	h.all
o.misc:	h.tcplibs
o.misc:	<Lib$Dir>.InetLib.sys.h.types
o.misc:	<Lib$Dir>.InetLib.sys.h.cdefs
o.misc:	<Lib$Dir>.InetLib.machine.h.endian
o.misc:	<Lib$Dir>.InetLib.machine.h.ansi
o.misc:	<Lib$Dir>.InetLib.machine.h.types
o.misc:	<Lib$Dir>.InetLib.sys.h.socket
o.misc:	<Lib$Dir>.InetLib.sys.h.cdefs
o.misc:	<Lib$Dir>.InetLib.sys.h.errno
o.misc:	C:h.kernel
o.misc:	<Lib$Dir>.InetLib.net.h.if
o.misc:	<Lib$Dir>.InetLib.sys.h.socket
o.misc:	<Lib$Dir>.InetLib.sys.h.time
o.misc:	<Lib$Dir>.InetLib.sys.h.cdefs
o.misc:	<Lib$Dir>.InetLib.net.h.if_arp
o.misc:	<Lib$Dir>.InetLib.sys.h.socket
o.misc:	<Lib$Dir>.InetLib.netinet.h.in
o.misc:	<Lib$Dir>.InetLib.sys.h.ioctl
o.misc:	<Lib$Dir>.InetLib.sys.h.ttycom
o.misc:	<Lib$Dir>.InetLib.sys.h.ioccom
o.misc:	<Lib$Dir>.InetLib.sys.h.ioccom
o.misc:	<Lib$Dir>.InetLib.sys.h.filio
o.misc:	<Lib$Dir>.InetLib.sys.h.ioccom
o.misc:	<Lib$Dir>.InetLib.sys.h.sockio
o.misc:	<Lib$Dir>.InetLib.sys.h.ioccom
o.misc:	<Lib$Dir>.InetLib.sys.h.cdefs
o.misc:	<Lib$Dir>.InetLib.h.netdb
o.misc:	<Lib$Dir>.InetLib.sys.h.cdefs
o.misc:	<Lib$Dir>.InetLib.netns.h.ns
o.misc:	<Lib$Dir>.InetLib.sys.h.cdefs
o.misc:	<Lib$Dir>.InetLib.sys.h.time
o.misc:	<Lib$Dir>.InetLib.h.socklib
o.misc:	<Lib$Dir>.InetLib.sys.h.types
o.misc:	<Lib$Dir>.InetLib.sys.h.socket
o.misc:	<Lib$Dir>.InetLib.sys.h.stat
o.misc:	<Lib$Dir>.InetLib.sys.h.time
o.misc:	<Lib$Dir>.InetLib.sys.h.cdefs
o.misc:	<Lib$Dir>.InetLib.sys.h.time
o.misc:	<Lib$Dir>.InetLib.sys.h.uio
o.misc:	<Lib$Dir>.InetLib.sys.h.cdefs
o.misc:	C:h.kernel
o.misc:	<Lib$Dir>.InetLib.h.inetlib
o.misc:	<Lib$Dir>.InetLib.sys.h.types
o.misc:	<Lib$Dir>.InetLib.netinet.h.in
o.misc:	C:h.kernel
o.misc:	C:h.swis
o.misc:	h.files
o.misc:	h.q
o.misc:	h.status
o.misc:	h.webftp
o.misc:	h.pads
o.misc:	h.privwebftp
o.misc:	h.autofns
o.dirparse:	c.dirparse
o.dirparse:	h.all
o.dirparse:	h.tcplibs
o.dirparse:	<Lib$Dir>.InetLib.sys.h.types
o.dirparse:	<Lib$Dir>.InetLib.sys.h.cdefs
o.dirparse:	<Lib$Dir>.InetLib.machine.h.endian
o.dirparse:	<Lib$Dir>.InetLib.machine.h.ansi
o.dirparse:	<Lib$Dir>.InetLib.machine.h.types
o.dirparse:	<Lib$Dir>.InetLib.sys.h.socket
o.dirparse:	<Lib$Dir>.InetLib.sys.h.cdefs
o.dirparse:	<Lib$Dir>.InetLib.sys.h.errno
o.dirparse:	C:h.kernel
o.dirparse:	<Lib$Dir>.InetLib.net.h.if
o.dirparse:	<Lib$Dir>.InetLib.sys.h.socket
o.dirparse:	<Lib$Dir>.InetLib.sys.h.time
o.dirparse:	<Lib$Dir>.InetLib.sys.h.cdefs
o.dirparse:	<Lib$Dir>.InetLib.net.h.if_arp
o.dirparse:	<Lib$Dir>.InetLib.sys.h.socket
o.dirparse:	<Lib$Dir>.InetLib.netinet.h.in
o.dirparse:	<Lib$Dir>.InetLib.sys.h.ioctl
o.dirparse:	<Lib$Dir>.InetLib.sys.h.ttycom
o.dirparse:	<Lib$Dir>.InetLib.sys.h.ioccom
o.dirparse:	<Lib$Dir>.InetLib.sys.h.ioccom
o.dirparse:	<Lib$Dir>.InetLib.sys.h.filio
o.dirparse:	<Lib$Dir>.InetLib.sys.h.ioccom
o.dirparse:	<Lib$Dir>.InetLib.sys.h.sockio
o.dirparse:	<Lib$Dir>.InetLib.sys.h.ioccom
o.dirparse:	<Lib$Dir>.InetLib.sys.h.cdefs
o.dirparse:	<Lib$Dir>.InetLib.h.netdb
o.dirparse:	<Lib$Dir>.InetLib.sys.h.cdefs
o.dirparse:	<Lib$Dir>.InetLib.netns.h.ns
o.dirparse:	<Lib$Dir>.InetLib.sys.h.cdefs
o.dirparse:	<Lib$Dir>.InetLib.sys.h.time
o.dirparse:	<Lib$Dir>.InetLib.h.socklib
o.dirparse:	<Lib$Dir>.InetLib.sys.h.types
o.dirparse:	<Lib$Dir>.InetLib.sys.h.socket
o.dirparse:	<Lib$Dir>.InetLib.sys.h.stat
o.dirparse:	<Lib$Dir>.InetLib.sys.h.time
o.dirparse:	<Lib$Dir>.InetLib.sys.h.cdefs
o.dirparse:	<Lib$Dir>.InetLib.sys.h.time
o.dirparse:	<Lib$Dir>.InetLib.sys.h.uio
o.dirparse:	<Lib$Dir>.InetLib.sys.h.cdefs
o.dirparse:	C:h.kernel
o.dirparse:	<Lib$Dir>.InetLib.h.inetlib
o.dirparse:	<Lib$Dir>.InetLib.sys.h.types
o.dirparse:	<Lib$Dir>.InetLib.netinet.h.in
o.dirparse:	C:h.kernel
o.dirparse:	C:h.swis
o.dirparse:	h.files
o.dirparse:	h.q
o.dirparse:	h.status
o.dirparse:	h.webftp
o.dirparse:	h.pads
o.dirparse:	h.privwebftp
o.dirparse:	h.autofns
