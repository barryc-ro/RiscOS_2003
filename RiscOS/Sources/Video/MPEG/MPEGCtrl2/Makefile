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
ASFLAGS = -ABSolute -depend !Depend ${THROWBACK} -o $@
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = MPEGCtrl
SOURCE    = s.${COMPONENT}
OBJECT    = o.${COMPONENT}
SAOBJECT  = o.${COMPONENT}SA
TARGET    = rm.${COMPONENT}
SATARGET  = rm.${COMPONENT}SA

#
# Generic rules:
#
ram: ${SATARGET} messages-${CMDHELP}
        ${RM} Messages
        @echo ${COMPONENT}: ram module built

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

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

${SAOBJECT}: ${SOURCE}
        ${MKDIR} o
        ${AS} -PreDefine "Standalone SETL {TRUE}" ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
        ${MKDIR} rm
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ Module
        Access $@ rw/r

${SATARGET}: ${SAOBJECT}
        ${MKDIR} rm
        ${LD} ${LDFLAGS} ${SAOBJECT}
        SetType $@ Module
        Access $@ rw/r

# Dynamic dependencies:
