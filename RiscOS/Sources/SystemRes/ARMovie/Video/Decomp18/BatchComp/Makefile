# Makefile for Decomp18 BatchComp
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

CFLAGS          = -c -depend !Depend $(THROWBACK) -Otime -DFASTIDCT -DOLD_SAD -DAIDCT $(INCLUDES)
CFLAGS2         = -gflv $(CFLAGS)
CFLAGS3         = -DMemCheck_MEMCHECK $(CFLAGS2)
AFLAGS          = -depend !Depend $(THROWBACK)
AFLAGS2         = -g $(AFLAGS)
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
LIBS            = C:o.stubs
OBJS            = o.filestuff \
                  o.mainplan \
                  o.reader \
                  o.do8YUV2CIF \
                  o.blatasm \
                  o.cdir \
                  o.coder \
                  o.countbit \
                  o.dct \
                  o.io \
                  o.mot_est \
                  o.pred \
                  o.quant \
                  o.putvlc \
                  o.putbits \
                  o.ratectrl \
                  o.sac \
                  o.snr \
                  o.tmnmain \
                  o.tmnarg \
                  o.getargs \
                  o.error \
                  o.batchcomp \
                  o.Logger \
                  o.checkpoint \
                  o.inkey \
                  o.setenv \
                  o.jrevdct \
                  o.h263_compif \
                  o.flushc
TARGETDEB       = BatchCompDeb
LIBSDEB         = C:o.stubs
OBJSDEB         = od.filestuff \
                  od.mainplan \
                  od.reader \
                  od.do8YUV2CIF \
                  od.blatasm \
                  od.cdir \
                  od.coder \
                  od.countbit \
                  od.dct \
                  od.io \
                  od.mot_est \
                  od.pred \
                  od.quant \
                  od.putvlc \
                  od.putbits \
                  od.ratectrl \
                  od.sac \
                  od.snr \
                  od.tmnmain \
                  od.tmnarg \
                  od.getargs \
                  od.error \
                  od.batchcomp \
                  od.Logger \
                  od.checkpoint \
                  od.inkey \
                  od.setenv \
                  od.jrevdct \
                  od.h263_compif \
                  od.flushc
TARGETMEM       = BatchCompMem
LIBSMEM         = C:o.stubs \
                  MemCheck:o.MC \
                  HierProf:o.HP
OBJSMEM         = om.filestuff \
                  om.mainplan \
                  om.reader \
                  om.do8YUV2CIF \
                  od.blatasm \
                  om.cdir \
                  om.coder \
                  om.countbit \
                  om.dct \
                  om.io \
                  om.mot_est \
                  om.pred \
                  om.quant \
                  om.putvlc \
                  om.putbits \
                  om.ratectrl \
                  om.sac \
                  om.snr \
                  om.tmnmain \
                  om.tmnarg \
                  om.getargs \
                  om.error \
                  om.batchcomp \
                  om.Logger \
                  om.checkpoint \
                  om.inkey \
                  om.setenv \
                  od.jrevdct \
                  om.h263_compif \
                  od.flushc

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .od .om .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.c.od:;    $(CC) $(CFLAGS2) -o $@ $<
.c.om:;    MemCheck.CC $(CC) $(CFLAGS3) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<
.s.od:;    $(AS) $(AFLAGS2) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all: $(TARGET)
        @echo $(COMPONENT): decoder build complete

debug: $(TARGETDEB)
        @echo $(COMPONENT): decoder debug build complete

memcheck: $(TARGETMEM)
        @echo $(COMPONENT): decoder memcheck build complete

clean:
        $(WIPE) o $(WFLAGS)
        $(WIPE) od $(WFLAGS)
        $(WIPE) om $(WFLAGS)
        $(RM) BatchComp
        $(RM) BatchCompDeb
        $(RM) BatchCompMem
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o
        $(MKDIR) od
        $(MKDIR) om

$(TARGET): $(OBJS) local_dirs
        $(LD) $(LDFLAGS) $(LIBS) $(OBJS)

$(TARGETDEB): $(OBJSDEB) local_dirs
        $(LD) $(LDFLAGS2) $(LIBSDEB) $(OBJSDEB)

$(TARGETMEM): $(OBJSMEM) local_dirs
        $(LD) $(LDFLAGS2) $(LIBSMEM) $(OBJSMEM)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.quant: c.quant h.sim h.config h.macros
o.Logger: c.Logger h.logger h.bool h.error h.bool C:h.stdlib
o.inkey: c.inkey C:h.swis C:h.kernel h.error
o.flushc: s.flushc
o.filestuff: c.filestuff h.bool C:h.ctype h.filestuff h.bool h.flt h.flt h.error
o.mainplan: c.mainplan h.HierProf h.reader h.flt h.filestuff h.bool h.flt h.cdir h.sim h.config h.macros h.error h.batchcomp h.bool h.flt h.Logger h.bool h.tmnarg h.config h.checkpoint h.bool h.setenv h.compIF h.bool h.batchcomp h.Tmnarg
o.reader: c.reader h.filestuff h.bool h.flt h.blatasm h.decerr C:h.string h.do8YUV2CIF h.reader h.flt h.error h.decerr
o.do8YUV2CIF: c.do8YUV2CIF
o.blatasm: s.blatasm
o.coder: c.coder h.sim h.config h.macros h.error
o.countbit: c.countbit h.sim h.config h.macros h.sactbls h.indices h.putvlc
o.dct: c.dct h.macros h.sim h.config h.macros
o.io: c.io h.sim h.config h.macros h.reader h.flt
o.mot_est: c.mot_est h.sim h.config h.macros
o.pred: c.pred h.sim h.config h.macros
o.putvlc: c.putvlc h.sim h.config h.macros h.vlc
o.putbits: c.putbits h.sim h.config h.macros
o.ratectrl: c.ratectrl h.Logger h.bool h.sim h.config h.macros
o.sac: c.sac h.sim h.config h.macros
o.snr: c.snr h.sim h.config h.macros
o.tmnmain: c.tmnmain h.sim h.config h.macros h.tmnarg h.config h.logger h.bool h.checkpoint h.bool
o.tmnarg: c.tmnarg h.tmnarg h.config h.sim h.config h.macros
o.getargs: c.getargs h.getargs C:h.swis C:h.kernel h.error
o.batchcomp: c.batchcomp h.getargs h.error h.bool h.batchcomp h.bool h.flt C:h.stdlib C:h.stdio h.filestuff h.bool h.flt h.compIF h.bool h.batchcomp h.Tmnarg h.config
o.checkpoint: c.checkpoint h.inkey h.checkpoint h.bool h.bool h.error h.Logger h.bool h.setenv C:h.stdlib h.reader h.flt
o.setenv: c.setenv h.setenv C:h.swis C:h.kernel
o.h263_compif: c.h263_compif h.bool h.batchcomp h.bool h.flt h.error h.tmnarg h.config h.t_init h.compIF h.bool h.batchcomp h.Tmnarg h.filestuff h.bool h.flt h.h263_pict
