# Makefile for Graphics Context Manager module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 07/01/98  RLougher Created
#

#
# Component specific options:
#
COMPONENT  = GCManager
ROM_MODULE = aof.${COMPONENT}

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
ASM	= objasm

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

CFLAGS   = -c ${INCLUDES} -zM -ffah -zps1
INCLUDES = -Itbox:,C:

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym
TBOXLIB   = tbox:o.toolboxlib
WIMPLIB   = tbox:o.wimplib

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
	o.antiTwit
#
# Rule patterns
#

.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $<
.s.o:;	$(ASM) $< $@

#
# Main rules:
#
#
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
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} $(WIMPLIB) ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM} \
        	-map > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#---------------------------------------------------------------------------
# Dynamic dependencies:
