# Makefile for STBBoot
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Aug-99  SNB          Created from OmegaNC2UI
#

#
# Generic options:
#
MKDIR   	= mkdir -p
CP      	= copy
TRANSLATE	= translate
RM      	= remove
WIPE		= -wipe

CPFLAGS 	= ~cfr~v
WFLAGS		= ~cf~vr

# Translate flags
TRANSFLAGS	= $(THROWBACK) -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"
TRANSMAPPINGS	= TranslateRes:Mappings
TRANSRES	= TranslateRes:
TRANSOUT	= $(INSTDIR)

# Copy Flags
CPFLAGS		= QRF~C~V

#
# Program specific options:
#
COMPONENT	= STBBoot
APP		= !STBBoot
SOURCE		=
TARGET		=

#
# Export Paths for Messages module
#
RESAPP		= $(INSTDIR)
RESRES 		= $(INSTDIR).Resources

#
# Generic rules:
#
rom: $(TARGET)
        @echo $(COMPONENT): ROM module built

export: $(EXPORTS)
        @echo $(COMPONENT): export complete

install_rom: $(TARGET)
        @echo $(COMPONENT): ROM module installed

install: translate dirs
        @echo Installing top level...
	$(CP) LocalUserIFRes:!Boot                        $(RESAPP).!Boot                        $(CPFLAGS)
	$(CP) LocalUserIFRes:!Sprites                     $(RESAPP).!Sprites                     $(CPFLAGS)
	$(CP) LocalUserIFRes:!Sprites22                   $(RESAPP).!Sprites22                   $(CPFLAGS)
	$(CP) LocalUserIFRes:!Sprites23                   $(RESAPP).!Sprites23                   $(CPFLAGS)
	$(CP) LocalUserIFRes:Library.IRDefault            $(RESAPP).Library.IRDefault            $(CPFLAGS)
	$(CP) LocalUserIFRes:Utilities.BootErrors         $(RESAPP).Utilities.BootErrors         $(CPFLAGS)
	$(CP) LocalUserIFRes:Utilities.BootVars           $(RESAPP).Utilities.BootVars           $(CPFLAGS)
	$(CP) LocalUserIFRes:Utilities.CheckLocal         $(RESAPP).Utilities.CheckLocal         $(CPFLAGS)
	$(CP) LocalUserIFRes:Utilities.ClearPtr           $(RESAPP).Utilities.ClearPtr           $(CPFLAGS)
	$(CP) LocalUserIFRes:Utilities.NormPtr            $(RESAPP).Utilities.NormPtr            $(CPFLAGS)
        @echo $(COMPONENT): RAM module installed

clean:
	@echo $(COMPONENT): cleaned

local_clean:
        $(WIPE) Translated $(WFLAGS)
        @echo $(COMPONENT): cleaned local resources directory

dirs:
        $(MKDIR) $(RESAPP)
        $(MKDIR) $(RESAPP).Choices
        $(MKDIR) $(RESAPP).Choices.Multicast
        $(MKDIR) $(RESAPP).Choices.Monitors
        $(MKDIR) $(RESAPP).Choices.Territory
        $(MKDIR) $(RESAPP).Desktop
        $(MKDIR) $(RESAPP).Desktop.220
        $(MKDIR) $(RESAPP).Desktop.300
        $(MKDIR) $(RESAPP).Desktop.400
        $(MKDIR) $(RESAPP).PreDesktop
        $(MKDIR) $(RESAPP).PreDesktop.220
        $(MKDIR) $(RESAPP).PreDesktop.300
        $(MKDIR) $(RESAPP).PreDesktop.400
        $(MKDIR) $(RESAPP).Library
        $(MKDIR) $(RESAPP).Resources
        $(MKDIR) $(RESAPP).Resources.220
        $(MKDIR) $(RESAPP).Resources.220.Misc
        $(MKDIR) $(RESAPP).Resources.300
        $(MKDIR) $(RESAPP).Resources.300.Misc
        $(MKDIR) $(RESAPP).Resources.400
        $(MKDIR) $(RESAPP).Resources.400.Misc
        $(MKDIR) $(RESAPP).Utilities

translate: dirs
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSRES)!Run                    -o $(TRANSOUT).!Run
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSRES)Utilities.Desktop       -o $(TRANSOUT).Utilities.Desktop
        @echo Translation Complete.
        
resources: 
        @echo $(COMPONENT): resource files copied
        
# Dynamic dependencies:
