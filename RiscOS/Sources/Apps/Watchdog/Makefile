# Makefile for Watchdog
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -ff ${INCLUDES} ${DFLAGS} -DCARET_MESSAGE_14
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
Datatoaofflags = -d data -l len -dp -s -p

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
#WRAPPER   = RISCOSLIB:s.ModuleWrap

#
# Include files
#
INCLUDES = -IC:

LIBS     = ${CLIB}
LIBSZ    = ${ROMCSTUBS}
LIBSZRAM = ${RLIB} ${CLIB}

DFLAGS   = -DREAL_SYSTEM

#
# Program specific options:
#
COMPONENT = Watchdog
APPNAME   = !Watchdog
TARGET    = aof.Watchdog
TARGETSA  = rm.Watchdog
APP       = abs.!RunImage
OBJS      = o.kill o.main o.mainwindow o.screen
OBJSZ     = oz.kill oz.main oz.mainwindow oz.screen oz.ModuleWrap
OBJSZRAM  = oz.kill oz.main oz.mainwindow oz.screen oz.ModuleWra1
EXPORTS   = 

.SUFFIXES: .oz .d

#
# Export Paths for Messages module
#
RESAPP = <resource$dir>.Apps.${APPNAME}

#
# Rule patterns
#
.cmhg.o:;   ${CMHG} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.d.o:;      datatoaof -f $< -o $@ $(Datatoaofflags)
.c.oz:;     ${CC} ${CFLAGS} -zM -zps1 -o $@ $<
.s.oz:;     ${AS} ${AFLAGS} $< $@
.d.oz:;     datatoaof -f $< -o $@ $(Datatoaofflags)

#
# build a relocatable module:
#
all: ${APP}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGETSA}
        ${CP} ${TARGETSA} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

ram:    ${TARGETSA}
	@echo ${COMPONENT}: soft-load module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${APP}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESAPP}
        ${CP} LocalRes:!BootROM ${RESAPP}.!Boot ${CPFLAGS}
        ${CP} LocalRes:!RunROM ${RESAPP}.!Run ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# Static dependencies
#
${APP}: ${OBJS} ${LIBS}
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

${TARGETSA}: ${OBJSZRAM} ${LIBSZRAM}
	${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAM} ${LIBSZRAM}

oz.ModuleWrap: s.ModuleWrap s.AppName
	${AS} ${AFLAGS} s.ModuleWrap $@

oz.ModuleWra1: s.ModuleWrap s.AppName
	${AS} ${AFLAGS} -predefine "REMOVE_RAMLOAD_CHECK SETA 1" s.ModuleWrap $@

#s.ModuleWrap: ${WRAPPER}
#        ${CP} ${WRAPPER} $@ ${CPFLAGS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJSZ} ${ROMCSTUBS} ${LIBSZ}
        ${LD} -o $@ -aof ${OBJSZ} ${LIBSZ}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
