# Project:  Sennan


# Toolflags:
CCflags        = -c -depend !Depend -IC: -fussy -throwback
C++flags       = -c -depend !Depend -IC: -throwback
Linkflags      = -aif -v -c++ -o $@ 
DrLinkflags    = -aif -verbose -nounused -o $@ 
ObjAsmflags    = -throwback -NoCache -depend !Depend
CMHGflags      = 
LibFileflags   = -c -o $@
Squeezeflags   = -o $@
Datatoaofflags = -d data -l len -dp -s -p

FILES = o.heap o.main o.swicall o.screen o.render o.ncdlogon \
        o.ctutils o.textbox o.muldiv o.cat \
        o.Empty o.mainwindow o.DBox o.kill \
        o.AddToTail o.UnLink o.InList

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
o.heap:	c.heap
o.heap:	h.Defs
o.heap:	C:h.kernel
o.heap:	h.LinkList
o.heap:	h.Defs
o.heap:	h.inlines
o.heap:	h.NHWidget
o.heap:	C:h.swis
o.heap:	h.swicall
o.heap:	h.defs
o.heap:	C:h.swis
o.screen:	c.screen
o.screen:	h.defs
o.screen:	C:h.kernel
o.screen:	h.LinkList
o.screen:	h.Defs
o.screen:	h.inlines
o.screen:	h.NHWidget
o.screen:	C:h.swis
o.screen:	h.swicall
o.screen:	h.defs
o.screen:	C:h.swis
o.ncdlogon:	c.ncdlogon
o.ncdlogon:	h.defs
o.ncdlogon:	C:h.kernel
o.ncdlogon:	h.LinkList
o.ncdlogon:	h.Defs
o.ncdlogon:	h.internal
o.ncdlogon:	h.defs
o.ncdlogon:	h.swicall
o.ncdlogon:	h.defs
o.ncdlogon:	C:h.swis
o.ctutils:	c.ctutils
o.ctutils:	h.ctutils
o.ctutils:	h.defs
o.ctutils:	C:h.kernel
o.ctutils:	h.LinkList
o.ctutils:	h.Defs
o.ctutils:	C:h.kernel
o.ctutils:	h.defs
o.ctutils:	h.inlines
o.ctutils:	h.NHWidget
o.ctutils:	C:h.swis
o.ctutils:	h.swicall
o.ctutils:	h.defs
o.ctutils:	C:h.swis
o.textbox:	c.textbox
o.textbox:	h.ctutils
o.textbox:	h.defs
o.textbox:	C:h.kernel
o.textbox:	h.LinkList
o.textbox:	h.Defs
o.textbox:	C:h.kernel
o.textbox:	h.inlines
o.textbox:	h.NHWidget
o.textbox:	C:h.swis
o.textbox:	C:h.kernel
o.textbox:	h.muldiv
o.textbox:	h.nhwidget
o.textbox:	C:h.swis
o.textbox:	h.swicall
o.textbox:	h.defs
o.textbox:	C:h.swis
o.cat:	c.cat
o.cat:	h.Defs
o.cat:	C:h.kernel
o.cat:	h.LinkList
o.cat:	h.Defs
o.cat:	h.heap
o.cat:	h.defs
o.cat:	h.objects
o.cat:	h.defs
o.cat:	h.render
o.cat:	h.defs
o.cat:	h.screen
o.cat:	h.defs
o.cat:	h.swicall
o.cat:	h.defs
o.cat:	C:h.swis
o.AddToTail:	c.AddToTail
o.AddToTail:	h.defs
o.AddToTail:	C:h.kernel
o.AddToTail:	h.LinkList
o.AddToTail:	h.Defs
o.AddToTail:	h.LinkList
o.UnLink:	c.UnLink
o.UnLink:	h.defs
o.UnLink:	C:h.kernel
o.UnLink:	h.LinkList
o.UnLink:	h.Defs
o.UnLink:	h.LinkList
o.kill:	c.kill
o.kill:	h.defs
o.kill:	C:h.kernel
o.kill:	h.LinkList
o.kill:	h.Defs
o.kill:	h.inlines
o.kill:	h.NHWidget
o.kill:	C:h.swis
o.kill:	h.swicall
o.kill:	h.defs
o.kill:	C:h.swis
o.kill:	h.wimp
o.kill:	h.defs
o.main:	c.main
o.main:	h.defs
o.main:	C:h.kernel
o.main:	h.LinkList
o.main:	h.Defs
o.main:	h.heap
o.main:	h.defs
o.main:	h.inlines
o.main:	h.NHWidget
o.main:	C:h.swis
o.main:	h.LinkList
o.main:	h.muldiv
o.main:	h.NCDLogon
o.main:	h.NHWidget
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
o.InList:	c.InList
o.InList:	h.defs
o.InList:	C:h.kernel
o.InList:	h.LinkList
o.InList:	h.Defs
o.InList:	h.LinkList
o.render:	c.render
o.render:	h.defs
o.render:	C:h.kernel
o.render:	h.LinkList
o.render:	h.Defs
o.render:	h.heap
o.render:	h.defs
o.render:	h.inlines
o.render:	h.NHWidget
o.render:	C:h.swis
o.render:	h.swicall
o.render:	h.defs
o.render:	C:h.swis
