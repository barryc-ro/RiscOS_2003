# ResEd shell Makefile


INCLUDES = -I^.common,C:

CPFLAGS = A~C~DF~L~N~P~Q~R~S~T~V

LIBS = ^.common.o.commonlib C:o.Stubs

CFILES = main.c document.c dragdrop.c protocol.c class.c fileinfo.c objflags.c rename.c copy.c genmsgs.c help.c

OFILES = main.o document.o dragdrop.o protocol.o class.o fileinfo.o objflags.o rename.o copy.o genmsgs.o help.o

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

