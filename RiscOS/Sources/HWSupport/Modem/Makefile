# Makefile for Modem driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 20 Sep 1996 Rich Buckley  Created
# 24 Apr 1997 Rich Buckley  Made machine aware
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
ASFLAGS = -depend !Depend -Stamp -quit -module -Throwback -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = Modem
SOURCE     = s.init
SOURCESA   = s.standalone
TARGET     = rm.${MACHINE}.${COMPONENT}
TARGETSA   = ${COMPONENT}
EXPORTS    = ${EXP_C_H}.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

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
        ${MKDIR} rm
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGETSA}: ${SOURCESA}
        ${AS} ${ASFLAGS} ${SOURCESA}

# Dynamic dependencies:
Modem: s.init
Modem: Hdr:ListOpts
Modem: Hdr:NdrDebug
Modem: Hdr:Macros
Modem: Hdr:System
Modem: Hdr:SWIs
Modem: Hdr:CPU.Generic26
Modem: Hdr:IO.GenericIO
Modem: Hdr:RISCOS
Modem: Hdr:Machine.<Machine>
Modem: Hdr:Machine.Machine
Modem: Hdr:ModHand
Modem: Hdr:HighFSI
Modem: Hdr:DevNos
Modem: Hdr:Services
Modem: Hdr:Symbols
Modem: Hdr:MsgTrans
Modem: Hdr:Proc
Modem: Hdr:Buffer
Modem: Hdr:ResourceFS
Modem: Hdr:Podule
Modem: Hdr:DeviceFS
Modem: Hdr:IO.IOEB
Modem: Hdr:IO.IOC
Modem: Hdr:RS423
Modem: Hdr:IO.IOMDL
Modem: Version
Modem: s.errors
Modem: s.macros
Modem: s.main
Modem: s.hardware
Modem: s.interrupts
Modem: s.devicecall
rm.Omega.Modem: Hdr:ListOpts
rm.Omega.Modem: Hdr:NdrDebug
rm.Omega.Modem: Hdr:Macros
rm.Omega.Modem: Hdr:System
rm.Omega.Modem: Hdr:SWIs
rm.Omega.Modem: Hdr:CPU.Generic26
rm.Omega.Modem: Hdr:IO.GenericIO
rm.Omega.Modem: Hdr:RISCOS
rm.Omega.Modem: Hdr:Machine.<Machine>
rm.Omega.Modem: Hdr:Machine.Machine
rm.Omega.Modem: Hdr:ModHand
rm.Omega.Modem: Hdr:HighFSI
rm.Omega.Modem: Hdr:DevNos
rm.Omega.Modem: Hdr:Services
rm.Omega.Modem: Hdr:Symbols
rm.Omega.Modem: Hdr:MsgTrans
rm.Omega.Modem: Hdr:Proc
rm.Omega.Modem: Hdr:Buffer
rm.Omega.Modem: Hdr:ResourceFS
rm.Omega.Modem: Hdr:Podule
rm.Omega.Modem: Hdr:DeviceFS
rm.Omega.Modem: Hdr:IO.IOEB
rm.Omega.Modem: Hdr:IO.IOC
rm.Omega.Modem: Hdr:RS423
rm.Omega.Modem: Hdr:IO.IOMDL
rm.Omega.Modem: Version
rm.Omega.Modem: s.errors
rm.Omega.Modem: s.macros
rm.Omega.Modem: s.main
rm.Omega.Modem: s.hardware
rm.Omega.Modem: s.interrupts
rm.Omega.Modem: s.devicecall
rm.Boca.Modem: Hdr:ListOpts
rm.Boca.Modem: Hdr:NdrDebug
rm.Boca.Modem: Hdr:Macros
rm.Boca.Modem: Hdr:System
rm.Boca.Modem: Hdr:SWIs
rm.Boca.Modem: Hdr:CPU.Generic26
rm.Boca.Modem: Hdr:IO.GenericIO
rm.Boca.Modem: Hdr:RISCOS
rm.Boca.Modem: Hdr:Machine.<Machine>
rm.Boca.Modem: Hdr:Machine.Machine
rm.Boca.Modem: Hdr:ModHand
rm.Boca.Modem: Hdr:HighFSI
rm.Boca.Modem: Hdr:DevNos
rm.Boca.Modem: Hdr:Services
rm.Boca.Modem: Hdr:Symbols
rm.Boca.Modem: Hdr:MsgTrans
rm.Boca.Modem: Hdr:Proc
rm.Boca.Modem: Hdr:Buffer
rm.Boca.Modem: Hdr:ResourceFS
rm.Boca.Modem: Hdr:Podule
rm.Boca.Modem: Hdr:DeviceFS
rm.Boca.Modem: Hdr:IO.IOEB
rm.Boca.Modem: Hdr:IO.IOC
rm.Boca.Modem: Hdr:RS423
rm.Boca.Modem: Hdr:IO.IOMDL
rm.Boca.Modem: Version
rm.Boca.Modem: s.errors
rm.Boca.Modem: s.macros
rm.Boca.Modem: s.main
rm.Boca.Modem: s.hardware
rm.Boca.Modem: s.interrupts
rm.Boca.Modem: s.devicecall
