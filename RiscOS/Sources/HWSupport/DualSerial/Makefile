# Makefile for Serial
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 29-Mar-96 Rich Buckley  Created
# 09-Sep-96 Rich Buckley  Added export
#

#
# Paths
#
EXP_HDR = <export$dir>
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

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
COMPONENT  = Serial
SOURCE     = s.GetAll
SOURCESA   = s.Standalone
TARGET     = rm.${COMPONENT}
TARGETSA   = ${COMPONENT}
EXPORTS    = ${EXP_C_H}.${COMPONENT}

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
        
${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
Serial: s.GetAll
Serial: Hdr:ListOpts
Serial: Hdr:NdrDebug
Serial: Hdr:Macros
Serial: Hdr:System
Serial: Hdr:SWIs
Serial: Hdr:CPU.Generic26
Serial: Hdr:IO.GenericIO
Serial: Hdr:RISCOS
Serial: Hdr:Machine.<Machine>
Serial: Hdr:ModHand
Serial: Hdr:HighFSI
Serial: Hdr:DevNos
Serial: Hdr:Services
Serial: Hdr:Symbols
Serial: Hdr:MsgTrans
Serial: Hdr:Proc
Serial: Hdr:Buffer
Serial: Hdr:ResourceFS
Serial: Hdr:DeviceFS
Serial: Hdr:IO.IOEB
Serial: Hdr:IO.IOC
Serial: Hdr:RS423
Serial: Hdr:IO.IOMDL
Serial: Version
Serial: s.errors
Serial: s.macros
Serial: s.main
Serial: s.hardware
Serial: s.interrupts
Serial: s.devicecall
Serial: s.IOCtls
