# URLclient
.SUFFIXES: .c .o

test: URLClient ParseURL
	@|

run: test
	@URLClient -test URLs

URLClient: URLclient.o
	link -o $@ URLclient.o c:o.stubs

ParseURL: parseurl.o generic.o
	link -o $@ parseurl.o generic.o TCPIPLibs:o.inetlib c:o.stubs

PathWrite: pathwrite.o
	link -o $@ pathwrite.o c:o.stubs	

speedtest: speedtest.c
	cc c.speedtest

parseurl.o: ^.c.parseurl
	cc -c -throwback -DTEST -Dmodule_realloc=realloc -Wp -I^,TCPIPLibs: ^.c.parseurl

generic.o: ^.c.generic
	cc -c ^.c.generic -I^,TCPIPLibs: -Wp

.c.o:; cc -c -throwback $*.c
