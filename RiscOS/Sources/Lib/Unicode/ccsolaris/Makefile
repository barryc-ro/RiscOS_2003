# Project:   Unicode

CC=gcc

CCflags=-funsigned-char

.c.o:;	$(CC) -c -DDEBUG=0 $(CCflags) -o $@ $<

Objects = autojp.o \
	unictype.o \
	utf8.o \
	utf16.o \
	encoding.o \
	iso2022.o \
	eightbit.o \
	shiftjis.o \
	johab.o \
	bigfive.o \
	enc_utf7.o \
	enc_utf8.o \
	enc_ascii.o \
	enc_utf16.o \
	enc_ucs4.o \
	enc_scsu.o \
	enc_system.o \
	acorn.o \
	combine.o \
	unix.o

all:	ucodelib.a textconv

ucodelib.a: $(Objects)
	${AR} r $@ $(Objects)

textconv: textconv.o ucodelib.a
	${CC} -o $@ textconv.o ucodelib.a

clean:
	@-rm mkunictype textconv
	@-rm unictype.c
	@-rm *.o
	@-rm ucodelib.a
	echo Done clean

realclean: clean
	find . -type l -print | xargs rm
	echo Done realclean

unictype.c: ../data/UnicodeData mkunictype
	./mkunictype < ../data/UnicodeData > unictype.c
	echo Made unictype.c

links: ; ./mklinks
	echo made links

# Do not faff with the following comment as Make looks for it explicitly:
# Dynamic dependencies:
