# ResEd Menu CSE Makefile


INCLUDES = -I^.common,C:

CPFLAGS = A~C~DF~L~N~P~Q~R~S~T~V

LIBS = ^.common.o.commonlib C:o.Stubs

CFILES = main.c menuedit.c protocol.c props.c relocate.c lassoo.c drag.c gui.c help.c

OFILES = main.o menuedit.o protocol.o props.o relocate.o lassoo.o drag.o gui.o help.o

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

