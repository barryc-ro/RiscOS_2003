#
# test/Makefile.ssl
#

DIR=		test
TOP=		..
CC=		cc
INCLUDES=	-I../include
CFLAG=		-g
INSTALLTOP=	/usr/local/ssl
MAKEFILE=	Makefile.ssl
MAKE=		make -f $(MAKEFILE)
MAKEDEPEND=	makedepend -f$(MAKEFILE)

PEX_LIBS=
EX_LIBS= -lnsl -lsocket

CFLAGS= $(INCLUDES) $(CFLAG)

GENERAL=Makefile.ssl

LIBCRYPTO= ../libcrypto.a
LIBSSL= -lssl

BNTEST=		bntest
IDEATEST=	ideatest
SHATEST=	shatest
SHA1TEST=	sha1test
MD2TEST=	md2test
MD5TEST=	md5test
RC4TEST=	rc4test
RC2TEST=	rc2test
DESTEST=	destest
RANDTEST=	randtest
DHTEST=		dhtest
METHTEST=	methtest

EXE=	$(BNTEST) $(IDEATEST) $(MD2TEST)  $(MD5TEST)  $(RC4TEST) \
	$(DESTEST) $(SHATEST) $(SHA1TEST) $(RANDTEST) $(DHTEST) \
	$(METHTEST) $(RC2TEST)

OBJ=	$(BNTEST).o $(IDEATEST).o $(MD2TEST).o  $(MD5TEST).o  $(RC4TEST).o \
	$(DESTEST).o $(SHATEST).o $(SHA1TEST).o $(RANDTEST).o $(DHTEST).o \
	$(METHTEST).o $(RC2TEST).o
SRC=	$(BNTEST).c $(IDEATEST).c $(MD2TEST).c  $(MD5TEST).c  $(RC4TEST).c \
	$(DESTEST).c $(SHATEST).c $(SHA1TEST).c $(RANDTEST).c $(DHTEST).c \
	$(METHTEST).c $(RC2TEST).c

EXHEADER= 
HEADER=	$(EXHEADER)

ALL=    $(GENERAL) $(SRC) $(HEADER)

top:
	(cd ..; $(MAKE) DIRS=$(DIR) all)

all:	exe

exe:	$(EXE)

files:
	perl $(TOP)/util/files.pl Makefile.ssl >> $(TOP)/MINFO

links:
	/bin/rm -f Makefile
	$(TOP)/util/point.sh Makefile.ssl Makefile ;

errors:

install:

tags:
	ctags $(SRC)

tests:	exe
	@(cd ../apps; $(MAKE)  CC='${CC}' CFLAG='${CFLAG}' INSTALLTOP='${INSTALLTOP}' PEX_LIBS='${PEX_LIBS}' EX_LIBS='${EX_LIBS}' all)
	./$(DESTEST)
	./$(IDEATEST)
	./$(SHATEST)
	./$(SHA1TEST)
	./$(MD5TEST)
	./$(MD2TEST)
	./$(RC2TEST)
	./$(RC4TEST)
	./$(RANDTEST)
	@sh ./testenc
	@sh ./tx509 2>/dev/null
	@sh ./trsa 2>/dev/null
	@sh ./tcrl 2>/dev/null
	@sh ./treq 2>/dev/null
	@echo starting big number library test, could take a while...
	@./$(BNTEST)|bc| awk '{ \
if ($$0 != "0") {print "error"; exit(1); } \
if (((NR+1)%64) == 0) print NR+1," tests done"; }'
	@echo "The following command should have some OK's and some failures"
	../apps/ssleay verify -CApath ../certs ../certs/*.pem
	@echo "Generate and certify a test certificate"
	./$(DHTEST)
	@sh ./testgen
	@sh ./testss

lint:
	lint -DLINT $(INCLUDES) $(SRC)>fluff

depend:
	$(MAKEDEPEND) $(INCLUDES) $(PROGS) $(SRC)

dclean:
	perl -pe 'if (/^# DO NOT DELETE THIS LINE/) {print; exit(0);}' $(MAKEFILE) >Makefile.new
	mv -f Makefile.new $(MAKEFILE)

clean:
	/bin/rm -f *.o *.obj lib tags core .nfs* *.old *.bak fluff $(EXE) *.ss

$(LIBCRYPTO):

$(BNTEST): $(BNTEST).o $(LIBCRYPTO)
	$(CC) -o $(BNTEST) $(CFLAGS) $(BNTEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(IDEATEST): $(IDEATEST).o $(LIBCRYPTO)
	$(CC) -o $(IDEATEST) $(CFLAGS) $(IDEATEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(MD2TEST): $(MD2TEST).o $(LIBCRYPTO)
	$(CC) -o $(MD2TEST) $(CFLAGS) $(MD2TEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(SHATEST): $(SHATEST).o $(LIBCRYPTO)
	$(CC) -o $(SHATEST) $(CFLAGS) $(SHATEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(SHA1TEST): $(SHA1TEST).o $(LIBCRYPTO)
	$(CC) -o $(SHA1TEST) $(CFLAGS) $(SHA1TEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(MD5TEST): $(MD5TEST).o $(LIBCRYPTO)
	$(CC) -o $(MD5TEST) $(CFLAGS) $(MD5TEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(RC2TEST): $(RC2TEST).o $(LIBCRYPTO)
	$(CC) -o $(RC2TEST) $(CFLAGS) $(RC2TEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(RC4TEST): $(RC4TEST).o $(LIBCRYPTO)
	$(CC) -o $(RC4TEST) $(CFLAGS) $(RC4TEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(DESTEST): $(DESTEST).o $(LIBCRYPTO)
	$(CC) -o $(DESTEST) $(CFLAGS) $(DESTEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(RANDTEST): $(RANDTEST).o $(LIBCRYPTO)
	$(CC) -o $(RANDTEST) $(CFLAGS) $(RANDTEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(DHTEST): $(DHTEST).o $(LIBCRYPTO)
	$(CC) -o $(DHTEST) $(CFLAGS) $(DHTEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

$(METHTEST): $(METHTEST).o $(LIBCRYPTO)
	$(CC) -o $(METHTEST) $(CFLAGS) $(METHTEST).o $(PEX_LIBS) $(LIBCRYPTO) $(EX_LIBS)

# DO NOT DELETE THIS LINE -- make depend depends on it.
