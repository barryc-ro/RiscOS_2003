#
##########################################################################
# Makefile for the IDL compiler, mnidlc
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
##########################################################################

ifneq ($(CLEARCASE_ROOT),)
   SRCHOME        = /vobs
else
   t              = $(subst /, ,$(shell pwd))
   SRCHOME        = /$(word 1,$t)/$(word 2,$t)/$(word 3,$t)
endif
SRC	  = $(SRCHOME)/mx/src

include $(SRC)/buildtools/defaults.mk

#####
# SLAX settings for ycosyn.y
#   Except for the slax binary, all the slax components used by mnidlc
#   are now a part of the yc source directory. To build, you need
#   only link /usr/local/slax to /vobs/omsdprod/$(ARCH)/oracle_lang/slax.
#   The link is needed so that newsx can find it's template files.
#####
SX        = /vobs/omsdprod/bin/$(ARCH)/newsx
SLAX_DEBUG=

INCLUDE	  = -I$(SRCHOME)/mx/pub \
            -I$(SRCHOME)/mx/inc \
            -I$(SRCHOME)/mx/src/inc \
            -I$(SRCHOME)/mx/src/ys/sosd/$(ARCH) \
            -I$(SRCHOME)/mx/src/mt/sosd/$(ARCH) \
            -I$(SRCHOME)/mx/src/mn \
            -I$(SRCHOME)/mx/src/mt \
            -I$(SRCHOME)/mx/src/yc

COMPONENT = YC

YCSRC	  = $(SRC)/yc
YCOSD     = $(YCSRC)/sosd/$(ARCH)

$(SRCHOME)/mx/src/mesg/%.mot : $(YCSRC)/mesg/%.msg
	$(MSGC) -I $(SRCHOME)/mx/src/mesg -o $@ $<

vpath %.c      $(YCOSD) $(YCSRC)
vpath %.a      $(LIB)
vpath %.$(SHX) $(LIB)

#####
# libycfe.a - the compiler front end. Parsing and semantic analysis
#####
LIBYCFE     = $(LIB)/libycfe.a
LIBYCFEOBJS = ycosyn.o ycosax.o ycpp.o ycppdir.o ycppexp.o \
              ycppget.o ycppif.o ycrec.o \
              ycprs.o ycsem.o ycsemty.o ycsemco.o \
              ycsemif.o ycifr.o pxslax.o ycprfree.o ycsemprg.o ycutil.o \
              ycsym.o
#####
# libycc.a - the C language mapping code generator.
#####
LIBYCC      = $(LIB)/libycc.a
LIBYCCOBJS  = yccmap.o yccsrv.o yccclnt.o ycctmpl.o yccname.o ycchdr.o \
              yccfp.o

#####
# libyccp.a - the C++ language mapping code generator.
#####
LIBYCCP     = $(LIB)/libyccp.a
#LIBYCCPOBJS = yccpmap.o yccpsrv.o yccpclnt.o yccptmpl.o yccphdr.o
LIBYCCPOBJS = yccpmap.o yccphdr.o yccpname.o yccptvar.o yccpclnt.o

#####
# libyci.a - the ITL language mapping code generator.
#####
LIBYCI      = $(LIB)/libyci.a
LIBYCIOBJS  = ycimap.o ycihdr.o yciclnt.o 

LIBS	    = $(LIBYCC) $(LIBYCCP) $(LIBYCFE) $(LIBYCI)

ifdef SHARE
SHLIBS      = $(LIB)/libycc.$(SHX) \
              $(LIB)/libyccp.$(SHX) \
              $(LIB)/libyci.$(SHX) \
              $(LIB)/libycfe.$(SHX)
endif

YCLIBS      = -lycc -lyccp -lycfe -lyci

LIBHOME     = -L$(LIB)

LOADLIBES   = -lyr -lyotk -lys

LIBOBJS     = $(LIBYCFEOBJS) $(LIBYCCOBJS) $(LIBYCCPOBJS) $(LIBYCIOBJS)

OBJS        = $(OBJ)/ycmain.o $(OBJ)/ycysmain.o

BINS        = $(BIN)/mnidlc

MSGS        = $(SRC)/mesg/OMNIDLC.mot

compile  : $(OBJS)

lib	 : $(LIBS)

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

$(LIBYCFE) : $(LIBYCFE)($(LIBYCFEOBJS))
	$(RANLIB) $@

$(LIBYCC) : $(LIBYCC)($(LIBYCCOBJS))
	$(RANLIB) $@

$(LIBYCCP) : $(LIBYCCP)($(LIBYCCPOBJS))
	$(RANLIB) $@

$(LIBYCI) : $(LIBYCI)($(LIBYCIOBJS))
	$(RANLIB) $@

$(OBJ)/%.o: %.c
	$(COMPILE.c) $(YCDEBUG) $(YCITL) $< $(OUTPUT_OPTION)

$(BIN)/%: $(OBJ)/%.o
	$(LINK.o) $+ $(LDLIBS) -o $@

$(BIN)/mnidlc    : $(OBJS) $(LIBS) $(SHLIBS)
	$(LINK.o) $(OBJS) $(LIBHOME) $(YCLIBS) $(LOADLIBES) $(LDLIBS) -o $@

$(BINS): $(LOADLIBES)

$(OBJ)/ycysmain.o : $(SRC)/ys/sosd/$(ARCH)/s0ysmain.c
	$(COMPILE.c) -DENTRY_POINT=ycMain $< $(OUTPUT_OPTION)

$(YCSRC)/yc.h : $(YCSRC)/ycotok.h $(YCSRC)/ycosyn.h $(YCSRC)/ycotre.h

$(YCSRC)/ycprs.c : $(YCSRC)/yc.h

ycosax.c ycosyn.c ycosyn.h ycotok.h: \
	ycosyn.y
	cd $(YCSRC); $(SX) -d ycosyn.y; mv -f slax.h ycotok.h; \
	rm -f ycosyn.pdl ycomsg.msg; \
	cat ycosyn.h | sed -e '/ word/s/ word/ sword/' -e '/VAFP/s/))/)/' -e '/VAFP/s/VAFP((pgstrm \*filobj, /(/' -e `cat ycosyn.h | awk '/PGSTRM/ { printf "%d,%dd", NR, NR+2 }'` > ycotmp.h ; \
	cat ycosyn.c | sed -e '/ word/s/ word/ sword/' -e '/VAFP/s/))/)/' -e '/VAFP/s/VAFP((pgstrm \*filobj, /(/' -e '/s\.h/s/s\.h/sysx\.h/' -e 's/typedef ub4 PXVALTYPE;/\#include <ycosyn0.h>/'  -e '/\/\*_/s/\/\*_//' -e '/_\*\//s/_\*\///' -e '/CONST_DATA pxtokn/s/CONST_DATA //' -e '/CONST_DATA void/s/CONST_DATA //' -e '/pxprod \*/s/pxprod \*/ub2 /' -e '/pgstrm/s/pgstrm \*,//'> ycotmp.c ; \
	mv -f ycotmp.h ycosyn.h; \
	mv -f ycotmp.c ycosyn.c

