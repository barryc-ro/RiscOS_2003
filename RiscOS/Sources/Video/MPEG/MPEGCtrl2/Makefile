# Makefile for MPEGControl v2
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name    Description
# ----       ----    -----------
# 25-Nov-98  BJGA    Created.
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
STRIPDEP= -stripdepnd
ASFLAGS = -ABSolute -depend !Depend ${THROWBACK} ${STANDALONE} -o $@
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = MPEGCtrl
SOURCE    = s.${COMPONENT}
OBJECT    = o.${COMPONENT}
TARGET    = rm.${COMPONENT}

#
# Generic rules:
#
ram: ${TARGET} messages-${CMDHELP}
        ${RM} Messages
        @echo ${COMPONENT}: ram module built

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

clean:
        ${RM} Messages
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${STRIPDEP}
        @echo ${COMPONENT}: cleaned

resources: messages-${CMDHELP}
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
        ${RM} Messages
        @echo ${COMPONENT}: resource files copied

messages-:
        ${CP} LocalRes:Messages Messages ${CPFLAGS}
        print LocalRes:CmdHelp { >> Messages }

messages-None:
        ${CP} LocalRes:Messages Messages ${CPFLAGS}

${OBJECT}: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
        ${MKDIR} rm
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ Module
        Access $@ rw/r

# Dynamic dependencies:
