# Makefile for Toolbox
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 06-Jan-94  AMcC   Created
# 04-Feb-98  SNB    Updated to allow RAM builds
#

#
# Component specific options:
#
COMPONENT  = Toolbox
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
TLD     = tlink
MKDIR   = cdir
MODSQZ  = modsqz
RESGEN	= resgen
RM      = remove
WIPE    = -wipe

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
 o.memory\
 o.memswis\
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
 o.memory\
 o.memswis\
 o.object\
 o.resf\
 o.task

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
${RESF}: @.Resources.${LOCALE}.Messages
	resgen messages_file ${RESF} Resources.${LOCALE}.Messages Resources.${COMPONENT}.Messages

${RAM_MODULE}: ${RAM_OBJS} ${TBOXLIB} ${CLIB} ${RESF} ${CLIB}
	${mkdir} rm
        ${LD} -o $@ -module ${RAM_OBJS} ${TBOXLIB} ${RESF} ${CLIB}
        Access $@ RW/R

o.mainRAM: main.c
	$(CC) ${CFLAGS} -o $@ main.c


#---------------------------------------------------------------------------
# Dynamic dependencies:
