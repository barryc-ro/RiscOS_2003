# Makefile for http
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
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS} -DCOMPAT_INET4
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

INSECURELIBS= ${INETLIB} ${SOCKLIB} 
SECURELIBS= ssleay.o.sslzm ssleay.o.cryptozm ${UNIXLIB} ${INSECURELIBS}
#
# Include files
#
INCLUDES  = -ITCPIPLibs:,C:,ssleay

#
# Program specific options:
#
COMPONENT = http
TARGET    = aof.${COMPONENT}
OBJS      = mimemap.o files.o http.o modhead.o
SECUREOBJS= os.mimemap os.files os.http modheadS.o
EXPORTS   = h.httppub

.SUFFIXES:   .os

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DNO_SECURE
.c.os:;     ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}mod rm.${COMPONENT}smod 

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.httppub ${EXP_HDR}.h.http ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT}mod rm.${COMPONENT}smod 
	${CP} rm.${COMPONENT}mod ${INSTDIR}.${COMPONENT}mod ${CPFLAGS}
	${CP} rm.${COMPONENT}smod ${INSTDIR}.${COMPONENT}smod ${CPFLAGS}
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
${TARGET}: ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}mod: ${OBJS} ${CLIB} ${INSECURELIBS}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${INSECURELIBS}

rm.${COMPONENT}smod: ${SECUREOBJS} ${CLIB} ${SECURELIBS}
        ${LD} -o $@ -module ${SECUREOBJS} ${CLIB} ${SECURELIBS}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.mimemap:	c.mimemap
o.mimemap:	C:h.kernel
o.mimemap:	h.mimemap
o.files:	c.files
o.files:	C:h.kernel
o.files:	h.files
o.http:	c.http
o.http:	C:h.kernel
o.http:	C:h.swis
o.http:	h.tcplibs
o.http:	TCPIPLibs:sys.h.types
o.http:	TCPIPLibs:sys.h.cdefs
o.http:	TCPIPLibs:machine.h.endian
o.http:	TCPIPLibs:machine.h.ansi
o.http:	TCPIPLibs:machine.h.types
o.http:	TCPIPLibs:sys.h.socket
o.http:	TCPIPLibs:sys.h.cdefs
o.http:	TCPIPLibs:h.errno
o.http:	TCPIPLibs:sys.h.errno
o.http:	C:h.kernel
o.http:	TCPIPLibs:net.h.if
o.http:	TCPIPLibs:sys.h.socket
o.http:	TCPIPLibs:sys.h.time
o.http:	TCPIPLibs:sys.h.cdefs
o.http:	TCPIPLibs:net.h.if_arp
o.http:	TCPIPLibs:sys.h.socket
o.http:	TCPIPLibs:netinet.h.in
o.http:	TCPIPLibs:sys.h.ioctl
o.http:	TCPIPLibs:sys.h.ttycom
o.http:	TCPIPLibs:sys.h.ioccom
o.http:	TCPIPLibs:sys.h.ioccom
o.http:	TCPIPLibs:sys.h.filio
o.http:	TCPIPLibs:sys.h.ioccom
o.http:	TCPIPLibs:sys.h.sockio
o.http:	TCPIPLibs:sys.h.ioccom
o.http:	TCPIPLibs:sys.h.cdefs
o.http:	TCPIPLibs:h.netdb
o.http:	TCPIPLibs:sys.h.cdefs
o.http:	TCPIPLibs:netns.h.ns
o.http:	TCPIPLibs:sys.h.cdefs
o.http:	TCPIPLibs:sys.h.time
o.http:	TCPIPLibs:h.socklib
o.http:	TCPIPLibs:sys.h.types
o.http:	TCPIPLibs:sys.h.socket
o.http:	TCPIPLibs:sys.h.stat
o.http:	TCPIPLibs:sys.h.time
o.http:	TCPIPLibs:sys.h.cdefs
o.http:	TCPIPLibs:sys.h.time
o.http:	TCPIPLibs:sys.h.uio
o.http:	TCPIPLibs:sys.h.cdefs
o.http:	C:h.kernel
o.http:	h.status
o.http:	h.httppub
o.http:	h.pads
o.http:	h.http
o.http:	h.files
o.http:	h.mimemap
