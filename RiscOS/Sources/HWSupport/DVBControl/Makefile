# Makefile for DVBControl
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 07/12/95  Rich Buckley   Modified from UARTDeviceDriver
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
COMPONENT  = DVBControl
SOURCE     = s.init
TARGET     = rm.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

all: ${TARGET}
        @echo ${COMPONENT}: module built

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

        
# Dynamic dependencies:
rm.DVBControl: Hdr:ListOpts
rm.DVBControl: Hdr:NdrDebug
rm.DVBControl: Hdr:Macros
rm.DVBControl: Hdr:System
rm.DVBControl: Hdr:SWIs
rm.DVBControl: Hdr:CPU.Generic26
rm.DVBControl: Hdr:IO.GenericIO
rm.DVBControl: Hdr:RISCOS
rm.DVBControl: Hdr:Machine.<Machine>
rm.DVBControl: Hdr:Machine.<Machine>
rm.DVBControl: Hdr:ModHand
rm.DVBControl: Hdr:HighFSI
rm.DVBControl: Hdr:DevNos
rm.DVBControl: Hdr:Services
rm.DVBControl: Hdr:Symbols
rm.DVBControl: Hdr:MsgTrans
rm.DVBControl: Hdr:Proc
rm.DVBControl: Hdr:Podule
rm.DVBControl: Hdr:IO.IOMDL
rm.DVBControl: Hdr:MPEGCtrl
rm.DVBControl: Version
rm.DVBControl: s.errors
rm.DVBControl: s.main
rm.DVBControl: s.hardware
rm.DVBControl: s.interrupts
rm.DVBControl: s.swicall
