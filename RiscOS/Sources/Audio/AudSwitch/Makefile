# Makefile for AudioSwitch
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-May-01  TA		  Created

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

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
CD      = dir
TOUCH   = create
STRIP   = -stripdepnd

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -depend !Depend ${THROWBACK} -zM -ffa ${INCLUDES} ${DFLAGS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr

DFLAGS     = ${OPTIONS}
DEBUG      = -DDEBUGLIB

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = C:debug.o.remotezm
DEBUGLIB   = C:DebugLib.o.DebugLibzm
INETLIB    = TCPIPLibs:o.inetlibzm
SOCKLIB    = TCPIPLibs:o.socklib5zm
UNIXLIB    = TCPIPLibs:o.unixlibzm
DEBUGLIBS  = ${DEBUGLIB} ${INETLIB} ${SOCKLIB} ${UNIXLIB}

#
# Include files
#
INCLUDES = -IC:,OS:

# Program specific options:
#
COMPONENT = AudSwitch
TARGET    = aof.${COMPONENT}
RMTARGET  = rm.${COMPONENT}
DBGTARGET = rm.d${COMPONENT}
EXPORTS   = ${EXP_C_H}.${COMPONENT}
DIRS      = o._dirs

OBJS      =  \
o.header     \
o.audio	     \
o.module

OBJS_D    =  \
o.header     \
od.audio     \
od.module

#
# Rule patterns
#
.SUFFIXES:
.SUFFIXES: .o .od .h .s .cmhg .c
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.c.od:;     ${CC}   ${CFLAGS} ${DEBUG} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;      ${AS}   ${AFLAGS} $< $@

# Build all variants
#
all:	ram debug rom

# Build a relocatable module:
#
ram:    ${RMTARGET}
        @echo ${COMPONENT}: all complete

# Build a debug module:
debug:  ${DBGTARGET}     
        @echo BaseDebug: all complete

#
# RISC OS ROM build rules:
#
rom:    ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o        ${WFLAGS}
        ${WIPE} od       ${WFLAGS}
        ${WIPE} linked   ${WFLAGS}
        ${WIPE} rm       ${WFLAGS}
        ${WIPE} aof      ${WFLAGS}
        ${RM} h.header
        ${RM} ${TARGET}
        ${STRIP}
        @echo ${COMPONENT}: cleaned

od.audio o.audio o.module od.module: h.header

${DIRS}:
        ${MKDIR} o
        ${MKDIR} od
        ${TOUCH} $@

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${DIRS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Relocatable module target
#
${RMTARGET}: ${OBJS} ${DIRS}
        ${MKDIR} rm
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}

${DBGTARGET}: ${OBJS_D} ${DIRS}
        ${MKDIR} rm
        ${LD} -rmf -o $@ ${OBJS_D} ${CLIB} ${DEBUGLIBS}

${EXP_C_H}.${COMPONENT}:     h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Dynamic dependencies:
