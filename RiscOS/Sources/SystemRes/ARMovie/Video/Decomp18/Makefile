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
CFLAGS2         = -gflv -DTQDEBUG $(CFLAGS)
CFLAGS3         = -DHierProf_PROFILE $(CFLAGS)
AFLAGS          = -depend !Depend $(THROWBACK)
AFLAGS2         = -g $(AFLAGS)
CPFLAGS         = ~CFR~V
LDFLAGS         = -rmf -first veneer$(VARIANT) -o $@
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
OBJS            = o.veneer$(VARIANT) \
                  o.control \
                  o.getblk \
                  o.getvlc \
                  o.getbits \
                  o.sac \
                  o.recon \
                  o.gethdr \
                  o.store \
                  o.jrevdct \
                  o.getpic \
                  o.dy_malloc \
                  o.mallarr \
                  o.arecon \
                  o.i2c \
                  o.agetbits \
                  o.idct \
                  o.aaddb \
                  o.amemset64 \
                  o.ady_malloc \
                  o.miniClib
TARGETDEB       = Deb$(VARIANT)
LIBSDEB         = C:o.stubs
OBJSDEB         = od.testdecomp \
                  od.control \
                  od.getblk \
                  od.getvlc \
                  od.getbits \
                  od.sac \
                  od.recon \
                  od.gethdr \
                  od.store \
                  od.testven$(VARIANT) \
                  od.jrevdct \
                  od.getpic \
                  od.dy_malloc \
                  od.mallarr \
                  od.arecon \
                  od.i2c \
                  od.agetbits \
                  od.idct \
                  od.aaddb \
                  od.amemset64 \
                  od.ady_malloc
TARGETPROF      = Pro$(VARIANT)
LIBSPROF        = C:o.stubs
OBJSPROF        = op.testdecomp \
                  op.control \
                  op.getblk \
                  op.getvlc \
                  op.getbits \
                  op.sac \
                  op.recon \
                  op.gethdr \
                  op.store \
                  op.testven$(VARIANT) \
                  op.jrevdct \
                  op.getpic \
                  op.dy_malloc \
                  op.mallarr \
                  op.arecon \
                  op.i2c \
                  op.agetbits \
                  op.idct \
                  op.aaddb \
                  HierProf:o.hp_t1 \
                  op.amemset64 \
                  op.ady_malloc

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .od .op .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.c.od:;    $(CC) $(CFLAGS2) -o $@ $<
.c.op:;    $(CC) $(CFLAGS3) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<
.s.od:;    $(AS) $(AFLAGS2) -o $@ $<
.s.op:;    $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): decoder build complete

debug: $(TARGETDEB)
        @echo $(COMPONENT): decoder debug build complete

profile: $(TARGETPROF)
        @echo $(COMPONENT): decoder profile build complete

clean:
        $(WIPE) o $(WFLAGS)
        $(WIPE) od $(WFLAGS)
        $(WIPE) op $(WFLAGS)
        $(RM) Dec
        $(RM) Deb
        $(RM) Pro
        $(RM) DecH
        $(RM) DebH
        $(RM) ProH
        $(RM) DecB
        $(RM) DebB
        $(RM) ProB
        $(RM) Dec24
        $(RM) Deb24
        $(RM) Pro24
        $(RM) Dec21
        $(RM) Deb21
        $(RM) Pro21
        $(RM) Dec246Y5UV
        $(RM) Deb246Y5UV
        $(RM) Pro246Y5UV
        $(RM) Dec216Y5UV
        $(RM) Deb216Y5UV
        $(RM) Pro216Y5UV
        $(RM) Dec16
        $(RM) Deb16
        $(RM) Pro16
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o
        $(MKDIR) od
        $(MKDIR) op
        Set Decomp18$Variant "$(VARIANT)"

$(TARGET): $(OBJS) local_dirs
        $(LD) $(LDFLAGS) $(LIBS) $(OBJS)
        Unset Decomp18$Variant

$(TARGETDEB): $(OBJSDEB) local_dirs
        $(LD) $(LDFLAGS2) $(LIBSDEB) $(OBJSDEB)
        Unset Decomp18$Variant

$(TARGETPROF): $(OBJSPROF) local_dirs
        $(LD) $(LDFLAGS3) $(LIBSPROF) $(OBJSPROF)
        Unset Decomp18$Variant

o.veneer$(VARIANT): s.veneer
        $(AS) $(AFLAGS) -o $@ s.veneer

od.testven$(VARIANT): s.testven
        $(AS) $(AFLAGS2) -o $@ s.testven

op.testven$(VARIANT): s.testven
        $(AS) $(AFLAGS) -o $@ s.testven

# ------------------------------------------------------------------------------
# Dynamic dependencies:
