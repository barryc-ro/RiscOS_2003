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
HTMLDIR		= $(RESAPP).Content.HTML
IMAGESDIR	= $(HTMLDIR).Images

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
	$(CP) LocalUserIFRes:!ARMBoot             $(INSTDIR).!ARMBoot      $(CPFLAGS)
	$(CP) LocalUserIFRes:!Boot                $(INSTDIR).!Boot         $(CPFLAGS)
	$(CP) LocalUserIFRes:!NFSBoot             $(INSTDIR).!NFSBoot      $(CPFLAGS)
	$(CP) LocalUserIFRes:!TFTPBoot            $(INSTDIR).!TFTPBoot     $(CPFLAGS)
	$(CP) LocalUserIFRes:Images.back/gif      $(IMAGESDIR).back/gif    $(CPFLAGS)
	$(CP) LocalUserIFRes:Images.bullet/gif    $(IMAGESDIR).bullet/gif  $(CPFLAGS)
	$(CP) LocalUserIFRes:Images.dsl4/gif      $(IMAGESDIR).dsl4/gif    $(CPFLAGS)
	$(CP) LocalUserIFRes:Images.mpeg/gif      $(IMAGESDIR).mpeg/gif    $(CPFLAGS)
	$(CP) LocalUserIFRes:Images.product/gif   $(IMAGESDIR).product/gif $(CPFLAGS)
	$(CP) LocalUserIFRes:Images.smpace/gif    $(IMAGESDIR).smpace/gif  $(CPFLAGS)
	$(CP) LocalUserIFRes:Images.splash        $(IMAGESDIR).splash      $(CPFLAGS)
        @echo $(COMPONENT): RAM module installed

clean:
	@echo $(COMPONENT): cleaned

local_clean:
        $(WIPE) Translated $(WFLAGS)
        @echo $(COMPONENT): cleaned local resources directory

dirs:
        $(MKDIR) $(RESAPP)
        $(MKDIR) $(RESAPP).Apps
        $(MKDIR) $(RESAPP).Content
        $(MKDIR) $(HTMLDIR)
        $(MKDIR) $(IMAGESDIR)
        $(MKDIR) $(RESAPP).Content.Java
        $(MKDIR) $(RESAPP).Content.MPEG
        $(MKDIR) $(RESAPP).Content.Sounds
        $(MKDIR) $(RESAPP).Public
        $(MKDIR) $(RESAPP).Versions

translate: dirs
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSRES)home/html  -o $(HTMLDIR).home/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSRES)mcast/html -o $(HTMLDIR).mcast/html
        @echo Translation Complete.
        
resources: 
        @echo $(COMPONENT): resource files copied
        
# Dynamic dependencies:
