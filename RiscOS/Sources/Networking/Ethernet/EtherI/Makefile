# Makefile for EtherI
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
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir


AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -pcc -ff -zps1 -zM -I${INCLUDES},. -DKERNEL -DINET \
	${DEBUG} -depend !Depend -DMXPATCH # -DSUPPORT_TRADITIONAL_MBUFS


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

#
# Include files
#
LEVEL=		^
INCLUDES=	TCPIPLibs:,C:

COMPONENT = EtherI
TARGET    = rm.EtherI
ROMTARGET = aof.EtherI
OBJS      = uniqueid.o if_en.o e2module.o support.o poduleirq.o en_io.o mbuf.o dallas.o enhdr.o
#	svcprintf.o

#
# Rule patterns
#
#  .SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build target
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${ROMTARGET}
        ${RM} map.${COMPONENT}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS}
        ${LD} -o $@ -rmf ${OBJS} ${CLIB}
        $(MODSQZ) $(TARGET)

#
# ROM Target 
#
${ROMTARGET}: ${OBJS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}
        
#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
o.uniqueid: s.uniqueid
o.uniqueid: Hdr:ListOpts
o.uniqueid: Hdr:Macros
o.uniqueid: Hdr:System
o.uniqueid: Hdr:SWIs
o.uniqueid: Hdr:CPU.Generic26
o.uniqueid: Hdr:IO.GenericIO
o.uniqueid: Hdr:RISCOS
o.uniqueid: Hdr:EnvNumbers
o.uniqueid: Hdr:Proc
o.uniqueid: h.VickySpace
o.if_en:	c.if_en
o.if_en:	C:h.kernel
o.if_en:	C:h.swis
o.if_en:	TCPIPLibs:h.delay
o.if_en:	TCPIPLibs:sys.h.types
o.if_en:	TCPIPLibs:sys.h.dcistructs
o.if_en:	TCPIPLibs:sys.h.mbuf
o.if_en:	TCPIPLibs:sys.h.errno
o.if_en:	TCPIPLibs:sys.^.h.errno
o.if_en:	C:h.kernel
o.if_en:	TCPIPLibs:h.int_hndlr
o.if_en:	TCPIPLibs:h.xcb
o.if_en:	TCPIPLibs:h.ioc
o.if_en:	h.if_en
o.if_en:	h.dallas
o.if_en:	h.e2module
o.if_en:	h.dallas
o.e2module:	c.e2module
o.e2module:	C:h.kernel
o.e2module:	C:h.swis
o.e2module:	TCPIPLibs:h.xcb
o.e2module:	TCPIPLibs:h.int_hndlr
o.e2module:	TCPIPLibs:sys.h.dcistructs
o.e2module:	TCPIPLibs:sys.h.errno
o.e2module:	TCPIPLibs:sys.^.h.errno
o.e2module:	C:h.kernel
o.e2module:	TCPIPLibs:sys.h.mbuf
o.e2module:	TCPIPLibs:netinet.h.if_ether
o.e2module:	TCPIPLibs:sys.h.types
o.e2module:	TCPIPLibs:sys.h.socket
o.e2module:	TCPIPLibs:net.h.if
o.e2module:	TCPIPLibs:sys.h.mbuf
o.e2module:	TCPIPLibs:net.^.net.h.if_arp
o.e2module:	TCPIPLibs:sys.h.socket
o.e2module:	TCPIPLibs:net.h.if_arp
o.e2module:	TCPIPLibs:netinet.h.in
o.e2module:	h.e2module
o.e2module:	h.if_en
o.e2module:	h.dallas
o.support:	c.support
o.support:	TCPIPLibs:sys.h.types
o.poduleirq: s.poduleirq
o.poduleirq: Hdr:ListOpts
o.poduleirq: Hdr:Macros
o.poduleirq: Hdr:System
o.poduleirq: Hdr:SWIs
o.poduleirq: Hdr:CPU.Generic26
o.poduleirq: Hdr:IO.GenericIO
o.poduleirq: Hdr:RISCOS
o.poduleirq: Hdr:EnvNumbers
o.poduleirq: Hdr:ModHand
o.en_io: s.en_io
o.en_io: Hdr:ListOpts
o.en_io: Hdr:Macros
o.en_io: Hdr:System
o.en_io: Hdr:SWIs
o.en_io: Hdr:CPU.Generic26
o.en_io: Hdr:IO.GenericIO
o.en_io: Hdr:RISCOS
o.en_io: Hdr:EnvNumbers
o.en_io: Hdr:ModHand
o.mbuf:	c.mbuf
o.mbuf:	C:h.kernel
o.mbuf:	C:h.swis
o.mbuf:	TCPIPLibs:sys.h.param
o.mbuf:	TCPIPLibs:sys.h.sysconf
o.mbuf:	TCPIPLibs:sys.^.machine.h.machparam
o.mbuf:	TCPIPLibs:sys.h.signal
o.mbuf:	TCPIPLibs:sys.h.types
o.mbuf:	TCPIPLibs:sys.h.errno
o.mbuf:	TCPIPLibs:sys.^.h.errno
o.mbuf:	C:h.kernel
o.mbuf:	TCPIPLibs:sys.h.mbuf
o.mbuf:	TCPIPLibs:netinet.h.if_ether
o.mbuf:	TCPIPLibs:sys.h.types
o.mbuf:	TCPIPLibs:sys.h.socket
o.mbuf:	TCPIPLibs:net.h.if
o.mbuf:	TCPIPLibs:sys.h.mbuf
o.mbuf:	TCPIPLibs:net.^.net.h.if_arp
o.mbuf:	TCPIPLibs:sys.h.socket
o.mbuf:	TCPIPLibs:net.h.if_arp
o.mbuf:	TCPIPLibs:netinet.h.in
o.dallas:	c.dallas
o.dallas:	h.dallas
