# Makefile for MPEGAudio
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 19-Sep-94  JRH          Created.
# 09-Feb-95  TMD	  Updated to include 'M' variant.
# 13-Feb-95  TMD	  Added install option for making !M-Code21.
# 29-Mar-95  TMD	  Added 'P' variant.
# 17-Oct-95  TMD	  Removed obsolete 'I' variant from disc build.
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
COMPONENT = MPEGAudio
SOURCE    = Hdr.Audio
SOURCEI   = Hdr.AudioI
SOURCEM	  = Hdr.AudioM
SOURCEP	  = Hdr.AudioP
TARGET    = rm.MPEGAudio
TARGETI   = rm.MPEGAudioI
TARGETM   = rm.MPEGAudioM
TARGETP	  = rm.MPEGAudioP

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

all: ${TARGET} ${TARGETI} ${TARGETM} ${TARGETP}
	@echo ${COMPONENT}: disc modules built

install: ${TARGET} ${TARGETM}
        ${CP} ${TARGET} ${INSTDIR}.MPEGAudio ${CPFLAGS}
        ${CP} ${TARGETM} ${INSTDIR}.MPEGAudioM ${CPFLAGS}
	@echo ${COMPONENT}: disc modules built

clean:
        ${RM} ${TARGET}
        ${RM} ${TARGETM}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${TARGETI}: ${SOURCEI}
        ${AS} ${ASFLAGS} -From ${SOURCEI} -To $@

${TARGETM}: ${SOURCEM}
        ${AS} ${ASFLAGS} -From ${SOURCEM} -To $@

${TARGETP}: ${SOURCEP}
        ${AS} ${ASFLAGS} -From ${SOURCEP} -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.MPEGAudioI: Hdr.GetAll
rm.MPEGAudioI: Hdr:ListOpts
rm.MPEGAudioI: Hdr:Macros
rm.MPEGAudioI: Hdr:System
rm.MPEGAudioI: Hdr:SWIs
rm.MPEGAudioI: Hdr:CPU.Generic26
rm.MPEGAudioI: Hdr:IO.GenericIO
rm.MPEGAudioI: Hdr:RISCOS
rm.MPEGAudioI: Hdr:Machine.<Machine>
rm.MPEGAudioI: Hdr:CMOS
rm.MPEGAudioI: Hdr:ModHand
rm.MPEGAudioI: Hdr:Services
rm.MPEGAudioI: Hdr:Proc
rm.MPEGAudioI: Hdr:FSNumbers
rm.MPEGAudioI: Hdr:NewErrors
rm.MPEGAudioI: Hdr:MsgTrans
rm.MPEGAudioI: Hdr:Debug
rm.MPEGAudioI: Hdr:VduExt
rm.MPEGAudioI: Hdr:Podule
rm.MPEGAudioI: Hdr:PoduleReg
rm.MPEGAudioI: Hdr:DevNos
rm.MPEGAudioI: Hdr:MPEGCtrl
rm.MPEGAudioI: Hdr:MPEGVideo
rm.MPEGAudioI: Hdr:MPEGAudio
rm.MPEGAudioI: Hdr:MPEGCard
rm.MPEGAudioI: Hdr:AV110
rm.MPEGAudioI: s.Module
rm.MPEGAudioP: Hdr.GetAll
rm.MPEGAudioP: Hdr:ListOpts
rm.MPEGAudioP: Hdr:Macros
rm.MPEGAudioP: Hdr:System
rm.MPEGAudioP: Hdr:SWIs
rm.MPEGAudioP: Hdr:CPU.Generic26
rm.MPEGAudioP: Hdr:IO.GenericIO
rm.MPEGAudioP: Hdr:RISCOS
rm.MPEGAudioP: Hdr:Machine.<Machine>
rm.MPEGAudioP: Hdr:CMOS
rm.MPEGAudioP: Hdr:ModHand
rm.MPEGAudioP: Hdr:Services
rm.MPEGAudioP: Hdr:Proc
rm.MPEGAudioP: Hdr:FSNumbers
rm.MPEGAudioP: Hdr:NewErrors
rm.MPEGAudioP: Hdr:MsgTrans
rm.MPEGAudioP: Hdr:Debug
rm.MPEGAudioP: Hdr:VduExt
rm.MPEGAudioP: Hdr:Podule
rm.MPEGAudioP: Hdr:PoduleReg
rm.MPEGAudioP: Hdr:DevNos
rm.MPEGAudioP: Hdr:MPEGCtrl
rm.MPEGAudioP: Hdr:MPEGVideo
rm.MPEGAudioP: Hdr:MPEGAudio
rm.MPEGAudioP: Hdr:MPEGCard
rm.MPEGAudioP: Hdr:AV110
rm.MPEGAudioP: s.Module
rm.MPEGAudio: Hdr.GetAll
rm.MPEGAudio: Hdr:ListOpts
rm.MPEGAudio: Hdr:Macros
rm.MPEGAudio: Hdr:System
rm.MPEGAudio: Hdr:SWIs
rm.MPEGAudio: Hdr:CPU.Generic26
rm.MPEGAudio: Hdr:IO.GenericIO
rm.MPEGAudio: Hdr:RISCOS
rm.MPEGAudio: Hdr:Machine.<Machine>
rm.MPEGAudio: Hdr:CMOS
rm.MPEGAudio: Hdr:ModHand
rm.MPEGAudio: Hdr:Services
rm.MPEGAudio: Hdr:Proc
rm.MPEGAudio: Hdr:FSNumbers
rm.MPEGAudio: Hdr:NewErrors
rm.MPEGAudio: Hdr:MsgTrans
rm.MPEGAudio: Hdr:Debug
rm.MPEGAudio: Hdr:VduExt
rm.MPEGAudio: Hdr:Podule
rm.MPEGAudio: Hdr:PoduleReg
rm.MPEGAudio: Hdr:DevNos
rm.MPEGAudio: Hdr:MPEGCtrl
rm.MPEGAudio: Hdr:MPEGVideo
rm.MPEGAudio: Hdr:MPEGAudio
rm.MPEGAudio: Hdr:MPEGCard
rm.MPEGAudio: Hdr:AV110
rm.MPEGAudio: s.Module
rm.MPEGAudioM: Hdr.GetAll
rm.MPEGAudioM: Hdr:ListOpts
rm.MPEGAudioM: Hdr:Macros
rm.MPEGAudioM: Hdr:System
rm.MPEGAudioM: Hdr:SWIs
rm.MPEGAudioM: Hdr:CPU.Generic26
rm.MPEGAudioM: Hdr:IO.GenericIO
rm.MPEGAudioM: Hdr:RISCOS
rm.MPEGAudioM: Hdr:Machine.<Machine>
rm.MPEGAudioM: Hdr:CMOS
rm.MPEGAudioM: Hdr:ModHand
rm.MPEGAudioM: Hdr:Services
rm.MPEGAudioM: Hdr:Proc
rm.MPEGAudioM: Hdr:FSNumbers
rm.MPEGAudioM: Hdr:NewErrors
rm.MPEGAudioM: Hdr:MsgTrans
rm.MPEGAudioM: Hdr:Debug
rm.MPEGAudioM: Hdr:VduExt
rm.MPEGAudioM: Hdr:Podule
rm.MPEGAudioM: Hdr:PoduleReg
rm.MPEGAudioM: Hdr:DevNos
rm.MPEGAudioM: Hdr:MPEGCtrl
rm.MPEGAudioM: Hdr:MPEGVideo
rm.MPEGAudioM: Hdr:MPEGAudio
rm.MPEGAudioM: Hdr:MPEGCard
rm.MPEGAudioM: Hdr:AV110
rm.MPEGAudioM: s.Module
