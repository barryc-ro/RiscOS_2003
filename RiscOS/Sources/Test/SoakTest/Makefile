# Makefile for SoakTest
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 10-Mar-95  TMD          Created.
# 28-Aug-96  BAR          Updated to build differetn vsn.s for different
#                         machine types
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
WIPE    = wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
WPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = SoakTest
SOURCE    = Hdr.GetAll
MPEGS	  = MPEG.Soak MPEG.Test MPEG.AudioLo MPEG.AudioHi
NONMPEGS  = Sprites.Soak.4bpp Sprites.Test.4bpp
TARGET    = rm.${MACHINE}.SoakTest

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
        @echo ${COMPONENT}: disc module installed

clean:
        -${WIPE} rm.* ${WPFLAGS}
        ${RM} Sprites.Soak.Frame0
        ${RM} Sprites.Test.Frame0
        ${RM} Sprites.Soak.32bpp
        ${RM} Sprites.Test.32bpp
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE} ${NONMPEGS}
        cdir rm.${MACHINE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

MPEG.Soak: Sprites.Soak.32bpp Tools.mpeg2encod par.Soak
	/Tools.mpeg2encod par.Soak MPEG.Soak

MPEG.Test: Sprites.Test.32bpp Tools.mpeg2encod par.Test
	/Tools.mpeg2encod par.Test MPEG.Test

Sprites.Soak.32bpp: Sprites.Soak.4bpp Tools.ChangeFSI
	/Tools.ChangeFSI Sprites.Soak.4bpp Sprites.Soak.32bpp S32,90,90 16:1 -nomode

Sprites.Test.32bpp: Sprites.Test.4bpp Tools.ChangeFSI
	/Tools.ChangeFSI Sprites.Test.4bpp Sprites.Test.32bpp S32,90,90 16:1 -nomode
	


# Dynamic dependencies:
rm.SoakTest: Hdr:ListOpts
rm.SoakTest: Hdr:Macros
rm.SoakTest: Hdr:System
rm.SoakTest: Hdr:SWIs
rm.SoakTest: Hdr:CPU.Generic26
rm.SoakTest: Hdr:IO.GenericIO
rm.SoakTest: Hdr:RISCOS
rm.SoakTest: Hdr:Machine.<Machine>
rm.SoakTest: Hdr:IO.IOMDL
rm.SoakTest: Hdr:CMOS
rm.SoakTest: Hdr:ModHand
rm.SoakTest: Hdr:Services
rm.SoakTest: Hdr:Proc
rm.SoakTest: Hdr:FSNumbers
rm.SoakTest: Hdr:NewErrors
rm.SoakTest: Hdr:MsgTrans
rm.SoakTest: Hdr:Debug
rm.SoakTest: Hdr:Podule
rm.SoakTest: Hdr:PoduleReg
rm.SoakTest: Hdr:HighFSI
rm.SoakTest: Hdr:Sprite
rm.SoakTest: Hdr:MPEGCtrl
rm.SoakTest: Hdr:MPEGVideo
rm.SoakTest: Hdr:MPEGAudio
rm.SoakTest: Hdr:MPEGCard
rm.SoakTest: s.Module
rm.Falcon.SoakTest: Hdr.<MACHINE>.GetAll_MS
rm.Falcon.SoakTest: Hdr:ListOpts
rm.Falcon.SoakTest: Hdr:Macros
rm.Falcon.SoakTest: Hdr:System
rm.Falcon.SoakTest: Hdr:SWIs
rm.Falcon.SoakTest: Hdr:CPU.Generic26
rm.Falcon.SoakTest: Hdr:IO.GenericIO
rm.Falcon.SoakTest: Hdr:RISCOS
rm.Falcon.SoakTest: Hdr:Machine.<Machine>
rm.Falcon.SoakTest: Hdr:IO.IOMDL
rm.Falcon.SoakTest: Hdr:CMOS
rm.Falcon.SoakTest: Hdr:ModHand
rm.Falcon.SoakTest: Hdr:Services
rm.Falcon.SoakTest: Hdr:Proc
rm.Falcon.SoakTest: Hdr:FSNumbers
rm.Falcon.SoakTest: Hdr:NewErrors
rm.Falcon.SoakTest: Hdr:MsgTrans
rm.Falcon.SoakTest: Hdr:Debug
rm.Falcon.SoakTest: Hdr:Podule
rm.Falcon.SoakTest: Hdr:PoduleReg
rm.Falcon.SoakTest: Hdr:HighFSI
rm.Falcon.SoakTest: Hdr:Sprite
rm.Falcon.SoakTest: Hdr:MPEGCtrl
rm.Falcon.SoakTest: Hdr:MPEGVideo
rm.Falcon.SoakTest: Hdr:MPEGAudio
rm.Falcon.SoakTest: Hdr:MPEGCard
rm.Falcon.SoakTest: Hdr:STBState
rm.Falcon.SoakTest: s.Module
rm.Omega.SoakTest: Hdr.<MACHINE>.GetAll_MS
rm.Omega.SoakTest: Hdr:ListOpts
rm.Omega.SoakTest: Hdr:Macros
rm.Omega.SoakTest: Hdr:System
rm.Omega.SoakTest: Hdr:SWIs
rm.Omega.SoakTest: Hdr:CPU.Generic26
rm.Omega.SoakTest: Hdr:IO.GenericIO
rm.Omega.SoakTest: Hdr:RISCOS
rm.Omega.SoakTest: Hdr:Machine.<Machine>
rm.Omega.SoakTest: Hdr:IO.IOMDL
rm.Omega.SoakTest: Hdr:CMOS
rm.Omega.SoakTest: Hdr:ModHand
rm.Omega.SoakTest: Hdr:Services
rm.Omega.SoakTest: Hdr:Proc
rm.Omega.SoakTest: Hdr:FSNumbers
rm.Omega.SoakTest: Hdr:NewErrors
rm.Omega.SoakTest: Hdr:MsgTrans
rm.Omega.SoakTest: Hdr:Debug
rm.Omega.SoakTest: Hdr:Podule
rm.Omega.SoakTest: Hdr:PoduleReg
rm.Omega.SoakTest: Hdr:HighFSI
rm.Omega.SoakTest: Hdr:Sprite
rm.Omega.SoakTest: s.Module
rm.Omega.SoakTest: <Src$Dir>.Kernel.TestSrc.ToggleLED
rm.RCA.SoakTest: Hdr.<MACHINE>.GetAll_MS
rm.RCA.SoakTest: Hdr:ListOpts
rm.RCA.SoakTest: Hdr:Macros
rm.RCA.SoakTest: Hdr:System
rm.RCA.SoakTest: Hdr:SWIs
rm.RCA.SoakTest: Hdr:CPU.Generic26
rm.RCA.SoakTest: Hdr:IO.GenericIO
rm.RCA.SoakTest: Hdr:RISCOS
rm.RCA.SoakTest: Hdr:Machine.<Machine>
rm.RCA.SoakTest: Hdr:IO.IOMDL
rm.RCA.SoakTest: Hdr:CMOS
rm.RCA.SoakTest: Hdr:ModHand
rm.RCA.SoakTest: Hdr:Services
rm.RCA.SoakTest: Hdr:Proc
rm.RCA.SoakTest: Hdr:FSNumbers
rm.RCA.SoakTest: Hdr:NewErrors
rm.RCA.SoakTest: Hdr:MsgTrans
rm.RCA.SoakTest: Hdr:Debug
rm.RCA.SoakTest: Hdr:Podule
rm.RCA.SoakTest: Hdr:PoduleReg
rm.RCA.SoakTest: Hdr:HighFSI
rm.RCA.SoakTest: Hdr:Sprite
rm.RCA.SoakTest: s.Module
rm.RCA.SoakTest: <Src$Dir>.Kernel.TestSrc.ToggleLED
rm.Boca.SoakTest: Hdr.<MACHINE>.GetAll_MS
rm.Boca.SoakTest: Hdr:ListOpts
rm.Boca.SoakTest: Hdr:Macros
rm.Boca.SoakTest: Hdr:System
rm.Boca.SoakTest: Hdr:SWIs
rm.Boca.SoakTest: Hdr:CPU.Generic26
rm.Boca.SoakTest: Hdr:IO.GenericIO
rm.Boca.SoakTest: Hdr:RISCOS
rm.Boca.SoakTest: Hdr:Machine.<Machine>
rm.Boca.SoakTest: Hdr:Machine.Machine
rm.Boca.SoakTest: Hdr:IO.IOMDL
rm.Boca.SoakTest: Hdr:CMOS
rm.Boca.SoakTest: Hdr:ModHand
rm.Boca.SoakTest: Hdr:Services
rm.Boca.SoakTest: Hdr:Proc
rm.Boca.SoakTest: Hdr:FSNumbers
rm.Boca.SoakTest: Hdr:NewErrors
rm.Boca.SoakTest: Hdr:MsgTrans
rm.Boca.SoakTest: Hdr:Debug
rm.Boca.SoakTest: Hdr:Podule
rm.Boca.SoakTest: Hdr:PoduleReg
rm.Boca.SoakTest: Hdr:HighFSI
rm.Boca.SoakTest: Hdr:Sprite
rm.Boca.SoakTest: s.Module
rm.Boca.SoakTest: <Src$Dir>.Kernel.TestSrc.ToggleLED
