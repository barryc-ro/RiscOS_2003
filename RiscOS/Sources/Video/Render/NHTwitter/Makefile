# Makefile for NHTwitter
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 31-Oct-95  TMD	  Created.
# 24-Feb-00  KJB          Adapted for multiple build variants.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
WIPE	= -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
WFLAGS  = ~cr~v

#
# Program specific options:
#
COMPONENT = NHTwitter
SOURCE    = Hdr.T${TYPE}
TARGET    = rm.T${TYPE}.NHTwitter

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

all: ${TARGET} ${TARGETM}
	@echo ${COMPONENT}: disc modules built

install:
	@echo ${COMPONENT}: disc modules installed

clean:
	${WIPE} rm.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Variants
${TARGET}: ${SOURCE}
	${MKDIR} rm.T${TYPE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
