# This file was created by Megamake.
# Copyright (C) Advanced RISC Machines 1992.

TARGET=armlink

CLX=${CLXPATH}/clx.a

CFLAGS=-D__acorn -DTARGET_IS_ACORN_RISC_OS=1 -c -O -I${CLXPATH} ${CFL}
CC=gcc
TOLOCAL=cp
DERIVED= datenow.h
OBJALL=aifcode.o hostdep.o link.o 

EXTDIR1=${CLXPATH}
EXTLIB1=${CLX}

HSOURCE=aifcode.h hostdep.h msgnos.h options.h 


noarg: ${TARGET}
	@echo "Made ${TARGET} (the default target)"

# Dependencies
aifcode.o:	aifcode.c
aifcode.o:	aifcode.h
aifcode.o:	$(EXTDIR1)/aif.h
	$(CC) $(CFLAGS) $(CFLaifcode) aifcode.c -o aifcode.o
hostdep.o:	hostdep.c
hostdep.o:	hostdep.h
hostdep.o:	options.h
	$(CC) $(CFLAGS) $(CFLhostdep) hostdep.c -o hostdep.o
ihctest.o:	ihctest.c
	$(CC) $(CFLAGS) $(CFLihctest) ihctest.c -o ihctest.o
link.o:	aifcode.h
link.o:	$(EXTDIR1)/aif.h
link.o:	$(EXTDIR1)/aof.h
link.o:	$(EXTDIR1)/asdfmt.h
link.o:	$(EXTDIR1)/chunkfmt.h
link.o:	$(EXTDIR1)/libflfmt.h
link.o:	$(EXTDIR1)/ovrflfmt.h
link.o:	$(EXTDIR1)/prgname.h
link.o:	datenow.h
link.o:	hostdep.h
link.o:	link.c
link.o:	msgnos.h
link.o:	options.h
	$(CC) $(CFLAGS) $(CFLlink) link.c -o link.o

${TARGET}: $(OBJALL)
${TARGET}: $(HSOURCE)
${TARGET}: $(DERIVED)
${TARGET}: $(EXTLIB1)
	$(CC) -o ${TARGET} $(OBJALL) $(EXTLIB1)
	@echo "Made ${TARGET}"

install: ${TARGET}
	install ${TARGET} ${RISCOS_BUILD_TOOLS}/${TARGET}
	strip ${RISCOS_BUILD_TOOLS}/${TARGET}

clean:
	-rm ${OBJALL}
	-rm ${TARGET}
	-rm $(DERIVED)
	@echo cleaned ${TARGET}

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned ${TARGET} links

links:
	./mklinks
	@echo made ${TARGET} links

datenow.h: FRC
	touch -f datenow.h
FRC:
