# Makefile for MPEGTransport
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 15-Mar-95  TMD	  Created
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = objasm
LD      = link
CP      = copy
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -ABSolute -depend !Depend ${THROWBACK} -o ${OBJECT}
LDFLAGS = -bin -o ${TARGET}
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = MPEGTrans
SOURCE    = Hdr.GetAll
TARGET    = rm.MPEGTrans
OBJECT    = o.MPEGTrans

#
# Generic rules:
#

all: ${TARGET}
        @echo ${COMPONENT}: disc module built

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: disc module installed

rom: ${TARGET}
	@echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${WIPE} o ${WFLAGS}
        -stripdepnd
        @echo ${COMPONENT}: cleaned

${TARGET}: ${OBJECT}
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ Module
        Access $@ rw/r

${OBJECT}: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
