# Makefile for BASICTrans
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 08-Jun-94  AMcC   Created.
# 08-Nov-94  AMcC   Updated to install BASIC64 (non ROM)

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = do mkdir -p
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
COMPONENT = BASICTrans
EXPORTS   = ${EXP_HDR}.${COMPONENT}
TARGETS   = rm.${COMPONENT}
MSGSMOD   = BASICTMsgs
MSGSTAR   = rm.${MSGSMOD}
MSGFILE   = rm.Messages

#
# Generic rules:
#
all: ${TARGETS}
        @echo ${COMPONENT}: module built

rom: ${TARGETS}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${TARGETS}
        ${MKDIR} ${INSTDIR}
        ${CP} rm.BASICTrans ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: ram module installed

install_rom: ${TARGETS}
        ${CP} rm.BASICTrans ${INSTDIR}.BASICTrans ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} rm.BASICTrans
        @echo ${COMPONENT}: cleaned

messages:
        Copy Resources.${Locale}.Messages ${MSGFILE} ${CPFLAGS}
        Print Resources.${Locale}.CmdHelp { >> ${MSGFILE} }
        Perl Build:GetVersion Trans$Version
        Do modgen ${MSGSTAR} ${MSGSMOD} ${MSGSMOD} <Trans$Version> ${MSGFILE} Resources.BASIC.Messages
        Remove ${MSGFILE}
        @echo ${COMPONENT}: messages module built

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.BASIC
	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.BASIC.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
        TokenCheck LocalRes:CmdHelp
        print LocalRes:CmdHelp { >> ${RESDIR}.BASIC.Messages }

rm.BASICTrans: s.TransSrc
        ${AS} ${ASFLAGS} s.TransSrc

${EXP_HDR}.BASICTrans: hdr.BASICTrans
        ${CP} hdr.BASICTrans $@ ${CPFLAGS}

# Dynamic dependencies:
