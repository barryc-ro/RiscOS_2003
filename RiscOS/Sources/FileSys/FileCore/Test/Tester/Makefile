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
