#
# Microsoft NMAKE file for src/ye
#
OMXROOT = m:
BUILDROOT = c:\mx

include m:\src\buildtools\winnt.mak

SRC	= $(SRCHOME)\mx\src

INCLUDE	=   /I $(OMXROOT)\src\ys\sosd\$(ARCH) \
	    /I $(OMXROOT)\pub \
	    /I $(OMXROOT)\inc \
	    /I $(OMXROOT)\src\inc \
	    /I $(OMXROOT)\src\ye \
	    /I $(OMXROOT)\src\yo

INTDIR = $(LIB)\ye32

COMPONENT = YE - MX Orb Event Logging and Event Channels

YESRC	  = $(OMXROOT)\src\ye
YDSRC	  = $(OMXROOT)\src\yd
YEOSD     = $(YESRC)\sosd\$(ARCH)
IDLSRC =    $(SRCHOME)\inc

MNIDLC_FLAGS = -S -a coa -I $(OMXROOT)\pub -s oracle -S -o $(YESRC)

IDLHDRS	= $(YESRC)\yeevent.h \
	  $(YESRC)\yeevlog.h $(YESRC)\yecevch.h \
	  $(YESRC)\ydidl.h $(INCSRC)\yeevent.h

MSGS =	    $(MSGDIR)\OMNYEEV.mot \
	    $(MSGDIR)\OMNYEEVLD.mot

LIBS =	    $(BIN)\ye32.dll $(LIB)\ye32.lib $(LIB)\ye32.exp $(LIB)\ye32.pdb

# libs for me to link with, beyond default LDLIBS.

DLLLIBES =  $(LIB)\yo32.lib $(LIB)\yotkyr32.lib \
	    $(LIB)\mn32.lib $(LIB)\mt32.lib $(LIB)\ys32.lib

LOADLIBES = $(DLLLIBES) $(LIB)\ye32.lib $(LIB)\yd32.lib

BINS =	$(BIN)\mnlogreader.exe \
	$(BIN)\mnlogsrv.exe \
	$(BIN)\mnlogctl.exe \
	$(BIN)\yeced.exe \
	$(BIN)\mnlogreader.pdb \
	$(BIN)\mnlogsrv.pdb \
	$(BIN)\mnlogctl.pdb \
	$(BIN)\yeced.pdb

# objects to be in the library.

LIBOBJS   = $(INTDIR)\yece.obj \
	    $(INTDIR)\yecevchS.obj \
	    $(INTDIR)\ydyoidlC.obj \
	    $(INTDIR)\yeev.obj \
	    $(INTDIR)\yeu.obj \
	    $(INTDIR)\yeeventS.obj \
	    $(INTDIR)\yeevlogS.obj \
	    $(INTDIR)\yemsg.obj \
	    $(INTDIR)\yeevf.obj \
	    $(INTDIR)\yeevlg.obj \
	    $(INTDIR)\ydidlC.obj \

OBJS =	    $(INTDIR)\yeced.obj \
	    $(INTDIR)\yeevld.obj \
	    $(INTDIR)\yeevctl.obj \
	    $(INTDIR)\yeevlr.obj

DIRS =	$(INTDIR)

all	 : $(DIRS) $(IDLHDRS) $(MSGS) $(LIBS) $(OBJS) $(BINS) 

msgs :	$(MSGS)

idlhdrs	 : $(IDLHDRS)

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

DEF_FILE = $(YEOSD)\dllye.def

RES_FILE = $(LIB)\ye32.res

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(LIB)\ye32.lib $(BIN)\ye32.dll : $(LIBOBJS) $(DEF_FILE) $(RES_FILE)
    @-echo Linking $@...
    $(LD) @<<
    $(LIBOBJS)
    $(DLLLIBES)
    $(LDLIBS)
    /OUT:$(BIN)\ye32.dll
    $(DLLFLAGS)
    $(LDDEBUG)
    $(RES_FILE)
    /pdb:$(LIB)\ye32.pdb
    /def:$(DEF_FILE)
    /implib:$(LIB)\ye32.lib
<<

$(LIB)\ye32.res: $(YEOSD)\ye32.rc
    rc /r /fo $(LIB)\ye32.res $(YEOSD)\ye32.rc

$(INTDIR)\yece.obj  : yece.c
	@$(COMPILE) /Fo$@ yece.c

$(INTDIR)\ydyoidlC.obj  : $(YDSRC)\ydyoidlC.c
	@$(COMPILE) /Fo$@ $(YDSRC)\ydyoidlC.c

$(INTDIR)\yecevchS.obj  : yecevchS.c
	@$(COMPILE) /Fo$@ yecevchS.c

$(INTDIR)\yeev.obj  : yeev.c
	@$(COMPILE) /Fo$@ yeev.c

$(INTDIR)\yeu.obj  : yeu.c
	@$(COMPILE) /Fo$@ yeu.c

$(INTDIR)\yeeventS.obj  : yeeventS.c
	@$(COMPILE) /Fo$@ yeeventS.c

$(INTDIR)\yeevlogS.obj  : yeevlogS.c
	@$(COMPILE) /Fo$@ yeevlogS.c

$(INTDIR)\yemsg.obj  : yemsg.c
	@$(COMPILE) /Fo$@ yemsg.c

$(INTDIR)\yeevf.obj  : yeevf.c
	@$(COMPILE) /Fo$@ yeevf.c

$(INTDIR)\yeevlg.obj  : yeevlg.c
	@$(COMPILE) /Fo$@ yeevlg.c

$(INTDIR)\ydidlC.obj  : ydidlC.c
	@$(COMPILE) /Fo$@ ydidlC.c

$(INTDIR)\yeced.obj  : yeced.c
	@$(COMPILE) /Fo$@ yeced.c

$(INTDIR)\yeevld.obj  : yeevld.c
	@$(COMPILE) /Fo$@ yeevld.c

$(INTDIR)\yeevctl.obj  : yeevctl.c
	@$(COMPILE) /Fo$@ yeevctl.c

$(INTDIR)\yeevlr.obj : yeevlr.c
	@$(COMPILE) /Fo$@ yeevlr.c

$(INCSRC)\yeevent.h: $(INCSRC)\yeevent.idl
	-$(YCIDL) -l -a coa -I $(OMXROOT)\pub -s oracle -o $(INCSRC) \
               $(INCSRC)\yeevent.idl

$(YESRC)\yeevent.h: $(INCSRC)\yeevent.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(INCSRC)\yeevent.idl

$(YESRC)\yecevch.h: $(OMXROOT)\pub\yecevch.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(OMXROOT)\pub\yecevch.idl

$(YESRC)\yeevlog.h: $(INCSRC)\yeevlog.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(INCSRC)\yeevlog.idl

$(YESRC)\ydidl.h: $(IDLSRC)\ydidl.idl
	-$(YCIDL) $(MNIDLC_FLAGS) $(IDLSRC)\ydidl.idl

$(MSGDIR)\OMNYEEV.mot : $(MSGDIR)\ye\OMNYEEV.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\ye\OMNYEEV.msg

$(MSGDIR)\OMNYEEVLD.mot : $(MSGDIR)\ye\OMNYEEVLD.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\ye\OMNYEEVLD.msg

$(BIN)\mnlogreader.exe : $(INTDIR)\yeevlr.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\yeevlr.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnlogsrv.exe : $(INTDIR)\yeevld.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\yeevld.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnlogctl.exe : $(INTDIR)\yeevctl.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\yeevctl.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\yeced.exe : $(INTDIR)\yeced.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\yeced.obj
	$(LOADLIBES) $(LDLIBS)
<<

