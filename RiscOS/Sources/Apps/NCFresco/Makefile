## fresco/Makefile - top level Fresco makefile.
#
# Although currently targeted at UN*X type make's, the intention is that
# this file be movable to a RISC OS compilation host if desired.
#
#############################################################################
#
# $Log$
# Revision 1.1.2.1  1997/02/18 15:40:07  smiddle
# NCFresco 1.06 in cross-compile form (!RunImage form only)
#
# Revision 1.104  1996/10/28 18:01:25  fresco
# Version frozen for NCFresco for ROM
#
# Revision 1.103  1996/10/25 10:54:31  fresco
# Merged SI_961024
#
# Revision 1.102  1996/10/24 20:41:07  fresco
# Added memcheck header file
#
# Revision 1.101  1996/10/24 02:29:28  fresco
# Merged SI_961023
#
# Revision 1.100  1996/10/18 11:45:05  fresco
# Merged SI_961017
#
# Revision 1.99  1996/10/16 02:52:11  fresco
# Merged SI_961015
#
# Revision 1.98  1996/10/15 15:35:31  fresco
# Added FRESCO_PC and fp targets
#
# Revision 1.97  1996/10/14 22:00:25  fresco
# Merged SI_961014
#
# Revision 1.96  1996/10/14 20:25:53  fresco
# Table width desperation stomping code
#
# Revision 1.95  1996/10/14 02:09:22  fresco
# Back to old table height sharing code
#
# Revision 1.94  1996/10/13 16:14:14  fresco
# Minor bug tweaks
#
# Revision 1.93  1996/10/13 14:47:39  fresco
# Intermediate state
#
# Revision 1.92  1996/10/10 23:46:31  fresco
# Makefile tweak
#
# Revision 1.91  1996/10/10 23:37:51  fresco
# Gopher tweak, etc.
#
# Revision 1.90  1996/10/10 23:33:41  fresco
# Merged SI_961010
#
# Revision 1.89  1996/10/09 17:15:41  fresco
# Fixed a few bugs
#
# Revision 1.88  1996/10/09 16:29:37  fresco
# Merged SI_961009
#
# Revision 1.87  1996/10/09 16:18:50  fresco
# Fixed more table bugs
#
# Revision 1.86  1996/10/04 00:57:22  fresco
# Merged SI_961003
#
# Revision 1.85  1996/10/03 17:17:11  fresco
# New memory management stuff (in directory memlib)
#
# Revision 1.84  1996/09/27 01:52:35  fresco
# Various production build tweaks
#
# Revision 1.83  1996/09/27 01:24:12  fresco
# Merged SI_960926
#
# Revision 1.82  1996/09/26 05:36:55  fresco
# Merged SI_960925
#
# Revision 1.81  1996/09/24 17:32:19  fresco
# 1.30 production
#
# Revision 1.80  1996/09/24 17:00:20  fresco
# Guess what - bug fixes
#
# Revision 1.79  1996/09/24 01:01:24  fresco
# <LI> bug fix
#
# Revision 1.78  1996/09/23 23:51:46  fresco
# Formatting bug fixes. Tidy up
#
# Revision 1.77  1996/09/23 16:58:17  fresco
# More bloody table formatting
#
# Revision 1.76  1996/09/23 03:17:26  fresco
# Merging 1.30a bits
#
# Revision 1.75  1996/09/23 02:08:56  fresco
# Minor tweaks
#
# Revision 1.74  1996/09/23 00:28:35  fresco
# 1.30 tentative
#
# Revision 1.73  1996/09/22 23:30:31  fresco
# Proto 1.30 version
#
# Revision 1.72  1996/09/20 16:59:27  fresco
# SGML_REPORTING changes, font fix
#
# Revision 1.71  1996/09/20 16:46:40  fresco
# Merged SI_960919
#
# Revision 1.70  1996/09/19 13:52:57  fresco
# Oops, undid changes I'd made and accidentally committed. Apologies. -- pdh
#
# Revision 1.69  1996/09/19 13:15:53  fresco
# Changed fontmenu.c to use RO3.1 swis: fixes a couple of bugs
#
# Revision 1.68  1996/09/16 19:05:10  fresco
# Merged SI_960916
#
# Revision 1.67  1996/09/16 12:57:23  fresco
# Merged SI_960913
#
# Revision 1.66  1996/09/12 07:09:42  fresco
# Merged SI_960911
#
# Revision 1.65  1996/09/12 07:02:57  fresco
# More tweaking to table borders
#
# Revision 1.64  1996/09/11 01:07:42  fresco
# Merged SI_960910
#
# Revision 1.63  1996/09/09 17:40:20  fresco
# Merged SI_960909
#
# Revision 1.62  1996/09/08 05:27:05  fresco
# Merged SI_960906
#
# Revision 1.61  1996/09/06 03:39:52  fresco
# Merged SI_960905 and bits
#
# Revision 1.60  1996/09/04 19:58:21  fresco
# Merged SI_960904 and TAR file
#
# Revision 1.59  1996/09/04 15:32:22  fresco
# Twweks to the debug code in access.c.  Tweeks to http.  Messed about with tablesize.c
#
# Revision 1.58  1996/09/02 17:21:40  fresco
# Tweeks to the drawfile code for tables and floating images.  New files for the \!Fresco directory
#
# Revision 1.57  1996/08/29 15:20:18  fresco
# Merged SI_960828
#
# Revision 1.56  1996/08/22 15:55:06  fresco
# Merged SI_960822
#
# Revision 1.55  1996/08/20 16:11:17  fresco
# Merged SI_960820
#
# Revision 1.54  1996/08/20 14:27:05  fresco
# Better table size handling
#
# Revision 1.53  1996/08/16 16:54:42  fresco
# Merged SI_960816
#
# Revision 1.52  1996/08/15 17:50:56  fresco
# A number of minor tweeks listed in the CHANGES file
#
# Revision 1.51  1996/08/14 00:06:41  fresco
# Merged BORRIS_960807 with SI_960813
#
# Revision 1.50  1996/08/07 18:29:55  fresco
# Merged BORRIS_960806 with SI_960807
#
# Revision 1.49  1996/08/06 18:42:30  fresco
# Intermediate and broken checkin
#
# Revision 1.48  1996/08/05 10:31:55  fresco
# Fixes brought in from Si to deal with broken tables code
#
# Revision 1.47  1996/07/29 17:32:16  fresco
# After the big merge... I have no idea what has been changed by Borris and Si
#
# Revision 1.44  1996/07/17 00:34:39  fresco
# Merged SI_960716
#
# Revision 1.43  1996/07/16 15:13:42  fresco
# Intermediate commit
#
# Revision 1.42  1996/07/15 15:55:43  fresco
# Minor changes
#
# Revision 1.41  1996/07/15 10:51:00  fresco
# Update builddate.h
#
# Revision 1.40  1996/07/12 18:23:49  fresco
# <PRE> mode handles tabs better
#
# Revision 1.39  1996/07/11 19:09:12  fresco
# Added couple more files
#
# Revision 1.38  1996/07/11 17:17:46  fresco
# Fixed RISC OS includes to be able to find tcpiplibs
#
# Revision 1.37  1996/07/11 06:13:29  fresco
# Reworked linebreak and markup behaviour, esp. for top of doc.
# <STYLE> reworked. Fixed table column group span bug.
#
# Revision 1.36  1996/07/11 00:26:53  fresco
# Merged at SI_960710_BRANCH
#
# Revision 1.35  1996/07/10 04:44:11  fresco
# Modifications to make compile
#
# Revision 1.34  1996/07/10 00:28:10  fresco
# Minor changes
#
# Revision 1.33  1996/07/09 16:49:33  fresco
# Made RISC OS stuff compile
#
# Revision 1.32  1996/07/09 15:01:34  fresco
# Lots of table fixes. Minor other fixes. Makefile reorganise.
#
# Revision 1.31  1996/07/05 11:28:59  fresco
# A number of changes in different places.
# Nicko has updated the version of RISC_OSLib to have a number of fixes.
# The formter code has had a couple of fixes to deal with floating tables
# and also errors when formatting stops half way though a table.
# The http code has been altered to support https using SSLeay.
# The frontend for !Fresco now supports calling of helper applications
# to load up files that can not be directly displayed.
#
# Revision 1.30  1996/07/04 20:23:43  fresco
# Minor adjustments to DIST target.
#
# Revision 1.29  1996/07/04 11:56:39  fresco
# Adding DIST target
#
# Revision 1.28  1996/07/03 14:16:49  fresco
# Fixed minor bugs resulting from merge
#
# Revision 1.27  1996/07/03 12:28:49  fresco
# Incorporated SI_960702_PATCHED => SI_960702_MERGED
#
#
# Revision 1.26  1996/07/03 09:49:35  fresco
# Auto open of text documents working
#
# Revision 1.25  1996/07/02 18:40:01  fresco
# Lots of changes to fix floating images and tables
#
#
# Revision 1.24.2.1  1996/07/03 12:10:53  fresco
# Merged in Si's changes to SI_960702_BRANCH
#
#
# Revision 1.24  1996/07/02 04:41:54  fresco
# Added FIXME target to find all those @@@@, XXX and FIXME comments
# that get added. Maybe someday we'll even sort them out!
#
# Revision 1.23  1996/06/30 23:42:22  fresco
# Tweaked attrconf
#
# Revision 1.22  1996/06/26 17:47:14  fresco
# Tidied up after changes to stream.c and sorted out configgui.c
#
# Revision 1.21  1996/06/26 15:17:45  fresco
# stream.c and h created and functions to do with streams moved around
#
# Revision 1.20  1996/06/25 06:54:03  fresco
# Reordered directories in TAGS: target to get Fresco definitions before
# stdalone ones. Updated CHANGES file to reflect work done.
#
# Revision 1.19  1996/06/25 01:16:31  fresco
# Few list bugs sorted out
#
# Revision 1.18  1996/06/24 17:19:38  fresco
# Nicko changes.
#
# Revision 1.17  1996/06/20 01:57:27  fresco
# Changes during debugging.
#
# Revision 1.16  1996/06/19 00:04:11  fresco
# Various edits.
#
# Revision 1.15  1996/06/18 00:52:52  fresco
# Moved Fresco run image into !Fresco directory and altered clean target.
#
# Revision 1.14  1996/06/17 06:13:35  fresco
# Started using regression testing on standalone parser. Even found some bugs
# from it already. Now fixed.
#
# Revision 1.13  1996/06/17 02:28:58  fresco
# Added html/regtests/htmlcheck link. Tidied Makefile a bit.
#
# Revision 1.12  1996/06/16 05:01:41  fresco
# Starting to add a test target.
#
# Revision 1.11  1996/06/16 04:25:28  fresco
# Minor adjustments.
#
# Revision 1.10  1996/06/16 03:27:49  borris
# Aborted attempt at xref target.
#
# Revision 1.9  1996/06/15 14:52:11  borris
# Getting stdalone target going.
#
# Revision 1.8  1996/06/14 09:38:32  fresco
# More kludges for poor RiscIx make program.
#
# Revision 1.7  1996/06/14 08:28:36  fresco
# Lose .PHONY target for RiscIx make.
#
# Revision 1.6  1996/06/14 08:22:01  borris
# Added bogus target as RiscIx make seems to fall straight past the all
# target into the clean target!
#
# Revision 1.5  1996/06/14 07:36:26  borris
# Cosmetic tweak.
#
# Revision 1.4  1996/06/14 07:21:56  borris
# New style global makefile. Part way through changes.
#
# Revision 1.3  1996/06/13 20:11:53  fresco
# Minor tweaks.
#
# Revision 1.2  1996/06/13 05:12:41  fresco
# Add link and tags targets to Makefile. You will need to do 'make links'
# after a 'cvs checkout' as CVS does not preserve links. This will be
# automated soon. BUGS file moved under repository tree.
#
#############################################################################
#
# Support targets:
#
# clean		Delete intermediate and final binaries and objects
# depends	Build dependency information
# dist		Directory suitable for distributing
# fixme		Search for comments indicating work needs doing
# fresco	Cross-build RISC OS Fresco executables (FRESCO)
# ffu		Fresco for Unix - (FFU)
# html		Run attrgen.py on attrconf, generating basic SGML tables
# links		Build any necessary soft links
# realclean	Clean and also delete generated source files/state
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

POSS_TARGETS=	br bu BUILDERS BUILDERS_RISCOS BUILDERS_UNIX clean \
		depends DIST FIXME FRESCO FRESCO_RISCOS HTML hr hu \
		HTMLCHECK HTMLCHECK_RISCOS HTMLCHECK_UNIX links    \
		memlib mu \
		MOST MOST_UNIX realclean srclist sr NCFRESCO \
		NCFRESCO_RISCOS TAGS TESTS xref

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
		tablesize.o oobject.o objects.o plugin.o pluginfile.o

BASE_VPATH=	..:../commonsrc:../fresparse:../parser
BASE_INCS=	-I.. -I../commonsrc -I../fresparse -I../parser -I../memlib

#############################################################################

# This set of variables is the basic control over what is built

RISCOS_INCS=	-I../RISC_OSLib -I../include/unix -I../tcpiplibs $(BASE_INCS) -J../clib
#RISCOS_INCS=	-I../RISC_OSLib -I../tcpiplibs $(BASE_INCS) -I../clib
# -Fn *includes* function names (at least for this option set)
#RISCOS_CFLAGS=	-DRISCOS $(RISCOS_INCS) -fa -apcs1 3/26/hardfp -Fn
RISCOS_CFLAGS=	-DRISCOS $(RISCOS_INCS) -fa -Fn
RISCOS_VPATH=	$(BASE_VPATH)
RISCOS_OBJS=	$(BASE_OBJS)
RISCOS_LIBS=	../RISC_OSLib/risc_oslib \
		../clib/o/inetlib \
		../clib/o/stubs

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

FRESCO_INCS=	-I../\!Fresco
# Don't forget the -Fn above
FRESCO_CFLAGS=	-DFRESCO $(FRESCO_INCS) -DDEVELOPMENT=0 -DSGML_REPORTING=0 -DOLD_COMMENTS=1 -DUSE_MARGINS=1 -DMEMLIB=1
FRESCO_VPATH=	../!Fresco
FRESCO_OBJS=	configgui.o fontmenu.o frontend.o guibits.o guidata.o \
		guimenu.o hotlist.o picker.o savedoc.o statusbar.o \
		listfonts2.o rlib_alloc.o \
		$(SGML_OBJS) $(HTML_OBJS) $(COMM_OBJS)
FRESCO_LIBS=    ../webimage/webimage ../memlib/memlib

NCFRESCO_INCS=	-I../\!NCFresco
NCFRESCO_BASECFLAGS= -DSTBWEB $(NCFRESCO_INCS) -DOLD_COMMENTS=1 -DSGML_PC_UNDEF_KEYS -DUSE_MARGINS=1 -DMEMLIB=0

# Production flag set
NCFRESCO_CFLAGS= $(NCFRESCO_BASECFLAGS) -DDEVELOPMENT=0 -DMEMWATCH=0 -DSGML_REPORTING=0 -DNDEBUG

# standard debug set
# NCFRESCO_CFLAGS= $(NCFRESCO_BASECFLAGS) -DDEVELOPMENT=2 -DMEMWATCH=2 -Fn -DSGML_REPORTING=1 -DLINK_DEBUG=0 -DUTIL_DEBUG=0

# MemCheck debug set
# NCFRESCO_CFLAGS= $(NCFRESCO_BASECFLAGS) -DDEVELOPMENT=2 -DMEMWATCH=0 -Fn -DSGML_REPORTING=1 -DLINK_DEBUG=0 -zpc1 -DMemCheck_MEMCHECK -fw -g

NCFRESCO_VPATH=	../!NCFresco
NCFRESCO_OBJS=	clipboard.o flex.o stbfe.o stbhist.o stbhots.o stbkeys.o stbmap.o \
		stbevent.o stbmenu.o stbopen.o stbredraw.o stbstatus.o stbtb.o stbutils.o \
		sutil.o heap.o da.o alarm.o \
		$(SGML_OBJS) $(HTML_OBJS) $(COMM_OBJS)
NCFRESCO_LIBS=	../webimage/webimage1
# NCFRESCO_LIBS=	../webimage/webimage1m ../memcheck/MC
NCFRESCO_LDFLAGS=

BUILDERS_INCS=	-I../bunix -I../RISC_OSLib
BUILDERS_CFLAGS=-DBUILDERS $(BUILDERS_INCS)  -DDEVELOPMENT=2 -DSGML_REPORTING=1
BUILDERS_VPATH=	$(RISCOS_VPATH)
BUILDERS_OBJS=	tables.o memwatch.o myassert.o bldrs.o debug.o rid.o \
		$(SGML_OBJS) $(HTML_OBJS) dump.o tablesize.o backend.o \
		obreak.o obullet.o oimage.o oinput.o oselect.o otable.o \
		otext.o otextarea.o basic2.o stream.o object.o config.o \
		oobject.o objects.o usrtrc.o textplot.o
BUILDERS_LIBS=

HTMLCHECK_INCS=	-I../stdalone
HTMLCHECK_CFLAGS= -DHTMLCHECK $(HTMLCHECK_INCS) -DDEVELOPMENT=1 -DSGML_REPORTING=1
HTMLCHECK_VPATH= ../stdalone
HTMLCHECK_OBJS=	$(SGML_OBJS) htmlcheck.o debug.o memwatch.o myassert.o \
		reports.o htmldefs.o baseprocs.o deliver.o
HTMLCHECK_LIBS=

MOST_INCS=	-I../unix  -I../RISC_OSLib -I../\!Fresco -I../tcpiplibs
MOST_CFLAGS=	-DMOST $(MOST_INCS) -DDEVELOPMENT=2 -DSGML_REPORTING=1 -DUSE_MARGINS=1
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
clean:
		@echo "Cleaning out objects and executables"
		cd ./fresco ; $(RM) *.o *,ff8 *,fd3
		cd ./unix ; $(RM) *.o
		cd ./ncfresco ; $(RM) *.o *,ff8 *,fd3
		cd ./stdalone; $(RM) *.o htmlcheck
		cd ./bunix ; $(RM) *.o builders
		cd ./hriscos ; $(RM) *.o htmlcheck,ff8
		cd ./\!Fresco ; $(RM) *,ff8 *,fd3
		cd ./\!NCFresco ; $(RM) *,ff8 *,fd3
		$(RM) */builddate.h $(PREDEPS)

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

fp:		FRESCO_PC

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

FRESCO_PC:
	@echo Making FRESCO on NT
	@cd fresco
	@nmake /nologo /f Makepc FRESCO_PC \
		LIBS="$(FRESCO_LIBS) $(RISCOS_LIBS)" \
		OBJS="$(FRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(FRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(FRESCO_CFLAGS) $(RISCOS_CFLAGS)"

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

realclean:	clean
		@echo "Cleaning generated files (eg htmldefs.c)"
		$(RM) -r $(ATTREXTRA) srclist RELEASE buildstamp TAGS

# File holding filenames of each source file. These are the files
# that are considered for searching with tools, etc.
srclist:
		@for dir in fresco parser commonsrc ./!Fresco ./!NCFresco \
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

NCFRESCO:		NCFRESCO_RISCOS

NCFRESCO_BUILD_LIBS=	$(RISCOS_BUILD_EXPORT)/Lib/WebImage/webimage1.o \
			$(RISCOS_BUILD_EXPORT)/Lib/CLib/stubs.o \
			$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib/risc_oslib.o \
			$(RISCOS_BUILD_EXPORT)/Lib/InetLib/inetlib.o

NCFRESCO_ROM_LIBS=	$(RISCOS_BUILD_EXPORT)/Lib/WebImage/webimage1.o \
			$(RISCOS_BUILD_EXPORT)/Lib/CLib/romstubs.o \
			$(RISCOS_BUILD_EXPORT)/Lib/InetLib/inetlibzm.o

NCFRESCO_BUILD_CFLAGS=	$(NCFRESCO_CFLAGS) -DRISCOS -fa -Fn \
			-I$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib \
			-I$(RISCOS_BUILD_EXPORT)/Lib/InetLib \
			$(BASE_INCS) -J$(RISCOS_BUILD_EXPORT)/Lib/CLib

NCFRESCO_ROM_CFLAGS=	$(NCFRESCO_CFLAGS) -DRISCOS -fa \
			-I$(RISCOS_BUILD_EXPORT)/Lib/RISC_OSLib \
			-I$(RISCOS_BUILD_EXPORT)/Lib/InetLib \
			$(BASE_INCS) -J$(RISCOS_BUILD_EXPORT)/Lib/CLib

NCFRESCO_ROM_LDFLAGS=	-aof

# This one is for NCFresco in the new style CVS controlled RISC OS build tree
NCFRESCO_BUILD:		$(PREDEPS)
		@echo "Making NCFRESCO for RISCOS in the build tree"
		@cd ./ncfresco; $(MAKE) NCFRESCO_RISCOS \
		LIBS="$(NCFRESCO_BUILD_LIBS)" \
		OBJS="$(NCFRESCO_OBJS) $(RISCOS_OBJS)" \
		VPATH="$(NCFRESCO_VPATH):$(RISCOS_VPATH)" \
		CFLAGS="$(NCFRESCO_BUILD_CFLAGS)" \
		LDFLAGS="$(NCFRESCO_LDFLAGS)"
		$(POSTACTS)

NCFRESCO_BUILD_ROM:	$(PREDEPS)
		@echo "Making NCFRESCO ROM version for RISCOS in the build tree"
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

NCFRESCO_BUILDSTAMP:
		@cp -f ../buildstamp .

# Create a fresh directory holding the items that constitute
# a release. Rebuild from scratch every time to be certain
# that lingering files such as cookies don't end up here.
NCFRESCO_DIST:  NCFRESCO_RISCOS
		@echo "Building the NCFRESCO RELEASE directory"
		-@$(RM) -r $(RELS)
		-@mkdir $(REL)
		@mkdir $(RELS)
		@cp -p \!NCFresco/Resources/UK/\!Boot1,feb $(RELS)/\!Boot,feb
		@cp -p \!NCFresco/Resources/UK/\!Run1,feb $(RELS)/\!Run,feb
		@cp -p \!NCFresco/\!RunImage,ff8 $(RELS)
		@cp -p \!NCFresco/Resources/UK/\!Sprites,ff9 $(RELS)
		@cp -p \!NCFresco/Resources/UK/Res,fae $(RELS)
		@cp -p \!NCFresco/Resources/UK/Sprites,ff9 $(RELS)
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

NCFRESCO_RELEASE: NCFRESCO_DIST
		@echo "Building NCFRESCO release pack"
		@cp -pr $(RELS) ../release/browser
		@./fixlicense ../release/browser/\!NCFresco/\!RunImage,ff8 1000000 "Acorn Network Computers Dev"
		@cp -pr ../release/stbmods/* ../release/browser/\!NCFresco
		@cd ../release ; tar cf ncfresco.tar browser
		@gzip -f ../release/ncfresco.tar
		@uuencode ../release/ncfresco.tar.gz ../release/ncfresco.tar.gz > ../release/ncfresco.tar.gz.uue
		@echo "Done"


NCFRESCO_TEST_RUNIMAGE: NCFRESCO_RISCOS
		@echo "Updating NCFresco installation on bungle"
		@rcp -r \!NCFresco/\!RunImage,ff8 'bungle:/export/home/ncd/default/boot/WWW/Dev/\!NCFresco'
		@echo "Done"
		@echo ""

NCFRESCO_TEST_RUNIMAGE_STAMP: NCFRESCO_RISCOS
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

# eof fresco/Makefile

