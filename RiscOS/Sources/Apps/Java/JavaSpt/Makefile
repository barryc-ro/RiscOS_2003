# Makefile for Java Support module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 12/05/97  RLougher Created
#

#
# Component specific options:
#
COMPONENT  = JavaSpt
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

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym

OBJS =\
	o.DynamicA \
	o.JavaSpt
	
#
# Rule patterns
#

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
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROM_MODULE}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#---------------------------------------------------------------------------
# Dynamic dependencies:
