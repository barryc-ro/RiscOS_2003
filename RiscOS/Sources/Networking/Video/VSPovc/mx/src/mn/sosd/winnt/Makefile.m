#
# Microsoft NMAKE file for src/mn
#
OMXROOT = m:
BUILDROOT = c:\mx

include m:\src\buildtools\winnt.mak

SRC	= $(SRCHOME)\mx\src

INCLUDE	=   /I $(OMXROOT)\src\ys\sosd\$(ARCH) \
	    /I $(OMXROOT)\src\mn\sosd\$(ARCH) \
	    /I $(OMXROOT)\src\mt\sosd\$(ARCH) \
	    /I $(OMXROOT)\pub \
	    /I $(OMXROOT)\inc \
	    /I $(OMXROOT)\src\inc \
	    /I $(OMXROOT)\src\mn \

INTDIR = $(LIB)\mn32

COMPONENT = MX transport layer

MNSRC	  = $(OMXROOT)\src\mn
MNOSD     = $(MNSRC)\sosd\$(ARCH)

S0MZMAIN  = $(OMXROOT)\src\mt\sosd\$(ARCH)\s0mzmain.c

MNIDLC_FLAGS =  -a coa -R mn.log.msg-path=$(MSGDIR)

LIBS = $(BIN)\mn32.dll $(LIB)\mn32.lib $(LIB)\mn32.exp $(LIB)\mn32.pdb

# libs for me to link with, beyond default LDLIBS.

DLLLIBES = $(LIB)\ys32.lib $(LIB)\mt32.lib

LOADLIBES = $(LIB)\mn32.lib $(DLLLIBES)

BINS = nothing

#Base objects

GENOBJS =   $(INTDIR)\mn.obj $(INTDIR)\mna.obj $(INTDIR)\mne.obj \
	    $(INTDIR)\mnm.obj $(INTDIR)\mnn.obj $(INTDIR)\mno.obj \
	    $(INTDIR)\mnt.obj $(INTDIR)\mntr.obj $(INTDIR)\mnts.obj

# RPC objects
RPCOBJS =   $(INTDIR)\mnr.obj $(INTDIR)\mnrc.obj $(INTDIR)\mnrs.obj \
	    $(INTDIR)\mnx.obj $(INTDIR)\mznt.obj $(INTDIR)\mnb.obj \
	    $(INTDIR)\mzmgs.obj

# Name server objects
NSOBJS  =   $(INTDIR)\mzn.obj $(INTDIR)\mznc.obj

# Process server objects
SCOBJS =    $(INTDIR)\mnsc.obj $(INTDIR)\mnscc.obj

# Utility objects
UTLOBJS =   $(INTDIR)\mnpsl.obj

# System specific objs
OSDOBJS =   $(INTDIR)\smnudp.obj \
	    $(INTDIR)\smni.obj $(INTDIR)\smng.obj $(INTDIR)\smnj.obj

# objects to be in the library.

LIBOBJS   = $(GENOBJS) $(RPCOBJS) $(NSOBJS) $(SCOBJS) $(UTLOBJS) \
	    $(OSDOBJS) 

# Main program objects
MAINOBJS =  $(INTDIR)\mnars.obj $(INTDIR)\mnrm.obj $(INTDIR)\mnls.obj \
	    $(INTDIR)\mnln.obj $(INTDIR)\mnscmd.obj $(INTDIR)\mnping.obj \
	    $(INTDIR)\mnps.obj $(INTDIR)\mnrid.obj $(INTDIR)\mnscs.obj \
	    $(INTDIR)\mnjoin.obj $(INTDIR)\mnscsop.obj $(INTDIR)\mznops.obj \
	    $(INTDIR)\mzns.obj

# entry point objs
OBJS =	    $(MAINOBJS) $(INTDIR)\s0mnars.obj $(INTDIR)\s0mnjoin.obj \
	    $(INTDIR)\s0mnln.obj $(INTDIR)\s0mnls.obj $(INTDIR)\s0mnping.obj \
	    $(INTDIR)\s0mnps.obj $(INTDIR)\s0mnrid.obj $(INTDIR)\s0mnrm.obj \
	    $(INTDIR)\s0mnscmd.obj $(INTDIR)\s0mnsc.obj $(INTDIR)\s0mznm.obj \

BINS      = $(BIN)\mnaddrsrv.exe $(BIN)\mnrpcnmsrv.exe \
	    $(BIN)\mnprocsrv.exe $(BIN)\mnping.exe \
            $(BIN)\mnls.exe $(BIN)\mnps.exe $(BIN)\mnrpcid.exe \
	    $(BIN)\mnscmd.exe $(BIN)\mnln.exe \
            $(BIN)\mnrm.exe $(BIN)\mnjoin.exe \
	    $(BIN)\mnaddrsrv.pdb $(BIN)\mnrpcnmsrv.pdb \
	    $(BIN)\mnprocsrv.pdb $(BIN)\mnping.pdb \
            $(BIN)\mnls.pdb $(BIN)\mnps.pdb $(BIN)\mnrpcid.pdb \
	    $(BIN)\mnscmd.pdb $(BIN)\mnln.pdb \
            $(BIN)\mnrm.pdb $(BIN)\mnjoin.pdb

DIRS =   $(INTDIR)

all	 : $(DIRS) $(LIBS) $(OBJS) $(BINS)

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

DEF_FILE = $(MNOSD)\dllmn.def

RES_FILE = $(LIB)\mn32.res

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(BIN)\mn32.dll $(LIB)\mn32.lib : $(LIBOBJS) $(DEF_FILE) $(RES_FILE)
    @-echo Linking $@...
    @echo LIBOBJS $(LIBOBJS)
    @echo DLLLIBES $(DLLLIBES)
    $(LD) @<<
    /OUT:$(BIN)\mn32.dll
    /ENTRY:smnDllInit
    $(DLLFLAGS)
    $(LDDEBUG)
    $(LIBOBJS)
    $(DLLLIBES)
    $(LDLIBS)
    $(RES_FILE)
    /pdb:$(LIB)\mn32.pdb
    /def:$(DEF_FILE)
    /implib:$(LIB)\mn32.lib
<<

$(LIB)\mn32.res: $(MNOSD)\mn32.rc
    rc /r /fo $(LIB)\mn32.res $(MNOSD)\mn32.rc

# ----------------------------------------------------------------
# lib objs


$(INTDIR)\mn.obj : mn.c
	@$(COMPILE) /Fo$@ mn.c

$(INTDIR)\mna.obj : mna.c
	@$(COMPILE) /Fo$@ mna.c

$(INTDIR)\mne.obj : mne.c
	@$(COMPILE) /Fo$@ mne.c

$(INTDIR)\mnm.obj : mnm.c
	@$(COMPILE) /Fo$@ mnm.c

$(INTDIR)\mnn.obj : mnn.c
	@$(COMPILE) /Fo$@ mnn.c

$(INTDIR)\mno.obj : mno.c
	@$(COMPILE) /Fo$@ mno.c

$(INTDIR)\mnt.obj : mnt.c
	@$(COMPILE) /Fo$@ mnt.c

$(INTDIR)\mntr.obj : mntr.c
	@$(COMPILE) /Fo$@ mntr.c

$(INTDIR)\mnts.obj : mnts.c
	@$(COMPILE) /Fo$@ mnts.c

$(INTDIR)\mnr.obj : mnr.c
	@$(COMPILE) /Fo$@ mnr.c

$(INTDIR)\mnrc.obj : mnrc.c
	@$(COMPILE) /Fo$@ mnrc.c

$(INTDIR)\mnrs.obj : mnrs.c
	@$(COMPILE) /Fo$@ mnrs.c

$(INTDIR)\mnx.obj : mnx.c
	@$(COMPILE) /Fo$@ mnx.c

$(INTDIR)\mznt.obj : mznt.c
	@$(COMPILE) /Fo$@ mznt.c

$(INTDIR)\mnb.obj : mnb.c
	@$(COMPILE) /Fo$@ mnb.c

$(INTDIR)\mzmgs.obj : mzmgs.c
	@$(COMPILE) /Fo$@ mzmgs.c

$(INTDIR)\mzn.obj : mzn.c
	@$(COMPILE) /Fo$@ mzn.c

$(INTDIR)\mznc.obj : mznc.c
	@$(COMPILE) /Fo$@ mznc.c

$(INTDIR)\mnsc.obj : mnsc.c
	@$(COMPILE) /Fo$@ mnsc.c

$(INTDIR)\mnscc.obj : mnscc.c
	@$(COMPILE) /Fo$@ mnscc.c

$(INTDIR)\mnpsl.obj : mnpsl.c
	@$(COMPILE) /Fo$@ mnpsl.c

$(INTDIR)\smnudp.obj : $(MNOSD)\smnudp.c
	@$(COMPILE) /Fo$@ $(MNOSD)\smnudp.c

$(INTDIR)\smni.obj : $(MNOSD)\smni.c
	@$(COMPILE) /Fo$@ $(MNOSD)\smni.c

$(INTDIR)\smng.obj : $(MNOSD)\smng.c
	@$(COMPILE) /Fo$@ $(MNOSD)\smng.c

$(INTDIR)\smnj.obj : $(MNOSD)\smnj.c
	@$(COMPILE) /Fo$@ $(MNOSD)\smnj.c

# ----------------------------------------------------------------
# main entry points

$(INTDIR)\mnars.obj : mnars.c
	@$(COMPILE) /Fo$@ mnars.c

$(INTDIR)\mnrm.obj  : mnrm.c
	@$(COMPILE) /Fo$@ mnrm.c

$(INTDIR)\mnls.obj  : mnls.c
	@$(COMPILE) /Fo$@ mnls.c

$(INTDIR)\mnln.obj  : mnln.c
	@$(COMPILE) /Fo$@ mnln.c

$(INTDIR)\mnscmd.obj  : mnscmd.c
	@$(COMPILE) /Fo$@ mnscmd.c

$(INTDIR)\mnping.obj  : mnping.c
	@$(COMPILE) /Fo$@ mnping.c

$(INTDIR)\mnps.obj  : mnps.c
	@$(COMPILE) /Fo$@ mnps.c

$(INTDIR)\mnrid.obj  : mnrid.c
	@$(COMPILE) /Fo$@ mnrid.c

$(INTDIR)\mnscs.obj  : mnscs.c
	@$(COMPILE) /Fo$@ mnscs.c

$(INTDIR)\mzns.obj  : mzns.c
	@$(COMPILE) /Fo$@ mzns.c

$(INTDIR)\mnjoin.obj  : mnjoin.c
	@$(COMPILE) /Fo$@ mnjoin.c

$(INTDIR)\mnscsop.obj  : mnscsop.c
	@$(COMPILE) /Fo$@ mnscsop.c

$(INTDIR)\mznops.obj  : mznops.c
	@$(COMPILE) /Fo$@ mznops.c

# ----------------------------------------------------------------
# main programs

$(INTDIR)\s0mnars.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) \
	    /D ENTRY_POINT=mnars /D DEFAULT_MTL_TOOL=MtlLogTool

$(INTDIR)\s0mnjoin.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) /D ENTRY_POINT=mnjoin

$(INTDIR)\s0mnln.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) /D ENTRY_POINT=mnln

$(INTDIR)\s0mnls.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) /D ENTRY_POINT=mnls

$(INTDIR)\s0mnping.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) /D ENTRY_POINT=mnping

$(INTDIR)\s0mnps.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) /D ENTRY_POINT=mnps

$(INTDIR)\s0mnrid.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) /D ENTRY_POINT=mnridMain

$(INTDIR)\s0mnrm.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) /D ENTRY_POINT=mnrm

$(INTDIR)\s0mnscmd.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) \
	    /D ENTRY_POINT=mnscmd /D DEFAULT_MTL_TOOL=MtlLogTool

$(INTDIR)\s0mnsc.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) /D ENTRY_POINT=mnscMain

$(INTDIR)\s0mznm.obj  : $(S0MZMAIN)
	@$(COMPILE) /Fo$@ $(S0MZMAIN) \
	    /D ENTRY_POINT=mznMain /D DEFAULT_MTL_TOOL=MtlLogTool

# ----------------------------------------------------------------
# programs

$(BIN)\mnaddrsrv.exe: $(INTDIR)\mnars.obj $(INTDIR)\s0mnars.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG)
	$(INTDIR)\mnars.obj
	$(INTDIR)\s0mnars.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnrpcnmsrv.exe: $(INTDIR)\s0mznm.obj $(INTDIR)\mzns.obj \
			$(INTDIR)\mznops.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG)
	$(INTDIR)\s0mznm.obj
	$(INTDIR)\mzns.obj
	$(INTDIR)\mznops.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnprocsrv.exe: $(INTDIR)\s0mnsc.obj $(INTDIR)\mnscs.obj \
			$(INTDIR)\mnscsop.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG)
	$(INTDIR)\s0mnsc.obj
	$(INTDIR)\mnscs.obj
	$(INTDIR)\mnscsop.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnping.exe: $(INTDIR)\s0mnping.obj $(INTDIR)\mnping.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG)
	$(INTDIR)\s0mnping.obj
	$(INTDIR)\mnping.obj
        $(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnls.exe:  $(INTDIR)\s0mnls.obj $(INTDIR)\mnls.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\s0mnls.obj $(INTDIR)\mnls.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnps.exe: $(INTDIR)\s0mnps.obj $(INTDIR)\mnps.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG)
	$(INTDIR)\s0mnps.obj
	$(INTDIR)\mnps.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnrpcid.exe: $(INTDIR)\s0mnrid.obj $(INTDIR)\mnrid.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG)
	$(INTDIR)\s0mnrid.obj
	$(INTDIR)\mnrid.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnscmd.exe: $(INTDIR)\s0mnscmd.obj $(INTDIR)\mnscmd.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG)
	$(INTDIR)\s0mnscmd.obj
	$(INTDIR)\mnscmd.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnln.exe: $(INTDIR)\s0mnln.obj $(INTDIR)\mnln.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\s0mnln.obj $(INTDIR)\mnln.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnrm.exe: $(INTDIR)\s0mnrm.obj $(INTDIR)\mnrm.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\s0mnrm.obj $(INTDIR)\mnrm.obj
	$(LOADLIBES) $(LDLIBS)
<<

$(BIN)\mnjoin.exe: $(INTDIR)\s0mnjoin.obj $(INTDIR)\mnjoin.obj
	@-echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG) $(INTDIR)\s0mnjoin.obj $(INTDIR)\mnjoin.obj
	$(LOADLIBES) $(LDLIBS)
<<
