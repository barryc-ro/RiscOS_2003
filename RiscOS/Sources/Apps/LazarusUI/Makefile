# Makefile for INetCardUI
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Jun-98  SNB          Created from OmegaNC2UI
#

#
# Generic options:
#
MKDIR   	= cdir
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
COMPONENT	= INetCardUI
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
        $(MKDIR) $(RESRES)
        $(MKDIR) $(RESRES).NVRAM
        $(MKDIR) $(RESRES).NCDialCtrl
        $(MKDIR) $(RESRES).NCDialCtrl.Files
        $(MKDIR) $(RESRES).NCDialCtrl.Files.PPP
        $(MKDIR) $(RESRES).STBPrint
        IfThere Resources.$(USERIF).$(LOCALE).$(APP) Then \
	  $(CP) Resources.$(USERIF).$(LOCALE).$(APP) $(RESAPP) T~V~CFQR
	$(CP) Resources.$(USERIF).UK.$(APP) $(RESAPP) T~V~CFQR

translate: dirs
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
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.ISPCapture.ServerCnfg -o $(RESAPP).HTML.ISPCapture.ServerCnfg
        @echo ** Translating NCMALite...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.badurl -o $(RESAPP).HTML.MA.badurl
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.badurldbox -o $(RESAPP).HTML.MA.badurldbox
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.connect -o $(RESAPP).HTML.MA.connect
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.logout -o $(RESAPP).HTML.MA.logout
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.pleasewait -o $(RESAPP).HTML.MA.pleasewait
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.splash -o $(RESAPP).HTML.MA.splash
        @echo ** Translating NCOptions...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Acknow -o $(RESAPP).HTML.Setup.Acknow
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.BSD -o $(RESAPP).HTML.Setup.BSD
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.CallWait -o $(RESAPP).HTML.Setup.CallWait
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.IntroWel -o $(RESAPP).HTML.Setup.IntroWel
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Options -o $(RESAPP).HTML.Setup.Options
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Printing -o $(RESAPP).HTML.Setup.Printing
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Printing2 -o $(RESAPP).HTML.Setup.Printing2
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.ssl -o $(RESAPP).HTML.Setup.ssl
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.StartUp -o $(RESAPP).HTML.Setup.StartUp
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Telephone -o $(RESAPP).HTML.Setup.Telephone
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Text -o $(RESAPP).HTML.Setup.Text
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Timeout -o $(RESAPP).HTML.Setup.Timeout
        
resources: translate
	@echo ** Exporting non-translatable resource files
        IfThere Resources.$(USERIF).$(LOCALE).$(APP) Then \
          $(CP) Resources.$(USERIF).$(LOCALE).$(APP) $(RESAPP) $(CPFLAGS) \
        Else \
          $(CP) Resources.$(USERIF).UK.$(APP) $(RESAPP) $(CPFLAGS)
        IfThere Resources.$(USERIF).$(LOCALE).NVRAM Then \
          $(CP) Resources.$(USERIF).$(LOCALE).NVRAM.Tags $(RESRES).NVRAM.Tags $(CPFLAGS) \
        Else \
          $(CP) Resources.$(USERIF).UK.NVRAM.Tags $(RESRES).NVRAM.Tags $(CPFLAGS)
        $(CP) Resources.$(USERIF).$(LOCALE).NCDialCtrl.Files.PPP.IP-Up $(RESRES).NCDialCtrl.Files.PPP.IP-Up $(CPFLAGS)
        $(CP) Resources.$(USERIF).$(LOCALE).STBPrint.PaperA4 $(RESRES).STBPrint.PaperA4 $(CPFLAGS)
        @echo $(COMPONENT): resource files copied
        
# Dynamic dependencies:
