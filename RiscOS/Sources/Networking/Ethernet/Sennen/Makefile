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

FILES = o.main o.mainwindow o.screen o.kill o.swiven \
        o.enternpc
        
LIBS  = C:o.Stubs C:o.inetlib

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
o.filegen:	c.filegen
o.filegen:	h.defs
o.filegen:	C:h.kernel
o.filegen:	h.inlines
o.filegen:	C:h.swis
o.filegen:	h.smartcard
o.filegen:	h.swicall
o.filegen:	h.defs
o.filegen:	C:h.swis
o.filegen:	C:Sockets.h.in
o.swicall: s.swicall
o.swiven: s.swiven
o.swiven: h.regdefs
o.swiven: h.swinos
o.swiven: h.macros
o.screen:	c.screen
o.screen:	h.defs
o.screen:	C:h.kernel
o.screen:	h.inlines
o.screen:	C:h.swis
o.screen:	h.swicall
o.screen:	h.defs
o.screen:	C:h.swis
o.kill:	c.kill
o.kill:	h.defs
o.kill:	C:h.kernel
o.kill:	h.inlines
o.kill:	C:h.swis
o.kill:	h.wimp
o.kill:	h.defs
o.main:	c.main
o.main:	h.defs
o.main:	C:h.kernel
o.main:	h.inlines
o.main:	C:h.swis
o.main:	h.screen
o.main:	h.defs
o.main:	h.SmartCard
o.main:	h.wimp
o.main:	h.defs
o.main:	C:Sockets.h.in
