# ResEd common Makefile

OFILES = dbmalloc.o dbox.o debug.o dragdrop.o error.o interactor.o menu.o\
	message.o registry.o saveas.o wimp.o toolbox.o focus.o swicall.o version.o 

export: commonlib.o
        @echo *** Commonlib build complete

commonlib.o: $(OFILES)
        libfile -c -o $@ $(OFILES)

.c.o:
        cc -c -depend !Depend -throwback $(CFLAGS) $(INCLUDES) -o $@ $<

.s.o:
        objasm -quit -CloseExec $(AFLAGS) -from $< -to $@

clean:
       -wipe o.* ~CRF~V
       @echo *** Common cleaned


# Dynamic dependencies:
