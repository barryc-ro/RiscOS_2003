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
		-I$(SRCHOME)/mx/src/ys/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/mn \
		-I$(SRCHOME)/mx/src/ye \
		-I$(SRCHOME)/mx/src/yo \

COMPONENT = YE

YESRC	  = $(SRC)/ye
YEOSD     = $(YESRC)/sosd/$(ARCH)

$(SRCHOME)/mx/src/mesg/%.mot : $(YESRC)/mesg/%.msg
	$(MSGC) -I $(SRCHOME)/mx/src/mesg -o $@ $<

vpath %.c       $(YEOSD) $(YESRC)
vpath %.a       $(LIB)
vpath %.idl     $(SRCHOME)/mx/src/inc
vpath %.$(SHX)  $(LIB)

IDLSRC	= $(SRCHOME)/mx/src/inc
INCSRC  = $(SRCHOME)/mx/inc

LIBYES    = $(LIB)/libyeS.a
LIBYEC    = $(LIB)/libyeC.a
LIBS	  = $(LIBYES) $(LIBYEC)
SHLIBS    = $(LIB)/libyeS.$(SHX) $(LIB)/libyeC.$(SHX)

LIBHOME   = -L$(LIB)
LOADLIBES =  -lyeS -lyeC -lyo -lyotk -lmn -lmt -lys 
# perversely, libyr is required for the dynamic build but not the static one
# otherwise libyotk cannot resolve yrGetAbsoluteName calls it never makes
LOADLIBES += -lyr

LIBYESOBJS = yece.o yeev.o yeu.o \
	    yeeventS.o yeevlogS.o \
	    yecevchS.o yemsg.o yeevf.o yeevlg.o

LIBYECOBJS = yeeventC.o yeevlogC.o ydidlC.o

LIBOBJS = $(LIBYESOBJS) $(LIBYECOBJS)

OBJS	  = $(OBJ)/yeced.o $(OBJ)/yeevld.o \
	    $(OBJ)/yeevctl.o $(OBJ)/yeevlr.o

ifeq ($(OLINT),)
        OLINT = olint
endif

SRCS	  = yeev.c yeu.c yemsg.c yeevf.c yeevlg.c \
	    yeevld.c yeevctl.c yeevlr.c \
	    yece.c yeced.c

BINS	 = $(BIN)/mnlogreader $(BIN)/mnlogsrv $(BIN)/mnlogctl \
	   $(BIN)/yeced

MSGDIR = $(SRC)/mesg

MSGS	=   $(MSGDIR)/OMNYEEV.mot $(MSGDIR)/OMNYEEVLD.mot $(MSGDIR)/OMNYECE.mot

MNIDLC_FLAGS =  -a coa -I ../../pub -o $(YESRC) -s oracle -S \
		-R "ys.log.msg-path=$(MSGDIR)" -R "mnidlc.c-cplus-kwd=true"

IDLHDRS	= $(YESRC)/yeevent.h \
	  $(YESRC)/yeevlog.h $(YESRC)/yecevch.h \
	  $(YESRC)/ydidl.h

yeced	: $(BIN)/yeced

mnlogsrv: $(BIN)/mnlogsrv

mnlogreader: $(BIN)/mnlogreader

mnlogctl: $(BIN)/mnlogctl

idlhdrs	: $(IDLHDRS)

olint	:
	$(OLINT) $(INCLUDE) $(SRCS)

# invoke as 'make flint FILE=foo.c'
flint	:
	$(OLINT) $(INCLUDE) $(FILE)

compile  : idlhdrs $(OBJS)

lib	 : idlhdrs $(LIBS)

share    : $(SHLIBS)

link	 : $(BINS) $(MSGS)

resource : $(MSGS)

cleanobj :
	rm -f $(LIBOBJS) $(OBJS)

cleanlib :
	rm -f $(LIBS) $(SHLIBS)

cleanbin :
	rm -f $(BINS)

$(LIBYES) : $(LIBYES)($(LIBYESOBJS))
	$(RANLIB) $@

$(LIBYEC) : $(LIBYEC)($(LIBYECOBJS))
	$(RANLIB) $@

$(OBJ)/%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

$(BIN)/%: $(OBJ)/%.o
	$(LINK.o) $< $(LIBHOME) $(LOADLIBES) $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnlogsrv : $(OBJ)/yeevld.o 
	$(LINK.o) $(OBJ)/yeevld.o \
		    $(LIBHOME) $(LOADLIBES) $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnlogreader : $(OBJ)/yeevlr.o
	$(LINK.o) $(OBJ)/yeevlr.o \
		    $(LIBHOME) $(LOADLIBES) $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnlogctl : $(OBJ)/yeevctl.o
	$(LINK.o) $< $(LIBHOME) $(LOADLIBES) $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BINS): $(LOADLIBES)

$(YESRC)/yeevent.h $(YESRC)/yeeventS.c $(YESRC)/yeeventC.c: \
    $(INCSRC)/yeevent.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f yeevent.h yeevent?.[ch]

$(YESRC)/yecevch.h $(YESRC)/yecevchS.c $(YESRC)/yecevchC.c: \
    $(SRCHOME)/mx/pub/yecevch.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f yecevch.h yecevch?.[ch]

$(YESRC)/yeevlog.h $(YESRC)/yeevlogS.c $(YESRC)/yeevlogC.c: \
    $(INCSRC)/yeevlog.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f yeevlog.h yeevlog?.[ch]

$(YESRC)/yexevent.h $(YESRC)/yexeventS.c $(YESRC)/yexeventC.c: \
    $(SRCHOME)/mx/src/inc/yexevent.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f yexevent.h yexevent?.[ch]

$(YESRC)/ydidl.h $(YESRC)/ydidlS.c $(YESRC)/ydidlC.c: \
    $(SRCHOME)/mx/src/inc/ydidl.idl
	$(YCIDL) $(MNIDLC_FLAGS) $<
clean::
	rm -f ydidl.h ydidl?.[ch]

clean::
	rm -f $(MSGDIR)/OMNYEEV.mot

clean::
	rm -f $(MSGDIR)/OMNYEEVLD.mot
