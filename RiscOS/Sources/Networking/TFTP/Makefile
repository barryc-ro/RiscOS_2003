# Makefile for TFTP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 07-Jul-99  KJB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

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

COMPONENT = TFTP
CMHGNAME  = TFTPhdr

EXPORTS = 

DEPENDS = -depend !Depend ${THROWBACK}

#
# Internet version compatibility
#
#COMPAT = -DCOMPAT_INET4


DEFINES = ${COMPAT} ${OPTIONS}

AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -ffcah -c -zM -zps1 -Wp ${DEFINES} ${INCLUDES} ${DEPENDS}
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
SOCKLIB   = TCPIPLibs:o.socklib5zm
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
 o.tftp \
 o.module

MODOBJS   = ${OBJS} ${CMHGOBJ}

#
# Rule patterns
#
.SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
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
${TARGET}: ${XTARGET} ${MODOBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${DIRS}
	${MKDIR} rm
        ${LD} -o $@ -rmf ${MODOBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${CLIB}
        $(MODSQZ) $@
        Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${MODOBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${MODOBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


o.module: h.${CMHGNAME}

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

# Dynamic dependencies:
