# Makefile for ARLib/Replay
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

COMPONENT       = ARLib/Replay
TARGET          = ReplayLib

OBJS            = o.arvid o.arinfo o.arinfo2 o.replayhdr

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

export_hdrs: h.arinfo h.arvid h.replayhdr h.replaymsgs dirs
        $(CP)   h.arinfo        $(LIBDIR).ARLib.h.arinfo        $(CPFLAGS)
        $(CP)   h.arvid         $(LIBDIR).ARLib.h.arvid         $(CPFLAGS)
        $(CP)   h.replayhdr     $(LIBDIR).ARLib.h.replayhdr     $(CPFLAGS)
        $(CP)   h.replaymsgs    $(LIBDIR).ARLib.h.replaymsgs    $(CPFLAGS)
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
o.arvid: c.arvid C:ARLib.h.macros C:ARLib.h.arvid C:ARLib.h.armovie C:ARLib.h.os C:ARLib.h.memalloc C:ARLib.h.replaytype
o.arinfo: c.arinfo C:ARLib.h.msgs C:ARLib.h.file C:ARLib.h.os C:ARLib.h.macros C:ARLib.h.strfn C:ARLib.h.arinfo C:ARLib.h.replaytype
o.arinfo2: c.arinfo2 C:ARLib.h.msgs C:ARLib.h.array C:ARLib.h.memalloc C:ARLib.h.file C:ARLib.h.os C:ARLib.h.filetypes C:ARLib.h.macros C:ARLib.h.strfn C:ARLib.h.array C:ARLib.h.arinfo C:ARLib.h.replaytype
o.replayhdr: c.replayhdr C:ARLib.h.msgs C:ARLib.h.spr C:ARLib.h.memalloc C:ARLib.h.os C:ARLib.h.werr C:ARLib.h.file C:ARLib.h.strfn C:ARLib.h.arvid C:ARLib.h.armovie C:ARLib.h.replaytype C:ARLib.h.replayhdr C:ARLib.h.replaymsgs
