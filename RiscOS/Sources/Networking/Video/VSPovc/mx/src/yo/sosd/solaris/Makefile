ifneq ($(CLEARCASE_ROOT),)
   SRCHOME        = /vobs
else
   t              = $(subst /, ,$(shell pwd))
   SRCHOME        = /$(word 1,$t)/$(word 2,$t)/$(word 3,$t)
endif
SRC	  = $(SRCHOME)/mx/src

include $(SRC)/buildtools/defaults.mk

INCLUDE	  = -I$(SRCHOME)/mx/pub \
	    -I$(SRCHOME)/mx/inc \
	    -I$(SRCHOME)/mx/src/inc \
	    -I$(SRCHOME)/mx/src/ys/sosd/$(ARCH) \
	    -I$(SRCHOME)/mx/src/mt/sosd/$(ARCH) \
	    -I$(SRCHOME)/mx/src/mn/sosd/$(ARCH) \
	    -I$(SRCHOME)/omsdev/osd/$(ARCH)/dprod/core/ship \
	    -I$(SRCHOME)/mx/src/mn \
	    -I$(SRCHOME)/mx/src/mt \
	    -I$(SRCHOME)/mx/src/yo \
	    -I$(SRCHOME)/mx/src/yo/sosd/$(ARCH) \
	    $(BSAFEINC)

COMPONENT = Object Runtime Layer

YOSRC	  = $(SRC)/yo
YOOSD     = $(YOSRC)/sosd/$(ARCH)

$(SRCHOME)/mx/src/mesg/%.mot : $(YOSRC)/mesg/%.msg
	$(MSGC) -I $(SRCHOME)/mx/src/mesg -o $@ $<

vpath %.c       $(YOOSD) $(YOSRC)
vpath %.a       $(LIB)
vpath %.idl     $(SRCHOME)/mx/src/inc
vpath $.$(SHX)  $(LIB)

LIBYO	  = $(LIB)/libyo.a
LIBYOTK   = $(LIB)/libyotk.a
LIBYOBS   = $(LIB)/libyobs.a
LIBYOBS40 = $(LIB)/libyobs40.a
LIBYOEX   = $(LIB)/libyoex.a
LIBYOBOA  = $(LIB)/libyoboa.a
ifneq ($(CRYPTO),)
  LIBS	  = $(LIBYOTK) $(LIBYO) $(LIBYOBOA) $(LIBYOBS)
  SHLIBS    = $(LIB)/libyo.$(SHX) \
	      $(LIB)/libyotk.$(SHX) \
	      $(LIB)/libyobs.$(SHX) \
	      $(LIB)/libyoboa.$(SHX)
else 
  ifneq ($(CRYPTO40),)
    LIBS  = $(LIBYOTK) $(LIBYO) $(LIBYOBOA) $(LIBYOBS40)
    SHLIBS = $(LIB)/libyo.$(SHX) \
	     $(LIB)/libyotk.$(SHX) \
	     $(LIB)/libyobs40.$(SHX) \
	     $(LIB)/libyoboa.$(SHX)
  else
    LIBS  = $(LIBYOTK) $(LIBYO) $(LIBYOBOA) $(LIBYOEX)
    SHLIBS = $(LIB)/libyo.$(SHX) \
	     $(LIB)/libyotk.$(SHX) \
	     $(LIB)/libyoex.$(SHX) \
	     $(LIB)/libyoboa.$(SHX)
  endif
endif


LOADLIBES = -lmn -lmis -lmt -lmn -lys

LIBYOOBJS = yo.o yoyd.o yocoa.o yogiop.o yox.o yort.o yobr.o yoevt.o \
	    yoidlS.o yoevtS.o ydyoidlC.o yostdC.o yotkx.o yot.o \
	    yocy.o yotransC.o

LIBYOTKOBJS = yotk.o yotkg.o yosx.o

LIBYOEXOBJS = yorc5x.o

LIBYOBSOBJS = yorc5.o yobs.o

LIBYOBOAOBJS= yoorb.o yoboa.o

LIBOBJS   = $(LIBYOTKOBJS) $(LIBYOOBJS) $(LIBYOBOAOBJS)

BINS  =

MSGDIR = $(SRC)/mesg
MSGS  = $(MSGDIR)/OMNYO.mot

MNIDLC_FLAGS = -a coa -s oracle -I $(SRCHOME)/mx/pub -o $(YOSRC) \
               -R "ys.log.msg-path=$(MSGDIR)"

IDLHDRS = $(YOSRC)/ydyoidl.h $(YOSRC)/yoevt.h $(YOSRC)/yoidl.h \
	    $(YOSRC)/yotrans.h $(YOSRC)/yostd.h

idlhdrs : $(IDLHDRS)

olint	:
	$(OLINT) $(INCLUDE) $(SRCS)

flint:
	$(OLINT) $(INCLUDE) $(FILE)

compile  : idlhdrs $(OBJS)

libyotk	 : $(LIBYOTK)

lib	 : idlhdrs $(LIBS)

ifneq ($(CRYPTO),)
crypto	 : $(LIBYOBS)
clean::
	    rm -f $(LIBYOBS) 

crypto-share : $(LIB)/libyobs.$(SHX)
clean::
	    rm -f $(LIB)/libyobs.$(SHX)
endif

# this "libyobs40" stuff is just so that it's obvious by looking at the 
# filename which version you have
ifneq ($(CRYPTO40),)
crypto	 : $(LIBYOBS40)
clean::
	    rm -f $(LIBYOBS40) 

crypto-share : $(LIB)/libyobs40.$(SHX)
clean::
	    rm -f $(LIB)/libyobs40.$(SHX)
endif

share    : $(SHLIBS)

link	 : $(BINS) $(MSGS)

resource : $(MSGS)

cleanobj :
	rm -f $(LIBOBJS) $(OBJS)

cleanlib :
	rm -f $(LIBS) $(SHLIBS)

cleanbin :
#	rm -f $(BINS)

$(LIBYO) : $(LIBYO)($(LIBYOOBJS))
	$(RANLIB) $@

$(LIBYOTK) : $(LIBYOTK)($(LIBYOTKOBJS))
	$(RANLIB) $@

$(LIBYOBOA) : $(LIBYOBOA)($(LIBYOBOAOBJS))
	$(RANLIB) $@

$(LIBYOBS) : $(LIBYOBS)($(LIBYOBSOBJS))
	$(RANLIB) $@

$(LIBYOBS40) : $(LIBYOBS40)($(LIBYOBSOBJS))
	$(RANLIB) $@

$(LIBYOEX) : $(LIBYOEX)($(LIBYOEXOBJS))
	$(RANLIB) $@

$(OBJ)/%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

$(BIN)/%: $(OBJ)/%.o
	$(LINK.o) $+ $(LDLIBS) -o $@

$(BINS): $(LOADLIBES)

yo.c: $(YOSRC)/yoidl.h $(YOSRC)/ydyoidl.h 
yocoa.c: $(YOSRC)/ydyoidl.h 
yoevt.c: $(YOSRC)/yoevt.h

$(YOSRC)/ydyoidl.h $(YOSRC)/ydyoidlC.c $(YOSRC)/ydyoidlS.c: ydyoidl.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f $(YOSRC)/ydyoidl.h $(YOSRC)/ydyoidlC.c $(YOSRC)/ydyoidlS.c

$(YOSRC)/yoidl.h $(YOSRC)/yoidlC.c $(YOSRC)/yoidlS.c: yoidl.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f $(YOSRC)/yoidl.h $(YOSRC)/yoidlC.c $(YOSRC)/yoidlS.c

$(YOSRC)/yoevt.h $(YOSRC)/yoevtC.c $(YOSRC)/yoevtS.c: yoevt.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f $(YOSRC)/yoevt.h $(YOSRC)/yoevtC.c $(YOSRC)/yoevtS.c

$(YOSRC)/yostd.h $(YOSRC)/yostdC.c $(YOSRC)/yostdS.c: \
   $(SRCHOME)/mx/pub/yostd.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f $(YOSRC)/yostd.h $(YOSRC)/yostdC.c $(YOSRC)/yostdS.c

$(YOSRC)/yotrans.h $(YOSRC)/yotransC.c: yotrans.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f $(YOSRC)/yotrans.h $(YOSRC)/yotransC.c

clean::
	rm -f $(MSGDIR)/OMNYO.mot
