# Makefile for SCInterface
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Jun-96   RWB 	  New for SCInterface
# 11-Sep-96   RWB	  Corrected a major flaw in ROM build
# 07 May 1997 RWB	  Now supports remote debug library
# 22 May 1997 RWB	  Changed debug build
# 24 Nov 1998 AJP         Use MINIJIS_CARD build flag to build for Funai3 cards

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
CD	= dir
DEFMOD  = DefMod
AR	= LibFile
STRIP   = stripdepnd

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
CMHGFLAGS = -p  ${DFLAGS} ${INCLUDES}

DFLAGS  = -DUSE_TINY ${DEBUGLIB} ${OPTS}

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
DEBUG_LIBS = <Lib$Dir>.DebugLib.o.DebugLibzm
SOCKLIB    = TCPIPLibs:o.socklibzm
INETLIB    = TCPIPLibs:o.inetlibzm
UNIXLIB    = TCPIPLibs:o.unixlibzm
ASMUTILS   = C:AsmUtils.o.AsmUtilsZM

#
# Include files
#
INCLUDES = -IC:AsmUtils,C:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = SCInter
TARGET    = aof.${COMPONENT}
EXPORTS   = ${EXP_C_H}.${COMPONENT}

.SUFFIXES:   .od

OBJS      =	\
o.module	\
o.sctrans	\
o.path		\
o.orga		\
o.schlumb	\
o.incard	\
o.minijis       \
o.records       \
o.machine       \
o.header

DOBJS      =	\
od.module	\
od.sctrans	\
od.path		\
od.orga		\
od.schlumb	\
od.incard	\
od.minijis      \
od.records      \
od.machine      \
od.header

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.c.od:;     ${CC} ${CFLAGS} -o $@ $<
.cmhg.od:;  ${CMHG} ${CMHGFLAGS} -o $@ $< -d h.$* 
.s.od:;     ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

debug: rm.${COMPONENT}D

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: all complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${RM} ${TARGET}
	${RM} h.header
        ${STRIP}
        @echo ${COMPONENT}: cleaned

dirs:
        ${MKDIR} o
        ${MKDIR} od
        ${MKDIR} rm
        ${MKDIR} linked
        ${MKDIR} aof
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} dirs header.h
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${ASMUTILS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}: ${OBJS} dirs header.h
        ${LD} -rmf -o $@ ${OBJS} ${ASMUTILS} ${CLIB}
        Access rm.${COMPONENT} RW/R

rm.${COMPONENT}D: ${DOBJS} ${DEBUG_LIBS} ${SOCKLIB} ${INETLIB} ${ASMUTILS} dirs header.h
        ${LD} -rmf -o $@ ${DOBJS} ${CLIB} ${DEBUG_LIBS} ${SOCKLIB} ${INETLIB} ${ASMUTILS} 
        Access rm.${COMPONENT}D RW/R

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Dynamic dependencies:
