# Makefile for Join v4
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 06 Mar 2001   BJGA            Created
#

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
AS              = objasm
CC              = cc
C++             = c++
CP              = copy
LD              = link
RM              = remove
WIPE            = -wipe

AFLAGS          = -depend !Depend $(THROWBACK)
CFLAGS          = -c -depend !Depend $(THROWBACK) -ffah $(INCLUDES)
C++FLAGS        = -c -depend !Depend $(THROWBACK) -ffa $(INCLUDES) -wp
CPFLAGS         = ~CFR~V
LDFLAGS         = -aif -c++ -o $@
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = Join
TARGET          = Join
LIBS            = C:o.stubs \
                  CPP:o.c++lib
OBJS            = o.join \
                  o.ProgInfo \
                  o.InPars \
                  o.ARChoices \
                  o.HelpSprite \
                  o.ARHeader \
                  o.Images \
                  o.Keys \
                  o.Sounds \
                  o.ARMovie \
                  o.Chunks \
                  o.SoundTrack \
                  o.ST_Type0 \
                  o.ST_Type1 \
                  o.ST_Type2 \
                  o.mc_veneer \
                  o.ARSoundRdr \
                  o.SoundLdr

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .c .c++ .s
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.c++.o:;   $(C++) $(C++FLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): all complete

clean:
        $(WIPE) o $(WFLAGS)
        $(RM) $(TARGET)
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o

$(TARGET): $(OBJS) local_dirs
        $(LD) $(LDFLAGS) $(OBJS) $(LIBS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
