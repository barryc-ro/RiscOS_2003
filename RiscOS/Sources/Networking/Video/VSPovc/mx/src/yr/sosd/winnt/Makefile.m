#
# Microsoft NMAKE file for src/yr
#
OMXROOT = m:
BUILDROOT = c:\mx

include m:\src\buildtools\winnt.mak

SRC	= $(SRCHOME)\mx\src

INCLUDE	=   /I $(OMXROOT)\src\ys\sosd\$(ARCH) \
	    /I $(OMXROOT)\src\mn\sosd\$(ARCH) \
	    /I $(OMXROOT)\pub \
	    /I $(OMXROOT)\inc \
	    /I $(OMXROOT)\src\inc \
	    /I $(OMXROOT)\src\yr \
	    /I $(OMXROOT)\src\yo

INTDIR = $(LIB)\yr32

COMPONENT = YR - MX Interface repository

YRSRC	  = $(OMXROOT)\src\yr
YROSD     = $(YRSRC)\sosd\$(ARCH)
YOSRC	  = $(OMXROOT)\src\yo
YOOSD     = $(YOSRC)\sosd\$(ARCH)

YSSRC	  = $(OMXROOT)\src\ys
YSOSD     = $(YSSRC)\sosd\$(ARCH)
S0YSMAIN  = $(YSOSD)\s0ysmain.c

MSGS	= $(MSGDIR)/OMNYR.mot

YCIDLC_FLAGS =  -a coa -R yc.log.msg-path=$(MSGDIR)

# libs for me to link with, beyond default LDLIBS.

YOTKYRDLL   =	$(BIN)\yotkyr32.dll
YRDLL   =	$(BIN)\yr32.dll

LIBYOTKYR   =	$(LIB)\yotkyr32.lib
LIBYR   =	$(LIB)\yr32.lib

YOTKYRDLLLIBES = $(LIB)\mn32.lib $(LIB)\mt32.lib $(LIB)\ys32.lib

YRDLLLIBES  =	$(LIBYOTKYR) $(LIB)\ye32.lib $(LIB)\yo32.lib $(YOTKYRDLLLIBES) 

LOADLIBES =	 $(YRDLLLIBES) $(LIBYR)

LIBS =      $(BIN)\yotkyr32.dll \
            $(LIB)\yotkyr32.lib \
            $(LIB)\yotkyr32.exp \
            $(LIB)\yotkyr32.pdb \
      	    $(BIN)\yr32.dll \
            $(LIB)\yr32.lib \
            $(LIB)\yr32.exp \
            $(LIB)\yr32.pdb \

LIBYOTKYROBJS = $(INTDIR)\yrwrite.obj \
		$(INTDIR)\yotk.obj \
		$(INTDIR)\yrdump.obj \
		$(INTDIR)\yr.obj \
		$(INTDIR)\yotkg.obj \
		$(INTDIR)\yosx.obj \
		$(INTDIR)\yrtk.obj \
		$(INTDIR)\yrdst.obj \
		$(INTDIR)\yrc.obj 

LIBYRSVOBJS =	$(INTDIR)\yralias.obj \
		$(INTDIR)\yrarray.obj \
		$(INTDIR)\yrattr.obj \
		$(INTDIR)\yrconst.obj \
		$(INTDIR)\yrcontd.obj \
		$(INTDIR)\yrcontr.obj \
		$(INTDIR)\yrcorbaS.obj \
		$(INTDIR)\yrdii.obj \
		$(INTDIR)\yrdiidlS.obj \
		$(INTDIR)\yrenum.obj \
		$(INTDIR)\yrexcept.obj \
		$(INTDIR)\yridltyp.obj \
		$(INTDIR)\yrintf.obj \
		$(INTDIR)\yrirobj.obj \
		$(INTDIR)\yrmcl.obj \
		$(INTDIR)\yrmerge.obj \
		$(INTDIR)\yrmgidlS.obj \
		$(INTDIR)\yrmgr.obj \
		$(INTDIR)\yrmodule.obj \
		$(INTDIR)\yroper.obj \
		$(INTDIR)\yrprim.obj \
		$(INTDIR)\yrread.obj \
		$(INTDIR)\yrrepos.obj \
		$(INTDIR)\yrseq.obj \
		$(INTDIR)\yrsrv.obj \
		$(INTDIR)\yrstring.obj \
		$(INTDIR)\yrstruct.obj \
		$(INTDIR)\yrsvutil.obj \
		$(INTDIR)\yrtypdef.obj \
		$(INTDIR)\yrtypidlS.obj \
		$(INTDIR)\yridef.obj \
		$(INTDIR)\yridefS.obj \
		$(INTDIR)\yrunion.obj

LIBOBJS =   $(LIBYRSVOBJS) $(LIBYOTKYROBJS)

OBJS =	    $(INTDIR)\yrsvmain.obj $(INTDIR)\yrmmain.obj \
	    $(INTDIR)\yrcl.obj $(INTDIR)\s0yrcl.obj

BINS =	    $(BIN)\mnirsrv.exe $(BIN)\mnirctl.exe $(BIN)\mnircl.exe \
	    $(BIN)\mnirsrv.pdb $(BIN)\mnirctl.pdb $(BIN)\mnircl.pdb

DIRS =   $(INTDIR)

all	 : $(DIRS) $(LIBS) $(OBJS) $(BINS) $(MSGS)

yotkyr32 : $(YOTKYRDLL)

compile  : $(OBJS)

lib	 : $(INTDIR) $(LIBS)

link	 : $(BINS)

cleanobj :
	del $(LIBOBJS)
	del $(OBJS)

cleanlib :
	del $(LIBS)

cleanbin :
	del $(BINS)

clean:
	del $(BINS)
	del $(LIBS)
	del $(LIBOBJS)
	del $(OBJS)
	del $(INTDIR)\vc40.*

$(BINS): $(LOADLIBES)

$(INTDIR) :
    if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"

# ----------------------------------------------------------------
# dll construction

YRDEF_FILE = $(YROSD)\dllyr.def

YRRES_FILE = $(LIB)\yr32.res

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(YRDLL) $(LIB)\yr32.lib : $(LIBYRSVOBJS) $(YRDEF_FILE) $(YRRES_FILE)
    @-echo Linking $@...
    $(LD) @<<
    /OUT:$@
    $(DLLFLAGS)
    $(LDDEBUG)
    $(LIBYRSVOBJS)
    $(YRDLLLIBES)
    $(LDLIBS)
    $(YRRES_FILE)
    /pdb:$(LIB)\yr32.pdb
    /def:$(YRDEF_FILE)
    /implib:$(LIB)\yr32.lib
<<

$(LIB)\yr32.res: $(YROSD)\yr32.rc
    rc /r /fo $(LIB)\yr32.res $(YROSD)\yr32.rc

YOTKYRDEF_FILE = $(YOOSD)\dllyotkyr.def

YOTKYRRES_FILE = $(LIB)\yotkyr32.res

$(YOTKYRDLL) $(LIB)\yotkyr32.lib: $(LIBYOTKYROBJS) $(YOTKYRDEF_FILE) $(YOTKYRRES_FILE)
    @-echo Linking $@...
    $(LD) @<<
    /OUT:$(YOTKYRDLL)
    $(DLLFLAGS)
    $(LDDEBUG)
    $(LIBYOTKYROBJS)
    $(YOTKYRDLLLIBES)
    $(LDLIBS)
    $(YOTKYRRES_FILE)
    /pdb:$(LIB)\yotkyr32.pdb
    /def:$(YOTKYRDEF_FILE)
    /implib:$(LIB)\yotkyr32.lib
<<

$(LIB)\yotkyr32.res: $(YROSD)\yotkyr32.rc
    rc /r /fo $(LIB)\yotkyr32.res $(YROSD)\yotkyr32.rc

# ----------------------------------------------------------------
# lib objs

$(INTDIR)\yrwrite.obj : yrwrite.c
	@$(COMPILE) /Fo$@ yrwrite.c

$(INTDIR)\yotk.obj  : $(YOSRC)\yotk.c
	@$(COMPILE) /Fo$@ $(YOSRC)\yotk.c

$(INTDIR)\yosx.obj  : $(YOSRC)\yosx.c
	@$(COMPILE) /Fo$@ $(YOSRC)\yosx.c

$(INTDIR)\yrdump.obj  : yrdump.c
	@$(COMPILE) /Fo$@ yrdump.c

$(INTDIR)\yr.obj  : yr.c
	@$(COMPILE) /Fo$@ yr.c

$(INTDIR)\yotkg.obj  : $(YOSRC)\yotkg.c
	@$(COMPILE) /Fo$@ $(YOSRC)\yotkg.c

$(INTDIR)\yrtk.obj  : yrtk.c
	@$(COMPILE) /Fo$@ yrtk.c

$(INTDIR)\yrdst.obj : yrdst.c
	@$(COMPILE) /Fo$@ yrdst.c

$(INTDIR)\yrc.obj  : yrc.c
	@$(COMPILE) /Fo$@ yrc.c

$(INTDIR)\yralias.obj : yralias.c
    @$(COMPILE) /Fo$@ yralias.c

$(INTDIR)\yrarray.obj : yrarray.c
    @$(COMPILE) /Fo$@ yrarray.c

$(INTDIR)\yrattr.obj : yrattr.c
    @$(COMPILE) /Fo$@ yrattr.c

$(INTDIR)\yrconst.obj : yrconst.c
    @$(COMPILE) /Fo$@ yrconst.c

$(INTDIR)\yrcontd.obj : yrcontd.c
    @$(COMPILE) /Fo$@ yrcontd.c

$(INTDIR)\yrcontr.obj : yrcontr.c
    @$(COMPILE) /Fo$@ yrcontr.c

$(INTDIR)\yrcorbaS.obj : yrcorbaS.c
    @$(COMPILE) /Fo$@ yrcorbaS.c

$(INTDIR)\yrdii.obj : yrdii.c
    @$(COMPILE) /Fo$@ yrdii.c

$(INTDIR)\yrdiidlS.obj : yrdiidlS.c
    @$(COMPILE) /Fo$@ yrdiidlS.c

$(INTDIR)\yrenum.obj : yrenum.c
    @$(COMPILE) /Fo$@ yrenum.c

$(INTDIR)\yrexcept.obj : yrexcept.c
    @$(COMPILE) /Fo$@ yrexcept.c

$(INTDIR)\yridltyp.obj : yridltyp.c
    @$(COMPILE) /Fo$@ yridltyp.c

$(INTDIR)\yrintf.obj : yrintf.c
    @$(COMPILE) /Fo$@ yrintf.c

$(INTDIR)\yrirobj.obj : yrirobj.c
    @$(COMPILE) /Fo$@ yrirobj.c

$(INTDIR)\yrmcl.obj : yrmcl.c
    @$(COMPILE) /Fo$@ yrmcl.c

$(INTDIR)\yrmerge.obj : yrmerge.c
    @$(COMPILE) /Fo$@ yrmerge.c

$(INTDIR)\yrmgidlS.obj : yrmgidlS.c
    @$(COMPILE) /Fo$@ yrmgidlS.c

$(INTDIR)\yrmgr.obj : yrmgr.c
    @$(COMPILE) /Fo$@ yrmgr.c

$(INTDIR)\yrmodule.obj : yrmodule.c
    @$(COMPILE) /Fo$@ yrmodule.c

$(INTDIR)\yroper.obj : yroper.c
    @$(COMPILE) /Fo$@ yroper.c

$(INTDIR)\yrprim.obj : yrprim.c
    @$(COMPILE) /Fo$@ yrprim.c

$(INTDIR)\yrread.obj : yrread.c
    @$(COMPILE) /Fo$@ yrread.c

$(INTDIR)\yrrepos.obj : yrrepos.c
    @$(COMPILE) /Fo$@ yrrepos.c

$(INTDIR)\yrseq.obj : yrseq.c
    @$(COMPILE) /Fo$@ yrseq.c

$(INTDIR)\yrsrv.obj : yrsrv.c
    @$(COMPILE) /Fo$@ yrsrv.c

$(INTDIR)\yrstring.obj : yrstring.c
    @$(COMPILE) /Fo$@ yrstring.c

$(INTDIR)\yrstruct.obj : yrstruct.c
    @$(COMPILE) /Fo$@ yrstruct.c

$(INTDIR)\yrsvutil.obj : yrsvutil.c
    @$(COMPILE) /Fo$@ yrsvutil.c

$(INTDIR)\yrtypdef.obj : yrtypdef.c
    @$(COMPILE) /Fo$@ yrtypdef.c

$(INTDIR)\yrtypidlS.obj : yrtypidlS.c
    @$(COMPILE) /Fo$@ yrtypidlS.c

$(INTDIR)\yrunion.obj: yrunion.c
    @$(COMPILE) /Fo$@ yrunion.c

$(INTDIR)\yridef.obj: yridef.c
    @$(COMPILE) /Fo$@ yridef.c

$(INTDIR)\yridefS.obj: yridefS.c
    @$(COMPILE) /Fo$@ yridefS.c

# ----------------------------------------------------------------
# main entry points

$(INTDIR)\yrcl.obj: yrcl.c
    @$(COMPILE) /Fo$@ yrcl.c

$(INTDIR)\s0yrcl.obj : $(S0YSMAIN)
	@$(COMPILE) /Fo$@ /D ENTRY_POINT=yrclMain $(S0YSMAIN)

$(INTDIR)\yrsvmain.obj : $(S0YSMAIN)
	@$(COMPILE) /Fo$@ /D ENTRY_POINT=yrsrvMain $(S0YSMAIN)

$(INTDIR)\yrmmain.obj : $(S0YSMAIN)
	@$(COMPILE) /Fo$@ /D ENTRY_POINT=yrmMain $(S0YSMAIN)

# ----------------------------------------------------------------
# programs

$(BIN)\mnirsrv.exe : $(INTDIR)\yrsvmain.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\yrsvmain.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnirctl.exe : $(INTDIR)\yrmmain.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\yrmmain.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnircl.exe : $(INTDIR)\yrcl.obj $(INTDIR)\s0yrcl.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\yrcl.obj $(INTDIR)\s0yrcl.obj
	$(LOADLIBES) $(LDLIBS)
<<


$(MSGDIR)\OMNYR.mot : $(MSGDIR)\yr\OMNYR.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yr\OMNYR.msg

