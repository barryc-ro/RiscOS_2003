# Makefile for MPEGCtrl
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-94  JRH          Created.
# 13-Feb-95  TMD	  Added install option for making !M-Code21.
# 17-Mar-95  TMD	  Delete CP from all target.
# 05-May-95  TMD	  Add Doc.MPEGPlayer to installed list
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
COMPONENT = MPEGCtFile
SOURCE    = Hdr.GetAll
TARGET    = rm.MPEGCtFile

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

all: ${TARGET}
        @echo ${COMPONENT}: disc module built

install: ${TARGET} Doc.MPEGPlayer
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        ${CP} Doc.MPEGPlayer ${INSTDIR}.^.MPEGPlayer ${CPFLAGS}
        @echo ${COMPONENT}: disc module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.MPEGCtrl: Hdr:ListOpts
rm.MPEGCtrl: Hdr:Macros
rm.MPEGCtrl: Hdr:System
rm.MPEGCtrl: Hdr:SWIs
rm.MPEGCtrl: Hdr:CPU.Generic26
rm.MPEGCtrl: Hdr:IO.GenericIO
rm.MPEGCtrl: Hdr:RISCOS
rm.MPEGCtrl: Hdr:Machine.<Machine>
rm.MPEGCtrl: Hdr:CMOS
rm.MPEGCtrl: Hdr:ModHand
rm.MPEGCtrl: Hdr:Services
rm.MPEGCtrl: Hdr:Proc
rm.MPEGCtrl: Hdr:FSNumbers
rm.MPEGCtrl: Hdr:NewErrors
rm.MPEGCtrl: Hdr:MsgTrans
rm.MPEGCtrl: Hdr:Debug
rm.MPEGCtrl: Hdr:VduExt
rm.MPEGCtrl: Hdr:Podule
rm.MPEGCtrl: Hdr:DevNos
rm.MPEGCtrl: Hdr:MPEGCtrl
rm.MPEGCtrl: Hdr:MPEGVideo
rm.MPEGCtrl: Hdr:MPEGAudio
rm.MPEGCtrl: s.Module
rm.MPEGCtFile: Hdr:ListOpts
rm.MPEGCtFile: Hdr:Macros
rm.MPEGCtFile: Hdr:System
rm.MPEGCtFile: Hdr:SWIs
rm.MPEGCtFile: Hdr:CPU.Generic26
rm.MPEGCtFile: Hdr:IO.GenericIO
rm.MPEGCtFile: Hdr:RISCOS
rm.MPEGCtFile: Hdr:Machine.<Machine>
rm.MPEGCtFile: Hdr:CMOS
rm.MPEGCtFile: Hdr:ModHand
rm.MPEGCtFile: Hdr:Services
rm.MPEGCtFile: Hdr:Proc
rm.MPEGCtFile: Hdr:FSNumbers
rm.MPEGCtFile: Hdr:NewErrors
rm.MPEGCtFile: Hdr:MsgTrans
rm.MPEGCtFile: Hdr:Debug
rm.MPEGCtFile: Hdr:VduExt
rm.MPEGCtFile: Hdr:Podule
rm.MPEGCtFile: Hdr:DevNos
rm.MPEGCtFile: Hdr:HighFSI
rm.MPEGCtFile: Hdr:MPEGCtrl
rm.MPEGCtFile: Hdr:MPEGVideo
rm.MPEGCtFile: Hdr:MPEGAudio
rm.MPEGCtFile: s.Module
