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
TARGET    = rm.${MACHINE}.CMOSreset

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
        cdir rm.${MACHINE}
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
rm.Omega.CMOSreset: Hdr:ListOpts
rm.Omega.CMOSreset: Hdr:Macros
rm.Omega.CMOSreset: Hdr:System
rm.Omega.CMOSreset: Hdr:SWIs
rm.Omega.CMOSreset: Hdr:CPU.Generic26
rm.Omega.CMOSreset: Hdr:IO.GenericIO
rm.Omega.CMOSreset: Hdr:RISCOS
rm.Omega.CMOSreset: Hdr:Machine.<Machine>
rm.Omega.CMOSreset: Hdr:Debug
rm.Omega.CMOSreset: Hdr:Proc
rm.Omega.CMOSreset: Hdr:CMOS
rm.Omega.CMOSreset: Hdr:ModHand
rm.Omega.CMOSreset: Hdr:Services
rm.Omega.CMOSreset: Hdr:MsgTrans
rm.Omega.CMOSreset: Hdr:FSNumbers
rm.Omega.CMOSreset: Hdr:NewErrors
rm.Omega.CMOSreset: Time+Date
rm.Omega.CMOSreset: s.<Machine>.Settings
rm.Peregrine.CMOSreset: Hdr:ListOpts
rm.Peregrine.CMOSreset: Hdr:Macros
rm.Peregrine.CMOSreset: Hdr:System
rm.Peregrine.CMOSreset: Hdr:SWIs
rm.Peregrine.CMOSreset: Hdr:CPU.Generic26
rm.Peregrine.CMOSreset: Hdr:IO.GenericIO
rm.Peregrine.CMOSreset: Hdr:RISCOS
rm.Peregrine.CMOSreset: Hdr:Machine.<Machine>
rm.Peregrine.CMOSreset: Hdr:Debug
rm.Peregrine.CMOSreset: Hdr:Proc
rm.Peregrine.CMOSreset: Hdr:CMOS
rm.Peregrine.CMOSreset: Hdr:ModHand
rm.Peregrine.CMOSreset: Hdr:Services
rm.Peregrine.CMOSreset: Hdr:MsgTrans
rm.Peregrine.CMOSreset: Hdr:FSNumbers
rm.Peregrine.CMOSreset: Hdr:NewErrors
rm.Peregrine.CMOSreset: Hdr:Podule
rm.Peregrine.CMOSreset: Hdr:PoduleReg
rm.Peregrine.CMOSreset: Time+Date
rm.Peregrine.CMOSreset: s.<Machine>.Settings
