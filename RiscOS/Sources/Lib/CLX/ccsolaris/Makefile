# This file was created by Megamake.
# Copyright (C) Advanced RISC Machines 1992.

CFLAGS=-c -O2 -I. $(CFL)
CC=gcc
DERIVED= datenow.h
OBJALL=bytesex.o chunkfls.o disass.o err.o fname.o prgname.o re.o ieeeflt.o \
dtos.o host_scr.o scr_drv.o 
HSOURCE=aif.h disass.h libflfmt.h scr_drv.h aof.h bytesex.h err.h \
ovrflfmt.h set_host.h aout.h chunkfls.h fname.h prgname.h asdfmt.h \
chunkfmt.h host_scr.h re.h ieeeflt.h 

noarg: clx.a
	@echo "Made clx.a (the default target)"

# Dependencies
bytesex.o:	bytesex.c
bytesex.o:	bytesex.h
	$(CC) $(CFLAGS) $(CFLbytesex) bytesex.c -o bytesex.o
disass.o:	disass.c
disass.o:	disass.h
	$(CC) $(CFLAGS) $(CFLdisass) disass.c -o disass.o
fname.o:	fname.c
fname.o:	fname.h
	$(CC) $(CFLAGS) $(CFLfname) fname.c -o fname.o
prgname.o:	prgname.c
prgname.o:	prgname.h
	$(CC) $(CFLAGS) $(CFLprgname) prgname.c -o prgname.o
scr_drv.o:	host_scr.h
scr_drv.o:	scr_drv.c
scr_drv.o:	scr_drv.h
scr_drv.o:	set_host.h
	$(CC) $(CFLAGS) $(CFLscr_drv) scr_drv.c -o scr_drv.o
chunkfls.o:	bytesex.h
chunkfls.o:	chunkfls.c
chunkfls.o:	chunkfls.h
chunkfls.o:	chunkfmt.h
	$(CC) $(CFLAGS) $(CFLchunkfls) chunkfls.c -o chunkfls.o
err.o:	err.c
err.o:	err.h
	$(CC) $(CFLAGS) $(CFLerr) err.c -o err.o
host_scr.o:	host_scr.c
host_scr.o:	host_scr.h
host_scr.o:	set_host.h
	$(CC) $(CFLAGS) $(CFLhost_scr) host_scr.c -o host_scr.o
re.o:	re.c
re.o:	re.h
	$(CC) $(CFLAGS) $(CFLre) re.c -o re.o
ieeeflt.o:	ieeeflt.c
ieeeflt.o:	ieeeflt.h
	$(CC) $(CFLAGS) $(CFLieeeflt) ieeeflt.c -o ieeeflt.o
dtos.o:	dtos.c
dtos.o:	ieeeflt.h
	$(CC) $(CFLAGS) $(CFLdtos) dtos.c -o dtos.o

# Dependencies to /nfs/sun1/bb/xdevt/release/bin/topcc -l from local to build directories

clx.a: $(OBJALL)
clx.a: $(HSOURCE)
clx.a: $(DERIVED)
	ar rc clx.a $(OBJALL)
#	ranlib clx.a
	@echo "Library file clx.a made"

release:
	@echo "No make release for clx"

clean:
	-rm ${OBJALL}
	-rm clx.a
	-rm ${DERIVED}
	@echo cleaned CLX

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned CLX links

links:
	./mklinks
	@echo made CLX links

datenow.h: FRC
	touch -f datenow.h
FRC:
