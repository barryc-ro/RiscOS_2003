# Makefile for Join
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 05 Dec 2000   BJGA            Created
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

CFLAGS          = -c -depend !Depend $(THROWBACK) -ffah $(INCLUDES) $(DFLAGS)
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = 
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IOSLibInclude:,C:
DFLAGS          = 

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = Join
TARGET          = Join
LIBS            = OSLib:o.OSLib C:o.stubs
OBJS            = o.join

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
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
        $(LD) -aif $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.join: c.join C:h.swis C:h.kernel C:h.kernel h.join h.my_adpcm
