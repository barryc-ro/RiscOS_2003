# Makefile for UDPV
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 13-May-99  SNB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

#
# Generic options:
#
AWK     = awk
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
TOUCH   = create

COMPONENT = UDPV
CMHGNAME  = UDPVHdr

EXPORTS = ${EXP_HDR}.h.${COMPONENT}

DEPENDS = -depend !Depend ${THROWBACK}

#
# Internet version compatibility
#
COMPAT = -DCOMPAT_INET4 -DINET

DEFINES = ${COMPAT} ${OPTIONS}

AFLAGS = ${DEPENDS} ${THROWBACK} -Stamp -quit
CFLAGS  = -ffca -c -zM -zps1 -wp -Otime ${DEFINES} ${INCLUDES} ${DEPENDS}
CMHGFLAGS = -p ${DEPENDS}

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

DIRS = o._dirs

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
RPCLIB    = TCPIPLibs:rpc.o.rpc_datazm TCPIPLibs:rpc.o.rpclibzm
INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm


#
# Include files
#
INCLUDES = -ITCPIPLibs:,C:

#
# Program specific options:
#
TARGET    = rm.${COMPONENT}
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${COMPONENT}

OBJS      =\
 o.bufs\
 o.module\
 o.mbuf\
 o.global\
 o.irqs\
 $(CMHGOBJ)

#
# Rule patterns
#
.SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# build a UDPV module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${XTARGET} ${OBJS} ${INETLIB} ${SOCKLIB} ${CLIB} ${DIRS}
	${MKDIR} rm
        ${LD} -o $@ -rmf ${OBJS} ${INETLIB} ${SOCKLIB} ${CLIB}
        $(MODSQZ) $@
        Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${INETLIB} ${SOCKLIB} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${INETLIB} ${SOCKLIB} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


h.${CMHGNAME}: ${CMHGOBJ}
	@|

${EXPORTS}: h.${CMHGNAME} ${DIRS}
	${AWK} -- "/.define UDPV\_00/,/endif/" h.${CMHGNAME} { > $@ }
	SetType $@ Text

#
# Specialised static dependencies
#

o.module: UDPVHdr.h

o.UDPVHdr h.UDPVHdr: VersionNum

#
# Dirs rule for building from clean
#

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

# Dynamic dependencies:
