# Makefile for UART
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 24/02/95  Rich Buckley  Modified from Serial makefile.
# 10/01/96  Rich	  Changes stand alone filename to UARTDriver
# 30/01/96  Rich	  Removed -Throwback
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
COMPONENT  = UART
SOURCE     = s.init
SOURCESA   = s.standalone
TARGET     = rm.${MACHINE}.UART
TARGETSA   = rm.${MACHINE}.UARTDriver
RESOURCES  = Resources.${LOCALE}.Messages

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
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# Makes a stand-alone version with messages bound in
#
all: ${TARGETSA}
        @echo ${COMPONENT}: Stand alone module built

${TARGET}: ${SOURCE}
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGETSA}: ${SOURCESA}
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCESA}
#
# Exported headers
#

# Dynamic dependencies:
rm.UART_SA: s.init
rm.UART_SA: Hdr:ListOpts
rm.UART_SA: Hdr:NdrDebug
rm.UART_SA: Hdr:Macros
rm.UART_SA: Hdr:System
rm.UART_SA: Hdr:SWIs
rm.UART_SA: Hdr:CPU.Generic26
rm.UART_SA: Hdr:IO.GenericIO
rm.UART_SA: Hdr:RISCOS
rm.UART_SA: Hdr:Machine.<Machine>
rm.UART_SA: Hdr:ModHand
rm.UART_SA: Hdr:HighFSI
rm.UART_SA: Hdr:DevNos
rm.UART_SA: Hdr:Services
rm.UART_SA: Hdr:Symbols
rm.UART_SA: Hdr:UpCall
rm.UART_SA: Hdr:CMOS
rm.UART_SA: Hdr:DeviceFS
rm.UART_SA: Hdr:Serial
rm.UART_SA: Hdr:RS423
rm.UART_SA: Hdr:Buffer
rm.UART_SA: Hdr:MsgTrans
rm.UART_SA: Hdr:Proc
rm.UART_SA: Hdr:ResourceFS
rm.UART_SA: Hdr:Podule
rm.UART_SA: Hdr:IO.IOMDL
rm.UART_SA: Version
rm.UART_SA: s.errors
rm.UART_SA: s.main
rm.UART_SA: s.hardware
rm.UART_SA: s.devicecall
rm.UART_SA: s.swicall
rm.UART_SA: s.interrupts
rm.UART_SA: s.upcall
rm.UART: Hdr:ListOpts
rm.UART: Hdr:NdrDebug
rm.UART: Hdr:Macros
rm.UART: Hdr:System
rm.UART: Hdr:SWIs
rm.UART: Hdr:CPU.Generic26
rm.UART: Hdr:IO.GenericIO
rm.UART: Hdr:RISCOS
rm.UART: Hdr:Machine.<Machine>
rm.UART: Hdr:ModHand
rm.UART: Hdr:HighFSI
rm.UART: Hdr:DevNos
rm.UART: Hdr:Services
rm.UART: Hdr:Symbols
rm.UART: Hdr:UpCall
rm.UART: Hdr:CMOS
rm.UART: Hdr:DeviceFS
rm.UART: Hdr:Serial
rm.UART: Hdr:RS423
rm.UART: Hdr:Buffer
rm.UART: Hdr:MsgTrans
rm.UART: Hdr:Proc
rm.UART: Hdr:ResourceFS
rm.UART: Hdr:Podule
rm.UART: Hdr:IO.IOMDL
rm.UART: Version
rm.UART: s.errors
rm.UART: s.main
rm.UART: s.hardware
rm.UART: s.devicecall
rm.UART: s.swicall
rm.UART: s.interrupts
rm.UART: s.upcall
rm.UARTDriver: s.init
rm.UARTDriver: Hdr:ListOpts
rm.UARTDriver: Hdr:NdrDebug
rm.UARTDriver: Hdr:Macros
rm.UARTDriver: Hdr:System
rm.UARTDriver: Hdr:SWIs
rm.UARTDriver: Hdr:CPU.Generic26
rm.UARTDriver: Hdr:IO.GenericIO
rm.UARTDriver: Hdr:RISCOS
rm.UARTDriver: Hdr:Machine.<Machine>
rm.UARTDriver: Hdr:Machine.Machine
rm.UARTDriver: Hdr:ModHand
rm.UARTDriver: Hdr:HighFSI
rm.UARTDriver: Hdr:DevNos
rm.UARTDriver: Hdr:Services
rm.UARTDriver: Hdr:Symbols
rm.UARTDriver: Hdr:UpCall
rm.UARTDriver: Hdr:CMOS
rm.UARTDriver: Hdr:DeviceFS
rm.UARTDriver: Hdr:Serial
rm.UARTDriver: Hdr:RS423
rm.UARTDriver: Hdr:Buffer
rm.UARTDriver: Hdr:MsgTrans
rm.UARTDriver: Hdr:Proc
rm.UARTDriver: Hdr:ResourceFS
rm.UARTDriver: Hdr:Podule
rm.UARTDriver: Hdr:IO.IOMDL
rm.UARTDriver: Version
rm.UARTDriver: s.errors
rm.UARTDriver: s.main
rm.UARTDriver: s.macros
rm.UARTDriver: s.hardware
rm.UARTDriver: s.devicecall
rm.UARTDriver: s.swicall
rm.UARTDriver: s.interrupts
rm.UARTDriver: s.upcall
rm.Omega.UARTDriver: s.init
rm.Omega.UARTDriver: Hdr:ListOpts
rm.Omega.UARTDriver: Hdr:NdrDebug
rm.Omega.UARTDriver: Hdr:Macros
rm.Omega.UARTDriver: Hdr:System
rm.Omega.UARTDriver: Hdr:SWIs
rm.Omega.UARTDriver: Hdr:CPU.Generic26
rm.Omega.UARTDriver: Hdr:IO.GenericIO
rm.Omega.UARTDriver: Hdr:RISCOS
rm.Omega.UARTDriver: Hdr:Machine.<Machine>
rm.Omega.UARTDriver: Hdr:Machine.Machine
rm.Omega.UARTDriver: Hdr:ModHand
rm.Omega.UARTDriver: Hdr:HighFSI
rm.Omega.UARTDriver: Hdr:DevNos
rm.Omega.UARTDriver: Hdr:Services
rm.Omega.UARTDriver: Hdr:Symbols
rm.Omega.UARTDriver: Hdr:UpCall
rm.Omega.UARTDriver: Hdr:CMOS
rm.Omega.UARTDriver: Hdr:DeviceFS
rm.Omega.UARTDriver: Hdr:Serial
rm.Omega.UARTDriver: Hdr:RS423
rm.Omega.UARTDriver: Hdr:Buffer
rm.Omega.UARTDriver: Hdr:MsgTrans
rm.Omega.UARTDriver: Hdr:Proc
rm.Omega.UARTDriver: Hdr:ResourceFS
rm.Omega.UARTDriver: Hdr:Podule
rm.Omega.UARTDriver: Hdr:IO.IOMDL
rm.Omega.UARTDriver: Version
rm.Omega.UARTDriver: s.errors
rm.Omega.UARTDriver: s.main
rm.Omega.UARTDriver: s.macros
rm.Omega.UARTDriver: s.hardware
rm.Omega.UARTDriver: s.devicecall
rm.Omega.UARTDriver: s.swicall
rm.Omega.UARTDriver: s.interrupts
rm.Omega.UARTDriver: s.upcall
rm.Boca.UARTDriver: s.init
rm.Boca.UARTDriver: Hdr:ListOpts
rm.Boca.UARTDriver: Hdr:NdrDebug
rm.Boca.UARTDriver: Hdr:Macros
rm.Boca.UARTDriver: Hdr:System
rm.Boca.UARTDriver: Hdr:SWIs
rm.Boca.UARTDriver: Hdr:CPU.Generic26
rm.Boca.UARTDriver: Hdr:IO.GenericIO
rm.Boca.UARTDriver: Hdr:RISCOS
rm.Boca.UARTDriver: Hdr:Machine.<Machine>
rm.Boca.UARTDriver: Hdr:Machine.Machine
rm.Boca.UARTDriver: Hdr:ModHand
rm.Boca.UARTDriver: Hdr:HighFSI
rm.Boca.UARTDriver: Hdr:DevNos
rm.Boca.UARTDriver: Hdr:Services
rm.Boca.UARTDriver: Hdr:Symbols
rm.Boca.UARTDriver: Hdr:UpCall
rm.Boca.UARTDriver: Hdr:CMOS
rm.Boca.UARTDriver: Hdr:DeviceFS
rm.Boca.UARTDriver: Hdr:Serial
rm.Boca.UARTDriver: Hdr:RS423
rm.Boca.UARTDriver: Hdr:Buffer
rm.Boca.UARTDriver: Hdr:MsgTrans
rm.Boca.UARTDriver: Hdr:Proc
rm.Boca.UARTDriver: Hdr:ResourceFS
rm.Boca.UARTDriver: Hdr:Podule
rm.Boca.UARTDriver: Hdr:IO.IOMDL
rm.Boca.UARTDriver: Version
rm.Boca.UARTDriver: s.errors
rm.Boca.UARTDriver: s.main
rm.Boca.UARTDriver: s.macros
rm.Boca.UARTDriver: s.hardware
rm.Boca.UARTDriver: s.devicecall
rm.Boca.UARTDriver: s.swicall
rm.Boca.UARTDriver: s.interrupts
rm.Boca.UARTDriver: s.upcall
rm.Boca.UART: Hdr:ListOpts
rm.Boca.UART: Hdr:NdrDebug
rm.Boca.UART: Hdr:Macros
rm.Boca.UART: Hdr:System
rm.Boca.UART: Hdr:SWIs
rm.Boca.UART: Hdr:CPU.Generic26
rm.Boca.UART: Hdr:IO.GenericIO
rm.Boca.UART: Hdr:RISCOS
rm.Boca.UART: Hdr:Machine.<Machine>
rm.Boca.UART: Hdr:Machine.Machine
rm.Boca.UART: Hdr:ModHand
rm.Boca.UART: Hdr:HighFSI
rm.Boca.UART: Hdr:DevNos
rm.Boca.UART: Hdr:Services
rm.Boca.UART: Hdr:Symbols
rm.Boca.UART: Hdr:UpCall
rm.Boca.UART: Hdr:CMOS
rm.Boca.UART: Hdr:DeviceFS
rm.Boca.UART: Hdr:Serial
rm.Boca.UART: Hdr:RS423
rm.Boca.UART: Hdr:Buffer
rm.Boca.UART: Hdr:MsgTrans
rm.Boca.UART: Hdr:Proc
rm.Boca.UART: Hdr:ResourceFS
rm.Boca.UART: Hdr:Podule
rm.Boca.UART: Hdr:IO.IOMDL
rm.Boca.UART: Version
rm.Boca.UART: s.errors
rm.Boca.UART: s.main
rm.Boca.UART: s.macros
rm.Boca.UART: s.hardware
rm.Boca.UART: s.devicecall
rm.Boca.UART: s.swicall
rm.Boca.UART: s.interrupts
rm.Boca.UART: s.upcall
rm.Omega.UART: Hdr:ListOpts
rm.Omega.UART: Hdr:NdrDebug
rm.Omega.UART: Hdr:Macros
rm.Omega.UART: Hdr:System
rm.Omega.UART: Hdr:SWIs
rm.Omega.UART: Hdr:CPU.Generic26
rm.Omega.UART: Hdr:IO.GenericIO
rm.Omega.UART: Hdr:RISCOS
rm.Omega.UART: Hdr:Machine.<Machine>
rm.Omega.UART: Hdr:Machine.Machine
rm.Omega.UART: Hdr:ModHand
rm.Omega.UART: Hdr:HighFSI
rm.Omega.UART: Hdr:DevNos
rm.Omega.UART: Hdr:Services
rm.Omega.UART: Hdr:Symbols
rm.Omega.UART: Hdr:UpCall
rm.Omega.UART: Hdr:CMOS
rm.Omega.UART: Hdr:DeviceFS
rm.Omega.UART: Hdr:Serial
rm.Omega.UART: Hdr:RS423
rm.Omega.UART: Hdr:Buffer
rm.Omega.UART: Hdr:MsgTrans
rm.Omega.UART: Hdr:Proc
rm.Omega.UART: Hdr:ResourceFS
rm.Omega.UART: Hdr:Podule
rm.Omega.UART: Hdr:IO.IOMDL
rm.Omega.UART: Version
rm.Omega.UART: s.errors
rm.Omega.UART: s.main
rm.Omega.UART: s.macros
rm.Omega.UART: s.hardware
rm.Omega.UART: s.devicecall
rm.Omega.UART: s.swicall
rm.Omega.UART: s.interrupts
rm.Omega.UART: s.upcall
