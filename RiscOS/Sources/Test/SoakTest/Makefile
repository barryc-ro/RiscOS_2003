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
# 29 Jul 1997 RWB	  Made image publicly readable.
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
        Access rm.${MACHINE}.${COMPONENT} RW/R


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
