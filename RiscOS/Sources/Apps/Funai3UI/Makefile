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
MAPPINGS	= UITranslateRes:Mappings

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
        $(MKDIR) $(RESRES).ICAClient
        IfThere Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.${APP} Then \
          $(CP) Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.${APP} ${RESAPP} T~V~CFQR
        IfThere Resources.${USERIF}.${LOCALE}.${APP} Then \
          $(CP) Resources.${USERIF}.${LOCALE}.${APP} ${RESAPP} T~V~CFQR
        IfThere Resources.${USERIF}.UK.${UI_VARIANT}.${APP} Then \
          $(CP) Resources.${USERIF}.UK.${UI_VARIANT}.${APP} ${RESAPP} T~V~CFQR
        IfThere Resources.${USERIF}.UK.${APP} Then \
          $(CP) Resources.${USERIF}.UK.${APP} ${RESAPP} T~V~CFQR
        IfThere Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.${APP} Then \
          $(CP) Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.${APP} $(RESAPP) T~V~CFQR
        IfThere Translate.${USERIF}.${LOCALE}.${APP} Then \
          $(CP) Translate.${USERIF}.${LOCALE}.${APP} $(RESAPP) T~V~CFQR
        IfThere Translate.${USERIF}.UK.${UI_VARIANT}.${APP} Then \
          $(CP) Translate.${USERIF}.UK.${UI_VARIANT}.${APP} $(RESAPP) T~V~CFQR
        IfThere Translate.${USERIF}.UK.${APP} Then \
          $(CP) Translate.${USERIF}.UK.${APP} $(RESAPP) T~V~CFQR

translate:
        Set UITranslateRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.
        @echo ** Translating NCDialler...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Dialling.Busy -o $(RESAPP).HTML.Dialling.Busy
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Dialling.Connect -o $(RESAPP).HTML.Dialling.Connect
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Dialling.Dialling -o $(RESAPP).HTML.Dialling.Dialling
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Dialling.Error -o $(RESAPP).HTML.Dialling.Error
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Dialling.NoDial -o $(RESAPP).HTML.Dialling.NoDial
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Dialling.Server -o $(RESAPP).HTML.Dialling.Server
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Dialling.Verify -o $(RESAPP).HTML.Dialling.Verify
        @echo ** Translating ISPCapture...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.ISPCapture.ISPC1 -o $(RESAPP).HTML.ISPCapture.ISPC1
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.ISPCapture.ISPC2 -o $(RESAPP).HTML.ISPCapture.ISPC2
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.ISPCapture.ISPC3 -o $(RESAPP).HTML.ISPCapture.ISPC3
        @echo ** Translating NCMALite...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.badpin -o $(RESAPP).HTML.MA.badpin
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.badsvc -o $(RESAPP).HTML.MA.badsvc
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.badurl -o $(RESAPP).HTML.MA.badurl
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.connectB -o $(RESAPP).HTML.MA.connectB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.connectN -o $(RESAPP).HTML.MA.connectN
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.connectP -o $(RESAPP).HTML.MA.connectP
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.connectSB -o $(RESAPP).HTML.MA.connectSB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.connectSP -o $(RESAPP).HTML.MA.connectSP
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.invalid -o $(RESAPP).HTML.MA.invalid
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.ispcfailB -o $(RESAPP).HTML.MA.ispcfailB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.ispcfailSB -o $(RESAPP).HTML.MA.ispcfailSB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.mismatch -o $(RESAPP).HTML.MA.mismatch
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.needpin -o $(RESAPP).HTML.MA.needpin
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.newpin -o $(RESAPP).HTML.MA.newpin
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.pin -o $(RESAPP).HTML.MA.pin
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.pleasewait -o $(RESAPP).HTML.MA.pleasewait
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.poweroff -o $(RESAPP).HTML.MA.poweroff
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.SCprobs -o $(RESAPP).HTML.MA.SCprobs
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.smartcardB -o $(RESAPP).HTML.MA.smartcardB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.smartcardP -o $(RESAPP).HTML.MA.smartcardP
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.smartcrdSB -o $(RESAPP).HTML.MA.smartcrdSB
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.smartcrdSP -o $(RESAPP).HTML.MA.smartcrdSP
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.splash -o $(RESAPP).HTML.MA.splash
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.MA.unblock -o $(RESAPP).HTML.MA.unblock
        @echo ** Translating NCOptions...
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.Acknow -o $(RESAPP).HTML.Setup.Acknow
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.BSD -o $(RESAPP).HTML.Setup.BSD
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.IntroWel -o $(RESAPP).HTML.Setup.IntroWel
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML-Ether.Setup.Options -o $(RESAPP).HTML-Ether.Setup.Options
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML-Modem.Setup.Options -o $(RESAPP).HTML-Modem.Setup.Options
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.Printing -o $(RESAPP).HTML.Setup.Printing
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.Printing2 -o $(RESAPP).HTML.Setup.Printing2
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.Screen -o $(RESAPP).HTML.Setup.Screen
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.Sounds -o $(RESAPP).HTML.Setup.Sounds
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.ssl -o $(RESAPP).HTML.Setup.ssl
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.StartUp -o $(RESAPP).HTML.Setup.StartUp
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.Telephone -o $(RESAPP).HTML.Setup.Telephone
        $(TRANSLATE) $(TRANSFLAGS) -m $(MAPPINGS) -i UITranslateRes:$(APP).HTML.Setup.Text -o $(RESAPP).HTML.Setup.Text
        
resources: translate dirs
        @echo ** Exporting non-translatable resource files
        Set LocalUIResources$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.
        $(CP) LocalUIResources:!Boot.!Run $(RESAPP).!Run $(CPFLAGS)
        $(CP) LocalUIResources:!Boot.Palette $(RESAPP).Palette $(CPFLAGS)
        $(CP) LocalUIResources:!Boot.HTML $(RESAPP).HTML $(CPFLAGS)
        IfThere LocalUIResources:!Boot.HTML-Ether Then $(CP) LocalUIResources:!Boot.HTML-Ether $(RESAPP).HTML-Ether $(CPFLAGS)
        IfThere LocalUIResources:!Boot.HTML-Modem Then $(CP) LocalUIResources:!Boot.HTML-Modem $(RESAPP).HTML-Modem $(CPFLAGS)
        IfThere LocalUIResources:NCFresco Then $(CP) LocalUIResources:NCFresco $(RESRES).NCFresco $(CPFLAGS)
        IfThere LocalUIResources:NCFrescoA Then $(CP) LocalUIResources:NCFrescoA $(RESRES).NCFrescoA $(CPFLAGS)
        $(CP) Resources.$(USERIF).UK.!Boot.Utils $(RESAPP).Utils $(CPFLAGS)
        $(CP) LocalUIResources:!Boot.Utils $(RESAPP).Utils $(CPFLAGS)
        $(CP) LocalUIResources:!Boot.Library $(RESAPP).Library $(CPFLAGS)
        $(CP) LocalUIResources:NVRAM.Tags $(RESRES).NVRAM.Tags $(CPFLAGS)
        $(CP) LocalUIResources:Wimp.Sprites $(RESRES).Wimp.Sprites $(CPFLAGS)
        IfThere LocalUIResources:WinFrameRO Then $(CP) LocalUIResources:WinFrameRO $(RESRES).ICAClient $(CPFLAGS)
        @echo $(COMPONENT): resource files copied
        
# Dynamic dependencies:
