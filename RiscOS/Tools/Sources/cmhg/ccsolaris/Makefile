COMPONENT=armcmhg

# Note CLXPATH should be set in the environment
#CLXPATH=/usr/local/riscos/clx
CLX=${CLXPATH}/clx.a

CFLAGS=-DNDEBUG -DPROCESS_VENEERS -c -O -I${CLXPATH} ${CFL}
ARMASM=armasm
CC=gcc
TOLOCAL=cp
OBJALL=cmhg.o outaof.o gen26.o gen32.o
DERIVED=gen26.c gen26.h gen32.c gen32.h
DERIVED_SRC=veneers.o veneers32.o

EXTDIR1=${CLXPATH}
EXTLIB1=${CLX}

all: ${COMPONENT}
	@echo "Made ${COMPONENT} (the default target)"

# Dependencies

veneers.o: veneers.s
	${ARMASM} -o $@ $<

veneers32.o: veneers32.s
	${ARMASM} -o $@ $<

gen26.h gen26.c: veneers.o
	aoftoc veneers.o gen26

gen32.h gen32.c: veneers32.o
	aoftoc veneers32.o gen32

cmhg.o: gen26.h gen32.h

${COMPONENT}: $(DERIVED_SRC)
${COMPONENT}: $(DERIVED)
${COMPONENT}: $(OBJALL)
${COMPONENT}: $(EXTLIB1)
	$(CC) -o ${COMPONENT} $(OBJALL) $(EXTLIB1)
	@echo "Made ${COMPONENT}"

clean:
	-rm ${OBJALL}
	-rm ${COMPONENT}
	-rm $(DERIVED)
	-rm $(DERIVED_SRC)
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
