# Makefile for ARLib/Sys
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

COMPONENT       = ARLib/Sys
TARGET          = SysLib

OBJS            = o.akbd o.msgs o.os o.res o.screenmode o.strfn o.sys o.werr o.bbcasm o.visdelasm

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

export_hdrs: h.akbd h.bbc h.filetypes h.macros h.msgs h.msgtrans h.os h.osreasons h.res h.screenmode h.strfn h.sys h.visdelay h.werr h.wimp dirs
        $(CP)   h.akbd          $(LIBDIR).ARLib.h.akbd          $(CPFLAGS)
        $(CP)   h.bbc           $(LIBDIR).ARLib.h.bbc           $(CPFLAGS)
        $(CP)   h.filetypes     $(LIBDIR).ARLib.h.filetypes     $(CPFLAGS)
        $(CP)   h.macros        $(LIBDIR).ARLib.h.macros        $(CPFLAGS)
        $(CP)   h.msgs          $(LIBDIR).ARLib.h.msgs          $(CPFLAGS)
        $(CP)   h.msgtrans      $(LIBDIR).ARLib.h.msgtrans      $(CPFLAGS)
        $(CP)   h.os            $(LIBDIR).ARLib.h.os            $(CPFLAGS)
        $(CP)   h.osreasons     $(LIBDIR).ARLib.h.osreasons     $(CPFLAGS)
        $(CP)   h.res           $(LIBDIR).ARLib.h.res           $(CPFLAGS)
        $(CP)   h.screenmode    $(LIBDIR).ARLib.h.screenmode    $(CPFLAGS)
        $(CP)   h.strfn         $(LIBDIR).ARLib.h.strfn         $(CPFLAGS)
        $(CP)   h.sys           $(LIBDIR).ARLib.h.sys           $(CPFLAGS)
        $(CP)   h.visdelay      $(LIBDIR).ARLib.h.visdelay      $(CPFLAGS)
        $(CP)   h.werr          $(LIBDIR).ARLib.h.werr          $(CPFLAGS)
        $(CP)   h.wimp          $(LIBDIR).ARLib.h.wimp          $(CPFLAGS)
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
