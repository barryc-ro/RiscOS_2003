# Makefile for NHTwitter
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 31-Oct-95  TMD	  Created.
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
COMPONENT = NHTwitter
SOURCE    = Hdr.GetAll
TARGET    = rm.NHTwitter

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

install:
	@echo ${COMPONENT}: disc modules installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.NHTwitter: Hdr:ListOpts
rm.NHTwitter: Hdr:Macros
rm.NHTwitter: Hdr:System
rm.NHTwitter: Hdr:SWIs
rm.NHTwitter: Hdr:CPU.Generic26
rm.NHTwitter: Hdr:IO.GenericIO
rm.NHTwitter: Hdr:RISCOS
rm.NHTwitter: Hdr:CMOS
rm.NHTwitter: Hdr:ModHand
rm.NHTwitter: Hdr:Services
rm.NHTwitter: Hdr:Proc
rm.NHTwitter: Hdr:FSNumbers
rm.NHTwitter: Hdr:NewErrors
rm.NHTwitter: Hdr:MsgTrans
rm.NHTwitter: Hdr:Debug
rm.NHTwitter: Hdr:VduExt
rm.NHTwitter: Hdr:Podule
rm.NHTwitter: Hdr:PoduleReg
rm.NHTwitter: Hdr:DevNos
rm.NHTwitter: s.Module
