# Makefile for CBAI
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 05-Jan-2001  BJGA         Created.
#

#
# Generic options:
#
MKDIR   = mkdir -p
CP      = copy
RM      = remove
WIPE    = -wipe
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = CBAI
SOURCE    = bas.IRQMod2
TARGET    = rm.$(COMPONENT)

#
# Generic rules:
#
rom: $(TARGET)
        @echo $(COMPONENT): rom module built

install_rom: $(TARGET)
        $(MKDIR) $(INSTDIR)
        $(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        @echo $(COMPONENT): rom module installed

install: $(TARGET)
        $(MKDIR) $(INSTDIR)
        $(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        @echo $(COMPONENT): module installed

clean:
        $(WIPE) rm $(WFLAGS)
        @echo $(COMPONENT): cleaned

$(TARGET): $(SOURCE)
        $(MKDIR) rm
        /$(SOURCE)

# Dynamic dependencies:
