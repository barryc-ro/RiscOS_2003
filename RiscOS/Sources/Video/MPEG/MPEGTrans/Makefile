# Makefile for MPEGTrans
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 15-Mar-95  TMD	  Created
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
COMPONENT = MPEGTrans
SOURCE    = Hdr.GetAll
TARGET    = rm.MPEGTrans

#
# Generic rules:
#

all: ${TARGET}
        @echo ${COMPONENT}: disc module built

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: disc module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

# Dynamic dependencies:
rm.MPEGTrans: Hdr:ListOpts
rm.MPEGTrans: Hdr:Macros
rm.MPEGTrans: Hdr:System
rm.MPEGTrans: Hdr:SWIs
rm.MPEGTrans: Hdr:CPU.Generic26
rm.MPEGTrans: Hdr:IO.GenericIO
rm.MPEGTrans: Hdr:RISCOS
rm.MPEGTrans: Hdr:Machine.<Machine>
rm.MPEGTrans: Hdr:CMOS
rm.MPEGTrans: Hdr:ModHand
rm.MPEGTrans: Hdr:Services
rm.MPEGTrans: Hdr:Proc
rm.MPEGTrans: Hdr:FSNumbers
rm.MPEGTrans: Hdr:NewErrors
rm.MPEGTrans: Hdr:MsgTrans
rm.MPEGTrans: Hdr:Debug
rm.MPEGTrans: Hdr:VduExt
rm.MPEGTrans: Hdr:Podule
rm.MPEGTrans: Hdr:DevNos
rm.MPEGTrans: Hdr:MPEGCtrl
rm.MPEGTrans: Hdr:MPEGVideo
rm.MPEGTrans: Hdr:MPEGAudio
rm.MPEGTrans: Hdr:DVBControl
rm.MPEGTrans: s.Module
