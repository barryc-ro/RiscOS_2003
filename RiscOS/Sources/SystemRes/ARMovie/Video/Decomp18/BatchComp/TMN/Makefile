# Makefile for Decomp18 BatchComp TMN library
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
CFLAGS2         = $(CFLAGS) -DLOGGING
CFLAGS3         = $(CFLAGS2) -g -DMemCheck_MEMCHECK
CFLAGS4         = $(CFLAGS) -g -DHierProf_PROFILE
CFLAGS5         = $(CFLAGS) -DNO_FP -zM -DFAST_AND_DIRTY
AFLAGS          = -depend !Depend $(THROWBACK)
CPFLAGS         = ~CFR~V
LDFLAGS         = -aif -o $@
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

COMPONENT       = Decomp18 BatchComp TMN library
TARGET          = h263CLib
OBJS            = o.mot_est \
                  o.asm_dct \
                  o.bits \
                  o.blatasm \
                  o.flushc \
                  o.sad_mb \
                  o.asac \
                  o.coder \
                  o.countbit \
                  o.image \
                  o.pred \
                  o.putvlc \
                  o.ratectrl \
                  o.sac \
                  o.snr \
                  o.tmnarg
TARGETLOG       = h263CLibL
OBJSLOG         = o.mot_est \
                  o.asm_dct \
                  o.bits \
                  o.blatasm \
                  o.flushc \
                  o.sad_mb \
                  o.asac \
                  ol.coder \
                  ol.countbit \
                  ol.image \
                  ol.pred \
                  ol.putvlc \
                  ol.ratectrl \
                  ol.sac \
                  ol.snr \
                  ol.tmnarg
TARGETMEM       = h263CLibM
OBJSMEM         = o.mot_est \
                  o.asm_dct \
                  o.bits \
                  o.blatasm \
                  o.flushc \
                  o.sad_mb \
                  o.asac \
                  om.coder \
                  om.countbit \
                  om.image \
                  om.pred \
                  om.putvlc \
                  om.ratectrl \
                  om.sac \
                  om.snr \
                  om.tmnarg
TARGETPRO       = h263CLibH
OBJSPRO         = o.mot_est \
                  o.asm_dct \
                  o.bits \
                  o.blatasm \
                  o.flushc \
                  o.sad_mb \
                  o.asac \
                  oh.coder \
                  oh.countbit \
                  oh.image \
                  oh.pred \
                  oh.putvlc \
                  oh.ratectrl \
                  oh.sac \
                  oh.snr \
                  oh.tmnarg
TARGETZM        = h263CLibZ
OBJSZM          = o.mot_est \
                  o.asm_dct \
                  o.bits \
                  o.blatasm \
                  o.flushc \
                  o.sad_mb \
                  o.asac \
                  oz.coder \
                  oz.countbit \
                  oz.image \
                  oz.pred \
                  oz.putvlc \
                  oz.ratectrl \
                  oz.sac \
                  oz.snr \
                  oz.tmnarg

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .ol .om .oh .oz .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.c.ol:;    $(CC) $(CFLAGS2) -o $@ $<
.c.om:;    MemCheck.CC $(CC) $(CFLAGS3) -o $@ $<
.c.oh:;    $(CC) $(CFLAGS4) -o $@ $<
.c.oz:;    $(CC) $(CFLAGS5) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): tmnlib build complete

logging: $(TARGETLOG)
        @echo $(COMPONENT): tmnlib logging build complete

memcheck: $(TARGETMEM)
        @echo $(COMPONENT): tmnlib memcheck build complete

profile: $(TARGETPRO)
        @echo $(COMPONENT): tmnlib profiling build complete

zM: $(TARGETZM)
        @echo $(COMPONENT): tmnlib zM build complete

clean:
        $(WIPE) o $(WFLAGS)
        $(WIPE) ol $(WFLAGS)
        $(WIPE) om $(WFLAGS)
        $(WIPE) oh $(WFLAGS)
        $(WIPE) oz $(WFLAGS)
        $(RM) h263CLib
        $(RM) h263CLibL
        $(RM) h263CLibM
        $(RM) h263CLibH
        $(RM) h263CLibZ
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o
        $(MKDIR) ol
        $(MKDIR) om
        $(MKDIR) oh
        $(MKDIR) oz

$(TARGET): $(OBJS) local_dirs
        $(LIBFILE) $(LIBFILEFLAGS) $(OBJS)

$(TARGETLOG): $(OBJSLOG) local_dirs
        $(LIBFILE) $(LIBFILEFLAGS) $(OBJSLOG)

$(TARGETMEM): $(OBJSMEM) local_dirs
        $(LIBFILE) $(LIBFILEFLAGS) $(OBJSMEM)

$(TARGETPRO): $(OBJSPRO) local_dirs
        $(LIBFILE) $(LIBFILEFLAGS) $(OBJSPRO)

$(TARGETZM): $(OBJSZM) local_dirs
        $(LIBFILE) $(LIBFILEFLAGS) $(OBJSZM)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
