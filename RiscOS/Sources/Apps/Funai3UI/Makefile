# Makefile for Funai3UI
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Jun-98  SNB          Original makefile dumped in preference to
#                         OmegaNC2UI's.
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

# Translation mappings file
MAPPINGS	= TranslateRes:Mappings

#
# Program specific options:
#
COMPONENT	= Funai3UI
APP		= !Boot
SOURCE		=
TARGET		=

#
# Export Paths for Messages module
#
EXPORTDIR	= $(RESDIR).^
RESAPP		= $(EXPORTDIR).$(APP)
RESRES 		= $(EXPORTDIR).Resources

#
# Generic rules:
#
rom: $(TARGET)
        @echo $(COMPONENT): ROM module built

export: $(EXPORTS)
        @echo $(COMPONENT): export complete

install_rom: $(TARGET)
        @echo $(COMPONENT): ROM module installed

clean:
	@echo $(COMPONENT): cleaned

local_clean:
        $(WIPE) Translated $(WFLAGS)
        @echo $(COMPONENT): cleaned local resources directory

dirs:
        @echo ** Creating directory structure
        $(MKDIR) $(EXPORTDIR)
        $(MKDIR) $(RESAPP)
        $(MKDIR) $(RESAPP).HTML.MA
        $(MKDIR) $(RESAPP).HTML-Ether
        $(MKDIR) $(RESAPP).HTML-Modem
        $(MKDIR) $(RESAPP).HTML-Ether.Setup
        $(MKDIR) $(RESAPP).HTML-Modem.Setup
        $(MKDIR) $(RESRES)
        $(MKDIR) $(RESRES).NVRAM
        $(MKDIR) $(RESRES).Wimp
        $(MKDIR) $(RESRES).NCFresco
        $(MKDIR) $(RESRES).NCFrescoA
        $(MKDIR) $(RESRES).ScrSaver
        IfThere Resources.$(USERIF).$(LOCALE).$(APP) Then \
	  $(CP) Resources.$(USERIF).$(LOCALE).$(APP) $(RESAPP) T~V~CFQR
	$(CP) Resources.$(USERIF).UK.$(APP) $(RESAPP) T~V~CFQR

translate:
        @echo ** Translating NCDialler...
	$(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Dialling.Busy -o $(RESAPP).HTML.Dialling.Busy
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Dialling.Connect -o $(RESAPP).HTML.Dialling.Connect
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Dialling.Dialling -o $(RESAPP).HTML.Dialling.Dialling
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Dialling.Error -o $(RESAPP).HTML.Dialling.Error
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Dialling.NoDial -o $(RESAPP).HTML.Dialling.NoDial
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Dialling.Server -o $(RESAPP).HTML.Dialling.Server
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Dialling.Verify -o $(RESAPP).HTML.Dialling.Verify
        @echo ** Translating ISPCapture...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.ISPCapture.ISPC1 -o $(RESAPP).HTML.ISPCapture.ISPC1
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.ISPCapture.ISPC2 -o $(RESAPP).HTML.ISPCapture.ISPC2
         $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.ISPCapture.ISPC3 -o $(RESAPP).HTML.ISPCapture.ISPC3
       @echo ** Translating NCMALite...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.badpin -o $(RESAPP).HTML.MA.badpin
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.badsvc -o $(RESAPP).HTML.MA.badsvc
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.badurl -o $(RESAPP).HTML.MA.badurl
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.connectB -o $(RESAPP).HTML.MA.connectB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.connectN -o $(RESAPP).HTML.MA.connectN
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.connectP -o $(RESAPP).HTML.MA.connectP
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.connectSB -o $(RESAPP).HTML.MA.connectSB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.connectSP -o $(RESAPP).HTML.MA.connectSP
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.invalid -o $(RESAPP).HTML.MA.invalid
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.ispcfailB -o $(RESAPP).HTML.MA.ispcfailB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.ispcfailSB -o $(RESAPP).HTML.MA.ispcfailSB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.mismatch -o $(RESAPP).HTML.MA.mismatch
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.needpin -o $(RESAPP).HTML.MA.needpin
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.newpin -o $(RESAPP).HTML.MA.newpin
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.pin -o $(RESAPP).HTML.MA.pin
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.pleasewait -o $(RESAPP).HTML.MA.pleasewait
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.poweroff -o $(RESAPP).HTML.MA.poweroff
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.SCprobs -o $(RESAPP).HTML.MA.SCprobs
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.smartcardB -o $(RESAPP).HTML.MA.smartcardB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.smartcardP -o $(RESAPP).HTML.MA.smartcardP
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.smartcrdSB -o $(RESAPP).HTML.MA.smartcrdSB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.smartcrdSP -o $(RESAPP).HTML.MA.smartcrdSP
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.splash -o $(RESAPP).HTML.MA.splash
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.unblock -o $(RESAPP).HTML.MA.unblock
        @echo ** Translating NCOptions...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Acknow -o $(RESAPP).HTML.Setup.Acknow
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.BSD -o $(RESAPP).HTML.Setup.BSD
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.IntroWel -o $(RESAPP).HTML.Setup.IntroWel
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML-Ether.Setup.Options -o $(RESAPP).HTML-Ether.Setup.Options
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML-Modem.Setup.Options -o $(RESAPP).HTML-Modem.Setup.Options
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Printing -o $(RESAPP).HTML.Setup.Printing
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Printing2 -o $(RESAPP).HTML.Setup.Printing2
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Sounds -o $(RESAPP).HTML.Setup.Sounds
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.ssl -o $(RESAPP).HTML.Setup.ssl
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.StartUp -o $(RESAPP).HTML.Setup.StartUp
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Telephone -o $(RESAPP).HTML.Setup.Telephone
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Text -o $(RESAPP).HTML.Setup.Text
        
resources: translate dirs
	@echo ** Exporting non-translatable resource files
        $(CP) LocalUserIFRes:!Boot.!Run $(RESAPP).!Run $(CPFLAGS)
        $(CP) LocalUserIFRes:!Boot.Palette $(RESAPP).Palette $(CPFLAGS)
        $(CP) LocalUserIFRes:!Boot.HTML $(RESAPP).HTML $(CPFLAGS)
        IfThere LocalUserIFRes:!Boot.HTML-Ether Then $(CP) LocalUserIFRes:!Boot.HTML-Ether $(RESAPP).HTML-Ether $(CPFLAGS)
        IfThere LocalUserIFRes:!Boot.HTML-Modem Then $(CP) LocalUserIFRes:!Boot.HTML-Modem $(RESAPP).HTML-Modem $(CPFLAGS)
        $(CP) LocalUserIFRes:!Boot.Utils $(RESAPP).Utils $(CPFLAGS)
        $(CP) LocalUserIFRes:NVRAM.Tags $(RESRES).NVRAM.Tags $(CPFLAGS)
        $(CP) LocalUserIFRes:Wimp.Sprites $(RESRES).Wimp.Sprites $(CPFLAGS)
        @echo $(COMPONENT): resource files copied
        
# Dynamic dependencies:
