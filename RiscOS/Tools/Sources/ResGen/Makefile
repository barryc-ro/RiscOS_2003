# Makefile for modgen, resgen, binaof, binasm
#
# Note about modgen:
#   modgen_old is modgen before addition of Ursula service call table.
#   modgen has the Ursula service call table.

#LIBTYPE=ansilib
LIBTYPE=stubs
CC=cc
CFLAGS=-ffah -c -DDDE -depend !Depend
#CFLAGS=-ffah -c
LD=Link
LIB=CLib:o.$(LIBTYPE)
SERVER=1.251
RELEASEFS=net:&

all_old:      resgen modgen_old binaof binasm

all:          resgen modgen binaof binasm

resgen:       resgen.o rveneers.o
              $(LD) -o ResGen o.resgen o.rveneers $(LIB)
              squeeze ResGen

modgen:       modgen.o veneers.o
              $(LD) -o ModGen o.modgen o.veneers $(LIB)
              squeeze ModGen

modgen_old:   modgenold.o oveneers.o
              $(LD) -o ModGen o.modgen o.oveneers $(LIB)
              squeeze ModGen

binaof:       binaof.o
              $(LD) -o BinAof o.binaof $(LIB)
              squeeze BinAof

binasm:       binasm.o
              $(LD) -o BinAsm o.binasm $(LIB)
              squeeze BinAsm

resgen.o:     resgen.c
              $(CC) $(CFLAGS) resgen.c

modgen.o:     modgen.c
              $(CC) $(CFLAGS) modgen.c

modgenold.o:  modgen.c
              $(CC) $(CFLAGS) -D NO_SERVICE_TABLE modgen.c

binaof.o:     binaof.c
              $(CC) $(CFLAGS) binaof.c

binasm.o:     binasm.c
              $(CC) $(CFLAGS) binasm.c

veneers.o:    veneers.s
              objasm s.veneers -to o.veneers -stamp -quit

oveneers.o:   oveneers.s
              objasm s.oveneers -to o.oveneers -stamp -quit

rveneers.o:   rveneers.s
              objasm s.rveneers -to o.rveneers -stamp -quit

install:    ; copy resgen %.resgen ~cfq
              copy resgen ^.library.ResGen ~cfq

clean:;       remove resgen
              remove o.resgen
              remove o.rveneers
              remove modgen
              remove o.modgen
              remove o.veneers
              remove o.oveneers
              remove binasm
              remove o.binasm
              remove binaof
              remove o.binaof
              remove o.modgenold

release:      ;net
              fs $(SERVER)
              adfs
            - cdir $(RELEASEFS).src.resgen$(DOTVSN)
            - cdir $(RELEASEFS).src.resgen$(DOTVSN).c
            - cdir $(RELEASEFS).src.resgen$(DOTVSN).h
              copy makefile $(RELEASEFS).src.resgen$(DOTVSN).makefile    ~cfq
              access $(RELEASEFS).src.resgen$(DOTVSN).makefile           wr/r
            - copy resgen $(RELEASEFS).bin.resgen.resgen$(VSN)           ~cfq
              access $(RELEASEFS).bin.resgen.resgen$(VSN)                wr/r
              copy c.* $(RELEASEFS).src.resgen$(DOTVSN).c.*              ~cfq
              access $(RELEASEFS).src.resgen$(DOTVSN).c.*                wr/r
              copy h.* $(RELEASEFS).src.resgen$(DOTVSN).h.*              ~cfq
              access $(RELEASEFS).src.resgen$(DOTVSN).h.*                wr/r

# Dynamic dependencies:
o.resgen:	c.resgen
o.resgen:	C:h.kernel
o.resgen:	C:h.swis
o.resgen:	h.objflfmt
o.binaof:	c.binaof
o.binaof:	C:h.kernel
o.binaof:	C:h.swis
o.binaof:	h.objflfmt
o.binasm:	c.binasm
o.binasm:	C:h.kernel
o.modgen:	c.modgen
o.modgen:	C:h.kernel
o.modgen:	C:h.swis
