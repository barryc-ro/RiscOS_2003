# Makefile for Modem driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 20 Sep 1996 Rich Buckley  Created
# 24 Apr 1997 Rich Buckley  Made machine aware
# 23 May 1997 Rich Buckley  Changed debug build
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
SOURCED    = s.debug
TARGET     = rm.${MACHINE}.${COMPONENT}
TARGETSA   = rm.${MACHINE}.${COMPONENT}SA
TARGETD    = rm.${MACHINE}.${COMPONENT}D
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
        ${RM} ${TARGETD}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

all: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

debug: ${TARGETD}
        @echo ${COMPONENT}: standalone module built

${TARGET}: ${SOURCE}
        ${MKDIR} rm
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGETSA}: ${SOURCESA}
        ${MKDIR} rm
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCESA}

${TARGETD}: ${SOURCED}
        ${MKDIR} rm
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCED}

# Dynamic dependencies:
rm.Boca.ModemSA: s.init
rm.Boca.ModemSA: Hdr:ListOpts
rm.Boca.ModemSA: Hdr:NdrDebug
rm.Boca.ModemSA: Hdr:Macros
rm.Boca.ModemSA: Hdr:System
rm.Boca.ModemSA: Hdr:SWIs
rm.Boca.ModemSA: Hdr:CPU.Generic26
rm.Boca.ModemSA: Hdr:IO.GenericIO
rm.Boca.ModemSA: Hdr:RISCOS
rm.Boca.ModemSA: Hdr:Machine.<Machine>
rm.Boca.ModemSA: Hdr:Machine.Machine
rm.Boca.ModemSA: Hdr:ModHand
rm.Boca.ModemSA: Hdr:HighFSI
rm.Boca.ModemSA: Hdr:DevNos
rm.Boca.ModemSA: Hdr:Services
rm.Boca.ModemSA: Hdr:Symbols
rm.Boca.ModemSA: Hdr:MsgTrans
rm.Boca.ModemSA: Hdr:Proc
rm.Boca.ModemSA: Hdr:Buffer
rm.Boca.ModemSA: Hdr:ResourceFS
rm.Boca.ModemSA: Hdr:Podule
rm.Boca.ModemSA: Hdr:DeviceFS
rm.Boca.ModemSA: Hdr:IO.IOEB
rm.Boca.ModemSA: Hdr:IO.IOC
rm.Boca.ModemSA: Hdr:RS423
rm.Boca.ModemSA: Hdr:IO.IOMDL
rm.Boca.ModemSA: VersionNum
rm.Boca.ModemSA: s.errors
rm.Boca.ModemSA: s.macros
rm.Boca.ModemSA: s.main
rm.Boca.ModemSA: s.hardware
rm.Boca.ModemSA: s.interrupts
rm.Boca.ModemSA: s.devicecall
