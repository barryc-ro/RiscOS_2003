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
o.dbmalloc:	c.dbmalloc
o.dbmalloc:	h.resed
o.dbmalloc:	h.message
o.dbmalloc:	h.error
o.dbmalloc:	h.debug
o.dbmalloc:	h.dbmalloc
o.dbmalloc:	h.swicall
o.dbmalloc:	h.wimp
o.dbox:	c.dbox
o.dbox:	h.resed
o.dbox:	h.message
o.dbox:	h.error
o.dbox:	h.debug
o.dbox:	h.dbmalloc
o.dbox:	h.swicall
o.dbox:	h.wimp
o.dbox:	h.dbox
o.debug:	c.debug
o.debug:	h.resed
o.debug:	h.message
o.debug:	h.error
o.debug:	h.debug
o.debug:	h.dbmalloc
o.debug:	h.swicall
o.dragdrop:	c.dragdrop
o.dragdrop:	h.resed
o.dragdrop:	h.message
o.dragdrop:	h.error
o.dragdrop:	h.debug
o.dragdrop:	h.dbmalloc
o.dragdrop:	h.swicall
o.dragdrop:	h.wimp
o.dragdrop:	h.dragdrop
o.dragdrop:	h.interactor
o.error:	c.error
o.error:	h.resed
o.error:	h.message
o.error:	h.error
o.error:	h.debug
o.error:	h.dbmalloc
o.error:	h.swicall
o.error:	h.wimp
o.interactor:	c.interactor
o.interactor:	h.resed
o.interactor:	h.message
o.interactor:	h.error
o.interactor:	h.debug
o.interactor:	h.dbmalloc
o.interactor:	h.wimp
o.interactor:	h.interactor
o.menu:	c.menu
o.menu:	h.resed
o.menu:	h.message
o.menu:	h.error
o.menu:	h.debug
o.menu:	h.dbmalloc
o.menu:	h.swicall
o.menu:	h.wimp
o.menu:	h.interactor
o.menu:	h.menu
o.message:	c.message
o.message:	h.resed
o.message:	h.message
o.message:	h.error
o.message:	h.debug
o.message:	h.dbmalloc
o.message:	h.swicall
o.registry:	c.registry
o.registry:	h.resed
o.registry:	h.message
o.registry:	h.error
o.registry:	h.debug
o.registry:	h.dbmalloc
o.registry:	h.registry
o.saveas:	c.saveas
o.saveas:	h.resed
o.saveas:	h.message
o.saveas:	h.error
o.saveas:	h.debug
o.saveas:	h.dbmalloc
o.saveas:	h.swicall
o.saveas:	h.wimp
o.saveas:	h.dbox
o.saveas:	h.interactor
o.saveas:	h.saveas
o.wimp:	c.wimp
o.wimp:	h.resed
o.wimp:	h.message
o.wimp:	h.error
o.wimp:	h.debug
o.wimp:	h.dbmalloc
o.wimp:	h.swicall
o.wimp:	h.wimp
o.toolbox:	c.toolbox
o.toolbox:	h.resed
o.toolbox:	h.message
o.toolbox:	h.error
o.toolbox:	h.debug
o.toolbox:	h.dbmalloc
o.toolbox:	h.resformat
o.toolbox:	h.wimp
o.toolbox:	h.toolbox
o.focus:	c.focus
o.focus:	h.resed
o.focus:	h.message
o.focus:	h.error
o.focus:	h.debug
o.focus:	h.dbmalloc
o.focus:	h.swicall
o.focus:	h.wimp
o.focus:	h.focus
o.version:	c.version
o.version:	h.resed
o.version:	h.message
o.version:	h.error
o.version:	h.debug
o.version:	h.dbmalloc
o.version:	h.version
