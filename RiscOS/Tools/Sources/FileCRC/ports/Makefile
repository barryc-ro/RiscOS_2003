
TARGET=FileCRC

CFLAGS=-c
CC=gcc

OBJALL=C-CRC.o Display.o FileCRC.o DBUG.o

noarg: ${TARGET}
	@echo "Made ${TARGET} (the default target)"

# Dependencies

${TARGET}: $(OBJALL)
	$(CC) -o ${TARGET} $(OBJALL)
	@echo "Made ${TARGET}"

install: ${TARGET}
	/usr/ucb/install ${TARGET} ${RISCOS_BUILD_TOOLS}/${TARGET}
	strip ${RISCOS_BUILD_TOOLS}/${TARGET}

clean:
	-rm ${OBJALL}
	-rm ${TARGET}
	@echo cleaned ${TARGET}

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned ${TARGET} links

links:
	./mklinks
	@echo made ${TARGET} links
