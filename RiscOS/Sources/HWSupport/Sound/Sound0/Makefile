# Makefile for Sound DMA Handler (Sound0)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
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
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = Sound0
SOURCE    = s.Sound0
TARGET    = rm.${MACHINE}.Sound0
EXPORTS   = ${EXP_HDR}.Sound

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

clean:
        ${RM} ${TARGET}
        ${RM} s.TokHelpSrc
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.SoundDMA
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.SoundDMA.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print Resources.${LOCALE}.CmdHelp { >> ${RESDIR}.SoundDMA.Messages }

${TARGET}: ${SOURCE} s.TokHelpSrc
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

s.TokHelpSrc: Hdr:Tokens HelpSrc
        tokenise Hdr:Tokens HelpSrc $@

${EXP_HDR}.Sound: hdr.Sound
        ${CP} hdr.Sound $@ ${CPFLAGS}

# Dynamic dependencies:
