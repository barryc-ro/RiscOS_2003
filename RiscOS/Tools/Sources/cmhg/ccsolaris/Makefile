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

all: cmhg
	@echo "Made cmhg (the default target)"

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

cmhg: $(DERIVED_SRC)
cmhg: $(DERIVED)
cmhg: $(OBJALL)
cmhg: $(EXTLIB1)
	$(CC) -o cmhg $(OBJALL) $(EXTLIB1)
	@echo "Made cmhg"

clean:
	-rm ${OBJALL}
	-rm cmhg
	-rm $(DERIVED)
	-rm $(DERIVED_SRC)
	@echo cleaned cmhg

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned cmhg links

links:
	./mklinks
	@echo made cmhg links

install: cmhg
	cp -p cmhg ${RISCOS_BUILD_TOOLS}
	@echo "Installed cmhg"
