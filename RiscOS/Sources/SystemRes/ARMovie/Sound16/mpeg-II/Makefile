# Makefile for mpeg-II audio codec
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 01 Mar 2001   BJGA            Created
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

COMPONENT       = mpeg layer II audio codec
TARGETS         = Play \
                  Playx2 \
                  Decode \
                  Decodex2 \
                  Encode \
                  Encodex2
PLAYLIB         = C:ARSndLib.o.Snd16Play
PLAYX2LIB       = C:ARSndLib.o.Snd16Playx2
COMPLIB_DEC     = C:ARSndLib.o.mpadec
COMPLIB_ENC     = C:ARSndLib.o.mpaenc

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
        $(RM) Decode
        $(RM) Decodex2
        $(RM) Encode
        $(RM) Encodex2
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o

Play: o.Play
        $(LD) $(LDFLAGS) o.Play $(COMPLIB_DEC) $(PLAYLIB)
        SetType $@ Data

Playx2: o.Playx2
        $(LD) $(LDFLAGS) o.Playx2 $(COMPLIB_DEC) $(PLAYX2LIB)
        SetType $@ Data

Decode: o.DecdHdr o.Decode
        $(LD) $(LDFLAGS) o.DecdHdr o.Decode $(COMPLIB_DEC)
        SetType $@ Data

Decodex2: o.DecdHdr o.Decodex2
        $(LD) $(LDFLAGS) o.DecdHdr o.Decodex2 $(COMPLIB_DEC)
        SetType $@ Data

Encode: o.EncdHdr o.Encode
        $(LD) $(LDFLAGS) o.EncdHdr o.Encode $(COMPLIB_ENC)
        SetType $@ Data

Encodex2: o.EncdHdr o.Encodex2
        $(LD) $(LDFLAGS) o.EncdHdr o.Encodex2 $(COMPLIB_ENC)
        SetType $@ Data

# ------------------------------------------------------------------------------
# Dynamic dependencies:
