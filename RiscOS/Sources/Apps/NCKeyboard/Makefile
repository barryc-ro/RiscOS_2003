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
CFLAGS  = -c -depend !Depend -ffah ${INCLUDES} ${BUILDFLAGS} ${THROWBACK}
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
SOCKLIB   = TCPIPLibs:o.socklib
INETLIB   = TCPIPLibs:o.inetlib
UNIXLIB   = TCPIPLibs:o.unixlib
DBGLIB    = <Lib$Dir>.DebugLib.o.debuglib

# -----------------------------------------------------------------------------
# Include files
#

INCLUDES = -Itbox:,C:

DIRS = o._dirs

LIBS      = ${CLIB} tbox:o.toolboxlib tbox:o.eventlib tbox:o.wimplib
LIBSD     = ${CLIB} tbox:o.toolboxlib tbox:o.eventlib tbox:o.wimplib ${DBGLIB} ${INETLIB} ${SOCKLIB}
LIBSZ     = ${ROMCSTUBS} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib
LIBSZRAM  = ${RLIB} ${CLIB} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib
LIBSZRAMD = ${RLIB} ${CLIB} tbox:o.toolboxlib tbox:o.eventlibm tbox:o.wimplib ${DBGLIB}zm ${INETLIB}zm ${SOCKLIB}zm

DFLAGS   = -DDEBUGLIB

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT = NCKeyboard
APPNAME   = !NCKeybd
TARGET    = aof.NCKeyboard
APP       = abs.!RunImage
OBJS      = o.handlers o.main o.oskbutils o.oskbime o.windowops
OBJSD     = od.handlers od.main od.oskbutils od.oskbime od.windowops
OBJSZ     = oz.handlers oz.main oz.oskbutils oz.oskbime oz.windowops oz.ModuleWrap
OBJSZRAM  = oz.handlers oz.main oz.oskbutils oz.oskbime oz.windowops oz.ModuleWra1
OBJSZRAMD = ozd.handlers ozd.main ozd.oskbutils ozd.oskbime ozd.windowops oz.ModuleWra1
EXPORTS   = 
INSTALLDIR = <Install$Dir>.${APPNAME}
WRAPPER   = RISC_OSLib:s.ModuleWrap
INSTRES   = ${INSTDIR}.Resources.${Locale}

NORMAL_TARGET = ${COMPONENT}
DEBUG_TARGET  = ${COMPONENT}d
MODULE_TARGET = ${COMPONENT}zm

.SUFFIXES: .oz .od .ozd

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
.c.ozd:;    ${CC} ${CFLAGS} -zM -zps1 ${DFLAGS} -o $@ $<
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

debug: ${APP}d
	@echo ${COMPONENT}: Debug application built
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
	${WIPE} ozd ${WFLAGS}
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

${DIRS}:
	@${MKDIR} o
	@${MKDIR} od
	@${MKDIR} oz
	@${MKDIR} ozd
	@${MKDIR} rm
	@${MKDIR} aof
	@${MKDIR} abs
	@${MKDIR} linked
	create ${DIRS}
	stamp  ${DIRS}

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

install: install_${OPTIONS}
	@echo INSTDIR: ${INSTDIR}: resource files copied
	@echo ${COMPONENT}: resource files copied
	@echo

install_STB: ram
	Set NCKeyboardUserIFRes$Path Resources.${UserIF}.${Locale}.,Resources.${UserIF}.UK.
        ${MKDIR} ${INSTRES}
	${CP} NCKeyboardUserIFRes:!BootRAM  ${INSTDIR}.!Boot        ${CPFLAGS}
	${CP} NCKeyboardUserIFRes:!RunRAM   ${INSTDIR}.!Run         ${CPFLAGS}
	${CP} NCKeyboardUserIFRes:CopyLocal ${INSTDIR}.CopyLocal    ${CPFLAGS}
	${CP} rm.${COMPONENT}               ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        ${CP} NCKeyboardUserIFRes:Messages  ${INSTRES}.Messages     ${CPFLAGS}
        ${CP} NCKeyboardUserIFRes:Res       ${INSTRES}.Res          ${CPFLAGS}
        ${CP} NCKeyboardUserIFRes:Sprites   ${INSTRES}.Sprites      ${CPFLAGS}
	${CP} <Lib$Dir>.LocaleRes.LocaleRes ${INSTDIR}.LocaleRes    ${CPFLAGS}

install_: ${APP}
	${MKDIR} ${INSTDIR}
	${CP} LocalUserIFRes:!Boot ${INSTDIR}.!Boot ${CPFLAGS}
	${CP} LocalUserIFRes:!Run ${INSTDIR}.!Run ${CPFLAGS}
	${CP} ${APP} ${INSTDIR}.!RunImage ${CPFLAGS}
	${CP} LocalUserIFRes:Messages ${INSTDIR}.Messages ${CPFLAGS}
	${CP} LocalUserIFRes:Res ${INSTDIR}.Res ${CPFLAGS}
	IfThere LocalUserIFRes:Graphics Then \
		${CP} LocalUserIFRes:Graphics ${INSTDIR}.Sprites ${CPFLAGS}
	IfThere LocalUserIFRes:Sprites Then \
		${CP} LocalUserIFRes:Sprites ${INSTDIR}.Sprites ${CPFLAGS}
	IfThere LocalUserIFRes:TVSprs Then \
		${CP} LocalUserIFRes:TVSprs ${INSTDIR}.TVSprs ${CPFLAGS}

# -----------------------------------------------------------------------------
# Static dependencies
#

${APP}: ${OBJS} ${LIBS} ${DIRS}
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

${APP}d: ${OBJSD} ${LIBSD} ${DIRS}
	${LD} ${LDFLAGS} -o $@ ${OBJSD} ${LIBSD}

rm.${COMPONENT}: ${OBJSZRAM} ${LIBSZRAM} ${DIRS}
	${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAM} ${LIBSZRAM}

rm.${COMPONENT}d: ${OBJSZRAMD} ${LIBSZRAMD} ${DIRS}
	${LD} ${LDFLAGS} -o $@ -module ${OBJSZRAMD} ${LIBSZRAMD}

oz.ModuleWrap: s.ModuleWrap s.AppName ${DIRS}
	${AS} ${AFLAGS} s.ModuleWrap $@

oz.ModuleWra1: s.ModuleWrap s.AppName ${DIRS}
	${AS} ${AFLAGS} -predefine "REMOVE_RAMLOAD_CHECK SETA 1" s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
	${CP} ${WRAPPER} $@ ${CPFLAGS}

# -----------------------------------------------------------------------------
# ROM target (re-linked at ROM Image build time)
#

${TARGET}: ${OBJSZ} ${ROMCSTUBS} ${LIBSZ} ${DIRS}
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
