# Makefile for DCS
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
COMPONENT  = DCS
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}

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
TLD	= tlink
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

CFLAGS   = -c -depend !Depend ${INCLUDES} -zM -ffa -zps1 ${DFLAGS}
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

RAM_OBJS =\
 o.ModNoDebug\
 o.create\
 o.delete\
 o.events\
 o.getstate\
 o.hide\
 o.mainRAM\
 o.miscop\
 o.show\
 o.task

DCSRESF=o.res_dcs
DCSMESS=o.msgs_dcs
QUITRESF=o.res_quit
QUITMESS=o.msgs_quit

ALLRESOURCES=${DCSRESF} ${DCSMESS} ${QUITRESF} ${QUITMESS}


#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $<

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

resources: Resources.${LOCALE}.DCS.Messages Resources.${LOCALE}.DCS.Res Resources.${LOCALE}.Quit.Messages Resources.${LOCALE}.Quit.Res
        ${CP} Resources.${LOCALE}.*   ${RESDIR}.*  ${CPFLAGS}
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

${DCSRESF}: @.Resources.${LOCALE}.DCS.Res
	resgen dcsres_file ${DCSRESF} Resources.${LOCALE}.DCS.Res Resources.DCS.Res
${DCSMESS}: @.Resources.${LOCALE}.DCS.Messages
	resgen dcsmess_file ${DCSMESS} Resources.${LOCALE}.DCS.Messages Resources.DCS.Messages
${QUITRESF}: @.Resources.${LOCALE}.Quit.Res
	resgen quitres_file ${QUITRESF} Resources.${LOCALE}.Quit.Res Resources.Quit.Res
${QUITMESS}: @.Resources.${LOCALE}.Quit.Messages
	resgen quitmess_file ${QUITMESS} Resources.${LOCALE}.Quit.Messages Resources.Quit.Messages

${RAM_MODULE}: ${RAM_OBJS} ${TBOXLIB} ${ALLRESOURCES} ${CLIB}
	${mkdir} rm
        ${LD} -o $@ -module ${RAM_OBJS} ${TBOXLIB} ${ALLRESOURCES} ${CLIB}
        Access $@ RW/R

o.mainRAM: main.c
	$(CC) ${CFLAGS} -o $@ main.c



#---------------------------------------------------------------------------
# Dynamic dependencies:
