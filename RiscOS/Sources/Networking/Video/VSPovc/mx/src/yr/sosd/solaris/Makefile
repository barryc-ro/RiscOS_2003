#
# Makefile for ../mx/src/yr
#
# A note on shared library builds: Switching back and forth between
# shared and unshared is not simply a matter of enabling SHARE. You should
# do a completely clean build when you transition. On Solaris for example,
# the shared libs are derived from the .o's in the static libraries - which
# are only built as PIC objects if SHARE is defined. Similarly, the shared
# libraries are only removed by cleanlib if SHARE is defined. In short:
#
# To switch from unshared builds to shared:
#    % make SHARE=1 clean all
# To switch from unshared to shared:
#    % make SHARE=1 clean
#    % make
#

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
            -I$(SRCHOME)/mx/src/yr \
            -I$(SRCHOME)/mx/src/ys/sosd/$(ARCH) \
            -I$(SRCHOME)/omsdev/osd/$(ARCH)/dprod/core/ship \
            -I$(SRCHOME)/mx/src/mn \
            -I$(SRCHOME)/mx/src/mn/sosd/$(ARCH) 

COMPONENT = Interface Repository

IDLSRC      = $(SRCHOME)/mx/pub
YCIDL_FLAGS = -I $(IDLSRC) -a coa -s oracle -S \
              -R "ys.log.msg-path=$(MSGDIR)" \
              -R "mnidlc.c-no-async=true" \
              -R "mnidlc.tmpl-hdr-only=true"

YRSRC	    = $(SRC)/yr
YROSD       = $(YRSRC)/sosd/$(ARCH)

$(SRCHOME)/mx/src/mesg/%.mot : $(YRSRC)/mesg/%.msg
	$(MSGC) -I $(SRCHOME)/mx/src/mesg -o $@ $<

vpath %.c      $(YROSD) $(YRSRC)
vpath %.a      $(LIB)
vpath %.idl    $(SRCHOME)/mx/src/inc
vpath %.$(SHX) $(LIB)

LIBYR	    = $(LIB)/libyr.a
LIBYRSV     = $(LIB)/libyrsv.a
LIBYRM      = $(LIB)/libyrm.a
LIBS	    = $(LIBYR) $(LIBYRSV) $(LIBYRM)

ifdef SHARE
SHLIBYR     = $(LIB)/libyr.$(SHX)
SHLIBYRSV   = $(LIB)/libyrsv.$(SHX)
SHLIBYRM    = $(LIB)/libyrm.$(SHX)
SHLIBS      = $(SHLIBYR) $(SHLIBYRSV) $(SHLIBYRM)
endif

YRSVLIBS    =  -lyrsv -lyr
###
# Note that we don't really need libyr for the client, but the shared
# link won't work right if you don't have it - somehow an interdependency 
# between libyotk and libyr isn't resolved in the shared lib case without
# without putting libyr on the link line again.
###
ifdef SHARE
YRCLLIBS    = -lyrm -lyr
else
YRCLLIBS    = -lyrm
endif


LOADLIBES   = -lyeS -lyo -lyotk -lyr -lys -lmn -lmt -lmn -lys
LIBHOME     = -L$(LIB)

LIBYROBJS   = yr.o yrc.o yrdst.o yrtk.o yrdump.o yrwrite.o

LIBYRSVOBJS =  $(OBJ)/yralias.o \
               $(OBJ)/yrarray.o \
               $(OBJ)/yrattr.o \
               $(OBJ)/yrconst.o \
               $(OBJ)/yrcontd.o \
               $(OBJ)/yrcontr.o \
               $(OBJ)/yrenum.o \
               $(OBJ)/yrexcept.o \
               $(OBJ)/yridltyp.o \
               $(OBJ)/yrintf.o \
               $(OBJ)/yrirobj.o \
               $(OBJ)/yrmodule.o \
               $(OBJ)/yroper.o \
               $(OBJ)/yrprim.o \
               $(OBJ)/yrrepos.o \
               $(OBJ)/yrseq.o \
               $(OBJ)/yrstring.o \
               $(OBJ)/yrstruct.o \
               $(OBJ)/yrtypdef.o \
               $(OBJ)/yrunion.o \
               $(OBJ)/yrsvutil.o \
               $(OBJ)/yrmgr.o \
               $(OBJ)/yrmgidlS.o \
               $(OBJ)/yrtypidlS.o \
               $(OBJ)/yrmerge.o \
               $(OBJ)/yrsrv.o \
               $(OBJ)/yrread.o \
               $(OBJ)/yrcorbaS.o \
               $(OBJ)/yrdiidlS.o \
               $(OBJ)/yrdii.o \
               $(OBJ)/yridef.o \
               $(OBJ)/yridefS.o


LIBYRMOBJS = $(OBJ)/yrmgidlC.o \
             $(OBJ)/yrtypidlC.o \
             $(OBJ)/yrcorbaC.o \
             $(OBJ)/yrmcl.o

LIBOBJS	= $(LIBYROBJS) $(LIBYRSVOBJS) $(LIBYRMOBJS)

YRSVOBJS  = $(OBJ)/yrsvmain.o 

YRCLOBJS  = $(OBJ)/yrcl.o $(OBJ)/s0yrcl.o

YRMOBJS = $(OBJ)/yrmmain.o

OBJS      = $(YRSVOBJS) $(YRMOBJS) $(YRCLOBJS)

BINS  = $(BIN)/mnirsrv $(BIN)/mnirctl $(BIN)/mnircl

MSGDIR = $(SRC)/mesg
MSGS	= $(MSGDIR)/OMNYR.mot

IDLHDRS = $(YRSRC)/yrcorba.h \
          $(YRSRC)/yrmgidl.h \
          $(YRSRC)/yrtypidl.h \
          $(YRSRC)/yrdiidl.h \
          $(YRSRC)/yridef.h

all      : compile lib link

libyr	 : $(LIBYR)

idlhdrs  : $(IDLHDRS)

compile  : $(OBJS)

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

clean::
	rm -f $(MSGS)

$(LIBYR) : $(LIBYR)($(LIBYROBJS))
	$(RANLIB) $@

$(LIBYRSV) : $(LIBYRSV)($(LIBYRSVOBJS))
	$(RANLIB) $@

$(LIBYRM) : $(LIBYRM)($(LIBYRMOBJS))
	$(RANLIB) $@

$(OBJ)/%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

$(BIN)/%: $(OBJ)/%.o
	$(LINK.o) $+ $(CRYPTOLIBS) $(LDLIBS) -o $@

$(BINS): $(LOADLIBES)

$(BIN)/mnirsrv : $(YRSVOBJS) $(LIBYR) $(LIBYRSV) $(SHLIBYR) $(SHLIBYRSV)
	$(LINK.o) $(YRSVOBJS) $(LIBHOME) $(YRSVLIBS) $(LOADLIBES) \
		$(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnircl : $(YRCLOBJS) $(LIBYRM) $(LIBYRSV) $(SHLIBYR) $(SHLIBYRSV)
	$(LINK.o) $(YRCLOBJS) $(LIBHOME) $(YRSVLIBS) $(LOADLIBES) \
		$(CRYPTOLIBS) $(LDLIBS) -o $@

$(BIN)/mnirctl : $(YRMOBJS) $(LIBYRM) $(SHLIBYRM)
	$(LINK.o) $(YRMOBJS) $(LIBHOME) $(YRCLLIBS) $(LOADLIBES) \
		$(CRYPTOLIBS) $(LDLIBS) -o $@

clean::

$(YRSRC)/yrcorba.h $(YRSRC)/yrcorbaS.c $(YRSRC)/yrcorbaC.c \
   $(YRSRC)/yrcorbaI.h: $(IDLSRC)/yrcorba.idl
	$(YCIDL) $(YCIDL_FLAGS) -o $(YRSRC) $<

cleanidl::
	rm -f $(YRSRC)/yrcorba.h $(YRSRC)/yrcorbaC.c $(YRSRC)/yrcorbaS.c \
		$(YRSRC)/yrcorbaI.h

$(YRSRC)/yrmgidl.h $(YRSRC)/yrmgidlS.c $(YRSRC)/yrmgidlC.c \
   $(YRSRC)/yrmgidlI.h: $(IDLSRC)/yrmgidl.idl
	$(YCIDL) $(YCIDL_FLAGS) -o $(YRSRC) $<

cleanidl::
	rm -f $(YRSRC)/yrmgidl.h $(YRSRC)/yrmgidlS.c $(YRSRC)/yrmgidlC.c \
		$(YRSRC)/yrmgidlI.h

$(YRSRC)/yrtypidl.h $(YRSRC)/yrtypidlS.c $(YRSRC)/yrtypidlC.c \
   $(YRSRC)/yrtypidlI.h: $(IDLSRC)/yrtypidl.idl
	$(YCIDL) $(YCIDL_FLAGS) -o $(YRSRC) $<

cleanidl::
	rm -f $(YRSRC)/yrtypidl.h $(YRSRC)/yrtypidlS.c \
		$(YRSRC)/yrtypidlC.c $(YRSRC)/yrtypidlI.h

$(YRSRC)/yridef.h $(YRSRC)/yridefS.c $(YRSRC)/yridefC.c $(YRSRC)/yridefI.h: \
   yridef.idl
	$(YCIDL) $(YCIDL_FLAGS) -o $(YRSRC) $<

cleanidl::
	rm -f $(YRSRC)/yridef.h $(YRSRC)/yridefS.c  \
		$(YRSRC)/yridefC.c $(YRSRC)/yridefI.h

$(OBJ)/s0yrcl.o : $(SRC)/ys/sosd/$(ARCH)/s0ysmain.c
	$(COMPILE.c) -DENTRY_POINT=yrclMain $< $(OUTPUT_OPTION)

$(OBJ)/yrsvmain.o : $(SRC)/ys/sosd/$(ARCH)/s0ysmain.c
	$(COMPILE.c) -DENTRY_POINT=yrsrvMain $< $(OUTPUT_OPTION)

$(OBJ)/yrmmain.o : $(SRC)/ys/sosd/$(ARCH)/s0ysmain.c
	$(COMPILE.c) -DENTRY_POINT=yrmMain $< $(OUTPUT_OPTION)

$(IDLSRC)/yrmgidl.idl : $(IDLSRC)/yrtypidl.idl

$(IDLSRC)/yrcorba.idl : $(IDLSRC)/yrtypidl.idl

$(YRSRC)/yrmgr.c : $(YRSRC)/yrmgidlI.h

$(YRSRC)/yralias.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrarray.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrattr.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrconst.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrcontd.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrcontr.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrenum.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrexcept.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yridltyp.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrintf.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrirobj.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrmodule.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yroper.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrprim.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrrepos.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrseq.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrstring.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrstruct.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrtypdef.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yrunion.c : $(YRSRC)/yrcorbaI.h

$(YRSRC)/yridef.c : $(YRSRC)/yridefI.h

$(YRSRC)/yrdiidl.h $(YRSRC)/yrdiidlS.c $(YRSRC)/yrdiidlC.c \
   $(YRSRC)/yrdiidlI.h: $(IDLSRC)/yrdiidl.idl
	$(YCIDL) $(YCIDL_FLAGS) -o $(YRSRC) $<
 
cleanidl::
	rm -f $(YRSRC)/yrdiidl.h $(YRSRC)/yrdiidlS.c \
		$(YRSRC)/yrdiidlC.c $(YRSRC)/yrdiidlI.h
 
$(YRSRC)/yrdii.c : $(YRSRC)/yrdiiI.h

