# Makefile for SCApp
# ==================
#

# -----------------------------------------------------------------------------
# Change list
#

# Date		Name	Description
# ====		====	===========
# 05-Apr-01	NB 	Created.

# -----------------------------------------------------------------------------
# Paths
#

EXP_HDR = <export$dir>

# -----------------------------------------------------------------------------
# Generic options
#
include StdTools
include AppLibs

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -ff ${INCLUDES} ${BUILDFLAGS} ${THROWBACK}
LDFLAGS =
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

# -----------------------------------------------------------------------------
# Libraries
#
BASELIB   = <Lib$Dir>.ThirdParty.WSS.BaseLib.o.BaseLibZM
WSSLIB    = <Lib$Dir>.ThirdParty.WSS.WSSLib.o.WSSLibZM

# -----------------------------------------------------------------------------
# Include files
#

INCLUDES = -Itbox:,C:,TCPIPLibs:,<Obey$Dir>.^.Common,<Lib$Dir>.ThirdParty.WSS.WSSLib

LIBS      = ${CLIB} ${WSSLIB} ${BASELIB}
LIBSZ     = ${ROMCSTUBS} ${WSSLIB} ${BASELIB}

DFLAGS   = 

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT = SCApp
APPNAME   = !SHOUTCast
TARGET    = aof.${COMPONENT}
APP       = abs.!RunImage

OBJS      = o.main o.utils
OBJSZ     = oz.main oz.utils oz.ModuleWrap

EXPORTS   = 
INSTALLDIR = ${INSTDIR}.${APPNAME}
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
# RAM builds
#

all: ${APP}
	@echo ${COMPONENT}: built
	@echo

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
	${CP} LocalRes:!BootROM    ${RESAPP}.!Boot     ${CPFLAGS}
	${CP} LocalRes:!HelperROM  ${RESAPP}.!Helper   ${CPFLAGS}
	${CP} LocalRes:!PlugInROM  ${RESAPP}.!PlugIn   ${CPFLAGS}
	${CP} LocalRes:!RunROM     ${RESAPP}.!Run      ${CPFLAGS}
	${CP} LocalRes:!Sprites    ${RESAPP}.!Sprites  ${CPFLAGS}
	${CP} LocalRes:Templates   ${RESAPP}.Templates ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied.

install: all
	${MKDIR} ${INSTALLDIR}
	${CP} LocalRes:!Boot     ${INSTALLDIR}.!Boot     ${CPFLAGS}
	${CP} LocalRes:!Help     ${INSTALLDIR}.!Help     ${CPFLAGS}
	${CP} LocalRes:!Helper   ${INSTALLDIR}.!Helper   ${CPFLAGS}
	${CP} LocalRes:!PlugIn   ${INSTALLDIR}.!PlugIn   ${CPFLAGS}
	${CP} LocalRes:!Run      ${INSTALLDIR}.!Run      ${CPFLAGS}
	${CP} LocalRes:!Sprites  ${INSTALLDIR}.!Sprites  ${CPFLAGS}
	${CP} LocalRes:CPUType   ${INSTALLDIR}.CPUType   ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTALLDIR}.Templates ${CPFLAGS}
	${CP} ${APP}             ${INSTALLDIR}.!RunImage ${CPFLAGS}
	@echo ${COMPONENT}: installed to ${INSTALLDIR}

# -----------------------------------------------------------------------------
# Static dependencies
#

${APP}: ${OBJS} ${LIBS} o.dirs
	${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

oz.ModuleWrap: s.ModuleWrap s.AppName o.dirs
	${AS} ${AFLAGS} s.ModuleWrap $@

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


# -----------------------------------------------------------------------------
# Dynamic dependencies:
