# Makefile for TextGadget
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   	Description
# ----       ----   	-----------
# 12/05/97  RLougher  	Created
# 04/06/98  SBrodie   	Added RAM build into same Makefile
# 11/08/98  MRobertshaw Updated file for Funai II Build

#
# Component specific options:
#
COMPONENT  = TextGadget
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}

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

CFLAGS   = -c -depend !Depend ${INCLUDES} -zM -ffah -zps1 ${DFLAGS}
#INCLUDES = -IC:tboxlibint,C:tboxlibs,C:
INCLUDES = -Itbox:,C:

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

OBJS =\
	o.Modhdr \
	o.TextGadget \
	o.TextArea \
	o.ScrollList \
	o.Scrollbar \
	o.riscos_uti \
	o.riscos_gra \
	o.Font \
	o.debug \
	o.TextMan \
	o.MemMan \
	o.Text \
	o.ScrollLisS \
	o.Utils \
	o.TAsel_ven \
	o.glib \
	o.glib3 \
	o.rmensure \
	o.string32

RAM_OBJS =\
	o.Modhdr \
	o.TextGadget \
	o.TextArea \
	o.ScrollList \
	o.Scrollbar \
	o.riscos_uti \
	o.riscos_gra \
	o.Font \
	o.debug \
	o.TextMan \
	o.MemMan \
	o.Text \
	o.ScrollLisS \
	o.Utils \
	o.TAsel_ven \
	o.glib \
	o.glib3 \
	o.rmensure \
	o.string32

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -p -o $@ $<
.s.o:;    $(ASM) $< $@

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
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${TBOXLIB} $(WIMPLIB) ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${TBOXLIB} $(WIMPLIB) ${ROMCSTUBS}

${RAM_MODULE}: ${RAM_OBJS} ${TBOXLIB} ${WIMPLIB} ${CLIB}
	${mkdir} rm
        ${LD} -o $@ -module ${RAM_OBJS} ${TBOXLIB} ${WIMPLIB} ${CLIB}
        Access $@ RW/R


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
