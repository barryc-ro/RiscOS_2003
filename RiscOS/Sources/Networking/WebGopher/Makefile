# Makefile for webgopher
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
CFLAGS  = -c -depend !Depend -zM ${INCLUDES} ${DFLAGS} -wp
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
INETLIB=TCPIPLibs:o.inetlibzm
SOCKLIB=TCPIPLibs:o.socklibzm
#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:

DFLAGS   = -DCOMPAT_INET4

#
# Program specific options:
#
COMPONENT = webgopher
TARGET    = aof.${COMPONENT}
OBJS      = modhead.o files.o gopher.o
EXPORTS   = h.gopherpub

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
	${CP} h.gopherpub ${EXP_HDR}.h.webgopher ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

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
o.files:	c.files
o.files:	C:h.kernel
o.files:	h.files
o.gopher:	c.gopher
o.gopher:	C:h.kernel
o.gopher:	C:h.swis
o.gopher:	h.tcplibs
o.gopher:	TCPIPLibs:sys.h.types
o.gopher:	TCPIPLibs:sys.h.cdefs
o.gopher:	TCPIPLibs:machine.h.endian
o.gopher:	TCPIPLibs:machine.h.ansi
o.gopher:	TCPIPLibs:machine.h.types
o.gopher:	TCPIPLibs:sys.h.socket
o.gopher:	TCPIPLibs:sys.h.cdefs
o.gopher:	TCPIPLibs:sys.h.errno
o.gopher:	C:h.kernel
o.gopher:	TCPIPLibs:net.h.if
o.gopher:	TCPIPLibs:sys.h.socket
o.gopher:	TCPIPLibs:sys.h.time
o.gopher:	TCPIPLibs:sys.h.cdefs
o.gopher:	TCPIPLibs:net.h.if_arp
o.gopher:	TCPIPLibs:sys.h.socket
o.gopher:	TCPIPLibs:netinet.h.in
o.gopher:	TCPIPLibs:sys.h.ioctl
o.gopher:	TCPIPLibs:sys.h.ttycom
o.gopher:	TCPIPLibs:sys.h.ioccom
o.gopher:	TCPIPLibs:sys.h.ioccom
o.gopher:	TCPIPLibs:sys.h.filio
o.gopher:	TCPIPLibs:sys.h.ioccom
o.gopher:	TCPIPLibs:sys.h.sockio
o.gopher:	TCPIPLibs:sys.h.ioccom
o.gopher:	TCPIPLibs:sys.h.cdefs
o.gopher:	TCPIPLibs:h.netdb
o.gopher:	TCPIPLibs:sys.h.cdefs
o.gopher:	TCPIPLibs:netns.h.ns
o.gopher:	TCPIPLibs:sys.h.cdefs
o.gopher:	TCPIPLibs:sys.h.time
o.gopher:	TCPIPLibs:h.socklib
o.gopher:	TCPIPLibs:sys.h.types
o.gopher:	TCPIPLibs:sys.h.socket
o.gopher:	TCPIPLibs:sys.h.stat
o.gopher:	TCPIPLibs:sys.h.time
o.gopher:	TCPIPLibs:sys.h.cdefs
o.gopher:	TCPIPLibs:sys.h.time
o.gopher:	TCPIPLibs:sys.h.uio
o.gopher:	TCPIPLibs:sys.h.cdefs
o.gopher:	C:h.kernel
o.gopher:	h.status
o.gopher:	h.gopherpub
o.gopher:	h.pads
o.gopher:	h.gopher
o.gopher:	h.files
