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
LD      = link -v -map
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
SSLDIR    = C:SSLeay
REMOTEDB  = C:debug.o.lremotezm

INSECURELIBS= ${INETLIB} ${SOCKLIB}
SECURELIBS= $(SSLDIR).o.ssl $(SSLDIR).o.libcrypto ${UNIXLIB} ${INSECURELIBS}
#
# Include files
#
INCLUDES  = -IC:,C:TCPIPLibs.,${SSLDIR}.
DFLAGS =  ${DB} -DCOMPAT_INET4 -DFLAVOUR_TRANSPORT_MODULES=0 -DHTTPMOD -DRISCOS -DACORN_BUILD
#
# Program specific options:
#
COMPONENT = http
TARGET    = aof.${COMPONENT}
OBJS      = ModHead.o mimemap.o files.o file.o http.o module.o csfs.o certs.o predirect.o layershttp.o
SECUREOBJS= os.ModHead os.mimemap os.files os.file os.http os.module os.csfs os.certs os.predirect os.layershttp
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
#all: rm.${COMPONENT}mod
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
o.files:	c.files
o.files:	C:h.kernel
o.files:	h.types
o.files:	h.files
o.file:	c.file
o.file:	C:h.swis
o.file:	C:h.kernel
o.file:	C:h.kernel
o.file:	h.types
o.file:	h.file
o.http:	c.http
o.http:	h.http
o.http:	C:h.swis
o.http:	C:h.kernel
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
o.http:	h.files
o.http:	h.file
o.http:	h.notlayer
o.http:	h.mimemap
o.http:	h.status
o.http:	h.httppub
o.http:	h.pads
o.http:	h.platforms
o.module:	c.module
o.module:	h.http
o.module:	C:h.swis
o.module:	C:h.kernel
o.module:	C:TCPIPLibs.sys.h.types
o.module:	C:TCPIPLibs.sys.h.cdefs
o.module:	C:TCPIPLibs.machine.h.endian
o.module:	C:TCPIPLibs.machine.h.ansi
o.module:	C:TCPIPLibs.machine.h.types
o.module:	C:TCPIPLibs.sys.h.socket
o.module:	C:TCPIPLibs.sys.h.cdefs
o.module:	C:TCPIPLibs.sys.h.errno
o.module:	C:h.kernel
o.module:	C:TCPIPLibs.net.h.if
o.module:	C:TCPIPLibs.sys.h.socket
o.module:	C:TCPIPLibs.sys.h.time
o.module:	C:TCPIPLibs.sys.h.cdefs
o.module:	C:TCPIPLibs.net.h.if_arp
o.module:	C:TCPIPLibs.sys.h.socket
o.module:	C:TCPIPLibs.netinet.h.in
o.module:	C:TCPIPLibs.sys.h.ioctl
o.module:	C:TCPIPLibs.sys.h.ttycom
o.module:	C:TCPIPLibs.sys.h.ioccom
o.module:	C:TCPIPLibs.sys.h.ioccom
o.module:	C:TCPIPLibs.sys.h.filio
o.module:	C:TCPIPLibs.sys.h.ioccom
o.module:	C:TCPIPLibs.sys.h.sockio
o.module:	C:TCPIPLibs.sys.h.ioccom
o.module:	C:TCPIPLibs.sys.h.cdefs
o.module:	C:TCPIPLibs.h.netdb
o.module:	C:TCPIPLibs.sys.h.cdefs
o.module:	C:TCPIPLibs.netns.h.ns
o.module:	C:TCPIPLibs.sys.h.cdefs
o.module:	C:TCPIPLibs.sys.h.time
o.module:	C:TCPIPLibs.h.socklib
o.module:	C:TCPIPLibs.sys.h.types
o.module:	C:TCPIPLibs.sys.h.socket
o.module:	C:TCPIPLibs.sys.h.stat
o.module:	C:TCPIPLibs.sys.h.time
o.module:	C:TCPIPLibs.sys.h.cdefs
o.module:	C:TCPIPLibs.sys.h.time
o.module:	C:TCPIPLibs.sys.h.uio
o.module:	C:TCPIPLibs.sys.h.cdefs
o.module:	C:h.kernel
o.module:	C:TCPIPLibs.h.inetlib
o.module:	C:TCPIPLibs.sys.h.types
o.module:	C:TCPIPLibs.netinet.h.in
o.module:	h.files
o.module:	h.file
o.module:	h.notlayer
o.module:	h.mimemap
o.module:	h.status
o.module:	h.httppub
o.module:	h.pads
o.module:	h.platforms
o.csfs:	c.csfs
o.csfs:	h.http
o.csfs:	C:h.swis
o.csfs:	C:h.kernel
o.csfs:	C:TCPIPLibs.sys.h.types
o.csfs:	C:TCPIPLibs.sys.h.cdefs
o.csfs:	C:TCPIPLibs.machine.h.endian
o.csfs:	C:TCPIPLibs.machine.h.ansi
o.csfs:	C:TCPIPLibs.machine.h.types
o.csfs:	C:TCPIPLibs.sys.h.socket
o.csfs:	C:TCPIPLibs.sys.h.cdefs
o.csfs:	C:TCPIPLibs.sys.h.errno
o.csfs:	C:h.kernel
o.csfs:	C:TCPIPLibs.net.h.if
o.csfs:	C:TCPIPLibs.sys.h.socket
o.csfs:	C:TCPIPLibs.sys.h.time
o.csfs:	C:TCPIPLibs.sys.h.cdefs
o.csfs:	C:TCPIPLibs.net.h.if_arp
o.csfs:	C:TCPIPLibs.sys.h.socket
o.csfs:	C:TCPIPLibs.netinet.h.in
o.csfs:	C:TCPIPLibs.sys.h.ioctl
o.csfs:	C:TCPIPLibs.sys.h.ttycom
o.csfs:	C:TCPIPLibs.sys.h.ioccom
o.csfs:	C:TCPIPLibs.sys.h.ioccom
o.csfs:	C:TCPIPLibs.sys.h.filio
o.csfs:	C:TCPIPLibs.sys.h.ioccom
o.csfs:	C:TCPIPLibs.sys.h.sockio
o.csfs:	C:TCPIPLibs.sys.h.ioccom
o.csfs:	C:TCPIPLibs.sys.h.cdefs
o.csfs:	C:TCPIPLibs.h.netdb
o.csfs:	C:TCPIPLibs.sys.h.cdefs
o.csfs:	C:TCPIPLibs.netns.h.ns
o.csfs:	C:TCPIPLibs.sys.h.cdefs
o.csfs:	C:TCPIPLibs.sys.h.time
o.csfs:	C:TCPIPLibs.h.socklib
o.csfs:	C:TCPIPLibs.sys.h.types
o.csfs:	C:TCPIPLibs.sys.h.socket
o.csfs:	C:TCPIPLibs.sys.h.stat
o.csfs:	C:TCPIPLibs.sys.h.time
o.csfs:	C:TCPIPLibs.sys.h.cdefs
o.csfs:	C:TCPIPLibs.sys.h.time
o.csfs:	C:TCPIPLibs.sys.h.uio
o.csfs:	C:TCPIPLibs.sys.h.cdefs
o.csfs:	C:h.kernel
o.csfs:	C:TCPIPLibs.h.inetlib
o.csfs:	C:TCPIPLibs.sys.h.types
o.csfs:	C:TCPIPLibs.netinet.h.in
o.csfs:	h.files
o.csfs:	h.file
o.csfs:	h.notlayer
o.csfs:	h.mimemap
o.csfs:	h.status
o.csfs:	h.httppub
o.csfs:	h.pads
o.csfs:	h.platforms
o.certs:	c.certs
o.certs:	h.http
o.certs:	C:h.swis
o.certs:	C:h.kernel
o.certs:	C:TCPIPLibs.sys.h.types
o.certs:	C:TCPIPLibs.sys.h.cdefs
o.certs:	C:TCPIPLibs.machine.h.endian
o.certs:	C:TCPIPLibs.machine.h.ansi
o.certs:	C:TCPIPLibs.machine.h.types
o.certs:	C:TCPIPLibs.sys.h.socket
o.certs:	C:TCPIPLibs.sys.h.cdefs
o.certs:	C:TCPIPLibs.sys.h.errno
o.certs:	C:h.kernel
o.certs:	C:TCPIPLibs.net.h.if
o.certs:	C:TCPIPLibs.sys.h.socket
o.certs:	C:TCPIPLibs.sys.h.time
o.certs:	C:TCPIPLibs.sys.h.cdefs
o.certs:	C:TCPIPLibs.net.h.if_arp
o.certs:	C:TCPIPLibs.sys.h.socket
o.certs:	C:TCPIPLibs.netinet.h.in
o.certs:	C:TCPIPLibs.sys.h.ioctl
o.certs:	C:TCPIPLibs.sys.h.ttycom
o.certs:	C:TCPIPLibs.sys.h.ioccom
o.certs:	C:TCPIPLibs.sys.h.ioccom
o.certs:	C:TCPIPLibs.sys.h.filio
o.certs:	C:TCPIPLibs.sys.h.ioccom
o.certs:	C:TCPIPLibs.sys.h.sockio
o.certs:	C:TCPIPLibs.sys.h.ioccom
o.certs:	C:TCPIPLibs.sys.h.cdefs
o.certs:	C:TCPIPLibs.h.netdb
o.certs:	C:TCPIPLibs.sys.h.cdefs
o.certs:	C:TCPIPLibs.netns.h.ns
o.certs:	C:TCPIPLibs.sys.h.cdefs
o.certs:	C:TCPIPLibs.sys.h.time
o.certs:	C:TCPIPLibs.h.socklib
o.certs:	C:TCPIPLibs.sys.h.types
o.certs:	C:TCPIPLibs.sys.h.socket
o.certs:	C:TCPIPLibs.sys.h.stat
o.certs:	C:TCPIPLibs.sys.h.time
o.certs:	C:TCPIPLibs.sys.h.cdefs
o.certs:	C:TCPIPLibs.sys.h.time
o.certs:	C:TCPIPLibs.sys.h.uio
o.certs:	C:TCPIPLibs.sys.h.cdefs
o.certs:	C:h.kernel
o.certs:	C:TCPIPLibs.h.inetlib
o.certs:	C:TCPIPLibs.sys.h.types
o.certs:	C:TCPIPLibs.netinet.h.in
o.certs:	h.files
o.certs:	h.file
o.certs:	h.notlayer
o.certs:	h.mimemap
o.certs:	h.status
o.certs:	h.httppub
o.certs:	h.pads
o.certs:	h.platforms
o.predirect:	c.predirect
o.predirect:	h.http
o.predirect:	C:h.swis
o.predirect:	C:h.kernel
o.predirect:	C:TCPIPLibs.sys.h.types
o.predirect:	C:TCPIPLibs.sys.h.cdefs
o.predirect:	C:TCPIPLibs.machine.h.endian
o.predirect:	C:TCPIPLibs.machine.h.ansi
o.predirect:	C:TCPIPLibs.machine.h.types
o.predirect:	C:TCPIPLibs.sys.h.socket
o.predirect:	C:TCPIPLibs.sys.h.cdefs
o.predirect:	C:TCPIPLibs.sys.h.errno
o.predirect:	C:h.kernel
o.predirect:	C:TCPIPLibs.net.h.if
o.predirect:	C:TCPIPLibs.sys.h.socket
o.predirect:	C:TCPIPLibs.sys.h.time
o.predirect:	C:TCPIPLibs.sys.h.cdefs
o.predirect:	C:TCPIPLibs.net.h.if_arp
o.predirect:	C:TCPIPLibs.sys.h.socket
o.predirect:	C:TCPIPLibs.netinet.h.in
o.predirect:	C:TCPIPLibs.sys.h.ioctl
o.predirect:	C:TCPIPLibs.sys.h.ttycom
o.predirect:	C:TCPIPLibs.sys.h.ioccom
o.predirect:	C:TCPIPLibs.sys.h.ioccom
o.predirect:	C:TCPIPLibs.sys.h.filio
o.predirect:	C:TCPIPLibs.sys.h.ioccom
o.predirect:	C:TCPIPLibs.sys.h.sockio
o.predirect:	C:TCPIPLibs.sys.h.ioccom
o.predirect:	C:TCPIPLibs.sys.h.cdefs
o.predirect:	C:TCPIPLibs.h.netdb
o.predirect:	C:TCPIPLibs.sys.h.cdefs
o.predirect:	C:TCPIPLibs.netns.h.ns
o.predirect:	C:TCPIPLibs.sys.h.cdefs
o.predirect:	C:TCPIPLibs.sys.h.time
o.predirect:	C:TCPIPLibs.h.socklib
o.predirect:	C:TCPIPLibs.sys.h.types
o.predirect:	C:TCPIPLibs.sys.h.socket
o.predirect:	C:TCPIPLibs.sys.h.stat
o.predirect:	C:TCPIPLibs.sys.h.time
o.predirect:	C:TCPIPLibs.sys.h.cdefs
o.predirect:	C:TCPIPLibs.sys.h.time
o.predirect:	C:TCPIPLibs.sys.h.uio
o.predirect:	C:TCPIPLibs.sys.h.cdefs
o.predirect:	C:h.kernel
o.predirect:	C:TCPIPLibs.h.inetlib
o.predirect:	C:TCPIPLibs.sys.h.types
o.predirect:	C:TCPIPLibs.netinet.h.in
o.predirect:	h.files
o.predirect:	h.file
o.predirect:	h.notlayer
o.predirect:	h.mimemap
o.predirect:	h.status
o.predirect:	h.httppub
o.predirect:	h.pads
o.predirect:	h.platforms
o.layershttp:	c.layershttp
o.layershttp:	h.http
o.layershttp:	C:h.swis
o.layershttp:	C:h.kernel
o.layershttp:	C:TCPIPLibs.sys.h.types
o.layershttp:	C:TCPIPLibs.sys.h.cdefs
o.layershttp:	C:TCPIPLibs.machine.h.endian
o.layershttp:	C:TCPIPLibs.machine.h.ansi
o.layershttp:	C:TCPIPLibs.machine.h.types
o.layershttp:	C:TCPIPLibs.sys.h.socket
o.layershttp:	C:TCPIPLibs.sys.h.cdefs
o.layershttp:	C:TCPIPLibs.sys.h.errno
o.layershttp:	C:h.kernel
o.layershttp:	C:TCPIPLibs.net.h.if
o.layershttp:	C:TCPIPLibs.sys.h.socket
o.layershttp:	C:TCPIPLibs.sys.h.time
o.layershttp:	C:TCPIPLibs.sys.h.cdefs
o.layershttp:	C:TCPIPLibs.net.h.if_arp
o.layershttp:	C:TCPIPLibs.sys.h.socket
o.layershttp:	C:TCPIPLibs.netinet.h.in
o.layershttp:	C:TCPIPLibs.sys.h.ioctl
o.layershttp:	C:TCPIPLibs.sys.h.ttycom
o.layershttp:	C:TCPIPLibs.sys.h.ioccom
o.layershttp:	C:TCPIPLibs.sys.h.ioccom
o.layershttp:	C:TCPIPLibs.sys.h.filio
o.layershttp:	C:TCPIPLibs.sys.h.ioccom
o.layershttp:	C:TCPIPLibs.sys.h.sockio
o.layershttp:	C:TCPIPLibs.sys.h.ioccom
o.layershttp:	C:TCPIPLibs.sys.h.cdefs
o.layershttp:	C:TCPIPLibs.h.netdb
o.layershttp:	C:TCPIPLibs.sys.h.cdefs
o.layershttp:	C:TCPIPLibs.netns.h.ns
o.layershttp:	C:TCPIPLibs.sys.h.cdefs
o.layershttp:	C:TCPIPLibs.sys.h.time
o.layershttp:	C:TCPIPLibs.h.socklib
o.layershttp:	C:TCPIPLibs.sys.h.types
o.layershttp:	C:TCPIPLibs.sys.h.socket
o.layershttp:	C:TCPIPLibs.sys.h.stat
o.layershttp:	C:TCPIPLibs.sys.h.time
o.layershttp:	C:TCPIPLibs.sys.h.cdefs
o.layershttp:	C:TCPIPLibs.sys.h.time
o.layershttp:	C:TCPIPLibs.sys.h.uio
o.layershttp:	C:TCPIPLibs.sys.h.cdefs
o.layershttp:	C:h.kernel
o.layershttp:	C:TCPIPLibs.h.inetlib
o.layershttp:	C:TCPIPLibs.sys.h.types
o.layershttp:	C:TCPIPLibs.netinet.h.in
o.layershttp:	h.files
o.layershttp:	h.file
o.layershttp:	h.notlayer
o.layershttp:	h.mimemap
o.layershttp:	h.status
o.layershttp:	h.httppub
o.layershttp:	h.pads
o.layershttp:	h.platforms
os.mimemap:	c.mimemap
os.mimemap:	C:h.kernel
os.mimemap:	h.mimemap
os.files:	c.files
os.files:	C:h.kernel
os.files:	h.types
os.files:	h.files
os.file:	c.file
os.file:	C:h.swis
os.file:	C:h.kernel
os.file:	C:h.kernel
os.file:	h.types
os.file:	h.file
os.http:	c.http
os.http:	h.http
os.http:	C:h.swis
os.http:	C:h.kernel
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
os.http:	h.files
os.http:	h.file
os.http:	h.notlayer
os.http:	h.mimemap
os.http:	C:SSLeay.h.objects
os.http:	C:SSLeay.h.asn1
os.http:	C:SSLeay.h.bn
os.http:	C:SSLeay.h.stack
os.http:	C:SSLeay.h.pem
os.http:	C:SSLeay.h.evp
os.http:	C:SSLeay.h.md2
os.http:	C:SSLeay.h.md5
os.http:	C:SSLeay.h.sha
os.http:	C:SSLeay.h.des
os.http:	C:SSLeay.h.rc4
os.http:	C:SSLeay.h.rc2
os.http:	C:SSLeay.h.blowfish
os.http:	C:SSLeay.h.idea
os.http:	C:SSLeay.h.mdc2
os.http:	C:SSLeay.h.des
os.http:	C:SSLeay.h.rsa
os.http:	C:SSLeay.h.bn
os.http:	C:SSLeay.h.dsa
os.http:	C:SSLeay.h.bn
os.http:	C:SSLeay.h.dh
os.http:	C:SSLeay.h.objects
os.http:	C:SSLeay.h.x509
os.http:	C:SSLeay.h.stack
os.http:	C:SSLeay.h.asn1
os.http:	C:SSLeay.h.rsa
os.http:	C:SSLeay.h.dsa
os.http:	C:SSLeay.h.dh
os.http:	C:SSLeay.h.evp
os.http:	C:SSLeay.h.x509_vfy
os.http:	C:SSLeay.h.pkcs7
os.http:	C:SSLeay.h.bio
os.http:	C:SSLeay.h.x509
os.http:	C:SSLeay.h.rsa
os.http:	C:SSLeay.h.ssl
os.http:	C:SSLeay.h.buffer
os.http:	C:SSLeay.h.bio
os.http:	C:SSLeay.h.x509
os.http:	C:SSLeay.h.ssl2
os.http:	C:SSLeay.h.ssl3
os.http:	C:SSLeay.h.buffer
os.http:	C:SSLeay.h.ssl23
os.http:	C:SSLeay.h.x509
os.http:	C:SSLeay.h.crypto
os.http:	C:TCPIPLibs.h.err
os.http:	C:TCPIPLibs.machine.h.ansi
os.http:	C:TCPIPLibs.sys.h.cdefs
os.http:	h.status
os.http:	h.httppub
os.http:	h.pads
os.http:	h.platforms
os.module:	c.module
os.module:	h.http
os.module:	C:h.swis
os.module:	C:h.kernel
os.module:	C:TCPIPLibs.sys.h.types
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	C:TCPIPLibs.machine.h.endian
os.module:	C:TCPIPLibs.machine.h.ansi
os.module:	C:TCPIPLibs.machine.h.types
os.module:	C:TCPIPLibs.sys.h.socket
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	C:TCPIPLibs.sys.h.errno
os.module:	C:h.kernel
os.module:	C:TCPIPLibs.net.h.if
os.module:	C:TCPIPLibs.sys.h.socket
os.module:	C:TCPIPLibs.sys.h.time
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	C:TCPIPLibs.net.h.if_arp
os.module:	C:TCPIPLibs.sys.h.socket
os.module:	C:TCPIPLibs.netinet.h.in
os.module:	C:TCPIPLibs.sys.h.ioctl
os.module:	C:TCPIPLibs.sys.h.ttycom
os.module:	C:TCPIPLibs.sys.h.ioccom
os.module:	C:TCPIPLibs.sys.h.ioccom
os.module:	C:TCPIPLibs.sys.h.filio
os.module:	C:TCPIPLibs.sys.h.ioccom
os.module:	C:TCPIPLibs.sys.h.sockio
os.module:	C:TCPIPLibs.sys.h.ioccom
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	C:TCPIPLibs.h.netdb
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	C:TCPIPLibs.netns.h.ns
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	C:TCPIPLibs.sys.h.time
os.module:	C:TCPIPLibs.h.socklib
os.module:	C:TCPIPLibs.sys.h.types
os.module:	C:TCPIPLibs.sys.h.socket
os.module:	C:TCPIPLibs.sys.h.stat
os.module:	C:TCPIPLibs.sys.h.time
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	C:TCPIPLibs.sys.h.time
os.module:	C:TCPIPLibs.sys.h.uio
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	C:h.kernel
os.module:	C:TCPIPLibs.h.inetlib
os.module:	C:TCPIPLibs.sys.h.types
os.module:	C:TCPIPLibs.netinet.h.in
os.module:	h.files
os.module:	h.file
os.module:	h.notlayer
os.module:	h.mimemap
os.module:	C:SSLeay.h.objects
os.module:	C:SSLeay.h.asn1
os.module:	C:SSLeay.h.bn
os.module:	C:SSLeay.h.stack
os.module:	C:SSLeay.h.pem
os.module:	C:SSLeay.h.evp
os.module:	C:SSLeay.h.md2
os.module:	C:SSLeay.h.md5
os.module:	C:SSLeay.h.sha
os.module:	C:SSLeay.h.des
os.module:	C:SSLeay.h.rc4
os.module:	C:SSLeay.h.rc2
os.module:	C:SSLeay.h.blowfish
os.module:	C:SSLeay.h.idea
os.module:	C:SSLeay.h.mdc2
os.module:	C:SSLeay.h.des
os.module:	C:SSLeay.h.rsa
os.module:	C:SSLeay.h.bn
os.module:	C:SSLeay.h.dsa
os.module:	C:SSLeay.h.bn
os.module:	C:SSLeay.h.dh
os.module:	C:SSLeay.h.objects
os.module:	C:SSLeay.h.x509
os.module:	C:SSLeay.h.stack
os.module:	C:SSLeay.h.asn1
os.module:	C:SSLeay.h.rsa
os.module:	C:SSLeay.h.dsa
os.module:	C:SSLeay.h.dh
os.module:	C:SSLeay.h.evp
os.module:	C:SSLeay.h.x509_vfy
os.module:	C:SSLeay.h.pkcs7
os.module:	C:SSLeay.h.bio
os.module:	C:SSLeay.h.x509
os.module:	C:SSLeay.h.rsa
os.module:	C:SSLeay.h.ssl
os.module:	C:SSLeay.h.buffer
os.module:	C:SSLeay.h.bio
os.module:	C:SSLeay.h.x509
os.module:	C:SSLeay.h.ssl2
os.module:	C:SSLeay.h.ssl3
os.module:	C:SSLeay.h.buffer
os.module:	C:SSLeay.h.ssl23
os.module:	C:SSLeay.h.x509
os.module:	C:SSLeay.h.crypto
os.module:	C:TCPIPLibs.h.err
os.module:	C:TCPIPLibs.machine.h.ansi
os.module:	C:TCPIPLibs.sys.h.cdefs
os.module:	h.status
os.module:	h.httppub
os.module:	h.pads
os.module:	h.platforms
os.csfs:	c.csfs
os.csfs:	h.http
os.csfs:	C:h.swis
os.csfs:	C:h.kernel
os.csfs:	C:TCPIPLibs.sys.h.types
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	C:TCPIPLibs.machine.h.endian
os.csfs:	C:TCPIPLibs.machine.h.ansi
os.csfs:	C:TCPIPLibs.machine.h.types
os.csfs:	C:TCPIPLibs.sys.h.socket
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	C:TCPIPLibs.sys.h.errno
os.csfs:	C:h.kernel
os.csfs:	C:TCPIPLibs.net.h.if
os.csfs:	C:TCPIPLibs.sys.h.socket
os.csfs:	C:TCPIPLibs.sys.h.time
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	C:TCPIPLibs.net.h.if_arp
os.csfs:	C:TCPIPLibs.sys.h.socket
os.csfs:	C:TCPIPLibs.netinet.h.in
os.csfs:	C:TCPIPLibs.sys.h.ioctl
os.csfs:	C:TCPIPLibs.sys.h.ttycom
os.csfs:	C:TCPIPLibs.sys.h.ioccom
os.csfs:	C:TCPIPLibs.sys.h.ioccom
os.csfs:	C:TCPIPLibs.sys.h.filio
os.csfs:	C:TCPIPLibs.sys.h.ioccom
os.csfs:	C:TCPIPLibs.sys.h.sockio
os.csfs:	C:TCPIPLibs.sys.h.ioccom
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	C:TCPIPLibs.h.netdb
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	C:TCPIPLibs.netns.h.ns
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	C:TCPIPLibs.sys.h.time
os.csfs:	C:TCPIPLibs.h.socklib
os.csfs:	C:TCPIPLibs.sys.h.types
os.csfs:	C:TCPIPLibs.sys.h.socket
os.csfs:	C:TCPIPLibs.sys.h.stat
os.csfs:	C:TCPIPLibs.sys.h.time
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	C:TCPIPLibs.sys.h.time
os.csfs:	C:TCPIPLibs.sys.h.uio
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	C:h.kernel
os.csfs:	C:TCPIPLibs.h.inetlib
os.csfs:	C:TCPIPLibs.sys.h.types
os.csfs:	C:TCPIPLibs.netinet.h.in
os.csfs:	h.files
os.csfs:	h.file
os.csfs:	h.notlayer
os.csfs:	h.mimemap
os.csfs:	C:SSLeay.h.objects
os.csfs:	C:SSLeay.h.asn1
os.csfs:	C:SSLeay.h.bn
os.csfs:	C:SSLeay.h.stack
os.csfs:	C:SSLeay.h.pem
os.csfs:	C:SSLeay.h.evp
os.csfs:	C:SSLeay.h.md2
os.csfs:	C:SSLeay.h.md5
os.csfs:	C:SSLeay.h.sha
os.csfs:	C:SSLeay.h.des
os.csfs:	C:SSLeay.h.rc4
os.csfs:	C:SSLeay.h.rc2
os.csfs:	C:SSLeay.h.blowfish
os.csfs:	C:SSLeay.h.idea
os.csfs:	C:SSLeay.h.mdc2
os.csfs:	C:SSLeay.h.des
os.csfs:	C:SSLeay.h.rsa
os.csfs:	C:SSLeay.h.bn
os.csfs:	C:SSLeay.h.dsa
os.csfs:	C:SSLeay.h.bn
os.csfs:	C:SSLeay.h.dh
os.csfs:	C:SSLeay.h.objects
os.csfs:	C:SSLeay.h.x509
os.csfs:	C:SSLeay.h.stack
os.csfs:	C:SSLeay.h.asn1
os.csfs:	C:SSLeay.h.rsa
os.csfs:	C:SSLeay.h.dsa
os.csfs:	C:SSLeay.h.dh
os.csfs:	C:SSLeay.h.evp
os.csfs:	C:SSLeay.h.x509_vfy
os.csfs:	C:SSLeay.h.pkcs7
os.csfs:	C:SSLeay.h.bio
os.csfs:	C:SSLeay.h.x509
os.csfs:	C:SSLeay.h.rsa
os.csfs:	C:SSLeay.h.ssl
os.csfs:	C:SSLeay.h.buffer
os.csfs:	C:SSLeay.h.bio
os.csfs:	C:SSLeay.h.x509
os.csfs:	C:SSLeay.h.ssl2
os.csfs:	C:SSLeay.h.ssl3
os.csfs:	C:SSLeay.h.buffer
os.csfs:	C:SSLeay.h.ssl23
os.csfs:	C:SSLeay.h.x509
os.csfs:	C:SSLeay.h.crypto
os.csfs:	C:TCPIPLibs.h.err
os.csfs:	C:TCPIPLibs.machine.h.ansi
os.csfs:	C:TCPIPLibs.sys.h.cdefs
os.csfs:	h.status
os.csfs:	h.httppub
os.csfs:	h.pads
os.csfs:	h.platforms
os.certs:	c.certs
os.certs:	h.http
os.certs:	C:h.swis
os.certs:	C:h.kernel
os.certs:	C:TCPIPLibs.sys.h.types
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	C:TCPIPLibs.machine.h.endian
os.certs:	C:TCPIPLibs.machine.h.ansi
os.certs:	C:TCPIPLibs.machine.h.types
os.certs:	C:TCPIPLibs.sys.h.socket
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	C:TCPIPLibs.sys.h.errno
os.certs:	C:h.kernel
os.certs:	C:TCPIPLibs.net.h.if
os.certs:	C:TCPIPLibs.sys.h.socket
os.certs:	C:TCPIPLibs.sys.h.time
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	C:TCPIPLibs.net.h.if_arp
os.certs:	C:TCPIPLibs.sys.h.socket
os.certs:	C:TCPIPLibs.netinet.h.in
os.certs:	C:TCPIPLibs.sys.h.ioctl
os.certs:	C:TCPIPLibs.sys.h.ttycom
os.certs:	C:TCPIPLibs.sys.h.ioccom
os.certs:	C:TCPIPLibs.sys.h.ioccom
os.certs:	C:TCPIPLibs.sys.h.filio
os.certs:	C:TCPIPLibs.sys.h.ioccom
os.certs:	C:TCPIPLibs.sys.h.sockio
os.certs:	C:TCPIPLibs.sys.h.ioccom
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	C:TCPIPLibs.h.netdb
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	C:TCPIPLibs.netns.h.ns
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	C:TCPIPLibs.sys.h.time
os.certs:	C:TCPIPLibs.h.socklib
os.certs:	C:TCPIPLibs.sys.h.types
os.certs:	C:TCPIPLibs.sys.h.socket
os.certs:	C:TCPIPLibs.sys.h.stat
os.certs:	C:TCPIPLibs.sys.h.time
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	C:TCPIPLibs.sys.h.time
os.certs:	C:TCPIPLibs.sys.h.uio
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	C:h.kernel
os.certs:	C:TCPIPLibs.h.inetlib
os.certs:	C:TCPIPLibs.sys.h.types
os.certs:	C:TCPIPLibs.netinet.h.in
os.certs:	h.files
os.certs:	h.file
os.certs:	h.notlayer
os.certs:	h.mimemap
os.certs:	C:SSLeay.h.objects
os.certs:	C:SSLeay.h.asn1
os.certs:	C:SSLeay.h.bn
os.certs:	C:SSLeay.h.stack
os.certs:	C:SSLeay.h.pem
os.certs:	C:SSLeay.h.evp
os.certs:	C:SSLeay.h.md2
os.certs:	C:SSLeay.h.md5
os.certs:	C:SSLeay.h.sha
os.certs:	C:SSLeay.h.des
os.certs:	C:SSLeay.h.rc4
os.certs:	C:SSLeay.h.rc2
os.certs:	C:SSLeay.h.blowfish
os.certs:	C:SSLeay.h.idea
os.certs:	C:SSLeay.h.mdc2
os.certs:	C:SSLeay.h.des
os.certs:	C:SSLeay.h.rsa
os.certs:	C:SSLeay.h.bn
os.certs:	C:SSLeay.h.dsa
os.certs:	C:SSLeay.h.bn
os.certs:	C:SSLeay.h.dh
os.certs:	C:SSLeay.h.objects
os.certs:	C:SSLeay.h.x509
os.certs:	C:SSLeay.h.stack
os.certs:	C:SSLeay.h.asn1
os.certs:	C:SSLeay.h.rsa
os.certs:	C:SSLeay.h.dsa
os.certs:	C:SSLeay.h.dh
os.certs:	C:SSLeay.h.evp
os.certs:	C:SSLeay.h.x509_vfy
os.certs:	C:SSLeay.h.pkcs7
os.certs:	C:SSLeay.h.bio
os.certs:	C:SSLeay.h.x509
os.certs:	C:SSLeay.h.rsa
os.certs:	C:SSLeay.h.ssl
os.certs:	C:SSLeay.h.buffer
os.certs:	C:SSLeay.h.bio
os.certs:	C:SSLeay.h.x509
os.certs:	C:SSLeay.h.ssl2
os.certs:	C:SSLeay.h.ssl3
os.certs:	C:SSLeay.h.buffer
os.certs:	C:SSLeay.h.ssl23
os.certs:	C:SSLeay.h.x509
os.certs:	C:SSLeay.h.crypto
os.certs:	C:TCPIPLibs.h.err
os.certs:	C:TCPIPLibs.machine.h.ansi
os.certs:	C:TCPIPLibs.sys.h.cdefs
os.certs:	h.status
os.certs:	h.httppub
os.certs:	h.pads
os.certs:	h.platforms
os.predirect:	c.predirect
os.predirect:	h.http
os.predirect:	C:h.swis
os.predirect:	C:h.kernel
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
os.predirect:	h.files
os.predirect:	h.file
os.predirect:	h.notlayer
os.predirect:	h.mimemap
os.predirect:	C:SSLeay.h.objects
os.predirect:	C:SSLeay.h.asn1
os.predirect:	C:SSLeay.h.bn
os.predirect:	C:SSLeay.h.stack
os.predirect:	C:SSLeay.h.pem
os.predirect:	C:SSLeay.h.evp
os.predirect:	C:SSLeay.h.md2
os.predirect:	C:SSLeay.h.md5
os.predirect:	C:SSLeay.h.sha
os.predirect:	C:SSLeay.h.des
os.predirect:	C:SSLeay.h.rc4
os.predirect:	C:SSLeay.h.rc2
os.predirect:	C:SSLeay.h.blowfish
os.predirect:	C:SSLeay.h.idea
os.predirect:	C:SSLeay.h.mdc2
os.predirect:	C:SSLeay.h.des
os.predirect:	C:SSLeay.h.rsa
os.predirect:	C:SSLeay.h.bn
os.predirect:	C:SSLeay.h.dsa
os.predirect:	C:SSLeay.h.bn
os.predirect:	C:SSLeay.h.dh
os.predirect:	C:SSLeay.h.objects
os.predirect:	C:SSLeay.h.x509
os.predirect:	C:SSLeay.h.stack
os.predirect:	C:SSLeay.h.asn1
os.predirect:	C:SSLeay.h.rsa
os.predirect:	C:SSLeay.h.dsa
os.predirect:	C:SSLeay.h.dh
os.predirect:	C:SSLeay.h.evp
os.predirect:	C:SSLeay.h.x509_vfy
os.predirect:	C:SSLeay.h.pkcs7
os.predirect:	C:SSLeay.h.bio
os.predirect:	C:SSLeay.h.x509
os.predirect:	C:SSLeay.h.rsa
os.predirect:	C:SSLeay.h.ssl
os.predirect:	C:SSLeay.h.buffer
os.predirect:	C:SSLeay.h.bio
os.predirect:	C:SSLeay.h.x509
os.predirect:	C:SSLeay.h.ssl2
os.predirect:	C:SSLeay.h.ssl3
os.predirect:	C:SSLeay.h.buffer
os.predirect:	C:SSLeay.h.ssl23
os.predirect:	C:SSLeay.h.x509
os.predirect:	C:SSLeay.h.crypto
os.predirect:	C:TCPIPLibs.h.err
os.predirect:	C:TCPIPLibs.machine.h.ansi
os.predirect:	C:TCPIPLibs.sys.h.cdefs
os.predirect:	h.status
os.predirect:	h.httppub
os.predirect:	h.pads
os.predirect:	h.platforms
os.layershttp:	c.layershttp
os.layershttp:	h.http
os.layershttp:	C:h.swis
os.layershttp:	C:h.kernel
os.layershttp:	C:TCPIPLibs.sys.h.types
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	C:TCPIPLibs.machine.h.endian
os.layershttp:	C:TCPIPLibs.machine.h.ansi
os.layershttp:	C:TCPIPLibs.machine.h.types
os.layershttp:	C:TCPIPLibs.sys.h.socket
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	C:TCPIPLibs.sys.h.errno
os.layershttp:	C:h.kernel
os.layershttp:	C:TCPIPLibs.net.h.if
os.layershttp:	C:TCPIPLibs.sys.h.socket
os.layershttp:	C:TCPIPLibs.sys.h.time
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	C:TCPIPLibs.net.h.if_arp
os.layershttp:	C:TCPIPLibs.sys.h.socket
os.layershttp:	C:TCPIPLibs.netinet.h.in
os.layershttp:	C:TCPIPLibs.sys.h.ioctl
os.layershttp:	C:TCPIPLibs.sys.h.ttycom
os.layershttp:	C:TCPIPLibs.sys.h.ioccom
os.layershttp:	C:TCPIPLibs.sys.h.ioccom
os.layershttp:	C:TCPIPLibs.sys.h.filio
os.layershttp:	C:TCPIPLibs.sys.h.ioccom
os.layershttp:	C:TCPIPLibs.sys.h.sockio
os.layershttp:	C:TCPIPLibs.sys.h.ioccom
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	C:TCPIPLibs.h.netdb
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	C:TCPIPLibs.netns.h.ns
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	C:TCPIPLibs.sys.h.time
os.layershttp:	C:TCPIPLibs.h.socklib
os.layershttp:	C:TCPIPLibs.sys.h.types
os.layershttp:	C:TCPIPLibs.sys.h.socket
os.layershttp:	C:TCPIPLibs.sys.h.stat
os.layershttp:	C:TCPIPLibs.sys.h.time
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	C:TCPIPLibs.sys.h.time
os.layershttp:	C:TCPIPLibs.sys.h.uio
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	C:h.kernel
os.layershttp:	C:TCPIPLibs.h.inetlib
os.layershttp:	C:TCPIPLibs.sys.h.types
os.layershttp:	C:TCPIPLibs.netinet.h.in
os.layershttp:	h.files
os.layershttp:	h.file
os.layershttp:	h.notlayer
os.layershttp:	h.mimemap
os.layershttp:	C:SSLeay.h.objects
os.layershttp:	C:SSLeay.h.asn1
os.layershttp:	C:SSLeay.h.bn
os.layershttp:	C:SSLeay.h.stack
os.layershttp:	C:SSLeay.h.pem
os.layershttp:	C:SSLeay.h.evp
os.layershttp:	C:SSLeay.h.md2
os.layershttp:	C:SSLeay.h.md5
os.layershttp:	C:SSLeay.h.sha
os.layershttp:	C:SSLeay.h.des
os.layershttp:	C:SSLeay.h.rc4
os.layershttp:	C:SSLeay.h.rc2
os.layershttp:	C:SSLeay.h.blowfish
os.layershttp:	C:SSLeay.h.idea
os.layershttp:	C:SSLeay.h.mdc2
os.layershttp:	C:SSLeay.h.des
os.layershttp:	C:SSLeay.h.rsa
os.layershttp:	C:SSLeay.h.bn
os.layershttp:	C:SSLeay.h.dsa
os.layershttp:	C:SSLeay.h.bn
os.layershttp:	C:SSLeay.h.dh
os.layershttp:	C:SSLeay.h.objects
os.layershttp:	C:SSLeay.h.x509
os.layershttp:	C:SSLeay.h.stack
os.layershttp:	C:SSLeay.h.asn1
os.layershttp:	C:SSLeay.h.rsa
os.layershttp:	C:SSLeay.h.dsa
os.layershttp:	C:SSLeay.h.dh
os.layershttp:	C:SSLeay.h.evp
os.layershttp:	C:SSLeay.h.x509_vfy
os.layershttp:	C:SSLeay.h.pkcs7
os.layershttp:	C:SSLeay.h.bio
os.layershttp:	C:SSLeay.h.x509
os.layershttp:	C:SSLeay.h.rsa
os.layershttp:	C:SSLeay.h.ssl
os.layershttp:	C:SSLeay.h.buffer
os.layershttp:	C:SSLeay.h.bio
os.layershttp:	C:SSLeay.h.x509
os.layershttp:	C:SSLeay.h.ssl2
os.layershttp:	C:SSLeay.h.ssl3
os.layershttp:	C:SSLeay.h.buffer
os.layershttp:	C:SSLeay.h.ssl23
os.layershttp:	C:SSLeay.h.x509
os.layershttp:	C:SSLeay.h.crypto
os.layershttp:	C:TCPIPLibs.h.err
os.layershttp:	C:TCPIPLibs.machine.h.ansi
os.layershttp:	C:TCPIPLibs.sys.h.cdefs
os.layershttp:	h.status
os.layershttp:	h.httppub
os.layershttp:	h.pads
os.layershttp:	h.platforms
