# Makefile for SerialFlash
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 02-Apr-97  SBF          Created.
#

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
COMPONENT = SerFlash
SOURCE    = s.SerFlash
TARGET    = rm.SerFlash

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.SerFlash: Hdr:ListOpts
rm.SerFlash: Hdr:Macros
rm.SerFlash: Hdr:DDVMacros
rm.SerFlash: Hdr:System
rm.SerFlash: Hdr:SWIs
rm.SerFlash: Hdr:CPU.Generic26
rm.SerFlash: Hdr:IO.GenericIO
rm.SerFlash: Hdr:RISCOS
rm.SerFlash: Hdr:ModHand
rm.SerFlash: Hdr:FSNumbers
rm.SerFlash: Hdr:NewErrors
rm.SerFlash: Hdr:CMOS
rm.SerFlash: Hdr:Symbols
rm.SerFlash: Hdr:Services
rm.SerFlash: Hdr:DeviceFS
rm.SerFlash: Hdr:Serial
rm.SerFlash: Hdr:RS423
rm.SerFlash: Hdr:HostFS
rm.SerFlash: Hdr:NdrDebug
rm.SerFlash: Hdr:MsgTrans
rm.SerFlash: Hdr:UpCall
rm.SerFlash: Hdr:Proc
rm.SerFlash: Version
rm.SerFlash: s.Macros
rm.SerFlash: s.FlashErrs
