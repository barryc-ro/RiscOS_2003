# Makefile for Decomp18
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 15 Dec 2000   BJGA            Created
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

CFLAGS          = -c -depend !Depend $(THROWBACK) -Otime -zps1 $(INCLUDES)
CFLAGS2         = -gflv $(CFLAGS)
CFLAGS3         = -DHierProf_PROFILE $(CFLAGS)
AFLAGS          = -depend !Depend $(THROWBACK)
AFLAGS2         = -g $(AFLAGS)
CPFLAGS         = ~CFR~V
LDFLAGS         = -rmf -first D$(VARIANT) -o $@
LDFLAGS2        = -aif -d -o $@
LDFLAGS3        = -aif -o $@
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = Decomp18
TARGET          = Dec$(VARIANT)
LIBS            =
OBJS            = o.D$(VARIANT) \
                  o.addblock \
                  o.ady_malloc \
                  o.adecodetc \
                  o.agetbits \
                  o.amemset64 \
                  o.arecon \
                  o.filter \
                  o.mkedgeimg \
                  o.doedgefilt \
                  o.dy_malloc \
                  o.entry \
                  o.getblk \
                  o.gethdr \
                  o.getpic \
                  o.getpicsub \
                  o.getvlc \
                  o.mallarr \
                  o.store \
                  o.miniClib
TARGETDEB       = DebugDec
LIBSDEB         = C:o.stubs
OBJSDEB         = od.testdecomp \
                  od.D \
                  od.testpatch \
                  od.addblock \
                  od.debug \
                  od.ady_malloc \
                  od.adecodetc \
                  od.agetbits \
                  od.amemset64 \
                  od.arecon \
                  od.filter \
                  od.mkedgeimg \
                  od.doedgefilt \
                  od.dy_malloc \
                  od.entry \
                  od.getblk \
                  od.gethdr \
                  od.getpic \
                  od.getpicsub \
                  od.getvlc \
                  od.mallarr \
                  od.store
TARGETPROF      = ProfDec
LIBSPROF        = HierProf:HP_T1.o \
                  C:o.stubs
OBJSPROF        = oh.testdecomp \
                  oh.D \
                  oh.testpatch \
                  oh.addblock \
                  oh.debug \
                  oh.ady_malloc \
                  oh.adecodetc \
                  oh.agetbits \
                  oh.amemset64 \
                  oh.arecon \
                  oh.filter \
                  oh.mkedgeimg \
                  oh.doedgefilt \
                  oh.dy_malloc \
                  oh.entry \
                  oh.getblk \
                  oh.gethdr \
                  oh.getpic \
                  oh.getpicsub \
                  oh.getvlc \
                  oh.mallarr \
                  oh.store
TARGETMEM       = MemDec
LIBSMEM         = MemCheck:MC.o \
                  C:o.stubs
OBJSMEM         = om.testdecomp \
                  om.D \
                  om.testpatch \
                  om.addblock \
                  om.debug \
                  om.ady_malloc \
                  om.adecodetc \
                  om.agetbits \
                  om.amemset64 \
                  om.arecon \
                  om.filter \
                  om.mkedgeimg \
                  om.doedgefilt \
                  om.dy_malloc \
                  om.entry \
                  om.getblk \
                  om.gethdr \
                  om.getpic \
                  om.getpicsub \
                  om.getvlc \
                  om.mallarr \
                  om.store

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .od .oh .om .s .hdr .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.c.od:;    $(CC) $(CFLAGS2) -o $@ $<
.c.oh:;    $(CC) $(CFLAGS3) -o $@ $<
.c.om:;    MemCheck.cc $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<
.s.od:;    $(AS) $(AFLAGS2) -o $@ $<
.s.oh:;    $(AS) $(AFLAGS) -o $@ $<
.s.om:;    $(AS) $(AFLAGS) -o $@ $<
.hdr.o:;   $(AS) $(AFLAGS) -o $@ $<
.hdr.od:;  $(AS) $(AFLAGS2) -o $@ $<
.hdr.oh:;  $(AS) $(AFLAGS) -o $@ $<
.hdr.om:;  $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): decoder build complete

debug: $(TARGETDEB)
        @echo $(COMPONENT): decoder debug build complete

profile: $(TARGETPROF)
        @echo $(COMPONENT): decoder profile build complete

memcheck: $(TARGETMEM)
        @echo $(COMPONENT): decoder memcheck build complete

clean:
        $(WIPE) o $(WFLAGS)
        $(WIPE) od $(WFLAGS)
        $(WIPE) oh $(WFLAGS)
        $(WIPE) om $(WFLAGS)
        $(RM) Dec
        $(RM) Dec16
        $(RM) Dec216Y5UV
        $(RM) Dec246Y5UV
        $(RM) DecB
        $(RM) DecFast21
        $(RM) DecFast24
        $(RM) DecFastYUV
        $(RM) DecH
        $(RM) Dec21
        $(RM) Dec24
        $(RM) DebugDec
        $(RM) ProfDec
        $(RM) MemDec
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o
        $(MKDIR) od
        $(MKDIR) oh
        $(MKDIR) om

$(TARGET): $(OBJS) local_dirs
        $(LD) $(LDFLAGS) $(LIBS) $(OBJS)
        SetType $(TARGET) FFD

$(TARGETDEB): $(OBJSDEB) local_dirs
        $(LD) $(LDFLAGS2) $(LIBSDEB) $(OBJSDEB)

$(TARGETPROF): $(OBJSPROF) local_dirs
        $(LD) $(LDFLAGS3) $(LIBSPROF) $(OBJSPROF)

$(TARGETMEM): $(OBJSMEM) local_dirs
        $(LD) $(LDFLAGS3) $(LIBSMEM) $(OBJSMEM)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
