# Makefile for MPEG2Video
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-94  JRH          Created.
# 13-Feb-95  TMD	  Updated with VideoM.
# 13-Feb-95  TMD	  Added install option for making !M-Code21.
# 28-Apr-95  TMD	  Adapted from MPEGVideo version.
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
COMPONENT = MPEG2Video
SOURCE    = Hdr.MPEG2Video
TARGET    = rm.MPEG2Video
SETUP	  = s.Setup.352x288
SETUPC	  = s.Setup.352x288C

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
	@echo ${COMPONENT}: disc modules built

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: disc modules installed

clean:
        ${RM} ${TARGET}
        ${RM} s.Data.BinSetup
        ${RM} s.Data.BinSetupC
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE} s.Data.BinSetupC
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

s.Data.BinSetup: ${SETUP} Tools.ConvSetup
	/Tools.ConvSetup ${SETUP} s.Data.BinSetup

s.Data.BinSetupC: ${SETUPC} Tools.ConvSetup
	/Tools.ConvSetup ${SETUPC} s.Data.BinSetupC

# Dynamic dependencies:
rm.MPEG2Video: Hdr.GetAll
rm.MPEG2Video: Hdr:ListOpts
rm.MPEG2Video: Hdr:Macros
rm.MPEG2Video: Hdr:System
rm.MPEG2Video: Hdr:SWIs
rm.MPEG2Video: Hdr:CPU.Generic26
rm.MPEG2Video: Hdr:IO.GenericIO
rm.MPEG2Video: Hdr:RISCOS
rm.MPEG2Video: Hdr:Machine.<Machine>
rm.MPEG2Video: Hdr:CMOS
rm.MPEG2Video: Hdr:ModHand
rm.MPEG2Video: Hdr:Services
rm.MPEG2Video: Hdr:Proc
rm.MPEG2Video: Hdr:FSNumbers
rm.MPEG2Video: Hdr:NewErrors
rm.MPEG2Video: Hdr:MsgTrans
rm.MPEG2Video: Hdr:Debug
rm.MPEG2Video: Hdr:VduExt
rm.MPEG2Video: Hdr:Podule
rm.MPEG2Video: Hdr:PoduleReg
rm.MPEG2Video: Hdr:DevNos
rm.MPEG2Video: Hdr:IIC
rm.MPEG2Video: Hdr:MPEGCtrl
rm.MPEG2Video: Hdr:MPEGVideo
rm.MPEG2Video: Hdr:MPEGAudio
rm.MPEG2Video: Hdr:MPEG2Card
rm.MPEG2Video: Hdr:L64002
rm.MPEG2Video: s.Module
rm.MPEG2Video: s.Errors
