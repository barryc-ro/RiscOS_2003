# Makefile for DADebug
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 19-May-99  BJGA         Created.
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
LD      = link
CP      = copy
RM      = remove
WIPE    = -wipe
ASFLAGS = -absolute -depend !Depend -throwback
LDFLAGS = -bin
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = DADebug
SOURCE    = s.${COMPONENT}
OBJECT    = o.${COMPONENT}
TARGET    = rm.${COMPONENT}

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: module built

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        -stripdepnd
        @echo ${COMPONENT}: cleaned

${TARGET}: ${OBJECT}
        ${MKDIR} rm
        ${LD} ${LDFLAGS} -o $@ $?
        SetType $@ Module
        Access $@ RW/R

${OBJECT}: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} -o $@ ${SOURCE}

# Dynamic dependencies:
