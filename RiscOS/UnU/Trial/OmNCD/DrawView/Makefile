# Project:  Pentire


# Toolflags:
CCflags  = -c -depend !Depend -IC: -fussy -throwback
C++flags = -c -depend !Depend -IC: -throwback
Linkflags = -aif -v -c++ -o $@ 
DrLinkflags    = -aif -verbose -nounused -o $@ 
ObjAsmflags    = -throwback -NoCache -depend !Depend
CMHGflags      = 
LibFileflags   = -c -o $@
Squeezeflags   = -o $@
Datatoaofflags = -d data -l len -dp -s -p

FILES = o.heap o.main o.swicall o.screen o.render o.muldiv o.mainwindow \
        o.GetType

LIBS  = C:o.Stubs

# Final targets:
!RunImage: ${FILES} ${LIBS}
        Link $(Linkflags) ${FILES} ${LIBS}


# User-editable dependencies:

.SUFFIXES: .d

# Static dependencies:
.c.o:;        cc $(CCflags) $< -o $@
.s.o:;        objasm $(ObjAsmflags) -from $< -to $@
.d.o:;      datatoaof -f $< -o $@ $(Datatoaofflags)


# Dynamic dependencies:
o.swicall: s.swicall
o.muldiv: s.muldiv
o.GetType: s.GetType
o.GetType: h.regdefs
o.GetType: h.swinos
o.GetType: h.macros
o.heap:	c.heap
o.heap:	h.Defs
o.heap:	C:h.kernel
o.heap:	h.inlines
o.heap:	h.NHWidget
o.heap:	C:h.swis
o.heap:	h.swicall
o.heap:	h.defs
o.heap:	C:h.swis
o.render:	c.render
o.render:	h.defs
o.render:	C:h.kernel
o.render:	h.heap
o.render:	h.defs
o.render:	h.inlines
o.render:	h.NHWidget
o.render:	C:h.swis
o.render:	h.swicall
o.render:	h.defs
o.render:	C:h.swis
o.screen:	c.screen
o.screen:	h.defs
o.screen:	C:h.kernel
o.screen:	h.inlines
o.screen:	h.NHWidget
o.screen:	C:h.swis
o.screen:	h.muldiv
o.screen:	h.swicall
o.screen:	h.defs
o.screen:	C:h.swis
o.main:	c.main
o.main:	h.defs
o.main:	C:h.kernel
o.main:	h.heap
o.main:	h.defs
o.main:	h.inlines
o.main:	h.NHWidget
o.main:	C:h.swis
o.main:	h.muldiv
o.main:	h.objects
o.main:	h.defs
o.main:	h.render
o.main:	h.defs
o.main:	h.screen
o.main:	h.defs
o.main:	h.swicall
o.main:	h.defs
o.main:	C:h.swis
o.main:	h.wimp
o.main:	h.defs
