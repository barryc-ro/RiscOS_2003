# Makefile for CDSupport
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 28aug95    diz    	  Created
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = CDSupport
SOURCE    = s.CDSupport
TARGET    = rm.CDSupport
EXPORTS   = ${EXP_HDR}.${COMPONENT}

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
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.CDSupport: hdr:ListOpts
rm.CDSupport: hdr:Macros
rm.CDSupport: hdr:System
rm.CDSupport: Hdr:SWIs
rm.CDSupport: Hdr:CPU.Generic26
rm.CDSupport: Hdr:IO.GenericIO
rm.CDSupport: Hdr:RISCOS
rm.CDSupport: hdr:ModHand
rm.CDSupport: hdr:Services
rm.CDSupport: hdr:FSNumbers
rm.CDSupport: hdr:HighFSI
rm.CDSupport: hdr:UpCall
rm.CDSupport: hdr:Proc
rm.CDSupport: hdr:NDRDebug
rm.CDSupport: hdr:DDVMacros
rm.CDSupport: hdr:NewErrors
rm.CDSupport: hdr:STBState
rm.CDSupport: hdr:CDSupport
rm.CDSupport: Version
