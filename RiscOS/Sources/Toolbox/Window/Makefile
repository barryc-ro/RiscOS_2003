# Makefile for Window
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 06-Jan-94  AMcC   Created
#

VPATH = @ gadgets

#
# Component specific options:
#
COMPONENT  = Window
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}

#
# Generic options:
#
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
MKDIR   = cdir
RM      = remove
WIPE    = -wipe
CD      = dir
MAKE    = amu

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

AFLAGS   = -depend !Depend -Stamp -quit -CloseExec
CFLAGS   = -c -depend !Depend ${INCLUDES} -zM -ffah -zps1 ${DFLAGS}
DFLAGS   = -DROM
INCLUDES = -I<Lib$Dir>.tboxlibint.,<Lib$Dir>.tboxlibs:,C:

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.c_abssym
TBOXLIB   = <Lib$Dir>.tboxlibint.o.tboxlibm
WIMPLIB   = <Lib$Dir>.tboxlibs.o.wimplib

OBJS =\
 o.ModNoDbg\
 o.actbut\
 o.adjuster\
 o.button\
 o.create\
 o.delete\
 o.display\
 o.draggable\
 o.events\
 o.gadgets\
 o.getstate\
 o.globals\
 o.hide\
 o.label\
 o.labelbox\
 o.main\
 o.miscop\
 o.numrange\
 o.optbut\
 o.popupmenu\
 o.radiobut\
 o.show\
 o.simple\
 o.slider\
 o.slider_ven\
 o.stringset\
 o.task\
 o.toolbar\
 o.toolbox\
 o.utils\
 o.writable

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $<
.s.o:;    ${AS} ${AFLAGS} -o $@ $<

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

resources: Resources.${LOCALE}.Messages
        ${MKDIR} ${RESDIR}
        ${CP} Resources.${LOCALE}.Messages ${RESDIR}.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${TBOXLIB} ${WIMPLIB} ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${TBOXLIB} ${WIMPLIB} ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM} \
              -map > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${WIMPLIB}:
        ${CD} ^.wimplib
        ${MAKE} @.wimplib
        back

${RAM_MODULE}: ${OBJS} ${TBOXLIB} ${WIMPLIB} ${CLIB}
	${mkdir} rm
        ${LD} -o $@ -module ${OBJS} ${TBOXLIB} ${WIMPLIB} ${CLIB}
        Access $@ RW/R

#---------------------------------------------------------------------------
# Dynamic dependencies:
o.actbut:	gadgets.c.actbut
o.actbut:	C:h.swis
o.actbut:	C:h.kernel
o.actbut:	C:h.kernel
o.actbut:	<Lib$Dir>.tboxlibint.h.const
o.actbut:	<Lib$Dir>.tboxlibint.h.macros
o.actbut:	<Lib$Dir>.tboxlibint.h.services
o.actbut:	<Lib$Dir>.tboxlibint.h.debug
o.actbut:	<Lib$Dir>.tboxlibint.h.mem
o.actbut:	<Lib$Dir>.tboxlibint.h.string32
o.actbut:	<Lib$Dir>.tboxlibint.h.messages
o.actbut:	C:h.kernel
o.actbut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.actbut:	<Lib$Dir>.tboxlibint.h.style
o.actbut:	<Lib$Dir>.tboxlibint.h.twimp
o.actbut:	C:h.kernel
o.actbut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.actbut:	<Lib$Dir>.tboxlibint.objects.h.window
o.actbut:	gadgets.^.h.globals
o.actbut:	gadgets.^.h.gadgets
o.actbut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.actbut:	<Lib$Dir>.tboxlibint.objects.h.window
o.actbut:	<Lib$Dir>.tboxlibint.h.twimp
o.actbut:	gadgets.^.h.utils
o.actbut:	<Lib$Dir>.tboxlibint.h.twimp
o.actbut:	C:h.kernel
o.actbut:	C:h.swis
o.actbut:	gadgets.^.h.object
o.actbut:	gadgets.^.h.task
o.actbut:	gadgets.^.h.object
o.actbut:	gadgets.^.h.veneers
o.actbut:	gadgets.h.actbut
o.actbut:	gadgets.h.simple
o.actbut:	h.main
o.actbut:	C:h.kernel
o.adjuster:	gadgets.c.adjuster
o.adjuster:	C:h.swis
o.adjuster:	C:h.kernel
o.adjuster:	C:h.kernel
o.adjuster:	<Lib$Dir>.tboxlibint.h.const
o.adjuster:	<Lib$Dir>.tboxlibint.h.macros
o.adjuster:	<Lib$Dir>.tboxlibint.h.services
o.adjuster:	<Lib$Dir>.tboxlibint.h.debug
o.adjuster:	<Lib$Dir>.tboxlibint.h.mem
o.adjuster:	<Lib$Dir>.tboxlibint.h.string32
o.adjuster:	<Lib$Dir>.tboxlibint.h.messages
o.adjuster:	C:h.kernel
o.adjuster:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.adjuster:	<Lib$Dir>.tboxlibint.h.style
o.adjuster:	<Lib$Dir>.tboxlibint.h.twimp
o.adjuster:	C:h.kernel
o.adjuster:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.adjuster:	<Lib$Dir>.tboxlibint.objects.h.window
o.adjuster:	gadgets.^.h.globals
o.adjuster:	gadgets.^.h.gadgets
o.adjuster:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.adjuster:	<Lib$Dir>.tboxlibint.objects.h.window
o.adjuster:	<Lib$Dir>.tboxlibint.h.twimp
o.adjuster:	gadgets.^.h.utils
o.adjuster:	<Lib$Dir>.tboxlibint.h.twimp
o.adjuster:	C:h.kernel
o.adjuster:	C:h.swis
o.adjuster:	gadgets.^.h.object
o.adjuster:	gadgets.^.h.task
o.adjuster:	gadgets.^.h.object
o.adjuster:	gadgets.h.adjuster
o.button:	gadgets.c.button
o.button:	C:h.swis
o.button:	C:h.kernel
o.button:	C:h.kernel
o.button:	<Lib$Dir>.tboxlibint.h.const
o.button:	<Lib$Dir>.tboxlibint.h.macros
o.button:	<Lib$Dir>.tboxlibint.h.services
o.button:	<Lib$Dir>.tboxlibint.h.debug
o.button:	<Lib$Dir>.tboxlibint.h.mem
o.button:	<Lib$Dir>.tboxlibint.h.string32
o.button:	<Lib$Dir>.tboxlibint.h.messages
o.button:	C:h.kernel
o.button:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.button:	<Lib$Dir>.tboxlibint.h.style
o.button:	<Lib$Dir>.tboxlibint.h.twimp
o.button:	C:h.kernel
o.button:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.button:	<Lib$Dir>.tboxlibint.objects.h.window
o.button:	gadgets.^.h.globals
o.button:	gadgets.^.h.gadgets
o.button:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.button:	<Lib$Dir>.tboxlibint.objects.h.window
o.button:	<Lib$Dir>.tboxlibint.h.twimp
o.button:	gadgets.^.h.utils
o.button:	<Lib$Dir>.tboxlibint.h.twimp
o.button:	C:h.kernel
o.button:	C:h.swis
o.button:	gadgets.^.h.object
o.button:	gadgets.^.h.task
o.button:	gadgets.^.h.object
o.button:	gadgets.^.h.main
o.button:	C:h.kernel
o.button:	gadgets.h.button
o.create:	c.create
o.create:	C:h.kernel
o.create:	C:h.swis
o.create:	<Lib$Dir>.tboxlibint.h.const
o.create:	<Lib$Dir>.tboxlibint.h.macros
o.create:	<Lib$Dir>.tboxlibint.h.services
o.create:	<Lib$Dir>.tboxlibint.h.debug
o.create:	<Lib$Dir>.tboxlibint.h.mem
o.create:	<Lib$Dir>.tboxlibint.h.string32
o.create:	<Lib$Dir>.tboxlibint.h.messages
o.create:	C:h.kernel
o.create:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.create:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.create:	<Lib$Dir>.tboxlibint.objects.h.window
o.create:	<Lib$Dir>.tboxlibint.h.twimp
o.create:	C:h.kernel
o.create:	h.object
o.create:	h.task
o.create:	h.gadgets
o.create:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.create:	<Lib$Dir>.tboxlibint.objects.h.window
o.create:	<Lib$Dir>.tboxlibint.h.twimp
o.create:	h.utils
o.create:	<Lib$Dir>.tboxlibint.h.twimp
o.create:	C:h.kernel
o.create:	C:h.swis
o.create:	h.toolbar
o.create:	C:h.kernel
o.create:	h.create
o.create:	C:h.kernel
o.create:	h.utils
o.create:	<Lib$Dir>.tboxlibint.h.twimp
o.create:	C:h.kernel
o.create:	C:h.swis
o.delete:	c.delete
o.delete:	C:h.kernel
o.delete:	C:h.swis
o.delete:	<Lib$Dir>.tboxlibint.h.const
o.delete:	<Lib$Dir>.tboxlibint.h.macros
o.delete:	<Lib$Dir>.tboxlibint.h.services
o.delete:	<Lib$Dir>.tboxlibint.h.debug
o.delete:	<Lib$Dir>.tboxlibint.h.mem
o.delete:	<Lib$Dir>.tboxlibint.h.string32
o.delete:	<Lib$Dir>.tboxlibint.h.messages
o.delete:	C:h.kernel
o.delete:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.delete:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.delete:	<Lib$Dir>.tboxlibint.objects.h.window
o.delete:	<Lib$Dir>.tboxlibint.h.twimp
o.delete:	C:h.kernel
o.delete:	h.object
o.delete:	h.task
o.delete:	h.gadgets
o.delete:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.delete:	<Lib$Dir>.tboxlibint.objects.h.window
o.delete:	<Lib$Dir>.tboxlibint.h.twimp
o.delete:	h.utils
o.delete:	<Lib$Dir>.tboxlibint.h.twimp
o.delete:	C:h.kernel
o.delete:	C:h.swis
o.delete:	h.delete
o.delete:	C:h.kernel
o.display:	gadgets.c.display
o.display:	C:h.swis
o.display:	C:h.kernel
o.display:	C:h.kernel
o.display:	<Lib$Dir>.tboxlibint.h.const
o.display:	<Lib$Dir>.tboxlibint.h.macros
o.display:	<Lib$Dir>.tboxlibint.h.services
o.display:	<Lib$Dir>.tboxlibint.h.debug
o.display:	<Lib$Dir>.tboxlibint.h.mem
o.display:	<Lib$Dir>.tboxlibint.h.string32
o.display:	<Lib$Dir>.tboxlibint.h.messages
o.display:	C:h.kernel
o.display:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.display:	<Lib$Dir>.tboxlibint.h.style
o.display:	<Lib$Dir>.tboxlibint.h.twimp
o.display:	C:h.kernel
o.display:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.display:	<Lib$Dir>.tboxlibint.objects.h.window
o.display:	gadgets.^.h.globals
o.display:	gadgets.^.h.gadgets
o.display:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.display:	<Lib$Dir>.tboxlibint.objects.h.window
o.display:	<Lib$Dir>.tboxlibint.h.twimp
o.display:	gadgets.^.h.utils
o.display:	<Lib$Dir>.tboxlibint.h.twimp
o.display:	C:h.kernel
o.display:	C:h.swis
o.display:	gadgets.^.h.object
o.display:	gadgets.^.h.task
o.display:	gadgets.^.h.object
o.display:	gadgets.h.display
o.draggable:	gadgets.c.draggable
o.draggable:	C:h.kernel
o.draggable:	C:h.swis
o.draggable:	<Lib$Dir>.tboxlibint.h.const
o.draggable:	<Lib$Dir>.tboxlibint.h.macros
o.draggable:	<Lib$Dir>.tboxlibint.h.services
o.draggable:	<Lib$Dir>.tboxlibint.h.debug
o.draggable:	<Lib$Dir>.tboxlibint.h.mem
o.draggable:	<Lib$Dir>.tboxlibint.h.string32
o.draggable:	<Lib$Dir>.tboxlibint.h.messages
o.draggable:	C:h.kernel
o.draggable:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.draggable:	<Lib$Dir>.tboxlibint.h.style
o.draggable:	<Lib$Dir>.tboxlibint.h.twimp
o.draggable:	C:h.kernel
o.draggable:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.draggable:	<Lib$Dir>.tboxlibint.objects.h.window
o.draggable:	gadgets.^.h.globals
o.draggable:	gadgets.^.h.gadgets
o.draggable:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.draggable:	<Lib$Dir>.tboxlibint.objects.h.window
o.draggable:	<Lib$Dir>.tboxlibint.h.twimp
o.draggable:	gadgets.^.h.utils
o.draggable:	<Lib$Dir>.tboxlibint.h.twimp
o.draggable:	C:h.kernel
o.draggable:	C:h.swis
o.draggable:	gadgets.^.h.object
o.draggable:	gadgets.^.h.task
o.draggable:	gadgets.^.h.object
o.draggable:	gadgets.^.h.main
o.draggable:	C:h.kernel
o.draggable:	gadgets.h.draggable
o.events:	c.events
o.events:	C:h.kernel
o.events:	C:h.swis
o.events:	<Lib$Dir>.tboxlibint.h.const
o.events:	<Lib$Dir>.tboxlibint.h.macros
o.events:	<Lib$Dir>.tboxlibint.h.services
o.events:	<Lib$Dir>.tboxlibint.h.debug
o.events:	<Lib$Dir>.tboxlibint.h.mem
o.events:	<Lib$Dir>.tboxlibint.h.string32
o.events:	<Lib$Dir>.tboxlibint.h.messages
o.events:	C:h.kernel
o.events:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.events:	<Lib$Dir>.tboxlibint.h.style
o.events:	<Lib$Dir>.tboxlibint.h.twimp
o.events:	C:h.kernel
o.events:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.events:	<Lib$Dir>.tboxlibint.objects.h.window
o.events:	h.object
o.events:	h.events
o.events:	C:h.kernel
o.events:	h.task
o.events:	h.gadgets
o.events:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.events:	<Lib$Dir>.tboxlibint.objects.h.window
o.events:	<Lib$Dir>.tboxlibint.h.twimp
o.events:	h.utils
o.events:	<Lib$Dir>.tboxlibint.h.twimp
o.events:	C:h.kernel
o.events:	C:h.swis
o.events:	h.show
o.events:	C:h.kernel
o.events:	h.hide
o.events:	C:h.kernel
o.events:	h.veneers
o.events:	h.events
o.gadgets:	c.gadgets
o.gadgets:	C:h.swis
o.gadgets:	C:h.kernel
o.gadgets:	C:h.kernel
o.gadgets:	<Lib$Dir>.tboxlibint.h.const
o.gadgets:	<Lib$Dir>.tboxlibint.h.macros
o.gadgets:	<Lib$Dir>.tboxlibint.h.services
o.gadgets:	<Lib$Dir>.tboxlibint.h.debug
o.gadgets:	<Lib$Dir>.tboxlibint.h.mem
o.gadgets:	<Lib$Dir>.tboxlibint.h.string32
o.gadgets:	<Lib$Dir>.tboxlibint.h.messages
o.gadgets:	C:h.kernel
o.gadgets:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.gadgets:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.gadgets:	<Lib$Dir>.tboxlibint.objects.h.window
o.gadgets:	<Lib$Dir>.tboxlibint.h.twimp
o.gadgets:	C:h.kernel
o.gadgets:	h.globals
o.gadgets:	h.gadgets
o.gadgets:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.gadgets:	<Lib$Dir>.tboxlibint.objects.h.window
o.gadgets:	<Lib$Dir>.tboxlibint.h.twimp
o.gadgets:	h.utils
o.gadgets:	<Lib$Dir>.tboxlibint.h.twimp
o.gadgets:	C:h.kernel
o.gadgets:	C:h.swis
o.gadgets:	h.object
o.gadgets:	h.task
o.gadgets:	h.object
o.gadgets:	h.miscop
o.gadgets:	C:h.kernel
o.gadgets:	h.veneers
o.gadgets:	h.utils
o.gadgets:	<Lib$Dir>.tboxlibint.h.twimp
o.gadgets:	C:h.kernel
o.gadgets:	C:h.swis
o.gadgets:	h.events
o.gadgets:	C:h.kernel
o.gadgets:	gadgets.h.actbut
o.gadgets:	gadgets.h.adjuster
o.gadgets:	gadgets.h.display
o.gadgets:	gadgets.h.draggable
o.gadgets:	gadgets.h.label
o.gadgets:	gadgets.h.labelbox
o.gadgets:	gadgets.h.numrange
o.gadgets:	gadgets.h.optbut
o.gadgets:	gadgets.h.popupmenu
o.gadgets:	gadgets.h.radiobut
o.gadgets:	gadgets.h.slider
o.gadgets:	gadgets.h.stringset
o.gadgets:	gadgets.h.button
o.gadgets:	gadgets.h.writable
o.gadgets:	gadgets.h.simple
o.getstate:	c.getstate
o.getstate:	C:h.kernel
o.getstate:	C:h.swis
o.getstate:	<Lib$Dir>.tboxlibint.h.const
o.getstate:	<Lib$Dir>.tboxlibint.h.macros
o.getstate:	<Lib$Dir>.tboxlibint.h.services
o.getstate:	<Lib$Dir>.tboxlibint.h.debug
o.getstate:	<Lib$Dir>.tboxlibint.h.mem
o.getstate:	<Lib$Dir>.tboxlibint.h.string32
o.getstate:	<Lib$Dir>.tboxlibint.h.messages
o.getstate:	C:h.kernel
o.getstate:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.getstate:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.getstate:	<Lib$Dir>.tboxlibint.objects.h.window
o.getstate:	<Lib$Dir>.tboxlibint.h.twimp
o.getstate:	C:h.kernel
o.getstate:	h.object
o.getstate:	h.task
o.getstate:	h.getstate
o.getstate:	C:h.kernel
o.globals:	c.globals
o.globals:	<Lib$Dir>.tboxlibint.h.const
o.globals:	<Lib$Dir>.tboxlibint.h.macros
o.globals:	<Lib$Dir>.tboxlibint.h.services
o.globals:	<Lib$Dir>.tboxlibint.h.debug
o.globals:	<Lib$Dir>.tboxlibint.h.mem
o.globals:	<Lib$Dir>.tboxlibint.h.string32
o.globals:	<Lib$Dir>.tboxlibint.h.messages
o.globals:	C:h.kernel
o.globals:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.globals:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.globals:	<Lib$Dir>.tboxlibint.objects.h.window
o.globals:	<Lib$Dir>.tboxlibint.h.twimp
o.globals:	C:h.kernel
o.globals:	<Lib$Dir>.tboxlibint.objects.h.menu
o.globals:	h.globals
o.globals:	C:h.kernel
o.hide:	c.hide
o.hide:	C:h.kernel
o.hide:	C:h.swis
o.hide:	<Lib$Dir>.tboxlibint.h.const
o.hide:	<Lib$Dir>.tboxlibint.h.macros
o.hide:	<Lib$Dir>.tboxlibint.h.services
o.hide:	<Lib$Dir>.tboxlibint.h.debug
o.hide:	<Lib$Dir>.tboxlibint.h.mem
o.hide:	<Lib$Dir>.tboxlibint.h.string32
o.hide:	<Lib$Dir>.tboxlibint.h.messages
o.hide:	C:h.kernel
o.hide:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.hide:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.hide:	<Lib$Dir>.tboxlibint.objects.h.window
o.hide:	<Lib$Dir>.tboxlibint.h.twimp
o.hide:	C:h.kernel
o.hide:	h.object
o.hide:	h.task
o.hide:	h.toolbar
o.hide:	C:h.kernel
o.hide:	h.events
o.hide:	C:h.kernel
o.hide:	h.hide
o.hide:	C:h.kernel
o.label:	gadgets.c.label
o.label:	C:h.swis
o.label:	C:h.kernel
o.label:	C:h.kernel
o.label:	<Lib$Dir>.tboxlibint.h.const
o.label:	<Lib$Dir>.tboxlibint.h.macros
o.label:	<Lib$Dir>.tboxlibint.h.services
o.label:	<Lib$Dir>.tboxlibint.h.debug
o.label:	<Lib$Dir>.tboxlibint.h.mem
o.label:	<Lib$Dir>.tboxlibint.h.string32
o.label:	<Lib$Dir>.tboxlibint.h.messages
o.label:	C:h.kernel
o.label:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.label:	<Lib$Dir>.tboxlibint.h.style
o.label:	<Lib$Dir>.tboxlibint.h.twimp
o.label:	C:h.kernel
o.label:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.label:	<Lib$Dir>.tboxlibint.objects.h.window
o.label:	gadgets.^.h.globals
o.label:	gadgets.^.h.gadgets
o.label:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.label:	<Lib$Dir>.tboxlibint.objects.h.window
o.label:	<Lib$Dir>.tboxlibint.h.twimp
o.label:	gadgets.^.h.utils
o.label:	<Lib$Dir>.tboxlibint.h.twimp
o.label:	C:h.kernel
o.label:	C:h.swis
o.label:	gadgets.^.h.object
o.label:	gadgets.^.h.task
o.label:	gadgets.^.h.object
o.label:	gadgets.h.label
o.labelbox:	gadgets.c.labelbox
o.labelbox:	C:h.swis
o.labelbox:	C:h.kernel
o.labelbox:	C:h.kernel
o.labelbox:	<Lib$Dir>.tboxlibint.h.const
o.labelbox:	<Lib$Dir>.tboxlibint.h.macros
o.labelbox:	<Lib$Dir>.tboxlibint.h.services
o.labelbox:	<Lib$Dir>.tboxlibint.h.debug
o.labelbox:	<Lib$Dir>.tboxlibint.h.mem
o.labelbox:	<Lib$Dir>.tboxlibint.h.string32
o.labelbox:	<Lib$Dir>.tboxlibint.h.messages
o.labelbox:	C:h.kernel
o.labelbox:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.labelbox:	<Lib$Dir>.tboxlibint.h.style
o.labelbox:	<Lib$Dir>.tboxlibint.h.twimp
o.labelbox:	C:h.kernel
o.labelbox:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.labelbox:	<Lib$Dir>.tboxlibint.objects.h.window
o.labelbox:	gadgets.^.h.globals
o.labelbox:	gadgets.^.h.gadgets
o.labelbox:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.labelbox:	<Lib$Dir>.tboxlibint.objects.h.window
o.labelbox:	<Lib$Dir>.tboxlibint.h.twimp
o.labelbox:	gadgets.^.h.utils
o.labelbox:	<Lib$Dir>.tboxlibint.h.twimp
o.labelbox:	C:h.kernel
o.labelbox:	C:h.swis
o.labelbox:	gadgets.^.h.object
o.labelbox:	gadgets.^.h.task
o.labelbox:	gadgets.^.h.object
o.labelbox:	gadgets.^.h.main
o.labelbox:	C:h.kernel
o.labelbox:	gadgets.^.h.veneers
o.labelbox:	gadgets.h.labelbox
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	<Lib$Dir>.tboxlibint.h.const
o.main:	<Lib$Dir>.tboxlibint.h.macros
o.main:	<Lib$Dir>.tboxlibint.h.services
o.main:	<Lib$Dir>.tboxlibint.h.debug
o.main:	<Lib$Dir>.tboxlibint.h.mem
o.main:	<Lib$Dir>.tboxlibint.h.string32
o.main:	<Lib$Dir>.tboxlibint.h.messages
o.main:	C:h.kernel
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	<Lib$Dir>.tboxlibint.h.rmensure
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	<Lib$Dir>.tboxlibint.objects.h.window
o.main:	<Lib$Dir>.tboxlibint.h.twimp
o.main:	C:h.kernel
o.main:	h.object
o.main:	h.create
o.main:	C:h.kernel
o.main:	h.task
o.main:	h.delete
o.main:	C:h.kernel
o.main:	h.show
o.main:	C:h.kernel
o.main:	h.hide
o.main:	C:h.kernel
o.main:	h.getstate
o.main:	C:h.kernel
o.main:	h.miscop
o.main:	C:h.kernel
o.main:	h.events
o.main:	C:h.kernel
o.main:	h.gadgets
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	<Lib$Dir>.tboxlibint.objects.h.window
o.main:	<Lib$Dir>.tboxlibint.h.twimp
o.main:	h.utils
o.main:	<Lib$Dir>.tboxlibint.h.twimp
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.veneers
o.main:	h.task
o.main:	h.main
o.main:	C:h.kernel
o.miscop:	c.miscop
o.miscop:	C:h.kernel
o.miscop:	C:h.swis
o.miscop:	<Lib$Dir>.tboxlibint.h.const
o.miscop:	<Lib$Dir>.tboxlibint.h.macros
o.miscop:	<Lib$Dir>.tboxlibint.h.services
o.miscop:	<Lib$Dir>.tboxlibint.h.debug
o.miscop:	<Lib$Dir>.tboxlibint.h.mem
o.miscop:	<Lib$Dir>.tboxlibint.h.string32
o.miscop:	<Lib$Dir>.tboxlibint.h.messages
o.miscop:	C:h.kernel
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.window
o.miscop:	<Lib$Dir>.tboxlibint.h.twimp
o.miscop:	C:h.kernel
o.miscop:	h.object
o.miscop:	h.task
o.miscop:	h.gadgets
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.window
o.miscop:	<Lib$Dir>.tboxlibint.h.twimp
o.miscop:	h.utils
o.miscop:	<Lib$Dir>.tboxlibint.h.twimp
o.miscop:	C:h.kernel
o.miscop:	C:h.swis
o.miscop:	h.events
o.miscop:	C:h.kernel
o.miscop:	h.toolbar
o.miscop:	C:h.kernel
o.miscop:	h.delete
o.miscop:	C:h.kernel
o.miscop:	h.miscop
o.miscop:	C:h.kernel
o.miscop:	h.utils
o.miscop:	<Lib$Dir>.tboxlibint.h.twimp
o.miscop:	C:h.kernel
o.miscop:	C:h.swis
o.numrange:	gadgets.c.numrange
o.numrange:	C:h.swis
o.numrange:	C:h.kernel
o.numrange:	C:h.kernel
o.numrange:	<Lib$Dir>.tboxlibint.h.const
o.numrange:	<Lib$Dir>.tboxlibint.h.macros
o.numrange:	<Lib$Dir>.tboxlibint.h.services
o.numrange:	<Lib$Dir>.tboxlibint.h.debug
o.numrange:	<Lib$Dir>.tboxlibint.h.mem
o.numrange:	<Lib$Dir>.tboxlibint.h.string32
o.numrange:	<Lib$Dir>.tboxlibint.h.messages
o.numrange:	C:h.kernel
o.numrange:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.numrange:	<Lib$Dir>.tboxlibint.h.style
o.numrange:	<Lib$Dir>.tboxlibint.h.twimp
o.numrange:	C:h.kernel
o.numrange:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.numrange:	<Lib$Dir>.tboxlibint.objects.h.window
o.numrange:	gadgets.^.h.globals
o.numrange:	gadgets.^.h.gadgets
o.numrange:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.numrange:	<Lib$Dir>.tboxlibint.objects.h.window
o.numrange:	<Lib$Dir>.tboxlibint.h.twimp
o.numrange:	gadgets.^.h.utils
o.numrange:	<Lib$Dir>.tboxlibint.h.twimp
o.numrange:	C:h.kernel
o.numrange:	C:h.swis
o.numrange:	gadgets.^.h.object
o.numrange:	gadgets.^.h.task
o.numrange:	gadgets.^.h.object
o.numrange:	gadgets.^.h.main
o.numrange:	C:h.kernel
o.numrange:	gadgets.h.numrange
o.numrange:	gadgets.h.slider
o.optbut:	gadgets.c.optbut
o.optbut:	C:h.swis
o.optbut:	C:h.kernel
o.optbut:	C:h.kernel
o.optbut:	<Lib$Dir>.tboxlibint.h.const
o.optbut:	<Lib$Dir>.tboxlibint.h.macros
o.optbut:	<Lib$Dir>.tboxlibint.h.services
o.optbut:	<Lib$Dir>.tboxlibint.h.debug
o.optbut:	<Lib$Dir>.tboxlibint.h.mem
o.optbut:	<Lib$Dir>.tboxlibint.h.string32
o.optbut:	<Lib$Dir>.tboxlibint.h.messages
o.optbut:	C:h.kernel
o.optbut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.optbut:	<Lib$Dir>.tboxlibint.h.style
o.optbut:	<Lib$Dir>.tboxlibint.h.twimp
o.optbut:	C:h.kernel
o.optbut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.optbut:	<Lib$Dir>.tboxlibint.objects.h.window
o.optbut:	gadgets.^.h.globals
o.optbut:	gadgets.^.h.gadgets
o.optbut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.optbut:	<Lib$Dir>.tboxlibint.objects.h.window
o.optbut:	<Lib$Dir>.tboxlibint.h.twimp
o.optbut:	gadgets.^.h.utils
o.optbut:	<Lib$Dir>.tboxlibint.h.twimp
o.optbut:	C:h.kernel
o.optbut:	C:h.swis
o.optbut:	gadgets.^.h.object
o.optbut:	gadgets.^.h.task
o.optbut:	gadgets.^.h.object
o.optbut:	gadgets.^.h.veneers
o.optbut:	gadgets.^.h.main
o.optbut:	C:h.kernel
o.optbut:	gadgets.h.optbut
o.popupmenu:	gadgets.c.popupmenu
o.popupmenu:	C:h.swis
o.popupmenu:	C:h.kernel
o.popupmenu:	C:h.kernel
o.popupmenu:	<Lib$Dir>.tboxlibint.h.const
o.popupmenu:	<Lib$Dir>.tboxlibint.h.macros
o.popupmenu:	<Lib$Dir>.tboxlibint.h.services
o.popupmenu:	<Lib$Dir>.tboxlibint.h.debug
o.popupmenu:	<Lib$Dir>.tboxlibint.h.mem
o.popupmenu:	<Lib$Dir>.tboxlibint.h.string32
o.popupmenu:	<Lib$Dir>.tboxlibint.h.messages
o.popupmenu:	C:h.kernel
o.popupmenu:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.popupmenu:	<Lib$Dir>.tboxlibint.h.style
o.popupmenu:	<Lib$Dir>.tboxlibint.h.twimp
o.popupmenu:	C:h.kernel
o.popupmenu:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.popupmenu:	<Lib$Dir>.tboxlibint.objects.h.window
o.popupmenu:	<Lib$Dir>.tboxlibint.objects.h.menu
o.popupmenu:	gadgets.^.h.globals
o.popupmenu:	gadgets.^.h.gadgets
o.popupmenu:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.popupmenu:	<Lib$Dir>.tboxlibint.objects.h.window
o.popupmenu:	<Lib$Dir>.tboxlibint.h.twimp
o.popupmenu:	gadgets.^.h.utils
o.popupmenu:	<Lib$Dir>.tboxlibint.h.twimp
o.popupmenu:	C:h.kernel
o.popupmenu:	C:h.swis
o.popupmenu:	gadgets.^.h.object
o.popupmenu:	gadgets.^.h.task
o.popupmenu:	gadgets.^.h.object
o.popupmenu:	gadgets.^.h.veneers
o.popupmenu:	gadgets.h.popupmenu
o.radiobut:	gadgets.c.radiobut
o.radiobut:	C:h.swis
o.radiobut:	C:h.kernel
o.radiobut:	C:h.kernel
o.radiobut:	<Lib$Dir>.tboxlibint.h.const
o.radiobut:	<Lib$Dir>.tboxlibint.h.macros
o.radiobut:	<Lib$Dir>.tboxlibint.h.services
o.radiobut:	<Lib$Dir>.tboxlibint.h.debug
o.radiobut:	<Lib$Dir>.tboxlibint.h.mem
o.radiobut:	<Lib$Dir>.tboxlibint.h.string32
o.radiobut:	<Lib$Dir>.tboxlibint.h.messages
o.radiobut:	C:h.kernel
o.radiobut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.radiobut:	<Lib$Dir>.tboxlibint.h.style
o.radiobut:	<Lib$Dir>.tboxlibint.h.twimp
o.radiobut:	C:h.kernel
o.radiobut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.radiobut:	<Lib$Dir>.tboxlibint.objects.h.window
o.radiobut:	gadgets.^.h.globals
o.radiobut:	gadgets.^.h.gadgets
o.radiobut:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.radiobut:	<Lib$Dir>.tboxlibint.objects.h.window
o.radiobut:	<Lib$Dir>.tboxlibint.h.twimp
o.radiobut:	gadgets.^.h.utils
o.radiobut:	<Lib$Dir>.tboxlibint.h.twimp
o.radiobut:	C:h.kernel
o.radiobut:	C:h.swis
o.radiobut:	gadgets.^.h.object
o.radiobut:	gadgets.^.h.task
o.radiobut:	gadgets.^.h.object
o.radiobut:	gadgets.^.h.veneers
o.radiobut:	gadgets.^.h.main
o.radiobut:	C:h.kernel
o.radiobut:	gadgets.h.radiobut
o.show:	c.show
o.show:	C:h.kernel
o.show:	C:h.swis
o.show:	<Lib$Dir>.tboxlibint.h.const
o.show:	<Lib$Dir>.tboxlibint.h.macros
o.show:	<Lib$Dir>.tboxlibint.h.services
o.show:	<Lib$Dir>.tboxlibint.h.debug
o.show:	<Lib$Dir>.tboxlibint.h.mem
o.show:	<Lib$Dir>.tboxlibint.h.string32
o.show:	<Lib$Dir>.tboxlibint.h.messages
o.show:	C:h.kernel
o.show:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.show:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.show:	<Lib$Dir>.tboxlibint.objects.h.window
o.show:	<Lib$Dir>.tboxlibint.h.twimp
o.show:	C:h.kernel
o.show:	h.object
o.show:	h.task
o.show:	h.hide
o.show:	C:h.kernel
o.show:	h.gadgets
o.show:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.show:	<Lib$Dir>.tboxlibint.objects.h.window
o.show:	<Lib$Dir>.tboxlibint.h.twimp
o.show:	h.utils
o.show:	<Lib$Dir>.tboxlibint.h.twimp
o.show:	C:h.kernel
o.show:	C:h.swis
o.show:	h.show
o.show:	C:h.kernel
o.show:	h.toolbar
o.show:	C:h.kernel
o.show:	h.main
o.show:	C:h.kernel
o.simple:	gadgets.c.simple
o.simple:	C:h.swis
o.simple:	C:h.kernel
o.simple:	C:h.kernel
o.simple:	<Lib$Dir>.tboxlibint.h.const
o.simple:	<Lib$Dir>.tboxlibint.h.macros
o.simple:	<Lib$Dir>.tboxlibint.h.services
o.simple:	<Lib$Dir>.tboxlibint.h.debug
o.simple:	<Lib$Dir>.tboxlibint.h.mem
o.simple:	<Lib$Dir>.tboxlibint.h.string32
o.simple:	<Lib$Dir>.tboxlibint.h.messages
o.simple:	C:h.kernel
o.simple:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.simple:	<Lib$Dir>.tboxlibint.h.style
o.simple:	<Lib$Dir>.tboxlibint.h.twimp
o.simple:	C:h.kernel
o.simple:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.simple:	<Lib$Dir>.tboxlibint.objects.h.window
o.simple:	gadgets.^.h.globals
o.simple:	gadgets.^.h.gadgets
o.simple:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.simple:	<Lib$Dir>.tboxlibint.objects.h.window
o.simple:	<Lib$Dir>.tboxlibint.h.twimp
o.simple:	gadgets.^.h.utils
o.simple:	<Lib$Dir>.tboxlibint.h.twimp
o.simple:	C:h.kernel
o.simple:	C:h.swis
o.simple:	gadgets.^.h.object
o.simple:	gadgets.^.h.task
o.simple:	gadgets.^.h.object
o.simple:	gadgets.^.h.veneers
o.simple:	gadgets.h.simple
o.slider:	gadgets.c.slider
o.slider:	C:h.swis
o.slider:	C:h.kernel
o.slider:	C:h.kernel
o.slider:	<Lib$Dir>.tboxlibint.h.const
o.slider:	<Lib$Dir>.tboxlibint.h.macros
o.slider:	<Lib$Dir>.tboxlibint.h.services
o.slider:	<Lib$Dir>.tboxlibint.h.debug
o.slider:	<Lib$Dir>.tboxlibint.h.mem
o.slider:	<Lib$Dir>.tboxlibint.h.string32
o.slider:	<Lib$Dir>.tboxlibint.h.messages
o.slider:	C:h.kernel
o.slider:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.slider:	<Lib$Dir>.tboxlibint.h.style
o.slider:	<Lib$Dir>.tboxlibint.h.twimp
o.slider:	C:h.kernel
o.slider:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.slider:	<Lib$Dir>.tboxlibint.objects.h.window
o.slider:	gadgets.^.h.globals
o.slider:	gadgets.^.h.gadgets
o.slider:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.slider:	<Lib$Dir>.tboxlibint.objects.h.window
o.slider:	<Lib$Dir>.tboxlibint.h.twimp
o.slider:	gadgets.^.h.utils
o.slider:	<Lib$Dir>.tboxlibint.h.twimp
o.slider:	C:h.kernel
o.slider:	C:h.swis
o.slider:	gadgets.^.h.object
o.slider:	gadgets.^.h.task
o.slider:	gadgets.^.h.object
o.slider:	gadgets.^.h.utils
o.slider:	<Lib$Dir>.tboxlibint.h.twimp
o.slider:	C:h.kernel
o.slider:	C:h.swis
o.slider:	gadgets.^.h.main
o.slider:	C:h.kernel
o.slider:	gadgets.h.slider
o.slider_ven: gadgets.s.slider_ven
o.stringset:	gadgets.c.stringset
o.stringset:	C:h.swis
o.stringset:	C:h.kernel
o.stringset:	C:h.kernel
o.stringset:	<Lib$Dir>.tboxlibint.h.const
o.stringset:	<Lib$Dir>.tboxlibint.h.macros
o.stringset:	<Lib$Dir>.tboxlibint.h.services
o.stringset:	<Lib$Dir>.tboxlibint.h.debug
o.stringset:	<Lib$Dir>.tboxlibint.h.mem
o.stringset:	<Lib$Dir>.tboxlibint.h.string32
o.stringset:	<Lib$Dir>.tboxlibint.h.messages
o.stringset:	C:h.kernel
o.stringset:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.stringset:	<Lib$Dir>.tboxlibint.h.style
o.stringset:	<Lib$Dir>.tboxlibint.h.twimp
o.stringset:	C:h.kernel
o.stringset:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.stringset:	<Lib$Dir>.tboxlibint.objects.h.window
o.stringset:	<Lib$Dir>.tboxlibint.objects.h.menu
o.stringset:	gadgets.^.h.globals
o.stringset:	gadgets.^.h.gadgets
o.stringset:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.stringset:	<Lib$Dir>.tboxlibint.objects.h.window
o.stringset:	<Lib$Dir>.tboxlibint.h.twimp
o.stringset:	gadgets.^.h.utils
o.stringset:	<Lib$Dir>.tboxlibint.h.twimp
o.stringset:	C:h.kernel
o.stringset:	C:h.swis
o.stringset:	gadgets.^.h.object
o.stringset:	gadgets.^.h.task
o.stringset:	gadgets.^.h.object
o.stringset:	gadgets.^.h.veneers
o.stringset:	gadgets.^.h.utils
o.stringset:	<Lib$Dir>.tboxlibint.h.twimp
o.stringset:	C:h.kernel
o.stringset:	C:h.swis
o.stringset:	gadgets.h.stringset
o.task:	c.task
o.task:	C:h.kernel
o.task:	C:h.swis
o.task:	<Lib$Dir>.tboxlibint.h.const
o.task:	<Lib$Dir>.tboxlibint.h.macros
o.task:	<Lib$Dir>.tboxlibint.h.services
o.task:	<Lib$Dir>.tboxlibint.h.debug
o.task:	<Lib$Dir>.tboxlibint.h.mem
o.task:	<Lib$Dir>.tboxlibint.h.string32
o.task:	<Lib$Dir>.tboxlibint.h.messages
o.task:	C:h.kernel
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	<Lib$Dir>.tboxlibint.objects.h.window
o.task:	<Lib$Dir>.tboxlibint.h.twimp
o.task:	C:h.kernel
o.task:	h.object
o.task:	h.globals
o.task:	h.delete
o.task:	C:h.kernel
o.task:	h.task
o.task:	h.gadgets
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	<Lib$Dir>.tboxlibint.objects.h.window
o.task:	<Lib$Dir>.tboxlibint.h.twimp
o.task:	h.utils
o.task:	<Lib$Dir>.tboxlibint.h.twimp
o.task:	C:h.kernel
o.task:	C:h.swis
o.task:	h.task
o.toolbar:	c.toolbar
o.toolbar:	C:h.kernel
o.toolbar:	C:h.swis
o.toolbar:	<Lib$Dir>.tboxlibint.h.const
o.toolbar:	<Lib$Dir>.tboxlibint.h.macros
o.toolbar:	<Lib$Dir>.tboxlibint.h.services
o.toolbar:	<Lib$Dir>.tboxlibint.h.debug
o.toolbar:	<Lib$Dir>.tboxlibint.h.mem
o.toolbar:	<Lib$Dir>.tboxlibint.h.string32
o.toolbar:	<Lib$Dir>.tboxlibint.h.messages
o.toolbar:	C:h.kernel
o.toolbar:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.toolbar:	<Lib$Dir>.tboxlibint.h.style
o.toolbar:	<Lib$Dir>.tboxlibint.h.twimp
o.toolbar:	C:h.kernel
o.toolbar:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.toolbar:	<Lib$Dir>.tboxlibint.objects.h.window
o.toolbar:	h.object
o.toolbar:	h.events
o.toolbar:	C:h.kernel
o.toolbar:	h.task
o.toolbar:	h.gadgets
o.toolbar:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.toolbar:	<Lib$Dir>.tboxlibint.objects.h.window
o.toolbar:	<Lib$Dir>.tboxlibint.h.twimp
o.toolbar:	h.utils
o.toolbar:	<Lib$Dir>.tboxlibint.h.twimp
o.toolbar:	C:h.kernel
o.toolbar:	C:h.swis
o.toolbar:	h.show
o.toolbar:	C:h.kernel
o.toolbar:	h.hide
o.toolbar:	C:h.kernel
o.toolbar:	h.veneers
o.toolbar:	h.toolbar
o.toolbar:	C:h.kernel
o.toolbox: s.toolbox
o.utils:	c.utils
o.utils:	h.utils
o.utils:	<Lib$Dir>.tboxlibint.h.twimp
o.utils:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.utils:	C:h.kernel
o.utils:	C:h.kernel
o.utils:	C:h.swis
o.utils:	<Lib$Dir>.tboxlibint.h.twimp
o.utils:	C:h.swis
o.utils:	<Lib$Dir>.tboxlibint.h.debug
o.utils:	<Lib$Dir>.tboxlibint.objects.h.window
o.utils:	<Lib$Dir>.tboxlibint.h.messages
o.utils:	C:h.kernel
o.utils:	C:h.stddef
o.writable:	gadgets.c.writable
o.writable:	C:h.swis
o.writable:	C:h.kernel
o.writable:	C:h.kernel
o.writable:	<Lib$Dir>.tboxlibint.h.const
o.writable:	<Lib$Dir>.tboxlibint.h.macros
o.writable:	<Lib$Dir>.tboxlibint.h.services
o.writable:	<Lib$Dir>.tboxlibint.h.debug
o.writable:	<Lib$Dir>.tboxlibint.h.mem
o.writable:	<Lib$Dir>.tboxlibint.h.string32
o.writable:	<Lib$Dir>.tboxlibint.h.messages
o.writable:	C:h.kernel
o.writable:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.writable:	<Lib$Dir>.tboxlibint.h.style
o.writable:	<Lib$Dir>.tboxlibint.h.twimp
o.writable:	C:h.kernel
o.writable:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.writable:	<Lib$Dir>.tboxlibint.objects.h.window
o.writable:	gadgets.^.h.globals
o.writable:	gadgets.^.h.gadgets
o.writable:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.writable:	<Lib$Dir>.tboxlibint.objects.h.window
o.writable:	<Lib$Dir>.tboxlibint.h.twimp
o.writable:	gadgets.^.h.utils
o.writable:	<Lib$Dir>.tboxlibint.h.twimp
o.writable:	C:h.kernel
o.writable:	C:h.swis
o.writable:	gadgets.^.h.object
o.writable:	gadgets.^.h.task
o.writable:	gadgets.^.h.object
o.writable:	gadgets.^.h.utils
o.writable:	<Lib$Dir>.tboxlibint.h.twimp
o.writable:	C:h.kernel
o.writable:	C:h.swis
o.writable:	gadgets.h.writable
