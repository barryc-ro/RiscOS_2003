#
# Microsoft NMAKE file for src/yd
#
OMXROOT = m:
BUILDROOT = c:\mx

include m:\src\buildtools\winnt.mak

TNSDRV    = Y:
TNSLIBDRV = K:
TNSINC    = /I $(TNSDRV)\Oracore3\public \
            /I $(TNSDRV)\network\include \
            /I $(TNSDRV)\RDBMS\public \
            /I $(TNSDRV)\RDBMS\include \
            /I $(TNSDRV)\RDBMS\src\server\osds\nt_pls \
            /I $(TNSDRV)\RDBMS\src\server\osds\nt_gen \
            /I $(TNSDRV)\nlsrtl3\public \
            /I $(TNSDRV)\trace\public 

SRC	= $(SRCHOME)\mx\src

INCLUDE	=   /I $(OMXROOT)\src\ys\sosd\$(ARCH) \
	    /I $(OMXROOT)\pub \
	    /I $(OMXROOT)\inc \
	    /I $(OMXROOT)\src\inc \
	    /I $(OMXROOT)\src\yd \
	    /I $(OMXROOT)\src\yo

INTDIR = $(LIB)\yd32

COMPONENT = YD - MX Orb Daemons and Utility Programs

YDSRC	  = $(OMXROOT)\src\yd
YDSRC	  = $(OMXROOT)\src\yd
YDOSD     = $(YDSRC)\sosd\$(ARCH)
IDLSRC =    $(SRCHOME)\inc

MNIDLC_FLAGS = -a coa -I $(OMXROOT)\pub -s oracle -o $(YDSRC)

IDLHDRS =   $(YDSRC)\ydidl.h $(YDSRC)\yoidl.h \
	    $(YDSRC)\ydyoidl.h $(YDSRC)\ydmtdidl.h \
            $(YDSRC)\ydnmidl.h $(YDSRC)\yostd.h $(YDSRC)\ydbri.h

MSGS	=   $(MSGDIR)\OMNYD.mot \
	    $(MSGDIR)\OMNYDIM.mot \
	    $(MSGDIR)\OMNYDRT.mot \
	    $(MSGDIR)\OMNYDCA.mot \
	    $(MSGDIR)\OMNYDSP.mot \
	    $(MSGDIR)\OMNYDMT.mot \
	    $(MSGDIR)\OMNYDUT.mot

LIBS =	    $(BIN)\yd32.dll $(LIB)\yd32.lib $(LIB)\yd32.exp $(LIB)\yd32.pdb

# libs for me to link with, beyond default LDLIBS.

DLLLIBES =  $(LIB)\ye32.lib \
	    $(LIB)\yo32.lib $(LIB)\yotkyr32.lib \
	    $(LIB)\mn32.lib $(LIB)\mt32.lib $(LIB)\ys32.lib

LOADLIBES = $(DLLLIBES) $(LIB)\yd32.lib

TNSLIBS   = $(TNSLIBDRV)\sqlnet\30200\win32\dlib.012897\ns30.lib \
            $(TNSLIBDRV)\sqlnet\30200\win32\dlib.012897\nt30.lib \
            $(TNSLIBDRV)\sqlnet\30200\win32\dlib.012897\ntt30.lib \
            $(TNSLIBDRV)\sqlnet\30200\win32\dlib.012897\nl30.lib \
            $(TNSLIBDRV)\sqlnet\30200\win32\dlib.012897\nasns30.lib \
            v:\oracore4\lib\core40.lib \
            v:\nlsrtl3\lib\nlsrtl33.lib \
            v:\rdbms\lib\ora802.lib \
            $(TNSLIBDRV)\TRACE\80200\win32\lib\dbg\otrace80.lib


#            $(TNSLIBDRV)\Oracore3\40200\win32\lib\core40.lib \
#            $(TNSLIBDRV)\NLSRTL3\33000_802\win32\lib\nlsrtl33.lib \
#            $(TNSLIBDRV)\RDBMS\80200\win32\lib\ora802.lib \
#            $(TNSLIBDRV)\TRACE\80200\win32\lib\dbg\otrace80.lib

BINS =	$(BIN)\mnorbsrv.exe \
	$(BIN)\mnorbmet.exe \
	$(BIN)\mnnmsrv.exe \
	$(BIN)\mnorbls.exe \
	$(BIN)\mnorbctl.exe \
	$(BIN)\mnorbadm.exe \
	$(BIN)\mnprocstat.exe \
	$(BIN)\mnobjadm.exe \
!IFNDEF NO_BRIDGE
	$(BIN)\mniiopbrsrv.exe \
	$(BIN)\mniiopbrsrv.pdb \
!ENDIF
	$(BIN)\mnorbsrv.pdb \
	$(BIN)\mnorbmet.pdb \
	$(BIN)\mnnmsrv.pdb \
	$(BIN)\mnorbls.pdb \
	$(BIN)\mnorbctl.pdb \
	$(BIN)\mnorbadm.pdb \
	$(BIN)\mnprocstat.pdb \
	$(BIN)\mnobjadm.pdb 

# objects to be in the library.

LIBOBJS   = $(INTDIR)\ydim.obj \
	    $(INTDIR)\ydca.obj \
	    $(INTDIR)\ydch.obj \
	    $(INTDIR)\ydmt.obj \
	    $(INTDIR)\ydsp.obj \
	    $(INTDIR)\ydrt.obj \
	    $(INTDIR)\ydnm.obj \
	    $(INTDIR)\ydidlS.obj \
	    $(INTDIR)\ydyoidlS.obj \
	    $(INTDIR)\ydmtdidlS.obj \
	    $(INTDIR)\ydnmidlS.obj \
	    $(INTDIR)\ydu.obj \
	    $(INTDIR)\ydq.obj \
	    $(INTDIR)\ydbriC.obj 

OBJS =	    $(INTDIR)\yd.obj \
	    $(INTDIR)\ydmtd.obj \
	    $(INTDIR)\ydnmd.obj \
	    $(INTDIR)\ydcf.obj \
	    $(INTDIR)\ydls.obj \
	    $(INTDIR)\ydspps.obj \
	    $(INTDIR)\ydadm.obj \
	    $(INTDIR)\ydchadm.obj \
!IFNDEF NO_BRIDGE
	    $(INTDIR)\ydbr.obj \
!ENDIF
	    $(INTDIR)\ydbriS.obj

DIRS =  $(INTDIR)

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

DEF_FILE = $(YDOSD)\dllyd.def

RES_FILE = $(LIB)\yd32.res

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(LIB)\yd32.lib $(BIN)\yd32.dll : $(LIBOBJS) $(DEF_FILE) $(RES_FILE)
    @-echo Linking $@...
    $(LD) @<<
    $(LIBOBJS)
    $(DLLLIBES)
    $(LDLIBS)
    /OUT:$(BIN)\yd32.dll
    $(DLLFLAGS)
    $(LDDEBUG)
    $(RES_FILE)
    /pdb:$(LIB)\yd32.pdb
    /def:$(DEF_FILE)
    /implib:$(LIB)\yd32.lib
<<

$(LIB)\yd32.res: $(YDOSD)\yd32.rc
    rc /r /fo $(LIB)\yd32.res $(YDOSD)\yd32.rc

# idl files...

$(YDSRC)\yoidl.h: $(IDLSRC)\yoidl.idl
	-$(YCIDL) $(MNIDLC_FLAGS) -S $(IDLSRC)\yoidl.idl

$(YDSRC)\ydidl.h: $(IDLSRC)\ydidl.idl
	-$(YCIDL) $(MNIDLC_FLAGS) -S $(IDLSRC)\ydidl.idl

$(YDSRC)\ydyoidl.h: $(IDLSRC)\ydyoidl.idl
	-$(YCIDL) $(MNIDLC_FLAGS) -S $(IDLSRC)\ydyoidl.idl

$(YDSRC)\ydmtdidl.h: $(IDLSRC)\ydmtdidl.idl
	-$(YCIDL) $(MNIDLC_FLAGS) -S $(IDLSRC)\ydmtdidl.idl

$(YDSRC)\ydnmidl.h: $(OMXROOT)\pub\ydnmidl.idl
	-$(YCIDL) $(MNIDLC_FLAGS) -S $(OMXROOT)\pub\ydnmidl.idl

$(YDSRC)\yostd.h: $(OMXROOT)\pub\yostd.idl
	-$(YCIDL) $(MNIDLC_FLAGS) -S $(OMXROOT)\pub\yostd.idl

$(YDSRC)\ydbri.h: $(IDLSRC)\ydbri.idl
	-$(YCIDL) $(MNIDLC_FLAGS) -S $(IDLSRC)\ydbri.idl

# messages...

$(MSGDIR)\OMNYD.mot : $(MSGDIR)\yd\OMNYD.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yd\OMNYD.msg
$(MSGDIR)\OMNYDIM.mot : $(MSGDIR)\yd\OMNYDIM.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yd\OMNYDIM.msg
$(MSGDIR)\OMNYDRT.mot : $(MSGDIR)\yd\OMNYDRT.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yd\OMNYDRT.msg
$(MSGDIR)\OMNYDCA.mot : $(MSGDIR)\yd\OMNYDCA.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yd\OMNYDCA.msg
$(MSGDIR)\OMNYDSP.mot : $(MSGDIR)\yd\OMNYDSP.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yd\OMNYDSP.msg
$(MSGDIR)\OMNYDMT.mot : $(MSGDIR)\yd\OMNYDMT.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yd\OMNYDMT.msg
$(MSGDIR)\OMNYDUT.mot : $(MSGDIR)\yd\OMNYDUT.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yd\OMNYDUT.msg

$(INTDIR)\ydim.obj : ydim.c
    @$(COMPILE) /Fo$@ ydim.c

$(INTDIR)\ydca.obj : ydca.c
    @$(COMPILE) /Fo$@ ydca.c

$(INTDIR)\ydch.obj : ydch.c
    @$(COMPILE) /Fo$@ ydch.c

$(INTDIR)\ydmt.obj : ydmt.c
    @$(COMPILE) /Fo$@ ydmt.c

$(INTDIR)\ydsp.obj : ydsp.c
    @$(COMPILE) /Fo$@ ydsp.c

$(INTDIR)\ydrt.obj : ydrt.c
    @$(COMPILE) /Fo$@ ydrt.c

$(INTDIR)\ydnm.obj : ydnm.c
    @$(COMPILE) /Fo$@ ydnm.c

$(INTDIR)\ydidlS.obj : ydidlS.c
    @$(COMPILE) /Fo$@ ydidlS.c

$(INTDIR)\ydyoidlS.obj : ydyoidlS.c
    @$(COMPILE) /Fo$@ ydyoidlS.c

$(INTDIR)\ydmtdidlS.obj : ydmtdidlS.c
    @$(COMPILE) /Fo$@ ydmtdidlS.c

$(INTDIR)\ydnmidlS.obj : ydnmidlS.c
    @$(COMPILE) /Fo$@ ydnmidlS.c

$(INTDIR)\ydu.obj : ydu.c
    @$(COMPILE) /Fo$@ ydu.c

$(INTDIR)\ydq.obj: ydq.c
    @$(COMPILE) /Fo$@ ydq.c

$(INTDIR)\yd.obj : yd.c
    @$(COMPILE) /Fo$@ yd.c

$(INTDIR)\ydmtd.obj : ydmtd.c
    @$(COMPILE) /Fo$@ ydmtd.c

$(INTDIR)\ydnmd.obj : ydnmd.c
    @$(COMPILE) /Fo$@ ydnmd.c

$(INTDIR)\ydcf.obj : ydcf.c
    @$(COMPILE) /Fo$@ ydcf.c

$(INTDIR)\ydls.obj : ydls.c
    @$(COMPILE) /Fo$@ ydls.c

$(INTDIR)\ydspps.obj : ydspps.c
    @$(COMPILE) /Fo$@ ydspps.c

$(INTDIR)\ydadm.obj: ydadm.c
    @$(COMPILE) /Fo$@ ydadm.c

$(INTDIR)\ydchadm.obj: ydchadm.c
    @$(COMPILE) /Fo$@ ydchadm.c

$(INTDIR)\ydbr.obj: ydbr.c
    @$(COMPILE) /D "WIN_NT" /U "_WINDOWS" /D "NS_TRACE" $(TNSINC) /Fo$@ ydbr.c

$(INTDIR)\ydyoidlC.obj: ydyoidlC.c
    @$(COMPILE) /Fo$@ ydyoidlC.c

$(INTDIR)\ydidlC.obj: ydidlC.c
    @$(COMPILE) /Fo$@ ydidlC.c

$(INTDIR)\ydmtdidlC.obj: ydmtdidlC.c
    @$(COMPILE) /Fo$@ ydmtdidlC.c

$(INTDIR)\ydnmidlC.obj: ydnmidlC.c
    @$(COMPILE) /Fo$@ ydnmidlC.c

$(INTDIR)\ydbriC.obj: ydbriC.c
    @$(COMPILE) /Fo$@ ydbriC.c

$(INTDIR)\ydbriS.obj: ydbriS.c
    @$(COMPILE) /Fo$@ ydbriS.c


$(BIN)\mnorbsrv.exe : $(INTDIR)\yd.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\yd.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnorbmet.exe : $(INTDIR)\ydmtd.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ydmtd.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnnmsrv.exe : $(INTDIR)\ydnmd.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ydnmd.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnorbls.exe : $(INTDIR)\ydls.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ydls.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnorbctl.exe : $(INTDIR)\ydcf.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ydcf.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnorbadm.exe : $(INTDIR)\ydadm.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ydadm.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnprocstat.exe : $(INTDIR)\ydspps.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ydspps.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnobjadm.exe : $(INTDIR)\ydchadm.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ydchadm.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mniiopbrsrv.exe : $(INTDIR)\ydbr.obj $(INTDIR)\ydbriS.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ydbr.obj $(INTDIR)\ydbriS.obj
	$(LOADLIBES) $(TNSLIBS) $(LDLIBS)
<<

