# Makefile for SoakTest2
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 10-Mar-95  TMD          Created.
# 03-Oct-96  TMD	  Modified MPEG2 version to support both stand-alone
#			  and ROM versions.
# 05-Nov-96  TMD	  'Setup' split into 'SetupPAL' and 'SetupNTSC'
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
COMPONENT = SoakTest2
SOURCE    = Hdr.SoakTest2
SOURCESA  = Hdr.SoakTest2S
MPEGS	  = MPEG.Soak MPEG.Test MPEG.AudioLo MPEG.AudioHi
NONMPEGS  = Sprites.Soak.4bpp Sprites.Test.4bpp
RESOURCES = Resources.!SoakTest.!Run \
	    Resources.!SoakTest.!RunImage \
	    Resources.!SoakTest.!Sprites \
	    Resources.!SoakTest.SetupPAL \
	    Resources.!SoakTest.SetupNTSC
TARGET    = rm.SoakTest2
TARGETSA  = rm.SoakTest2S
RESAPP    = <resource$dir>.Apps.!SoakTest

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

# Stand-alone version
romsa: ${TARGETSA}
        @echo ${COMPONENT}: stand-alone module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

all: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: disc modules built

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: disc module installed

clean:
        ${RM} ${TARGET}
        ${RM} Sprites.Soak.Frame0
        ${RM} Sprites.Test.Frame0
        @echo ${COMPONENT}: cleaned

resources: ${MPEGS} \
	   ${RESAPP}.!Run \
	   ${RESAPP}.!RunImage \
	   ${RESAPP}.!Sprites \
	   ${RESAPP}.SetupPAL \
	   ${RESAPP}.SetupNTSC \
	   ${RESAPP}.AudioHi \
	   ${RESAPP}.AudioLo \
	   ${RESAPP}.Soak \
	   ${RESAPP}.Test
	@echo ${COMPONENT}: resources built	

${RESAPP}:
	${MKDIR} ${RESAPP}

${RESAPP}.!Run: ${RESAPP}
	${CP} Resources.!SoakTest.!Run      ${RESAPP}.!Run      ${CPFLAGS}

${RESAPP}.!RunImage: ${RESAPP}
	${CP} Resources.!SoakTest.!RunImage ${RESAPP}.!RunImage ${CPFLAGS}
	ifthere Resources.${MACHINE}.!RunImage then ${CP} Resources.${MACHINE}.!RunImage ${RESAPP}.!RunImage ${CPFLAGS}

${RESAPP}.!Sprites: ${RESAPP}
	${CP} Resources.!SoakTest.!Sprites  ${RESAPP}.!Sprites  ${CPFLAGS}

${RESAPP}.SetupPAL: ${RESAPP}
	${CP} Resources.!SoakTest.SetupPAL  ${RESAPP}.SetupPAL  ${CPFLAGS}

${RESAPP}.SetupNTSC: ${RESAPP}
	${CP} Resources.!SoakTest.SetupNTSC ${RESAPP}.SetupNTSC ${CPFLAGS}

${RESAPP}.AudioHi: ${RESAPP} MPEG.AudioHi
	${CP} MPEG.AudioHi                  ${RESAPP}.AudioHi   ${CPFLAGS}

${RESAPP}.AudioLo: ${RESAPP} MPEG.AudioLo
	${CP} MPEG.AudioLo                  ${RESAPP}.AudioLo   ${CPFLAGS}

${RESAPP}.Soak: ${RESAPP} MPEG.Soak
	${CP} MPEG.Soak                     ${RESAPP}.Soak      ${CPFLAGS}

${RESAPP}.Test: ${RESAPP} MPEG.Test
	${CP} MPEG.Test                     ${RESAPP}.Test      ${CPFLAGS}

${TARGET}: ${SOURCE} ${MPEGS} ${NONMPEGS} ${RESOURCES}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${TARGETSA}: ${SOURCESA} ${MPEGS} ${NONMPEGS} ${RESOURCES}
        ${AS} ${ASFLAGS} -From ${SOURCESA} -To $@

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
