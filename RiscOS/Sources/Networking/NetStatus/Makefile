# Makefile for NetStatus
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
COMPONENT = NetStatus
SOURCE    = s.Module
TARGET    = rm.NetStatus

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

$(TARGET): $(SOURCE)
        $(AS) $(ASFLAGS) $(SOURCE)

maketime:
        MakeTime

version:
        Version

# Dynamic dependencies:
