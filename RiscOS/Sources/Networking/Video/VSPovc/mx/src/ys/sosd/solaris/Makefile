ifneq ($(CLEARCASE_ROOT),)
   SRCHOME        = /vobs
else
   t              = $(subst /, ,$(shell pwd))
   SRCHOME        = /$(word 1,$t)/$(word 2,$t)/$(word 3,$t)
endif
SRC	  = $(SRCHOME)/mx/src
include $(SRC)/buildtools/defaults.mk

INCLUDE	  = -I$(SRCHOME)/mx/src/ys/sosd/$(ARCH) \
	    -I$(SRCHOME)/mx/pub \
	    -I$(SRCHOME)/mx/inc \
	    -I$(SRCHOME)/mx/src/inc \
	    -I$(SRCHOME)/mx/src/ys

COMPONENT = MX service layer

S0YSMAIN  = $(SRCHOME)/mx/src/ys/sosd/$(ARCH)/s0ysmain.c

YSSRC	  = $(SRC)/ys
YSOSD     = $(YSSRC)/sosd/$(ARCH)

$(SRCHOME)/mx/src/mesg/%.mot : $(YSSRC)/mesg/%.msg
	$(MSGC) -I $(SRCHOME)/mx/src/mesg -o $@ $<

vpath %.c       $(YSOSD) $(YSSRC)
vpath %.a       $(LIB)
vpath %.$(SHX)  $(LIB)

LIBYS	  = $(LIB)/libys.a
LIBS	  = $(LIBYS)
ifdef SHX
SHLIBS    = $(LIB)/libys.$(SHX)
endif

LIBOBJS   = sysi.o sysrld.o sysfp.o ys.o syst.o ysb8.o ysc.o yse.o ysfmt.o \
	    yshsh.o ysid.o ysl.o yslst.o ysm.o ysmdmp.o ysr.o ysrarg.o \
	    ysrprs.o yssp.o ysstr.o ystm.o yst.o ysv.o ysver.o ysx.o yscksm.o \
	    ysrgx.o yslog.o ysmsg.o ysrand.o ysthr.o ysfe.o systhr.o ysfo.o \
	    ysbv.o

LIBOBJS  += $(SS_YS_LIBOBJS)

OBJS	  = $(OBJ)/s0mnmsgc.o $(OBJ)/ysmsgc.o
OBJS     += $(SS_YS_OBJS)

BINS	  = $(BIN)/mnmsgc
BINS     += $(SS_YS_BINS)

MSGS	  = $(SRC)/mesg/OMNMSGC.mot $(SRC)/mesg/OMNYS.mot
MSGS     += $(SS_YS_MSGS)

compile  : $(OBJS)

lib	 : $(LIBS)

share    : $(SHLIBS)

link	 : $(BINS) $(MSGS)

resource : $(MSGS)

cleanobj :
	rm -f $(OBJS) $(LIBOBJS)

cleanlib :
	rm -f $(LIBS) $(SHLIBS)

cleanbin :
	rm -f $(BINS)

$(LIBYS) : $(LIBYS)($(LIBOBJS))
	$(RANLIB) $@

$(LIBYS)(ysver.o) : $(SRCHOME)/mx/src/ys/sosd/$(ARCH)/sysi.h

# If you take this out, you will inexorably break the nCube port and be
# subject to gross suffering and lethally dire consequences.
ifeq ($(SS_YS_OPTMALLOC), yes)
malloc.o : malloc.c
	$(COMPILE.c) -O $< $(OUTPUT_OPTION)
endif

$(OBJ)/%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)
	
$(BIN)/%: $(OBJ)/%.o
	$(LINK.o) $+ $(LDLIBS) -o $@

$(OBJ)/s0mnmsgc.o : $(S0YSMAIN)
	$(CC) $(CFLAGS) -DENTRY_POINT=ysmsgc -c -o $@ $?

$(BIN)/mnmsgc : $(OBJ)/s0mnmsgc.o $(OBJ)/ysmsgc.o $(LIBYS)
	$(LINK.o) $+ $(LIBHOME) $(LOADLIBES) $(LDLIBS) -o $@

$(BIN)/nsp: $(OBJ)/nsps.o -lmt -lys
	$(LINK.o) $+ -o $@ $(MPFLAGS)
