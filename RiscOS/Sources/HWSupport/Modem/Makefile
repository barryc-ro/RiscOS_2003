# Makefile for Modem driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 20 Sep 1996 Rich Buckley  Created
# 24 Apr 1997 Rich Buckley  Made machine aware
# 23 May 1997 Rich Buckley  Changed debug build
# 22 Apr 1999 David Cotton  Altered to use LocalRes
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
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -Throwback -To $@ -From
CPFLAGS = ~cfr~v

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
