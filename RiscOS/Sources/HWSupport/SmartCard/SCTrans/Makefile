# Makefile for SCTransport
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-May-96  RWB 	  New for SCTransport
# 25-Jul-96  RWB	  Changed install rules
# 11-Sep-96  RWB	  Corrected a major flaw in ROM build
# 07 May 1997 RWB	  Now supports remote debug library

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}

DFLAGS = -DUSE_TINY ${DEBUG}
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
#
# Include files
#
INCLUDES = -IC:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = SCTrans
TARGET    = aof.${COMPONENT}

.SUFFIXES:   .od

OBJS      =	o.sys o.com o.data o.api o.atr o.t0_sm o.t1 o.chan o.irqs \
	  	o.serial o.header 

DOBJS      =	od.sys od.com od.data od.api od.atr od.t0_sm od.t1 od.chan od.irqs \
	  	od.serial od.header 

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@
.c.od:;     ${CC} ${CFLAGS} -o $@ $< -DREMOTE_DEBUG
.cmhg.od:;  ${CMHG} ${CMHGFLAGS} -o $@ $< h.$* -DREMOTE_DEBUG
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

export_hdrs:
	${CP} h.SCTransport ${EXP_C_H}.SCTransport ${CPFLAGS}
        @echo ${COMPONENT}: export complete (headers)

export_libs:
        @echo ${COMPONENT}: export complete (libs)

export: export_${PHASE}

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${RM} h.header
        @echo ${COMPONENT}: cleaned
dirs:
        ${MKDIR} o
        ${MKDIR} od
        ${MKDIR} rm
        ${MKDIR} linked
        ${MKDIR} map
        ${MKDIR} aof

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} dirs
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}: ${OBJS} ${CLIB} dirs
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        Access rm.${COMPONENT} RW/R

rm.${COMPONENT}D: ${DOBJS} ${REMOTEDB} dirs
        ${LD} -rmf -o $@ ${DOBJS} ${CLIB} ${REMOTEDB}
        Access rm.${COMPONENT}D RW/R

# dummy target
h.header: cmhg.header
	@echo

#
# Dynamic dependencies:
