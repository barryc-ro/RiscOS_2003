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
CP              = copy
generate        = generate
RM              = remove
WIPE            = wipe
MODGEN          = modgen
MODSQZ          = modsqz -f
APPEND          = echo
CPFLAGS         = ~cfr~v
WFLAGS          = ~cf~vr
APPFLAGS        = { >> rm.Via }

#
# Program specific options:
#
COMPONENT       = Choices<Locale>

#
# Resources directories:
#
LOCALRES        = LocalUserIFRes:


INVIA           = Resources.General.<Locale>.ViaFile
INDIR           = rm.$(MACHINE)
INMOD           = $(INDIR).$(COMPONENT)

#
# Output files and directories:
#
OUTVIA          = rm.Via
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
        @echo $(COMPONENT): cleaned

local_clean:
        @echo $(COMPONENT): cleaned local resources directory

dirs:
        $(MKDIR) $(INDIR)
        $(MKDIR) $(INSTDIR)

generate: dirs
        @echo Creating the Via file...
        $(CP) $(INVIA) $(OUTVIA) $(CPFLAGS)
        $(APPEND) $(LOCALRES)Options !Boot.HTML.Setup.Options$(APPFLAGS)
        @echo Generating the ResourcesFS archive module...
        @do $(MODGEN) $(INMOD) $(COMPONENT) $(COMPONENT) 1.05 -via $(OUTVIA)
        @echo Squeezing the module...
        $(MODSQZ) $(INMOD)
        @echo Module generation complete.

resources:
        @echo $(COMPONENT): resource files copied

# Dynamic dependencies:
