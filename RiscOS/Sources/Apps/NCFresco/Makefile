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
		NCFRESCO_RISCOS riscoslib TAGS TESTS xref

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
PREDEPS=	builddate.h
POSTACTS=	@$(RM) $(PREDEPS)
#PREDEPS=
#POSTACTS=

#############################################################################

SGML_OBJS=	attrparse.o elements.o entities.o sgmlparser.o statemach.o \
		support.o portutil.o genproc.o chopper.o

HTML_OBJS=	block.o builders.o deliver.o frames.o fonts.o forms.o head.o \
		hparse.o htmldefs.o lists.o misc.o phrase.o reports.o special.o \
		objparse.o

COMM_OBJS=	Serial.o access.o auth.o backend.o bittabs.o config.o \
		dfsupport.o dir2html.o drawfile.o dump.o imagemap.o \
		images.o layout.o licence.o makeerror.o memwatch.o \
		myassert.o object.o obreak.o obullet.o oimage.o oinput.o \
		oselect.o otable.o otext.o otextarea.o printing.o render.o \
		resolve.o rid.o tables.o threads.o \
		unwind.o url.o util.o version.o webfonts.o \
		cookie.o cache2.o cache3.o stream.o debug.o usrtrc.o \
		tablesize.o oobject.o objects.o plugin.o pluginfile.o \
		files.o rlib_alloc.o fvpr.o remote.o coalesce.o keyhl.o

BASE_VPATH=	..:../commonsrc:../fresparse:../parser
BASE_INCS=	-I.. -I../commonsrc -I../fresparse -I../parser

#############################################################################

# This set of variables is the basic control over what is built

RISCOS_INCS=	-I../RISC_OSLib -I../include/unix -I../tcpiplibs $(BASE_INCS)  -I../memlib -J../clib
#RISCOS_INCS=	-I../RISC_OSLib -I../tcpiplibs $(BASE_INCS) -I../clib
# -Fn *includes* function names (at least for this option set)
RISCOS_CFLAGS=	-DRISCOS $(RISCOS_INCS) -fa -apcs 3/26 -Fn
# RISCOS_CFLAGS=	-DRISCOS $(RISCOS_INCS) -ffa -DCOMPAT_INET4
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

UNIX_INCS=	-I../rocompat -I../RISC_OSLib $(BASE_INCS) -I../unix
UNIX_CFLAGS=	-DUNIX $(UNIX_INCS) -O -g \
		-Wall \
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
UNIX_VPATH=	../rocompat:../RISC_OSLib:$(BASE_VPATH)
UNIX_OBJS=	basic1.o $(BASE_OBJS)
UNIX_LIBS=	-lc -lm

PC_CFLAGS =     -apcs 3/26/hardfp

FRESCO_INCS=	-I../\!Fresco
# Don't forget the -Fn above
#FRESCO_CFLAGS=	-DFRESCO $(FRESCO_INCS) -DDEVELOPMENT=0 -DSGML_REPORTING=0 -DOLD_COMMENTS=0 -DUSE_MARGINS=1 -DMEMLIB=1
FRESCO_CFLAGS=	-DFRESCO $(FRESCO_INCS) -DDEVELOPMENT=1 -DSGML_REPORTING=0 -DOLD_COMMENTS=0 -DUSE_MARGINS=1 -DMEMLIB=1
FRESCO_VPATH=	../!Fresco
FRESCO_OBJS=	configgui.o fontmenu.o frontend.o guibits.o guidata.o \
		guimenu.o hotlist.o picker.o savedoc.o statusbar.o \
		listfonts2.o history.o \
		$(SGML_OBJS) $(HTML_OBJS) $(COMM_OBJS)
FRESCO_LIBS=    ../webimage/webimage ../memlib/memlib

# ----------------------------------------------------------------------

# Consider changing to OLD_COMMENTS=0

NCFRESCO_INCS=	-I../\!NCFresco
NCFRESCO_BASECFLAGS= -wp -DSTBWEB $(NCFRESCO_INCS) -DOLD_COMMENTS=0 -DSGML_PC_UNDEF_KEYS -DUSE_MARGINS=1 -DMEMLIB=1 -DCOMPAT_INET4

# Production flag set
# NCFRESCO_CFLAGS= $(NCFRESCO_BASECFLAGS) -DDEVELOPMENT=0 -DMEMWATCH=0 -DSGML_REPORTING=0 -DNDEBUG

# standard debug set
NCFRESCO_CFLAGS= $(NCFRESCO_BASECFLAGS) -DDEVELOPMENT=2 -DMEMWATCH=2 -Fn -DSGML_REPORTING=1 -DLINK_DEBUG=1

# MemCheck debug set
# NCFRESCO_CFLAGS= $(NCFRESCO_BASECFLAGS) -DDEVELOPMENT=2 -DMEMWATCH=0 -Fn -DSGML_REPORTING=1 -DLINK_DEBUG=0 -zpc1 -DMemCheck_MEMCHECK -fw -g

# Hierprof debug set
#NCFRESCO_CFLAGS= $(NCFRESCO_BASECFLAGS) -DDEVELOPMENT=0 -DMEMWATCH=0 -Fn -DSGML_REPORTING=0 -DNDEBUG -DHierProf_PROFILE

NCFRESCO_VPATH=	../!NCFresco
NCFRESCO_BASEOBJS=	clipboard.o stbfe.o stbhist.o stbhots.o stbkeys.o stbmap.o \
		stbevent.o stbmenu.o stbopen.o stbredraw.o stbstatus.o stbtb.o stbutils.o \
		alarm.o sutil.o ncreg.o internal.o \
		$(SGML_OBJS) $(HTML_OBJS) $(COMM_OBJS)
NCFRESCO_MEMOBJS= flex.o heap.o da.o

NCFRESCO_OBJS=	$(NCFRESCO_BASEOBJS)
# NCFRESCO_OBJS=	$(NCFRESCO_BASEOBJS) $(NCFRESCO_MEMOBJS)
NCFRESCO_LIBS=	../webimage/webimage1
# NCFRESCO_LIBS=	../webimage/webimage1m ../memcheck/MC
NCFRESCO_LDFLAGS=

# -------------
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
BUILDERS_CFLAGS=-DBUILDERS $(BUILDERS_INCS)  -DDEVELOPMENT=2 -DSGML_REPORTING=1 -g
BUILDERS_VPATH=	$(RISCOS_VPATH)
BUILDERS_OBJS=	tables.o memwatch.o myassert.o bldrs.o debug.o rid.o \
		$(SGML_OBJS) $(HTML_OBJS) dump.o tablesize.o backend.o \
		obreak.o obullet.o oimage.o oinput.o oselect.o otable.o \
		otext.o otextarea.o basic2.o stream.o object.o config.o \
		oobject.o objects.o usrtrc.o textplot.o rectplot.o \
		coalesce.o fvpr.o
BUILDERS_LIBS=	-lefence
CHECKERS_CFLAGS=-DCHECKER -DBUILDERS $(BUILDERS_INCS)  -DDEVELOPMENT=2 -DSGML_REPORTING=1
CHECKERS_OBJS=	tables.o memwatch.o myassert.o debug.o rid.o \
		$(SGML_OBJS) $(HTML_OBJS) dump.o tablesize.o backend.o \
		obreak.o obullet.o oimage.o oinput.o oselect.o otable.o \
		otext.o otextarea.o basic2.o stream.o object.o config.o \
		oobject.o objects.o usrtrc.o textplot.o rectplot.o \
		coalesce.o fvpr.o
CHECKERS_LIBS=

HTMLCHECK_INCS=	-I../stdalone
HTMLCHECK_CFLAGS= -DHTMLCHECK $(HTMLCHECK_INCS) -DDEVELOPMENT=1 -DSGML_REPORTING=1
HTMLCHECK_VPATH= ../stdalone
HTMLCHECK_OBJS=	$(SGML_OBJS) htmlcheck.o debug.o memwatch.o myassert.o \
		reports.o htmldefs.o baseprocs.o deliver.o usrtrc.o
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

builddate.h:	buildstamp buildstamp-trigger
		@echo "Building builddate.h"
		@./buildstamp "%d-%b-%y (%H%M)" > builddate.h
		@$(RM) buildstamp-trigger

buildstamp:	buildstamp.c
		@echo "Building buildstamp"
		@$(CC) -o buildstamp buildstamp.c

buildstamp-trigger:
		@echo "Hi there" > buildstamp-trigger

# Arrange that all files will be recompiled. See also realclean
# Note the use of cd && rm - this is for directories that might
# not exist when one doesn't want rm * executed in the wrong
# directory (as happened to me!!!).
clean:		sanity
		@echo "Cleaning out objects and executables"
		-cd ./fresco_pc && $(RM) *.o *,ff8 *,fd3
		-@cd ./fresco ; $(RM) *.o *,ff8 *,fd3
		-@cd ./cbfresco && $(RM) *.o
		-@cd ./cbfresco_pc && $(RM) *.o
		-@cd ./cbfresco_zm && $(RM) *.o
		-@cd ./unix ; $(RM) *.o
		-@cd ./ncfresco ; $(RM) *.o *,ff8 *,fd3
		-@cd ./stdalone; $(RM) *.o htmlcheck
		-@cd ./bunix ; $(RM) *.o builders
		-@cd ./cunix ; $(RM) *.o builders parse_check format_check check_all
		-@cd ./hriscos ; $(RM) *.o htmlcheck,ff8
		-@cd ./\!Fresco ; $(RM) *,ff8 *,fd3
		-@cd ./\!NCFresco ; $(RM) *,ff8 *,fd3
		-@cd ./abs && $(RM) *
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

FRESCO_NT:	$(PREDEPS)
		@echo Making FRESCO on NT
		@cd fresco_pc
		@nmake /nologo /f Makepc FRESCO_PC \
		LIBS="$(FRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(FRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(FRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(FRESCO_CFLAGS) $(RISCOS_CFLAGS) $(PC_CFLAGS)"

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

realclean:	clean sanity
		@cd ./RISC_OSLib && $(RM) *.o risc_oslib
		@cd ./memlib && $(RM) *.o memlib
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
		@for dir in fresco parser commonsrc ./!Fresco ./!NCFresco ./plingcbfresco \
		fresparse ncfresco stdalone unix bunix rocompat html http libs mimemapmod \
		tabtests webftp webgopher webimage clib RISC_OSLib include \
		tcpiplibs ; do find $$dir -type f \
		\( -name \*.h -o -name \*.c \) ; done > srclist

srclist_ncfresco:
		@for dir in fresco parser commonsrc ./!NCFresco \
		fresparse ncfresco stdalone unix bunix rocompat html http libs mimemapmod \
		tabtests webftp webgopher webimage clib RISC_OSLib include \
		tcpiplibs ; do find $$dir -type f \
		\( -name \*.h -o -name \*.c \) ; done > srclist

srclist_fresco:
		@for dir in fresco parser commonsrc ./!Fresco  \
		fresparse ncfresco stdalone unix bunix rocompat memlib html http libs mimemapmod \
		tabtests webftp webgopher webimage clib RISC_OSLib include \
		tcpiplibs ; do find $$dir -type f \
		\( -name \*.h -o -name \*.c \) ; done > srclist



STDALONE:	HTMLCHECK_UNIX BUILDERS_UNIX

sr:		NCFRESCO_RISCOS

NCFRESCO:	NCFRESCO_RISCOS

NCFRESCO_BUILD_LIBS=	$(RISCOS_BUILD_EXPORT)/Lib/WebImage/webimage1.o \
			$(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o \
			$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib/risc_oslib.o \
			$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/inetlib.o \
			$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/socklib.o \
			$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlib.o

NCFRESCO_ROM_LIBS=	$(RISCOS_BUILD_EXPORT)/Lib/WebImage/webimage1.o \
			$(RISCOS_BUILD_EXPORT)/Lib/CLib/romstubs.o \
			$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs/inetlibzm.o \
			$(RISCOS_BUILD_EXPORT)/Lib/MemLib/memlibzm.o

NCFRESCO_BUILD_CFLAGS=	$(NCFRESCO_CFLAGS) -DRISCOS -fa -Fn \
			-I$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib \
			-I$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs \
			-I$(RISCOS_BUILD_EXPORT)/Lib/MemLib \
			$(BASE_INCS) -J$(RISCOS_BUILD_EXPORT)/Lib/CLib

NCFRESCO_ROM_CFLAGS=	$(NCFRESCO_CFLAGS) -DRISCOS -fa \
			-I$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib \
			-I$(RISCOS_BUILD_EXPORT)/Lib/TCPIPLibs \
			-I$(RISCOS_BUILD_EXPORT)/Lib/MemLib \
			$(BASE_INCS) -J$(RISCOS_BUILD_EXPORT)/Lib/CLib

NCFRESCO_ROM_LDFLAGS=	-aof

# This one is for NCFresco in the new style CVS controlled RISC OS build tree
NCFRESCO_BUILD:		$(PREDEPS)
		@echo "Making NCFRESCO for RISCOS in the build tree"
		@-mkdir abs
		@cd ./ncfresco; $(MAKE) NCFRESCO_RISCOS \
		LIBS="$(NCFRESCO_BUILD_LIBS)" \
		OBJS="$(NCFRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_BUILD_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_LDFLAGS)"
		$(POSTACTS)

NCFRESCO_BUILD_ROM:	$(PREDEPS)
		@echo "Making NCFRESCO ROM version for RISCOS in the build tree"
		@-mkdir aof
		@cd ./ncfresco; $(MAKE) NCFRESCO_RISCOS \
		LIBS="$(NCFRESCO_ROM_LIBS)" \
		OBJS="$(NCFRESCO_ROM_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_ROM_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_ROM_LDFLAGS)"
		$(POSTACTS)

NCFRESCO_RISCOS:	$(PREDEPS)
		@echo "Making NCFRESCO for RISCOS"
		@cd ./ncfresco; $(MAKE) NCFRESCO_RISCOS \
		LIBS="$(NCFRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(NCFRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_CFLAGS) $(RISCOS_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_LDFLAGS)"
		$(POSTACTS)


NCFRESCO_NT:
		@echo Making NCFRESCO on NT
		@cd ncfresco_pc
		@nmake /nologo /f Makepc NCFRESCO_PC \
		LIBS="$(NCFRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(NCFRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_CFLAGS) $(RISCOS_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_LDFLAGS)" \
		$(POSTACTS)

NCFRESCO_MODULE:	$(PREDEPS)
		@echo "Making NCFRESCO module for RISCOS"
		@cd ./ncfresco; $(MAKE) NCFRESCO_MODULE \
		LIBS="$(NCFRESCO_MODLIBS)" \
		OBJS="$(NCFRESCO_MODOBJS) $(RISCOS_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_MODCFLAGS) $(RISCOS_CFLAGS)" \
		ASFLAGS="$(NCFRESCO_ASFLAGS)" \
		LDFLAGS="$(NCFRESCO_MODLDFLAGS)"
		$(POSTACTS)


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


NCFRESCO_BUILDSTAMP:
		@cp -f ../buildstamp .

# Create a fresh directory holding the items that constitute
# a release. Rebuild from scratch every time to be certain
# that lingering files such as cookies don't end up here.
NCFRESCO_DIST:  NCFRESCO_BUILD
		@echo "Building the NCFRESCO RELEASE directory"
		-@$(RM) -r $(RELS)
		-@mkdir $(REL)
		@mkdir $(RELS)
		@cp -p \!NCFresco/Resources/UK/\!Boot1,feb $(RELS)/\!Boot,feb
		@cp -p \!NCFresco/Resources/UK/\!Run3,feb $(RELS)/\!Run,feb
		@cp -p ./abs/ncfresco $(RELS)/\!RunImage,ff8
		@cp -p \!NCFresco/Resources/UK/\!Sprites3,ff9 $(RELS)/\!Sprites,ff9
		@cp -p \!NCFresco/Resources/UK/Res,fae $(RELS)
		@cp -p \!NCFresco/Resources/UK/Sprites3,ff9 $(RELS)/Sprites,ff9
		@cp -p \!NCFresco/Resources/UK/TileV,ff9 $(RELS)
		@cp -p \!NCFresco/Resources/UK/TileN,ff9 $(RELS)
		@cp -p \!NCFresco/Resources/UK/\!Help $(RELS)
		@cp -p \!NCFresco/Resources/UK/MimeMap $(RELS)
		@cp -p \!NCFresco/Resources/UK/Config $(RELS)
		@cp -p \!NCFresco/Resources/UK/Messages $(RELS)
		@cp -p \!NCFresco/Resources/UK/Help,faf $(RELS)
		@cp -p \!NCFresco/Resources/UK/Welcome,faf $(RELS)
		@cp -p \!NCFresco/Resources/UK/SetScrap,ffb $(RELS)

		@chmod 644 $(RELS)/\!Help
		@chmod 644 $(RELS)/MimeMap
		@chmod 644 $(RELS)/Config
		@chmod 644 $(RELS)/Messages

#		@tar cf ncfresco.tar $(RELS)
#		@gzip ncfresco.tar

		@echo ""
		@echo "Built an Ncfresco release directory under $(REL)"
		@echo ""
		@echo "Please check this directory thoroughly before releasing it!"
		@echo ""

NCFRESCO_RELEASE: NCFRESCO_BUILD
		@echo "Building NCFRESCO release pack"
		@cp -pr $(RELS) ../release/browser
		@./fixlicense ../release/browser/\!NCFresco/\!RunImage,ff8 1000000 "Acorn Network Computers Dev"
		@cp -pr ../release/stbmods/* ../release/browser/\!NCFresco
		@cd ../release ; tar cf ncfresco.tar browser
		@gzip -f ../release/ncfresco.tar
		@uuencode ../release/ncfresco.tar.gz ../release/ncfresco.tar.gz > ../release/ncfresco.tar.gz.uue
		@echo "Done"


NCFRESCO_TEST_RUNIMAGE: NCFRESCO_BUILD
		@echo "Updating NCFresco installation on bungle"
		@rcp -r abs/ncfresco 'bungle:/export/home/ncd/default/boot/WWW/Dev/\!NCFresco'
		@echo "Done"
		@echo ""

NCFRESCO_TEST_RUNIMAGE_STAMP: NCFRESCO_BUILD
		@echo "Updating NCFresco installation on bungle"
		@rcp -r ../release/browser/\!NCFresco/\!RunImage,ff8 'bungle:/export/home/ncd/default/boot/WWW/Dev/\!NCFresco'
		@echo "Done"
		@echo ""

NCFRESCO_TEST:	NCFRESCO_DIST
		@echo "Installing NCFresco release on bungle"
		@rcp -r RELEASE/\!NCFresco 'bungle:/export/home/ncd/default/boot/WWW/Dev'
		@echo "Done"
		@echo ""

NCFRESCO_BINARIES:
		@echo "packaging NCFresco binary resources"
		@tar cvf ../ncfrescobin.tar \!NCFresco/Resources/UK/*Sprites* \!NCFresco/Resources/UK/Res* \!NCFresco/Resources/UK/Messages
		@gzip -f ../ncfrescobin.tar
		@uuencode ../ncfrescobin.tar.gz ../ncfrescobin.tar.gz > ../ncfrescobin.tar.gz.uue

FIXL:
		@echo "Making FIXL for UNIX"
		@cd ./fixl; $(MAKE) FIXL_UNIX \
		LIBS="$(FIXL_LIBS) $(UNIX_LIBS)" \
		OBJS="$(FIXL_OBJS)" \
		VPATH="$(FIXL_VPATH):$(UNIX_VPATH)" \
		CFLAGS="$(FIXL_CFLAGS) $(UNIX_CFLAGS)"

# This is ordered to get matches in the main files first. Note that !Fresco
# starts with a !, which find takes as meaninga predicate. Hence ./
TAGS:		srclist_fresco
		@echo "Building emacs TAGS file for fresco"
		@etags `cat srclist`
		@$(RM) srclist

TAGS_NCFRESCO:	srclist_ncfresco
		@echo "Building emacs TAGS file for ncfresco"
		@etags `cat srclist`
		@$(RM) srclist

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

fresh:		HTML TAGS
		./makelinks

#############################################################################

# eof fresco/Makefile

