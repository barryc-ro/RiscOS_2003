# Makefile for RCMM keyboard/mouse driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 15-Jul-99  SBF          Created
# 27-Jul-99  SBF          Added DebugLib support
# 24-Aug-99  SBF/KJB      Added support for standard and debug targets

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

#
# Generic options:
#
MKDIR   = do mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD      = dir
TOUCH   = create

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
INCLUDES = -IC:,TCPIPLibs:

# Program specific options:
#
COMPONENT = RCMMDriver
TARGET    = aof.${COMPONENT}
RMTARGET  = rm.${COMPONENT}
DBGTARGET = rm.RCMMDebug
EXPORTS   =
DIRS      = o._dirs

OBJS      =  \
o.driver     \
o.key        \
o.mouse      \
o.header     \
o.module

OBJS_D    =  \
od.driver    \
od.key       \
od.mouse     \
o.header     \
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
        @echo RCMMDebug: all complete

#
# RISC OS ROM build rules:
#
rom:    ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${RMTARGET}
        ${MKDIR} ${INSTDIR}
        ${CP} ${RMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ram module installed

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
        @echo ${COMPONENT}: cleaned

o.module od.module: h.header

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
