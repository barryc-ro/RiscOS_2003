# Makefile for SerialFlash
# ========================
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 19-Feb-98  SBF          Created
#

# -----------------------------------------------------------------------------
# Generic options:
#
MKDIR           = cdir
AS              = aasm
CP              = copy
RM              = remove
WIPE            = -wipe

CCFLAGS         = -c -depend !Depend -IC:
ASFLAGS         = ${THROWBACK} -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS         = ~cfr~v~n
WFLAGS          = ~cr~vr

# -----------------------------------------------------------------------------
# Program specific options:
#
COMPONENT       = SerFlash
SOURCE          = s.SerFlash
TARGET          = rm.${COMPONENT}

# -----------------------------------------------------------------------------
# Generic rules:
#

all:    ${TARGET}
        @echo ${COMPONENT}: module built

rom:    ${TARGET}
        @echo ${COMPONENT}: ROM module built

export:
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ROM module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

clean_all:
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned all

o.dirs:
        @${MKDIR} rm

${TARGET}: o.dirs
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
