# Makefile for AREncode
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 17 Nov 2000   BJGA            Created
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
AFLAGS          = -depend !Depend $(THROWBACK) -Stamp -quit
CPFLAGS         = ~CFR~V
LDFLAGS         = 
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:
DFLAGS          = -DNDEBUG

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = AREncode
FILES           = Resources.!Boot \
                  LocalRes:!Help \
                  LocalRes:!Run \
                  !RunImage \
                  Resources.!Sprites \
                  LocalRes:messages \
                  LocalRes:2D.Templates \
                  LocalRes:3D.Templates

LIBS            = C:ARLib.o.ReplayLib C:ARLib.o.ARLib \
                  C:ARLib.o.DBoxLib C:ARLib.o.WimpLib \
                  C:ARLib.o.SprLib C:ARLib.o.FileLib \
                  C:ARLib.o.MemLib C:ARLib.o.SysLib \
                  C:ARLib.o.StrucLib C:o.stubs
OBJS            = o.main o.compress o.cvtwin o.hdrwin o.calldiffer o.prefs o.tracks o.filters

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(FILES)
        @echo $(COMPONENT): all complete

install: $(FILES)
        $(MKDIR) $(INSTDIR).!$(COMPONENT)
        |
        $(CP) Resources.!Boot       $(INSTDIR).!$(COMPONENT).!Boot      $(CPFLAGS)
        $(CP) LocalRes:!Help        $(INSTDIR).!$(COMPONENT).!Help      $(CPFLAGS)
        $(CP) LocalRes:!Run         $(INSTDIR).!$(COMPONENT).!Run       $(CPFLAGS)
        $(CP) !RunImage             $(INSTDIR).!$(COMPONENT).!RunImage  $(CPFLAGS)
        $(CP) Resources.!Sprites    $(INSTDIR).!$(COMPONENT).!Sprites   $(CPFLAGS)
        $(CP) LocalRes:messages     $(INSTDIR).!$(COMPONENT).messages   $(CPFLAGS)
        $(CP) LocalRes:2D.Templates $(INSTDIR).!$(COMPONENT).Template2D $(CPFLAGS)
        $(CP) LocalRes:3D.Templates $(INSTDIR).!$(COMPONENT).Templates  $(CPFLAGS)
        |
        @echo $(COMPONENT): installed

clean:
        $(WIPE) o $(WFLAGS)
        $(RM) !RunImage
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o

!RunImage: $(OBJS) local_dirs
        $(LD) -aif $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
