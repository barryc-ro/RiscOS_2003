# Makefile for mpaenc library
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

LIBCOMPONENT    = mpaenc library
MPAENCLIB       = mpaenc
LIBOBJS         = o.common \
                  o.encode \
                  o.musicin \
                  o.psy \
                  o.subs \
                  o.tonal \
                  o.tables \
                  o.tables2 \
                  o.tables3a \
                  o.tables3b \
                  o.tables3c \
                  o.tables4 \
                  o.tables5 \
                  o.constants \
                  o.fp \
                  o.debug \
                  o._math \
                  o._memcpy \
                  o._bodge_c \
                  o._div_s \
                  o._trigstuff

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
        $(RM) $(MPAENCLIB)
        @echo $(LIBCOMPONENT): cleaned

export: export_$(PHASE)

export_hdrs:
        $(MKDIR) <Lib$Dir>.ARSndLib.h
        $(CP) h.mpaenc <Lib$Dir>.ARSndLib.h.mpaenc $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (hdrs)

export_libs: $(MPAENCLIB) local_dirs
        $(MKDIR) <Lib$Dir>.ARSndLib.o
        $(CP) $(MPAENCLIB) <Lib$Dir>.ARSndLib.o.$(MPAENCLIB) $(CPFLAGS)
        @echo $(LIBCOMPONENT): export complete (libs)

local_dirs:
        $(MKDIR) o

$(MPAENCLIB): $(LIBOBJS)
        $(LIBFILE) $(LIBFILEFLAGS) $(LIBOBJS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
