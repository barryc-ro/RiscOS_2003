# URLclient
.SUFFIXES: .c .o

test: URLClient
	@|

run: test
	@URLClient -test URLs
	@URLClient -test FileURLs

URLClient: URLclient.o
	link -o $@ URLclient.o c:o.stubs

PathWrite: pathwrite.o
	link -o $@ pathwrite.o c:o.stubs	

speedtest: speedtest.c
	cc c.speedtest

.c.o:; cc -c -throwback $*.c
