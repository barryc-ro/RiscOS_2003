CCflags 	= -c -depend !Depend -IOS:,C: -throwback 
Linkflags 	= -aif -c++ -o $@
ObjAsmflags 	= -throwback -NoCache -depend !Depend

Libraries 	= C:o.Stubs
ObjectFiles 	= @.o.Args @.o.BGet @.o.BPut @.o.File @.o.Find @.o.FSControl \
		  @.o.GBPB @.o.logger @.o.mymalloc @.o.Random @.o.Tester

@.FSBash:    	$(ObjectFiles) $(Libraries)
        	Link $(Linkflags) $(ObjectFiles) $(Libraries)

.SUFFIXES:	.c .o

.c.o:;		cc $(CCflags) -o $@ $<

# Dynamic dependencies:
o.Args: c.Args C:h.kernel C:h.swis h.tester h.logger
o.BGet: c.BGet C:h.kernel C:h.swis h.Tester h.logger
o.File: c.File C:h.kernel C:h.swis h.tester h.logger
o.Find: c.Find C:h.kernel C:h.swis h.Tester h.logger
o.FSControl: c.FSControl C:h.kernel C:h.swis h.tester h.logger
o.GBPB: c.GBPB C:h.kernel C:h.swis h.Tester h.logger
o.logger: c.logger h.logger
o.Random: c.Random C:h.kernel C:h.swis h.Tester h.logger h.mymalloc
o.Tester: c.Tester C:h.kernel C:h.swis h.tester h.logger h.version h.mymalloc
