# Makefile for AudioCtrl
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-May-96  amg          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = aasm
CP      = copy
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = AudioCtrl
SOURCE    = s.GetAll
TARGET    = rm.${COMPONENT}
TARGETSA  = rm.StandAlone.${COMPONENT}
EXPORTS   = ${EXP_HDR}.${COMPONENT}
RESOURCES = LocalRes:Messages

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${MKDIR} rm
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

#
# Makes a stand-alone version with messages bound in
#
standalone: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

${TARGETSA}: s.StandAlone ${RESOURCES}
        ${MKDIR} rm.StandAlone
        ${AS} ${ASFLAGS} s.StandAlone

# Dynamic dependencies:
