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
generate       = generate
RM              = remove
WIPE            = -wipe
MODGEN          = modgen
MODSQZ          = modsqz -f
APPEND          = echo
CPFLAGS         = ~cfr~v
WFLAGS          = ~cf~vr
APPFLAGS        = { >> rm.Via }

#
# Program specific options:
#
COMPONENT       = NCOptRes

#
# Resources directories:
#
LOCALRES        = LocalUserIFRes:


INVIA           = Resources.General.UK.ViaFile
INMOD           = rm.$(COMPONENT)

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

export: $(EXPORTS)
        @echo $(COMPONENT): export complete

install_rom: $(TARGET)
        @echo $(COMPONENT): ROM module installed

install: generate dirs
        @echo Building Choices dir...
        $(CP) $(INMOD) $(OUTMOD) $(CPFLAGS)
        @echo $(COMPONENT): RAM module installed

clean:
        $(WIPE) rm $(WFLAGS)
        @echo $(COMPONENT): cleaned

local_clean:
        @echo $(COMPONENT): cleaned local resources directory

dirs:
        $(MKDIR) rm
        $(MKDIR) $(INSTDIR)

generate: dirs
        @echo Creating the Via file...
        $(CP) $(INVIA) $(OUTVIA) $(CPFLAGS)
        $(APPEND) $(LOCALRES)Options Resources.NCOptions.Options$(APPFLAGS)
        @echo Generating the ResourcesFS archive module...
        $(MODGEN) $(INMOD) $(COMPONENT) $(COMPONENT) 1.00 -via $(OUTVIA)
        @echo Squeezing the module...
        $(MODSQZ) $(INMOD)
        @echo Module generation complete.

resources:
        @echo $(COMPONENT): resource files copied

# Dynamic dependencies:
