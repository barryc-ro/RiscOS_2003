# Makefile for DCS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 06-Jan-94  AMcC   Created
#

#
# Component specific options:
#
COMPONENT  = DCS
ROM_MODULE = aof.${COMPONENT}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2

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

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

CFLAGS   = -c -depend !Depend ${INCLUDES} -zM -ffa -zps1 ${DFLAGS}
DFLAGS   = -DROM
INCLUDES = -I<Lib$Dir>.tboxlibint.,<Lib$Dir>.tboxlibs:,C:

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym
TBOXLIB   = <Lib$Dir>.tboxlibint.o.tboxlibm

OBJS =\
 o.ModNoDebug\
 o.create\
 o.delete\
 o.events\
 o.getstate\
 o.hide\
 o.main\
 o.miscop\
 o.show\
 o.task

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $<

#
# Main rules:
#
#
rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources: Resources.${LOCALE}.DCS.Messages Resources.${LOCALE}.DCS.Res Resources.${LOCALE}.Quit.Messages Resources.${LOCALE}.Quit.Res
        ${CP} Resources.${LOCALE}.*   ${RESDIR}.*  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${TBOXLIB} ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${TBOXLIB} ${ROMCSTUBS}

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
o.create:	c.create
o.create:	C:h.stdlib
o.create:	C:h.stddef
o.create:	C:h.kernel
o.create:	C:h.swis
o.create:	<Lib$Dir>.tboxlibint.h.const
o.create:	<Lib$Dir>.tboxlibint.h.macros
o.create:	<Lib$Dir>.tboxlibint.h.services
o.create:	<Lib$Dir>.tboxlibint.h.debug
o.create:	<Lib$Dir>.tboxlibint.h.mem
o.create:	<Lib$Dir>.tboxlibint.h.messages
o.create:	C:h.kernel
o.create:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.create:	<Lib$Dir>.tboxlibint.h.objsupport
o.create:	C:h.stddef
o.create:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.create:	<Lib$Dir>.tboxlibint.objects.h.window
o.create:	<Lib$Dir>.tboxlibint.h.twimp
o.create:	C:h.kernel
o.create:	<Lib$Dir>.tboxlibint.h.slist
o.create:	<Lib$Dir>.tboxlibint.h.string32
o.create:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.create:	<Lib$Dir>.tboxlibint.objects.h.window
o.create:	<Lib$Dir>.tboxlibint.objects.h.dcs
o.create:	<Lib$Dir>.tboxlibint.objects.h.window
o.create:	<Lib$Dir>.tboxlibint.objects.h.quit
o.create:	h.object
o.create:	h.task
o.create:	h.create
o.create:	C:h.kernel
o.delete:	c.delete
o.delete:	C:h.kernel
o.delete:	C:h.swis
o.delete:	<Lib$Dir>.tboxlibint.h.const
o.delete:	<Lib$Dir>.tboxlibint.h.macros
o.delete:	<Lib$Dir>.tboxlibint.h.services
o.delete:	<Lib$Dir>.tboxlibint.h.debug
o.delete:	<Lib$Dir>.tboxlibint.h.mem
o.delete:	<Lib$Dir>.tboxlibint.h.messages
o.delete:	C:h.kernel
o.delete:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.delete:	<Lib$Dir>.tboxlibint.h.slist
o.delete:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.delete:	<Lib$Dir>.tboxlibint.objects.h.dcs
o.delete:	<Lib$Dir>.tboxlibint.objects.h.window
o.delete:	<Lib$Dir>.tboxlibint.h.twimp
o.delete:	C:h.kernel
o.delete:	h.object
o.delete:	h.task
o.delete:	h.delete
o.delete:	C:h.kernel
o.events:	c.events
o.events:	C:h.kernel
o.events:	C:h.swis
o.events:	<Lib$Dir>.tboxlibint.h.const
o.events:	<Lib$Dir>.tboxlibint.h.macros
o.events:	<Lib$Dir>.tboxlibint.h.services
o.events:	<Lib$Dir>.tboxlibint.h.debug
o.events:	<Lib$Dir>.tboxlibint.h.mem
o.events:	<Lib$Dir>.tboxlibint.h.messages
o.events:	C:h.kernel
o.events:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.events:	<Lib$Dir>.tboxlibint.h.slist
o.events:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.events:	<Lib$Dir>.tboxlibint.objects.h.dcs
o.events:	<Lib$Dir>.tboxlibint.objects.h.window
o.events:	<Lib$Dir>.tboxlibint.h.twimp
o.events:	C:h.kernel
o.events:	<Lib$Dir>.tboxlibint.objects.h.quit
o.events:	h.object
o.events:	h.events
o.events:	C:h.kernel
o.events:	h.task
o.getstate:	c.getstate
o.getstate:	C:h.kernel
o.getstate:	C:h.swis
o.getstate:	<Lib$Dir>.tboxlibint.h.const
o.getstate:	<Lib$Dir>.tboxlibint.h.macros
o.getstate:	<Lib$Dir>.tboxlibint.h.services
o.getstate:	<Lib$Dir>.tboxlibint.h.debug
o.getstate:	<Lib$Dir>.tboxlibint.h.mem
o.getstate:	<Lib$Dir>.tboxlibint.h.messages
o.getstate:	C:h.kernel
o.getstate:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.getstate:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.getstate:	<Lib$Dir>.tboxlibint.objects.h.generic
o.getstate:	h.object
o.getstate:	h.task
o.getstate:	h.getstate
o.getstate:	C:h.kernel
o.hide:	c.hide
o.hide:	C:h.kernel
o.hide:	C:h.swis
o.hide:	<Lib$Dir>.tboxlibint.h.const
o.hide:	<Lib$Dir>.tboxlibint.h.macros
o.hide:	<Lib$Dir>.tboxlibint.h.services
o.hide:	<Lib$Dir>.tboxlibint.h.debug
o.hide:	<Lib$Dir>.tboxlibint.h.mem
o.hide:	<Lib$Dir>.tboxlibint.h.messages
o.hide:	C:h.kernel
o.hide:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.hide:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.hide:	<Lib$Dir>.tboxlibint.objects.h.generic
o.hide:	h.object
o.hide:	h.task
o.hide:	h.hide
o.hide:	C:h.kernel
o.miscop:	c.miscop
o.miscop:	C:h.kernel
o.miscop:	C:h.swis
o.miscop:	<Lib$Dir>.tboxlibint.h.const
o.miscop:	<Lib$Dir>.tboxlibint.h.macros
o.miscop:	<Lib$Dir>.tboxlibint.h.services
o.miscop:	<Lib$Dir>.tboxlibint.h.debug
o.miscop:	<Lib$Dir>.tboxlibint.h.mem
o.miscop:	<Lib$Dir>.tboxlibint.h.messages
o.miscop:	C:h.kernel
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.window
o.miscop:	<Lib$Dir>.tboxlibint.h.twimp
o.miscop:	C:h.kernel
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.dcs
o.miscop:	<Lib$Dir>.tboxlibint.objects.h.window
o.miscop:	h.object
o.miscop:	h.task
o.miscop:	h.miscop
o.miscop:	C:h.kernel
o.task:	c.task
o.task:	C:h.kernel
o.task:	C:h.swis
o.task:	<Lib$Dir>.tboxlibint.h.const
o.task:	<Lib$Dir>.tboxlibint.h.macros
o.task:	<Lib$Dir>.tboxlibint.h.services
o.task:	<Lib$Dir>.tboxlibint.h.debug
o.task:	<Lib$Dir>.tboxlibint.h.mem
o.task:	<Lib$Dir>.tboxlibint.h.messages
o.task:	C:h.kernel
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	<Lib$Dir>.tboxlibint.h.slist
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	<Lib$Dir>.tboxlibint.objects.h.dcs
o.task:	<Lib$Dir>.tboxlibint.objects.h.window
o.task:	<Lib$Dir>.tboxlibint.h.twimp
o.task:	C:h.kernel
o.task:	<Lib$Dir>.tboxlibint.objects.h.quit
o.task:	<Lib$Dir>.tboxlibint.objects.h.window
o.task:	h.object
o.task:	h.task
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	<Lib$Dir>.tboxlibint.h.const
o.main:	<Lib$Dir>.tboxlibint.h.macros
o.main:	<Lib$Dir>.tboxlibint.h.services
o.main:	<Lib$Dir>.tboxlibint.h.debug
o.main:	<Lib$Dir>.tboxlibint.h.mem
o.main:	<Lib$Dir>.tboxlibint.h.messages
o.main:	C:h.kernel
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	<Lib$Dir>.tboxlibint.h.rmensure
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	<Lib$Dir>.tboxlibint.objects.h.DCS
o.main:	<Lib$Dir>.tboxlibint.objects.h.window
o.main:	<Lib$Dir>.tboxlibint.h.twimp
o.main:	C:h.kernel
o.main:	<Lib$Dir>.tboxlibint.objects.h.Quit
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
o.main:	h.task
o.main:	h.main
o.main:	C:h.kernel
o.show:	c.show
o.show:	C:h.kernel
o.show:	C:h.swis
o.show:	<Lib$Dir>.tboxlibint.h.const
o.show:	<Lib$Dir>.tboxlibint.h.macros
o.show:	<Lib$Dir>.tboxlibint.h.services
o.show:	<Lib$Dir>.tboxlibint.h.debug
o.show:	<Lib$Dir>.tboxlibint.h.mem
o.show:	<Lib$Dir>.tboxlibint.h.messages
o.show:	C:h.kernel
o.show:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.show:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.show:	<Lib$Dir>.tboxlibint.objects.h.dcs
o.show:	<Lib$Dir>.tboxlibint.objects.h.window
o.show:	<Lib$Dir>.tboxlibint.h.twimp
o.show:	C:h.kernel
o.show:	<Lib$Dir>.tboxlibint.objects.h.quit
o.show:	h.object
o.show:	h.task
o.show:	h.show
o.show:	C:h.kernel
