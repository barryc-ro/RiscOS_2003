# Makefile for Obey
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 08-Jun-94  AMcC         Created.
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
COMPONENT = Obey
SOURCE    = s.Obey
TARGET    = rm.Obey

#
# Generic rules:
#
rom: $(TARGET)
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: $(TARGET)
        $(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        @echo ${COMPONENT}: rom module installed

clean:
        $(RM) $(TARGET)
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

$(TARGET): $(SOURCE)
        $(AS) $(ASFLAGS) $(SOURCE)

# Dynamic dependencies:
