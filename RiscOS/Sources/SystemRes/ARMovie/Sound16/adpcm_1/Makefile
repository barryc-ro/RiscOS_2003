# Makefile for version 1 adpcm audio codec
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 27 Feb 2001   BJGA            Created
#

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
AS              = objasm
CP              = copy
CC              = cc
RM              = remove
LD              = link
WIPE            = -wipe
LIBFILE         = libfile

CFLAGS          = -c -zps1 -ffah -depend !Depend $(THROWBACK) $(INCLUDES)
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = -rmf -o $@
WFLAGS          = ~CFR~V
LIBFILEFLAGS    = -c -o $@

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = adpcm (version 1) audio codec
TARGETS         = Play \
                  Playx2
PLAYLIB         = C:ARSndLib.o.Snd16Play
PLAYX2LIB       = C:ARSndLib.o.Snd16Playx2

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGETS) local_dirs
        @echo $(COMPONENT): all complete

clean:
        $(WIPE) o $(WFLAGS)
        $(RM) Play
        $(RM) Playx2
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o

Play: o.Play
        $(LD) $(LDFLAGS) o.Play $(PLAYLIB)
        SetType $@ Data

Playx2: o.Playx2
        $(LD) $(LDFLAGS) o.Playx2 $(PLAYX2LIB)
        SetType $@ Data

# ------------------------------------------------------------------------------
# Dynamic dependencies:
