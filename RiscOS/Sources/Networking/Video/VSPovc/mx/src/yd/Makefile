ifneq ($(CLEARCASE_ROOT),)
   SRCHOME        = /vobs
else
   t              = $(subst /, ,$(shell pwd))
   SRCHOME        = /$(word 1,$t)/$(word 2,$t)/$(word 3,$t)
endif
SRC	  = $(SRCHOME)/mx/src

include $(SRC)/buildtools/defaults.mk

FILE = /dev/null

INCLUDE  =	-I$(SRCHOME)/mx/pub \
		-I$(SRCHOME)/mx/inc \
		-I$(SRCHOME)/mx/src/inc \
		-I$(SRCHOME)/mx/src/ys/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/mt/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/mn \
		-I$(SRCHOME)/mx/src/mt \
		-I$(SRCHOME)/mx/src/yd \
		-I$(SRCHOME)/mx/src/yd/sosd/$(ARCH) \
		-I$(SRCHOME)/mx/src/yo \
		-I$(SRCHOME)/network_src/include \
		-I$(SRCHOME)/oracore/include \
		-I$(SRCHOME)/nlsrtl/include \
		-I$(SRCHOME)/otrace_bin/public \
		-I$(SRCHOME)/rdbms/include

COMPONENT = YD

YDSRC	  = $(SRC)/yd
YDOSD     = $(YDSRC)/sosd/$(ARCH)

$(SRCHOME)/mx/src/mesg/%.mot : $(YDSRC)/mesg/%.msg
	$(MSGC) -I $(SRCHOME)/mx/src/mesg -o $@ $<

ifeq ($(OLINT),)
        OLINT = olint
endif

vpath %.c       $(YDOSD) $(YDSRC)
vpath %.a       $(LIB)
vpath %.idl     $(SRCHOME)/mx/src/inc
vpath %.$(SHX)  $(LIB)

IDLSRC	= $(SRCHOME)/mx/src/inc

LIBYDS    = $(LIB)/libydS.a
LIBYDC    = $(LIB)/libydC.a
LIBYD    = $(LIB)/libyd.a
LIBS	  = $(LIBYD) $(LIBYDS) $(LIBYDC)
SHLIBS    = $(LIB)/libydS.$(SHX) $(LIB)/libydC.$(SHX) $(LIB)/libyd.$(SHX)

NETHOME=$(SRCHOME)/network_src
NAETAB = $(NETHOME)/lib/naeet.o $(NETHOME)/lib/naect.o
LNNOLIB=-lnno
NETLIBS= $(NAETAB) -losn -lnmcpi -lnsgl -lnsglc -lnnfg -lnncc -lnnci -lnng $(LNNOLIB) -lnmp -lnpl -lncr -lnnci -lnr -lns -lnt -lntp -lntr -lntt -lntus $(PROTOCOLLIBS) -lnt -lna -lnae $(ANOLIBS) -lnau -lnmd -lnms $(LSNMPLIB) -lnms -lsns -lnl1 -lnlnv

TNSLIBS   = -L /vobs/network_src/lib $(NETLIBS) \
            -L /vobs/oracore/lib -L /vobs/nlsrtl/lib -L /vobs/otrace_bin/lib \
            -L /vobs/rdbms/lib -lgeneric -lcommon  -lepc \
            -lnlsrtl3 -lcore3 -lnlsrtl3 -lcore3

LIBHOME   = -L$(LIB)
LOADLIBES = -lyeS -lyo -lyotk -lyr -lys -lmn -lmt -lmn -lys
YDCLIB	  = -lyd 
YDSLIB	  = -lydS -lyd
LIBYDSOBJS = ydim.o ydca.o ydch.o ydmt.o ydsp.o ydrt.o \
	    ydnm.o ydidlS.o ydyoidlS.o ydmtdidlS.o ydnmidlS.o
LIBYDOBJS = ydu.o ydq.o
LIBYDCOBJS = ydidlC.o ydyoidlC.o ydmtdidlC.o ydnmidlC.o ydbriC.o

LIBOBJS = $(LIBYDSOBJS) $(LIBYDCOBJS) $(LIBYDOBJS) 

OBJS	  = $(OBJ)/yd.o $(OBJ)/ydmtd.o $(OBJ)/ydnmd.o \
	    $(OBJ)/ydcf.o $(OBJ)/ydls.o \
	    $(OBJ)/ydspps.o $(OBJ)/ydadm.o $(OBJ)/ydchadm.o
YDBROBJS = $(OBJ)/ydbr.o $(OBJ)/ydbriS.o

SRCS =	    ydim.c ydca.c ydch.c ydmt.c ydsp.c ydrt.c ydnm.c \
	    ydu.c ydq.c \
	    yd.c ydmtd.c ydnmd.c ydls.c ydcf.c ydspps.c ydadm.c ydbr.c ydbriS.c

BINS  = $(BIN)/mnorbsrv $(BIN)/mnorbmet $(BIN)/mnnmsrv \
	$(BIN)/mnorbls  $(BIN)/mnorbctl $(BIN)/mnorbadm \
        $(BIN)/mnprocstat

MSGDIR	= $(SRC)/mesg

MSGS	=   $(MSGDIR)/OMNYD.mot \
	    $(MSGDIR)/OMNYDIM.mot \
	    $(MSGDIR)/OMNYDRT.mot \
	    $(MSGDIR)/OMNYDCA.mot \
	    $(MSGDIR)/OMNYDSP.mot \
	    $(MSGDIR)/OMNYDMT.mot \
	    $(MSGDIR)/OMNYDUT.mot \
	    $(MSGDIR)/OMNYDBR.mot

IDLHDRS =   $(YDSRC)/ydidl.h $(YDSRC)/yoidl.h \
	    $(YDSRC)/ydyoidl.h $(YDSRC)/ydmtdidl.h \
            $(YDSRC)/ydnmidl.h $(YDSRC)/yostd.h $(YDSRC)/ydbri.h

MSGDIR = $(SRC)/mesg

MNIDLC_FLAGS =  -a coa -I ../../pub -o $(YDSRC) -s oracle \
		-R "ys.log.msg-path=$(MSGDIR)"

idlhdrs	: $(IDLHDRS)

olint	:
	$(OLINT) $(INCLUDE) $(SRCS)

flint:
	$(OLINT) $(INCLUDE) $(FILE)

compile  : idlhdrs $(OBJS)

lib	 : idlhdrs $(LIBS)

share    : $(SHLIBS)

link	 : $(BINS) $(MSGS)

resource : $(MSGS)

cleanobj :
	rm -f $(LIBOBJS) $(OBJS) $(YDBROBJS)

cleanlib :
	rm -f $(LIBS) $(SHLIBS)

cleanbin :
	rm -f $(BINS) $(BIN)/mniiopbrsrv

$(LIBYD) : $(LIBYD)($(LIBYDOBJS))
	$(RANLIB) $@

$(LIBYDC) : $(LIBYDC)($(LIBYDCOBJS))
	$(RANLIB) $@

$(LIBYDS) : $(LIBYDS)($(LIBYDSOBJS))
	$(RANLIB) $@

$(OBJ)/%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

# rules for programs having different names than their main.o

$(BIN)/mniiopbrsrv: $(OBJ)/ydbr.o $(OBJ)/ydbriS.o 
	$(LINK.o) $(OBJ)/ydbriS.o $< $(LIBHOME) $(LOADLIBES) \
            $(CRYPTOLIBS) $(TNSLIBS) $(LDLIBS) -o $@

$(BIN)/mnorbsrv: $(OBJ)/yd.o $(YDSLIB) $(YDLIB)
	$(LINK.o) $< $(LIBHOME) -lydS -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnobjadm: $(OBJ)/ydchadm.o $(YDSLIB) $(YDLIB)
	$(LINK.o) $< $(LIBHOME) -lydS -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnorbmet: $(OBJ)/ydmtd.o $(YDSLIB) $(YDLIB)
	$(LINK.o) $< $(LIBHOME) -lydS -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnnmsrv: $(OBJ)/ydnmd.o $(YDSLIB) $(YDLIB)
	$(LINK.o) $< $(LIBHOME) -lydS -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnorbls: $(OBJ)/ydls.o $(LIBYDC) $(LIBYD)
	$(LINK.o) $< $(LIBHOME) -lydC -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnorbadm: $(OBJ)/ydadm.o $(LIBYDC) $(LIBYD)
	$(LINK.o) $< $(LIBHOME) -lydC -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnprocstat: $(OBJ)/ydspps.o $(LIBYDC) $(LIBYD)
	$(LINK.o) $< $(LIBHOME) -lydC -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnorbctl: $(OBJ)/ydcf.o $(LIBYDC) $(LIBYD)
	$(LINK.o) $< $(LIBHOME) -lydC -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

# default rule for debug programs having same name as main.o
$(BIN)/%: $(OBJ)/%.o $(LIBYDC) $(LIBYD)
	$(LINK.o) $< $(LIBHOME) -lydC -lyd $(LOADLIBES) \
	    $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BINS): $(LOADLIBES)

$(YDSRC)/yoidl.h $(YDSRC)/yoidlS.c $(YDSRC)/yoidlC.c: $(IDLSRC)/yoidl.idl
	$(YCIDL) $(MNIDLC_FLAGS) -S $<
clean::
	rm -f yoidl.h yoidl?.[ch]

$(YDSRC)/ydidl.h $(YDSRC)/ydidlS.c $(YDSRC)/ydidlC.c: $(IDLSRC)/ydidl.idl
	$(YCIDL) $(MNIDLC_FLAGS) -S $<
clean::
	rm -f ydidl.h ydidl?.[ch]

$(YDSRC)/ydyoidl.h $(YDSRC)/ydyoidlS.c $(YDSRC)/ydyoidlC.c: \
	$(IDLSRC)/ydyoidl.idl
	$(YCIDL) $(MNIDLC_FLAGS) -S $<
clean::
	rm -f ydyoidl.h ydyoidl?.[ch]

$(YDSRC)/ydmtdidl.h $(YDSRC)/ydmtdidlS.c $(YDSRC)/ydmtdidlC.c: \
	$(IDLSRC)/ydmtdidl.idl
	$(YCIDL) $(MNIDLC_FLAGS) -S $<
clean::
	rm -f ydmtdidl.h ydmtdidl?.[ch]


$(YDSRC)/ydnmidl.h $(YDSRC)/ydnmidlS.c $(YDSRC)/ydnmidlC.c: \
	$(SRCHOME)/mx/pub/ydnmidl.idl
	$(YCIDL) $(MNIDLC_FLAGS) -S $<
clean::
	rm -f ydnmidl.h ydnmidl?.[ch]


$(YDSRC)/yostd.h $(YDSRC)/yostdS.c $(YDSRC)/yostdC.c: \
	$(SRCHOME)/mx/pub/yostd.idl
	$(YCIDL) $(MNIDLC_FLAGS) -S $<
clean::
	rm -f yostd.h yostd?.[ch]

$(YDSRC)/ydbri.h $(YDSRC)/ydbriS.c $(YDSRC)/ydbriC.c: \
	$(IDLSRC)/ydbri.idl
	$(YCIDL) $(MNIDLC_FLAGS) -S $<
clean::
	rm -f ydbri.h ydbri?.[ch]

.PHONY: mniiopbrsrv

mniiopbrsrv: $(BIN)/mniiopbrsrv
