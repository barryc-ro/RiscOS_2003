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
CCFLAGS  = -pcc -c -depend !Depend ${INCLUDES} -zM -Wp -ffa -DRISCOS -DRISCOS_ATM25 -DRISCOS_MPEG -throwback
# Flags for debugging ...
#CCFLAGS  = 
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC:,TCPIPLibs:include.

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


module.o:  module.c h.cmhg

h.cmhg:   cmhg.VSPHdr
          cmhg -d h.cmhg cmhg.VSPHdr



# Dynamic dependencies:
o.mbuf:	c.mbuf
o.mbuf:	C:h.kernel
o.mbuf:	C:h.swis
o.mbuf:	TCPIPLibs:include.sys.h.types
o.mbuf:	h.misc
o.mbuf:	TCPIPLibs:include.sys.h.mbuf
o.vspc:	c.vspc
o.vspc:	TCPIPLibs:include.sys.h.types
o.vspc:	C:h.kernel
o.vspc:	C:h.swis
o.vspc:	h.vsp
o.vspc:	h.misc
o.vspc:	h.zap
o.zap:	c.zap
o.zap:	TCPIPLibs:include.sys.h.types
o.zap:	h.vsp
o.zap:	h.zap
o.zap:	h.misc
o.zap:	C:h.kernel
o.zap:	C:h.swis
o.module:	c.module
o.module:	TCPIPLibs:include.sys.h.types
o.module:	C:h.stdio
o.module:	h.vsp
o.module:	C:h.errno
o.module:	h.zap
o.module:	h.misc
o.module:	TCPIPLibs:include.sys.h.socket
o.module:	TCPIPLibs:include.sys.h.ioctl
o.module:	TCPIPLibs:include.sys.h.ttychars
o.module:	TCPIPLibs:include.sys.h.ttydev
o.module:	TCPIPLibs:include.netinet.h.in
o.module:	TCPIPLibs:include.net.h.if
o.module:	TCPIPLibs:include.sys.h.mbuf
o.module:	TCPIPLibs:include.net.h.if_arp
o.module:	TCPIPLibs:include.sys.h.socket
o.module:	TCPIPLibs:include.h.netdb
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.dci4
o.module:	TCPIPLibs:include.sys.h.mbuf
o.module:	h.cmhg
