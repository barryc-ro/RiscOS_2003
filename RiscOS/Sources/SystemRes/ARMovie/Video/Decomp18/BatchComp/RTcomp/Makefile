# Makefile for Decomp18 RTcomp
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 13 Mar 2001   BJGA            Created
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

CFLAGS          = -c -depend !Depend $(THROWBACK) $(INCLUDES) -Otime -DOFFLINE_RATE_CONTROL -DFASTIDCT -DAIDCT
CFLAGS3         = $(CFLAGS) -g -DMemCheck_MEMCHECK
CFLAGS4         = $(CFLAGS) -g -DHierProf_PROFILE
CFLAGS5         = $(CFLAGS) -DNO_FP -zM
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = -aof -o $@
LDFLAGS2        = -aif -d -o $@
WFLAGS          = ~CFR~V
LIBFILEFLAGS    = -c -o $@

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = Decomp18 RTcomp
TARGET          = RTcomp
LIBS            = ^.TMN.h263CLib
OBJS            = o.rtcomp
TARGETMEM       = RTcompM
LIBSMEM         = ^.TMN.h263CLib
OBJSMEM         = om.rtcomp
TARGETZM        = RTcompZ
LIBSZM          = ^.TMN.h263CLibZ
OBJSZM          = oz.rtcomp
TARGETTST       = testrtcomp
LIBSTST         = C:o.stubs \
                  ^.TMN.h263CLib
OBJSTST         = o.rtcomp \
                  o.testrtcomp \
                  o.Malloc

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .om .oz .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.c.om:;    MemCheck.CC $(CC) $(CFLAGS3) -o $@ $<
.c.oz:;    $(CC) $(CFLAGS5) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): rtcomp build complete

memcheck: $(TARGETMEM)
        @echo $(COMPONENT): rtcomp memcheck build complete

zM: $(TARGETZM)
        @echo $(COMPONENT): rtcomp zM build complete

test: $(TARGETTST)
        @echo $(COMPONENT): rtcomp test build complete

clean:
        $(WIPE) o $(WFLAGS)
        $(WIPE) om $(WFLAGS)
        $(WIPE) oz $(WFLAGS)
        $(RM) RTcomp
        $(RM) RTcompM
        $(RM) RTcompZ
        $(RM) testrtcomp
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o
        $(MKDIR) om
        $(MKDIR) oz

$(TARGET): $(OBJS) local_dirs
        $(LD) $(LDFLAGS) $(LIBS) $(OBJS)

$(TARGETMEM): $(OBJSMEM) local_dirs
        $(LD) $(LDFLAGS) $(LIBSMEM) $(OBJSMEM)

$(TARGETZM): $(OBJSZM) local_dirs
        $(LD) $(LDFLAGS) $(LIBSZM) $(OBJSZM)

$(TARGETTST): $(OBJSTST) local_dirs
        $(LD) $(LDFLAGS2) $(LIBSTST) $(OBJSTST)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
