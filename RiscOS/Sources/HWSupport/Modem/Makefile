# Makefile for Modem driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 1996-09-20 RWB          Created
# 1997-04-24 RWB          Made machine aware
# 1997-05-23 RWB          Changed debug build
# 1999-04-22 DC           Altered to use LocalRes
# 1999-11-23 SBF          Added support for StripDepnd tool
#

#
# Paths
#
EXP_HDR = <export$dir>
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
WIPE    = -wipe
STRIP   = stripdepnd
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -Throwback -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

#
# Program specific options:
#
COMPONENT  = Modem
SOURCE     = s.init
SOURCESA   = s.standalone
SOURCED    = s.debug
TARGET     = rm.${MACHINE}.${COMPONENT}
TARGETSA   = rm.${MACHINE}.${COMPONENT}SA
TARGETD    = rm.${MACHINE}.${COMPONENT}D
EXPORTS    = ${EXP_C_H}.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${RM} ${TARGET}
        ${RM} ${TARGETSA}
        ${RM} ${TARGETD}
        ${WIPE} rm.${MACHINE} ${WFLAGS}
        ${STRIP}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

all: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

debug: ${TARGETD}
        @echo ${COMPONENT}: standalone module built

${TARGET}: ${SOURCE}
        ${MKDIR} rm
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGETSA}: ${SOURCESA}
        ${MKDIR} rm
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCESA}

${TARGETD}: ${SOURCED}
        ${MKDIR} rm
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCED}

# Dynamic dependencies:
