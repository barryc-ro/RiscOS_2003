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
SOURCE     = s.init
SOURCESA   = s.standalone
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
Serial: s.init
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
rm.Serial: Hdr:ListOpts
rm.Serial: Hdr:NdrDebug
rm.Serial: Hdr:Macros
rm.Serial: Hdr:System
rm.Serial: Hdr:SWIs
rm.Serial: Hdr:CPU.Generic26
rm.Serial: Hdr:IO.GenericIO
rm.Serial: Hdr:RISCOS
rm.Serial: Hdr:Machine.<Machine>
rm.Serial: Hdr:ModHand
rm.Serial: Hdr:HighFSI
rm.Serial: Hdr:DevNos
rm.Serial: Hdr:Services
rm.Serial: Hdr:Symbols
rm.Serial: Hdr:MsgTrans
rm.Serial: Hdr:Proc
rm.Serial: Hdr:Buffer
rm.Serial: Hdr:ResourceFS
rm.Serial: Hdr:DeviceFS
rm.Serial: Hdr:IO.IOEB
rm.Serial: Hdr:IO.IOC
rm.Serial: Hdr:RS423
rm.Serial: Hdr:IO.IOMDL
rm.Serial: Version
rm.Serial: s.errors
rm.Serial: s.macros
rm.Serial: s.main
rm.Serial: s.hardware
rm.Serial: s.interrupts
rm.Serial: s.devicecall
