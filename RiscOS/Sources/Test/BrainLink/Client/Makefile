# Makefile for Brainlink Client
# =============================
#

# -----------------------------------------------------------------------------
# Change list
#

# Date		Name	Description
# ====		====	===========
# 30-Apr-01	NB 	Created.

# -----------------------------------------------------------------------------
# Paths
#

EXP_HDR = <export$dir>

# -----------------------------------------------------------------------------
# Generic options
#
include Makefiles:STDTools
include Makefiles:APPLibs

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -g -depend !Depend -ff ${INCLUDES} ${BUILDFLAGS} ${THROWBACK}
LDFLAGS = 
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

# -----------------------------------------------------------------------------
# Libraries
#
SERIALLIB = <Lib$Dir>.BrainLink.serial.o.BLserial
ZMODEMLIB = <Lib$Dir>.BrainLink.zmodem.o.BLzmodem

# -----------------------------------------------------------------------------
# Include files
#

INCLUDES = -Itbox:,C:,<Lib$Dir>.BrainLink

LIBS      = ${CLIB} ${TBOXLIB} ${EVENTLIB} ${WIMPLIB} ${SERIALLIB} ${ZMODEMLIB}
LIBSZ     = ${ROMCSTUBS} ${TBOXLIB} ${EVENTLIB} ${WIMPLIB} ${SERIALLIB}ZM ${SERIALLIB}ZM

DFLAGS   = 

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT = BLClient
APPNAME   = !BrainLink
TARGET    = aof.${COMPONENT}
APP       = abs.!RunImage

OBJS      = \
 o.Main\
 o.screen\
 o.buffer\
 o.gui\
 o.error\
 o.handler\
 o.script

OBJSZ     = oz.ModuleWrap \
 oz.Main\
 oz.screen\
 oz.buffer\
 oz.gui\
 oz.error\
 oz.handler\
 oz.script

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
	${CP} LocalRes:!Boot       ${RESAPP}.!Boot      ${CPFLAGS}
	${CP} LocalRes:!Run        ${RESAPP}.!Run       ${CPFLAGS}
	${CP} LocalRes:!Sprites22  ${RESAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Res         ${RESAPP}.Res        ${CPFLAGS}
	${CP} LocalRes:Messages    ${RESAPP}.Messages   ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied.

install: all
	${MKDIR} ${INSTALLDIR}
	${CP} LocalRes:!Boot      ${INSTALLDIR}.!Boot      ${CPFLAGS}
	${CP} LocalRes:!Run       ${INSTALLDIR}.!Run       ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTALLDIR}.!Sprites22 ${CPFLAGS}	
	${CP} LocalRes:Res        ${INSTALLDIR}.Res        ${CPFLAGS}
	${CP} LocalRes:!Help      ${INSTALLDIR}.!Help      ${CPFLAGS}
	${CP} ${APP}              ${INSTALLDIR}.!RunImage  ${CPFLAGS}
	${CP} LocalRes:Messages   ${INSTALLDIR}.Messages   ${CPFLAGS}
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
