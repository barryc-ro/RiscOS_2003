#
# $Header: Makefile.mak@@\main\oms_mx3.3_dev\oms_mx3.3_nt_dev\0 \
# Checked out on Tue Feb 24 1997 13:6 by jchangav \
# Copyright (c) 1997 by Oracle Corporation. All Rights Reserved. \
# $
#
#
# Microsoft NMAKE file for src/mt
#
OMXROOT = m:
BUILDROOT = c:\mx

include m:\src\buildtools\winnt.mak

SRC	= $(SRCHOME)\mx\src

INCLUDE	=   /I $(OMXROOT)\src\ys\sosd\$(ARCH) \
	    /I $(OMXROOT)\src\mt\sosd\$(ARCH) \
	    /I $(OMXROOT)\pub \
	    /I $(OMXROOT)\inc \
	    /I $(OMXROOT)\src\inc \
	    /I $(OMXROOT)\src\ys \

INTDIR = $(LIB)\mt32

COMPONENT = MT - obsolescent utilities

MTSRC	  = $(OMXROOT)\src\mt
MTOSD     = $(MTSRC)\sosd\$(ARCH)

# libs for me to link with, beyond default LDLIBS.

LOADLIBES = $(LIB)\ys32.lib

BINS = nothing

# objects to be in the library.

LIBOBJS   = $(INTDIR)\mtcctx.obj \
	    $(INTDIR)\mtsp.obj \
	    $(INTDIR)\mtl.obj $(INTDIR)\smtl.obj \
	    $(INTDIR)\mtrgx.obj \
	    $(INTDIR)\ss_memmove.obj

OBJS =	nothing

LIBS =	$(BIN)\mt32.dll $(LIB)\mt32.lib  $(LIB)\mt32.exp  $(LIB)\mt32.pdb

DIRS =	$(INTDIR)

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

# $(BINS): $(LOADLIBES)

$(INTDIR) :
    if not exist "$(INTDIR)\$(NULL)" mkdir "$(INTDIR)"

DEF_FILE = $(MTOSD)\dllmt.def

RES_FILE = $(LIB)\mt32.res

DLLFLAGS = /nologo /subsystem:windows /dll /incremental:no /machine:I386

$(BIN)\mt32.dll $(LIB)\mt32.lib : $(LIBOBJS) $(DEF_FILE) $(RES_FILE)
    @-echo Linking $@...
    $(LD) @<<
    $(LIBOBJS)
    $(LOADLIBES)
    $(LDLIBS)
    /OUT:$(BIN)\mt32.dll
    $(DLLFLAGS)
    $(LDDEBUG)
    $(RES_FILE)
    /pdb:$(LIB)\mt32.pdb
    /def:$(DEF_FILE)
    /implib:$(LIB)\mt32.lib
<<

$(LIB)\mt32.res: $(MTOSD)\mt32.rc
    rc /r /fo $(LIB)\mt32.res $(MTOSD)\mt32.rc

$(INTDIR)\mtcctx.obj  : mtcctx.c
	@$(COMPILE) /Fo$@ mtcctx.c

$(INTDIR)\mtsp.obj  : mtsp.c
	@$(COMPILE) /Fo$@ mtsp.c

$(INTDIR)\mtl.obj : mtl.c
	@$(COMPILE) /Fo$@ mtl.c

$(INTDIR)\smtl.obj : $(MTOSD)\smtl.c
	@$(COMPILE) /Fo$@ $(MTOSD)\smtl.c

$(INTDIR)\mtrgx.obj : mtrgx.c
	@$(COMPILE) /Fo$@ mtrgx.c

$(INTDIR)\ss_memmove.obj : $(MTOSD)\ss_memmove.c
	@$(COMPILE) /Fo$@ $(MTOSD)\ss_memmove.c



