# Makefile for TextGadget
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 12/5/97  RLougher  Created
#

#
# Component specific options:
#
COMPONENT  = JavaGadget
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
#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $<
.s.o:;    $(ASM) $< $@

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

${ROM_MODULE}: ${OBJS} ${TBOXLIB} $(WIMPLIB) ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${TBOXLIB} $(WIMPLIB) ${ROMCSTUBS}

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
