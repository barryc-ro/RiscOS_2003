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
oz.main:	c.main
oz.main:	C:h.swis
oz.main:	C:h.kernel
oz.main:	OS:h.os
oz.main:	OS:h.types
oz.main:	OS:h.wimp
oz.main:	OS:h.osspriteop
oz.main:	OS:h.font
oz.main:	C:h.soundfile
oz.main:	OS:h.osgbpb
oz.main:	h.play_scode
oz.play_scode:	c.play_scode
oz.play_scode:	OS:h.osgbpb
oz.play_scode:	OS:h.types
oz.play_scode:	OS:h.os
oz.play_scode:	C:h.replay
oz.play_scode:	OS:h.osspriteop
oz.play_scode:	C:h.soundfile
oz.play_scode:	h.play_scode
o.utils:	c.utils
o.utils:	h.utils
o.main:	c.main
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	OS:h.os
o.main:	OS:h.types
o.main:	OS:h.wimp
o.main:	OS:h.osspriteop
o.main:	OS:h.font
o.main:	C:h.soundfile
o.main:	OS:h.osgbpb
o.main:	h.play_scode
o.main:	h.utils
o.play_scode:	c.play_scode
o.play_scode:	OS:h.osgbpb
o.play_scode:	OS:h.types
o.play_scode:	OS:h.os
o.play_scode:	C:h.replay
o.play_scode:	OS:h.osspriteop
o.play_scode:	C:h.soundfile
o.play_scode:	h.play_scode
