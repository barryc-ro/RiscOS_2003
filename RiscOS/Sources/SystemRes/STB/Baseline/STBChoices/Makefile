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
COMPONENT	= STBChoices
APP		= !STBBoot
SOURCE		=
TARGET		=

# Resources directories
LOCALRES        = LocalUserIFRes:
RESCHOICES      = $(LOCALRES)Choices
RESDESKTOP      = $(LOCALRES)Desktop
RESPREDESK      = $(LOCALRES)PreDesktop

# Translate directorites
TRANSCHOICES    = $(TRANSRES)Choices
TRANSDESKTOP    = $(TRANSRES)Desktop
TRANSPREDESK    = $(TRANSRES)PreDesktop

# Output directories.
OUTCHOICES      = $(INSTDIR).Choices
OUTDESKTOP      = $(INSTDIR).Desktop
OUTPREDESK      = $(INSTDIR).PreDesktop

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
        @echo Building Choices dir...
	$(CP) $(RESCHOICES).DEBUG                $(OUTCHOICES).DEBUG              $(CPFLAGS)
	$(CP) $(RESCHOICES).MPEGdtt              $(OUTCHOICES).MPEGdtt            $(CPFLAGS)
	$(CP) $(RESCHOICES).Multicast.Config     $(OUTCHOICES).Multicast.Config   $(CPFLAGS)
	$(CP) $(RESCHOICES).NCFresco             $(OUTCHOICES).NCFresco           $(CPFLAGS)
	$(CP) $(RESCHOICES).Splash               $(OUTCHOICES).Splash             $(CPFLAGS)
	$(CP) $(RESCHOICES).STBBoot              $(OUTCHOICES).STBBoot            $(CPFLAGS)
	$(CP) $(RESCHOICES).Territory.Config     $(OUTCHOICES).Territory.Config   $(CPFLAGS)
	$(CP) $(RESCHOICES).Territory.Override   $(OUTCHOICES).Territory.Override $(CPFLAGS)
        @echo Building Desktop dir...
	$(CP) $(RESDESKTOP).300.STB3             $(OUTDESKTOP).300.STB3           $(CPFLAGS)
        @echo Building PreDesktop dir...
	$(CP) $(RESPREDESK).DEBUG                $(OUTPREDESK).DEBUG              $(CPFLAGS)
	$(CP) $(RESPREDESK).MsgQueue             $(OUTPREDESK).MsgQueue           $(CPFLAGS)
	$(CP) $(RESPREDESK).Territory            $(OUTPREDESK).Territory          $(CPFLAGS)
        @echo $(COMPONENT): RAM module installed

clean:
	@echo $(COMPONENT): cleaned

local_clean:
        $(WIPE) Translated $(WFLAGS)
        @echo $(COMPONENT): cleaned local resources directory

dirs:
        $(MKDIR) $(RESAPP)
        $(MKDIR) $(OUTCHOICES)
        $(MKDIR) $(OUTCHOICES).Multicast
        $(MKDIR) $(OUTCHOICES).Monitors
        $(MKDIR) $(OUTCHOICES).Territory
        $(MKDIR) $(OUTDESKTOP).220
        $(MKDIR) $(OUTDESKTOP).300
        $(MKDIR) $(OUTDESKTOP).400
        $(MKDIR) $(OUTPREDESK).220
        $(MKDIR) $(OUTPREDESK).300
        $(MKDIR) $(OUTPREDESK).400

translate: dirs
        @echo Translating Choices dir...
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSCHOICES).Multicast.Channels    -o $(OUTCHOICES).Multicast.Channels
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSCHOICES).Multicast.Errors      -o $(OUTCHOICES).Multicast.Errors
        @echo Translating Desktop dir...
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSDESKTOP).300.NCFresco          -o $(OUTDESKTOP).300.NCFresco
        @echo Translating PreDesktop dir...
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSPREDESK).300.STB3              -o $(OUTPREDESK).300.STB3
        @echo Translation Complete.
        
resources: 
        @echo $(COMPONENT): resource files copied
        
# Dynamic dependencies:
