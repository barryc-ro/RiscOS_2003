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
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS}
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
INETLIB=InetInc:^.^.libs.inetlib.inetlibzm
SOCKLIB=InetInc:^.^.libs.socklib.socklibzm
#
# Include files
#
INCLUDES = -IInetInc:,InetInc:^.^.libs.inetlib,InetInc:^.^.libs.socklib,InetInc:^.^.libs.unixlib,C:

DFLAGS = -DVERBOSEDEBUG=0 -DRISCOS -DDEBUG=0 -DPROG_IDENT="\"NCFresco\""

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
o.webftp:	InetInc:h.errno
o.webftp:	C:h.kernel
o.webftp:	h.inetlib
o.webftp:	InetInc:sys.h.types
o.webftp:	InetInc:netinet.h.in
o.webftp:	h.socklib
o.webftp:	InetInc:sys.h.types
o.webftp:	InetInc:sys.h.socket
o.webftp:	InetInc:sys.h.stat
o.webftp:	InetInc:sys.h.time
o.webftp:	InetInc:sys.h.uio
o.webftp:	C:h.kernel
o.webftp:	InetInc:sys.h.ioctl
o.webftp:	InetInc:sys.h.ttychars
o.webftp:	InetInc:sys.h.ttydev
o.webftp:	InetInc:h.netdb
o.webftp:	h.unixlib
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
o.q:	InetInc:h.errno
o.q:	C:h.kernel
o.q:	h.inetlib
o.q:	InetInc:sys.h.types
o.q:	InetInc:netinet.h.in
o.q:	h.socklib
o.q:	InetInc:sys.h.types
o.q:	InetInc:sys.h.socket
o.q:	InetInc:sys.h.stat
o.q:	InetInc:sys.h.time
o.q:	InetInc:sys.h.uio
o.q:	C:h.kernel
o.q:	InetInc:sys.h.ioctl
o.q:	InetInc:sys.h.ttychars
o.q:	InetInc:sys.h.ttydev
o.q:	InetInc:h.netdb
o.q:	h.unixlib
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
o.files:	h.inetlib
o.files:	InetInc:sys.h.types
o.files:	InetInc:netinet.h.in
o.files:	h.socklib
o.files:	InetInc:sys.h.types
o.files:	InetInc:sys.h.socket
o.files:	InetInc:sys.h.stat
o.files:	InetInc:sys.h.time
o.files:	InetInc:sys.h.time
o.files:	InetInc:sys.h.uio
o.files:	C:h.kernel
o.files:	InetInc:h.netdb
o.files:	h.unixlib
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
o.support: hdr.SwiNames
o.support: hdr.RegNames
o.misc:	c.misc
o.misc:	h.all
o.misc:	InetInc:h.errno
o.misc:	C:h.kernel
o.misc:	h.inetlib
o.misc:	InetInc:sys.h.types
o.misc:	InetInc:netinet.h.in
o.misc:	h.socklib
o.misc:	InetInc:sys.h.types
o.misc:	InetInc:sys.h.socket
o.misc:	InetInc:sys.h.stat
o.misc:	InetInc:sys.h.time
o.misc:	InetInc:sys.h.uio
o.misc:	C:h.kernel
o.misc:	InetInc:sys.h.ioctl
o.misc:	InetInc:sys.h.ttychars
o.misc:	InetInc:sys.h.ttydev
o.misc:	InetInc:h.netdb
o.misc:	h.unixlib
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
o.dirparse:	InetInc:h.errno
o.dirparse:	C:h.kernel
o.dirparse:	h.inetlib
o.dirparse:	InetInc:sys.h.types
o.dirparse:	InetInc:netinet.h.in
o.dirparse:	h.socklib
o.dirparse:	InetInc:sys.h.types
o.dirparse:	InetInc:sys.h.socket
o.dirparse:	InetInc:sys.h.stat
o.dirparse:	InetInc:sys.h.time
o.dirparse:	InetInc:sys.h.uio
o.dirparse:	C:h.kernel
o.dirparse:	InetInc:sys.h.ioctl
o.dirparse:	InetInc:sys.h.ttychars
o.dirparse:	InetInc:sys.h.ttydev
o.dirparse:	InetInc:h.netdb
o.dirparse:	h.unixlib
o.dirparse:	C:h.kernel
o.dirparse:	C:h.swis
o.dirparse:	h.files
o.dirparse:	h.q
o.dirparse:	h.status
o.dirparse:	h.webftp
o.dirparse:	h.pads
o.dirparse:	h.privwebftp
o.dirparse:	h.autofns
