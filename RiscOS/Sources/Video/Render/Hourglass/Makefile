# Makefile for Hourglass
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
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
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = Hourglass
SOURCE    = NewOS
TARGET    = rm.Hourglass
EXPORTS   = ${EXP_HDR}.${COMPONENT}

SOURCE200 = OldOS
TARGET200 = rm.Hourgls200

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
        ${RM} ${TARGET200}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

#
# Extra Development rules:
#
install: ${TARGET}
        Access ${TARGET200} R/r
        Access ${TARGET} R/r
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ${TARGET} installed

${TARGET200}: ${SOURCE200}
        ${AS} ${ASFLAGS} -From ${SOURCE200} -To $@

maketime:
        MakeTime

version:
        Version

# Dynamic dependencies:
rm.Hourgls200: s.Module
rm.Hourgls200: Hdr:ListOpts
rm.Hourgls200: Hdr:Macros
rm.Hourgls200: Hdr:System
rm.Hourgls200: Hdr:SWIs
rm.Hourgls200: Hdr:CPU.Generic26
rm.Hourgls200: Hdr:IO.GenericIO
rm.Hourgls200: Hdr:RISCOS
rm.Hourgls200: Hdr:ModHand
rm.Hourgls200: Hdr:Services
rm.Hourgls200: Hdr:Hourglass
rm.Hourgls200: Hdr:MsgTrans
rm.Hourgls200: Hdr:FSNumbers
rm.Hourgls200: Hdr:NewErrors
rm.Hourgls200: Hdr:Proc
rm.Hourgls200: Time+Date
rm.Hourgls200: Hdr:NdrDebug
rm.Hourglass: s.Module
rm.Hourglass: Hdr:ListOpts
rm.Hourglass: Hdr:Macros
rm.Hourglass: Hdr:System
rm.Hourglass: Hdr:SWIs
rm.Hourglass: Hdr:CPU.Generic26
rm.Hourglass: Hdr:IO.GenericIO
rm.Hourglass: Hdr:RISCOS
rm.Hourglass: Hdr:ModHand
rm.Hourglass: Hdr:Services
rm.Hourglass: Hdr:Hourglass
rm.Hourglass: Hdr:MsgTrans
rm.Hourglass: Hdr:FSNumbers
rm.Hourglass: Hdr:NewErrors
rm.Hourglass: Hdr:Proc
rm.Hourglass: Time+Date
rm.Hourglass: Hdr:NdrDebug
