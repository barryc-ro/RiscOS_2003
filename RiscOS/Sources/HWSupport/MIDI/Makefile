# MakeFile for MIDI Podule
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created for version 3.11
# 17-Jun-97  KJB          Version 3.14 sources found
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = MIDI
SOURCE    = s.MIDI
TARGET    = rm.MIDI
EXPORTS   = ${EXP_HDR}.${COMPONENT}

#
# Generic options:
#
MKDIR    = cdir
AS       = aasm
CP       = copy
MODGEN   = modgen
RM       = remove
TOKENISE = tokenise

ASFLAGS  = -depend !Depend -stamp -closeexec -quit
CPFLAGS  = ~cfr~v

TOKENS   = Hdr:Tokens

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

resources:
        @|${MKDIR} ${RESDIR}
        @|${CP} ${LDIR}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${RM} ${MODULE}
        ${RM} rm.MIDIlog
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

rm.MIDIlog: s.MIDIlog
        ${AS} ${ASFLAGS} s.MIDIlog

#---------------------------------------------------------------------------
# Dynamic dependencies:
