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
RAM_MODULE = rm.${COMPONENT}
ROM_MODULE = aof.${COMPONENT}

FULL_COMPONENT = JavaSupport

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
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

ROM_OBJS	= or.DynamicA or.JavaSpt
RAM_OBJS	= o.DynamicA o.JavaSpt

#
# Rule patterns
#

.SUFFIXES:	.o .od .or .s .c .cmhg .h
.s.o:;	$(ASM) $< $@

#
# Main rules:
#
#
all: ${RAM_MODULE} o.dirs
        @echo ${COMPONENT}: Module built (RAM)

rom: ${ROM_MODULE} o.dirs
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} or.* ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

o.dirs:
	${MKDIR} o
	${MKDIR} or
	create $@

#
# Targets
#
${RAM_MODULE}:	${RAM_OBJS} ${CLIB}
	${MKDIR} rm
	$(LD) -o $@ -rmf ${RAM_OBJS}
        ${MODSQZ} $@
        Access $@ RW/R

${ROM_MODULE}: ${OBJS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${ROM_OBJS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROM_MODULE}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#---------------------------------------------------------------------------
# Dynamic dependencies:
