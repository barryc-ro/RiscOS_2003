# Makefile for Decomp902
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 21 Dec 2000   BJGA            Created
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

CFLAGS          = -c -depend !Depend $(THROWBACK) $(INCLUDES)
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = -rmf -first veneer -o $@
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = 

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = Decomp902
TARGET          = Decompress
LIBS            =
OBJS            = o.veneer \
                  o.control \
                  o.hqvdc_s \
                  o.ModPred \
                  o.env \
                  o.yuvblit \
                  o.dx_plane

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): decoder build complete

clean:
        $(WIPE) o $(WFLAGS)
        $(RM) Decompress
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o

$(TARGET): $(OBJS) local_dirs
        $(LD) $(LDFLAGS) $(LIBS) $(OBJS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.ModPred: c.ModPred h.datatype
o.dx_plane: s.dx_plane
o.control: c.control h.datatype h.indeodefs h.datatype
o.yuvblit: s.yuvblit hdr.plook
