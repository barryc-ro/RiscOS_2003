#
# $Header: Makefile.mak@@\main\oms_mx3.3_dev\oms_mx3.3_nt_dev\0 \
# Checked out on Thu Feb 19 1997 15:42 by jchangav \
# Copyright (c) 1997 by Oracle Corporation. All Rights Reserved. \
# $
#
#
# Microsoft NMAKE file for src/ys
#
OMXROOT = m:
BUILDROOT = c:\mx

include m:\src\buildtools\winnt.mak

SRC	= $(SRCHOME)\mx\src

INTDIR = $(LIB)\ys32

INCLUDE	=   /I $(OMXROOT)\src\ys\sosd\$(ARCH) \
	    /I $(OMXROOT)\pub \
	    /I $(OMXROOT)\inc \
	    /I $(OMXROOT)\src\inc \
	    /I $(OMXROOT)\src\ys \

COMPONENT = YS - MX service layer

YSSRC	  = $(OMXROOT)\src\ys
YSOSD     = $(YSSRC)\sosd\$(ARCH)
S0YSMAIN  = $(YSOSD)\s0ysmain.c

MNIDLC_FLAGS =  -a coa -R ys.log.msg-path=$(MSGDIR)

# libs for me to link with, beyond default LDLIBS.

LOADLIBES = $(LIB)\ys32.lib

BINS =	$(BIN)\mnmsgc.exe \
	$(BIN)\svcinstall.exe \
	$(BIN)\svcremove.exe \
	$(BIN)\svcstart.exe \
	$(BIN)\sysilog.exe \
	$(BIN)\mnmsgc.pdb \
	$(BIN)\svcinstall.pdb \
	$(BIN)\svcremove.pdb \
	$(BIN)\svcstart.pdb \
	$(BIN)\sysilog.pdb

MSGS	  = $(MSGDIR)\OMNMSGC.mot $(MSGDIR)\OMNYS.mot


# objects to be in the library.

LIBOBJS   = $(INTDIR)\ys.obj \
	    $(INTDIR)\ysb8.obj $(INTDIR)\ysbv.obj $(INTDIR)\ysc.obj \
	    $(INTDIR)\yscksm.obj $(INTDIR)\yse.obj $(INTDIR)\ysfe.obj \
	    $(INTDIR)\ysfmt.obj $(INTDIR)\ysfo.obj $(INTDIR)\yshsh.obj \
	    $(INTDIR)\ysid.obj $(INTDIR)\ysl.obj $(INTDIR)\yslog.obj \
	    $(INTDIR)\yslst.obj $(INTDIR)\ysm.obj $(INTDIR)\ysmdmp.obj \
	    $(INTDIR)\ysmsg.obj $(INTDIR)\ysr.obj $(INTDIR)\ysrand.obj \
	    $(INTDIR)\ysrarg.obj $(INTDIR)\ysrgx.obj $(INTDIR)\ysrprs.obj \
	    $(INTDIR)\yssp.obj $(INTDIR)\ysstr.obj $(INTDIR)\yst.obj \
	    $(INTDIR)\ysthr.obj $(INTDIR)\ystm.obj $(INTDIR)\ysv.obj \
	    $(INTDIR)\ysver.obj $(INTDIR)\ysx.obj \
	    $(INTDIR)\sysfp.obj $(INTDIR)\sysi.obj $(INTDIR)\sysrld.obj \
	    $(INTDIR)\syst.obj $(INTDIR)\systhr.obj \
	    $(INTDIR)\sysb8.obj $(INTDIR)\syssp.obj \
	    $(INTDIR)\sysx.obj $(INTDIR)\syssvc.obj

OBJS =	$(INTDIR)\ysmsgc.obj $(INTDIR)\s0mnmsgc.obj \
	$(INTDIR)\svcinstall.obj $(INTDIR)\svcremove.obj \
	$(INTDIR)\svcstart.obj \
	$(INTDIR)\sysilog.obj $(INTDIR)\sysmsg.res \
	$(YSOSD)\sysmsg.rc $(YSOSD)\MSG00001.bin

LIBS =	$(BIN)\ys32.dll $(LIB)\ys32.lib $(LIB)\ys32.exp $(LIB)\ys32.pdb

DIRS =   $(INTDIR)

all	 : $(DIRS) $(LIBS) $(OBJS) $(BINS) $(MSGS)

compile  : $(OBJS)

lib	 : $(INTDIR) $(LIBS)

link	 : $(BINS)

cleanobj :
	-del $(LIBOBJS)
	-del $(OBJS)

cleanlib :
	-del $(LIBS)

cleanbin :
	-del $(BINS)

clean:
	-del $(BINS)
	-del $(LIBS)
	-del $(LIBOBJS)
	-del $(OBJS)
	-del $(INTDIR)\vc40.*

$(BINS): $(LOADLIBES)

$(INTDIR) :
    if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"

DEF_FILE = $(YSOSD)\dllys.def

RES_FILE = $(LIB)\ys32.res

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(BIN)\ys32.dll $(LIB)\ys32.lib : $(LIBOBJS) $(DEF_FILE) $(RES_FILE)
    @-echo Linking $@...
    $(LD) @<<
    $(LIBOBJS)
    $(LDLIBS)
    $(RES_FILE)
    /OUT:$(BIN)\ys32.dll
    $(DLLFLAGS)
    $(LDDEBUG)
    /pdb:$(LIB)\ys32.pdb
    /def:$(DEF_FILE)
    /implib:$(LIB)\ys32.lib
<<

$(LIB)\ys32.res: $(YSOSD)\ys32.rc
    rc /r /fo $(LIB)\ys32.res $(YSOSD)\ys32.rc

$(BIN)\mnmsgc.exe : $(INTDIR)\ysmsgc.obj $(INTDIR)\s0mnmsgc.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\ysmsgc.obj $(INTDIR)\s0mnmsgc.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\svcinstall.exe : $(INTDIR)\svcinstall.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\svcinstall.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\svcremove.exe : $(INTDIR)\svcremove.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\svcremove.obj
	$(LOADLIBES) $(LDLIBS)
<<
$(BIN)\svcstart.exe : $(INTDIR)\svcstart.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\svcstart.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\sysilog.exe : $(INTDIR)\sysilog.obj $(INTDIR)\sysmsg.res
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\sysilog.obj $(INTDIR)\sysmsg.res
	$(LOADLIBES) $(LDLIBS)
<<

$(INTDIR)\s0mnmsgc.obj: $(S0YSMAIN)
    @$(COMPILE) /Fo$@ $(S0YSMAIN) /D ENTRY_POINT=ysmsgc

$(INTDIR)\ysmsgc.obj: ysmsgc.c
    @$(COMPILE) /Fo$@ ysmsgc.c

$(INTDIR)\sysfp.obj : $(YSOSD)\sysfp.c
	@$(COMPILE) /Fo$@ $(YSOSD)\sysfp.c

$(INTDIR)\sysi.obj : $(YSOSD)\sysi.c
	@$(COMPILE) /Fo$@ $(YSOSD)\sysi.c

$(INTDIR)\sysb8.obj : $(YSOSD)\sysb8.c
	@$(COMPILE) /Fo$@ $(YSOSD)\sysb8.c

$(INTDIR)\syssp.obj : $(YSOSD)\syssp.c
	@$(COMPILE) /Fo$@ $(YSOSD)\syssp.c

$(INTDIR)\sysx.obj : $(YSOSD)\sysx.c
	@$(COMPILE) /Fo$@ $(YSOSD)\sysx.c

$(INTDIR)\syssvc.obj : $(YSOSD)\syssvc.c
	@$(COMPILE) /Fo$@ $(YSOSD)\syssvc.c

$(INTDIR)\sysrld.obj : $(YSOSD)\sysrld.c
	@$(COMPILE) /Fo$@ $(YSOSD)\sysrld.c

$(INTDIR)\syst.obj : $(YSOSD)\syst.c
	@$(COMPILE) /Fo$@ $(YSOSD)\syst.c

$(INTDIR)\systhr.obj : $(YSOSD)\systhr.c
	@$(COMPILE) /Fo$@ $(YSOSD)\systhr.c

$(INTDIR)\ys.obj : ys.c
	@$(COMPILE) /Fo$@ ys.c

$(INTDIR)\ysb8.obj : ysb8.c
	@$(COMPILE) /Fo$@ ysb8.c

$(INTDIR)\ysbv.obj : ysbv.c
	@$(COMPILE) /Fo$@ ysbv.c

$(INTDIR)\ysc.obj : ysc.c
	@$(COMPILE) /Fo$@ ysc.c

$(INTDIR)\yscksm.obj : yscksm.c
	@$(COMPILE) /Fo$@ yscksm.c

$(INTDIR)\yse.obj : yse.c
	@$(COMPILE) /Fo$@ yse.c

$(INTDIR)\ysfe.obj : ysfe.c
	@$(COMPILE) /Fo$@ ysfe.c

$(INTDIR)\ysfmt.obj : ysfmt.c
	@$(COMPILE) /Fo$@ ysfmt.c

$(INTDIR)\ysfo.obj : ysfo.c
	@$(COMPILE) /Fo$@ ysfo.c

$(INTDIR)\yshsh.obj : yshsh.c
	@$(COMPILE) /Fo$@ yshsh.c

$(INTDIR)\ysid.obj : ysid.c
	@$(COMPILE) /Fo$@ ysid.c

$(INTDIR)\ysl.obj : ysl.c
	@$(COMPILE) /Fo$@ ysl.c

$(INTDIR)\yslog.obj : yslog.c
	@$(COMPILE) /Fo$@ yslog.c

$(INTDIR)\yslst.obj : yslst.c
	@$(COMPILE) /Fo$@ yslst.c

$(INTDIR)\ysm.obj : ysm.c
	@$(COMPILE) /Fo$@ ysm.c

$(INTDIR)\ysmdmp.obj : ysmdmp.c
	@$(COMPILE) /Fo$@ ysmdmp.c

$(INTDIR)\ysmsg.obj : ysmsg.c
	@$(COMPILE) /Fo$@ ysmsg.c

$(INTDIR)\ysr.obj : ysr.c
	@$(COMPILE) /Fo$@ ysr.c

$(INTDIR)\ysrand.obj : ysrand.c
	@$(COMPILE) /Fo$@ ysrand.c

$(INTDIR)\ysrarg.obj : ysrarg.c
	@$(COMPILE) /Fo$@ ysrarg.c

$(INTDIR)\ysrgx.obj : ysrgx.c
	@$(COMPILE) /Fo$@ ysrgx.c

$(INTDIR)\ysrprs.obj : ysrprs.c
	@$(COMPILE) /Fo$@ ysrprs.c

$(INTDIR)\yssp.obj : yssp.c
	@$(COMPILE) /Fo$@ yssp.c

$(INTDIR)\ysstr.obj : ysstr.c
	@$(COMPILE) /Fo$@ ysstr.c

$(INTDIR)\yst.obj : yst.c
	@$(COMPILE) /Fo$@ yst.c

$(INTDIR)\ysthr.obj : ysthr.c
	@$(COMPILE) /Fo$@ ysthr.c

$(INTDIR)\ystm.obj : ystm.c
	@$(COMPILE) /Fo$@ ystm.c

$(INTDIR)\ysv.obj : ysv.c
	@$(COMPILE) /Fo$@ ysv.c

$(INTDIR)\ysver.obj : ysver.c
	@$(COMPILE) /Fo$@ ysver.c

$(INTDIR)\ysx.obj : ysx.c
	@$(COMPILE) /Fo$@ ysx.c

$(INTDIR)\svcinstall.obj : $(YSOSD)\svcinstall.c
	@$(COMPILE) /Fo$@ $(YSOSD)\svcinstall.c

$(INTDIR)\svcremove.obj : $(YSOSD)\svcremove.c
	@$(COMPILE) /Fo$@ $(YSOSD)\svcremove.c

$(INTDIR)\svcstart.obj  : $(YSOSD)\svcstart.c
	@$(COMPILE) /Fo$@ $(YSOSD)\svcstart.c

$(INTDIR)\sysilog.obj : $(YSOSD)\sysilog.c
	@$(COMPILE) /Fo$@ $(YSOSD)\sysilog.c

$(INTDIR)\sysmsg.res : $(YSOSD)\sysmsg.rc $(YSOSD)\MSG00001.bin
	rc  /l 0x409 /fo$(INTDIR)\sysmsg.res /i $(YSOSD) \
	    /d _DEBUG $(YSOSD)\sysmsg.rc

$(YSOSD)\sysmsg.rc $(YSOSD)\MSG00001.bin : $(YSOSD)\sysmsg.mc
	mc -r $(YSOSD) $(YSOSD)\sysmsg.mc

$(MSGDIR)\OMNMSGC.mot : $(MSGDIR)\ys\OMNMSGC.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\ys\OMNMSGC.msg

$(MSGDIR)\OMNYS.mot : $(MSGDIR)\ys\OMNYS.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\ys\OMNYS.msg



