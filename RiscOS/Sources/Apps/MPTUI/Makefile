# Makefile for MPTUI
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Sep-98  SBF          Created from FunaiUI Makefile
# 30-Mar-99  JBeranek     Modified to work for MPTUI
# 31-Mar-99  JBeranek     Modified to allow translations to be exported locally for
#                         development.  Will also now fall back to UK resources if
#                         resources for current LOCALE do not exist.
#

#
# Generic options:
#
MKDIR   	= cdir
AS		= aasm
CP      	= copy
TRANSLATOR	= translator
RM      	= remove
WIPE		= -wipe

CCFLAGS 	= -c -depend !Depend -IC:
ASFLAGS 	= -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS 	= ~cfr~v
WFLAGS		= ~cf~vr

# Translator flags
TRANSFLAGS	= ${THROWBACK} -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"

#
# Program specific options:
#
COMPONENT	= MPTUI
APP		= !Boot
SOURCE		=
TARGET		=

#
# Export Paths for Messages module
#
RESAPP		= ${RESDIR}.${APP}
RESRES 		= ${RESDIR}.Resources

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: ROM module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        @echo $(COMPONENT}: ROM module installed

clean:
	@echo ${COMPONENT}: cleaned

local_clean:
        @echo ${COMPONENT}: cleaned local resources directory
        ${WIPE} Translated ${WFLAGS}

dirs:
        @echo ** Creating directory structure
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESDIR}.${APP}
        ${MKDIR} ${RESRES}
        ${MKDIR} ${RESRES}.NVRAM
        ${MKDIR} ${RESRES}.Wimp
        IfThere Resources.${USERIF}.${LOCALE}.${APP} Then \
	  ${CP} Resources.${USERIF}.${LOCALE}.${APP} ${RESDIR}.${APP} T~V~CFQR \
	Else \
	  ${CP} Resources.${USERIF}.UK.${APP} ${RESDIR}.${APP} T~V~CFQR

translate: dirs
        @echo ** Translating NCDialler...
	$(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Dialling.Busy -o ${RESAPP}.HTML.Dialling.Busy
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Dialling.Connect -o ${RESAPP}.HTML.Dialling.Connect
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Dialling.Dialling -o ${RESAPP}.HTML.Dialling.Dialling
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Dialling.Error -o ${RESAPP}.HTML.Dialling.Error
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Dialling.NoDial -o ${RESAPP}.HTML.Dialling.NoDial
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Dialling.Server -o ${RESAPP}.HTML.Dialling.Server
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Dialling.Verify -o ${RESAPP}.HTML.Dialling.Verify
        @echo ** Translating ISPCapture...
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.ISPCapture.ISPC1 -o ${RESAPP}.HTML.ISPCapture.ISPC1
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.ISPCapture.ISPC2 -o ${RESAPP}.HTML.ISPCapture.ISPC2
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.ISPCapture.ServerCnfg -o ${RESAPP}.HTML.ISPCapture.ServerCnfg
        @echo ** Translating NCMALite...
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.badpin -o ${RESAPP}.HTML.MA.badpin
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.badubp -o ${RESAPP}.HTML.MA.badubp
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.badurl -o ${RESAPP}.HTML.MA.badurl
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.badurldbox -o ${RESAPP}.HTML.MA.badurldbox
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.connect -o ${RESAPP}.HTML.MA.connect
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.correct -o ${RESAPP}.HTML.MA.correct
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.damaged -o ${RESAPP}.HTML.MA.damaged
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.invalid -o ${RESAPP}.HTML.MA.invalid
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.logout -o ${RESAPP}.HTML.MA.logout
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.mismatch -o ${RESAPP}.HTML.MA.mismatch
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.newpin -o ${RESAPP}.HTML.MA.newpin
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.pin -o ${RESAPP}.HTML.MA.pin
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.pleasewait -o ${RESAPP}.HTML.MA.pleasewait
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.poweroff -o ${RESAPP}.HTML.MA.poweroff
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.SCprobs -o ${RESAPP}.HTML.MA.SCprobs
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.splash -o ${RESAPP}.HTML.MA.splash
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.MA.unblock -o ${RESAPP}.HTML.MA.unblock
        @echo ** Translating NCOptions...
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.Acknow -o ${RESAPP}.HTML.Setup.Acknow
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.BSD -o ${RESAPP}.HTML.Setup.BSD
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.IntroWel -o ${RESAPP}.HTML.Setup.IntroWel
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.Options -o ${RESAPP}.HTML.Setup.Options
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.Printing -o ${RESAPP}.HTML.Setup.Printing
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.Printing2 -o ${RESAPP}.HTML.Setup.Printing2
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.Sounds -o ${RESAPP}.HTML.Setup.Sounds
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.ssl -o ${RESAPP}.HTML.Setup.ssl
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.StartUp -o ${RESAPP}.HTML.Setup.StartUp
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.Telephone -o ${RESAPP}.HTML.Setup.Telephone
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.Text -o ${RESAPP}.HTML.Setup.Text
        $(TRANSLATOR) $(TRANSFLAGS) -m TranslateRes:Mappings -i TranslateRes:$(APP).HTML.Setup.Timeout -o ${RESAPP}.HTML.Setup.Timeout
        
resources: translate
	@echo ** Exporting untranslated resource files
        IfThere Resources.${USERIF}.${LOCALE}.${APP} Then \
          ${CP} Resources.${USERIF}.${LOCALE}.${APP} ${RESAPP} ${CPFLAGS} \
        Else \
          ${CP} Resources.${USERIF}.UK.${APP} ${RESAPP} ${CPFLAGS}
        IfThere Resources.${USERIF}.${LOCALE}.NVRAM Then \
          ${CP} Resources.${USERIF}.${LOCALE}.NVRAM.Tags ${RESRES}.NVRAM.Tags ${CPFLAGS} \
        Else \
          ${CP} Resources.${USERIF}.UK.NVRAM.Tags ${RESRES}.NVRAM.Tags ${CPFLAGS}
        IfThere Resources.${USERIF}.${LOCALE}.Wimp Then \
          ${CP} Resources.${USERIF}.${LOCALE}.Wimp.Sprites ${RESRES}.Wimp.Sprites ${CPFLAGS} \
        Else \
          ${CP} Resources.${USERIF}.UK.Wimp.Sprites ${RESRES}.Wimp.Sprites ${CPFLAGS}
        @echo $(COMPONENT): resource files copied
        
# Dynamic dependencies:
