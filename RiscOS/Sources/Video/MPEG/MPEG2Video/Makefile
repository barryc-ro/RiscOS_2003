# Makefile for MPEG2Video
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-94  JRH          Created.
# 13-Feb-95  TMD	  Updated with VideoM.
# 13-Feb-95  TMD	  Added install option for making !M-Code21.
# 28-Apr-95  TMD	  Adapted from MPEGVideo version.
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
ASFLAGS = -depend !Depend -Stamp -quit -module -pd "tvtype SETS \"${TV}\""
CPFLAGS = ~cfr~v
LDFLAGS = -bin -o ${TARGET}
WFLAGS  = ~c~fr~v

#
# Program specific options:
#
COMPONENT = MPEG2Video
SOURCE    = Hdr.${COMPONENT}
OBJECT    = o.${COMPONENT}
TARGET    = rm.${COMPONENT}
SETUP	  = s.Setup.352x288
SETUPC	  = s.Setup.352x288C

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

all: ${TARGET}
	@echo ${COMPONENT}: disc modules built

install: ${TARGET} install_dirs
        ${MKDIR} ${INSTDIR}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: disc modules installed

install_dirs:
    ${MKDIR} ${INSTDIR}

clean:
        ${RM} ${TARGET}
        ${RM} s.Data.BinSetup
        ${RM} s.Data.BinSetupC
        ${WIPE} o ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE} s.Data.BinSetupC
        ${MKDIR} o
        ${AS} -o ${OBJECT} ${SOURCE} ${ASFLAGS}
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ Module

s.Data.BinSetup: ${SETUP} Tools.ConvSetup
	/Tools.ConvSetup ${SETUP} s.Data.BinSetup

s.Data.BinSetupC: ${SETUPC} Tools.ConvSetup
	/Tools.ConvSetup ${SETUPC} s.Data.BinSetupC

# Dynamic dependencies:
