# Makefile for version 2 adpcm audio codec and adpcm library
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

COMPONENT       = adpcm (version 2) audio codec
LIBCOMPONENT    = adpcm library
TARGETS         = Play \
                  Playx2 \
                  Decode \
                  Decodex2 \
                  Encode \
                  Encodex2
ADPCMLIB        = adpcm
LIBOBJS         = o.adpcm
PLAYLIB         = C:ARSndLib.o.Snd16Play
PLAYX2LIB       = C:ARSndLib.o.Snd16Playx2
COMPLIB         = C:ARSndLib.o.adpcm

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
        $(RM) $(ADPCMLIB)
        @echo $(COMPONENT) and $(LIBCOMPONENT): cleaned

export: export_$(PHASE)

export_hdrs:
        $(MKDIR) <Lib$Dir>.ARSndLib.h
        $(CP) h.adpcm <Lib$Dir>.ARSndLib.h.adpcm $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (hdrs)

export_libs: $(ADPCMLIB) local_dirs
        $(MKDIR) <Lib$Dir>.ARSndLib.o
        $(CP) $(ADPCMLIB) <Lib$Dir>.ARSndLib.o.$(ADPCMLIB) $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (libs)

local_dirs:
        $(MKDIR) o

Play: o.Play
        $(LD) $(LDFLAGS) o.Play $(COMPLIB) $(PLAYLIB)
        SetType $@ Data

Playx2: o.Playx2
        $(LD) $(LDFLAGS) o.Playx2 $(COMPLIB) $(PLAYX2LIB)
        SetType $@ Data

Decode: o.DecdHdr o.Decode
        $(LD) $(LDFLAGS) o.DecdHdr o.Decode $(COMPLIB)
        SetType $@ Data

Decodex2: o.DecdHdr o.Decodex2
        $(LD) $(LDFLAGS) o.DecdHdr o.Decodex2 $(COMPLIB)
        SetType $@ Data

Encode: o.EncdHdr o.Encode
        $(LD) $(LDFLAGS) o.EncdHdr o.Encode $(COMPLIB)
        SetType $@ Data

Encodex2: o.EncdHdr o.Encodex2
        $(LD) $(LDFLAGS) o.EncdHdr o.Encodex2 $(COMPLIB)
        SetType $@ Data

$(ADPCMLIB): $(LIBOBJS)
        $(LIBFILE) $(LIBFILEFLAGS) $(LIBOBJS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
