# This file was created by hand.

COMPONENT=decaof
CLX=${CLXPATH}/clx.a

CC=gcc
CFLAGS=-c -O2 -I$(CLXPATH)

OBJALL=decaof.o decasd.o

# Dependencies

${COMPONENT}: $(OBJALL) $(CLX)
	$(CC) -s -o $@ $(OBJALL) $(CLX)
	@echo "Made $@"

install: ${COMPONENT}
	install ${COMPONENT} ${RISCOS_BUILD_TOOLS}/${COMPONENT}
	strip ${RISCOS_BUILD_TOOLS}/${COMPONENT}

clean:
	rm -rf ${OBJALL} ${COMPONENT}
	@echo cleaned ${COMPONENT}

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned ${COMPONENT} links

links:
	./mklinks
	@echo made ${COMPONENT} links

.c.o:
	${CC} ${CFLAGS} ${CFL$*} -o $@ $<

# Dependencies.
