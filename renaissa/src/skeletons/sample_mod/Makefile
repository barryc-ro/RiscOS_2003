# Makefile for Sample Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Mar-00  SNB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

#
# Generic options:
#
AWK     = awk
MKDIR   = mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD      = dir
TOUCH   = create

COMPONENT = Sample
CMHGNAME  = samplehdr

EXPORTS = ${EXP_HDR}.h.${COMPONENT}

DEPENDS = -depend !Depend ${THROWBACK}
COMPAT  = -DCOMPAT_INET4

#
# Building macros
#
DEFINES = ${OPTIONS} -UTRACE ${COMPAT}
AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -ffcah -c -zM -zps1 ${DEFINES} ${INCLUDES} ${DEPENDS}
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
ASMUTILS  = C:AsmUtils.o.AsmUtilsZM
DEBUBLIB  = C:DebugLib.o.DebugLibzm


LIBS      = ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${ASMUTILS}
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

OBJS      = \
 o.module \
 $(CMHGOBJ)

#
# Rule patterns
#
.SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# build a module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: all complete

dirs:
        ${MKDIR} ${INSTDIR}
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
${TARGET}: ${XTARGET} ${OBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -rmf ${OBJS} ${LIBS} ${CLIB}
        $(MODSQZ) $@
        Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${DIRS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


${EXPORTS}: h.${CMHGNAME} ${DIRS}
        ${CP} h.${CMHGNAME} $@ ${CPFLAGS}

o.module: VersionNum h.${CMHGNAME}

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

# Dynamic dependencies:
