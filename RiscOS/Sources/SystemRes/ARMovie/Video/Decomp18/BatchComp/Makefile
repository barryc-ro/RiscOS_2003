# Makefile for Decomp18 BatchComp
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 21 Dec 2000   BJGA            Created
# 13 Mar 2001   BJGA            Substantially altered
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

CFLAGS          = -c -depend !Depend $(THROWBACK) $(INCLUDES) -Otime -DOFFLINE_RATE_CONTROL -DFASTIDCT -DAIDCT
CFLAGS2         = $(CFLAGS) -DLOGGING
CFLAGS3         = $(CFLAGS2) -g -DMemCheck_MEMCHECK
CFLAGS4         = $(CFLAGS) -g -DHierProf_PROFILE
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = -aif -o $@
LDFLAGS2        = -aif -d -o $@
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = Decomp18 BatchComp
TARGET          = BatchComp
LIBS            = C:o.stubs \
                  TMN.h263CLib
OBJS            = o.mainplan \
                  o.batchcomp \
                  o.cdir \
                  o.checkpoint \
                  o.do8YUV2CIF \
                  o.error \
                  o.filestuff \
                  o.getargs \
                  o.h263_compi \
                  o.inkey \
                  o.io \
                  o.Logger \
                  o.reader \
                  o.setenv \
                  o.tmall \
                  o.tmnmain
TARGETLOG       = BatchCompL
LIBSLOG         = C:o.stubs \
                  TMN.h263CLibL
OBJSLOG         = ol.mainplan \
                  ol.batchcomp \
                  ol.cdir \
                  ol.checkpoint \
                  ol.do8YUV2CIF \
                  ol.error \
                  ol.filestuff \
                  ol.getargs \
                  ol.h263_compi \
                  ol.inkey \
                  ol.io \
                  ol.Logger \
                  ol.reader \
                  ol.setenv \
                  ol.tmall \
                  ol.tmnmain
TARGETMEM       = BatchCompM
LIBSMEM         = C:o.stubs \
                  TMN.h263CLibM \
                  MemCheck:o.MC
OBJSMEM         = om.mainplan \
                  om.batchcomp \
                  om.cdir \
                  om.checkpoint \
                  om.do8YUV2CIF \
                  om.error \
                  om.filestuff \
                  om.getargs \
                  om.h263_compi \
                  om.inkey \
                  om.io \
                  om.Logger \
                  om.reader \
                  om.setenv \
                  om.tmall \
                  om.tmnmain
TARGETPRO       = BatchCompH
LIBSPRO         = C:o.stubs \
                  TMN.h263CLibH \
                  HierProf:o.HP_T1
OBJSPRO         = oh.mainplan \
                  ol.batchcomp \
                  ol.cdir \
                  ol.checkpoint \
                  ol.do8YUV2CIF \
                  ol.error \
                  ol.filestuff \
                  ol.getargs \
                  ol.h263_compi \
                  ol.inkey \
                  ol.io \
                  ol.Logger \
                  ol.reader \
                  ol.setenv \
                  ol.tmall \
                  ol.tmnmain

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .ol .om .oh .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.c.ol:;    $(CC) $(CFLAGS2) -o $@ $<
.c.om:;    MemCheck.CC $(CC) $(CFLAGS3) -o $@ $<
.c.oh:;    $(CC) $(CFLAGS4) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): batchcomp build complete

logging: $(TARGETLOG)
        @echo $(COMPONENT): batchcomp logging build complete

memcheck: $(TARGETMEM)
        @echo $(COMPONENT): batchcomp memcheck build complete

profile: $(TARGETPRO)
        @echo $(COMPONENT): batchcomp profiling build complete

clean:
        $(WIPE) o $(WFLAGS)
        $(WIPE) ol $(WFLAGS)
        $(WIPE) om $(WFLAGS)
        $(WIPE) oh $(WFLAGS)
        $(RM) BatchComp
        $(RM) BatchCompL
        $(RM) BatchCompM
        $(RM) BatchCompH
        @Dir TMN
        @amu_machine clean
        @stripdepnd
        @Up
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o
        $(MKDIR) ol
        $(MKDIR) om
        $(MKDIR) oh

$(TARGET): $(LIBS) $(OBJS) local_dirs
        $(LD) $(LDFLAGS) $(LIBS) $(OBJS)

$(TARGETLOG): $(LIBSLOG) $(OBJSLOG) local_dirs
        $(LD) $(LDFLAGS) $(LIBSLOG) $(OBJSLOG)

$(TARGETMEM): $(LIBSMEM) $(OBJSMEM) local_dirs
        $(LD) $(LDFLAGS2) $(LIBSMEM) $(OBJSMEM)

$(TARGETPRO): $(LIBSPRO) $(OBJSPRO) local_dirs
        $(LD) $(LDFLAGS2) $(LIBSPRO) $(OBJSPRO)

TMN.h263CLib:
        @Dir TMN
        @amu_machine
        @Up

TMN.h263CLibL:
        @Dir TMN
        @amu_machine logging
        @Up

TMN.h263CLibM:
        @Dir TMN
        @amu_machine memcheck
        @Up

TMN.h263CLibH:
        @Dir TMN
        @amu_machine profile
        @Up

# ------------------------------------------------------------------------------
# Dynamic dependencies:
