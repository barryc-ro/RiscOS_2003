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
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -zps1 -ff ${INCLUDES} ${DFLAGS} -wp
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}
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
INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm
INETLIB_NORM=TCPIPLibs:o.inetlib
SOCKLIB_NORM=TCPIPLibs:o.socklib
#REMOTEDB  = <Lib$Dir>.debug.o.remotezm
REMOTEDB  = 
#
# Include files
#

INCLUDES  = -IC:,TCPIPLibs:,<Lib$Dir>.

# enable malloc debugging using the following define.
#-DDEBUG_MALLOC
DFLAGS = -DVERBOSEDEBUG=0 -DRISCOS -DPROG_IDENT="\"NCBrowser\"" -DCOMPAT_INET4 -DFTP_PUT -DCSFS ${FTP_DB} ${FTP_REMOTE_DB} -DCMHG_FINALISE=1

#
# Program specific options:
#
COMPONENT = webftp
TARGET    = aof.${COMPONENT}
OBJS      =  webftp.o q.o files.o header.o support.o misc.o db.o dirparse.o 
EXPORTS   = h.webftp

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
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
${TARGET}: ${OBJS} ${ROMCSTUBS} ${INETLIB} ${SOCKLIB} ${REMOTEDB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${INETLIB} ${SOCKLIB} ${REMOTEDB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB} ${INETLIB} ${SOCKLIB} ${REMOTEDB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${INETLIB} ${SOCKLIB} ${REMOTEDB}
        Access rm.${COMPONENT} RW/R

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

test: o.test1
        ${LD} -o $@ o.test1 ${CLIB} ${INETLIB_NORM}
        @echo Test built

o.test1: c.test1
        ${CC} -c ${INCLUDES} ${DFLAGS} -o $@ c.test1

# Dynamic dependencies:
o.webftp:	c.webftp
o.webftp:	h.all
o.webftp:	h.tcplibs
o.webftp:	TCPIPLibs:sys.h.types
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:machine.h.endian
o.webftp:	TCPIPLibs:machine.h.ansi
o.webftp:	TCPIPLibs:machine.h.types
o.webftp:	TCPIPLibs:sys.h.socket
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:sys.h.errno
o.webftp:	C:h.kernel
o.webftp:	TCPIPLibs:net.h.if
o.webftp:	TCPIPLibs:sys.h.socket
o.webftp:	TCPIPLibs:sys.h.time
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:net.h.if_arp
o.webftp:	TCPIPLibs:sys.h.socket
o.webftp:	TCPIPLibs:netinet.h.in
o.webftp:	TCPIPLibs:sys.h.ioctl
o.webftp:	TCPIPLibs:sys.h.ttycom
o.webftp:	TCPIPLibs:sys.h.ioccom
o.webftp:	TCPIPLibs:sys.h.ioccom
o.webftp:	TCPIPLibs:sys.h.filio
o.webftp:	TCPIPLibs:sys.h.ioccom
o.webftp:	TCPIPLibs:sys.h.sockio
o.webftp:	TCPIPLibs:sys.h.ioccom
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:h.netdb
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:netns.h.ns
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:sys.h.time
o.webftp:	TCPIPLibs:h.socklib
o.webftp:	TCPIPLibs:sys.h.types
o.webftp:	TCPIPLibs:sys.h.socket
o.webftp:	TCPIPLibs:sys.h.stat
o.webftp:	TCPIPLibs:sys.h.time
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:sys.h.time
o.webftp:	TCPIPLibs:sys.h.uio
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	C:h.kernel
o.webftp:	TCPIPLibs:h.inetlib
o.webftp:	TCPIPLibs:sys.h.types
o.webftp:	TCPIPLibs:netinet.h.in
o.webftp:	TCPIPLibs:netinet.h.tcp_timer
o.webftp:	C:h.kernel
o.webftp:	C:h.swis
o.webftp:	h.files
o.webftp:	h.q
o.webftp:	h.status
o.webftp:	h.webftp
o.webftp:	h.pads
o.webftp:	h.privwebftp
o.webftp:	h.autofns
o.webftp:	h.csfs
o.webftp:	c.webftp
o.webftp:	h.all
o.webftp:	h.tcplibs
o.webftp:	TCPIPLibs:sys.h.types
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:machine.h.endian
o.webftp:	TCPIPLibs:machine.h.ansi
o.webftp:	TCPIPLibs:machine.h.types
o.webftp:	TCPIPLibs:sys.h.socket
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:sys.h.errno
o.webftp:	C:h.kernel
o.webftp:	TCPIPLibs:net.h.if
o.webftp:	TCPIPLibs:sys.h.socket
o.webftp:	TCPIPLibs:sys.h.time
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:net.h.if_arp
o.webftp:	TCPIPLibs:sys.h.socket
o.webftp:	TCPIPLibs:netinet.h.in
o.webftp:	TCPIPLibs:sys.h.ioctl
o.webftp:	TCPIPLibs:sys.h.ttycom
o.webftp:	TCPIPLibs:sys.h.ioccom
o.webftp:	TCPIPLibs:sys.h.ioccom
o.webftp:	TCPIPLibs:sys.h.filio
o.webftp:	TCPIPLibs:sys.h.ioccom
o.webftp:	TCPIPLibs:sys.h.sockio
o.webftp:	TCPIPLibs:sys.h.ioccom
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:h.netdb
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:netns.h.ns
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:sys.h.time
o.webftp:	TCPIPLibs:h.socklib
o.webftp:	TCPIPLibs:sys.h.types
o.webftp:	TCPIPLibs:sys.h.socket
o.webftp:	TCPIPLibs:sys.h.stat
o.webftp:	TCPIPLibs:sys.h.time
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	TCPIPLibs:sys.h.time
o.webftp:	TCPIPLibs:sys.h.uio
o.webftp:	TCPIPLibs:sys.h.cdefs
o.webftp:	C:h.kernel
o.webftp:	TCPIPLibs:h.inetlib
o.webftp:	TCPIPLibs:sys.h.types
o.webftp:	TCPIPLibs:netinet.h.in
o.webftp:	TCPIPLibs:netinet.h.tcp_timer
o.webftp:	C:h.kernel
o.webftp:	C:h.swis
o.webftp:	h.files
o.webftp:	h.q
o.webftp:	h.status
o.webftp:	h.webftp
o.webftp:	h.pads
o.webftp:	h.privwebftp
o.webftp:	h.autofns
o.webftp:	h.csfs
o.q:	c.q
o.q:	h.all
o.q:	h.tcplibs
o.q:	TCPIPLibs:sys.h.types
o.q:	TCPIPLibs:sys.h.cdefs
o.q:	TCPIPLibs:machine.h.endian
o.q:	TCPIPLibs:machine.h.ansi
o.q:	TCPIPLibs:machine.h.types
o.q:	TCPIPLibs:sys.h.socket
o.q:	TCPIPLibs:sys.h.cdefs
o.q:	TCPIPLibs:sys.h.errno
o.q:	C:h.kernel
o.q:	TCPIPLibs:net.h.if
o.q:	TCPIPLibs:sys.h.socket
o.q:	TCPIPLibs:sys.h.time
o.q:	TCPIPLibs:sys.h.cdefs
o.q:	TCPIPLibs:net.h.if_arp
o.q:	TCPIPLibs:sys.h.socket
o.q:	TCPIPLibs:netinet.h.in
o.q:	TCPIPLibs:sys.h.ioctl
o.q:	TCPIPLibs:sys.h.ttycom
o.q:	TCPIPLibs:sys.h.ioccom
o.q:	TCPIPLibs:sys.h.ioccom
o.q:	TCPIPLibs:sys.h.filio
o.q:	TCPIPLibs:sys.h.ioccom
o.q:	TCPIPLibs:sys.h.sockio
o.q:	TCPIPLibs:sys.h.ioccom
o.q:	TCPIPLibs:sys.h.cdefs
o.q:	TCPIPLibs:h.netdb
o.q:	TCPIPLibs:sys.h.cdefs
o.q:	TCPIPLibs:netns.h.ns
o.q:	TCPIPLibs:sys.h.cdefs
o.q:	TCPIPLibs:sys.h.time
o.q:	TCPIPLibs:h.socklib
o.q:	TCPIPLibs:sys.h.types
o.q:	TCPIPLibs:sys.h.socket
o.q:	TCPIPLibs:sys.h.stat
o.q:	TCPIPLibs:sys.h.time
o.q:	TCPIPLibs:sys.h.cdefs
o.q:	TCPIPLibs:sys.h.time
o.q:	TCPIPLibs:sys.h.uio
o.q:	TCPIPLibs:sys.h.cdefs
o.q:	C:h.kernel
o.q:	TCPIPLibs:h.inetlib
o.q:	TCPIPLibs:sys.h.types
o.q:	TCPIPLibs:netinet.h.in
o.q:	TCPIPLibs:netinet.h.tcp_timer
o.q:	C:h.kernel
o.q:	C:h.swis
o.q:	h.files
o.q:	h.q
o.q:	h.status
o.q:	h.webftp
o.q:	h.pads
o.q:	h.privwebftp
o.q:	h.autofns
o.q:	h.csfs
o.files:	c.files
o.files:	h.tcplibs
o.files:	TCPIPLibs:sys.h.types
o.files:	TCPIPLibs:sys.h.cdefs
o.files:	TCPIPLibs:machine.h.endian
o.files:	TCPIPLibs:machine.h.ansi
o.files:	TCPIPLibs:machine.h.types
o.files:	TCPIPLibs:sys.h.socket
o.files:	TCPIPLibs:sys.h.cdefs
o.files:	TCPIPLibs:sys.h.errno
o.files:	C:h.kernel
o.files:	TCPIPLibs:net.h.if
o.files:	TCPIPLibs:sys.h.socket
o.files:	TCPIPLibs:sys.h.time
o.files:	TCPIPLibs:sys.h.cdefs
o.files:	TCPIPLibs:net.h.if_arp
o.files:	TCPIPLibs:sys.h.socket
o.files:	TCPIPLibs:netinet.h.in
o.files:	TCPIPLibs:sys.h.ioctl
o.files:	TCPIPLibs:sys.h.ttycom
o.files:	TCPIPLibs:sys.h.ioccom
o.files:	TCPIPLibs:sys.h.ioccom
o.files:	TCPIPLibs:sys.h.filio
o.files:	TCPIPLibs:sys.h.ioccom
o.files:	TCPIPLibs:sys.h.sockio
o.files:	TCPIPLibs:sys.h.ioccom
o.files:	TCPIPLibs:sys.h.cdefs
o.files:	TCPIPLibs:h.netdb
o.files:	TCPIPLibs:sys.h.cdefs
o.files:	TCPIPLibs:netns.h.ns
o.files:	TCPIPLibs:sys.h.cdefs
o.files:	TCPIPLibs:sys.h.time
o.files:	TCPIPLibs:h.socklib
o.files:	TCPIPLibs:sys.h.types
o.files:	TCPIPLibs:sys.h.socket
o.files:	TCPIPLibs:sys.h.stat
o.files:	TCPIPLibs:sys.h.time
o.files:	TCPIPLibs:sys.h.cdefs
o.files:	TCPIPLibs:sys.h.time
o.files:	TCPIPLibs:sys.h.uio
o.files:	TCPIPLibs:sys.h.cdefs
o.files:	C:h.kernel
o.files:	TCPIPLibs:h.inetlib
o.files:	TCPIPLibs:sys.h.types
o.files:	TCPIPLibs:netinet.h.in
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
o.misc:	TCPIPLibs:sys.h.types
o.misc:	TCPIPLibs:sys.h.cdefs
o.misc:	TCPIPLibs:machine.h.endian
o.misc:	TCPIPLibs:machine.h.ansi
o.misc:	TCPIPLibs:machine.h.types
o.misc:	TCPIPLibs:sys.h.socket
o.misc:	TCPIPLibs:sys.h.cdefs
o.misc:	TCPIPLibs:sys.h.errno
o.misc:	C:h.kernel
o.misc:	TCPIPLibs:net.h.if
o.misc:	TCPIPLibs:sys.h.socket
o.misc:	TCPIPLibs:sys.h.time
o.misc:	TCPIPLibs:sys.h.cdefs
o.misc:	TCPIPLibs:net.h.if_arp
o.misc:	TCPIPLibs:sys.h.socket
o.misc:	TCPIPLibs:netinet.h.in
o.misc:	TCPIPLibs:sys.h.ioctl
o.misc:	TCPIPLibs:sys.h.ttycom
o.misc:	TCPIPLibs:sys.h.ioccom
o.misc:	TCPIPLibs:sys.h.ioccom
o.misc:	TCPIPLibs:sys.h.filio
o.misc:	TCPIPLibs:sys.h.ioccom
o.misc:	TCPIPLibs:sys.h.sockio
o.misc:	TCPIPLibs:sys.h.ioccom
o.misc:	TCPIPLibs:sys.h.cdefs
o.misc:	TCPIPLibs:h.netdb
o.misc:	TCPIPLibs:sys.h.cdefs
o.misc:	TCPIPLibs:netns.h.ns
o.misc:	TCPIPLibs:sys.h.cdefs
o.misc:	TCPIPLibs:sys.h.time
o.misc:	TCPIPLibs:h.socklib
o.misc:	TCPIPLibs:sys.h.types
o.misc:	TCPIPLibs:sys.h.socket
o.misc:	TCPIPLibs:sys.h.stat
o.misc:	TCPIPLibs:sys.h.time
o.misc:	TCPIPLibs:sys.h.cdefs
o.misc:	TCPIPLibs:sys.h.time
o.misc:	TCPIPLibs:sys.h.uio
o.misc:	TCPIPLibs:sys.h.cdefs
o.misc:	C:h.kernel
o.misc:	TCPIPLibs:h.inetlib
o.misc:	TCPIPLibs:sys.h.types
o.misc:	TCPIPLibs:netinet.h.in
o.misc:	TCPIPLibs:netinet.h.tcp_timer
o.misc:	C:h.kernel
o.misc:	C:h.swis
o.misc:	h.files
o.misc:	h.q
o.misc:	h.status
o.misc:	h.webftp
o.misc:	h.pads
o.misc:	h.privwebftp
o.misc:	h.autofns
o.misc:	h.csfs
o.db:	c.db
o.db:	h.all
o.db:	h.tcplibs
o.db:	TCPIPLibs:sys.h.types
o.db:	TCPIPLibs:sys.h.cdefs
o.db:	TCPIPLibs:machine.h.endian
o.db:	TCPIPLibs:machine.h.ansi
o.db:	TCPIPLibs:machine.h.types
o.db:	TCPIPLibs:sys.h.socket
o.db:	TCPIPLibs:sys.h.cdefs
o.db:	TCPIPLibs:sys.h.errno
o.db:	C:h.kernel
o.db:	TCPIPLibs:net.h.if
o.db:	TCPIPLibs:sys.h.socket
o.db:	TCPIPLibs:sys.h.time
o.db:	TCPIPLibs:sys.h.cdefs
o.db:	TCPIPLibs:net.h.if_arp
o.db:	TCPIPLibs:sys.h.socket
o.db:	TCPIPLibs:netinet.h.in
o.db:	TCPIPLibs:sys.h.ioctl
o.db:	TCPIPLibs:sys.h.ttycom
o.db:	TCPIPLibs:sys.h.ioccom
o.db:	TCPIPLibs:sys.h.ioccom
o.db:	TCPIPLibs:sys.h.filio
o.db:	TCPIPLibs:sys.h.ioccom
o.db:	TCPIPLibs:sys.h.sockio
o.db:	TCPIPLibs:sys.h.ioccom
o.db:	TCPIPLibs:sys.h.cdefs
o.db:	TCPIPLibs:h.netdb
o.db:	TCPIPLibs:sys.h.cdefs
o.db:	TCPIPLibs:netns.h.ns
o.db:	TCPIPLibs:sys.h.cdefs
o.db:	TCPIPLibs:sys.h.time
o.db:	TCPIPLibs:h.socklib
o.db:	TCPIPLibs:sys.h.types
o.db:	TCPIPLibs:sys.h.socket
o.db:	TCPIPLibs:sys.h.stat
o.db:	TCPIPLibs:sys.h.time
o.db:	TCPIPLibs:sys.h.cdefs
o.db:	TCPIPLibs:sys.h.time
o.db:	TCPIPLibs:sys.h.uio
o.db:	TCPIPLibs:sys.h.cdefs
o.db:	C:h.kernel
o.db:	TCPIPLibs:h.inetlib
o.db:	TCPIPLibs:sys.h.types
o.db:	TCPIPLibs:netinet.h.in
o.db:	TCPIPLibs:netinet.h.tcp_timer
o.db:	C:h.kernel
o.db:	C:h.swis
o.db:	h.files
o.db:	h.q
o.db:	h.status
o.db:	h.webftp
o.db:	h.pads
o.db:	h.privwebftp
o.db:	h.autofns
o.db:	h.csfs
o.dirparse:	c.dirparse
o.dirparse:	h.all
o.dirparse:	h.tcplibs
o.dirparse:	TCPIPLibs:sys.h.types
o.dirparse:	TCPIPLibs:sys.h.cdefs
o.dirparse:	TCPIPLibs:machine.h.endian
o.dirparse:	TCPIPLibs:machine.h.ansi
o.dirparse:	TCPIPLibs:machine.h.types
o.dirparse:	TCPIPLibs:sys.h.socket
o.dirparse:	TCPIPLibs:sys.h.cdefs
o.dirparse:	TCPIPLibs:sys.h.errno
o.dirparse:	C:h.kernel
o.dirparse:	TCPIPLibs:net.h.if
o.dirparse:	TCPIPLibs:sys.h.socket
o.dirparse:	TCPIPLibs:sys.h.time
o.dirparse:	TCPIPLibs:sys.h.cdefs
o.dirparse:	TCPIPLibs:net.h.if_arp
o.dirparse:	TCPIPLibs:sys.h.socket
o.dirparse:	TCPIPLibs:netinet.h.in
o.dirparse:	TCPIPLibs:sys.h.ioctl
o.dirparse:	TCPIPLibs:sys.h.ttycom
o.dirparse:	TCPIPLibs:sys.h.ioccom
o.dirparse:	TCPIPLibs:sys.h.ioccom
o.dirparse:	TCPIPLibs:sys.h.filio
o.dirparse:	TCPIPLibs:sys.h.ioccom
o.dirparse:	TCPIPLibs:sys.h.sockio
o.dirparse:	TCPIPLibs:sys.h.ioccom
o.dirparse:	TCPIPLibs:sys.h.cdefs
o.dirparse:	TCPIPLibs:h.netdb
o.dirparse:	TCPIPLibs:sys.h.cdefs
o.dirparse:	TCPIPLibs:netns.h.ns
o.dirparse:	TCPIPLibs:sys.h.cdefs
o.dirparse:	TCPIPLibs:sys.h.time
o.dirparse:	TCPIPLibs:h.socklib
o.dirparse:	TCPIPLibs:sys.h.types
o.dirparse:	TCPIPLibs:sys.h.socket
o.dirparse:	TCPIPLibs:sys.h.stat
o.dirparse:	TCPIPLibs:sys.h.time
o.dirparse:	TCPIPLibs:sys.h.cdefs
o.dirparse:	TCPIPLibs:sys.h.time
o.dirparse:	TCPIPLibs:sys.h.uio
o.dirparse:	TCPIPLibs:sys.h.cdefs
o.dirparse:	C:h.kernel
o.dirparse:	TCPIPLibs:h.inetlib
o.dirparse:	TCPIPLibs:sys.h.types
o.dirparse:	TCPIPLibs:netinet.h.in
o.dirparse:	TCPIPLibs:netinet.h.tcp_timer
o.dirparse:	C:h.kernel
o.dirparse:	C:h.swis
o.dirparse:	h.files
o.dirparse:	h.q
o.dirparse:	h.status
o.dirparse:	h.webftp
o.dirparse:	h.pads
o.dirparse:	h.privwebftp
o.dirparse:	h.autofns
o.dirparse:	h.csfs
