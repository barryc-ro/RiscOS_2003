ifneq ($(CLEARCASE_ROOT),)
   SRCHOME        = /vobs
else
   t              = $(subst /, ,$(shell pwd))
   SRCHOME        = /$(word 1,$t)/$(word 2,$t)/$(word 3,$t)
endif
SRC	  = $(SRCHOME)/mx/src
include $(SRC)/buildtools/defaults.mk

INCLUDE	  =	-I$(SRCHOME)/mx/pub \
		-I$(SRCHOME)/mx/inc \
		-I$(SRCHOME)/mx/src/inc \
		-I$(SRCHOME)/mx/src/mn/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/ys/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/mt \
		-I$(SRCHOME)/mx/src/mt/sosd/$(ARCH)

COMPONENT = MT

MTSRC	  = $(SRC)/mt
MTOSD     = $(MTSRC)/sosd/$(ARCH)

$(SRCHOME)/mx/src/mesg/%.mot : $(MTSRC)/mesg/%.msg
	$(MSGC) -I $(SRCHOME)/mx/src/mesg -o $@ $<

vpath %.c       $(MTOSD) $(MTSRC)
vpath %.a       $(LIB)
vpath %.$(SHX)  $(LIB)

LIBMT	  = $(LIB)/libmt.a
LIBS	  = $(LIBMT)
SHLIBS    = $(LIB)/libmt.$(SHX)

CTXOBJS   = mtcctx.o
MTSPOBJS  = mtsp.o
MTLOBJS   = mtl.o 
MISCOBJS  = mtrgx.o

LIBOBJS   = $(CTXOBJS) $(MTSPOBJS) $(MTLOBJS) $(MISCOBJS)
LIBOBJS  += $(SS_MT_LIBOBJS)

OBJS      =
OBJS     += $(SS_MT_OBJS)

BINS      =
BINS     += $(SS_MT_BINS)

MSGDIR	  = $(SRC)/mesg
MSGS	  = $(MSGDIR)/OMNMT.mot

LOADLIBES = -lys

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
	@true #rm -f $(BINS)

clean::
	rm -f $(MSGS)

$(LIBMT) : $(LIBMT)($(LIBOBJS))
	$(RANLIB) $@

$(BIN)/omslogd : $(OBJ)/ssmtldmns.o
	$(LINK.o) -Nheap 200000 -Ncomm 200000 $+ -o $@ $(MPFLAGS)

$(OBJ)/%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

$(BIN)/%: $(OBJ)/%.o
	$(LINK.o) $< $(LOADLIBES) $(LDLIBS) -o $@

$(BINS) : $(LOADLIBES)
