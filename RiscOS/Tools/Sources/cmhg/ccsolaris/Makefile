# Note CLXPATH should be set in the environment
#CLXPATH=/usr/local/riscos/clx
CLX=${CLXPATH}/clx.a

CFLAGS=-DNDEBUG -DPROCESS_VENEERS -c -O -I${CLXPATH} ${CFL}
ARMASM=armasm
CC=gcc
TOLOCAL=cp
OBJALL=cmhg.o outaof.o gen.o
DERIVED=gen.c gen.h
DERIVED_SRC=veneers.o

EXTDIR1=${CLXPATH}
EXTLIB1=${CLX}

all: cmhg
	@echo "Made cmhg (the default target)"

# Dependencies

veneers.o: veneers.s
	${ARMASM} -o $@ $<

gen.h gen.c: veneers.o
	aoftoc veneers.o gen

cmhg.o: gen.h

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
