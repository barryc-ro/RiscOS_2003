# ROM linker.

TARGET=romlinker

CLX=${CLXPATH}/clx.a

CFLAGS=-c -I${CLXPATH}
CC=gcc
TOLOCAL=cp
OBJALL=filereader.o filewriter.o romlinker.o memory.o makerom.o makeexprom.o

EXTDIR1=${CLXPATH}
EXTLIB1=${CLX}

noarg: ${TARGET}
	@echo "Made ${TARGET} (the default target)"

${TARGET}: $(OBJALL)
	$(CC) -o ${TARGET} $(OBJALL) $(EXTLIB1)
	@echo "Made ${TARGET}"

install: ${TARGET}
	/usr/ucb/install ${TARGET} ${RISCOS_BUILD_TOOLS}/${TARGET}
	strip ${RISCOS_BUILD_TOOLS}/${TARGET}

clean:
	-rm -f ${OBJALL}
	-rm -f ${TARGET}
	@echo cleaned ${TARGET}

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned ${TARGET} links

links:
	./mklinks
	@echo made ${TARGET} links
