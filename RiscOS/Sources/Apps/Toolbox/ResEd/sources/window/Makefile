# ResEd Window CSE Makefile


INCLUDES = -I^.common,C:

CPFLAGS = A~C~DF~L~N~P~Q~R~S~T~V

LIBS = ^.common.o.commonlib C:o.Stubs

CFILES = main.c protocol.c relocate.c windowedit.c windinfo.c grid.c coords.c align.c\
         gadget.c props.c colours.c extent.c gui.c keycuts.c lassoo.c drag.c move.c\
         resize.c gadgdefs.c winddrag.c toolbars.c help.c

OFILES = main.o protocol.o relocate.o windowedit.o windinfo.o grid.o coords.o align.o\
         gadget.o props.o colours.o extent.o gui.o keycuts.o lassoo.o drag.o move.o\
         resize.o gadgdefs.o winddrag.o toolbars.o help.o

export: ${TARGET}
        @echo *** ${TARGET} build complete

${TARGET}: ${OFILES} ${LIBS}
	link ${LDFLAGS} -o $@ ${OFILES} ${LIBS}

${CFILES}: h.icondefs
	stamp $@
	
h.icondefs: ${TEMPLATES}
        ${WINEDIT} -h ${TEMPLATES} > $@

.c.o:
        cc -c $(CFLAGS) $(INCLUDES) -o $@ $<

.s.o:
        objasm -quit -CloseExec $(AFLAGS) -from $< -to $@


clean:
	remove h.icondefs
        -wipe o.* ~CRF~V
       	@echo *** Misc cleaned

