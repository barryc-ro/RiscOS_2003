# Note CLXPATH should be set in the environment
#CLXPATH=/usr/local/riscos/clx
CLX=${CLXPATH}/clx.a

CFLAGS=-DNDEBUG -c -O -I${CLXPATH} ${CFL}
COMPONENT=aoftoc
CC=gcc

OBJS=aoftoc.o
LIBS=${CLX}

all: ${COMPONENT}
	@echo "Made ${COMPONENT} (the default target)"

# Dependencies

${COMPONENT}: $(OBJS)
	$(CC) -o ${COMPONENT} $(OBJS) $(LIBS)
	@echo "Made ${COMPONENT}"

clean:
	-rm -f ${OBJS}
	-rm -f ${COMPONENT}
	@echo cleaned ${COMPONENT}

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned ${COMPONENT} links

links:
	./mklinks
	@echo made ${COMPONENT} links

install: ${COMPONENT}
	install ${COMPONENT} ${RISCOS_BUILD_TOOLS}
	@echo "Installed ${COMPONENT}"
