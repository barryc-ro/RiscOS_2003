# Makefile for STBChoices
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Aug-99  SNB          Created from OmegaNC2UI
# 23-May-00  SAR          Modified to generate ResourceFS archive
#                         (module) of STB options pages
#

#
# Generic options:
#
MKDIR           = mkdir -p
PL              = perl
CP              = copy
generate        = generate
RM              = remove
WIPE            = wipe
MODGEN          = modgen
MODSQZ          = modsqz -f
APPEND          = echo
CHMOD           = attr $(CHMODFLAGS)
CPFLAGS         = ~cfr~v
WFLAGS          = ~cf~vr
CHMODFLAGS      = +ow +or +wr -ww
APPFLAGS        = { >> rm.Via }

#
# Translate phase:
#
TRANSLATE       = translate
TRANSFLAGS      = $(THROWBACK) -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"
INTRANDIR       = Translate.$(UserIF).$(Locale)
OUTTRANDIR      = Translated
TRANSMAPPINGS   = $(INTRANDIR).Mappings

#
# Program specific options:
#
COMPONENT       = Choices$(Locale)
VERSIONVAR      = STBChoices$Version
VERSION         = <STBChoices$Version>
PLSCRIPT        = Build:GetVersion

#
# Resources directories:
#
LOCALRES        = LocalUserIFRes:


VIA             = Resources.$(UserIF).$(Locale).ViaFile
INDIR           = rm.$(MACHINE)
INMOD           = $(INDIR).$(COMPONENT)

#
# Output files and directories:
#
OUTMOD          = $(INSTDIR).$(COMPONENT)

#
# Generic rules:
#
rom: $(TARGET)
        @echo $(COMPONENT): ROM module built

ram: generate dirs
        @echo $(COMPONENT): RAM module built

export: $(EXPORTS)
        @echo $(COMPONENT): export complete

install_rom: $(TARGET)
        @echo $(COMPONENT): ROM module installed

install: generate dirs
        @echo Building Choices dir...
        $(CP) $(INMOD) $(OUTMOD) $(CPFLAGS)
        @echo $(COMPONENT): RAM module installed

clean:
        ifthere rm then $(WIPE) rm $(WFLAGS)
        ifthere $(OUTTRANDIR) then $(WIPE) $(OUTTRANDIR) $(WFLAGS)
        @echo $(COMPONENT): cleaned

local_clean:
        @echo $(COMPONENT): cleaned local resources directory

dirs:
        $(MKDIR) $(INDIR)
        $(MKDIR) $(INSTDIR)
        $(MKDIR) $(OUTTRANDIR)

generate: translate versionvar dirs
        @echo Generating the ResourcesFS archive module...
        @do $(MODGEN) $(INMOD) $(COMPONENT) $(COMPONENT) $(VERSION) -via $(VIA)
        @echo Squeezing the module...
        $(MODSQZ) $(INMOD)
 $(CHMOD) $(INMOD)
        @echo Module generation complete.

resources:
        @echo $(COMPONENT): resource files copied

translate: dirs
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).Acknow     -o $(OUTTRANDIR).Acknow
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).BSD        -o $(OUTTRANDIR).BSD
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).ImageLib   -o $(OUTTRANDIR).ImageLib
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).Options    -o $(OUTTRANDIR).Options
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).Printing   -o $(OUTTRANDIR).Printing
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).Printing2  -o $(OUTTRANDIR).Printing2
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).Software   -o $(OUTTRANDIR).Software
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).Sounds     -o $(OUTTRANDIR).Sounds
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).SSL        -o $(OUTTRANDIR).SSL
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).StartUp    -o $(OUTTRANDIR).StartUp
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).Television -o $(OUTTRANDIR).Television
        $(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(INTRANDIR).Text       -o $(OUTTRANDIR).Text

versionvar:
        ${PL} ${PLSCRIPT} ${VERSIONVAR}

# Dynamic dependencies:
