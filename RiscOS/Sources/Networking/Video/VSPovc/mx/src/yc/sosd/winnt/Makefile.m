#
# Microsoft NMAKE file for src/yc
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
	    /I $(OMXROOT)\src\yc \

INTDIR = $(LIB)\yc32

COMPONENT = MX Corba IDL Compiler

YCSRC	  = $(OMXROOT)\src\yc
YCOSD     = $(YCSRC)\sosd\$(ARCH)

YSSRC	  = $(OMXROOT)\src\ys
YSOSD     = $(YSSRC)\sosd\$(ARCH)
S0YSMAIN  = $(YSOSD)\s0ysmain.c

MSGS	=   $(MSGDIR)\OMNIDLC.mot

S0MZMAIN  = $(OMXROOT)\src\mt\sosd\$(ARCH)\s0mzmain.c

YCIDLC_FLAGS =  -a coa -R yc.log.msg-path=$(MSGDIR)

LIBS =	    nothing

# libs for me to link with, beyond default LDLIBS.

LOADLIBES = $(LIB)\yotkyr32.lib $(LIB)\mn32.lib \
	    $(LIB)\mt32.lib $(LIB)\ys32.lib 

LIBYCFEOBJS =	$(INTDIR)\ycosyn.obj $(INTDIR)\ycosax.obj \
		$(INTDIR)\ycpp.obj $(INTDIR)\ycppdir.obj \
		$(INTDIR)\ycppexp.obj $(INTDIR)\ycppget.obj \
		$(INTDIR)\ycppif.obj $(INTDIR)\ycrec.obj \
		$(INTDIR)\ycprs.obj $(INTDIR)\ycsem.obj \
		$(INTDIR)\ycsemty.obj $(INTDIR)\ycsemco.obj \
		$(INTDIR)\ycsemif.obj $(INTDIR)\ycifr.obj \
		$(INTDIR)\pxslax.obj $(INTDIR)\ycprfree.obj \
		$(INTDIR)\ycsemprg.obj $(INTDIR)\ycutil.obj \
		$(INTDIR)\ycsym.obj
#####
# libycc - the C language mapping code generator.
#####
LIBYCCOBJS =	$(INTDIR)\yccmap.obj $(INTDIR)\yccsrv.obj \
		$(INTDIR)\yccclnt.obj $(INTDIR)\ycctmpl.obj \
		$(INTDIR)\yccname.obj $(INTDIR)\ycchdr.obj \
		$(INTDIR)\yccfp.obj

#####
# libyccp - the C++ language mapping code generator.
#####
LIBYCCPOBJS =	$(INTDIR)\yccpmap.obj $(INTDIR)\yccphdr.obj \
		$(INTDIR)\yccpname.obj $(INTDIR)\yccptvar.obj \
		$(INTDIR)\yccpclnt.obj 

#####
# libyci - the ITL language mapping code generator.
#####
LIBYCIOBJS =	$(INTDIR)\ycimap.obj $(INTDIR)\ycihdr.obj \
		$(INTDIR)\yciclnt.obj

LIBOBJS =   nothing

OBJS =	    $(INTDIR)\ycmain.obj $(INTDIR)\ycysmain.obj \
	    $(LIBYCCOBJS) $(LIBYCCPOBJS) $(LIBYCFEOBJS) $(LIBYCIOBJS)

BINS =	    $(BIN)\mnidlc.exe $(BIN)\mnidlc.pdb

DIRS =	    $(INTDIR)

all	 : $(DIRS) $(LIBS) $(OBJS) $(BINS) $(MSGS)

msgs :	$(MSGS)

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

# ----------------------------------------------------------------
# main entry points

$(INTDIR)\ycmain.obj : ycmain.c
	@$(COMPILE) /Fo$@ ycmain.c

# ----------------------------------------------------------------
# main programs

$(INTDIR)\ycysmain.obj : $(S0YSMAIN)
	@$(COMPILE) /Fo$@ $(S0YSMAIN) /D ENTRY_POINT=ycMain 

# ----------------------------------------------------------------
# programs

$(BIN)\mnidlc.exe: $(OBJS) $(LOADLIBES)
	@echo Linking $@
	@$(LD) /OUT:$@ @<<
	$(LDFLAGS) $(LDDEBUG)
	$(OBJS) $(LOADLIBES) $(LDLIBS)
<<

$(INTDIR)\ycosyn.obj : ycosyn.c
	@$(COMPILE) /Fo$@ ycosyn.c

$(INTDIR)\ycsym.obj : ycsym.c
	@$(COMPILE) /Fo$@ ycsym.c

$(INTDIR)\ycosax.obj : ycosax.c
	@$(COMPILE) /Fo$@ ycosax.c

$(INTDIR)\ycpp.obj : ycpp.c
	@$(COMPILE) /Fo$@ ycpp.c

$(INTDIR)\ycppdir.obj : ycppdir.c
	@$(COMPILE) /Fo$@ ycppdir.c

$(INTDIR)\ycppexp.obj : ycppexp.c
	@$(COMPILE) /Fo$@ ycppexp.c

$(INTDIR)\ycppget.obj : ycppget.c
	@$(COMPILE) /Fo$@ ycppget.c

$(INTDIR)\ycppif.obj : ycppif.c
	@$(COMPILE) /Fo$@ ycppif.c

$(INTDIR)\ycrec.obj : ycrec.c
	@$(COMPILE) /Fo$@ ycrec.c

$(INTDIR)\ycprs.obj : ycprs.c
	@$(COMPILE) /Fo$@ ycprs.c

$(INTDIR)\ycsem.obj : ycsem.c
	@$(COMPILE) /Fo$@ ycsem.c

$(INTDIR)\ycsemty.obj : ycsemty.c
	@$(COMPILE) /Fo$@ ycsemty.c

$(INTDIR)\ycsemco.obj : ycsemco.c
	@$(COMPILE) /Fo$@ ycsemco.c

$(INTDIR)\ycsemif.obj : ycsemif.c
	@$(COMPILE) /Fo$@ ycsemif.c

$(INTDIR)\ycifr.obj : ycifr.c
	@$(COMPILE) /Fo$@ ycifr.c

$(INTDIR)\pxslax.obj : pxslax.c
	@$(COMPILE) /Fo$@ pxslax.c

$(INTDIR)\ycprfree.obj : ycprfree.c
	@$(COMPILE) /Fo$@ ycprfree.c

$(INTDIR)\ycsemprg.obj : ycsemprg.c
	@$(COMPILE) /Fo$@ ycsemprg.c

$(INTDIR)\ycutil.obj : ycutil.c
	@$(COMPILE) /Fo$@ ycutil.c

$(INTDIR)\yccmap.obj : yccmap.c
	@$(COMPILE) /Fo$@ yccmap.c

$(INTDIR)\yccsrv.obj : yccsrv.c
	@$(COMPILE) /Fo$@ yccsrv.c

$(INTDIR)\yccclnt.obj : yccclnt.c
	@$(COMPILE) /Fo$@ yccclnt.c

$(INTDIR)\ycctmpl.obj : ycctmpl.c
	@$(COMPILE) /Fo$@ ycctmpl.c

$(INTDIR)\yccname.obj : yccname.c
	@$(COMPILE) /Fo$@ yccname.c

$(INTDIR)\ycchdr.obj : ycchdr.c
	@$(COMPILE) /Fo$@ ycchdr.c

$(INTDIR)\yccfp.obj : yccfp.c
	@$(COMPILE) /Fo$@ yccfp.c

$(INTDIR)\yccpmap.obj : yccpmap.c
	@$(COMPILE) /Fo$@ yccpmap.c

$(INTDIR)\yccphdr.obj : yccphdr.c
	@$(COMPILE) /Fo$@ yccphdr.c

$(INTDIR)\yccpname.obj : yccpname.c
	@$(COMPILE) /Fo$@ yccpname.c

$(INTDIR)\yccptvar.obj : yccptvar.c
	@$(COMPILE) /Fo$@ yccptvar.c

$(INTDIR)\yccpclnt.obj : yccpclnt.c
	@$(COMPILE) /Fo$@ yccpclnt.c

$(INTDIR)\ycimap.obj : ycimap.c
	@$(COMPILE) /Fo$@ ycimap.c

$(INTDIR)\ycihdr.obj : ycihdr.c
	@$(COMPILE) /Fo$@ ycihdr.c

$(INTDIR)\yciclnt.obj : yciclnt.c
	@$(COMPILE) /Fo$@ yciclnt.c

$(MSGDIR)\OMNIDLC.mot : $(MSGDIR)\yc\OMNIDLC.msg
	-$(MSGC) -I $(SRCHOME)\mesg -o $@ $(MSGDIR)\yc\OMNIDLC.msg

