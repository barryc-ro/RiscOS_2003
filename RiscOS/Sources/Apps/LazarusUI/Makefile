# Makefile for LazarusUI
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Jun-98  SNB          Created from OmegaNC2UI
# 09-Jun-00  SBF          Tweaked to build both Lazarus and Bethany UIs
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
COMPONENT	= LazarusUI
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
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.ISPCapture.ISPC3 -o $(RESAPP).HTML.ISPCapture.ISPC3
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.ISPCapture.ISPC4 -o $(RESAPP).HTML.ISPCapture.ISPC4
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.ISPCapture.ISPC5 -o $(RESAPP).HTML.ISPCapture.ISPC5
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.ISPCapture.ISPC6 -o $(RESAPP).HTML.ISPCapture.ISPC6
        @echo ** Translating NCMALite...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.badurldbox -o $(RESAPP).HTML.MA.badurldbox
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.connect -o $(RESAPP).HTML.MA.connect
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.invalid -o $(RESAPP).HTML.MA.invalid
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.logout -o $(RESAPP).HTML.MA.logout
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.MA.pleasewait -o $(RESAPP).HTML.MA.pleasewait
        @echo ** Translating NCOptions...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Acknow -o $(RESAPP).HTML.Setup.Acknow
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.BSD -o $(RESAPP).HTML.Setup.BSD
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.CallWait -o $(RESAPP).HTML.Setup.CallWait
        ### Check for Beth1 build...
        If "<LocaleList>" = "Australia,France,Ireland,Italy,SAfrica,UK" Then \
          $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Country1 -o $(RESAPP).HTML.Setup.Country
        ### Check for Beth2 build...
        If "<LocaleList>" = "Germany,Spain,Turkey" Then \
          $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Country2 -o $(RESAPP).HTML.Setup.Country
        ### If a multi-locale build then we require CountryCfm also...
        If "<LocaleList>" <> "" Then \
          $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.CountryCfm -o $(RESAPP).HTML.Setup.CountryCfm
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.ImageLib -o $(RESAPP).HTML.Setup.ImageLib
        ### If a single-locale (standard) build then use the original (Lazarus) options screen...
        If "<LocaleList>" = "" Then \
          $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Options1 -o $(RESAPP).HTML.Setup.Options
        ### Otherwise use the options screen that has the "country configuration" button (Bethany variant)
        ### (Would liked to have used the "Else" case here, but the expanded line length exceeds 255 characters!)
        If "<LocaleList>" <> "" Then \
          $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Options2 -o $(RESAPP).HTML.Setup.Options
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Printing -o $(RESAPP).HTML.Setup.Printing
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Printing2 -o $(RESAPP).HTML.Setup.Printing2
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.SSL -o $(RESAPP).HTML.Setup.SSL
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.StartUp -o $(RESAPP).HTML.Setup.StartUp
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Telephone -o $(RESAPP).HTML.Setup.Telephone
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Text -o $(RESAPP).HTML.Setup.Text
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Setup.Timeout -o $(RESAPP).HTML.Setup.Timeout
        @echo ** Translating miscellaneous screens...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i TranslateRes:$(APP).HTML.Other.HeroList -o $(RESAPP).HTML.Other.HeroList

        
resources: translate
	@echo ** Exporting non-translatable resource files
	SetMacro LazUILocalRes$path Resources.$(USERIF).$(LOCALE).,Resources.$(USERIF).UK.
        $(CP) LazUILocalRes:$(APP).!BootNet $(RESAPP).!BootNet $(CPFLAGS)
        $(CP) LazUILocalRes:$(APP) $(RESAPP) $(CPFLAGS)
        $(CP) LazUILocalRes:$(APP).!Run $(RESAPP).!Run $(CPFLAGS)
        $(CP) LazUILocalRes:$(APP).Palette $(RESAPP).Palette $(CPFLAGS)
        $(CP) LazUILocalRes:NVRAM.$(MACHINE).Tags $(RESRES).NVRAM.Tags $(CPFLAGS)
        $(CP) LazUILocalRes:STBPrint.PaperA4 $(RESRES).STBPrint.PaperA4 $(CPFLAGS)
        $(CP) LazUILocalRes:$(APP).bin.command $(RESAPP).bin.command $(CPFLAGS)
        $(CP) LazUILocalRes:$(APP).bin.FlashPod $(RESAPP).bin.FlashPod $(CPFLAGS)
        @echo $(COMPONENT): resource files copied
        UnSet LazUILocalRes$path
        
# Dynamic dependencies:
