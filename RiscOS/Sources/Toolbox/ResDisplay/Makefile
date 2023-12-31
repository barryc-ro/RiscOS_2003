# Makefile for ResDisplay
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
COMPONENT  = ResDisplay
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
AS      = objasm
CP      = copy
LD      = link
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
 o.auxiliary\
 o.create\
 o.defaults\
 o.delete\
 o.events\
 o.Filter\
 o.getstate\
 o.hide\
 o.layout\
 o.main\
 o.miscop\
 o.redraw\
 o.selection\
 o.show\
 o.task

RAM_OBJS =\
 o.Modhdr_NoD\
 o.auxiliary\
 o.create\
 o.defaults\
 o.delete\
 o.events\
 o.Filter\
 o.getstate\
 o.hide\
 o.layout\
 o.mainRAM\
 o.miscop\
 o.redraw\
 o.selection\
 o.show\
 o.task

MSGSF=o.msgs
RESF=o.res

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

resources: Resources.${LOCALE}.Messages Resources.${LOCALE}.Res
        ${MKDIR} ${RESDIR}
        ${CP} Resources.${LOCALE}.* ${RESDIR}.* ${CPFLAGS}
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

${MSGSF}: @.Resources.${LOCALE}.Messages
	resgen messages_file ${MSGSF} Resources.${LOCALE}.Messages Resources.${COMPONENT}.Messages

${RESF}: @.Resources.${LOCALE}.Res
	resgen res_file ${RESF} Resources.${LOCALE}.Res Resources.${COMPONENT}.Res


${RAM_MODULE}: ${RAM_OBJS} ${TBOXLIB} ${CLIB} ${RESF} ${MSGSF}
	${mkdir} rm
        ${LD} -o $@ -module ${RAM_OBJS} ${TBOXLIB} ${CLIB} ${RESF} ${MSGSF}
        Access $@ RW/R

o.mainRAM: main.c
	$(CC) ${CFLAGS} -o $@ main.c


#---------------------------------------------------------------------------
# Dynamic dependencies:
