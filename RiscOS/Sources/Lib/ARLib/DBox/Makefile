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
o.dbox: c.dbox C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.os C:ARLib.h.akbd C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.wimpt C:ARLib.h.win C:ARLib.h.menu C:ARLib.h.event C:ARLib.h.dbox C:ARLib.h.res C:ARLib.h.template C:ARLib.h.alarm C:ARLib.h.msgs C:ARLib.h.coords C:ARLib.h.help C:h.swis C:h.kernel C:ARLib.h.icons C:ARLib.h.wmisc
o.dboxes2: c.dboxes2 C:ARLib.h.dbox C:ARLib.h.template C:ARLib.h.wimp C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.dboxes C:ARLib.h.strfn C:ARLib.h.wmisc C:ARLib.h.win C:ARLib.h.icons
o.dboxinc: c.dboxinc C:ARLib.h.akbd C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.dboxinc C:ARLib.h.dbox
o.dboxquery: c.dboxquery C:ARLib.h.dbox C:ARLib.h.msgs C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.os C:ARLib.h.dboxquery
o.popup: c.popup C:ARLib.h.dbox C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.wimp C:ARLib.h.menu C:ARLib.h.saveas C:ARLib.h.xfersend C:ARLib.h.template C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimp C:ARLib.h.wimpx C:ARLib.h.wimpt C:ARLib.h.win C:ARLib.h.macros C:ARLib.h.screenmode C:ARLib.h.popup
o.saveas: c.saveas C:ARLib.h.dboxes C:ARLib.h.dbox C:ARLib.h.wimp C:ARLib.h.fileicon C:ARLib.h.help C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.macros C:ARLib.h.msgs C:ARLib.h.xfersend C:ARLib.h.visdelay C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.strfn C:ARLib.h.saveas
o.tasksave: c.tasksave C:ARLib.h.msgs C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.file C:ARLib.h.filetypes C:ARLib.h.macros C:ARLib.h.popup C:ARLib.h.dbox C:ARLib.h.xfersend C:ARLib.h.wimp C:ARLib.h.strfn C:ARLib.h.tasksum C:ARLib.h.taskwindow C:ARLib.h.memalloc
o.tasksum: c.tasksum C:ARLib.h.dbox C:ARLib.h.dboxquery C:ARLib.h.font C:ARLib.h.os C:ARLib.h.drawmod C:ARLib.h.msgs C:h.swis C:h.kernel C:ARLib.h.template C:ARLib.h.wimp C:ARLib.h.werr C:ARLib.h.wimpx C:ARLib.h.win C:ARLib.h.macros C:ARLib.h.tasksum C:ARLib.h.taskwindow C:ARLib.h.memalloc C:ARLib.h.wmisc
