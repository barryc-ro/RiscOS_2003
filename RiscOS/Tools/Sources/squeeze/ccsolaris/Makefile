# Note CLXPATH should be set in the environment
#CLXPATH=/usr/local/riscos/clx
CLX=${CLXPATH}/clx.a

CFLAGS=-DDEBUGGING=0 -c -O -I${CLXPATH} ${CFL}
CC=gcc
AS=armasm
SQ_OBJS=squeeze.o unsqueeze1.o unsqrm1.o
XP_OBJS=xpand.o asmcall1.o
DERIVED= unsqueeze1.c unsqrm1.c unsqueeze1.h unsqrm1.h asmcall1.c asmcall1.h
DERIVED_SRC= unsqueeze.o unsqrm.o asmcall.o

EXTDIR1=${CLXPATH}
EXTLIB1=${CLX}

all: armsqueeze armxpand
	@echo "Made armsqueeze (the default target)"

# Dependencies

asmcall1.h asmcall1.c: asmcall.o
	aoftoc asmcall.o asmcall1

unsqueeze1.h unsqueeze1.c: unsqueeze.o
	aoftoc unsqueeze.o unsqueeze1

unsqrm1.h unsqrm1.c: unsqrm.o
	aoftoc unsqrm.o unsqrm1

armsqueeze: $(SQ_OBJS) $(EXTLIB1) derived
	$(CC) -o armsqueeze ${SQ_OBJS} $(EXTLIB1)
	@echo "Made armsqueeze"

armxpand: $(XP_OBJS) $(DERIVED) $(EXTLIB1)
	$(CC) -o armxpand ${XP_OBJS} $(EXTLIB1)
	@echo "Made armxpand"

clean:
	-rm ${SQ_OBJS} ${XP_OBJS}
	-rm armsqueeze armxpand
	-rm $(DERIVED)
	-rm $(DERIVED_SRC)
	@echo cleaned squeeze

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned squeeze links

derived: ${DERIVED}
	@echo Made derived files

links:
	./mklinks
	@echo made squeeze links

install: armsqueeze
	cp -p armsqueeze ${RISCOS_BUILD_TOOLS}
#	cp -p armxpand ${RISCOS_BUILD_TOOLS}
	@echo "Installed xpand and squeeze"
