# Makefile for Chrontel7003
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name    Description
# ----       ----    -----------
# 18-Jul-00  RJC     Created.

#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
LD      = link
CP      = copy
RM      = remove
WIPE    = -wipe
ASFLAGS = -ABSolute -depend !Depend ${THROWBACK} -o $@
LDFLAGS = -rmf -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = Chron7003
SOURCE    = s.GetAll
OBJECT    = o.Chrontel
TARGET    = rm.Chrontel
EXPORTS   = 

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
        ${RM} ${TARGET}
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${OBJECT}: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
        ${MKDIR} rm
        ${LD} ${LDFLAGS} ${OBJECT}
        Access $@ rw/r

# Dynamic dependencies:
