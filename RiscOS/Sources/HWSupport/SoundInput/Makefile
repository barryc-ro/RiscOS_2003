# Makefile for SoundInput
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 29-Mar-96 Rich Buckley  Created
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
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = SoundInput
SOURCE     = s.init
SOURCESA   = s.standalone
TARGET     = rm.${COMPONENT}
TARGETSA   = ${COMPONENT}
EXPORTS    = ${EXP_HDR}.${COMPONENT}

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

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${RM} ${TARGET}
        ${RM} ${TARGETSA}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

all: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}
        
${TARGETSA}: ${SOURCESA}
        ${AS} ${ASFLAGS} ${SOURCESA}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# Dynamic dependencies:
SoundInput: s.init
SoundInput: Hdr:ListOpts
SoundInput: Hdr:NdrDebug
SoundInput: Hdr:Macros
SoundInput: Hdr:System
SoundInput: Hdr:SWIs
SoundInput: Hdr:CPU.Generic26
SoundInput: Hdr:IO.GenericIO
SoundInput: Hdr:RISCOS
SoundInput: Hdr:Machine.<Machine>
SoundInput: Hdr:ModHand
SoundInput: Hdr:HighFSI
SoundInput: Hdr:DevNos
SoundInput: Hdr:Services
SoundInput: Hdr:Symbols
SoundInput: Hdr:MsgTrans
SoundInput: Hdr:Proc
SoundInput: Hdr:Buffer
SoundInput: Hdr:ResourceFS
SoundInput: Hdr:IO.IOMDL
SoundInput: Version
SoundInput: s.errors
SoundInput: s.main
SoundInput: s.hardware
SoundInput: s.interrupts
SoundInput: s.swicall
rm.SoundInput: Hdr:ListOpts
rm.SoundInput: Hdr:NdrDebug
rm.SoundInput: Hdr:Macros
rm.SoundInput: Hdr:System
rm.SoundInput: Hdr:SWIs
rm.SoundInput: Hdr:CPU.Generic26
rm.SoundInput: Hdr:IO.GenericIO
rm.SoundInput: Hdr:RISCOS
rm.SoundInput: Hdr:Machine.<Machine>
rm.SoundInput: Hdr:ModHand
rm.SoundInput: Hdr:HighFSI
rm.SoundInput: Hdr:DevNos
rm.SoundInput: Hdr:Services
rm.SoundInput: Hdr:Symbols
rm.SoundInput: Hdr:MsgTrans
rm.SoundInput: Hdr:Proc
rm.SoundInput: Hdr:Buffer
rm.SoundInput: Hdr:ResourceFS
rm.SoundInput: Hdr:IO.IOMDL
rm.SoundInput: Version
rm.SoundInput: s.errors
rm.SoundInput: s.main
rm.SoundInput: s.hardware
rm.SoundInput: s.interrupts
rm.SoundInput: s.swicall
