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
TARGET     = rm.UART
TARGETSA   = rm.UARTDriver
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
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGETSA}: ${SOURCESA}
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
