# Makefile for MPEGVideo
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-94  JRH          Created.
# 13-Feb-95  TMD	  Updated with VideoM.
# 13-Feb-95  TMD	  Added install option for making !M-Code21.
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
COMPONENT = MPEGVideo
SOURCE    = Hdr.Video
SOURCEI   = Hdr.VideoI
SOURCEM	  = Hdr.VideoM
TARGET    = rm.MPEGVideo
TARGETI   = rm.MPEGVideoI
TARGETM   = rm.MPEGVideoM

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

all: ${TARGET} ${TARGETM}
	@echo ${COMPONENT}: disc modules built

install: ${TARGET} ${TARGETM}
        ${CP} ${TARGET} ${INSTDIR}.MPEGVideo ${CPFLAGS}
        ${CP} ${TARGETM} ${INSTDIR}.MPEGVideoM ${CPFLAGS}
	@echo ${COMPONENT}: disc modules installed

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

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.MPEGVideoI: Hdr.GetAll
rm.MPEGVideoI: Hdr:ListOpts
rm.MPEGVideoI: Hdr:Macros
rm.MPEGVideoI: Hdr:System
rm.MPEGVideoI: Hdr:SWIs
rm.MPEGVideoI: Hdr:CPU.Generic26
rm.MPEGVideoI: Hdr:IO.GenericIO
rm.MPEGVideoI: Hdr:RISCOS
rm.MPEGVideoI: Hdr:Machine.<Machine>
rm.MPEGVideoI: Hdr:CMOS
rm.MPEGVideoI: Hdr:ModHand
rm.MPEGVideoI: Hdr:Services
rm.MPEGVideoI: Hdr:Proc
rm.MPEGVideoI: Hdr:FSNumbers
rm.MPEGVideoI: Hdr:NewErrors
rm.MPEGVideoI: Hdr:MsgTrans
rm.MPEGVideoI: Hdr:Debug
rm.MPEGVideoI: Hdr:VduExt
rm.MPEGVideoI: Hdr:Podule
rm.MPEGVideoI: Hdr:PoduleReg
rm.MPEGVideoI: Hdr:DevNos
rm.MPEGVideoI: Hdr:MPEGCtrl
rm.MPEGVideoI: Hdr:MPEGVideo
rm.MPEGVideoI: Hdr:MPEGCard
rm.MPEGVideoI: Hdr:AV110
rm.MPEGVideoI: Hdr:CL450
rm.MPEGVideoI: s.Module
rm.MPEGVideoI: s.Errors
rm.MPEGVideoM: Hdr.GetAll
rm.MPEGVideoM: Hdr:ListOpts
rm.MPEGVideoM: Hdr:Macros
rm.MPEGVideoM: Hdr:System
rm.MPEGVideoM: Hdr:SWIs
rm.MPEGVideoM: Hdr:CPU.Generic26
rm.MPEGVideoM: Hdr:IO.GenericIO
rm.MPEGVideoM: Hdr:RISCOS
rm.MPEGVideoM: Hdr:Machine.<Machine>
rm.MPEGVideoM: Hdr:CMOS
rm.MPEGVideoM: Hdr:ModHand
rm.MPEGVideoM: Hdr:Services
rm.MPEGVideoM: Hdr:Proc
rm.MPEGVideoM: Hdr:FSNumbers
rm.MPEGVideoM: Hdr:NewErrors
rm.MPEGVideoM: Hdr:MsgTrans
rm.MPEGVideoM: Hdr:Debug
rm.MPEGVideoM: Hdr:VduExt
rm.MPEGVideoM: Hdr:Podule
rm.MPEGVideoM: Hdr:PoduleReg
rm.MPEGVideoM: Hdr:DevNos
rm.MPEGVideoM: Hdr:MPEGCtrl
rm.MPEGVideoM: Hdr:MPEGVideo
rm.MPEGVideoM: Hdr:MPEGCard
rm.MPEGVideoM: Hdr:AV110
rm.MPEGVideoM: Hdr:CL450
rm.MPEGVideoM: s.Module
rm.MPEGVideoM: s.Errors
rm.MPEGVideo: Hdr.GetAll
rm.MPEGVideo: Hdr:ListOpts
rm.MPEGVideo: Hdr:Macros
rm.MPEGVideo: Hdr:System
rm.MPEGVideo: Hdr:SWIs
rm.MPEGVideo: Hdr:CPU.Generic26
rm.MPEGVideo: Hdr:IO.GenericIO
rm.MPEGVideo: Hdr:RISCOS
rm.MPEGVideo: Hdr:Machine.<Machine>
rm.MPEGVideo: Hdr:CMOS
rm.MPEGVideo: Hdr:ModHand
rm.MPEGVideo: Hdr:Services
rm.MPEGVideo: Hdr:Proc
rm.MPEGVideo: Hdr:FSNumbers
rm.MPEGVideo: Hdr:NewErrors
rm.MPEGVideo: Hdr:MsgTrans
rm.MPEGVideo: Hdr:Debug
rm.MPEGVideo: Hdr:VduExt
rm.MPEGVideo: Hdr:Podule
rm.MPEGVideo: Hdr:PoduleReg
rm.MPEGVideo: Hdr:DevNos
rm.MPEGVideo: Hdr:MPEGCtrl
rm.MPEGVideo: Hdr:MPEGVideo
rm.MPEGVideo: Hdr:MPEGCard
rm.MPEGVideo: Hdr:AV110
rm.MPEGVideo: Hdr:CL450
rm.MPEGVideo: s.Module
rm.MPEGVideo: s.Errors
