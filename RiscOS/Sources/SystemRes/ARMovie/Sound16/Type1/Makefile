# Makefile for type-1 16-bit audio codecs
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 13 Dec 2000   BJGA            Created
#

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
CP              = copy
RM              = remove
WIPE            = -wipe

CPFLAGS         = ~CFR~V
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = Type-1 16-bit audio codecs

# ------------------------------------------------------------------------------
# Build rules
#

all: Drivers.Fill8UM
        $(MKDIR) adpcm
        /bas.MakeSnd16
        @echo $(COMPONENT): all complete

clean:
        $(WIPE) Drivers $(WFLAGS)
        $(WIPE) adpcm $(WFLAGS)
        $(RM) SoundE8
        $(RM) SoundE8x2
        $(RM) SoundS8
        $(RM) SoundS8x2
        $(RM) SoundU8
        $(RM) SoundU8x2
        $(RM) SoundS16
        $(RM) SoundS16x2
        $(RM) SoundU16
        $(RM) SoundU16x2
        @echo $(COMPONENT): cleaned

# Only one driver code chunk listed to avoid repeated execution
Drivers.Fill8UM:
        $(MKDIR) Drivers
        /bas.FillCode

# ------------------------------------------------------------------------------
# Dynamic dependencies:
