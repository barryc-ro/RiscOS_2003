# Project:   Unicode

CC = armcc
AS = armasm
CCflags = -c -I$(RISCOS_BUILD_EXPORT)/Lib/CLib \
	-ffa -Ospace -DRISCOS -Fn

ASflags = -Stamp -quit

.SUFFIXES: .c .s .o .oz .od .odz

.c.o:;	$(CC) -DROM=0 $(CCflags) -o $@ $<

.c.od:;	$(CC) -DROM=0 -DDEBUG=1 -Fn $(CCflags) -o $@ $<

.c.oz:;	$(CC) -zM -DROM=1 $(CCflags) -o $@ $<

.c.odz:;$(CC) -zM -DROM=1 -DDEBUG=1 -Fn  $(CCflags) -o $@ $<

.s.o:;	$(AS) $(ASflags) -from $< -to $@

Objects =  autojp.o unictype.o \
	utf8.o \
	encoding.o \
	iso2022.o \
	koi8r.o \
	microsoft.o \
	acorn.o \
	apple.o \
	shiftjis.o \
	bigfive.o \
	enc_utf8.o \
	enc_ascii.o \
	enc_utf16.o \
	enc_ucs4.o

ObjectsD =  autojp.od unictype.od \
	utf8.od \
	encoding.od \
	iso2022.od \
	koi8r.od \
	microsoft.od \
	acorn.od \
	apple.od \
	shiftjis.od \
	bigfive.od \
	enc_utf8.od \
	enc_ascii.od \
	enc_utf16.od \
	enc_ucs4.od

all: ucodelib

ucodelib: $(Objects)
	armlib -c -o ucodelib $(Objects)

ucodelibd: $(ObjectsD)
	armlib -c -o ucodelibd $(ObjectsD)

ucodelibzm: $(ObjectsZM)
	armlib -c -o ucodelibzm $(ObjectsZM)

ucodelibdzm: $(ObjectsDZM)
	armlib -c -o ucodelibdzm $(ObjectsDZM)

riscos_libs: ucodelib ucodelibd

riscos_export: riscos_libs
	@-mkdir $(RISCOS_BUILD_EXPORT)
	@-mkdir $(RISCOS_BUILD_EXPORT)/Lib
	@-mkdir $(RISCOS_BUILD_EXPORT)/Lib/Unicode
	@echo Made export directories
	@-cp -p ucodelib $(RISCOS_BUILD_EXPORT)/Lib/Unicode/ucodelib.o
	@-cp -p ucodelibd $(RISCOS_BUILD_EXPORT)/Lib/Unicode/ucodelibd.o
	@-cp -p ucodelibzm $(RISCOS_BUILD_EXPORT)/Lib/Unicode/ucodelibzm.o
	@cp -p autojp.h $(RISCOS_BUILD_EXPORT)/Lib/Unicode/
	@cp -p charsets.h $(RISCOS_BUILD_EXPORT)/Lib/Unicode/
	@cp -p unictype.h $(RISCOS_BUILD_EXPORT)/Lib/Unicode/
	@cp -p encoding.h $(RISCOS_BUILD_EXPORT)/Lib/Unicode/
	@cp -p iso10646.h $(RISCOS_BUILD_EXPORT)/Lib/Unicode/
	@cp -p utf8.h $(RISCOS_BUILD_EXPORT)/Lib/Unicode/
	@echo Copied libraries

clean:
	@-rm mkunictype
	@-rm unictype.c
	@-rm *.o
	echo Done clean

realclean: clean
	find . -type l -print | xargs rm
	echo Done realclean

unictype.c: ../data/UNIDATA2 mkunictype
	./mkunictype < ../data/UNIDATA2 > unictype.c
	echo Made unictype.c

mkunictype: mkunictype.c
	rsh starman 'cd /projects2/smiddle/renaissa/RiscOS/Sources/Lib/Unicode/unix ; cc -o mkunictype mkunictype.c'
	echo Made mkunictype

# Do not faff with the following comment as Make looks for it explicitly:
# Dynamic dependencies:
