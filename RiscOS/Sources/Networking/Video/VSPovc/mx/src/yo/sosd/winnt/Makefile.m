#
# Microsoft NMAKE file for src/yo
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
	    /I $(OMXROOT)\src\yo \
	    /I $(OMXROOT)\src\mn \
	    $(BSAFEINC)

INTDIR = $(LIB)\yo32

COMPONENT = YO - MX Object Layer

YOSRC	  = $(OMXROOT)\src\yo
YOOSD     = $(YOSRC)\sosd\$(ARCH)
IDLSRC =    $(SRCHOME)\inc

MNIDLC_FLAGS = -a coa -I $(OMXROOT)\pub -s oracle -o $(YOSRC)

IDLHDRS =   $(YOSRC)\yoidl.h $(YOSRC)\ydyoidl.h \
            $(YOSRC)\yotrans.h $(YOSRC)\yoevt.h $(YOSRC)\yostd.h

MSGS	=   $(MSGDIR)\OMNYO.mot

LIBS =	    $(BIN)\yo32.dll $(LIB)\yo32.lib $(LIB)\yo32.exp 

# libs for me to link with, beyond default LDLIBS.

DLLLIBES =  $(LIB)\mn32.lib $(LIB)\mt32.lib $(LIB)\ys32.lib

LOADLIBES = $(LIB)\yotkyr32.lib $(LIB)\yoex32.lib $(DLLLIBES) $(LIB)\ys32.lib

BINS = nothing

# objects to be in the library.

LIBOBJS   = $(INTDIR)\yo.obj \
	    $(INTDIR)\yoyd.obj \
	    $(INTDIR)\yocoa.obj \
	    $(INTDIR)\yogiop.obj \
	    $(INTDIR)\yox.obj \
	    $(INTDIR)\yort.obj \
	    $(INTDIR)\yobr.obj \
	    $(INTDIR)\yosx.obj \
	    $(INTDIR)\yot.obj \
	    $(INTDIR)\yoevt.obj \
	    $(INTDIR)\yoidlS.obj \
	    $(INTDIR)\yoevtS.obj \
	    $(INTDIR)\ydyoidlC.obj \
	    $(INTDIR)\yostdC.obj \
	    $(INTDIR)\yotkx.obj \
	    $(INTDIR)\yoorb.obj \
	    $(INTDIR)\yoboa.obj \
            $(INTDIR)\yocy.obj \
            $(INTDIR)\yotransC.obj

LIBEXOBJS = $(INTDIR)\yorc5x.obj

LIBBSOBJS = $(INTDIR)\yorc5.obj $(INTDIR)\yobs.obj

OBJS = nothing

LIBS =	$(BIN)\yoex32.dll $(LIB)\yoex32.lib \
                 $(LIB)\yoex32.exp $(LIB)\yoex32.pdb \
        $(BIN)\yo32.dll $(LIB)\yo32.lib $(LIB)\yo32.exp 
CRYPTLIB = $(BIN)\yobs32.dll $(LIB)\yobs32.lib

DIRS =	$(INTDIR)

all	 : $(DIRS) $(IDLHDRS) $(LIBS) $(OBJS) $(BINS) $(MSGS)

idlhdrs	 : $(IDLHDRS)

compile  : $(OBJS)

lib	 : $(INTDIR) $(LIBS)

crypto	: $(CRYPTLIB)

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

# $(BINS): $(LOADLIBES)

$(CRYPTLIB): $(BIN)\yobs32.dll $(LIB)\yobs32.lib \
		    $(LIB)\yobs32.exp $(LIB)\yobs32.pdb

$(INTDIR) :
    if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"

$(YOSRC)\ydyoidl.h ydyoidlC.c ydyoidlS.c: $(IDLSRC)\ydyoidl.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(IDLSRC)\ydyoidl.idl

$(YOSRC)\yoidl.h yoidlS.c yoidlC.c: $(IDLSRC)\yoidl.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(IDLSRC)\yoidl.idl

$(YOSRC)\yoevt.h yoevtS.c yoevtC.c: $(YOSRC)\yoevt.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(YOSRC)\yoevt.idl

$(YOSRC)\yostd.h yostdC.c yostdS.c: $(OMXROOT)\pub\yostd.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(OMXROOT)\pub\yostd.idl

$(YOSRC)\yotrans.h yotransC.c yotransS.c: $(IDLSRC)\yotrans.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(IDLSRC)\yotrans.idl

$(MSGDIR)\OMNYO.mot : $(MSGDIR)\yo\OMNYO.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yo\OMNYO.msg

# ----------------------------------------------------------------

YODEF_FILE = $(YOOSD)\dllyo.def

YORES_FILE = $(LIB)\yo32.res

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(LIB)\yo32.lib $(BIN)\yo32.dll : $(LIBOBJS) $(YODEF_FILE) $(YORES_FILE)
    @-echo Linking $@...
    $(LD) @<<
    $(LIBOBJS)
    $(LOADLIBES)
    $(LDLIBS)
    /OUT:$(BIN)\yo32.dll
    $(DLLFLAGS)
    $(LDDEBUG)
    $(YORES_FILE)
    /def:$(YODEF_FILE)
    /implib:$(LIB)\yo32.lib
<<

$(LIB)\yo32.res: $(YOOSD)\yo32.rc
    rc /r /fo $(LIB)\yo32.res $(YOOSD)\yo32.rc

# ----------------------------------------------------------------

YOEXDEF_FILE = $(YOOSD)\dllyoex.def

YOEXRES_FILE = $(LIB)\yoex32.res

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(LIB)\yoex32.lib $(BIN)\yoex32.dll : $(LIBEXOBJS) $(YOEXDEF_FILE) $(YOEXRES_FILE)
    @-echo Linking $@...
    $(LD) @<<
    $(LIBEXOBJS)
    $(DLLLIBES)
    $(LDLIBS)
    /OUT:$(BIN)\yoex32.dll
    $(DLLFLAGS)
    $(LDDEBUG)
    $(YOEXRES_FILE)
    /pdb:$(LIB)\yoex32.pdb
    /def:$(YOEXDEF_FILE)
    /implib:$(LIB)\yoex32.lib
<<

$(LIB)\yoex32.res: $(YOOSD)\yoex32.rc
    rc /r /fo $(LIB)\yoex32.res $(YOOSD)\yoex32.rc
# ----------------------------------------------------------------

YOBSDEF_FILE = $(YOOSD)\dllyobs.def

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(LIB)\yobs32.lib $(BIN)\yobs32.dll : $(LIBBSOBJS) $(YOBSDEF_FILE)
    @-echo Linking $@...
    $(LD) @<<
    $(LIBBSOBJS)
    $(DLLLIBES)
    $(LDLIBS)
    /OUT:$(BIN)\yobs32.dll
    $(DLLFLAGS)
    $(LDDEBUG)
    /pdb:$(LIB)\yobs32.pdb
    /def:$(YOBSDEF_FILE)
    /implib:$(LIB)\yobs32.lib
<<

# ----------------------------------------------------------------

$(INTDIR)\yo.obj : yo.c
	@$(COMPILE) /Fo$@ yo.c

$(INTDIR)\yoyd.obj  : yoyd.c
	@$(COMPILE) /Fo$@ yoyd.c

$(INTDIR)\yocoa.obj : yocoa.c
	@$(COMPILE)  /Fo$@ yocoa.c

$(INTDIR)\yogiop.obj : yogiop.c
	@$(COMPILE) /Fo$@ yogiop.c

$(INTDIR)\yox.obj  : yox.c
	@$(COMPILE) /Fo$@ yox.c

$(INTDIR)\yort.obj : yort.c
	@$(COMPILE) /Fo$@ yort.c

$(INTDIR)\yobr.obj : yobr.c
	@$(COMPILE) /Fo$@ yobr.c

$(INTDIR)\yosx.obj  : yosx.c
	@$(COMPILE) /Fo$@ yosx.c

$(INTDIR)\yot.obj  : yot.c
	@$(COMPILE) /Fo$@ yot.c

$(INTDIR)\yoevt.obj : yoevt.c
	@$(COMPILE) /Fo$@ yoevt.c

$(INTDIR)\yoidlS.obj : yoidlS.c
	@$(COMPILE) /Fo$@ yoidlS.c

$(INTDIR)\yoevtS.obj : yoevtS.c
	@$(COMPILE) /Fo$@ yoevtS.c

$(INTDIR)\ydyoidlC.obj : ydyoidlC.c
	@$(COMPILE) /Fo$@ ydyoidlC.c

$(INTDIR)\yostdC.obj  : yostdC.c
	@$(COMPILE) /Fo$@ yostdC.c

$(INTDIR)\yotkx.obj  : yotkx.c
	@$(COMPILE) /Fo$@ yotkx.c

$(INTDIR)\yoorb.obj : yoorb.c
	@$(COMPILE) /Fo$@ yoorb.c

$(INTDIR)\yoboa.obj : yoboa.c
	@$(COMPILE) /Fo$@ yoboa.c

$(INTDIR)\yorc5.obj : yorc5.c
	@$(COMPILE) /Fo$@ yorc5.c

$(INTDIR)\yorc5x.obj : yorc5x.c
	@$(COMPILE) /Fo$@ yorc5x.c

$(INTDIR)\yobs.obj : yobs.c
	@$(COMPILE) /Fo$@ yobs.c

$(INTDIR)\yocy.obj : yocy.c
	@$(COMPILE) /Fo$@ yocy.c

$(INTDIR)\yotransC.obj : yotransC.c
	@$(COMPILE) /Fo$@ yotransC.c
