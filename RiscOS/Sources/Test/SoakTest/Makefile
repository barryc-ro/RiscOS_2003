# Makefile for SoakTest
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 10-Mar-95  TMD          Created.
# 28-Aug-96  BAR          Updated to build differetn vsn.s for different
#                         machine types.
# 29 Jul 97  RWB          Made image publicly readable.
# 04 Feb 00  SBF          Added StripDepnd support.
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
STRIP   = stripdepnd
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
WPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = SoakTest
SOURCE    = Hdr.GetAll
MPEGS     = MPEG.Soak MPEG.Test MPEG.AudioLo MPEG.AudioHi
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
        ${STRIP}
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
