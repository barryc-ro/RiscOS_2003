# Makefile for ARLib/DBox
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

COMPONENT       = ARLib/DBox
TARGET          = DBoxLib

OBJS            = o.dbox o.dboxes2 o.dboxinc o.dboxquery o.popup o.saveas o.tasksave o.tasksum

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

export_hdrs: h.dbox h.dboxes h.dboxinc h.dboxquery h.popup h.saveas h.tasksum dirs
        $(CP)   h.dbox          $(LIBDIR).ARLib.h.dbox          $(CPFLAGS)
        $(CP)   h.dboxes        $(LIBDIR).ARLib.h.dboxes        $(CPFLAGS)
        $(CP)   h.dboxinc       $(LIBDIR).ARLib.h.dboxinc       $(CPFLAGS)
        $(CP)   h.dboxquery     $(LIBDIR).ARLib.h.dboxquery     $(CPFLAGS)
        $(CP)   h.popup         $(LIBDIR).ARLib.h.popup         $(CPFLAGS)
        $(CP)   h.saveas        $(LIBDIR).ARLib.h.saveas        $(CPFLAGS)
        $(CP)   h.tasksum       $(LIBDIR).ARLib.h.tasksum       $(CPFLAGS)
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
