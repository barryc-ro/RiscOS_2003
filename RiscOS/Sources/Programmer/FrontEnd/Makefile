# Project:   FrontEnd


# Toolflags:
CCflags = -c -depend !Depend -IC: -zM -ffah -DTEST_RELEASE 
Linkflags = -m -o $@ 
ObjAsmflags = -depend !Depend -Stamp -quit -CloseExec
CMHGflags = 
LibFileflags = -c -o $@
Squeezeflags = -o $@
AAsmflags = -depend !Depend -quit -CloseExec -To $@ -From


# Final targets:
@.FrontEnd:   @.FrontEndU 
        Squeeze $(Squeezeflags) @.FrontEndU 
@.FrontEndU:   @.o.FEbkg @.o.FEcmdline @.o.FEdbox @.o.FEexit @.o.FEfileicon \
        @.o.FEhelp @.o.FEiconbar @.o.FEinfo @.o.FEinit @.o.FEinterr @.o.FEmem @.o.FEmenus \
        @.o.FEmsg @.o.FEoptions @.o.FEoutput @.o.FEsaveas @.o.FEtask @.o.FEtxtwin @.o.frontend \
        @.o.gen @.o.lexer @.o.parser @.o.slist @.o.utils @.o.base \
        C:rm.RISC_OSLib C:o.Stubs @.o.FEhdr @.o.dirs
        Link $(Linkflags) -via objfiles 


# User-editable dependencies:
clean:; remove FrontEndU
        remove FrontEnd
        wipe o.*

@.o.dirs:
	cdir @.o
	create $@

release: @.FrontEnd
        delete @.o.FEhdr

# Static dependencies:
@.o.FEbkg:   @.c.FEbkg
        cc $(ccflags) -o @.o.FEbkg @.c.FEbkg 
@.o.FEcmdline:   @.c.FEcmdline
        cc $(ccflags) -o @.o.FEcmdline @.c.FEcmdline 
@.o.FEdbox:   @.c.FEdbox
        cc $(ccflags) -o @.o.FEdbox @.c.FEdbox 
@.o.FEexit:   @.c.FEexit
        cc $(ccflags) -o @.o.FEexit @.c.FEexit 
@.o.FEfileicon:   @.c.FEfileicon
        cc $(ccflags) -o @.o.FEfileicon @.c.FEfileicon 
@.o.FEhelp:   @.c.FEhelp
        cc $(ccflags) -o @.o.FEhelp @.c.FEhelp 
@.o.FEiconbar:   @.c.FEiconbar
        cc $(ccflags) -o @.o.FEiconbar @.c.FEiconbar 
@.o.FEinfo:   @.c.FEinfo
        cc $(ccflags) -o @.o.FEinfo @.c.FEinfo 
@.o.FEinit:   @.c.FEinit
        cc $(ccflags) -o @.o.FEinit @.c.FEinit 
@.o.FEinterr:   @.c.FEinterr
        cc $(ccflags) -o @.o.FEinterr @.c.FEinterr 
@.o.FEmem:   @.c.FEmem
        cc $(ccflags) -o @.o.FEmem @.c.FEmem 
@.o.FEmenus:   @.c.FEmenus
        cc $(ccflags) -o @.o.FEmenus @.c.FEmenus 
@.o.FEmsg:   @.c.FEmsg
        cc $(ccflags) -o @.o.FEmsg @.c.FEmsg 
@.o.FEoptions:   @.c.FEoptions
        cc $(ccflags) -o @.o.FEoptions @.c.FEoptions 
@.o.FEoutput:   @.c.FEoutput
        cc $(ccflags) -o @.o.FEoutput @.c.FEoutput 
@.o.FEsaveas:   @.c.FEsaveas
        cc $(ccflags) -o @.o.FEsaveas @.c.FEsaveas 
@.o.FEtask:   @.c.FEtask
        cc $(ccflags) -o @.o.FEtask @.c.FEtask 
@.o.FEtxtwin:   @.c.FEtxtwin
        cc $(ccflags) -o @.o.FEtxtwin @.c.FEtxtwin 
@.o.frontend:   @.c.frontend
        cc $(ccflags) -o @.o.frontend @.c.frontend 
@.o.gen:   @.c.gen
        cc $(ccflags) -o @.o.gen @.c.gen 
@.o.lexer:   @.c.lexer
        cc $(ccflags) -o @.o.lexer @.c.lexer 
@.o.parser:   @.c.parser
        cc $(ccflags) -o @.o.parser @.c.parser 
@.o.slist:   @.c.slist
        cc $(ccflags) -o @.o.slist @.c.slist 
@.o.utils:   @.c.utils
        cc $(ccflags) -o @.o.utils @.c.utils 
@.o.base:   @.s.base
        objasm $(objasmflags) -from @.s.base -to @.o.base
@.o.FEhdr:   @.cmhg.FEhdr
        cmhg @.cmhg.FEhdr -o @.o.FEhdr


# Dynamic dependencies:
