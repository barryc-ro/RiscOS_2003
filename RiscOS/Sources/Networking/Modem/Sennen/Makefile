# Project:  Sennen


# Toolflags:
CCflags        = -c -depend !Depend -IC: -fussy -throwback
C++flags       = -c -depend !Depend -IC: -throwback
Linkflags      = -aif -v -c++ -o $@ 
DrLinkflags    = -aif -verbose -nounused -o $@ 
ObjAsmflags    = -throwback -NoCache -depend !Depend
CMHGflags      = 
LibFileflags   = -c -o $@
Squeezeflags   = -f -v $@
Datatoaofflags = -d data -l len -dp -s -p

FILES = o.main o.swicall o.mainwindow o.screen o.kill

LIBS  = C:o.Stubs

# Final targets:
!RunImage: ${FILES} ${LIBS}
        Link $(Linkflags) ${FILES} ${LIBS}
        Squeeze $(Squeezeflags)


# User-editable dependencies:

.SUFFIXES: .d

# Static dependencies:
.c.o:;        cc $(CCflags) $< -o $@
.s.o:;        objasm $(ObjAsmflags) -from $< -to $@
.d.o:;      datatoaof -f $< -o $@ $(Datatoaofflags)


# Dynamic dependencies:
o.main:	c.main
o.main:	h.defs
o.main:	C:h.kernel
o.main:	h.inlines
o.main:	C:h.swis
o.main:	h.screen
o.main:	h.defs
o.main:	h.SmartCard
o.main:	h.swicall
o.main:	h.defs
o.main:	C:h.swis
o.main:	h.wimp
o.main:	h.defs
