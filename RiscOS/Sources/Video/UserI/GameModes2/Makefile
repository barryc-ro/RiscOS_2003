# Makefile for GameModes2
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

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
ASFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~CFR~V

DIRS    = rm._dirs

#
# Program specific options:
#
COMPONENT = GameModes2
SOURCE    = s.GameModes2
TARGET    = rm.GameModes2

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
        ${STRIP}
        @echo ${COMPONENT}: cleaned

${DIRS}:
        @${MKDIR} rm
        create ${DIRS}
        stamp  ${DIRS}

${TARGET}: ${SOURCE} ${DIRS}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
