# Project:   memflex

CC = armcc
AS = armasm
CCflags = -c -I$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib \
	-I$(RISCOS_BUILD_EXPORT)/Lib/CLib \
	-ffa -Ospace -DRISCOS -DACORN_BUILD

ASflags = -Stamp -quit

.SUFFIXES: .c .s .o .oz .od .odz

.c.o:;	$(CC) -DROM=0 $(CCflags) -o $@ $<

.c.od:;	$(CC) -DROM=0 -DMEM_DEBUG -DDEBUG -Fn $(CCflags) -o $@ $<

.c.oz:;	$(CC) -zM -DROM=1 $(CCflags) -o $@ $<

.c.odz:;$(CC) -zM -DROM=1 -DMEM_DEBUG -DDEBUG -Fn  $(CCflags) -o $@ $<

.s.o:;	$(AS) $(ASflags) -from $< -to $@

Objects =  dynamic.o malloc.o realloc.o calloc.o valloc.o \
           mallocsize.o mallocstat.o memflex.o subflex.o

ObjectsD =  dynamic.o malloc.od realloc.od calloc.od valloc.od \
           mallocsize.od mallocstat.od memflex.od subflex.od

ObjectsZM =  dynamic.o malloc.oz realloc.oz calloc.oz valloc.oz \
           mallocsize.oz mallocstat.oz memflex.oz subflex.oz

ObjectsDZM =  dynamic.o malloc.odz realloc.odz calloc.odz valloc.odz \
           mallocsize.odz mallocstat.odz memflex.odz subflex.odz

all: memlib memlibzm memlibd memlibdzm

memlib: $(Objects)
	armlib -c -o memlib $(Objects)

memlibd: $(ObjectsD)
	armlib -c -o memlibd $(ObjectsD)

memlibzm: $(ObjectsZM)
	armlib -c -o memlibzm $(ObjectsZM)

memlibdzm: $(ObjectsDZM)
	armlib -c -o memlibdzm $(ObjectsDZM)

riscos_libs: memlib memlibzm memlibd memlibdzm

riscos_export: riscos_libs
	@-mkdir $(RISCOS_BUILD_EXPORT)
	@-mkdir $(RISCOS_BUILD_EXPORT)/Lib
	@-mkdir $(RISCOS_BUILD_EXPORT)/Lib/MemLib
	@echo Made export directories
	@cp -p memlib $(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlib.o
	@cp -p memlibd $(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibd.o
	@cp -p memlibzm $(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibzm.o
	@cp -p memlibdzm $(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibdzm.o
	@cp -p memheap.h $(RISCOS_BUILD_EXPORT)/Lib/MemLib/
	@cp -p memflex.h $(RISCOS_BUILD_EXPORT)/Lib/MemLib/
	@cp -p dynamic.h $(RISCOS_BUILD_EXPORT)/Lib/MemLib/
	@cp -p subflex.h $(RISCOS_BUILD_EXPORT)/Lib/MemLib/
	@cp -p mallinfo.h $(RISCOS_BUILD_EXPORT)/Lib/MemLib/
	@echo Copied libraries

clean:
	@-rm *.o
	@-rm *.od
	@-rm *.oz
	@-rm *.odz
	@-rm memlib
	@-rm memlibd
	@-rm memlibzm
	@-rm memlibdzm

# Do not faff with the following comment as Make looks for it explicitly:
# Dynamic dependencies:
