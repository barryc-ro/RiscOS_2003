# Makefile for ARLib/Wimp
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

COMPONENT       = ARLib/Wimp
TARGET          = WimpLib

OBJS            = o.alarm o.baricon o.clipboard o.coords o.device o.event o.fileicon o.help \
                  o.iconmenu o.icons o.menu o.pane o.redraw o.template o.wimp o.wimpt \
                  o.win o.wintitle o.wmisc o.wmiscclose o.wmiscflush o.wmiscshift o.xferrecv \
                  o.xfersend o.poll o.wimpasm o.msgclaim o.plotsprite o.taskwindow o.wmiscquit

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

export_hdrs: h.alarm h.baricon h.clipboard h.coords h.device h.event h.fileicon h.help \
             h.iconmenu h.icons h.menu h.msgclaim h.pane h.plotsprite h.redraw h.taskman \
             h.taskwindow h.template h.wimpt h.wimpx h.win h.wmisc h.xferrecv h.xfersend dirs
        $(CP)   h.alarm         $(LIBDIR).ARLib.h.alarm         $(CPFLAGS)
        $(CP)   h.baricon       $(LIBDIR).ARLib.h.baricon       $(CPFLAGS)
        $(CP)   h.clipboard     $(LIBDIR).ARLib.h.clipboard     $(CPFLAGS)
        $(CP)   h.coords        $(LIBDIR).ARLib.h.coords        $(CPFLAGS)
        $(CP)   h.device        $(LIBDIR).ARLib.h.device        $(CPFLAGS)
        $(CP)   h.event         $(LIBDIR).ARLib.h.event         $(CPFLAGS)
        $(CP)   h.fileicon      $(LIBDIR).ARLib.h.fileicon      $(CPFLAGS)
        $(CP)   h.help          $(LIBDIR).ARLib.h.help          $(CPFLAGS)
        $(CP)   h.iconmenu      $(LIBDIR).ARLib.h.iconmenu      $(CPFLAGS)
        $(CP)   h.icons         $(LIBDIR).ARLib.h.icons         $(CPFLAGS)
        $(CP)   h.menu          $(LIBDIR).ARLib.h.menu          $(CPFLAGS)
        $(CP)   h.msgclaim      $(LIBDIR).ARLib.h.msgclaim      $(CPFLAGS)
        $(CP)   h.pane          $(LIBDIR).ARLib.h.pane          $(CPFLAGS)
        $(CP)   h.plotsprite    $(LIBDIR).ARLib.h.plotsprite    $(CPFLAGS)
        $(CP)   h.redraw        $(LIBDIR).ARLib.h.redraw        $(CPFLAGS)
        $(CP)   h.taskman       $(LIBDIR).ARLib.h.taskman       $(CPFLAGS)
        $(CP)   h.taskwindow    $(LIBDIR).ARLib.h.taskwindow    $(CPFLAGS)
        $(CP)   h.template      $(LIBDIR).ARLib.h.template      $(CPFLAGS)
        $(CP)   h.wimpt         $(LIBDIR).ARLib.h.wimpt         $(CPFLAGS)
        $(CP)   h.wimpx         $(LIBDIR).ARLib.h.wimpx         $(CPFLAGS)
        $(CP)   h.win           $(LIBDIR).ARLib.h.win           $(CPFLAGS)
        $(CP)   h.wmisc         $(LIBDIR).ARLib.h.wmisc         $(CPFLAGS)
        $(CP)   h.xferrecv      $(LIBDIR).ARLib.h.xferrecv      $(CPFLAGS)
        $(CP)   h.xfersend      $(LIBDIR).ARLib.h.xfersend      $(CPFLAGS)
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
