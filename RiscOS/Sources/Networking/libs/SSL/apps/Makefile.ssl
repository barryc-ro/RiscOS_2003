#
# SSLeay/apps/Makefile.ssl
#

DIR=		apps
TOP=		..
CC=		gcc
INCLUDES=	-I../include
CFLAG=		-g
INSTALLTOP=	/usr/local/ssl
MAKE=		make -f Makefile.ssl
MAKEDEPEND=	makedepend -fMakefile.ssl
MAKEFILE=	Makefile.ssl
RM=		/bin/rm -f

PEX_LIBS=
EX_LIBS= 

CFLAGS= -DMONOLITH $(INCLUDES) $(CFLAG)

GENERAL=Makefile

LIBCRYPTO=../libcrypto.a
LIBSSL=../libssl.a

SSLEAY= ssleay

SCRIPTS=CA.sh der_chop

EXE= $(SSLEAY)

E_EXE=	verify asn1pars req dgst dh enc gendh errstr ca \
	crl rsa x509 genrsa s_server s_client s_filter speed \
	hashdir s_time s_mult version pkcs7 crl2pkcs7

PROGS= $(SSLEAY).c

A_OBJ=apps.o
A_SRC=apps.c
S_OBJ=	s_cb.o s_socket.o s_eio.o
S_SRC=	s_cb.c s_socket.c s_eio.c

E_OBJ=	verify.o asn1pars.o req.o dgst.o dh.o enc.o gendh.o errstr.o ca.o \
	pkcs7.o crl2p7.o \
	crl.o rsa.o x509.o genrsa.o s_server.o s_client.o s_filter.o speed.o \
	hashdir.o s_time.o s_mult.o $(A_OBJ) $(S_OBJ) version.o

#	pem_mail.os 

E_SRC=	verify.c asn1pars.c req.c dgst.c dh.c enc.c gendh.c errstr.c ca.c \
	pkcs7.c crl2p7.c \
	crl.c rsa.c x509.c genrsa.c s_server.c s_client.c s_filter.c speed.c \
	hashdir.c s_time.c s_mult.c $(A_SRC) $(S_SRC) version.c

#	pem_mail.c 

SRC=$(E_SRC)

EXHEADER=
HEADER=	apps.h progs.h s_apps.h s_eio.h \
	testcert.h \
	$(EXHEADER)

ALL=    $(GENERAL) $(SRC) $(HEADER)

top:
	@(cd ..; $(MAKE) DIRS=$(DIR) all)

all:	exe

exe:	$(EXE)

files:
	perl $(TOP)/util/files.pl Makefile.ssl >> $(TOP)/MINFO

install: mklinks
	@for i in $(EXE) $(SCRIPTS) mklinks; \
	do  \
	(echo installing $$i; \
	 cp $$i $(INSTALLTOP)/bin/$$i; \
	 chmod 755 $(INSTALLTOP)/bin/$$i ) \
	 done; \
	cp ssleay.conf $(INSTALLTOP)/lib
	chmod 644 $(INSTALLTOP)/lib/ssleay.conf
	cd $(INSTALLTOP)/bin; \
	/bin/sh ./mklinks; \
	/bin/rm -f ./mklinks

tags:
	ctags $(SRC)

tests:

links:
	/bin/rm -f Makefile
	$(TOP)/util/point.sh Makefile.ssl Makefile ;

lint:
	lint -DLINT $(INCLUDES) $(SRC)>fluff

depend:
	$(MAKEDEPEND) $(INCLUDES) $(PROGS) $(SRC)

dclean:
	perl -pe 'if (/^# DO NOT DELETE THIS LINE/) {print; exit(0);}' $(MAKEFILE) >Makefile.new
	mv -f Makefile.new $(MAKEFILE)

errors:

clean:
	/bin/rm -f *.o *.obj lib tags core .nfs* *.old *.bak fluff $(EXE)

$(LIBSSL):
	(cd ../ssl; $(MAKE))

$(LIBCRYPTO):
	(cd ../crypto; $(MAKE))

$(SSLEAY): progs.h $(E_OBJ) $(SSLEAY).o $(LIBCRYPTO) $(LIBSSL)
	$(RM) $(SSLEAY)
	$(CC) -o $(SSLEAY) $(CFLAGS) $(SSLEAY).o $(E_OBJ) $(PEX_LIBS) $(LIBSSL) $(LIBCRYPTO) $(EX_LIBS)

progs.h: g_ssleay.pl Makefile.ssl
	perl ./g_ssleay.pl $(E_EXE) >progs.h

mklinks:
	perl ./g_ssleay.pl $(E_EXE) >progs.h

# DO NOT DELETE THIS LINE -- make depend depends on it.
