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
WRAPPER   = RISCOSLIB:s.ModuleWrap

#
# Include files
#
INCLUDES = -IC:

LIBS     = ${CLIB}
LIBSZ    = ${ROMCSTUBS}
LIBSZRAM = ${RLIB} ${CLIB}

DFLAGS   =

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
RDIR      = Resources.${LOCALE}
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
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${APP}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESAPP}
        ${CP} ${RDIR}.!BootROM ${RESAPP}.!Boot ${CPFLAGS}
        ${CP} ${RDIR}.!RunROM ${RESAPP}.!Run ${CPFLAGS}
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

s.ModuleWrap: ${WRAPPER}
        ${CP} ${WRAPPER} $@ ${CPFLAGS}

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
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
oz.ModuleWra1: s.ModuleWrap
oz.ModuleWra1: s.AppName
oz.ModuleWra1: Hdr:ListOpts
oz.ModuleWra1: Hdr:Macros
oz.ModuleWra1: Hdr:System
oz.ModuleWra1: Hdr:SWIs
oz.ModuleWra1: Hdr:CPU.Generic26
oz.ModuleWra1: Hdr:IO.GenericIO
oz.ModuleWra1: Hdr:RISCOS
oz.ModuleWra1: Hdr:ModHand
oz.ModuleWra1: Hdr:EnvNumbers
oz.ModuleWra1: Hdr:Proc
oz.ModuleWra1: Hdr:FSNumbers
oz.ModuleWra1: Hdr:NewErrors
oz.ModuleWra1: Hdr:Services
oz.ModuleWra1: Hdr:ResourceFS
oz.ModuleWra1: Hdr:MsgTrans
oz.ModuleWra1: Hdr:Variables
oz.ModuleWra1: Hdr:UpCall
oz.ModuleWra1: s.ResFiles
oz.ModuleWra1: s.sutil
o.kill:	c.kill
o.kill:	h.defs
o.kill:	C:h.kernel
o.kill:	h.inlines
o.kill:	C:h.swis
o.kill:	h.wimp
o.kill:	h.defs
main:	c.main
main:	h.defs
main:	C:h.kernel
main:	h.inlines
main:	C:h.swis
main:	h.screen
main:	h.defs
main:	h.SmartCard
main:	h.wimp
main:	h.defs
o.screen:	c.screen
o.screen:	h.defs
o.screen:	C:h.kernel
o.screen:	h.inlines
o.screen:	C:h.swis
o.main:	c.main
o.main:	h.defs
o.main:	C:h.kernel
o.main:	h.inlines
o.main:	C:h.swis
o.main:	h.screen
o.main:	h.defs
o.main:	h.SmartCard
o.main:	h.wimp
o.main:	h.defs
oz.kill:	c.kill
oz.kill:	h.defs
oz.kill:	C:h.kernel
oz.kill:	h.inlines
oz.kill:	C:h.swis
oz.kill:	h.wimp
oz.kill:	h.defs
oz.main:	c.main
oz.main:	h.defs
oz.main:	C:h.kernel
oz.main:	h.inlines
oz.main:	C:h.swis
oz.main:	h.screen
oz.main:	h.defs
oz.main:	h.SmartCard
oz.main:	h.wimp
oz.main:	h.defs
oz.screen:	c.screen
oz.screen:	h.defs
oz.screen:	C:h.kernel
oz.screen:	h.inlines
oz.screen:	C:h.swis
oz.ModuleWrap: s.ModuleWrap
oz.ModuleWrap: s.AppName
oz.ModuleWrap: Hdr:ListOpts
oz.ModuleWrap: Hdr:Macros
oz.ModuleWrap: Hdr:System
oz.ModuleWrap: Hdr:SWIs
oz.ModuleWrap: Hdr:CPU.Generic26
oz.ModuleWrap: Hdr:IO.GenericIO
oz.ModuleWrap: Hdr:RISCOS
oz.ModuleWrap: Hdr:ModHand
oz.ModuleWrap: Hdr:EnvNumbers
oz.ModuleWrap: Hdr:Proc
oz.ModuleWrap: Hdr:FSNumbers
oz.ModuleWrap: Hdr:NewErrors
oz.ModuleWrap: Hdr:Services
oz.ModuleWrap: Hdr:ResourceFS
oz.ModuleWrap: Hdr:MsgTrans
oz.ModuleWrap: Hdr:Variables
oz.ModuleWrap: Hdr:UpCall
oz.ModuleWrap: s.ResFiles
oz.ModuleWrap: s.sutil
