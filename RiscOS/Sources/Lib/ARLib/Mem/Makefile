# Makefile for ARLib/Mem
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

COMPONENT       = ARLib/Mem
TARGET          = MemLib

OBJS            = o.c_alloc o.d_alloc o.m_alloc o.f_alloc o.h_alloc o.flex o.heap

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

export_hdrs: h.flex h.flex_int h.heap h.memalloc h.mem_int dirs
        $(CP)   h.flex          $(LIBDIR).ARLib.h.flex          $(CPFLAGS)
        $(CP)   h.flex_int      $(LIBDIR).ARLib.h.flex_int      $(CPFLAGS)
        $(CP)   h.heap          $(LIBDIR).ARLib.h.heap          $(CPFLAGS)
        $(CP)   h.memalloc      $(LIBDIR).ARLib.h.memalloc      $(CPFLAGS)
        $(CP)   h.mem_int       $(LIBDIR).ARLib.h.mem_int       $(CPFLAGS)
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
o.c_alloc: c.c_alloc C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.memalloc C:ARLib.h.mem_int
o.f_alloc: c.f_alloc C:ARLib.h.flex C:ARLib.h.memalloc C:ARLib.h.mem_int
o.d_alloc: c.d_alloc C:ARLib.h.os C:h.kernel C:h.swis C:ARLib.h.werr C:ARLib.h.wimpt C:ARLib.h.wimp C:ARLib.h.memalloc C:ARLib.h.mem_int
o.m_alloc: c.m_alloc C:h.kernel C:ARLib.h.os C:h.swis C:ARLib.h.werr C:ARLib.h.memalloc C:ARLib.h.mem_int
o.h_alloc: c.h_alloc C:ARLib.h.heap C:ARLib.h.os C:ARLib.h.memalloc C:ARLib.h.mem_int
o.flex: c.flex C:h.kernel C:h.swis C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.flex C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.msgs C:ARLib.h.memalloc C:ARLib.h.mem_int C:ARLib.h.flex_int
o.heap: c.heap C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.heap C:ARLib.h.os C:ARLib.h.flex C:ARLib.h.msgs C:h.kernel C:ARLib.h.mem_int
