# Makefile for WaveSynth
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = WaveSynth
SOURCE    = s.WaveSynth
TARGET    = rm.${MACHINE}.WaveSynth

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.Jordan.WaveSynth: Hdr:ListOpts
rm.Jordan.WaveSynth: Hdr:Macros
rm.Jordan.WaveSynth: Hdr:System
rm.Jordan.WaveSynth: Hdr:SWIs
rm.Jordan.WaveSynth: Hdr:CPU.Generic26
rm.Jordan.WaveSynth: Hdr:IO.GenericIO
rm.Jordan.WaveSynth: Hdr:RISCOS
rm.Jordan.WaveSynth: Hdr:Machine.<Machine>
rm.Jordan.WaveSynth: Hdr:ModHand
rm.Jordan.WaveSynth: Hdr:PublicWS
rm.Jordan.WaveSynth: Hdr:Sound
rm.Jordan.WaveSynth: Hdr:Proc
rm.Jordan.WaveSynth: Hdr:MsgTrans
rm.Jordan.WaveSynth: Hdr:Services
rm.Jordan.WaveSynth: Version
rm.Jordan.WaveSynth: s.MsgCode
rm.Omega.WaveSynth: Hdr:ListOpts
rm.Omega.WaveSynth: Hdr:Macros
rm.Omega.WaveSynth: Hdr:System
rm.Omega.WaveSynth: Hdr:SWIs
rm.Omega.WaveSynth: Hdr:CPU.Generic26
rm.Omega.WaveSynth: Hdr:IO.GenericIO
rm.Omega.WaveSynth: Hdr:RISCOS
rm.Omega.WaveSynth: Hdr:Machine.<Machine>
rm.Omega.WaveSynth: Hdr:ModHand
rm.Omega.WaveSynth: Hdr:PublicWS
rm.Omega.WaveSynth: Hdr:Sound
rm.Omega.WaveSynth: Hdr:Proc
rm.Omega.WaveSynth: Hdr:MsgTrans
rm.Omega.WaveSynth: Hdr:Services
rm.Omega.WaveSynth: Version
rm.Omega.WaveSynth: s.MsgCode
