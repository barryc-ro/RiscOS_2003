# Makefile for NCKeyboard
# =======================
#

# -----------------------------------------------------------------------------
# Change list
#

# Date		Name	Description
# ====		====	===========
# 17-Aug-98	SBF	NC on-screen keyboard
# 04-Sep-98	KJB	ROM build optimised for link 5.08 to reduce module
#			binary size (so you must use 5.08 or later if
#			doing a ROM build)
# 04-Sep-98	ADH	-DFUNAI3 added to DFLAGSg29
# 27-Oct-98	ADH	-DFUNAI3_POLL added to DFLAGS (temporary). The
#			'clean' rule now cleans, and an 'o.dirs' rule is
#			used to correctly recreate o, oz, etc. Some
#			aesthetic formatting changes done in passing.
# 29-Oct-98	ADH	-DFUNAI3_POLL removed from DFLAGS.
# 12-Nov-98	SBF	Removed -DFUNAI3; define this in the build components
#			file instead using ${BUILDFLAGS}
# 08-Oct-99	SNB	Added support to build STB Baseline style NCKeyboard.
#			This is a ram module plus a !NCKeyboard psuedo-app.
#			Builds if OPTIONS=STB passed into makefile.
# 26-Oct-99	SBF	Changed conditionals to use "IfThere" to suppress
#			bogus build errors
# 01-Feb-00	JPB	Added ${THROWBACK} to CFLAGS to allow throwback
#			to be added in make obey files.

# -----------------------------------------------------------------------------
# Paths
#

EXP_HDR = <export$dir>

# -----------------------------------------------------------------------------
# Generic options
#

MKDIR   = mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
STRIP   = stripdepnd

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -ff ${INCLUDES} ${BUILDFLAGS} ${THROWBACK}
LDFLAGS =
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

# -----------------------------------------------------------------------------
# Libraries
#

CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
SOCKLIB   = TCPIPLibs:o.socklibzm
INETLIB   = TCPIPLibs:o.inetlibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm
LIBSD     = <Lib$Dir>.DebugLib.o.debuglibzm

# -----------------------------------------------------------------------------
# Include files
#

INCLUDES = -Itbox:,C:

LIBS      = ${CLIB} tbox:o.toolboxlib tbox:o.eventlib tbox:o.wimplib
LIBSZ     = ${ROMCSTUBS} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib
LIBSZRAM  = ${RLIB} ${CLIB} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib
LIBSZRAMD = ${RLIB} ${CLIB} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib ${LIBSD} ${UNIXLIB} ${INETLIB} ${SOCKLIB}

DFLAGS   = -DDEBUGLIB

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT = NCKeyboard
APPNAME   = !NCKeybd
TARGET    = aof.NCKeyboard
APP       = abs.!RunImage
OBJS      = o.handlers o.main o.oskbutils o.oskbime o.windowops
OBJSZ     = oz.handlers oz.main oz.oskbutils oz.oskbime oz.windowops oz.ModuleWrap
OBJSZRAM  = oz.handlers oz.main oz.oskbutils oz.oskbime oz.windowops oz.ModuleWra1
OBJSZRAMD = od.handlers od.main od.oskbutils od.oskbime od.windowops oz.ModuleWra1
EXPORTS   = 
INSTALLDIR = <Install$Dir>.${APPNAME}
WRAPPER   = RISC_OSLib:s.ModuleWrap

NORMAL_TARGET = ${COMPONENT}
DEBUG_TARGET  = ${COMPONENT}d
MODULE_TARGET = ${COMPONENT}zm

.SUFFIXES: .oz .od

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESAPP = <resource$dir>.Apps.${APPNAME}

# -----------------------------------------------------------------------------
# Rule patterns
#

.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.od:;     ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -zM -zps1 -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

# -----------------------------------------------------------------------------
# Build a relocatable module
#

all: ${APP}
	@echo ${COMPONENT}: built
	@echo

# Binary Build Environment augmenting rules
BBETYPE   = local
bbe-local: bbe-generic
  BBE_Export_Dir Resources.${UI_VARIANT}

# -----------------------------------------------------------------------------
# RISC OS ROM build rules
#

rom: ${TARGET}
	@echo ${COMPONENT}: rom module built
	@echo

export: ${EXPORTS}
	@echo ${COMPONENT}: export complete
	@echo

install_rom: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed
	@echo

ram: rm.${COMPONENT}
	@echo ${COMPONENT}: soft-load module installed
	@echo

ram_debug: rm.${COMPONENT}d
	@echo ${COMPONENT}: soft-load debug module installed
	@echo

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} oz ${WFLAGS}
	${WIPE} od ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} aof ${WFLAGS}
	${WIPE} abs ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${APP}
        ${RM} s.ModuleWrap
	${STRIP}
	@echo
	@echo ${COMPONENT}: cleaned

o.dirs:
	@${MKDIR} o
	@${MKDIR} od
	@${MKDIR} oz
	@${MKDIR} rm
	@${MKDIR} aof
	@${MKDIR} abs
	@${MKDIR} linked
	create o.dirs
	stamp  o.dirs

resources:
	${MKDIR} ${RESAPP}
	Set NCKeyboardUserIFRes$Path Resources.${UI_VARIANT}.${Locale}.,Resources.${UserIF}.${Locale}.,Resources.${UserIF}.UK.
	TokenCheck NCKeyboardUserIFRes:Messages
	${CP} NCKeyboardUserIFRes:!BootROM ${RESAPP}.!Boot ${CPFLAGS}
	${CP} NCKeyboardUserIFRes:!RunROM ${RESAPP}.!Run ${CPFLAGS}
	${CP} NCKeyboardUserIFRes:Messages ${RESAPP}.Messages ${CPFLAGS}
	IfThere NCKeyboardUserIFRes:Sprites Then \
		${CP} NCKeyboardUserIFRes:Sprites ${RESAPP}.Sprites ${CPFLAGS}
	IfThere NCKeyboardUserIFRes:TVSprs Then \
		${CP} NCKeyboardUserIFRes:TVSprs ${RESAPP}.TVSprs ${CPFLAGS}
	${CP} NCKeyboardUserIFRes:Res ${RESAPP}.Res ${CPFLAGS}
	UnSet NCKeyboardUserIFRes$Path
	@echo ${COMPONENT}: resource files copied
	@echo

install: install_${OPTIONS} install_common
	@echo INSTDIR: ${INSTDIR}: resource files copied
	@echo ${COMPONENT}: resource files copied
	@echo

install_common:
	${MKDIR} ${INSTDIR}

install_STB: ram
	${CP} LocalUserIFRes:!Boot ${INSTDIR}.!Boot ${CPFLAGS}
	${CP} LocalUserIFRes:!Run ${INSTDIR}.!Run ${CPFLAGS}
	${CP} LocalUserIFRes:Messages ${INSTDIR}.Messages ${CPFLAGS}
	${CP} LocalUserIFRes:Res ${INSTDIR}.Res ${CPFLAGS}
	${CP} rm.${COMPONENT} ${INSTDIR}.${RMDIR}.${COMPONENT} ${CPFLAGS}

install_: ${APP}
	${CP} LocalUserIFRes:!BootROM ${INSTDIR}.!Boot ${CPFLAGS}
	${CP} LocalUserIFRes:!Run ${INSTDIR}.!Run ${CPFLAGS}
	${CP} LocalUserIFRes:Messages ${INSTDIR}.Messages ${CPFLAGS}
	IfThere LocalUserIFRes:Graphics Then \
		${CP} LocalUserIFRes:Graphics ${INSTDIR}.Sprites ${CPFLAGS}
	IfThere LocalUserIFRes:Sprites Then \
		${CP} LocalUserIFRes:Sprites ${INSTDIR}.Sprites ${CPFLAGS}
	IfThere LocalUserIFRes:TVSprs Then \
		${CP} LocalUserIFRes:TVSprs ${INSTDIR}.TVSprs ${CPFLAGS}
	${CP} LocalUserIFRes:Res ${INSTDIR}.Res ${CPFLAGS}
	${CP} ${APP} ${INSTDIR}.!RunImage ${CPFLAGS}

# -----------------------------------------------------------------------------
# Static dependencies
#

${APP}: ${OBJS} ${LIBS} o.dirs
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

rm.${COMPONENT}: ${OBJSZRAM} ${LIBSZRAM} o.dirs
	${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAM} ${LIBSZRAM}

rm.${COMPONENT}d: ${OBJSZRAMD} ${LIBSZRAMD} o.dirs
	${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAMD} ${LIBSZRAMD}

oz.ModuleWrap: s.ModuleWrap s.AppName o.dirs
	${AS} ${AFLAGS} s.ModuleWrap $@

oz.ModuleWra1: s.ModuleWrap s.AppName o.dirs
	${AS} ${AFLAGS} -predefine "REMOVE_RAMLOAD_CHECK SETA 1" s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
	${CP} ${WRAPPER} $@ ${CPFLAGS}

# -----------------------------------------------------------------------------
# ROM target (re-linked at ROM Image build time)
#

${TARGET}: ${OBJSZ} ${ROMCSTUBS} ${LIBSZ} o.dirs
	${LD} -o $@ -aof ${OBJSZ} ${LIBSZ}

# -----------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#

rom_link:
	${MKDIR} linked
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# -----------------------------------------------------------------------------
# Dynamic dependencies:
