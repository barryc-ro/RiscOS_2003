# Makefile for DHCP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 26-Mar-99  SNB          Created
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
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir
TOUCH   = create

COMPONENT = DHCP
CMHGNAME  = DHCPhdr

DEPENDS = -depend !Depend ${THROWBACK}

#
# Internet version compatibility
#
COMPAT = -DCOMPAT_INET4


DEFINES = ${COMPAT} ${OPTIONS} -UTRACE

AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -ffcah -c -zM -zps1 -wp ${DEFINES} ${INCLUDES} ${DEPENDS}
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

OBJS      =	\
 o.dhcp		\
 o.dhcperrors	\
 o.module	\
 o.interfaces	\
 o.iparp	\
 o.packets	\
 o.sockets	\
 $(CMHGOBJ)

#
# Rule patterns
#
.SUFFIXES:  .o .s .c .h .x .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# build a DHCP module
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
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${XTARGET} ${OBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${DIRS}
	${MKDIR} rm
        ${LD} -o $@ -rmf ${OBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${CLIB}
        $(MODSQZ) $@
        Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${ROMCSTUBS}

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

o.module: VersionNum

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

# Dynamic dependencies:
