#
# SSLeay/crypto/Makefile
#

DIR=		crypto
TOP=		..
CC=		cc
INCLUDE=	-I. -I../include
INCLUDES=	-I.. -I../../include
CFLAG=		-g
INSTALLTOP=	/usr/local/ssl
MAKE=           make -f Makefile.ssl
MAKEDEPEND=     makedepend -f Makefile.ssl
MAKEFILE=       Makefile.ssl
RM=             /bin/rm -f
AR=		ar r

MAKE=		make -f Makefile.ssl
MAKEDEPEND=	makedepend -f Makefile.ssl
MAKEFILE=	Makefile.ssl

PEX_LIBS=
EX_LIBS=
 
CFLAGS= $(INCLUDE) $(CFLAG) -DCFLAGS=" \"$(CC) $(CFLAG)\" "

LIBS=

SDIRS=	md sha mdc2 \
	des rc4 rc2 idea bf \
	bn rsa dsa dh \
	buffer bio stack lhash rand err objects \
	evp pem x509 \
	asn1 conf txt_db pkcs7

GENERAL=Makefile README

LIB= $(TOP)/libcrypto.a
LIBSRC=	cryptlib.c mem.c cversion.c
LIBOBJ= cryptlib.o mem.o cversion.o

SRC= $(LIBSRC)

EXHEADER= crypto.h cryptall.h
HEADER=	cryptlib.h date.h $(EXHEADER)

ALL=    $(GENERAL) $(SRC) $(HEADER)

top:
	@(cd ..; $(MAKE) DIRS=$(DIR) all)

all: date.h lib subdirs

date.h: ../Makefile.ssl ../VERSION
	echo "#define DATE	\"`date`\"" >date.h

subdirs:
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making all in $$i..."; \
	$(MAKE) CC='$(CC)' INCLUDES='${INCLUDES}' CFLAG='${CFLAG}' INSTALLTOP='${INSTALLTOP}' PEX_LIBS='${PEX_LIBS}' EX_LIBS='${EX_LIBS}' BN_MULW='${BN_MULW}' DES_ENC='${DES_ENC}' BF_ENC='${BF_ENC}' AR='${AR}' all ); \
	done;

files:
	perl $(TOP)/util/files.pl Makefile.ssl >> $(TOP)/MINFO
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making 'files' in $$i..."; \
	$(MAKE) files ); \
	done;

links:
	/bin/rm -f Makefile 
	$(TOP)/util/point.sh Makefile.ssl Makefile ;
	$(TOP)/util/mklink.sh ../include $(EXHEADER) ;
	$(TOP)/util/mklink.sh ../test $(TEST) ;
	$(TOP)/util/mklink.sh ../apps $(APPS) ;
	$(TOP)/util/point.sh Makefile.ssl Makefile;
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making links in $$i..."; \
	$(MAKE) links ); \
	done;

lib:	$(LIBOBJ)
	$(AR) $(LIB) $(LIBOBJ)
	sh $(TOP)/util/ranlib.sh $(LIB)
	@touch lib

libs:
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making libs in $$i..."; \
	$(MAKE) CC='$(CC)' CFLAG='${CFLAG}' INSTALLTOP='${INSTALLTOP}' PEX_LIBS='${PEX_LIBS}' EX_LIBS='${EX_LIBS}' AR='${AR}' lib ); \
	done;

tests:
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making tests in $$i..."; \
	$(MAKE) CC='$(CC)' CFLAG='${CFLAG}' INSTALLTOP='${INSTALLTOP}' PEX_LIBS='${PEX_LIBS}' EX_LIBS='${EX_LIBS}' AR='${AR}' tests ); \
	done;

install:
	@for i in $(EXHEADER) ;\
	do \
	(cp $$i $(INSTALLTOP)/include/$$i; \
	chmod 644 $(INSTALLTOP)/include/$$i ); \
	done;
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making install in $$i..."; \
	$(MAKE) CC='$(CC)' CFLAG='${CFLAG}' INSTALLTOP='${INSTALLTOP}' PEX_LIBS='${PEX_LIBS}' EX_LIBS='${EX_LIBS}' install ); \
	done;

lint:
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making lint in $$i..."; \
	$(MAKE) CC='$(CC)' CFLAG='${CFLAG}' INSTALLTOP='${INSTALLTOP}' PEX_LIBS='${PEX_LIBS}' EX_LIBS='${EX_LIBS}' lint ); \
	done;

depend:
	$(MAKEDEPEND) $(INCLUDE) $(PROGS) $(LIBSRC)
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making depend in $$i..."; \
	$(MAKE) MAKEFILE='${MAKEFILE}' INCLUDES='${INCLUDES}' MAKEDEPEND='${MAKEDEPEND}' depend ); \
	done;

clean:
	/bin/rm -f *.o */*.o *.obj lib tags core .pure .nfs* *.old *.bak fluff
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making clean in $$i..."; \
	$(MAKE) CC='$(CC)' CFLAG='${CFLAG}' INSTALLTOP='${INSTALLTOP}' PEX_LIBS='${PEX_LIBS}' EX_LIBS='${EX_LIBS}' clean ); \
	done;

dclean:
	perl -pe 'if (/^# DO NOT DELETE THIS LINE/) {print; exit(0);}' $(MAKEFILE) >Makefile.new
	mv -f Makefile.new $(MAKEFILE)
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making dclean in $$i..."; \
	$(MAKE) CC='$(CC)' CFLAG='${CFLAG}' INSTALLTOP='${INSTALLTOP}' PEX_LIBS='${PEX_LIBS}' EX_LIBS='${EX_LIBS}' dclean ); \
	done;

errors:
	perl ./err/err_code.pl */*.c ../ssl/*.c ../rsaref/*.c
	@for i in $(SDIRS) ;\
	do \
	(cd $$i; echo "making errors in $$i..."; \
	$(MAKE) errors ); \
	done;

# DO NOT DELETE THIS LINE -- make depend depends on it.
