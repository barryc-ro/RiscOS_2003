# Makefile for type-1 16-bit audio codecs and common 16-bit play veneers
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 19 Feb 2001   BJGA            Created
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

COMPONENT       = Type-1 16-bit audio codecs
LIBCOMPONENT    = Common 16-bit play veneers
TARGETS         = SoundE8 \
                  SoundE8x2 \
                  SoundS8 \
                  SoundS8x2 \
                  SoundS16 \
                  SoundS16x2 \
                  SoundU8 \
                  SoundU8x2 \
                  SoundU16 \
                  SoundU16x2
MONOLIB         = Snd16Play
STEREOLIB       = Snd16Playx2

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
        $(RM) Snd16Play
        $(RM) Snd16Playx2
        @echo $(COMPONENT) and $(LIBCOMPONENT): cleaned

export: export_$(PHASE)

export_hdrs:
        @echo $(LIBCOMPONENT): export complete (hdrs)

export_libs: $(STEREOLIB) $(MONOLIB) local_dirs
        $(MKDIR) <Lib$Dir>.ARSndLib.o
        $(CP) $(MONOLIB)   <Lib$Dir>.ARSndLib.o.$(MONOLIB)   $(CPFLAGS)
        $(CP) $(STEREOLIB) <Lib$Dir>.ARSndLib.o.$(STEREOLIB) $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (libs)

local_dirs:
        $(MKDIR) o

SoundE8: o.SoundE8
        $(LD) $(LDFLAGS) o.SoundE8
        SetType $@ Data

SoundE8x2: o.SoundE8x2
        $(LD) $(LDFLAGS) o.SoundE8x2
        SetType $@ Data

SoundS8: o.SoundS8
        $(LD) $(LDFLAGS) o.SoundS8
        SetType $@ Data

SoundS8x2: o.SoundS8x2
        $(LD) $(LDFLAGS) o.SoundS8x2
        SetType $@ Data

SoundU8: o.SoundU8
        $(LD) $(LDFLAGS) o.SoundU8
        SetType $@ Data

SoundU8x2: o.SoundU8x2
        $(LD) $(LDFLAGS) o.SoundU8x2
        SetType $@ Data

SoundS16: o.SoundS16
        $(LD) $(LDFLAGS) o.SoundS16
        SetType $@ Data

SoundS16x2: o.SoundS16x2
        $(LD) $(LDFLAGS) o.SoundS16x2
        SetType $@ Data

SoundU16: o.SoundU16
        $(LD) $(LDFLAGS) o.SoundU16
        SetType $@ Data

SoundU16x2: o.SoundU16x2
        $(LD) $(LDFLAGS) o.SoundU16x2
        SetType $@ Data

$(MONOLIB): o.Snd16Play
        $(LIBFILE) $(LIBFILEFLAGS) o.Snd16Play

$(STEREOLIB): o.Snd16Playx2
        $(LIBFILE) $(LIBFILEFLAGS) o.Snd16Playx2

# ------------------------------------------------------------------------------
# Dynamic dependencies:
