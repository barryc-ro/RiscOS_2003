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
CFLAGS  = -c -depend !Depend -zM -zps1 -ff -wp ${INCLUDES} ${DFLAGS} -DCOMPAT_INET4
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
SSLDIR    = <Lib$Dir>.SSL

INSECURELIBS= ${INETLIB} ${SOCKLIB}
SECURELIBS= $(SSLDIR).o.ssllib $(SSLDIR).o.cryptolib ${UNIXLIB} ${INSECURELIBS}
#
# Include files
#
INCLUDES  = -IC:,TCPIPLibs:,$(SSLDIR)

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
.c.os:;     ${CC} ${CFLAGS} -o $@ $< -DVERIFY_MD
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}mod rm.${COMPONENT}smod
#all: rm.${COMPONENT}mod

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
        ${WIPE} os.* ${WFLAGS}
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
${TARGET}: ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}
        ${LD} -o $@ -aof ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}

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
os.mimemap:	c.mimemap
os.mimemap:	C:h.kernel
os.mimemap:	h.mimemap
os.mimemap:	c.mimemap
os.mimemap:	C:h.kernel
os.mimemap:	h.mimemap
os.files:	c.files
os.files:	C:h.kernel
os.files:	h.files
os.http:	c.http
os.http:	C:h.kernel
os.http:	C:h.swis
os.http:	h.tcplibs
os.http:	TCPIPLibs:sys.h.types
os.http:	TCPIPLibs:sys.h.cdefs
os.http:	TCPIPLibs:machine.h.endian
os.http:	TCPIPLibs:machine.h.ansi
os.http:	TCPIPLibs:machine.h.types
os.http:	TCPIPLibs:sys.h.socket
os.http:	TCPIPLibs:sys.h.cdefs
os.http:	TCPIPLibs:sys.h.errno
os.http:	C:h.kernel
os.http:	TCPIPLibs:net.h.if
os.http:	TCPIPLibs:sys.h.socket
os.http:	TCPIPLibs:sys.h.time
os.http:	TCPIPLibs:sys.h.cdefs
os.http:	TCPIPLibs:net.h.if_arp
os.http:	TCPIPLibs:sys.h.socket
os.http:	TCPIPLibs:netinet.h.in
os.http:	TCPIPLibs:sys.h.ioctl
os.http:	TCPIPLibs:sys.h.ttycom
os.http:	TCPIPLibs:sys.h.ioccom
os.http:	TCPIPLibs:sys.h.ioccom
os.http:	TCPIPLibs:sys.h.filio
os.http:	TCPIPLibs:sys.h.ioccom
os.http:	TCPIPLibs:sys.h.sockio
os.http:	TCPIPLibs:sys.h.ioccom
os.http:	TCPIPLibs:sys.h.cdefs
os.http:	TCPIPLibs:h.netdb
os.http:	TCPIPLibs:sys.h.cdefs
os.http:	TCPIPLibs:netns.h.ns
os.http:	TCPIPLibs:sys.h.cdefs
os.http:	TCPIPLibs:sys.h.time
os.http:	TCPIPLibs:h.socklib
os.http:	TCPIPLibs:sys.h.types
os.http:	TCPIPLibs:sys.h.socket
os.http:	TCPIPLibs:sys.h.stat
os.http:	TCPIPLibs:sys.h.time
os.http:	TCPIPLibs:sys.h.cdefs
os.http:	TCPIPLibs:sys.h.time
os.http:	TCPIPLibs:sys.h.uio
os.http:	TCPIPLibs:sys.h.cdefs
os.http:	C:h.kernel
os.http:	h.status
os.http:	<Lib$Dir>.SSL.h.objects
os.http:	<Lib$Dir>.SSL.h.asn1
os.http:	<Lib$Dir>.SSL.h.bn
os.http:	<Lib$Dir>.SSL.h.stack
os.http:	<Lib$Dir>.SSL.h.pem
os.http:	<Lib$Dir>.SSL.h.rsa
os.http:	<Lib$Dir>.SSL.h.bn
os.http:	<Lib$Dir>.SSL.h.dsa
os.http:	<Lib$Dir>.SSL.h.bn
os.http:	<Lib$Dir>.SSL.h.x509
os.http:	<Lib$Dir>.SSL.h.stack
os.http:	<Lib$Dir>.SSL.h.asn1
os.http:	<Lib$Dir>.SSL.h.envelope
os.http:	<Lib$Dir>.SSL.h.md2
os.http:	<Lib$Dir>.SSL.h.md5
os.http:	<Lib$Dir>.SSL.h.sha
os.http:	<Lib$Dir>.SSL.h.des
os.http:	<Lib$Dir>.SSL.h.rc4
os.http:	<Lib$Dir>.SSL.h.rc2
os.http:	<Lib$Dir>.SSL.h.idea
os.http:	<Lib$Dir>.SSL.h.rsa
os.http:	<Lib$Dir>.SSL.h.dsa
os.http:	<Lib$Dir>.SSL.h.dh
os.http:	<Lib$Dir>.SSL.h.envelope
os.http:	<Lib$Dir>.SSL.h.ssl
os.http:	h.httppub
os.http:	h.pads
os.http:	h.http
os.http:	h.files
os.http:	h.mimemap
