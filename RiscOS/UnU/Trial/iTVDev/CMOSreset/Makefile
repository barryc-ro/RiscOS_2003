# Makefile for CMOSreset
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 10-Mar-95  JRH          Created.
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
COMPONENT = CMOSreset
SOURCE    = s.Module
TARGET    = rm.CMOSreset

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

# { > Log }

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

#
# Extra Development rules:
#
install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ${TARGET} installed

maketime:
        MakeTime

version:
        Version

# Dynamic dependencies:
rm.CMOSreset: Hdr:ListOpts
rm.CMOSreset: Hdr:Macros
rm.CMOSreset: Hdr:System
rm.CMOSreset: Hdr:SWIs
rm.CMOSreset: Hdr:CPU.Generic26
rm.CMOSreset: Hdr:IO.GenericIO
rm.CMOSreset: Hdr:RISCOS
rm.CMOSreset: Hdr:Debug
rm.CMOSreset: Hdr:Proc
rm.CMOSreset: Hdr:CMOS
rm.CMOSreset: Hdr:ModHand
rm.CMOSreset: Hdr:Services
rm.CMOSreset: Hdr:MsgTrans
rm.CMOSreset: Hdr:FSNumbers
rm.CMOSreset: Hdr:NewErrors
rm.CMOSreset: Hdr:Podule
rm.CMOSreset: Hdr:PoduleReg
rm.CMOSreset: Hdr:MPEGCard
rm.CMOSreset: Time+Date
rm.CMOSreset: s.Settings
