# Makefile for FileSwitch
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
STRIP   = stripdepnd
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = FileSwitch
SOURCE    = s.FileSwitch
TARGET    = rm.FileSwitch
EXPORTS   = ${EXP_HDR}.LowFSI \
            ${EXP_HDR}.HighFSI

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
        ${STRIP}
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
        TokenCheck LocalRes:CmdHelp
	print LocalRes:CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

${TARGET}: ${SOURCE} s.TokHelpSrc
        ${AS} ${ASFLAGS} ${SOURCE}

s.TokHelpSrc: Hdr:Tokens HelpSrc
        tokenise Hdr:Tokens HelpSrc $@

${EXP_HDR}.LowFSI: hdr.LowFSI
        ${CP} hdr.LowFSI $@ ${CPFLAGS}

${EXP_HDR}.HighFSI: hdr.HighFSI
        ${CP} hdr.HighFSI $@ ${CPFLAGS}

# Dynamic dependencies:
