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
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -zps1 -ff -wp ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
INETLIB   = C:TCPIPLibs.o.inetlibzm
SOCKLIB   = C:TCPIPLibs.o.socklibzm
UNIXLIB   = C:TCPIPLibs.o.unixlibzm
SSLDIR    = C:SSL
REMOTEDB  = C:debug.o.lremotezm

INSECURELIBS= ${INETLIB} ${SOCKLIB}
SECURELIBS= $(SSLDIR).o.ssllib $(SSLDIR).o.cryptolib ${UNIXLIB} ${INSECURELIBS}
#
# Include files
#
INCLUDES  = -IC:,C:TCPIPLibs.,${SSLDIR}.
DFLAGS =  ${DB} -DCOMPAT_INET4 -DUSING_INET5 -DUSE_MIMEMAPMOD=0 -DPROXY_REDIRECT=0 -DVERIFY_MD
#
# Program specific options:
#
COMPONENT = http
TARGET    = aof.${COMPONENT}
OBJS      = mimemap.o files.o http.o ModHead.o
SECUREOBJS= os.mimemap os.files os.http os.ModHead os.predirect
EXPORTS   = h.httppub h.pads

.SUFFIXES:   .os

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $< -DNO_SECURE
.c.os:;     ${CC} ${CFLAGS} -o $@ $< 
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< -DNO_SECURE
.cmhg.os:;  ${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}mod rm.${COMPONENT}smod

#
# RISC OS ROM build rules:
#
rom:	rom_${TYPE}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.httppub ${EXP_HDR}.h.http ${CPFLAGS}
        ${CP} h.pads ${EXP_HDR}.h.pads ${CPFLAGS}
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

rom_secure: ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}
        ${LD} -o ${TARGET} -aof ${SECUREOBJS} ${ROMCSTUBS} ${SECURELIBS}

rom_ rom_insecure: ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}
        ${LD} -o ${TARGET} -aof ${OBJS} ${ROMCSTUBS} ${INSECURELIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}mod: ${OBJS} ${REMOTEDB} ${CLIB} ${INSECURELIBS}
        ${LD} -o $@ -module ${OBJS} ${REMOTEDB} ${CLIB} ${INSECURELIBS}
        Access $@ RW/R

rm.${COMPONENT}smod: ${SECUREOBJS} ${REMOTEDB} ${CLIB} ${SECURELIBS}
        ${LD} -o $@ -module ${SECUREOBJS} ${REMOTEDB} ${CLIB} ${SECURELIBS}
        Access $@ RW/R

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
o.http:	C:TCPIPLibs.sys.h.types
o.http:	C:TCPIPLibs.sys.h.cdefs
o.http:	C:TCPIPLibs.machine.h.endian
o.http:	C:TCPIPLibs.machine.h.ansi
o.http:	C:TCPIPLibs.machine.h.types
o.http:	C:TCPIPLibs.sys.h.socket
o.http:	C:TCPIPLibs.sys.h.cdefs
o.http:	C:TCPIPLibs.sys.h.errno
o.http:	C:h.kernel
o.http:	C:TCPIPLibs.net.h.if
o.http:	C:TCPIPLibs.sys.h.socket
o.http:	C:TCPIPLibs.sys.h.time
o.http:	C:TCPIPLibs.sys.h.cdefs
o.http:	C:TCPIPLibs.net.h.if_arp
o.http:	C:TCPIPLibs.sys.h.socket
o.http:	C:TCPIPLibs.netinet.h.in
o.http:	C:TCPIPLibs.sys.h.ioctl
o.http:	C:TCPIPLibs.sys.h.ttycom
o.http:	C:TCPIPLibs.sys.h.ioccom
o.http:	C:TCPIPLibs.sys.h.ioccom
o.http:	C:TCPIPLibs.sys.h.filio
o.http:	C:TCPIPLibs.sys.h.ioccom
o.http:	C:TCPIPLibs.sys.h.sockio
o.http:	C:TCPIPLibs.sys.h.ioccom
o.http:	C:TCPIPLibs.sys.h.cdefs
o.http:	C:TCPIPLibs.h.netdb
o.http:	C:TCPIPLibs.sys.h.cdefs
o.http:	C:TCPIPLibs.netns.h.ns
o.http:	C:TCPIPLibs.sys.h.cdefs
o.http:	C:TCPIPLibs.sys.h.time
o.http:	C:TCPIPLibs.h.socklib
o.http:	C:TCPIPLibs.sys.h.types
o.http:	C:TCPIPLibs.sys.h.socket
o.http:	C:TCPIPLibs.sys.h.stat
o.http:	C:TCPIPLibs.sys.h.time
o.http:	C:TCPIPLibs.sys.h.cdefs
o.http:	C:TCPIPLibs.sys.h.time
o.http:	C:TCPIPLibs.sys.h.uio
o.http:	C:TCPIPLibs.sys.h.cdefs
o.http:	C:h.kernel
o.http:	C:TCPIPLibs.h.inetlib
o.http:	C:TCPIPLibs.sys.h.types
o.http:	C:TCPIPLibs.netinet.h.in
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
os.http:	C:TCPIPLibs.sys.h.types
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	C:TCPIPLibs.machine.h.endian
os.http:	C:TCPIPLibs.machine.h.ansi
os.http:	C:TCPIPLibs.machine.h.types
os.http:	C:TCPIPLibs.sys.h.socket
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	C:TCPIPLibs.sys.h.errno
os.http:	C:h.kernel
os.http:	C:TCPIPLibs.net.h.if
os.http:	C:TCPIPLibs.sys.h.socket
os.http:	C:TCPIPLibs.sys.h.time
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	C:TCPIPLibs.net.h.if_arp
os.http:	C:TCPIPLibs.sys.h.socket
os.http:	C:TCPIPLibs.netinet.h.in
os.http:	C:TCPIPLibs.sys.h.ioctl
os.http:	C:TCPIPLibs.sys.h.ttycom
os.http:	C:TCPIPLibs.sys.h.ioccom
os.http:	C:TCPIPLibs.sys.h.ioccom
os.http:	C:TCPIPLibs.sys.h.filio
os.http:	C:TCPIPLibs.sys.h.ioccom
os.http:	C:TCPIPLibs.sys.h.sockio
os.http:	C:TCPIPLibs.sys.h.ioccom
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	C:TCPIPLibs.h.netdb
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	C:TCPIPLibs.netns.h.ns
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	C:TCPIPLibs.sys.h.time
os.http:	C:TCPIPLibs.h.socklib
os.http:	C:TCPIPLibs.sys.h.types
os.http:	C:TCPIPLibs.sys.h.socket
os.http:	C:TCPIPLibs.sys.h.stat
os.http:	C:TCPIPLibs.sys.h.time
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	C:TCPIPLibs.sys.h.time
os.http:	C:TCPIPLibs.sys.h.uio
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	C:h.kernel
os.http:	C:TCPIPLibs.h.inetlib
os.http:	C:TCPIPLibs.sys.h.types
os.http:	C:TCPIPLibs.netinet.h.in
os.http:	h.status
os.http:	C:SSL.h.objects
os.http:	C:SSL.h.asn1
os.http:	C:SSL.h.bn
os.http:	C:SSL.h.stack
os.http:	C:SSL.h.pem
os.http:	C:SSL.h.rsa
os.http:	C:SSL.h.bn
os.http:	C:SSL.h.dsa
os.http:	C:SSL.h.bn
os.http:	C:SSL.h.x509
os.http:	C:SSL.h.stack
os.http:	C:SSL.h.asn1
os.http:	C:SSL.h.envelope
os.http:	C:SSL.h.md2
os.http:	C:SSL.h.md5
os.http:	C:SSL.h.sha
os.http:	C:SSL.h.des
os.http:	C:SSL.h.rc4
os.http:	C:SSL.h.rc2
os.http:	C:SSL.h.idea
os.http:	C:SSL.h.rsa
os.http:	C:SSL.h.dsa
os.http:	C:SSL.h.dh
os.http:	C:SSL.h.envelope
os.http:	C:SSL.h.rsa
os.http:	C:SSL.h.ssl
os.http:	C:SSL.h.x509
os.http:	h.httppub
os.http:	h.pads
os.http:	h.http
os.http:	h.files
os.http:	h.mimemap
os.predirect:	c.predirect
os.predirect:	C:SSL.h.objects
os.predirect:	C:SSL.h.asn1
os.predirect:	C:SSL.h.bn
os.predirect:	C:SSL.h.stack
os.predirect:	C:SSL.h.pem
os.predirect:	C:SSL.h.rsa
os.predirect:	C:SSL.h.bn
os.predirect:	C:SSL.h.dsa
os.predirect:	C:SSL.h.bn
os.predirect:	C:SSL.h.x509
os.predirect:	C:SSL.h.stack
os.predirect:	C:SSL.h.asn1
os.predirect:	C:SSL.h.envelope
os.predirect:	C:SSL.h.md2
os.predirect:	C:SSL.h.md5
os.predirect:	C:SSL.h.sha
os.predirect:	C:SSL.h.des
os.predirect:	C:SSL.h.rc4
os.predirect:	C:SSL.h.rc2
os.predirect:	C:SSL.h.idea
os.predirect:	C:SSL.h.rsa
os.predirect:	C:SSL.h.dsa
os.predirect:	C:SSL.h.dh
os.predirect:	C:SSL.h.envelope
os.predirect:	C:SSL.h.rsa
os.predirect:	C:SSL.h.ssl
os.predirect:	C:SSL.h.x509
os.predirect:	C:h.kernel
os.predirect:	C:h.swis
os.predirect:	h.tcplibs
os.predirect:	C:TCPIPLibs.sys.h.types
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	C:TCPIPLibs.machine.h.endian
os.predirect:	C:TCPIPLibs.machine.h.ansi
os.predirect:	C:TCPIPLibs.machine.h.types
os.predirect:	C:TCPIPLibs.sys.h.socket
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	C:TCPIPLibs.sys.h.errno
os.predirect:	C:h.kernel
os.predirect:	C:TCPIPLibs.net.h.if
os.predirect:	C:TCPIPLibs.sys.h.socket
os.predirect:	C:TCPIPLibs.sys.h.time
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	C:TCPIPLibs.net.h.if_arp
os.predirect:	C:TCPIPLibs.sys.h.socket
os.predirect:	C:TCPIPLibs.netinet.h.in
os.predirect:	C:TCPIPLibs.sys.h.ioctl
os.predirect:	C:TCPIPLibs.sys.h.ttycom
os.predirect:	C:TCPIPLibs.sys.h.ioccom
os.predirect:	C:TCPIPLibs.sys.h.ioccom
os.predirect:	C:TCPIPLibs.sys.h.filio
os.predirect:	C:TCPIPLibs.sys.h.ioccom
os.predirect:	C:TCPIPLibs.sys.h.sockio
os.predirect:	C:TCPIPLibs.sys.h.ioccom
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	C:TCPIPLibs.h.netdb
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	C:TCPIPLibs.netns.h.ns
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	C:TCPIPLibs.sys.h.time
os.predirect:	C:TCPIPLibs.h.socklib
os.predirect:	C:TCPIPLibs.sys.h.types
os.predirect:	C:TCPIPLibs.sys.h.socket
os.predirect:	C:TCPIPLibs.sys.h.stat
os.predirect:	C:TCPIPLibs.sys.h.time
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	C:TCPIPLibs.sys.h.time
os.predirect:	C:TCPIPLibs.sys.h.uio
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	C:h.kernel
os.predirect:	C:TCPIPLibs.h.inetlib
os.predirect:	C:TCPIPLibs.sys.h.types
os.predirect:	C:TCPIPLibs.netinet.h.in
os.predirect:	h.status
os.predirect:	h.httppub
os.predirect:	h.pads
os.predirect:	h.http
os.predirect:	h.files
os.predirect:	h.mimemap
