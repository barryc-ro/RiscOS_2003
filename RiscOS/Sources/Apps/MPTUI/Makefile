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
CP      	= copy
TRANSLATE	= translate
RM      	= remove
WIPE		= -wipe

CPFLAGS 	= ~cfr~v
WFLAGS		= ~cf~vr

# Translate flags
TRANSFLAGS	= ${THROWBACK} -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"

# Translation mappings file
MAPPINGS	= TranslateRes:Mappings

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
EXPORTDIR	= ${RESDIR}.^
RESAPP		= ${EXPORTDIR}.${APP}
RESRES 		= ${EXPORTDIR}.Resources

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
        ${MKDIR} ${EXPORTDIR}
        ${MKDIR} ${RESAPP}
        ${MKDIR} ${RESRES}
        ${MKDIR} ${RESRES}.NVRAM
        ${MKDIR} ${RESRES}.Wimp
        IfThere Resources.${USERIF}.${LOCALE}.${APP} Then \
	  ${CP} Resources.${USERIF}.${LOCALE}.${APP} ${RESAPP} T~V~CFQR
	${CP} Resources.${USERIF}.UK.${APP} ${RESAPP} T~V~CFQR

translate: dirs
        @echo ** Translating NCDialler...
	${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Dialling.Busy -o ${RESAPP}.HTML.Dialling.Busy
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Dialling.Connect -o ${RESAPP}.HTML.Dialling.Connect
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Dialling.Dialling -o ${RESAPP}.HTML.Dialling.Dialling
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Dialling.Error -o ${RESAPP}.HTML.Dialling.Error
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Dialling.NoDial -o ${RESAPP}.HTML.Dialling.NoDial
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Dialling.Server -o ${RESAPP}.HTML.Dialling.Server
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Dialling.Verify -o ${RESAPP}.HTML.Dialling.Verify
        @echo ** Translating ISPCapture...
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.ISPCapture.ISPC1 -o ${RESAPP}.HTML.ISPCapture.ISPC1
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.ISPCapture.ISPC2 -o ${RESAPP}.HTML.ISPCapture.ISPC2
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.ISPCapture.ServerCnfg -o ${RESAPP}.HTML.ISPCapture.ServerCnfg
        @echo ** Translating NCMALite...
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.badpin -o ${RESAPP}.HTML.MA.badpin
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.badubp -o ${RESAPP}.HTML.MA.badubp
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.badurl -o ${RESAPP}.HTML.MA.badurl
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.badurldbox -o ${RESAPP}.HTML.MA.badurldbox
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.connect -o ${RESAPP}.HTML.MA.connect
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.correct -o ${RESAPP}.HTML.MA.correct
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.damaged -o ${RESAPP}.HTML.MA.damaged
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.invalid -o ${RESAPP}.HTML.MA.invalid
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.logout -o ${RESAPP}.HTML.MA.logout
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.mismatch -o ${RESAPP}.HTML.MA.mismatch
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.newpin -o ${RESAPP}.HTML.MA.newpin
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.pin -o ${RESAPP}.HTML.MA.pin
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.pleasewait -o ${RESAPP}.HTML.MA.pleasewait
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.poweroff -o ${RESAPP}.HTML.MA.poweroff
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.SCprobs -o ${RESAPP}.HTML.MA.SCprobs
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.splash -o ${RESAPP}.HTML.MA.splash
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.MA.unblock -o ${RESAPP}.HTML.MA.unblock
        @echo ** Translating NCOptions...
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.Acknow -o ${RESAPP}.HTML.Setup.Acknow
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.BSD -o ${RESAPP}.HTML.Setup.BSD
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.IntroWel -o ${RESAPP}.HTML.Setup.IntroWel
	${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.SetupD.IntroWel -o ${RESAPP}.HTML.SetupD.IntroWel
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.Options -o ${RESAPP}.HTML.Setup.Options
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.Printing -o ${RESAPP}.HTML.Setup.Printing
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.Printing2 -o ${RESAPP}.HTML.Setup.Printing2
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.Sounds -o ${RESAPP}.HTML.Setup.Sounds
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.ssl -o ${RESAPP}.HTML.Setup.ssl
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.StartUp -o ${RESAPP}.HTML.Setup.StartUp
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.Telephone -o ${RESAPP}.HTML.Setup.Telephone
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.Text -o ${RESAPP}.HTML.Setup.Text
        ${TRANSLATE} $(TRANSFLAGS) -m ${MAPPINGS} -i TranslateRes:$(APP).HTML.Setup.Timeout -o ${RESAPP}.HTML.Setup.Timeout
        
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
