# Makefile for Graphics Context Manager module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 07/01/98  RLougher Created
# 18/08/98  MRobertshaw  Modified For Funai II Build 
#

#
# Component specific options:
#
COMPONENT  = GCManager
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}

#
# Generic options:
#
MKDIR   = mkdir -p
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
ASM     = objasm
MODSQZ  = modsqz

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

ASMFLAGS = -depend !Depend ${THROWBACK}
CMHGFLAGS = -depend !Depend -p ${THROWBACK}
CFLAGS   = -c ${INCLUDES} ${THROWBACK} -depend !Depend -zM -ffah -zps1
INCLUDES = -Itbox:,C:,C:MemLib

HDRFILE  = h.ModHdr
CMHGFILE = cmhg.ModHdr

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.c_abssym
TBOXLIB   = tbox:o.toolboxlib
WIMPLIB   = tbox:o.wimplib
MEMLIB    = C:MemLib.o.memlib

OBJS =\
        o.ModHdr \
        o.arc \
        o.copyArea \
        o.filledPoly \
        o.flush \
        o.line \
        o.plot \
        o.roundRect \
        o.text \
        o.circle \
        o.filledRect \
        o.gc \
        o.main \
        o.point \
        o.rect \
        o.setAtts \
        o.setFont \
        o.sprite \
        o.triangle \
        o.clearRect \
        o.ellipse \
        o.filledRRec \
        o.getClpRect \
        o.op \
        o.polygon \
        o.redraw \
        o.setClpRect \
        o.task \
        o.utils \
        o.memory \
        o.mpeg  \
        o.antiTwit

RAM_OBJS =\
        o.ModHdr \
        o.arc \
        o.copyArea \
        o.filledPoly \
        o.flush \
        o.line \
        o.plot \
        o.roundRect \
        o.text \
        o.circle \
        o.filledRect \
        o.gc \
        o.main \
        o.point \
        o.rect \
        o.setAtts \
        o.setFont \
        o.sprite \
        o.triangle \
        o.clearRect \
        o.ellipse \
        o.filledRRec \
        o.getClpRect \
        o.op \
        o.polygon \
        o.redraw \
        o.setClpRect \
        o.task \
        o.utils \
        o.memory \
        o.mpeg  \
        o.antiTwit
#
# Rule patterns
#

.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;  $(ASM) ${ASMFLAGS} $< $@

#
# Main rules:
#
#
all: ${RAM_MODULE}
        @echo ${COMPONENT}: Module built (RAM)

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${RM} ${HDRFILE}
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${MEMLIB} ${WIMPLIB} ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} $(MEMLIB) $(WIMPLIB) ${ROMCSTUBS}

${RAM_MODULE}: ${RAM_OBJS} ${MEMLIB} ${WIMPLIB} ${CLIB}
        ${MKDIR} rm
        ${LD} -o $@ -rmf ${RAM_OBJS} $(MEMLIB) $(WIMPLIB) ${CLIB}
        ${MODSQZ} $@

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

o.main: ${HDRFILE}

${HDRFILE}: ${CMHGFILE}
        ${CMHG} -p ${CMHGFILE} o.ModHdr $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
