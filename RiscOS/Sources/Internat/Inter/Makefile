# Makefile for International module
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

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = Inter
SOURCE    = s.InterMod
TARGET    = rm.Inter

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
        ${RM} s.IntHelpTok
        ${RM} s.IntHelp
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.Internatio
 	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.Internatio.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print LocalRes:CmdHelp { >> ${RESDIR}.Internatio.Messages }

${TARGET}: ${SOURCE} s.IntHelpTok
        ${AS} ${ASFLAGS} ${SOURCE}

s.IntHelpTok: ${TOKENS} s.IntHelp
        ${TOKENISE} ${TOKENS} s.IntHelp $@

s.IntHelp: IntHelpSrc IntHelpBody
        ${AS} -depend !Depend -Stamp -quit -To $@ -From IntHelpSrc

# Dynamic dependencies:
