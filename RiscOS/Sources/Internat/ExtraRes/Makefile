# Makefile for International Keyboard
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 08-Sep-98  KJB          Created.
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC	= cc
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit -To $@ -From
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = ExtraRes
SOURCE    = s.ExtraRes
TARGET    = rm.ExtraRes
OBJECT    = o.ExtraRes

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
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${OBJECT}: ${SOURCE} ${LAYOUTS}
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
	${MKDIR} rm
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ &FFA

# Dynamic dependencies:
