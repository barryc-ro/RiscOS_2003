# Makefile for Sound Event Scheduler (Sound2)
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
COMPONENT = Sound2
SOURCE    = s.Sound2
TARGET    = rm.Sound2

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
        ${RM} s.TokHelpSrc
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.SoundSched
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.SoundSched.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE} s.TokHelpSrc
        ${AS} ${ASFLAGS} ${SOURCE}

s.TokHelpSrc: Hdr:Tokens HelpSrc
        tokenise Hdr:Tokens HelpSrc $@

# Dynamic dependencies:
