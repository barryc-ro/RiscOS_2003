# Makefile for Browser
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date		Name	Description
# ----		----	-----------
# 1997-06-24	BAL	Created
# 1997-07-18	BAL	Tidied up for CVS.  Hooks are in place for a debug
#			build, but I'm not sure exactly what's required for
#			one so it'll probably need tweaking.
# 1997-07-22	ADH	Duly tweaked. Extras added to cope with test Browser
#			(bouncy blue arrow), Surfboard browser, Phoenix
#			(acorn on fire), and Intertalk browser (spinning
#			acorn)  (spinning acorn) variants.
#			See ReadMe for details on which of these use
#			different object directories and which just have
#			different compile time options. Updated the clean
#			and clean_all rules. Took DFLAGS out of CFLAGS to
#			make changing options more flexible. Note that the
#			same .s directory is still currently used for all
#			builds. Did lots of other stuff, too.


# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Browse
APP		= !Browse
MODULE		= rm.!Browse
ROM_MODULE	= aof.Browse
LDIR		= Resources.${LOCALE}.${SYSTEM}


# ------------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR		= <resource$dir>.Resources2.${COMPONENT}
RESAPP		= <resource$dir>.Apps.${APP}


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe

AFLAGS		= ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS		= ${THROWBACK} -depend !Depend -ffa ${INCLUDES} -wp
CPFLAGS		= ~cfr~v
SQFLAGS		= -f
WFLAGS		= ~cf~vr

# Flags for the standard Desktop build (plus debug), the Surfboard ROM
# build (plus debug), the Surfboard RAM build (plus debug), and the
# Intertalk build (plus debug).

DFLAGS		= -DCOMPAT_INET4 -DSINGLE_USER -DALIAS_URLS -DHACK_LINEONE
DDFLAGS		= ${DFLAGS} -DSTRICT_PARSER -DTRACE
DZFLAGS		= -DCOMPAT_INET4 -DSINGLE_USER -DHIDE_CGI -DANTI_TWITTER
DDZFLAGS	= ${DZFLAGS} -DSTRICT_PARSER -DTRACE
D_SURFFLAGS	= -DCOMPAT_INET4 -DSINGLE_USER -DHIDE_CGI -DANTI_TWITTER -DHACK_LINEONE
DD_SURFFLAGS	= ${D_SURFFLAGS} -DSTRICT_PARSER -DTRACE
D_ITALKFLAGS	= -DCOMPAT_INET4 -DALIAS_URLS
DD_ITALKFLAGS	= ${D_ITALKFLAGS} -DSTRICT_PARSER -DTRACE


# ------------------------------------------------------------------------------
# Libraries
#

CLIB		= CLib:o.stubs
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap
HTMLLIB		= HTMLLib:o.HTMLLib
IMAGELIB	= ImageLib:o.ImageLib
PNGLIB		= ImageLib:o.libpng-lib
PNGLIBZ		= ImageLib:o.libpng-lzm
ZLIB		= ImageLib:o.zlib_lib
URILIB		= Libraries.URILib.o.URILib
FLEXLIB		= tbox:o.flexlib
EVENTLIB	= tbox:o.eventlib
TOOLBOXLIB	= tbox:o.toolboxlib
WIMPLIB		= tbox:o.wimplib
INETLIB		= TCPIPLibs:o.inetlib
UNIXLIB		= TCPIPLibs:o.unixlib

LIBSM = \
 ${HTMLLIB}zm \
 ${URILIB} \
 ${FLEXLIB} \
 ${EVENTLIB}m \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${INETLIB}zm \
 ${UNIXLIB}zm \
 ${IMAGELIB}zm \
 ${PNGLIBZ} \
 ${ZLIB}zm

LIBS = \
 ${CLIB} \
 ${HTMLLib} \
 ${URILib} \
 ${FLEXLIB} \
 ${EVENTLIB} \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${INETLIB} \
 ${UNIXLIB} \
 ${IMAGELIB} \
 ${PNGLIB} \
 ${ZLIB}

LIBSD = \
 ${CLIB} \
 ${HTMLLib} \
 ${URILib} \
 ${FLEXLIB} \
 ${EVENTLIB} \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${INETLIB} \
 ${UNIXLIB} \
 ${IMAGELIB} \
 ${PNGLIB} \
 ${ZLIB}


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -Itbox:,C:,HTMLLib:,ImageLib:,Libraries.URILib.,TCPIPLibs:

FILES = \
 ${LDIR}.!Boot \
 ${LDIR}.!Run \
 ${LDIR}.!Sprites \
 ${LDIR}.!Sprites22 \
 ${LDIR}.Messages \
 ${LDIR}.Res \
 ${TARGET}

# Include these in the ROM module
RESFILES = \
 ${LDIR}.Sprites \
 ${LDIR}.!Sprites \
 ${LDIR}.!Sprites22

OBJS = \
 o.Authorise	o.Browser	o.Fetch		o.FetchPage	\
 o.FontManage	o.Forms		o.Frames	o.FromROSLib	\
 o.Global	o.Handlers	o.History	o.Images	\
 o.Main		o.Memory	o.Menus		o.Mouse		\
 o.Printing	o.Redraw	o.Reformat	o.ROSLib	\
 o.Save		o.Tables	o.TokenUtils	o.Toolbars	\
 o.URLutils	o.Utils		o.Windows	o.JavaScript	\
 o.LineOne	o.Meta

OBJSZ = \
 oz.Authorise	oz.Browser	oz.Fetch	oz.FetchPage	\
 oz.FontManage	oz.Forms	oz.Frames	oz.FromROSLib	\
 oz.Global	oz.Handlers	oz.History	oz.Images	\
 oz.Main	oz.Memory	oz.Menus	oz.Mouse	\
 oz.Printing	oz.Redraw	oz.Reformat	o.ROSLib	\
 oz.Save	oz.Tables	oz.TokenUtils	oz.Toolbars	\
 oz.URLutils	oz.Utils	oz.Windows	oz.JavaScript	\
 oz.LineOne	oz.Meta

OBJSD = \
 od.Authorise	od.Browser	od.Fetch	od.FetchPage	\
 od.FontManage	od.Forms	od.Frames	od.FromROSLib	\
 od.Global	od.Handlers	od.History	od.Images	\
 od.Main	od.Memory	od.Menus	od.Mouse	\
 od.Printing	od.Redraw	od.Reformat	o.ROSLib	\
 od.Save	od.svcprint	od.Tables	od.TokenUtils	\
 od.Toolbars	od.Trace	od.URLutils	od.Utils	\
 od.Windows	od.JavaScript	od.LineOne	od.Meta

OBJSDZ = \
 odz.Authorise	odz.Browser	odz.Fetch	odz.FetchPage	\
 odz.FontManage	odz.Forms	odz.Frames	odz.FromROSLib	\
 odz.Global	odz.Handlers	odz.History	odz.Images	\
 odz.Main	odz.Memory	odz.Menus	odz.Mouse	\
 odz.Printing	odz.Redraw	odz.Reformat	o.ROSLib	\
 odz.Save	odz.svcprint	odz.Tables	odz.TokenUtils	\
 odz.Toolbars	odz.Trace	odz.URLutils	odz.Utils	\
 odz.Windows	odz.JavaScript	odz.LineOne	odz.Meta

OBJS_SURF = \
 o_surf.Authorise	o_surf.Browser		o_surf.Fetch		o_surf.FetchPage	\
 o_surf.FontManage	o_surf.Forms		o_surf.Frames		o_surf.FromROSLib	\
 o_surf.Global		o_surf.Handlers		o_surf.History		o_surf.Images		\
 o_surf.Main		o_surf.Memory		o_surf.Menus		o_surf.Mouse		\
 o_surf.Printing	o_surf.Redraw		o_surf.Reformat		o.ROSLib		\
 o_surf.Save		o_surf.Tables		o_surf.TokenUtils	o_surf.Toolbars		\
 o_surf.URLutils	o_surf.Utils		o_surf.Windows		o_surf.JavaScript	\
 o_surf.LineOne		o_surf.Meta

OBJSD_SURF = \
 od_surf.Authorise	od_surf.Browser		od_surf.Fetch		od_surf.FetchPage	\
 od_surf.FontManage	od_surf.Forms		od_surf.Frames		od_surf.FromROSLib	\
 od_surf.Global		od_surf.Handlers	od_surf.History		od_surf.Images		\
 od_surf.Main		od_surf.Memory		od_surf.Menus		od_surf.Mouse		\
 od_surf.Printing	od_surf.Redraw		od_surf.Reformat	o.ROSLib		\
 od_surf.Save		od_surf.svcprint	od_surf.Tables		od_surf.TokenUtils	\
 od_surf.Toolbars	od_surf.Trace		od_surf.URLutils	od_surf.Utils		\
 od_surf.Windows	od_surf.JavaScript	od_surf.LineOne		od_surf.Meta

OBJS_ITALK = \
 o_italk.Authorise	o_italk.Browser		o_italk.Fetch		\
 o_italk.FetchPage	o_italk.FontManage	o_italk.Forms		\
 o_italk.Frames		o_italk.FromROSLib	o_italk.Global		\
 o_italk.Handlers	o_italk.History		o_italk.Images		\
 o_italk.Main		o_italk.Memory		o_italk.Menus		\
 o_italk.Mouse		o_italk.Printing	o_italk.Redraw		\
 o_italk.Reformat	o.ROSLib		o_italk.Save		\
 o_italk.Tables		o_italk.TokenUtils	o_italk.Toolbars	\
 o_italk.URLutils	o_italk.Utils		o_italk.Windows		\
 o_italk.JavaScript	o_italk.LineOne		o_italk.Surf

OBJSD_ITALK = \
 od_italk.Authorise	od_italk.Browser	od_italk.Fetch		\
 od_italk.FetchPage	od_italk.FontManage	od_italk.Forms		\
 od_italk.Frames	od_italk.FromROSLib	od_italk.Global		\
 od_italk.Handlers	od_italk.History	od_italk.Images		\
 od_italk.Main		od_italk.Memory		od_italk.Menus		\
 od_italk.Mouse		od_italk.Printing	od_italk.Redraw		\
 od_italk.Reformat	o.ROSLib		od_italk.Save		\
 od_italk.svcprint	od_italk.Tables		od_italk.TokenUtils	\
 od_italk.Toolbars	od_italk.Trace		od_italk.URLutils	\
 od_italk.Utils		od_italk.Windows	od_italk.JavaScript	\
 od_italk.LineOne	od_italk.Surf


# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .o_surf .od_surf .o_italk .od_italk .s .c
.c.o:;		${CC} ${CFLAGS} ${DFLAGS}        -c -o $@ $<
.c.oz:;		${CC} ${CFLAGS} ${DZFLAGS}       -c -zM -zps1 -DROM -o $@ $<
.c.od:;		${CC} ${CFLAGS} ${DDFLAGS}       -c -g -o $@ $<
.c.odz:;	${CC} ${CFLAGS} ${DDZFLAGS}      -c -zM -zps1 -DROM -o $@ $<
.c.o_surf:;	${CC} ${CFLAGS} ${D_SURFFLAGS}   -c -o $@ $<
.c.od_surf:;	${CC} ${CFLAGS} ${DD_SURFFLAGS}  -c -g -o $@ $<
.c.o_italk:;	${CC} ${CFLAGS} ${D_ITALKFLAGS}  -c -o $@ $<
.c.od_italk:;	${CC} ${CFLAGS} ${DD_ITALKFLAGS} -c -g -o $@ $<
.s.o:;		${AS} ${AFLAGS} $< $@


# ------------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
	@echo ${COMPONENT}: Application built (Disc)

rom: ${ROM_MODULE}
	@echo ${COMPONENT}: Module built (ROM)

install_common: ${FILES}
	${CP} ${LDIR}.!Boot		${INSTDIR}.!Boot	${CPFLAGS}
	${CP} ${TARGET}			${INSTDIR}.!RunImage	${CPFLAGS}
	${CP} ${LDIR}.!Sprites		${INSTDIR}.!Sprites	${CPFLAGS}
	${CP} ${LDIR}.!Sprites22	${INSTDIR}.!Sprites22	${CPFLAGS}
	${CP} ${LDIR}.Choices		${INSTDIR}.Choices	${CPFLAGS}
	${CP} ${LDIR}.Messages		${INSTDIR}.Messages	${CPFLAGS}
	${CP} ${LDIR}.Res		${INSTDIR}.Res		${CPFLAGS}
	${CP} ${LDIR}.Sprites		${INSTDIR}.Sprites	${CPFLAGS}
	${MKDIR} ${INSTDIR}.User
	${CP} ${LDIR}.User.Hotlist	${INSTDIR}.User.Hotlist	${CPFLAGS}

install: install_common
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}
	-Access ${INSTDIR}.!Boot	lr/r
	-Access ${INSTDIR}.!Run		lr/r
	-Access ${INSTDIR}.!RunImage	lr/r
	-Access ${INSTDIR}.!Sprites	lr/r
	-Access ${INSTDIR}.!Sprites22	lr/r
	-Access ${INSTDIR}.Choices	wr/r
	-Access ${INSTDIR}.Messages	lr/r
	-Access ${INSTDIR}.Res		lr/r
	-Access ${INSTDIR}.Sprites	lr/r
	-Access ${INSTDIR}.User.Hotlist	wr/r
	@echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
	${CP} ${LDIR}.!RunD		${INSTDIR}.!Run		${CPFLAGS}
	@echo ${COMPONENT}: Debug application installed ${INSTDIR}

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: Module installed (ROM)

resources:
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESDIR}.User
	${CP} ${LDIR}.ROM.!Boot		${RESAPP}.!Boot		${CPFLAGS}
	${CP} ${LDIR}.ROM.!Run		${RESAPP}.!Run		${CPFLAGS}
	${CP} ${LDIR}.!Sprites		${RESDIR}.!Sprites	${CPFLAGS}
	${CP} ${LDIR}.!Sprites22	${RESDIR}.!Sprites22	${CPFLAGS}
	${CP} ${LDIR}.Choices		${RESDIR}.Choices	${CPFLAGS}
	${CP} ${LDIR}.Messages		${RESDIR}.Messages	${CPFLAGS}
	${CP} ${LDIR}.Res		${RESDIR}.Res		${CPFLAGS}
	${CP} ${LDIR}.Sprites		${RESDIR}.Sprites	${CPFLAGS}
	${CP} ${LDIR}.User.Hotlist	${RESDIR}.User.Hotlist	${CPFLAGS}
	@echo ${COMPONENT}: Resource files copied to Messages module

clean:
	@echo starting
	${WIPE}	abs		${WFLAGS}
	${WIPE}	aof		${WFLAGS}
	${WIPE}	linked		${WFLAGS}
	${WIPE}	map		${WFLAGS}
	${WIPE}	o		${WFLAGS}
	${WIPE}	od		${WFLAGS}
	${WIPE}	oz		${WFLAGS}
	${WIPE}	odz		${WFLAGS}
	${WIPE}	o_surf		${WFLAGS}
	${WIPE}	od_surf		${WFLAGS}
	${WIPE}	o_italk		${WFLAGS}
	${WIPE}	od_italk	${WFLAGS}
	${WIPE}	rm		${WFLAGS}
	${RM}	s.AppName
	${RM}	s.ModuleWrap
	${RM}	o.dirs
	@echo ${COMPONENT}: Cleaned

o.dirs:
	@${MKDIR} abs
	@${MKDIR} aof
	@${MKDIR} linked
	@${MKDIR} map
	@${MKDIR} o
	@${MKDIR} od
	@${MKDIR} oz
	@${MKDIR} odz
	@${MKDIR} o_surf
	@${MKDIR} od_surf
	@${MKDIR} o_italk
	@${MKDIR} od_italk
	@${MKDIR} rm
	@${MKDIR} Targets
	@${MKDIR} Targets.Browse
	@${MKDIR} Targets.Browse.!Browse
	@${MKDIR} Targets.Intertalk
	@${MKDIR} Targets.Intertalk.!Browse
	@${MKDIR} Targets.Surfboard
	@${MKDIR} Targets.Surfboard.!Browse
	@${MKDIR} Targets.Desktop
	@${MKDIR} Targets.Desktop.!Browse
	@${MKDIR} Targets.Phoenix
	@${MKDIR} Targets.Phoenix.!Phoenix
	create o.dirs
	stamp  o.dirs


# ------------------------------------------------------------------------------
# Development rules
#

linkmap: ${OBJS} ${LIBS}
	${LD} -map -o null:x ${OBJS} ${LIBS} > map.linked

map:
	${LD} -map -bin -base 0 -o null: ${OBJS} ${LIBS} > map.base0

clean_all: clean
	${WIPE}	Targets		${WFLAGS}
	@echo ${COMPONENT}: Cleaned all


# ------------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: ${OBJS} ${LIBS} o.dirs
	${LD} -o $@ ${OBJS} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_D: ${OBJSD} ${LIBSD} o.dirs
	${LD} -debug -o $@ ${OBJSD} ${LIBSD}

abs.!RI_Surf: ${OBJS_SURF} ${LIBS} o.dirs
	${LD} -o $@ ${OBJS_SURF} ${LIBS}

abs.!RI_DSurf: ${OBJSD_SURF} ${LIBSD} o.dirs
	${LD} -debug -o $@ ${OBJSD_SURF} ${LIBSD}

abs.!RI_ITalk: ${OBJS_ITALK} ${LIBS} o.dirs
	${LD} -o $@ ${OBJS_ITALK} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_DITalk: ${OBJSD_ITALK} ${LIBSD} o.dirs
	${LD} -debug -o $@ ${OBJSD_ITALK} ${LIBSD}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB} o.dirs
	${LD} -o $@ -module oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMCSTUBS} ${LIBSM} o.dirs
	${LD} -o $@ -aof oz.ModuleWrap ${OBJSZ} ${LIBSM} ${ROMCSTUBS}

rm.!BrowseD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
	${LD} -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM}

# final link for ROM Image (using given base address)
rom_link:
	${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} \
		${ROM_MODULE} ${ABSSYM} -map > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

oz.ModuleWrap: s.ModuleWrap s.AppName ${RESFILES}
	${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
	${CP} ${WRAPPER} $@ ${CPFLAGS}

s.AppName: ${LDIR}.Messages
	awk -f awk.AppName ${LDIR}.Messages > $@


# Dynamic dependencies:
