# Makefile for Window
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 06-Jan-94  AMcC   Created
# 04-Feb-98  SNB    Updated to allow RAM builds
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
TLD	= tlink
MSQZ	= modsqz
MKDIR   = cdir
RM      = remove
WIPE    = -wipe
CD      = dir
MAKE    = amu

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

AFLAGS   = -depend !Depend -Stamp -quit -CloseExec
CFLAGS   = -c -depend !Depend ${INCLUDES} -zM -ffah -zps1 ${DFLAGS}
DFLAGS   = -D${SYSTEM}
ROMFLAGS = -DROM
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

RAM_OBJS =\
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
 o.mainRAM\
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

RESF=o.msgs

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
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
        ${WIPE} rm ${WFLAGS}
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


${RESF}: @.Resources.${LOCALE}.Messages
	resgen messages_file ${RESF} Resources.${LOCALE}.Messages Resources.${COMPONENT}.Messages

${RAM_MODULE}: ${RAM_OBJS} ${TBOXLIB} ${WIMPLIB} ${RESF} ${CLIB}
	${mkdir} rm
        ${LD} -o $@ -module ${RAM_OBJS} ${TBOXLIB} ${WIMPLIB} ${RESF} ${CLIB}
        ${MSQZ} $@
        Access $@ RW/R

o.mainRAM: main.c
	$(CC) ${CFLAGS} -o $@ main.c

#---------------------------------------------------------------------------
# Dynamic dependencies:
