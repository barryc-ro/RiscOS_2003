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

CFLAGS   = -c -depend !Depend ${INCLUDES} -zM -ffa -zps1
INCLUDES = -I<Lib$Dir>.tboxlibs.,C:

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.c_abssym
TBOXLIB   = <Lib$Dir>.tboxlibs.o.toolboxlib
WIMPLIB   = <Lib$Dir>.tboxlibs.o.wimplib

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
o.TextGadget:	c.TextGadget
o.TextGadget:	C:h.string
o.TextGadget:	C:h.kernel
o.TextGadget:	C:h.swis
o.TextGadget:	<Lib$Dir>.tboxlibs.h.toolbox
o.TextGadget:	<Lib$Dir>.tboxlibs.h.wimp
o.TextGadget:	<Lib$Dir>.tboxlibs.h.window
o.TextGadget:	<Lib$Dir>.tboxlibs.h.gadgets
o.TextGadget:	<Lib$Dir>.tboxlibs.h.wimplib
o.TextGadget:	h.TextGadgets
o.TextGadget:	h.myswis
o.TextGadget:	h.glib
o.TextGadget:	C:h.kernel
o.TextGadget:	h.rmensure
o.TextGadget:	h.TextArea
o.TextGadget:	h.TextAreaP
o.TextGadget:	h.modmem
o.TextGadget:	h.TextArea
o.TextGadget:	h.TextMan
o.TextGadget:	h.MemMan
o.TextGadget:	C:h.kernel
o.TextGadget:	h.ScrollList
o.TextGadget:	h.ScrollLisP
o.TextGadget:	h.modmem
o.TextGadget:	h.ScrollList
o.TextGadget:	h.Scrollbar
o.TextGadget:	h.ScrollbarP
o.TextGadget:	h.modmem
o.TextGadget:	h.Scrollbar
o.TextGadget:	h.riscos_utils
o.TextGadget:	<Lib$Dir>.tboxlibs.h.toolbox
o.TextGadget:	h.modmem
o.TextArea:	c.TextArea
o.TextArea:	C:h.kernel
o.TextArea:	C:h.swis
o.TextArea:	<Lib$Dir>.tboxlibs.h.toolbox
o.TextArea:	<Lib$Dir>.tboxlibs.h.wimp
o.TextArea:	<Lib$Dir>.tboxlibs.h.window
o.TextArea:	<Lib$Dir>.tboxlibs.h.gadgets
o.TextArea:	<Lib$Dir>.tboxlibs.h.wimplib
o.TextArea:	h.glib
o.TextArea:	C:h.kernel
o.TextArea:	h.string32
o.TextArea:	h.riscos_utils
o.TextArea:	<Lib$Dir>.tboxlibs.h.toolbox
o.TextArea:	h.TextGadgets
o.TextArea:	h.TextArea
o.TextArea:	h.TextAreaP
o.TextArea:	h.modmem
o.TextArea:	h.TextArea
o.TextArea:	h.TextMan
o.TextArea:	h.MemMan
o.TextArea:	C:h.kernel
o.TextArea:	h.Font
o.TextArea:	h.Text
o.TextArea:	h.Scrollbar
o.TextArea:	h.ScrollbarP
o.TextArea:	h.modmem
o.TextArea:	h.Scrollbar
o.TextArea:	h.myswis
o.TextArea:	h.utils
o.ScrollList:	c.ScrollList
o.ScrollList:	C:h.kernel
o.ScrollList:	C:h.swis
o.ScrollList:	<Lib$Dir>.tboxlibs.h.toolbox
o.ScrollList:	<Lib$Dir>.tboxlibs.h.wimp
o.ScrollList:	<Lib$Dir>.tboxlibs.h.window
o.ScrollList:	<Lib$Dir>.tboxlibs.h.gadgets
o.ScrollList:	<Lib$Dir>.tboxlibs.h.wimplib
o.ScrollList:	h.glib
o.ScrollList:	C:h.kernel
o.ScrollList:	h.string32
o.ScrollList:	h.riscos_utils
o.ScrollList:	<Lib$Dir>.tboxlibs.h.toolbox
o.ScrollList:	h.Font
o.ScrollList:	h.TextGadgets
o.ScrollList:	h.ScrollList
o.ScrollList:	h.ScrollLisP
o.ScrollList:	h.modmem
o.ScrollList:	h.ScrollList
o.ScrollList:	h.ScrollLisS
o.ScrollList:	h.modmem
o.ScrollList:	h.ScrollList
o.ScrollList:	h.debug
o.ScrollList:	h.myswis
o.ScrollList:	h.utils
o.Scrollbar:	c.Scrollbar
o.Scrollbar:	C:h.kernel
o.Scrollbar:	C:h.swis
o.Scrollbar:	<Lib$Dir>.tboxlibs.h.toolbox
o.Scrollbar:	<Lib$Dir>.tboxlibs.h.wimp
o.Scrollbar:	<Lib$Dir>.tboxlibs.h.window
o.Scrollbar:	<Lib$Dir>.tboxlibs.h.gadgets
o.Scrollbar:	<Lib$Dir>.tboxlibs.h.wimplib
o.Scrollbar:	h.glib
o.Scrollbar:	C:h.kernel
o.Scrollbar:	h.string32
o.Scrollbar:	h.riscos_utils
o.Scrollbar:	<Lib$Dir>.tboxlibs.h.toolbox
o.Scrollbar:	h.TextGadgets
o.Scrollbar:	h.Scrollbar
o.Scrollbar:	h.ScrollbarP
o.Scrollbar:	h.modmem
o.Scrollbar:	h.Scrollbar
o.Scrollbar:	h.debug
o.Scrollbar:	h.myswis
o.Scrollbar:	h.Utils
o.riscos_uti:	c.riscos_uti
o.riscos_uti:	C:h.kernel
o.riscos_uti:	C:h.swis
o.riscos_uti:	<Lib$Dir>.tboxlibs.h.wimp
o.riscos_uti:	<Lib$Dir>.tboxlibs.h.wimplib
o.riscos_uti:	<Lib$Dir>.tboxlibs.h.toolbox
o.riscos_uti:	<Lib$Dir>.tboxlibs.h.window
o.riscos_uti:	<Lib$Dir>.tboxlibs.h.gadgets
o.riscos_uti:	h.riscos_utils
o.riscos_uti:	<Lib$Dir>.tboxlibs.h.toolbox
o.TextMan:	c.TextMan
o.TextMan:	C:h.kernel
o.TextMan:	C:h.swis
o.TextMan:	h.TextMan
o.TextMan:	h.MemMan
o.TextMan:	C:h.kernel
o.MemMan:	c.MemMan
o.MemMan:	C:h.swis
o.MemMan:	C:h.kernel
o.MemMan:	C:h.kernel
o.MemMan:	h.MemMan
o.MemMan:	C:h.kernel
o.glib:	c.glib
o.glib:	C:h.kernel
o.glib:	C:h.swis
o.glib:	h.glib
o.glib:	C:h.kernel
o.glib:	<Lib$Dir>.tboxlibs.h.wimp
o.glib:	<Lib$Dir>.tboxlibs.h.toolbox
o.glib:	<Lib$Dir>.tboxlibs.h.window
o.glib:	<Lib$Dir>.tboxlibs.h.gadgets
o.glib:	h.glibP
o.glib:	<Lib$Dir>.tboxlibs.h.toolbox
o.glib:	<Lib$Dir>.tboxlibs.h.window
o.glib:	<Lib$Dir>.tboxlibs.h.wimplib
o.glib3:	c.glib3
o.glib3:	C:h.kernel
o.glib3:	C:h.swis
o.glib3:	h.glib
o.glib3:	C:h.kernel
o.glib3:	<Lib$Dir>.tboxlibs.h.wimp
o.glib3:	<Lib$Dir>.tboxlibs.h.toolbox
o.glib3:	<Lib$Dir>.tboxlibs.h.window
o.glib3:	<Lib$Dir>.tboxlibs.h.gadgets
o.glib3:	h.glibP
o.glib3:	<Lib$Dir>.tboxlibs.h.toolbox
o.glib3:	<Lib$Dir>.tboxlibs.h.window
o.glib3:	<Lib$Dir>.tboxlibs.h.wimplib
o.rmensure:	c.rmensure
o.rmensure:	C:h.kernel
o.rmensure:	C:h.swis
o.rmensure:	h.rmensure
o.string32:	c.string32
o.string32:	h.string32
