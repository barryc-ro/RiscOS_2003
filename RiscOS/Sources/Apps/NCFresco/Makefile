# fresco/Makefile - top level Fresco makefile.
#
# Although currently targeted at UN*X type make's, the intention is that
# this file be movable to a RISC OS compilation host if desired.
#
# DL: BE WARNED THIS IS A COLLECTION OF SCRIPTS MASQUERADING AS A MAKEFILE:
# IT CONTAINS NO SOURCE DEPENDENCIRES SO DO NOT RELY ON IT FOR RECOMPILATION PURPOSES.
# IF YOU TOUCH A HEADER FILE YOU *MUST* DO "make clean".
#
#############################################################################
#
#
#############################################################################
#
# Support targets:
#
# clean		Delete intermediate and final binaries and objects
# checkers	Build the checker programs (UNIX initially)
# depends	Build dependency information
# dist		Directory suitable for distributing
# fixme		Search for comments indicating work needs doing
# fresco	Cross-build RISC OS Fresco executables (FRESCO)
# fresh		Useful after a make realclean
# ffu		Fresco for Unix - (FFU)
# html		Run attrgen.py on attrconf, generating basic SGML tables
# links		Build any necessary soft links
# plotcheck	Plotting checker
# realclean	Clean and also delete generated source files/state
# riscoslib     Build RISC_OSLib on Unix
# sanity	Perform directory tree sanity checks
# srclist	File holding filename of each source file
# stdalone	Stand alone version of parser (STDALONE)
# ncfresco	Cross-build STB Web executables (NCFRESCO)
# tags		Construct emacs TAGS file
# tests		Whatever automatic tests we can perform
# unix		Natively build unix executables (only some!) (UNIX)
# xref		Use cxref to build HTML cross-reference files
#
# The capitalised name in brackets is a #define symbol that will be defined
# for this build variant. Please make all code that is program dependent
# use one of these defines to avoid excessive numbers of control defines.
#
#############################################################################

POSS_TARGETS=	br bu cu BUILDERS BUILDERS_RISCOS BUILDERS_UNIX CHECKERS_UNIX clean \
		depends DIST FIXME FRESCO FRESCO_RISCOS HTML hr hu \
		HTMLCHECK HTMLCHECK_RISCOS HTMLCHECK_UNIX links    \
		memlib mu \
		MOST MOST_UNIX realclean sanity srclist sr NCFRESCO \
		NCFRESCO_RISCOS riscoslib TAGS TESTS xref \
		PROFILE  pu PLOTCHECK PLOTCHECK_UNIX ru RECONS

all:
		@echo
		@echo "Please specify which target you wish to make."
		@echo "The availables choices are:"
		@echo
		@echo "$(POSS_TARGETS)"
		@echo

# RiscIx make doesn't appear to support .PHONY specially, so don't
# make it the first target or all targets get made!

.PHONY:		$(POSS_TARGETS)

MAKE=		make
RM=		rm -f

#.c.o
#		@echo "Source: $<"
#		@$(CC) -c -M $< $(CFLAGS)

#############################################################################

#PREDEPS=	builddate.h sanity
#POSTACTS=	@$(RM) $(PREDEPS)
PREDEPS=        #sanity
POSTACTS=


PROF_IN=	profile.cfg
PROF_C=		commonsrc/profdat.c
PROF_H=		commonsrc/profdat.h

#############################################################################

SGML_OBJS=	attrparse.o elements.o entities.o sgmlparser.o statemach.o \
		support.o portutil.o genproc.o chopper.o colours.o

HTML_OBJS=	block.o builders.o deliver.o frames.o fonts.o forms.o head.o \
		hparse.o htmldefs.o lists.o misc.o phrase.o reports.o special.o \
		objparse.o

COMM_OBJS=	Serial.o access.o auth.o backend.o bittabs.o config.o \
		dfsupport.o dir2html.o drawfile.o dump.o imagemap.o \
		images.o layout.o licence.o makeerror.o memwatch.o \
		myassert.o object.o obreak.o obullet.o oimage.o oinput.o \
		oselect.o otable.o otext.o otextarea.o printing.o render.o \
		resolve.o rid.o tables.o threads.o gbf.o oscaff.o \
		unwind.o url.o util.o version.o webfonts.o \
		cookie.o cache2.o cache3.o stream.o debug.o usrtrc.o \
		oobject.o objects.o plugin.o pluginfile.o \
		files.o remote.o keyhl.o \
		buildtime.o format.o colspan.o loformat.o cs2.o \
		highlight.o frameutils.o csgraph.o rdebug.o submit.o

BASE_VPATH=	..:../commonsrc:../fresparse:../parser
BASE_INCS=	-I.. -I../commonsrc -I../fresparse -I../parser

#############################################################################

# This set of variables is the basic control over what is built

RISCOS_INCS=	-I../RISC_OSLib -I../include/unix -I../tcpiplibs $(BASE_INCS)  -I../memlib -J../clib
#RISCOS_INCS=	-I../RISC_OSLib -I../tcpiplibs $(BASE_INCS) -I../clib
# -Fn *includes* function names (at least for this option set)

RISCOS_CFLAGS_DBG=	-DRISCOS $(RISCOS_INCS) -fa -apcs 3/26 -Fn
RISCOS_CFLAGS_REL=	-DRISCOS $(RISCOS_INCS) -ffa -DCOMPAT_INET4

RISCOS_CFLAGS=  $(RISCOS_CFLAGS_DBG)
RISCOS_VPATH=	$(BASE_VPATH)
RISCOS_OBJS=	$(BASE_OBJS)
RISCOS_LIBS=	../RISC_OSLib/risc_oslib \
		../clib/o/inetlib \
		../clib/o/socklib \
		../clib/o/stubs

# Socklib is only needed in Fresco itself by the remote debugging code
# (none of its members should get linked into release builds)

unused1=	\
		-W \
		-Waggregate-return \
		-Wcast-align \
		-Wcast-qual \
		-Wconversion \
		-Wenum-clash \
		-Winline \
		-Wno-char-subscript \
		-Wno-switch \
		-Wpointer-arith \
		-Wredundant-decls \
		-Wshadow \
		-Wtraditional \
		-Wunused \
		-Wwrite-strings \
		-pedantic

zzz=		-W \
		-Waggregate-return \
		-Wcast-align \
		-Wconversion \
		-Winline \
		-Wno-switch \
		-Wpointer-arith \
		-Wunused \
		-Wwrite-strings \
		-pedantic \
		\

UNIX_GCC_FLAGS=-Wall \
		-Wformat \
		-Wimplicit \
		-Wcomment \
		-Wmissing-declarations \
		-Wtrigraphs \
		-Wreturn-type \
		-Wparentheses \
		-Wstrict-prototypes \
		-Wmissing-prototypes \
		-Wnested-externs \
		-Wno-switch \
		-Wuninitialized \
		-ansi -funsigned-char

UNIX_INCS=	-I../rocompat -I../RISC_OSLib $(BASE_INCS) -I../unix
UNIX_CFLAGS=	-DUNIX $(UNIX_INCS) -O -g $(UNIX_GCC_FLAGS)
#UNIX_CFLAGS=	-DUNIX $(UNIX_INCS) -O -g

UNIX_VPATH=	../rocompat:../RISC_OSLib:$(BASE_VPATH)
UNIX_OBJS=	basic1.o $(BASE_OBJS)
UNIX_LIBS=	-lc -lm

PC_CFLAGS =     -apcs 3/26/hardfp

FRESCO_INCS=	-I../\!Fresco
# Don't forget the -Fn above
FRESCO_CFLAGS_COMMON = -DFRESCO $(FRESCO_INCS) -DSGML_REPORTING=0 -DOLD_COMMENTS=0 -DUSE_MARGINS=1 -DMEMLIB=1 -DFRAMES=1 -DNEW_TEXTAREA=1
FRESCO_CFLAGS_REL=	-DDEVELOPMENT=0 $(FRESCO_CFLAGS_COMMON)
FRESCO_CFLAGS_DBG=	-DDEVELOPMENT=1 $(FRESCO_CFLAGS_COMMON) -DMEMWATCH=1

FRESCO_CFLAGS=  $(FRESCO_CFLAGS_DBG)

# fvpr.o is NOT NEEDED in production builds (_REL) but in debug builds (_DBG)

FRESCO_VPATH=	../!Fresco
FRESCO_OBJS=	configgui.o fontmenu.o frontend.o guibits.o guidata.o \
		guimenu.o hotlist.o picker.o savedoc.o statusbar.o \
		listfonts2.o history.o rlib_alloc.o event2.o saveas2.o ole.o \
		fvpr.o transfer.o \
		$(SGML_OBJS) $(HTML_OBJS) $(COMM_OBJS)
FRESCO_LIBS=    ../webimage/webimage ../memlib/memlib

# -----------------------------------------------------------------------------
#
# C compiler flags for CBFresco (v similar to NCFresco)
#

CBFRESCO_VPATH = ../!CBFresco/source

CBFRESCO_INCS=	-I../plingcbfresco/source
CBFRESCO_BASECFLAGS= -DSTBWEB $(CBFRESCO_INCS) $(RISCOS_INCS) -DOLD_COMMENTS=1 -DSGML_PC_UNDEF_KEYS -DUSE_MARGINS=1 -DMEMLIB=0 -DCBPROJECT -Duse_toolbox=1

# Production flag set
CBFRESCO_CFLAGSN= $(CBFRESCO_BASECFLAGS) -DDEVELOPMENT=0 -DMEMWATCH=0 -DSGML_REPORTING=0 -DNDEBUG

# standard debug set
CBFRESCO_CFLAGSD= $(CBFRESCO_BASECFLAGS) -DDEVELOPMENT=2 -DMEMWATCH=2 -Fn -DSGML_REPORTING=1 -DLINK_DEBUG=0 -DUTIL_DEBUG=0

#
# end CBFresco C compiler flags
#
# -------------

BUILDERS_INCS=	-I../bunix -I../RISC_OSLib
BUILDERS_CFLAGS=-DBUILDERS $(BUILDERS_INCS) -DDEVELOPMENT=2 -DSGML_REPORTING=1 -g
BUILDERS_VPATH=	$(RISCOS_VPATH)
BUILDERS_OBJS=	tables.o memwatch.o myassert.o bldrs.o debug.o rid.o \
		$(SGML_OBJS) $(HTML_OBJS) dump.o tablesize.o backend.o \
		obreak.o obullet.o oimage.o oinput.o oselect.o otable.o \
		otext.o otextarea.o basic2.o stream.o object.o config.o \
		oobject.o objects.o usrtrc.o textplot.o rectplot.o \
		coalesce.o fvpr.o gbf.o format.o keyhl.o colspan.o \
		loformat.o cs2.o oscaff.o \
		csgraph.o
BUILDERS_LIBS=

CHECKERS_CFLAGS=-DCHECKER -DBUILDERS -DNO_PTRS $(BUILDERS_INCS) -DDEVELOPMENT=2 -DSGML_REPORTING=1
CHECKERS_OBJS=	tables.o memwatch.o myassert.o debug.o rid.o \
		$(SGML_OBJS) $(HTML_OBJS) dump.o tablesize.o backend.o \
		obreak.o obullet.o oimage.o oinput.o oselect.o otable.o \
		otext.o otextarea.o basic2.o stream.o object.o config.o \
		oobject.o objects.o usrtrc.o textplot.o rectplot.o \
		coalesce.o fvpr.o gbf.o format.o keyhl.o colspan.o \
		loformat.o cs2.o oscaff.o \
		csgraph.o
CHECKERS_LIBS=

HTMLCHECK_INCS=	-I../stdalone
HTMLCHECK_CFLAGS= -DHTMLCHECK $(HTMLCHECK_INCS) -DDEVELOPMENT=1 -DSGML_REPORTING=1
HTMLCHECK_VPATH= ../stdalone
HTMLCHECK_OBJS=	$(SGML_OBJS) htmlcheck.o debug.o memwatch.o myassert.o \
		reports.o htmldefs.o baseprocs.o deliver.o usrtrc.o gbf.o
HTMLCHECK_LIBS=

MOST_INCS=	-I../unix  -I../RISC_OSLib -I../\!Fresco -I../tcpiplibs -I../memlib
MOST_CFLAGS=	-DMOST $(MOST_INCS) -DDEVELOPMENT=2 -DSGML_REPORTING=1 -DUSE_MARGINS=1 -DOLD_COMMENTS=0
MOST_VPATH=	$(FRESCO_VPATH):../commonsrc
MOST_OBJS=	$(FRESCO_OBJS)
MOST_LIBS=

FIXL_INCS=	-I../!Fresco -I../commonsrc
FIXL_CFLAGS=	$(FIXL_INCS)
FIXL_VPATH=	../!Fresco ../commonsrc
FIXL_OBJS=	SerialEnc.o bittabenc.o fixlicence.o

PLOTCHECK_INCS=	-I../plotcheck -I../RISC_OSLib
PLOTCHECK_CFLAGS=-DBUILDERS -DPLOTCHECK -DNO_PTRS $(PLOTCHECK_INCS)  -DDEVELOPMENT=2 -DSGML_REPORTING=0 -DBE_DEBUG=0 -g $(UNIX_CFLAGS) -DPROFILE=1
PLOTCHECK_VPATH=$(RISCOS_VPATH)
PLOTCHECK_OBJS=	tables.o memwatch.o bldrs.o debug.o rid.o \
		$(SGML_OBJS) $(HTML_OBJS)  backend.o \
		obreak.o obullet.o oimage.o oinput.o oselect.o otable.o \
		otext.o otextarea.o basic2.o stream.o object.o config.o \
		oobject.o objects.o usrtrc.o rectplot.o \
		coalesce.o fvpr.o gbf.o format.o keyhl.o colspan.o \
		loformat.o cs2.o oscaff.o csgraph.o rdebug.o \
		profdat.o profile.o dump.o
#PLOTCHECK_LIBS= #-pg 
PLOTCHECK_LIBS=-L/usr/local/lib/gcc-lib/sparc-sun-solaris2.3/2.7.1/ -lgcc


RECONS_INCS=	-I../reconsparse -I../RISC_OSLib
RECONS_CFLAGS=-DBUILDERS -DRECONS -DHTMLCHECK -DNO_PTRS $(RECONS_INCS)  -DDEVELOPMENT=0 -DSGML_REPORTING=1 -O9
RECONS_VPATH=$(RISCOS_VPATH)
RECONS_OBJS=	tables.o memwatch.o bldrs.o debug.o rid.o \
		$(SGML_OBJS) $(HTML_OBJS) dump.o tablesize.o backend.o \
		obreak.o obullet.o oimage.o oinput.o oselect.o otable.o \
		otext.o otextarea.o basic2.o stream.o object.o config.o \
		oobject.o objects.o usrtrc.o rectplot.o \
		coalesce.o fvpr.o gbf.o
RECONS_LIBS=

#############################################################################


# Use this if you don't have automatic python execution.
PYTHON=


# These are ordered - See attrgen.py for what they mean, and see the definition
# for the html target for usage. This list of files is deleted when we do a
# 'make realclean'.
ATTREXTRA=	fresparse/htmldefs.c fresparse/htmldefs.h fresparse/htmleprocs.h \
		stdalone/baseprocs.c

# Generator program - part of the SGML package
ATTRGEN=	parser/attrgen.py

# HTML defintion - part of HTML/Fresco client
ATTRCONF=	fresparse/attrconf

#############################################################################

# Base VPATH. fresco/ncfresco/unix/stdalone may add further items.
BASEVPATH=	../commonsrc:../fresparse:../parser

# Base INCS. As VPATH.
BASEINCS=	-I../commonsrc -I../fresparse -I../parser -I../RISC_OSLib \
		-I../tcpiplibs

#############################################################################

# Directories involved in making a release
REL=		RELEASE
RELF=		$(REL)/\!Fresco
RELFD=		$(RELF)/Docs
RELFR=		$(RELF)/RMStore
RELS=		$(REL)/\!NCFresco

#############################################################################
#
# Basic targets. Please keep alphabetically sorted.
#

br:		BUILDERS_RISCOS

bu:		BUILDERS_UNIX

BUILDERS_RISCOS:	$(PREDEPS)
		@echo "Making BUILDERS for RISCOS"
		@cd ./builders; $(MAKE) BUILDERS_RISCOS \
		LIBS="$(BUILDERS_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(BUILDERS_OBJS) $(UNIX_OBJS)" \
		VPATH="$(BUILDERS_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(BUILDERS_CFLAGS) $(RISCOS_CFLAGS)"
		$(POSTACTS)

BUILDERS_UNIX:	$(PREDEPS)
		@echo "Making BUILDERS for UNIX"
		@cd ./bunix; $(MAKE) $@ \
		LIBS="$(BUILDERS_LIBS) $(UNIX_LIBS)" \
		OBJS="$(BUILDERS_OBJS) $(UNIX_OBJS)" \
		VPATH="$(BUILDERS_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(BUILDERS_CFLAGS) $(UNIX_CFLAGS)"
		$(POSTACTS)

CHECKERS_UNIX:	$(PREDEPS)
		@echo "Making CHECKERS for UNIX"
		@cd ./cunix; $(MAKE) CHECKERS_UNIX \
		LIBS="$(CHECKERS_LIBS) $(UNIX_LIBS)" \
		OBJS="$(CHECKERS_OBJS) $(UNIX_OBJS)" \
		VPATH="$(BUILDERS_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(DEFALSO) $(CHECKERS_CFLAGS) $(UNIX_CFLAGS)"
		$(POSTACTS)

cu: CHECKERS_UNIX

# A circular dependency spanning two executions of make is
# used to ensure that builddate.h is always out of date. The
# problem to be avoid is an error preventing individual targets
# completing enough to remove builddate.h themselves.

#builddate.h:	buildstamp buildstamp-trigger
#		@echo "Building builddate.h"
#		@./buildstamp "%d-%b-%y (%H%M)" > builddate.h
#		@$(RM) buildstamp-trigger
#
#buildstamp:	buildstamp.c
#		@echo "Building buildstamp"
#		@$(CC) -o buildstamp buildstamp.c

#buildstamp-trigger:
#		@echo "Hi there" > buildstamp-trigger

# Arrange that all files will be recompiled. See also realclean
# Note the use of cd && rm - this is for directories that might
# not exist when one doesn't want rm * executed in the wrong
# directory (as happened to me!!!).
clean:		sanity
		@echo "Cleaning out objects and executables"
		-@cd ./fresco_pc && $(RM) *.o *,ff8 *,fd3
		-@cd ./fresco && $(RM) *.o *,ff8 *,fd3
		-@cd ./cbfresco && $(RM) *.o
		-@cd ./cbfresco_pc && $(RM) *.o
		-@cd ./cbfresco_zm && $(RM) *.o
		-@cd ./unix && $(RM) *.o
		-@cd ./ncfresco && $(RM) *.o
		-@cd ./ncfresco_zm && $(RM) *.o
		-@cd ./ncfresco_rel && $(RM) *.o
		-@cd ./stdalone && $(RM) *.o htmlcheck
		-@cd ./bunix && $(RM) *.o builders
		-@cd ./cunix && $(RM) *.o builders parse_check format_check check_all
		-@cd ./hriscos && $(RM) *.o htmlcheck,ff8
		-@cd ./\!Fresco && $(RM) *,ff8 *,fd3
		-@cd ./abs && $(RM) *
		-@cd ./aof && $(RM) *
		-@cd ./rm && $(RM) *
		-@cd ./fresco_rel && $(RM) *.o
		-@cd ./fresco_mc && $(RM) *.o
		-@cd ./fresco_file && $(RM) *.o
		-@cd ./plotcheck && $(RM) plotcheck *.o
		@echo "Removing some build control files"
		@$(RM) */builddate.h $(PREDEPS)

depends:
		@echo "Generating dependencies"
		@echo "Not!"


# Create a fresh directory holding the items that constitute
# a release. Rebuild from scratch every time to be certain
# that lingering files such as cookies don't end up here.
# (Should this make both !Fresco and !NCFresco releases?)
FRESCO_DIST:
		@echo "Building the RELEASE directory"
		-@$(RM) -r $(RELF)
		-@mkdir $(REL)
		@mkdir $(RELF)
		@mkdir $(RELFD)
		@mkdir $(RELFR)
		@cp -p \!Fresco/Docs/antlogo,ff9 $(RELFD)
		@cp -p \!Fresco/RMStore/WebGopher,ffa $(RELFR)
		@cp -p \!Fresco/RMStore/gif,ffd $(RELFR)
		@cp -p \!Fresco/RMStore/httpmod,ffa $(RELFR)
		@cp -p \!Fresco/RMStore/jpeg,ffd $(RELFR)
		@cp -p \!Fresco/RMStore/png,ffd $(RELFR)
		@cp -p \!Fresco/RMStore/sprite,ffd $(RELFR)
		@cp -p \!Fresco/RMStore/webftp,ffa $(RELFR)
		@cp -p \!Fresco/RMStore/xbm,ffd $(RELFR)
		@cp -p \!Fresco/\!Boot,feb $(RELF)
		@cp -p \!Fresco/\!Help $(RELF)
		@chmod 644 $(RELF)/\!Help
		@cp -p \!Fresco/\!RelRun,feb $(RELF)/\!Run,feb
		@cp -p \!Fresco/\!runimage,ff8 $(RELF)
		@cp -p \!Fresco/Sprites,ff9 $(RELF)
		@cp -p \!Fresco/\!Sprites,ff9 $(RELF)
		@cp -p \!Fresco/\!Sprites22,ff9 $(RELF)
		@cp -p \!Fresco/\!Sprites23,ff9 $(RELF)
		@cp -p \!Fresco/Templates,fec $(RELF)
		@cp -p \!Fresco/Welcome,faf $(RELF)
		@cp -p \!Fresco/Messages $(RELF)

		@tar vcf $(REL)/fresco.tar $(RELF)

		@echo ""
		@echo "Built a release directory under $(REL)"
		@echo ""
		@echo "Please check this directory thoroughly before releasing it!"
		@echo "In particular, check dates and version numbers."
		@echo ""

# Search the source code for magic comment markers indicating
# that further thought or work is required.
FIXME:		srclist
		@echo "Searching for magic comments."
		@./fixsearch `cat srclist`
		@$(RM) srclist

fr:		FRESCO_RISCOS

frel:		FRESCO_REL

fmc:		FRESCO_MC

ffo:            FRESCO_FILEONLY

fp:		FRESCO_NT

FRESCO:		FRESCO_RISCOS

# The main cross build.
FRESCO_RISCOS:	$(PREDEPS)
		@echo "Making FRESCO for RISCOS"
		@cd ./fresco; $(MAKE) FRESCO_RISCOS \
		LIBS="$(FRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(FRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(FRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(FRESCO_CFLAGS) $(RISCOS_CFLAGS)"
		$(POSTACTS)

FRESCO_REL:	$(PREDEPS)
		@echo "Making FRESCO for RISCOS production build"
		@cd ./fresco_rel; $(MAKE) FRESCO_RISCOS \
		LIBS="$(FRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(FRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(FRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(FRESCO_CFLAGS_REL) $(RISCOS_CFLAGS_REL)"
		$(POSTACTS)

FRESCO_FILEONLY:	$(PREDEPS)
		@echo "Making FRESCO for RISCOS file only version"
		@cd ./fresco_file; $(MAKE) FRESCO_RISCOS \
		LIBS="$(FRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(FRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(FRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(FRESCO_CFLAGS_REL) $(RISCOS_CFLAGS_REL) -DFILEONLY"
		$(POSTACTS)

FRESCO_MC:      $(PREDEPS)
		@echo "Making FRESCO_MC for RISCOS"
		@cd ./fresco_mc; $(MAKE) FRESCO_RISCOS \
		LIBS="$(FRESCO_LIBS) ../memcheck/mclib $(RISCOS_LIBS)" \
		OBJS="$(FRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(FRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(FRESCO_CFLAGS_DBG) $(RISCOS_CFLAGS_DBG) -zpc1 -DMemCheck_MEMCHECK -fw"
		$(POSTACTS)

FRESCO_NT:
		@echo Making FRESCO on NT
		@cd fresco_pc
		@nmake /nologo /f Makepc FRESCO_PC \
		LIBS="$(FRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(FRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(FRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(FRESCO_CFLAGS) $(RISCOS_CFLAGS) $(PC_CFLAGS)"

FRESCO_NTREL:
		@echo Making FRESCO on NT
		@cd fresco_pc
		@nmake /nologo /f Makepc FRESCO_PC \
		LIBS="$(FRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(FRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(FRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(FRESCO_CFLAGS_REL) $(RISCOS_CFLAGS_REL) $(PC_CFLAGS)"

hr:		HTMLCHECK_RISCOS

# This requires a suitable python interpreter, but otherwise can be run on
# any of the platforms.
HTML:		$(ATTRGEN) $(ATTRCONF)
		@echo "Building htmldefs.c etc from attrconf"
		@$(PYTHON) $(ATTRGEN) $(ATTRCONF) $(ATTREXTRA)

HTMLCHECK_RISCOS:	$(PREDEPS)
		@echo "Making HTMLCHECK for RISCOS"
		@cd ./hriscos; $(MAKE) HTMLCHECK_RISCOS \
		LIBS="$(HTMLCHECK_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(HTMLCHECK_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(HTMLCHECK_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(HTMLCHECK_CFLAGS) $(RISCOS_CFLAGS)"
		$(POSTACTS)

HTMLCHECK_UNIX:	$(PREDEPS)
		@echo "Making HTMLCHECK for UNIX"
		@cd ./stdalone; $(MAKE) HTMLCHECK_UNIX \
		LIBS="$(HTMLCHECK_LIBS) $(UNIX_LIBS)" \
		OBJS="$(HTMLCHECK_OBJS) $(UNIX_OBJS)" \
		VPATH="$(HTMLCHECK_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(HTMLCHECK_CFLAGS) $(UNIX_CFLAGS)"
		$(POSTACTS)

hu:		HTMLCHECK_UNIX


links:
		@echo "Building any necessary soft links"
		@./makelinks

MEMLIB:
		@echo "Building memlib library."
		@cd ./memlib; $(MAKE) memlib


MOST:		MOST_UNIX

MOST_UNIX:	$(PREDEPS)
		@echo "Making MOST for UNIX"
		@cd ./unix; $(MAKE) MOST_UNIX \
		LIBS="$(MOST_LIBS) $(UNIX_LIBS)" \
		OBJS="$(MOST_OBJS) $(UNIX_OBJS)" \
		VPATH="$(MOST_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(MOST_CFLAGS) $(UNIX_CFLAGS)"
		$(POSTACTS)

mr:		MOST_RISCOS

mu:		MOST_UNIX

# NUKE all places that might be using profdat.c inforation. Makes up
# for lack of proper dependencies!
PROFILE:
		@echo "Building profile source files from profile configuration file"
		@bin/profgen.py $(PROF_IN) $(PROF_C) $(PROF_H)
		@echo "You should do a make clean now!"

realclean:	clean sanity
		-@cd ./RISC_OSLib && $(RM) *.o risc_oslib
		-@cd ./memlib && $(RM) *.o memlib
		@echo "Cleaning generated files (eg htmldefs.c)"
		@$(RM) -r $(ATTREXTRA) srclist RELEASE buildstamp TAGS \
		cscope.files cscope.out
		@echo "Removing ALL symbolic links"
		@find . -type l -exec rm {} \;

riscoslib:
		@echo "Making RISC_OSLib"
		@cd ./RISC_OSLib; $(MAKE) -f MakeUnix

# File holding filenames of each source file. These are the files
# that are considered for searching with tools, etc.
srclist:
		-@for dir in fresco parser commonsrc ./!Fresco ./!NCFresco ./plingcbfresco \
		fresparse ncfresco stdalone unix bunix rocompat html http libs mimemapmod \
		tabtests webftp webgopher webimage clib RISC_OSLib include plotcheck \
		tcpiplibs ; do find $$dir -type f \
		\( -name \*.h -o -name \*.c \) ; done > srclist

srclist_fresco:
		-@for dir in fresco parser commonsrc ./!Fresco  \
		fresparse ncfresco stdalone unix bunix rocompat memlib html http libs mimemapmod \
		tabtests webftp webgopher webimage clib RISC_OSLib include \
		tcpiplibs ; do find $$dir -type f \
		\( -name \*.h -o -name \*.c \) ; done > srclist

STDALONE:	HTMLCHECK_UNIX BUILDERS_UNIX


CBFRESCO:	CBFRESCO_RISCOS

CBFRESCO_RISCOS:	$(PREDEPS)
		@echo "Making CBFRESCO for RISCOS"
		@cd ./cbfresco; $(MAKE) CBFRESCO_RISCOS \
		LIBS="$(NCFRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(NCFRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(CBFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(CBFRESCO_CFLAGSN) $(RISCOS_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_LDFLAGS)"
		$(POSTACTS)

CBFRESCO_ZM:	$(PREDEPS)
		@echo "Making CBFRESCO module for RISCOS"
		@cd ./cbfresco_zm; $(MAKE) CBFRESCO_RISCOS \
		LIBS="$(NCFRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(NCFRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(CBFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(CBFRESCO_CFLAGSN) $(RISCOS_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_LDFLAGS)"
		$(POSTACTS)

CBFRESCO_NT:
		@echo Making CBFRESCO on NT
		@cd cbfresco_pc
		@nmake /nologo /f Makepc CBFRESCO_PC \
		LIBS="$(NCFRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(NCFRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(CBFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(CBFRESCO_CFLAGSD) $(RISCOS_CFLAGS) $(PC_CFLAGS)"

FIXL:
		@echo "Making FIXL for UNIX"
		@cd ./fixl; $(MAKE) FIXL_UNIX \
		LIBS="$(FIXL_LIBS) $(UNIX_LIBS)" \
		OBJS="$(FIXL_OBJS)" \
		VPATH="$(FIXL_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(FIXL_CFLAGS) $(UNIX_CFLAGS)"

# Go for all files these days so tags-replace is certain to work.
# Use a better build script I swiped - it handles the very long
# command line problem and also places a TAGS symlink in each
# subdirectory for convenience.
TAGS:		srclist_fresco
		@echo "Building emacs TAGS file for fresco"
		@./MAKE_ETAGS

#		@etags `cat srclist`
#		@$(RM) srclist

TESTS:
		@echo "Performing parser tests."
		@cd ./html/regtests ; $(MAKE)



xref:
		@echo "Building cross referencing HTML files."
		@echo "Not!"

xref1:
		@echo "Building cross referencing HTML files."
		@cd ./unix; $(MAKE) xref OBJS="$(FRES_OBJS)" CFLAGS="$(CLAGS)" \
			BASEVPATH="$(BASEVPATH)" BASEINCS="$(BASEINCS)" \
			OPTS="$(OPTS)"

wg:		WEBGOPHER

WEBGOPHER:
		@echo "Building WebGopher module"
		@cd webgopher ; $(MAKE) WEBGOPHER

#############################################################################

# This runs checks that look for potential problems with the source/CVS
# tree. Please use every so often.

# NB: CURE THE PROBLEM - DON'T JUST STOP THIS REPORTING IT!

sanity:		cvs-checks.py cvs-checks
		@./cvs-checks

#############################################################################

fresh:		HTML PROFILE TAGS
		./makelinks

#############################################################################

pu:		PLOTCHECK

PLOTCHECK:	PLOTCHECK_UNIX

PLOTCHECK_UNIX:	$(PREDEPS)
		@echo "Making PLOTCHECK for UNIX"
		@cd ./plotcheck; $(MAKE) $@ \
		LIBS="$(PLOTCHECK_LIBS) $(UNIX_LIBS)" \
		OBJS="$(PLOTCHECK_OBJS) $(UNIX_OBJS)" \
		VPATH="$(PLOTCHECK_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(PLOTCHECK_CFLAGS) $(UNIX_CFLAGS)"
		$(POSTACTS)

#############################################################################

ru:		RECONS

RECONS:		RECONS_UNIX

RECONS_UNIX:	$(PREDEPS)
		@echo "Making RECONS for UNIX"
		@cd ./reconsparse; $(MAKE) $@ \
		LIBS="$(RECONS_LIBS) $(UNIX_LIBS)" \
		OBJS="$(RECONS_OBJS) $(UNIX_OBJS)" \
		VPATH="$(RECONS_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(RECONS_CFLAGS) $(UNIX_CFLAGS)"
		$(POSTACTS)

#############################################################################

# ===========================================================================
#
# NCFRESCO defines
#
# There are three sets of instructions for some items
# NCFRESCO_BUILD_xxx	- NC build tree standalone application
# NCFRESCO_ROM_xxx	- NC build tree ROM module
# NCFRESCO_RAM_xxx	- NC build tree standalone module
# NCFRESCO_ANT_xxx	- ANT build tree standalone application
#
# ===========================================================================

#uncomment this to build dependencies...
RISCOS_BUILD_EXPORT=../../../../Export

#Two possibilities for exporting to
#RISCOS_BUILD_RESOURCES=../../../../Install
#RISCOS_BUILD_OBJECTS=../../../../Objects
RISCOS_BUILD_RESOURCES=../NCFrescoRO/Resources
RISCOS_BUILD_OBJECTS=../NCFrescoRO/aof
RISCOS_BUILD_HEADERS=../NCFrescoRO/h

NCFRESCO_COUNTRY=		UK
NCFRESCO_WHICH=			NC2

CP=				cp
CPOPTIONS=			-p

NCFRESCO_BUILD_RESOURCES=	$(RISCOS_BUILD_RESOURCES)/Resources/NCFresco
NCFRESCO_BUILD_APPS=		$(RISCOS_BUILD_RESOURCES)/Apps/!NCFresco
NCFRESCO_TEST_DIR=		bungle:/export/ftp/Custom/


NCFRESCO_INCS=			-I../\!NCFresco -I..
NCFRESCO_BUILD_INCS=		-I$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib \
				-I$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs \
				-I$(RISCOS_BUILD_EXPORT)/Lib/MemLib \
				-I$(RISCOS_BUILD_EXPORT)/Lib/WebImage \
				-I$(RISCOS_BUILD_EXPORT)/Lib \
				-I$(RISCOS_BUILD_EXPORT)/C \
				-I$(RISCOS_BUILD_EXPORT)/Lib/Unicode \
				-J$(RISCOS_BUILD_EXPORT)/Lib/CLib

NCFRESCO_BASE_CFLAGS=		-DNEW_TEXTAREA=1 -DBOCA=0 -wp -DSTBWEB -DSTBWEB_BUILD -DOLD_COMMENTS=0 -DUSE_MARGINS=1 -DMEMLIB=1 -DCOMPAT_INET4 \
				$(NCFRESCO_INCS) -DFRAMES=1 -DITERATIVE_PANIC=1 -DNO_LICENSEE -DLOCK_FILES=1 -DNEW_HL=0 -DUSE_FILEWATCH=1 -DSSL_UI=0 \
				-DIGNORE_CAPTION=1 -DUNICODE=1 -DNEW_BREAKS=1

# several alternative flag sets
NCFRESCO_PRODUCTION_CFLAGS=	$(NCFRESCO_BASE_CFLAGS) -DDEVELOPMENT=0 -DMEMWATCH=0 -DSGML_REPORTING=0 -DNDEBUG
NCFRESCO_DEBUG_CFLAGS=		$(NCFRESCO_BASE_CFLAGS) -DDEVELOPMENT=2 -DMEMWATCH=2 -Fn -DSGML_REPORTING=1 -DREMOTE_DEBUG
NCFRESCO_MEMCHECK_CFLAGS=	$(NCFRESCO_BASE_CFLAGS) -DDEVELOPMENT=2 -DMEMWATCH=0 -Fn -DSGML_REPORTING=1 -zpc1 -DMemCheck_MEMCHECK -fw -g
NCFRESCO_HIERPROF_CFLAGS=	$(NCFRESCO_BASE_CFLAGS) -DDEVELOPMENT=0 -DMEMWATCH=0 -Fn -DSGML_REPORTING=0 -DNDEBUG -DHierProf_PROFILE

# choose one here
NCFRESCO_ANT_CFLAGS=		$(NCFRESCO_PRODUCTION_CFLAGS) -DRISCOS -fa -DSTBWEB_ROM=0 \
				$(BASE_INCS) $(NCFRESCO_BUILD_INCS)

NCFRESCO_BUILD_CFLAGS=		$(NCFRESCO_DEBUG_CFLAGS) -DRISCOS -fa -Fn -DSTBWEB_ROM=0 \
				$(BASE_INCS) $(NCFRESCO_BUILD_INCS)

NCFRESCO_ROM_CFLAGS=		$(NCFRESCO_PRODUCTION_CFLAGS) -DRISCOS -fa -zM -DSTBWEB_ROM=1 \
				$(BASE_INCS) $(NCFRESCO_BUILD_INCS)

NCFRESCO_RAM_CFLAGS=		$(NCFRESCO_PRODUCTION_CFLAGS) -DRISCOS -fa -zM -DSTBWEB_ROM=1 \
				$(BASE_INCS) $(NCFRESCO_BUILD_INCS)

NCFRESCO_REL_CFLAGS=		$(NCFRESCO_PRODUCTION_CFLAGS) -DRISCOS -fa -Fn -DSTBWEB_ROM=0 \
				$(BASE_INCS) $(NCFRESCO_BUILD_INCS)

NCFRESCO_ANT_ASFLAGS=		-Ihdr
NCFRESCO_BUILD_ASFLAGS=		-I$(RISCOS_BUILD_EXPORT)/Hdr/Global -I$(RISCOS_BUILD_EXPORT)/Hdr/Interface -I$(RISCOS_BUILD_EXPORT)/Hdr/Interface2 -I..
NCFRESCO_ROM_ASFLAGS=		$(NCFRESCO_BUILD_ASFLAGS)
NCFRESCO_RAM_ASFLAGS=		$(NCFRESCO_BUILD_ASFLAGS) -predefine \"REMOVE_RAMLOAD_CHECK SETL {TRUE}\"
NCFRESCO_REL_ASFLAGS=		$(NCFRESCO_BUILD_ASFLAGS)

NCFRESCO_ANT_LDFLAGS=
NCFRESCO_BUILD_LDFLAGS=	
NCFRESCO_ROM_LDFLAGS=
NCFRESCO_RAM_LDFLAGS=
NCFRESCO_REL_LDFLAGS=

NCFRESCO_VPATH=			../!NCFresco

NCFRESCO_BASE_OBJS=		clipboard.o stbfe.o stbhist.o stbhots.o stbkeys.o stbmap.o \
				stbevent.o stbmenu.o stbopen.o stbredraw.o stbstatus.o stbtb.o stbutils.o \
				alarm.o sutil.o ncreg.o internal.o flex.o heap.o da.o stblinks.o stbcodec.o \
				fvpr.o \
				$(SGML_OBJS) $(HTML_OBJS) $(COMM_OBJS)


NCFRESCO_ANT_OBJS=		$(NCFRESCO_BASE_OBJS)
NCFRESCO_BUILD_OBJS=		$(NCFRESCO_BASE_OBJS) rlib_alloc.o
NCFRESCO_ROM_OBJS=		$(NCFRESCO_BASE_OBJS) rolib.o ModuleWrap.o
NCFRESCO_RAM_OBJS=		$(NCFRESCO_BASE_OBJS) rlib_alloc.o ModuleWrap.o
NCFRESCO_REL_OBJS=		$(NCFRESCO_BASE_OBJS) rlib_alloc.o


NCFRESCO_ANT_LIBS=		../webimage/webimage1

NCFRESCO_BUILD_LIBS=		$(RISCOS_BUILD_EXPORT)/Lib/WebImage/webimage1da.o \
				$(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o \
				../RISC_OSLib/risc_oslib \
				$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/inetlib.o \
				$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/socklib.o \
				$(RISCOS_BUILD_EXPORT)/Lib/Unicode/ucodelibd.o \
				$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibd.o \
				$(RISCOS_BUILD_EXPORT)/Lib/debug/remote.o

#				$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib/risc_oslib.o \
#				$(RISCOS_BUILD_EXPORT)/Lib/Trace/T_PL.o \
#				../memcheck/mclib

NCFRESCO_REL_LIBS=		$(RISCOS_BUILD_EXPORT)/Lib/WebImage/webimage1da.o \
				$(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o \
				$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib/risc_oslib.o \
				$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/inetlib.o \
				$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/socklib.o \
				$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlib.o \
				$(RISCOS_BUILD_EXPORT)/Lib/Unicode/ucodelib.o

NCFRESCO_ROM_LIBS=		$(RISCOS_BUILD_EXPORT)/Lib/WebImage/webimage1da.o \
				$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib/romcstubs.o \
				$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/inetlibzm.o \
				$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/socklibzm.o \
				$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibzm.o \
				$(RISCOS_BUILD_EXPORT)/Lib/Unicode/ucodelibz.o

NCFRESCO_RAM_LIBS=		$(RISCOS_BUILD_EXPORT)/Lib/WebImage/webimage1da.o \
				$(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o \
				../RISC_OSLib/risc_oslibzm \
				$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/inetlibzm.o \
				$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/socklibzm.o \
				$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibzm.o \
				$(RISCOS_BUILD_EXPORT)/Lib/debug/remotezm.o \
				$(RISCOS_BUILD_EXPORT)/Lib/Unicode/ucodelibz.o

# --------------------

sr:		NCFRESCO_RISCOS

NCFRESCO:	NCFRESCO_RISCOS

NCFRESCO_DEPENDS:
		cd ncfresco ; armdepend -M Makefile $(NCFRESCO_BUILD_CFLAGS) ../parser/*.c ../fresparse/*.c ../parser/*.c ../commonsrc/*.c ../!NCFresco/*.c
		cd ncfresco ; chmod a+w Makefile
		@echo NCFresco dependencies generated

# Export the NCFresco resources for ROM building
NCFRESCO_BUILD_ROM_EXPORT_OLD:
		@-mkdir $(RISCOS_BUILD_RESOURCES)
		@-mkdir $(RISCOS_BUILD_RESOURCES)/Resources
		@-mkdir $(RISCOS_BUILD_RESOURCES)/Resources/NCFresco
		@-mkdir $(RISCOS_BUILD_RESOURCES)/Apps
		@-mkdir $(RISCOS_BUILD_RESOURCES)/Apps/!NCFresco
		$(CP) $(CPOPTIONS) Resources/Common/$(NCFRESCO_COUNTRY)/!BootROM,feb			$(NCFRESCO_BUILD_APPS)/!Boot,feb
		$(CP) $(CPOPTIONS) Resources/Common/$(NCFRESCO_COUNTRY)/SetScrap,ffb			$(NCFRESCO_BUILD_RESOURCES)
		$(CP) $(CPOPTIONS) Resources/Common/$(NCFRESCO_COUNTRY)/MimeMap				$(NCFRESCO_BUILD_RESOURCES)
		$(CP) $(CPOPTIONS) Resources/Common/$(NCFRESCO_COUNTRY)/Welcome,faf			$(NCFRESCO_BUILD_RESOURCES)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/!RunROM,feb		$(NCFRESCO_BUILD_APPS)/!Run,feb
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/!SpritesROM,ff9	$(NCFRESCO_BUILD_APPS)/!Sprites,ff9
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/Res,fae		$(NCFRESCO_BUILD_RESOURCES)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/SpritesROM,ff9	$(NCFRESCO_BUILD_RESOURCES)/Sprites,ff9
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/Config		$(NCFRESCO_BUILD_RESOURCES)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/Messages		$(NCFRESCO_BUILD_RESOURCES)
		@-mkdir $(RISCOS_BUILD_OBJECTS)
		$(CP) $(CPOPTIONS) aof/ncfresco $(RISCOS_BUILD_OBJECTS)
		$(CP) $(CPOPTIONS) commonsrc/plugin.h $(RISCOS_BUILD_HEADERS)/plugin


FINDOPTIONS=-name CVS -prune -o -name '*~' -prune -o -type f -print

# copy the files in each wanted directory except for the links and CVS directories
NCFRESCO_BUILD_ROM_EXPORT:
		find Resources/Common/UK $(FINDOPTIONS) -exec $(CP) $(CPOPTIONS) {} $(RISCOS_BUILD_RESOURCES)/Common/UK/ \;
		find Resources/RCA/UK $(FINDOPTIONS) -exec $(CP) $(CPOPTIONS) {} $(RISCOS_BUILD_RESOURCES)/RCA/UK/ \;
		find Resources/NC2/UK $(FINDOPTIONS) -exec $(CP) $(CPOPTIONS) {} $(RISCOS_BUILD_RESOURCES)/NC2/UK/ \;
		find Resources/CM/UK $(FINDOPTIONS) -exec $(CP) $(CPOPTIONS) {} $(RISCOS_BUILD_RESOURCES)/Peregrine2/UK/ \;
		-$(CP) $(CPOPTIONS) aof/ncfresco			$(RISCOS_BUILD_OBJECTS)
		-$(CP) $(CPOPTIONS) aof/ncf_flash			$(RISCOS_BUILD_OBJECTS)
		$(CP) $(CPOPTIONS) commonsrc/plugin.h			$(RISCOS_BUILD_HEADERS)/plugin

# NCFresco application in the new style CVS controlled RISC OS build tree
NCFRESCO_BUILD:		$(PREDEPS)
		@echo "Making NCFRESCO for RISCOS in the build tree"
		@-mkdir abs
		@cd ./ncfresco; $(MAKE) NCFRESCO_RISCOS \
		LIBS="$(NCFRESCO_BUILD_LIBS)" \
		OBJS="$(NCFRESCO_BUILD_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		ASFLAGS="$(NCFRESCO_BUILD_ASFLAGS)" \
		CFLAGS="$(NCFRESCO_BUILD_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_BUILD_LDFLAGS)"
		$(POSTACTS)

# NCFresco production application in the new style CVS controlled RISC OS build tree
NCFRESCO_REL:		$(PREDEPS)
		@echo "Making NCFRESCO for RISCOS in the build tree"
		@-mkdir abs
		@cd ./ncfresco_rel; $(MAKE) NCFRESCO_RISCOS \
		LIBS="$(NCFRESCO_REL_LIBS)" \
		OBJS="$(NCFRESCO_REL_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		ASFLAGS="$(NCFRESCO_REL_ASFLAGS)" \
		CFLAGS="$(NCFRESCO_REL_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_REL_LDFLAGS)"
		$(POSTACTS)

# NCFresco ROM aof file in the new style CVS controlled RISC OS build tree
NCFRESCO_ROM:	$(PREDEPS)
		@echo "Making NCFRESCO ROM version for RISCOS in the build tree"
		@-mkdir aof
		@cd ./ncfresco_zm; $(MAKE) NCFRESCO_AOF \
		LIBS="$(NCFRESCO_ROM_LIBS)" \
		OBJS="$(NCFRESCO_ROM_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_ROM_CFLAGS)" \
		ASFLAGS="$(NCFRESCO_ROM_ASFLAGS)" \
		LDFLAGS="$(NCFRESCO_ROM_LDFLAGS)"
		$(POSTACTS) \

# NCFresco ROM aof file in the new style CVS controlled RISC OS build tree
NCFRESCO_FLASH:	$(PREDEPS)
		@echo "Making NCFRESCO Flash version for RISCOS in the build tree"
		@-mkdir aof
		@cd ./ncfresco_zm; $(MAKE) NCFRESCO_FLASH \
		LIBS="$(NCFRESCO_ROM_LIBS)" \
		OBJS="$(NCFRESCO_ROM_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_ROM_CFLAGS)" \
		ASFLAGS="$(NCFRESCO_RAM_ASFLAGS)" \
		LDFLAGS="$(NCFRESCO_ROM_LDFLAGS)"
		$(POSTACTS) \

# NCFresco application in ANT build tree
NCFRESCO_RISCOS:	$(PREDEPS)
		@echo "Making NCFRESCO for RISCOS"
		@cd ./ncfresco; $(MAKE) NCFRESCO_RISCOS \
		LIBS="$(NCFRESCO_ANT_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(NCFRESCO_ANT_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_ANT_CFLAGS) $(RISCOS_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_ANT_LDFLAGS)"
		$(POSTACTS)


# NCFresco application cross build on NT
NCFRESCO_NT:
		@echo Making NCFRESCO on NT
		@cd ncfresco_pc
		@nmake /nologo /f Makepc NCFRESCO_PC \
		LIBS="$(NCFRESCO_ANT_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(NCFRESCO_ANT_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="-I../tcpiplibs $(NCFRESCO_ANT_CFLAGS) $(RISCOS_CFLAGS) -USTBWEB_BUILD" \
		LDFLAGS="$(NCFRESCO_ANT_LDFLAGS)" \
		$(POSTACTS)

# NCFresco softload module in the new style CVS controlled RISC OS build tree
NCFRESCO_RAM:	$(PREDEPS)
		@echo "Making NCFRESCO module for RISCOS"
		@cd ./ncfresco_zm; $(MAKE) NCFRESCO_MODULE \
		LIBS="$(NCFRESCO_RAM_LIBS)" \
		OBJS="$(NCFRESCO_RAM_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_RAM_CFLAGS)" \
		ASFLAGS="$(NCFRESCO_RAM_ASFLAGS)" \
		LDFLAGS="$(NCFRESCO_RAM_LDFLAGS)"
		$(POSTACTS)

ncfresco_srclist:
		@for dir in fresparse parser commonsrc ./!NCFresco \
		http webftp webgopher webimage \
		 ; do find $$dir -type f \
		\( -name \*.h -o -name \*.c \) ; done > srclist

NCFRESCO_TAGS:	ncfresco_srclist
		@echo "Building emacs TAGS file for ncfresco"
		@etags `cat srclist`
		@$(RM) srclist

# Create a fresh directory holding the items that constitute
# a release. Rebuild from scratch every time to be certain
# that lingering files such as cookies don't end up here.
NCFRESCO_DIST:  NCFRESCO_BUILD
		@echo "Building the NCFRESCO RELEASE directory"
		-@$(RM) -r $(RELS)
		-@mkdir $(REL)
		@mkdir $(RELS)
		$(CP) $(CPOPTIONS) Resources/Common/$(NCFRESCO_COUNTRY)/\!Boot,feb			$(RELS)
		$(CP) $(CPOPTIONS) Resources/Common/$(NCFRESCO_COUNTRY)/SetScrap,ffb			$(RELS)
		$(CP) $(CPOPTIONS) Resources/Common/$(NCFRESCO_COUNTRY)/MimeMap				$(RELS)
		$(CP) $(CPOPTIONS) Resources/Common/$(NCFRESCO_COUNTRY)/Welcome,faf			$(RELS)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/\!Run,feb		$(RELS)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/\!Sprites,ff9	$(RELS)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/Res,fae		$(RELS)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/Sprites,ff9		$(RELS)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/Config		$(RELS)
		$(CP) $(CPOPTIONS) Resources/$(NCFRESCO_WHICH)/$(NCFRESCO_COUNTRY)/Messages		$(RELS)

		$(CP) $(CPOPTIONS) ./abs/ncfresco							$(RELS)/\!RunImage,ff8

		@chmod 644 $(RELS)/MimeMap
		@chmod 644 $(RELS)/Config
		@chmod 644 $(RELS)/Messages

		@echo ""
		@echo "Built an Ncfresco release directory under $(REL)"
		@echo ""
		@echo "Please check this directory thoroughly before releasing it!"
		@echo ""


NCFRESCO_RELEASE: NCFRESCO_BUILD FIXL
		@echo "Building NCFRESCO release pack"
		@cp -pr $(RELS) ../release/browser
		@./fixlicense ../release/browser/\!NCFresco/\!RunImage,ff8 1000000 "Acorn Network Computers Dev"
		@cp -pr ../release/stbmods/* ../release/browser/\!NCFresco
		@cd ../release ; tar cf ncfresco.tar browser
		@gzip -f ../release/ncfresco.tar
		@uuencode ../release/ncfresco.tar.gz ../release/ncfresco.tar.gz > ../release/ncfresco.tar.gz.uue
		@echo "Done"

NCFRESCO_TEST:	NCFRESCO_DIST
		@echo "Installing NCFresco release on bungle"
		@rcp -pr RELEASE/\!NCFresco '$(NCFRESCO_TEST_DIR)/Apps'
		@echo "Done"
		@echo ""

NCFRESCO_BINARIES:
		@echo "packaging NCFresco binary resources"
		@tar cvf ~/tmp/ncfrescobin.tar \!NCFresco/Resources/UK/*Sprites* \!NCFresco/Resources/UK/Res* \!NCFresco/Resources/UK/Messages
		@gzip -f ~/tmp/ncfrescobin.tar
		@uuencode ~/tmp/ncfrescobin.tar.gz ~/tmp/ncfrescobin.tar.gz > ~/tmp/ncfrescobin.tar.gz.uue

# ========================================================================================================================


# eof fresco/Makefile

