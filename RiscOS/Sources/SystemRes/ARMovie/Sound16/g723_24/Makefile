# Makefile for g723_24 audio codec and g723_24 library
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 05 Mar 2001   BJGA            Created
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

COMPONENT       = g723_24 audio codec
LIBCOMPONENT    = g723_24 library
TARGETS         = Play \
                  Playx2 \
                  Decode \
                  Decodex2 \
                  Encode \
                  Encodex2
G723_24LIB      = g723_24
LIBOBJS         = o.g723_24 \
                  o.g72x
PLAYLIB         = C:ARSndLib.o.Snd16Play
PLAYX2LIB       = C:ARSndLib.o.Snd16Playx2
COMPLIB         = C:ARSndLib.o.g723_24

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
        $(RM) $(G723_24LIB)
        @echo $(COMPONENT) and $(LIBCOMPONENT): cleaned

export: export_$(PHASE)

export_hdrs:
        $(MKDIR) <Lib$Dir>.ARSndLib.h
        $(CP) h.g723_24 <Lib$Dir>.ARSndLib.h.g723_24 $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (hdrs)

export_libs: $(G723_24LIB) local_dirs
        $(MKDIR) <Lib$Dir>.ARSndLib.o
        $(CP) $(G723_24LIB) <Lib$Dir>.ARSndLib.o.$(G723_24LIB) $(CPFLAGS)
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

$(G723_24LIB): $(LIBOBJS)
        $(LIBFILE) $(LIBFILEFLAGS) $(LIBOBJS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
