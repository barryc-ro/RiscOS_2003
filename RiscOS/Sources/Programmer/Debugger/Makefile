# Makefile for Debugger
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 08-Jun-94  AMcC         Created.
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
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = Debugger
SOURCE    = s.Debugger
TARGET    = rm.${MACHINE}.Debugger
EXPORTS   = ${EXP_HDR}.Debugger

#
# Generic rules:
#
rom: $(TARGET) local_dirs
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: $(TARGET)
        $(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} rm.${MACHINE} ${WFLAGS}
        ${RM} rm.${MACHINE}
        $(RM) $(TARGET)
        $(RM) TokHelpSrc
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

local_dirs:
        ${MKDIR} rm.${MACHINE}

$(TARGET): $(SOURCE) TokHelpSrc
        ${MKDIR} rm.${MACHINE}
        $(AS) $(ASFLAGS) $(SOURCE)

${EXP_HDR}.Debugger: hdr.Debugger
        ${CP} hdr.Debugger $@ ${CPFLAGS}

TokHelpSrc: $(TOKENS) HelpSrc
        $(TOKENISE) $(TOKENS) HelpSrc $@


# Dynamic dependencies:
