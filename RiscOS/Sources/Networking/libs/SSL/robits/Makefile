#
# SSLeay/robits/Makefile
#

DIR=	robits
TOP=	..
CC=	cc
INCLUDES= -I.. -I../include
CFLAG=-g
INSTALLTOP=/usr/local/ssl
MAKE=		make -f Makefile.ssl
MAKEDEPEND=	makedepend -fMakefile.ssl
MAKEFILE=	Makefile.ssl

CFLAGS= $(INCLUDES) $(CFLAG)

ERR=robits
ERRC=robits_err
GENERAL=Makefile README
TEST=
APPS=

LIB=$(TOP)/cryptolib
# LIBSRC=	dumbfs.c envconf.c
# LIBOBJ= dumbfs.o envconf.o
LIBSRC=	robits.c
LIBOBJ= robits.o

SRC= $(LIBSRC)

EXHEADER= robits.h
HEADER=	$(EXHEADER)

ALL=    $(GENERAL) $(SRC) $(HEADER)

top:
	(cd ..; $(MAKE) DIRS=$(DIR))

all:	lib

lib:	$(LIBOBJ)
	armlib -i -o $(LIB) $(LIBOBJ)
#	$(TOP)/util/ranlib.sh $(LIB)
	@touch lib

links:
	/bin/rm -f Makefile
	$(TOP)/util/point.sh Makefile.ssl Makefile
	$(TOP)/util/mklink.sh ../../include $(EXHEADER)
	$(TOP)/util/mklink.sh ../../test $(TEST)
	$(TOP)/util/mklink.sh ../../apps $(APPS)

install:
	@for i in $(EXHEADER) ; \
	do  \
	(cp $$i $(INSTALLTOP)/include/$$i; \
	chmod 644 $(INSTALLTOP)/include/$$i ) \
	done;

tags:
	ctags $(SRC)

tests:

lint:
	lint -DLINT $(INCLUDES) $(SRC)>fluff

depend:
	$(MAKEDEPEND) $(INCLUDES) $(PROGS) $(LIBSRC)

dclean:
	sed -e '/^# DO NOT DELETE THIS LINE/ q' $(MAKEFILE) >Makefile.new
	mv -f Makefile.new $(MAKEFILE)

clean:
	/bin/rm -f *.o *.obj lib tags core .nfs* *.old *.bak fluff *.lib

# DO NOT DELETE THIS LINE -- make depend depends on it.
