# Makefile for ARLib/ARLib
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 14 Nov 2000   BJGA            Created
#

# ------------------------------------------------------------------------------
# Paths
#

LIBDIR          = <Lib$Dir>

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
AS              = objasm
CP              = copy
CC              = cc
RM              = remove
LB              = libfile
WIPE            = -wipe

CFLAGS          = -c -depend !Depend $(THROWBACK) -ffahw -zo -strict $(INCLUDES) $(DFLAGS)
AFLAGS          = -depend !Depend $(THROWBACK) -Stamp -quit
CPFLAGS         = ~CLFNR~V
LBFLAGS         = -c
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:
DFLAGS          = -DNDEBUG

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = ARLib/ARLib
TARGET          = ARLib

OBJS            = o.arhdr o.arhdr1 o.armovie o.arsndcode o.arsnd o.arlibmain

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all:    $(TARGET)
        @echo $(COMPONENT): library built

export: export_$(PHASE)

export_hdrs: h.arline h.armovie h.arsound h.arutils h.replaytype h.utils dirs
        $(CP)   h.arline        $(LIBDIR).ARLib.h.arline        $(CPFLAGS)
        $(CP)   h.armovie       $(LIBDIR).ARLib.h.armovie       $(CPFLAGS)
        $(CP)   h.arsound       $(LIBDIR).ARLib.h.arsound       $(CPFLAGS)
        $(CP)   h.arutils       $(LIBDIR).ARLib.h.arutils       $(CPFLAGS)
        $(CP)   h.replaytype    $(LIBDIR).ARLib.h.replaytype    $(CPFLAGS)
        $(CP)   h.utils         $(LIBDIR).ARLib.h.utils         $(CPFLAGS)
        @echo $(COMPONENT): export complete (hdrs)

export_libs: $(TARGET) dirs
        $(CP)   $(TARGET)       $(LIBDIR).ARLib.o.$(TARGET)     $(CPFLAGS)
        @echo $(COMPONENT): export complete (libs)

clean:
        $(WIPE) o $(WFLAGS)
        $(RM) $(TARGET)
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o

dirs:
        $(MKDIR) $(LIBDIR).ARLib.h
        $(MKDIR) $(LIBDIR).ARLib.o

$(TARGET): $(OBJS) local_dirs
        $(LB) $(LBFLAGS) -o $(TARGET) $(OBJS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.arhdr: c.arhdr C:ARLib.h.msgs C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.armovie C:ARLib.h.memalloc C:ARLib.h.replaytype C:ARLib.h.arline C:ARLib.h.utils C:ARLib.h.arutils
o.arhdr1: c.arhdr1 C:ARLib.h.msgs C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.armovie C:ARLib.h.memalloc C:ARLib.h.replaytype C:ARLib.h.utils
o.armovie: c.armovie C:ARLib.h.alarm C:h.kernel C:ARLib.h.msgs C:ARLib.h.os C:h.swis C:ARLib.h.werr C:ARLib.h.wimp C:ARLib.h.armovie C:ARLib.h.memalloc C:ARLib.h.replaytype C:ARLib.h.arsound C:ARLib.h.arutils C:ARLib.h.utils
o.arsndcode: c.arsndcode C:ARLib.h.alarm C:h.kernel C:ARLib.h.msgs C:ARLib.h.os C:h.swis C:ARLib.h.werr C:ARLib.h.device C:ARLib.h.armovie C:ARLib.h.memalloc C:ARLib.h.replaytype C:ARLib.h.arsound C:ARLib.h.arutils C:ARLib.h.utils
o.arsnd: c.arsnd C:ARLib.h.arutils C:ARLib.h.armovie C:ARLib.h.os C:ARLib.h.memalloc C:ARLib.h.replaytype C:ARLib.h.utils
o.arlibmain: c.arlibmain C:ARLib.h.win C:ARLib.h.wimp C:ARLib.h.armovie C:ARLib.h.os C:ARLib.h.memalloc C:ARLib.h.replaytype C:ARLib.h.arsound C:ARLib.h.utils
