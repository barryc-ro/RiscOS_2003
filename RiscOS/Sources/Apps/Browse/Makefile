# Makefile for Browser
# ====================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
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
# 1997-08-08	ADH	Got sick of having to scroll the task window right
#			all the time to see what was compiling, so played
#			around with the SUFFIXES (rule patterns) section
#			to make the run-time output more readable.
# 1997-08-17	ADH	Adding object files was a nightmare, so the objects
#			list has been unrolled to one per line. This makes
#			the MakeFile very tall, so a consistent comment
#			syntax has been ensured to allow folding text
#			editors to fold the text sensibly - start on
#			NL, hash, NL; end on hash, space, hyphen.
# 1997-08-28	ADH	Added Argonet build. Not much to do in the Makefile
#			but needs the appropriate TaskObey files.
# 1998-04-30	ADH	Added JavaScript build support, including BASEFLAGS
#			in the compile-time options.
# 1998-07-02	ADH	Prior to 2.06 intermediate checkin, took out
#			-DJAVASCRIPT from DFLAGS and added comment about
#			needing to increase !Run file WimpSlot values.
# 1998-07-07	ADH	Tried to make JavaScript builds easier; if the
#			component to make is installj rather than install or
#			installd, then !Run, !Boot and About are sourced
#			from !RunJ, !BootJ and AboutJ. Define JSUFFIX on the
#			command line to be -DJAVASCRIPT if doing this, too.
#			The TARGET binary name should be suffixed with '_J',
#			since even though the exported copy in "Targets"
#			overwrites the current exported Surfboard, Desktop
#			or whatever build, you still probably want to keep
#			the previous symbols table.
# 1998-07-31	ADH	The above is insufficient; a link is still tried
#			with MemLib, NSPRLib and JSLib even though they
#			may ultimately not be included. So these must be
#			built at all times. In addition, note that
#			USE_MALLOC_REPLACEMENT is now defined in the
#			BASEFLAGS (i.e. defined for every build).
# 1998-09-10	ADH	Cleared up the whole MemLib issue through NSPRLib.
#			JS_USING_MEMLIB local option in Main.c controls
#			whether or not MemLib is ever used. The NSPRLib
#			based redefinition of malloc() etc. on a global
#			basis by adding "-DUSE_NSPRLIB_ALLOC_REGISTRATION"
#			has NOT been done by default here. Added also
#			lines to wipe CVS control files from exported
#			"About" directories (WFLAGS used at last!).
# 1998-10-08	ADH	ResJ now used for Res file of JavaScript builds;
#			needed as the Choices dialogue now has JS
#			specific regions. install_common now installs
#			anything in 'User' dir and wipes CVS control files
#			from the exported copy. 'resources' rule updated
#			similarly for both 'User' and 'About' directories.

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Browse
APP		= !Browse
MODULE		= rm.!Browse
ROM_MODULE	= aof.Browse
LDIR		= Resources.${LOCALE}.${SYSTEM}

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR		= <resource$dir>.Resources2.${COMPONENT}
RESAPP		= <resource$dir>.Apps.${APP}


# -----------------------------------------------------------------------------
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


# -----------------------------------------------------------------------------
# Flags
#

# BASEFLAGS holds common compile time options that all builds will use.
# Others are for the standard Desktop build (plus debug), the Surfboard
# ROM build (plus debug), the Surfboard RAM build (plus debug), and the
# Intertalk build (plus debug).

BASEFLAGS	= -DCOMPAT_INET4 -DRISCOS -DNSPR20

# Examples of other possibly useful BASEFLAGS settings

# BASEFLAGS	= -DCOMPAT_INET4 -DRISCOS -DNSPR20 -DUSE_NSPRLIB_ALLOC_REGISTRATIONS
# BASEFLAGS	= -DCOMPAT_INET4 -DRISCOS -DNSPR20 -DUNIFONT

# Additional build-dependent flags. Note JSUFFIX *must* be defined on the
# command line.

DFLAGS		= ${BASEFLAGS} ${JSUFFIX} -DSINGLE_USER
DDFLAGS		= ${DFLAGS} -fn -DSTRICT_PARSER -DTRACE
DZFLAGS		= ${BASEFLAGS} -DSINGLE_USER -DHIDE_CGI -DANTI_TWITTER
DDZFLAGS	= ${DZFLAGS} -fn -DSTRICT_PARSER -DTRACE
D_SURFFLAGS	= ${BASEFLAGS} -DSINGLE_USER -DANTI_TWITTER -DREMOTE_HOTLIST -DHIDE_CGI -DALIAS_URLS -DHACK_LINEONE
DD_SURFFLAGS	= ${D_SURFFLAGS} -fn -DSTRICT_PARSER -DTRACE
D_ITALKFLAGS	= ${BASEFLAGS} -DALIAS_URLS
DD_ITALKFLAGS	= ${D_ITALKFLAGS} -fn -DSTRICT_PARSER -DTRACE


# -----------------------------------------------------------------------------
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
JPEGLIB		= ImageLib:o.libjpeg
JPEGLIBZ	= ImageLib:o.libjpeg-zm
ZLIB		= C:zlib.o.zlib
URILIB		= Libraries.URILib.o.URILib
FLEXLIB		= tbox:o.flexlib
EVENTLIB	= tbox:o.eventlib
TOOLBOXLIB	= tbox:o.toolboxlib
WIMPLIB		= tbox:o.wimplib
RENDERLIB	= tbox:o.renderlib
INETLIB		= TCPIPLibs:o.inetlib
UNICODELIB	= C:Unicode.o.ucodelib
MEMLIB		= C:MemLib.o.MemLib
NSPRLIB		= C:NSPRLib.o.NSPRLib
JSLIB		= C:JSLib.o.JSLib

LIBSM = \
 ${HTMLLIB}zm \
 ${URILIB} \
 ${FLEXLIB}zm \
 ${EVENTLIB}m \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${INETLIB}zm \
 ${IMAGELIB}zm \
 ${PNGLIBZ} \
 ${JPEGLIBZ} \
 ${ZLIB}zm \
 ${UNICODELIB}z \
 ${JSLIB}zm \
 ${NSPRLIB}zm \
 ${MEMLIB}z \
 ${RENDERLIB}

LIBS = \
 ${CLIB} \
 ${HTMLLib} \
 ${URILib} \
 ${FLEXLIB} \
 ${EVENTLIB} \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${INETLIB} \
 ${IMAGELIB} \
 ${PNGLIB} \
 ${JPEGLIB} \
 ${ZLIB} \
 ${UNICODELIB} \
 ${JSLIB} \
 ${NSPRLIB} \
 ${MEMLIB} \
 ${RENDERLIB}

LIBSD = \
 ${CLIB} \
 ${HTMLLib} \
 ${URILib} \
 ${FLEXLIB} \
 ${EVENTLIB} \
 ${TOOLBOXLIB} \
 ${WIMPLIB} \
 ${INETLIB} \
 ${IMAGELIB} \
 ${PNGLIB} \
 ${JPEGLIB} \
 ${ZLIB} \
 ${UNICODELIB} \
 ${JSLIB} \
 ${NSPRLIB} \
 ${MEMLIB} \
 ${RENDERLIB}


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -Itbox:,C:,HTMLLib:,ImageLib:,Libraries.URILib.,TCPIPLibs:,C:NSPRLib.,C:JSLib.

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


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
 o.About		\
 o.Authorise		\
 o.Browser		\
 o.Choices		\
 o.Cookies		\
 o.CSIM			\
 o.DragBox		\
 o.Encoding		\
 o.Fetch		\
 o.FetchHTML		\
 o.FetchPage		\
 o.Find			\
 o.FontManage		\
 o.Forms		\
 o.Frames		\
 o.FromROSLib		\
 o.Global		\
 o.Handlers		\
 o.History		\
 o.Hotlist		\
 o.Images		\
 o.ImgHistory		\
 o.ItemInfo		\
 o.JavaScript		\
 o.JSconsts		\
 o.JSdocument		\
 o.JSnavigtr		\
 o.JSscreen		\
 o.JSURL		\
 o.JSutils		\
 o.JSwindow		\
 o.LineOne		\
 o.Main			\
 o.Markers		\
 o.Memory		\
 o.Menus		\
 o.Meta			\
 o.MimeMap		\
 o.Mouse		\
 o.Multiuser		\
 o.Object		\
 o.OpenURL		\
 o.PlugIn		\
 o.Printing		\
 o.PrintStyle		\
 o.Protocols		\
 o.Redraw		\
 o.Reformat		\
 o.RMA			\
 o.Save			\
 o.SaveDraw		\
 o.SaveFile		\
 o.SaveObject		\
 o.SaveSHelp		\
 o.SaveText		\
 o.SetPBoxes		\
 o.Tables		\
 o.TokenUtils		\
 o.Toolbars		\
 o.Unifont		\
 o.URLstat		\
 o.URLutils		\
 o.URLveneer		\
 o.Utils		\
 o.Windows		\
 o.ROSLib

OBJSZ = \
 oz.About		\
 oz.Authorise		\
 oz.Browser		\
 oz.Choices		\
 oz.Cookies		\
 oz.CSIM		\
 oz.DragBox		\
 oz.Encoding		\
 oz.Fetch		\
 oz.FetchHTML		\
 oz.FetchPage		\
 oz.Find		\
 oz.FontManage		\
 oz.Forms		\
 oz.Frames		\
 oz.FromROSLib		\
 oz.Global		\
 oz.Handlers		\
 oz.History		\
 oz.Hotlist		\
 oz.Images		\
 oz.ImgHistory		\
 oz.ItemInfo		\
 oz.JavaScript		\
 oz.JSconsts		\
 oz.JSdocument		\
 oz.JSnavigtr		\
 oz.JSscreen		\
 oz.JSURL		\
 oz.JSutils		\
 oz.JSwindow		\
 oz.LineOne		\
 oz.Main		\
 oz.Markers		\
 oz.Memory		\
 oz.Menus		\
 oz.Meta		\
 oz.MimeMap		\
 oz.Mouse		\
 oz.Multiuser		\
 oz.Object		\
 oz.OpenURL		\
 oz.PlugIn		\
 oz.Printing		\
 oz.PrintStyle		\
 oz.Protocols		\
 oz.Redraw		\
 oz.Reformat		\
 oz.RMA			\
 oz.Save		\
 oz.SaveDraw		\
 oz.SaveFile		\
 oz.SaveObject		\
 oz.SaveSHelp		\
 oz.SaveText		\
 oz.SetPBoxes		\
 oz.Tables		\
 oz.TokenUtils		\
 oz.Toolbars		\
 oz.Unifont		\
 oz.URLstat		\
 oz.URLutils		\
 oz.URLveneer		\
 oz.Utils		\
 oz.Windows		\
 o.ROSLib

OBJSD = \
 od.About		\
 od.Authorise		\
 od.Browser		\
 od.Choices		\
 od.Cookies		\
 od.CSIM		\
 od.DragBox		\
 od.Encoding		\
 od.Fetch		\
 od.FetchHTML		\
 od.FetchPage		\
 od.Find		\
 od.FontManage		\
 od.Forms		\
 od.Frames		\
 od.FromROSLib		\
 od.Global		\
 od.Handlers		\
 od.History		\
 od.Hotlist		\
 od.Images		\
 od.ImgHistory		\
 od.ItemInfo		\
 od.JavaScript		\
 od.JSconsts		\
 od.JSdocument		\
 od.JSnavigtr		\
 od.JSscreen		\
 od.JSURL		\
 od.JSutils		\
 od.JSwindow		\
 od.LineOne		\
 od.Main		\
 od.Markers		\
 od.Memory		\
 od.Menus		\
 od.Meta		\
 od.MimeMap		\
 od.Mouse		\
 od.Multiuser		\
 od.Object		\
 od.OpenURL		\
 od.PlugIn		\
 od.Printing		\
 od.PrintStyle		\
 od.Protocols		\
 od.Redraw		\
 od.Reformat		\
 od.RMA			\
 od.Save		\
 od.SaveDraw		\
 od.SaveFile		\
 od.SaveObject		\
 od.SaveSHelp		\
 od.SaveText		\
 od.SetPBoxes		\
 od.svcprint		\
 od.Tables		\
 od.TokenUtils		\
 od.Toolbars		\
 od.Trace		\
 od.Unifont		\
 od.URLstat		\
 od.URLutils		\
 od.URLveneer		\
 od.Utils		\
 od.Windows		\
 o.ROSLib

OBJSDZ = \
 odz.About		\
 odz.Authorise		\
 odz.Browser		\
 odz.Choices		\
 odz.Cookies		\
 odz.CSIM		\
 odz.DragBox		\
 odz.Encoding		\
 odz.Fetch		\
 odz.FetchHTML		\
 odz.FetchPage		\
 odz.Find		\
 odz.FontManage		\
 odz.Forms		\
 odz.Frames		\
 odz.FromROSLib		\
 odz.Global		\
 odz.Handlers		\
 odz.History		\
 odz.Hotlist		\
 odz.Images		\
 odz.ImgHistory		\
 odz.ItemInfo		\
 odz.JavaScript		\
 odz.JSconsts		\
 odz.JSdocument		\
 odz.JSnavigtr		\
 odz.JSscreen		\
 odz.JSURL		\
 odz.JSutils		\
 odz.JSwindow		\
 odz.LineOne		\
 odz.Main		\
 odz.Markers		\
 odz.Memory		\
 odz.Menus		\
 odz.Meta		\
 odz.MimeMap		\
 odz.Mouse		\
 odz.Multiuser		\
 odz.Object		\
 odz.OpenURL		\
 odz.PlugIn		\
 odz.Printing		\
 odz.PrintStyle		\
 odz.Protocols		\
 odz.Redraw		\
 odz.Reformat		\
 odz.RMA		\
 odz.Save		\
 odz.SaveDraw		\
 odz.SaveFile		\
 odz.SaveObject		\
 odz.SaveSHelp		\
 odz.SaveText		\
 odz.SetPBoxes		\
 odz.svcprint		\
 odz.Tables		\
 odz.TokenUtils		\
 odz.Toolbars		\
 odz.Trace		\
 odz.Unifont		\
 odz.URLstat		\
 odz.URLutils		\
 odz.URLveneer		\
 odz.Utils		\
 odz.Windows		\
 o.ROSLib

OBJS_SURF = \
 o_surf.About		\
 o_surf.Authorise	\
 o_surf.Browser		\
 o_surf.Choices		\
 o_surf.Cookies		\
 o_surf.CSIM		\
 o_surf.DragBox		\
 o_surf.Encoding	\
 o_surf.Fetch		\
 o_surf.FetchHTML	\
 o_surf.FetchPage	\
 o_surf.Find		\
 o_surf.FontManage	\
 o_surf.Forms		\
 o_surf.Frames		\
 o_surf.FromROSLib	\
 o_surf.Global		\
 o_surf.Handlers	\
 o_surf.History		\
 o_surf.Images		\
 o_surf.ImgHistory	\
 o_surf.ItemInfo	\
 o_surf.JavaScript	\
 o_surf.JSconsts	\
 o_surf.JSdocument	\
 o_surf.JSnavigtr	\
 o_surf.JSscreen	\
 o_surf.JSURL		\
 o_surf.JSutils		\
 o_surf.JSwindow	\
 o_surf.LineOne		\
 o_surf.Main		\
 o_surf.Markers		\
 o_surf.Memory		\
 o_surf.Menus		\
 o_surf.Meta		\
 o_surf.MimeMap		\
 o_surf.Mouse		\
 o_surf.Multiuser	\
 o_surf.Object		\
 o_surf.OpenURL		\
 o_surf.PlugIn		\
 o_surf.Printing	\
 o_surf.PrintStyle	\
 o_surf.Protocols	\
 o_surf.Redraw		\
 o_surf.Reformat	\
 o_surf.RMA		\
 o_surf.Save		\
 o_surf.SaveDraw	\
 o_surf.SaveFile	\
 o_surf.SaveObject	\
 o_surf.SaveSHelp	\
 o_surf.SaveText	\
 o_surf.SetPBoxes	\
 o_surf.Tables		\
 o_surf.TokenUtils	\
 o_surf.Toolbars	\
 o_surf.Unifont		\
 o_surf.URLstat		\
 o_surf.URLutils	\
 o_surf.URLveneer	\
 o_surf.Utils		\
 o_surf.Windows		\
 o.ROSLib

OBJSD_SURF = \
 od_surf.About		\
 od_surf.Authorise	\
 od_surf.Browser	\
 od_surf.Choices	\
 od_surf.Cookies	\
 od_surf.CSIM		\
 od_surf.DragBox	\
 od_surf.Encoding	\
 od_surf.Fetch		\
 od_surf.FetchHTML	\
 od_surf.FetchPage	\
 od_surf.Find		\
 od_surf.FontManage	\
 od_surf.Forms		\
 od_surf.Frames		\
 od_surf.FromROSLib	\
 od_surf.Global		\
 od_surf.Handlers	\
 od_surf.History	\
 od_surf.Images		\
 od_surf.ImgHistory	\
 od_surf.ItemInfo	\
 od_surf.JavaScript	\
 od_surf.JSconsts	\
 od_surf.JSdocument	\
 od_surf.JSnavigtr	\
 od_surf.JSscreen	\
 od_surf.JSURL		\
 od_surf.JSutils	\
 od_surf.JSwindow	\
 od_surf.LineOne	\
 od_surf.Main		\
 od_surf.Markers	\
 od_surf.Memory		\
 od_surf.Menus		\
 od_surf.Meta		\
 od_surf.MimeMap	\
 od_surf.Mouse		\
 od_surf.Multiuser	\
 od_surf.Object		\
 od_surf.OpenURL	\
 od_surf.PlugIn		\
 od_surf.Printing	\
 od_surf.PrintStyle	\
 od_surf.Protocols	\
 od_surf.Redraw		\
 od_surf.Reformat	\
 od_surf.RMA		\
 od_surf.Save		\
 od_surf.SaveDraw	\
 od_surf.SaveFile	\
 od_surf.SaveObject	\
 od_surf.SaveSHelp	\
 od_surf.SaveText	\
 od_surf.SetPBoxes	\
 od_surf.svcprint	\
 od_surf.Tables		\
 od_surf.TokenUtils	\
 od_surf.Toolbars	\
 od_surf.Trace		\
 od_surf.Unifont	\
 od_surf.URLstat	\
 od_surf.URLutils	\
 od_surf.URLveneer	\
 od_surf.Utils		\
 od_surf.Windows	\
 o.ROSLib

OBJS_ITALK = \
 o_italk.About		\
 o_italk.Authorise	\
 o_italk.Browser	\
 o_italk.Choices	\
 o_italk.Cookies	\
 o_italk.CSIM		\
 o_italk.DragBox	\
 o_italk.Encoding	\
 o_italk.Fetch		\
 o_italk.FetchHTML	\
 o_italk.FetchPage	\
 o_italk.Find		\
 o_italk.FontManage	\
 o_italk.Forms		\
 o_italk.Frames		\
 o_italk.FromROSLib	\
 o_italk.Global		\
 o_italk.Handlers	\
 o_italk.History	\
 o_italk.Hotlist	\
 o_italk.Images		\
 o_italk.ImgHistory	\
 o_italk.ItemInfo	\
 o_italk.JavaScript	\
 o_italk.JSconsts	\
 o_italk.JSdocument	\
 o_italk.JSnavigtr	\
 o_italk.JSscreen	\
 o_italk.JSURL		\
 o_italk.JSutils	\
 o_italk.JSwindow	\
 o_italk.LineOne	\
 o_italk.Main		\
 o_italk.Markers	\
 o_italk.Memory		\
 o_italk.Menus		\
 o_italk.Meta		\
 o_italk.MimeMap	\
 o_italk.Mouse		\
 o_italk.Multiuser	\
 o_italk.Object		\
 o_italk.OpenURL	\
 o_italk.PlugIn		\
 o_italk.Printing	\
 o_italk.PrintStyle	\
 o_italk.Protocols	\
 o_italk.Redraw		\
 o_italk.Reformat	\
 o_italk.RMA		\
 o_italk.Save		\
 o_italk.SaveDraw	\
 o_italk.SaveFile	\
 o_italk.SaveObject	\
 o_italk.SaveSHelp	\
 o_italk.SaveText	\
 o_italk.SetPBoxes	\
 o_italk.Tables		\
 o_italk.TokenUtils	\
 o_italk.Toolbars	\
 o_italk.Unifont	\
 o_italk.URLstat	\
 o_italk.URLutils	\
 o_italk.URLveneer	\
 o_italk.Utils		\
 o_italk.Windows	\
 o.ROSLib

OBJSD_ITALK = \
 od_italk.About		\
 od_italk.Authorise	\
 od_italk.Browser	\
 od_italk.Choices	\
 od_italk.Cookies	\
 od_italk.CSIM		\
 od_italk.DragBox	\
 od_italk.Encoding	\
 od_italk.Fetch		\
 od_italk.FetchHTML	\
 od_italk.FetchPage	\
 od_italk.Find		\
 od_italk.FontManage	\
 od_italk.Forms		\
 od_italk.Frames	\
 od_italk.FromROSLib	\
 od_italk.Global	\
 od_italk.Handlers	\
 od_italk.History	\
 od_italk.Hotlist	\
 od_italk.Images	\
 od_italk.ImgHistory	\
 od_italk.ItemInfo	\
 od_italk.JavaScript	\
 od_italk.JSconsts	\
 od_italk.JSdocument	\
 od_italk.JSnavigtr	\
 od_italk.JSscreen	\
 od_italk.JSURL		\
 od_italk.JSutils	\
 od_italk.JSwindow	\
 od_italk.LineOne	\
 od_italk.Main		\
 od_italk.Markers	\
 od_italk.Memory	\
 od_italk.Menus		\
 od_italk.Meta		\
 od_italk.MimeMap	\
 od_italk.Mouse		\
 od_italk.Multiuser	\
 od_italk.Object	\
 od_italk.OpenURL	\
 od_italk.PlugIn	\
 od_italk.Printing	\
 od_italk.PrintStyle	\
 od_italk.Protocols	\
 od_italk.Redraw	\
 od_italk.Reformat	\
 od_italk.RMA		\
 od_italk.Save		\
 od_italk.SaveDraw	\
 od_italk.SaveFile	\
 od_italk.SaveObject	\
 od_italk.SaveSHelp	\
 od_italk.SaveText	\
 od_italk.SetPBoxes	\
 od_italk.svcprint	\
 od_italk.Tables	\
 od_italk.TokenUtils	\
 od_italk.Toolbars	\
 od_italk.Trace		\
 od_italk.Unifont	\
 od_italk.URLstat	\
 od_italk.URLutils	\
 od_italk.URLveneer	\
 od_italk.Utils		\
 od_italk.Windows	\
 o.ROSLib


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .o_surf .od_surf .o_italk .od_italk .s .c

.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CFLAGS} ${DFLAGS}        -c -o $@ $<

.c.oz:;		@echo
		@echo Compiling $<
		@echo ${DZFLAGS}
		@${CC} ${CFLAGS} ${DZFLAGS}       -c -zM -DROM -o $@ $<

.c.od:;		@echo
		@echo Compiling $<
		@echo ${DDFLAGS}
		@${CC} ${CFLAGS} ${DDFLAGS}       -c -g -o $@ $<

.c.odz:;	@echo
		@echo Compiling $<
		@echo ${DDZFLAGS}
		@${CC} ${CFLAGS} ${DDZFLAGS}      -c -zM -DROM -o $@ $<

.c.o_surf:;	@echo
		@echo Compiling $<
		@echo ${D_SURFFLAGS}
		@${CC} ${CFLAGS} ${D_SURFFLAGS}   -c -o $@ $<

.c.od_surf:;	@echo
		@echo Compiling $<
		@echo ${DD_SURFFLAGS}
		@${CC} ${CFLAGS} ${DD_SURFFLAGS}  -c -g -o $@ $<

.c.o_italk:;	@echo
		@echo Compiling $<
		@echo ${D_ITALKFLAGS}
		@${CC} ${CFLAGS} ${D_ITALKFLAGS}  -c -o $@ $<

.c.od_italk:;	@echo
		@echo Compiling $<
		@echo ${DD_ITALKFLAGS}
		@${CC} ${CFLAGS} ${DD_ITALKFLAGS} -c -g -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@echo ${AFLAGS}
		@${AS} ${AFLAGS} $< $@



# -----------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
	@echo ${COMPONENT}: Application built (Disc)

rom: ${ROM_MODULE}
	@echo ${COMPONENT}: Module built (ROM)

install_common: ${FILES}
	@echo
	${CP} ${TARGET}				${INSTDIR}.!RunImage		${CPFLAGS}~n
	${CP} ${LDIR}.!Sprites			${INSTDIR}.!Sprites		${CPFLAGS}n
	${CP} ${LDIR}.!Sprites22		${INSTDIR}.!Sprites22		${CPFLAGS}n
	${CP} ${LDIR}.8desktop			${INSTDIR}.8desktop		${CPFLAGS}n
	${CP} ${LDIR}.Choices			${INSTDIR}.Choices		${CPFLAGS}n
	${CP} ${LDIR}.Controls			${INSTDIR}.Controls		${CPFLAGS}n
	${CP} ${LDIR}.Messages			${INSTDIR}.Messages		${CPFLAGS}n
	${CP} ${LDIR}.Sprites			${INSTDIR}.Sprites		${CPFLAGS}n
	-${CP} ${LDIR}.Sprites22		${INSTDIR}.Sprites22		${CPFLAGS}n
	-${CP} ${LDIR}.User			${INSTDIR}.User			${CPFLAGS}n
	${WIPE} ${INSTDIR}.User.CVS	${WFLAGS}

install: install_common
	${CP} ${LDIR}.!Boot		${INSTDIR}.!Boot	${CPFLAGS}~n
	${CP} ${LDIR}.!Run		${INSTDIR}.!Run		${CPFLAGS}~n
	-${CP} ${LDIR}.About		${INSTDIR}.About	${CPFLAGS}~n
	-${CP} ${LDIR}.Res		${INSTDIR}.Res		${CPFLAGS}~n
	${WIPE} ${INSTDIR}.About.CVS	${WFLAGS}
	-Access ${INSTDIR}.*		wr/r
	-Access ${INSTDIR}.User.*	wr/r
	-Access ${INSTDIR}.About.*	wr/r
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR}

installd: install_common
	${CP} ${LDIR}.!Boot		${INSTDIR}.!Boot	${CPFLAGS}~n
	${CP} ${LDIR}.!RunD		${INSTDIR}.!Run		${CPFLAGS}~n
	-${CP} ${LDIR}.About		${INSTDIR}.About	${CPFLAGS}~n
	-${CP} ${LDIR}.Res		${INSTDIR}.Res		${CPFLAGS}~n
	${WIPE} ${INSTDIR}.About.CVS	${WFLAGS}
	@echo
	@echo ${COMPONENT}: Debug application installed ${INSTDIR}

installj: install_common
	${CP} ${LDIR}.!BootJ		${INSTDIR}.!Boot	${CPFLAGS}~n
	${CP} ${LDIR}.!RunJ		${INSTDIR}.!Run		${CPFLAGS}~n
	${CP} ${LDIR}.AboutJ		${INSTDIR}.About	${CPFLAGS}~n
	${CP} ${LDIR}.ResJ		${INSTDIR}.Res		${CPFLAGS}~n
	${WIPE} ${INSTDIR}.About.CVS	${WFLAGS}
	-Access ${INSTDIR}.*		wr/r
	-Access ${INSTDIR}.User.*	wr/r
	-Access ${INSTDIR}.About.*	wr/r
	@echo
	@echo ${COMPONENT}: Application installed to ${INSTDIR} (JavaScript build)

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
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
	${CP} ${LDIR}.Controls		${RESDIR}.Controls	${CPFLAGS}
	${CP} ${LDIR}.Messages		${RESDIR}.Messages	${CPFLAGS}
	${CP} ${LDIR}.Res		${RESDIR}.Res		${CPFLAGS}
	${CP} ${LDIR}.Sprites		${RESDIR}.Sprites	${CPFLAGS}
	${CP} ${LDIR}.User		${RESDIR}.User		${CPFLAGS}
	${CP} ${LDIR}.About		${RESDIR}.About		${CPFLAGS}
	${WIPE} ${RESDIR}.User.CVS	${WFLAGS}
	${WIPE} ${RESDIR}.About.CVS	${WFLAGS}
	@echo
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
	@echo
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
	@${MKDIR} syms
	@${MKDIR} Targets
	@${MKDIR} Targets.Argonet
	@${MKDIR} Targets.Argonet.!VoyBrowse
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


# -----------------------------------------------------------------------------
# Development rules
#

linkmap: ${OBJS} ${LIBS}
	${LD} -map -o null:x ${OBJS} ${LIBS} > map.linked

map:
	${LD} -map -bin -base 0 -o null: ${OBJS} ${LIBS} > map.base0

clean_all: clean
	${WIPE}	Targets		${WFLAGS}
	@echo ${COMPONENT}: Cleaned all


# -----------------------------------------------------------------------------
# Final linkfor ROM Image (using given base address)
#

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


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: ${OBJS} ${LIBS} o.dirs
	@echo
	${LD} -s syms.!RI -o $@ ${OBJS} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_J: ${OBJS} ${LIBS} o.dirs
	@echo
	${LD} -s syms.!RI_J -o $@ ${OBJS} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_D: ${OBJSD} ${LIBSD} o.dirs
	@echo
	${LD} -debug -s syms.!RI_D -o $@ ${OBJSD} ${LIBSD}

abs.!RI_Surf: ${OBJS_SURF} ${LIBS} o.dirs
	@echo
	${LD}  -s syms.!RI_Surf -o $@ ${OBJS_SURF} ${LIBS}

abs.!RI_Surf_J: ${OBJS_SURF} ${LIBS} o.dirs
	@echo
	${LD}  -s syms.!RI_Surf_J -o $@ ${OBJS_SURF} ${LIBS}

abs.!RI_DSurf: ${OBJSD_SURF} ${LIBSD} o.dirs
	@echo
	${LD} -debug -s syms.!RI_DSurf -o $@ ${OBJSD_SURF} ${LIBSD}

abs.!RI_ITalk: ${OBJS_ITALK} ${LIBS} o.dirs
	@echo
	${LD} -s syms.!RI_ITalk -o $@ ${OBJS_ITALK} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_ITalk_J: ${OBJS_ITALK} ${LIBS} o.dirs
	@echo
	${LD} -s syms.!RI_ITalk_J -o $@ ${OBJS_ITALK} ${LIBS}
	${SQUEEZE} ${SQFLAGS} $@

abs.!RI_DITalk: ${OBJSD_ITALK} ${LIBSD} o.dirs
	@echo
	${LD} -debug -s syms.!RI_DITalk -o $@ ${OBJSD_ITALK} ${LIBSD}

${MODULE}: oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB} o.dirs
	@echo
	${LD} -s syms.RAMMod -o $@ -module oz.ModuleWrap ${OBJSZ} ${LIBSM} ${CLIB}

${ROM_MODULE}: oz.ModuleWrap ${OBJSZ} ${ROMCSTUBS} ${LIBSM} o.dirs
	@echo
	${LD} -s syms.ROMMod -o $@ -aof oz.ModuleWrap ${OBJSZ} ${LIBSM} ${ROMCSTUBS}

rm.!BrowseD: odz.ModuleWrap ${OBJSDZ} ${LIBSM} o.dirs
	@echo
	${LD} -s syms.ROMDMod -o $@ -module oz.ModuleWrap ${OBJSDZ} ${LIBSM}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
