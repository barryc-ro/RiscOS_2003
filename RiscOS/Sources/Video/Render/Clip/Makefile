# Makefile for Clip
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 29 Nov 2000   BJGA            Created
#

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
AS              = objasm
CP              = copy
RM              = remove
LD              = link
WIPE            = -wipe

AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = 
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = Clip
TARGET          = rm.Clip
OBJS            = o.modhdr o.filter o.errors o.msgstuff o.GenSWIs o.RegionMan o.debug

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): all complete

rom: $(TARGET)
        @echo $(COMPONENT): rom module built

resources:
        $(MKDIR) $(RESDIR).$(COMPONENT)
        $(CP) LocalRes:Messages $(RESDIR).$(COMPONENT).Messages $(CPFLAGS)
        @echo $(COMPONENT): resource files copied

install: $(TARGET)
        $(MKDIR) $(INSTDIR)
        $(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        $(CP) LocalRes:Messages $(INSTDIR).Messages $(CPFLAGS)
        @echo $(COMPONENT): module installed

install_rom: $(TARGET)
        $(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        @echo $(COMPONENT): rom module installed

clean:
        $(WIPE) o $(WFLAGS)
        $(WIPE) rm $(WFLAGS)
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o
        $(MKDIR) rm

$(TARGET): $(OBJS) local_dirs
        $(LD) -bin $(LDFLAGS) -o $@ $(OBJS)
        SetType $@ Module

# ------------------------------------------------------------------------------
# Dynamic dependencies:
