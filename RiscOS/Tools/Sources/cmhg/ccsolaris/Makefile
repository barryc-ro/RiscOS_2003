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

all: armcmhg
	@echo "Made armcmhg (the default target)"

# Dependencies

veneers.o: veneers.s
	${ARMASM} -o $@ $<

gen.h gen.c: veneers.o
	aoftoc veneers.o gen

cmhg.o: gen.h

armcmhg: $(DERIVED_SRC)
armcmhg: $(DERIVED)
armcmhg: $(OBJALL)
armcmhg: $(EXTLIB1)
	$(CC) -o armcmhg $(OBJALL) $(EXTLIB1)
	@echo "Made armcmhg"

clean:
	-rm ${OBJALL}
	-rm armcmhg
	-rm $(DERIVED)
	-rm $(DERIVED_SRC)
	@echo cleaned cmhg

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned cmhg links

links:
	./mklinks
	@echo made cmhg links

install: armcmhg
	cp -p armcmhg ${RISCOS_BUILD_TOOLS}
	@echo "Installed cmhg"
