# Makefile for soundplay App
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 08-Feb-95  SJM          Created from CD.
#

#
# Paths
#
EXP_HDR = <export$dir>
#INSTDIR = <Install$Dir>.!soundplay
#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link -v
RM      = remove
WIPE    = -wipe
COMPRESS = squash

AFLAGS = -depend !Depend -Stamp -quit
DFLAGS = ${DFLAGS1}
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS} -fa ${THROWBACK}
CPFLAGS = ~cfr~v${CPFLAGS1}
WFLAGS  = ~c~v
LDFLAGS =
#
# Libraries
#
CLIB      = CLIB:o.stubs
OSLIB     = OSLIB:o.oslib
RLIB      = RISCOSLIB:o.risc_oslib
RLIBSTRONG= libs.strongarm.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
WRAPPER   = RISCOSLIB:s.ModuleWrap
#
# Include files
#
INCLUDES = -IOS:,C:

LIBS=${OSLIB} C:o.replay C:o.soundfile
#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESAPP = <resource$dir>.Apps.${APP}

#
# Program specific options:
#
COMPONENT   = soundplay
APP         = !soundplay
APPLICATION = abs.!RunImage
TARGET      = aof.soundplay
LDIR        = Resources.${LOCALE}

OBJS= 	main.o play_scode.o utils.o
OBJSZ= 	oz.main oz.play_scode oz.utils

#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -zM -zps1 -o $@ $< -DSOUNDPLAY_ROM=1
.s.o:;      ${AS} ${AFLAGS} $< $@
.s.oz:;     ${AS} ${AFLAGS} $< $@

#
# build a !RunImage module:
#
all: ${APPLICATION}
	@echo ${APPLICATION} built

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
	@echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} oz.* ${WFLAGS}
	${WIPE} linked.* ${WFLAGS}
	${WIPE} map.* ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${APPLICATION}
	@echo ${COMPONENT}: cleaned

resources:
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESDIR}
	${CP} ${LDIR}.!Boot     ${RESAPP}.!Boot         ${CPFLAGS}
	${CP} ${LDIR}.!Help     ${RESAPP}.!Help         ${CPFLAGS}
	${CP} ${LDIR}.!Run2     ${RESAPP}.!Run          ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied

install: ${RESFILES} ${APPLICATION}
	${MKDIR} ${INSTDIR}
	${CP} ${LDIR}.!Boot     ${INSTDIR}.!Boot        ${CPFLAGS}
	${CP} ${LDIR}.!Help     ${INSTDIR}.!Help        ${CPFLAGS}
	${CP} ${LDIR}.!Run1     ${INSTDIR}.!Run         ${CPFLAGS}
	${CP} ${APPLICATION} 	${INSTDIR}.!RunImage	${CPFLAGS}
	squeeze ${INSTDIR}.!RunImage
	access ${INSTDIR}.!RunImage RW/R
	@echo ${COMPONENT}: installed

#
# Static dependencies:
#
${APPLICATION}: ${OBJS} ${CLIB} ${LIBS}
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${CLIB} ${LIBS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: oz.ModuleWrap ${OBJSZ} ${ROMSTUBS} ${LIBS}
	${LD} -o $@ -aof oz.ModuleWrap ${OBJSZ} ${ROMSTUBS} ${LIBS}

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

oz.ModuleWrap: s.ModuleWrap s.AppName
	${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
        ${CP} ${WRAPPER} $@ ${CPFLAGS}

# Dynamic dependencies:
