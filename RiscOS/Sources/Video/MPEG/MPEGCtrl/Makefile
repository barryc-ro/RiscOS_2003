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
# 10-Mar-97  TMD	  Add option to make system-only version
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
COMPONENT = MPEGCtrl
SOURCE    = Hdr.GetAll
TARGET    = rm.MPEGCtrl

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

sysonly: rm.SysOnly.MPEGCtrl
	@echo System-stream only version of MPEGCtrl built

rm.SysOnly.MPEGCtrl: ${SOURCE} Hdr.SysOnly
	${AS} ${ASFLAGS} -From Hdr.SysOnly -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.SysOnly.MPEGCtrl: Hdr.GetAll
rm.SysOnly.MPEGCtrl: Hdr:ListOpts
rm.SysOnly.MPEGCtrl: Hdr:Macros
rm.SysOnly.MPEGCtrl: Hdr:System
rm.SysOnly.MPEGCtrl: Hdr:SWIs
rm.SysOnly.MPEGCtrl: Hdr:CPU.Generic26
rm.SysOnly.MPEGCtrl: Hdr:IO.GenericIO
rm.SysOnly.MPEGCtrl: Hdr:RISCOS
rm.SysOnly.MPEGCtrl: Hdr:CMOS
rm.SysOnly.MPEGCtrl: Hdr:ModHand
rm.SysOnly.MPEGCtrl: Hdr:Services
rm.SysOnly.MPEGCtrl: Hdr:Proc
rm.SysOnly.MPEGCtrl: Hdr:FSNumbers
rm.SysOnly.MPEGCtrl: Hdr:NewErrors
rm.SysOnly.MPEGCtrl: Hdr:MsgTrans
rm.SysOnly.MPEGCtrl: Hdr:Debug
rm.SysOnly.MPEGCtrl: Hdr:VduExt
rm.SysOnly.MPEGCtrl: Hdr:Podule
rm.SysOnly.MPEGCtrl: Hdr:DevNos
rm.SysOnly.MPEGCtrl: Hdr:MPEGCtrl
rm.SysOnly.MPEGCtrl: Hdr:MPEGVideo
rm.SysOnly.MPEGCtrl: Hdr:MPEGAudio
rm.SysOnly.MPEGCtrl: s.Module
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
