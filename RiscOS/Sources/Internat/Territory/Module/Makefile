# Makefile for Territory modules
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
AS      = objasm
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
ASFLAGS = -depend !Depend ${THROWBACK} -PD "Territory SETS \"${COMPONENT}\"" -o $@
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WPFLAGS = ~cfr~v

#
# Program specific options:
#
SOURCE    = s.Territory
OBJECT    = o.${COMPONENT}
TARGET    = rm.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export:
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WPFLAGS}
        ${WIPE} rm ${WPFLAGS}
        @echo Territory modules: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck Resources.${COMPONENT}.Territory
	TokenCheck LocalRes:Messages
        ${CP} Resources.${COMPONENT}.Territory ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
	Print LocalRes:Messages { >> ${RESDIR}.${COMPONENT}.Messages }
        @echo ${COMPONENT}: resource files copied

${OBJECT}: ${SOURCE}
	${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
	${MKDIR} rm
	${LD} ${LDFLAGS} ${OBJECT}
	SetType ${TARGET} &FFA

# Dynamic dependencies:
