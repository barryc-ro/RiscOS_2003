# Makefile for ARLib/File
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

COMPONENT       = ARLib/File
TARGET          = FileLib

OBJS            = o.file o.file2 o.filecanon o.filescan o.fileasm o.opt o.filer o.fileract

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

export_hdrs: h.file h.fileract h.filereason h.opt dirs
        $(CP)   h.file          $(LIBDIR).ARLib.h.file          $(CPFLAGS)
        $(CP)   h.fileract      $(LIBDIR).ARLib.h.fileract      $(CPFLAGS)
        $(CP)   h.filereason    $(LIBDIR).ARLib.h.filereason    $(CPFLAGS)
        $(CP)   h.opt           $(LIBDIR).ARLib.h.opt           $(CPFLAGS)
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
o.file: c.file C:ARLib.h.os C:h.swis C:h.kernel C:ARLib.h.filereason C:ARLib.h.file
o.file2: c.file2 C:ARLib.h.os C:h.swis C:h.kernel C:ARLib.h.file C:ARLib.h.filereason
o.filecanon: c.filecanon C:h.swis C:h.kernel C:ARLib.h.file C:ARLib.h.os C:ARLib.h.filereason
o.filescan: c.filescan C:h.swis C:h.kernel C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.file C:ARLib.h.filereason C:ARLib.h.filetypes C:ARLib.h.strfn C:ARLib.h.sys
o.fileasm: s.fileasm Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS ^.Hdr.CRegs Hdr:Proc ^.Hdr.Macros2 Hdr:HighFSI
o.opt: c.opt C:ARLib.h.msgs C:ARLib.h.visdelay C:ARLib.h.macros C:ARLib.h.opt C:ARLib.h.os
o.filer: c.filer C:ARLib.h.file C:ARLib.h.os C:ARLib.h.strfn C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.fileract
o.fileract: c.fileract C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.file C:ARLib.h.osreasons C:ARLib.h.strfn C:ARLib.h.fileract
