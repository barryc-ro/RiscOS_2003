# Project:   memflex

CC = armcc
AS = armasm
CCflags = -c -I$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib \
	-I$(RISCOS_BUILD_EXPORT)/Lib/CLib \
	-ffa -Ospace -DRISCOS

ASflags = -Stamp -quit

.SUFFIXES: .c .s .o .oD

Objects =  dynamic.o malloc.o realloc.o calloc.o valloc.o \
           mallocsize.o mallocstat.o memflex.o subflex.o

all: memlib

memlib: $(Objects)
	armlib -c -o memlib $(Objects)

riscos_libs: memlib

riscos_export: riscos_libs
	@-mkdir $(RISCOS_BUILD_EXPORT)
	@-mkdir $(RISCOS_BUILD_EXPORT)/Lib
	@-mkdir $(RISCOS_BUILD_EXPORT)/Lib/MemLib
	@echo Made export directories
	@cp -p memlib $(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlib.o
	@echo Copied library

clean:
	@-rm *.o
	@-rm memlib

.c.o:;	$(CC) $(CCflags) -o $@ $<

.s.o:;	$(AS) $(ASflags) -from $< -to $@

# Do not faff with the following comment as Make looks for it explicitly:
# Dynamic dependencies:
