#
# SSLeay/ssl/Makefile
#

DIR=	ssl
TOP=	..
CC=	cc
INCLUDES= -I../crypto -I../include
CFLAG=-g
INSTALLTOP=/usr/local/ssl
MAKE=		make -f Makefile.ssl
MAKEDEPEND=	makedepend -fMakefile.ssl
MAKEFILE=	Makefile.ssl

CFLAGS= $(INCLUDES) $(CFLAG)

ERR=ssl
ERRC=ssl_err
GENERAL=Makefile README
TEST=
APPS=

LIB=$(TOP)/libssl.a
LIBSRC=	ssl_lib.c ssl_err2.c ssl_trc.c ssl_cert.c ssl_sess.c \
	ssl_srvr.c ssl_clnt.c ssl_pkt.c ssl_auth.c ssl_stat.c \
	ssl_asn1.c ssl_txt.c \
	ssl_rsa.c ssl_null.c ssl_des.c ssl_idea.c ssl_rc4.c \
	ssl_rc2.c bio_ssl.c \
	ssl_md5.c ssl_sha.c $(ERRC).c
LIBOBJ= ssl_lib.o ssl_err2.o ssl_trc.o ssl_cert.o ssl_sess.o \
	ssl_srvr.o ssl_clnt.o ssl_pkt.o  ssl_auth.o ssl_stat.o \
	ssl_asn1.o ssl_txt.o \
	ssl_rsa.o ssl_null.o ssl_des.o ssl_idea.o ssl_rc4.o \
	ssl_rc2.o bio_ssl.o \
	ssl_md5.o ssl_sha.o $(ERRC).o

SRC= $(LIBSRC)

EXHEADER= ssl.h
HEADER=	$(EXHEADER) ssl_locl.h ssl_trc.h \
	ssl_rsa.h ssl_null.h ssl_md5.h ssl_sha.h ssl_des.h ssl_idea.h \
	ssl_rc4.h ssl_rc2.h

ALL=    $(GENERAL) $(SRC) $(HEADER)

top:
	(cd ..; $(MAKE) DIRS=$(DIR) all)

all:	lib

lib:	$(LIBOBJ)
	armlib -c -o $(TOP)/ssllib $(LIBOBJ)
#	$(TOP)/util/ranlib.sh $(LIB)
	@touch lib

files:
	perl $(TOP)/util/files.pl Makefile.ssl >> $(TOP)/MINFO

links:
	/bin/rm -f Makefile
	$(TOP)/util/point.sh Makefile.ssl Makefile ;
	$(TOP)/util/mklink.sh ../include $(EXHEADER)
	$(TOP)/util/mklink.sh ../test $(TEST)
	$(TOP)/util/mklink.sh ../apps $(APPS)

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
	perl -pe 'if (/^# DO NOT DELETE THIS LINE/) {print; exit(0);}' $(MAKEFILE) >Makefile.new
	mv -f Makefile.new $(MAKEFILE)

clean:
	/bin/rm -f *.o *.obj lib tags core .nfs* *.old *.bak fluff

errors:
	perl $(TOP)/util/err-ins.pl $(ERR).err $(ERR).h
	perl ../crypto/error/err_genc.pl $(ERR).h $(ERRC).c >$(ERRC).c

# DO NOT DELETE THIS LINE -- make depend depends on it.
