# URLclient
.SUFFIXES: .c .o

test: URLClient FTPProxy
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

FTPProxy: FTPPrxSrc
	FTPPrxSrc

clean:
        -delete FTPProxy
        -delete PrintWrite
        -delete speedtest
        -delete URLClient
        -wipe o.* fr~c~v


.c.o:; cc -c -throwback $*.c
