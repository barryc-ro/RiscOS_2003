# Makefile for Toolbox
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
COMPONENT  = Toolbox
ROM_MODULE = aof.${COMPONENT}
TARGET     = rm.${COMPONENT}

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
MODSQZ  = modsqz
RESGEN	= resgen
RM      = remove
TLD     = tlink
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

AFLAGS   = -depend !Depend -Stamp -quit -CloseExec
CFLAGS   = -c -depend !Depend ${INCLUDES} -zM -ffah -zps1 ${DFLAGS}
CFLAGSR  = -c -depend !Depend ${INCLUDES} -zM -ffah -zps1 ${DFLAGSR}
DFLAGS   = -DROM
DFLAGSR  =
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
 o.Modhdr_NoD\
 o.base\
 o.callback\
 o.event\
 o.filters\
 o.filter_ven\
 o.globals\
 o.main\
 o.object\
 o.resf\
 o.task

RAM_OBJS =\
 o.Modhdr_NoD\
 o.base\
 o.callback\
 o.event\
 o.filters\
 o.filter_ven\
 o.globals\
 o.mainRAM\
 o.object\
 o.resf\
 o.task\
 o.messages

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
all: ${TARGET}
	@echo ${COMPONENT}: Module built (Relocatable)

${TARGET}: ${RAM_OBJS} ${TBOXLIB}
	${MKDIR} rm
        ${TLD} -rmf -o $@ ${RAM_OBJS} ${TBOXLIB}
        ${MODSQZ} $@

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
        ${WIPE} rm ${WFLAGS}
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

#
# Explicit rules
#
o.mainRAM: c.main
	${CC} ${CFLAGSR} -o o.mainRAM c.main

@.o.messages: @.Messages
        ${RESGEN} messages_file @.o.messages @.Messages Resources.Toolbox.Messages

#---------------------------------------------------------------------------
# Dynamic dependencies:
o.base: s.base
o.callback: s.callback
o.event:	c.event
o.event:	C:h.kernel
o.event:	C:h.swis
o.event:	<Lib$Dir>.tboxlibint.h.const
o.event:	<Lib$Dir>.tboxlibint.h.macros
o.event:	<Lib$Dir>.tboxlibint.h.services
o.event:	<Lib$Dir>.tboxlibint.h.debug
o.event:	<Lib$Dir>.tboxlibint.h.mem
o.event:	<Lib$Dir>.tboxlibint.h.twimp
o.event:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.event:	C:h.kernel
o.event:	<Lib$Dir>.tboxlibint.h.messages
o.event:	C:h.kernel
o.event:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.event:	h.globals
o.event:	h.task
o.event:	h.object
o.event:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.event:	C:h.kernel
o.event:	h.resf
o.event:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.event:	C:h.kernel
o.event:	h.filters
o.event:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.event:	C:h.kernel
o.event:	C:h.kernel
o.event:	C:h.kernel
o.event:	h.task
o.event:	h.event
o.event:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.event:	C:h.kernel
o.filters:	c.filters
o.filters:	C:h.kernel
o.filters:	C:h.swis
o.filters:	<Lib$Dir>.tboxlibint.h.const
o.filters:	<Lib$Dir>.tboxlibint.h.macros
o.filters:	<Lib$Dir>.tboxlibint.h.services
o.filters:	<Lib$Dir>.tboxlibint.h.debug
o.filters:	<Lib$Dir>.tboxlibint.h.mem
o.filters:	<Lib$Dir>.tboxlibint.h.twimp
o.filters:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.filters:	C:h.kernel
o.filters:	<Lib$Dir>.tboxlibint.h.messages
o.filters:	C:h.kernel
o.filters:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.filters:	h.globals
o.filters:	h.task
o.filters:	h.object
o.filters:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.filters:	C:h.kernel
o.filters:	h.resf
o.filters:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.filters:	C:h.kernel
o.filters:	h.filters
o.filters:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.filters:	C:h.kernel
o.filters:	C:h.kernel
o.filters:	C:h.kernel
o.filters:	h.task
o.filters:	h.object
o.filters:	h.event
o.filters:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.filters:	C:h.kernel
o.filters:	h.filters
o.filter_ven: s.filter_ven
o.globals:	c.globals
o.globals:	<Lib$Dir>.tboxlibint.h.const
o.globals:	<Lib$Dir>.tboxlibint.h.macros
o.globals:	<Lib$Dir>.tboxlibint.h.services
o.globals:	<Lib$Dir>.tboxlibint.h.debug
o.globals:	<Lib$Dir>.tboxlibint.h.mem
o.globals:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.globals:	h.globals
o.globals:	h.task
o.globals:	h.object
o.globals:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.globals:	C:h.kernel
o.globals:	<Lib$Dir>.tboxlibint.h.twimp
o.globals:	C:h.kernel
o.globals:	h.resf
o.globals:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.globals:	C:h.kernel
o.globals:	h.filters
o.globals:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.globals:	C:h.kernel
o.globals:	C:h.kernel
o.globals:	C:h.kernel
o.globals:	C:h.kernel
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
o.main:	h.globals
o.main:	h.task
o.main:	h.object
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	C:h.kernel
o.main:	<Lib$Dir>.tboxlibint.h.twimp
o.main:	C:h.kernel
o.main:	h.resf
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	C:h.kernel
o.main:	h.filters
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	C:h.kernel
o.main:	C:h.kernel
o.main:	C:h.kernel
o.main:	<Lib$Dir>.tboxlibint.h.services
o.main:	h.object
o.main:	h.task
o.main:	h.resf
o.main:	h.event
o.main:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.main:	C:h.kernel
o.main:	h.filters
o.main:	h.callback
o.main:	h.main
o.main:	C:h.kernel
o.object:	c.object
o.object:	C:h.kernel
o.object:	C:h.swis
o.object:	<Lib$Dir>.tboxlibint.h.const
o.object:	<Lib$Dir>.tboxlibint.h.macros
o.object:	<Lib$Dir>.tboxlibint.h.services
o.object:	<Lib$Dir>.tboxlibint.h.debug
o.object:	<Lib$Dir>.tboxlibint.h.mem
o.object:	<Lib$Dir>.tboxlibint.h.string32
o.object:	<Lib$Dir>.tboxlibint.h.messages
o.object:	C:h.kernel
o.object:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.object:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.object:	h.globals
o.object:	h.task
o.object:	h.object
o.object:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.object:	C:h.kernel
o.object:	<Lib$Dir>.tboxlibint.h.twimp
o.object:	C:h.kernel
o.object:	h.resf
o.object:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.object:	C:h.kernel
o.object:	h.filters
o.object:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.object:	C:h.kernel
o.object:	C:h.kernel
o.object:	C:h.kernel
o.object:	h.task
o.object:	h.resf
o.object:	h.event
o.object:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.object:	C:h.kernel
o.object:	h.object
o.resf:	c.resf
o.resf:	C:h.swis
o.resf:	C:h.kernel
o.resf:	C:h.kernel
o.resf:	<Lib$Dir>.tboxlibint.h.const
o.resf:	<Lib$Dir>.tboxlibint.h.macros
o.resf:	<Lib$Dir>.tboxlibint.h.services
o.resf:	<Lib$Dir>.tboxlibint.h.debug
o.resf:	<Lib$Dir>.tboxlibint.h.mem
o.resf:	<Lib$Dir>.tboxlibint.h.messages
o.resf:	C:h.kernel
o.resf:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.resf:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.resf:	h.globals
o.resf:	h.task
o.resf:	h.object
o.resf:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.resf:	C:h.kernel
o.resf:	<Lib$Dir>.tboxlibint.h.twimp
o.resf:	C:h.kernel
o.resf:	h.resf
o.resf:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.resf:	C:h.kernel
o.resf:	h.filters
o.resf:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.resf:	C:h.kernel
o.resf:	C:h.kernel
o.resf:	C:h.kernel
o.resf:	h.resf
o.resf:	h.task
o.resf:	h.object
o.task:	c.task
o.task:	C:h.kernel
o.task:	C:h.swis
o.task:	<Lib$Dir>.tboxlibint.h.const
o.task:	<Lib$Dir>.tboxlibint.h.macros
o.task:	<Lib$Dir>.tboxlibint.h.services
o.task:	<Lib$Dir>.tboxlibint.h.debug
o.task:	<Lib$Dir>.tboxlibint.h.mem
o.task:	<Lib$Dir>.tboxlibint.h.twimp
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	C:h.kernel
o.task:	<Lib$Dir>.tboxlibint.h.string32
o.task:	<Lib$Dir>.tboxlibint.h.messages
o.task:	C:h.kernel
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	h.globals
o.task:	h.task
o.task:	h.object
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	C:h.kernel
o.task:	h.resf
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	C:h.kernel
o.task:	h.filters
o.task:	<Lib$Dir>.tboxlibint.objects.h.toolbox
o.task:	C:h.kernel
o.task:	C:h.kernel
o.task:	C:h.kernel
o.task:	<Lib$Dir>.tboxlibint.h.services
o.task:	h.resf
o.task:	h.filters
o.task:	h.object
o.task:	h.callback
o.task:	h.task
