

AS=armasm
MODLIBS=	../ssllib ../cryptolib
MODOBJS=	cmhg.o tabserv.o rosslmod.o
PROGRAM=        ../bin/sslmodule,ffa
STUBS=          ../lib/sslstubs,ffd

all:		$(PROGRAM) $(STUBS)

tabclient.o:	tabclient.s tabdata.s
		$(AS) -o tabclient.o tabclient.s


tabserv.o:	tabserv.s tabdata.s
		$(AS) -o tabserv.o tabserv.s

rosslmod.o:	rosslmod.c
		$(CC) $(CFLAGS) -c -o rosslmod.o rosslmod.c


$(STUBS): tabclient.o
		armlib -c $(STUBS)
		armlib -i -o $(STUBS) tabclient.o

tabdata.s:	ssl-fns
		./mktabdata

$(PROGRAM):	$(MODOBJS) $(MODLIBS)
		armlink -rmf -o $(PROGRAM) $(MODOBJS) $(MODLIBS) \
		-map -xref -list ../bin/mapmodule \
		-symbols ../bin/symmodule \
		/riscos/lib/stubs.o /riscos/lib/unixlib.o \
		/riscos/lib/inetlib.o /riscos/lib/socklib.o

cmhg.o:		cmhg-in
		cmhg cmhg-in cmhg.o

clean:
		rm -f $(MODOJBS) $(PROGRAM) $(STUBS) *.o
