# Makefile for MPEGControl
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 07-Sep-00  BJGA	  Created from MPEGTransport makefile
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
ASFLAGS = -depend !Depend ${THROWBACK} -o ${OBJECT}
LDFLAGS = -bin -o ${TARGET}
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = MPEGCtrl
SOURCE    = Hdr.GetAll
TARGET    = rm.MPEGCtrl
OBJECT    = o.MPEGCtrl

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
