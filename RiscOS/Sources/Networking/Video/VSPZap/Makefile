# Makefile for VSPZap
#

#
# Paths
#
EXP_HDR = <export$dir>

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
CD	= dir
DEFMOD  = DefMod
AR	= LibFile

ASFLAGS = -depend !Depend -Stamp -quit -module
CCFLAGS  = -pcc -c -depend !Depend ${INCLUDES} -zM -Wp -ffa -DRISCOS -DRISCOS_ATM25 -DRISCOS_MPEG
# Flags for debugging ...
#CCFLAGS  = 
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC: -ITCPIPLibs:

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@

#
# Program specific options:
#
COMPONENT = VSPZap
TARGET    = aof.VSPZap
TARGETSA  = rm.VSPZap
EXPORTS   = 
OBJS      = o.zap o.mbuf o.module o.vspc o.poduleirq o.vsphdr

#
# Generic rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: all complete

rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGETSA}: ${OBJS} ${CLIB}
        ${LD} -o $@ -rmf ${OBJS} ${CLIB}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


#
# Dynamic dependencies:
o.vspc:	c.vspc
o.vspc:	h.types
o.vspc:	C:h.kernel
o.vspc:	C:h.swis
o.vspc:	h.vsp
o.vspc:	h.misc
o.vspc:	h.zap
o.poduleirq: s.poduleirq
o.poduleirq: Hdr:ListOpts
o.poduleirq: Hdr:Macros
o.poduleirq: Hdr:System
o.poduleirq: Hdr:EnvNumbers
o.zap:	c.zap
o.zap:	h.types
o.zap:	h.vsp
o.zap:	h.zap
o.zap:	h.misc
o.zap:	C:h.kernel
o.zap:	C:h.swis
o.mbuf:	c.mbuf
o.mbuf:	C:h.kernel
o.mbuf:	C:h.swis
o.mbuf:	h.types
o.mbuf:	h.misc
o.mbuf:	h.mbuf
o.module:	c.module
o.module:	h.types
o.module:	h.stdio
o.module:	h.vsp
o.module:	h.misc
o.module:	C:h.errno
o.module:	h.zap
o.module:	TCPIPLibs:sys.h.socket
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	TCPIPLibs:sys.h.ioctl
o.module:	TCPIPLibs:sys.h.ttycom
o.module:	TCPIPLibs:sys.h.ioccom
o.module:	TCPIPLibs:sys.h.ioccom
o.module:	TCPIPLibs:sys.h.filio
o.module:	TCPIPLibs:sys.h.ioccom
o.module:	TCPIPLibs:sys.h.sockio
o.module:	TCPIPLibs:sys.h.ioccom
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	TCPIPLibs:netinet.h.in
o.module:	TCPIPLibs:net.h.if
o.module:	TCPIPLibs:sys.h.socket
o.module:	TCPIPLibs:sys.h.time
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	TCPIPLibs:net.h.if_arp
o.module:	TCPIPLibs:sys.h.socket
o.module:	TCPIPLibs:h.netdb
o.module:	TCPIPLibs:sys.h.cdefs
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.dci4
o.module:	h.mbuf
