# Makefile for ARLib/Spr
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

COMPONENT       = ARLib/Spr
TARGET          = SprLib

OBJS            = o.spr o.sprasm o.spr_disp o.res_spr o.spr_icons

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

export_hdrs: h.res_spr h.spr h.spr_disp h.spr_intern dirs
        $(CP)   h.res_spr       $(LIBDIR).ARLib.h.res_spr       $(CPFLAGS)
        $(CP)   h.spr           $(LIBDIR).ARLib.h.spr           $(CPFLAGS)
        $(CP)   h.spr_disp      $(LIBDIR).ARLib.h.spr_disp      $(CPFLAGS)
        $(CP)   h.spr_intern    $(LIBDIR).ARLib.h.spr_intern    $(CPFLAGS)
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
