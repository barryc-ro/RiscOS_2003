# This makefile made by Makatic3




ProjectName		=	CLX

Objects	=	\
	o.bytesex\
	o.chunkfls\
	o.disass\
	o.err\
	o.fname\
	o.prgname\
	o.re\
	o.wholefls\

.SUFFIXES:	.o .c

All:	clxlib

clean:
	wipe @.o.* ~c~rv

clxlib:	$(Objects)
	libfile -c -o $@ $(Objects)

.c.o:
	cc -c -fah -depend !Depend -throwback -I,C: -o $@ $<

# Dynamic dependencies:
o.bytesex:	c.bytesex
o.bytesex:	h.bytesex
o.chunkfls:	c.chunkfls
o.chunkfls:	h.chunkfls
o.chunkfls:	h.chunkfmt
o.chunkfls:	h.bytesex
o.disass:	c.disass
o.disass:	h.disass
o.err:	c.err
o.err:	h.err
o.fname:	c.fname
o.fname:	h.fname
o.prgname:	c.prgname
o.prgname:	h.prgname
o.re:	c.re
o.re:	h.re
o.wholefls:	c.wholefls
o.wholefls:	h.wholefls
