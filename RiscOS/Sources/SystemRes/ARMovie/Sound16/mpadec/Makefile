# Makefile for mpadec library
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 28 Feb 2001   BJGA            Created
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
LIBFILE         = libfile

CFLAGS          = -c -zps1 -ffah -depend !Depend $(THROWBACK) $(INCLUDES)
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = -rmf -o $@
WFLAGS          = ~CFR~V
LIBFILEFLAGS    = -c -o $@

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:

# ------------------------------------------------------------------------------
# Program specific options:
#

LIBCOMPONENT    = mpadec library
MPADECLIB       = mpadec
LIBOBJS         = o.mpadec \
                  o.decode \
                  o.frame \
                  o.bits \
                  o.tables \
                  o.generate \
                  o.asmgenerate \
                  o.filter \
                  o.asmfilter \
                  o._bodge_c \
                  o._bodge_s \
                  o._memcpy

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all:
        @echo $(LIBCOMPONENT): all complete

clean:
        $(WIPE) o $(WFLAGS)
        $(RM) $(MPADECLIB)
        @echo $(LIBCOMPONENT): cleaned

export: export_$(PHASE)

export_hdrs:
        $(MKDIR) <Lib$Dir>.ARSndLib.h
        $(CP) h.mpadec <Lib$Dir>.ARSndLib.h.mpadec $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (hdrs)

export_libs: $(MPADECLIB) local_dirs
        $(MKDIR) <Lib$Dir>.ARSndLib.o
        $(CP) $(MPADECLIB) <Lib$Dir>.ARSndLib.o.$(MPADECLIB) $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (libs)

local_dirs:
        $(MKDIR) o

$(MPADECLIB): $(LIBOBJS)
        $(LIBFILE) $(LIBFILEFLAGS) $(LIBOBJS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
