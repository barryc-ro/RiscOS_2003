# ResEd Misc CSE Makefile


INCLUDES = -I^.common,C:

CPFLAGS = A~C~DF~L~N~P~Q~R~S~T~V

LIBS = ^.common.o.commonlib C:o.Stubs

CFILES = colours.c gui.c main.c objdefs.c object.c objedit.c protocol.c relocate.c help.c

OFILES = colours.o gui.o main.o objdefs.o object.o objedit.o protocol.o relocate.o help.o

export: ${TARGET}
        @echo *** ${TARGET} build complete

${TARGET}: ${OFILES} ${LIBS} h.icondefs
	link ${LDFLAGS} -o $@ ${OFILES} ${LIBS}

${CFILES}: h.icondefs
	stamp $@
	
h.icondefs: ${TEMPLATES}
        ${WINEDIT} -h ${TEMPLATES} > $@

.c.o:   h.icondefs
        cc -c $(CFLAGS) $(INCLUDES) -o $@ $<

.s.o:
        objasm -quit -CloseExec $(AFLAGS) -from $< -to $@

clean:
	remove h.icondefs
        -wipe o.* ~CRF~V
       	@echo *** Misc cleaned


# Dynamic dependencies:
