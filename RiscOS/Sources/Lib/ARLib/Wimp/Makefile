# Makefile for ARLib/Wimp
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 14 Nov 2000   BJGA            Created
#

# ------------------------------------------------------------------------------
# Paths
#

LIBDIR          = <Lib$Dir>

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
AS              = objasm
CP              = copy
CC              = cc
RM              = remove
LB              = libfile
WIPE            = -wipe

CFLAGS          = -c -depend !Depend $(THROWBACK) -ffahw -zo -strict $(INCLUDES) $(DFLAGS)
AFLAGS          = -depend !Depend $(THROWBACK) -Stamp -quit
CPFLAGS         = ~CLFNR~V
LBFLAGS         = -c
WFLAGS          = ~CFR~V

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES        = -IC:
DFLAGS          = -DNDEBUG

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT       = ARLib/Wimp
TARGET          = WimpLib

OBJS            = o.alarm o.baricon o.clipboard o.coords o.device o.event o.fileicon o.help \
                  o.iconmenu o.icons o.menu o.pane o.redraw o.template o.wimp o.wimpt \
                  o.win o.wintitle o.wmisc o.wmiscclose o.wmiscflush o.wmiscshift o.xferrecv \
                  o.xfersend o.poll o.wimpasm o.msgclaim o.plotsprite o.taskwindow o.wmiscquit

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .c
.c.o:;     $(CC) $(CFLAGS) -o $@ $<
.s.o:;     $(AS) $(AFLAGS) -o $@ $<

# ------------------------------------------------------------------------------
# Build rules
#

all:    $(TARGET)
        @echo $(COMPONENT): library built

export: export_$(PHASE)

export_hdrs: h.alarm h.baricon h.clipboard h.coords h.device h.event h.fileicon h.help \
             h.iconmenu h.icons h.menu h.msgclaim h.pane h.plotsprite h.redraw h.taskman \
             h.taskwindow h.template h.wimpt h.wimpx h.win h.wmisc h.xferrecv h.xfersend dirs
        $(CP)   h.alarm         $(LIBDIR).ARLib.h.alarm         $(CPFLAGS)
        $(CP)   h.baricon       $(LIBDIR).ARLib.h.baricon       $(CPFLAGS)
        $(CP)   h.clipboard     $(LIBDIR).ARLib.h.clipboard     $(CPFLAGS)
        $(CP)   h.coords        $(LIBDIR).ARLib.h.coords        $(CPFLAGS)
        $(CP)   h.device        $(LIBDIR).ARLib.h.device        $(CPFLAGS)
        $(CP)   h.event         $(LIBDIR).ARLib.h.event         $(CPFLAGS)
        $(CP)   h.fileicon      $(LIBDIR).ARLib.h.fileicon      $(CPFLAGS)
        $(CP)   h.help          $(LIBDIR).ARLib.h.help          $(CPFLAGS)
        $(CP)   h.iconmenu      $(LIBDIR).ARLib.h.iconmenu      $(CPFLAGS)
        $(CP)   h.icons         $(LIBDIR).ARLib.h.icons         $(CPFLAGS)
        $(CP)   h.menu          $(LIBDIR).ARLib.h.menu          $(CPFLAGS)
        $(CP)   h.msgclaim      $(LIBDIR).ARLib.h.msgclaim      $(CPFLAGS)
        $(CP)   h.pane          $(LIBDIR).ARLib.h.pane          $(CPFLAGS)
        $(CP)   h.plotsprite    $(LIBDIR).ARLib.h.plotsprite    $(CPFLAGS)
        $(CP)   h.redraw        $(LIBDIR).ARLib.h.redraw        $(CPFLAGS)
        $(CP)   h.taskman       $(LIBDIR).ARLib.h.taskman       $(CPFLAGS)
        $(CP)   h.taskwindow    $(LIBDIR).ARLib.h.taskwindow    $(CPFLAGS)
        $(CP)   h.template      $(LIBDIR).ARLib.h.template      $(CPFLAGS)
        $(CP)   h.wimpt         $(LIBDIR).ARLib.h.wimpt         $(CPFLAGS)
        $(CP)   h.wimpx         $(LIBDIR).ARLib.h.wimpx         $(CPFLAGS)
        $(CP)   h.win           $(LIBDIR).ARLib.h.win           $(CPFLAGS)
        $(CP)   h.wmisc         $(LIBDIR).ARLib.h.wmisc         $(CPFLAGS)
        $(CP)   h.xferrecv      $(LIBDIR).ARLib.h.xferrecv      $(CPFLAGS)
        $(CP)   h.xfersend      $(LIBDIR).ARLib.h.xfersend      $(CPFLAGS)
        @echo $(COMPONENT): export complete (hdrs)

export_libs: $(TARGET) dirs
        $(CP)   $(TARGET)       $(LIBDIR).ARLib.o.$(TARGET)     $(CPFLAGS)
        @echo $(COMPONENT): export complete (libs)

clean:
        $(WIPE) o $(WFLAGS)
        $(RM) $(TARGET)
        @echo $(COMPONENT): cleaned

local_dirs:
        $(MKDIR) o

dirs:
        $(MKDIR) $(LIBDIR).ARLib.h
        $(MKDIR) $(LIBDIR).ARLib.o

$(TARGET): $(OBJS) local_dirs
        $(LB) $(LBFLAGS) -o $(TARGET) $(OBJS)

# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.poll: s.poll
o.msgclaim: c.msgclaim C:ARLib.h.win C:ARLib.h.wimp C:ARLib.h.msgclaim
o.alarm: c.alarm C:ARLib.h.alarm C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.os C:ARLib.h.msgs C:h.swis C:h.kernel
o.baricon: c.baricon C:ARLib.h.os C:ARLib.h.screenmode C:ARLib.h.spr C:ARLib.h.memalloc C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.win C:ARLib.h.baricon
o.clipboard: c.clipboard C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.wimp C:ARLib.h.file C:ARLib.h.os C:ARLib.h.macros C:ARLib.h.msgs C:ARLib.h.werr C:ARLib.h.wimpx C:ARLib.h.win C:ARLib.h.taskman C:ARLib.h.xfersend C:ARLib.h.clipboard C:ARLib.h.memalloc
o.coords: c.coords C:ARLib.h.wimp C:ARLib.h.coords
o.device: c.device C:ARLib.h.alarm C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.wimp C:ARLib.h.msgs C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimpt C:ARLib.h.win C:ARLib.h.device
o.event: c.event C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.werr C:ARLib.h.wimpt C:ARLib.h.win C:ARLib.h.menu C:ARLib.h.alarm C:ARLib.h.event
o.fileicon: c.fileicon C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.fileicon C:ARLib.h.werr
o.help: c.help C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.wimpt C:ARLib.h.werr C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.dbox C:ARLib.h.msgs C:ARLib.h.help C:ARLib.h.menu
o.iconmenu: c.iconmenu C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.wimp C:ARLib.h.help C:ARLib.h.dbox C:ARLib.h.msgs C:ARLib.h.wimpx C:ARLib.h.os C:ARLib.h.wimpt C:ARLib.h.werr C:ARLib.h.macros C:ARLib.h.iconmenu
o.icons: c.icons C:ARLib.h.msgs C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.icons C:ARLib.h.wmisc C:ARLib.h.win
o.menu: c.menu C:ARLib.h.os C:ARLib.h.wimp C:ARLib.h.werr C:ARLib.h.menu C:ARLib.h.res_spr C:ARLib.h.spr C:ARLib.h.memalloc C:ARLib.h.msgs
o.pane: c.pane C:ARLib.h.coords C:ARLib.h.wimp C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.win C:ARLib.h.macros C:ARLib.h.screenmode C:ARLib.h.wmisc C:ARLib.h.pane
o.redraw: c.redraw C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.redraw
o.template: c.template C:h.swis C:h.kernel C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.res C:ARLib.h.res_spr C:ARLib.h.spr C:ARLib.h.memalloc C:ARLib.h.werr C:ARLib.h.template C:ARLib.h.msgs
o.wimp: c.wimp C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp
o.wimpt: c.wimpt C:ARLib.h.os C:ARLib.h.bbc C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.werr C:ARLib.h.alarm C:ARLib.h.msgs C:ARLib.h.win C:h.swis C:h.kernel C:ARLib.h.wmisc C:ARLib.h.wimpt
o.win: c.win C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.werr C:ARLib.h.wimpt C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.msgs C:ARLib.h.win
o.wintitle: c.wintitle C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.werr C:ARLib.h.win
o.wmiscclose: c.wmiscclose C:ARLib.h.akbd C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.os C:ARLib.h.fileract C:ARLib.h.werr C:ARLib.h.wmisc C:ARLib.h.win
o.wmiscflush: c.wmiscflush C:ARLib.h.akbd C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.wimp C:h.swis C:h.kernel C:ARLib.h.wimpx C:ARLib.h.os C:ARLib.h.werr C:ARLib.h.wimpt C:ARLib.h.win C:ARLib.h.macros C:ARLib.h.wmisc
o.wmiscshift: c.wmiscshift C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.wmisc C:ARLib.h.win
o.xferrecv: c.xferrecv C:ARLib.h.os C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.wimpt C:ARLib.h.win C:ARLib.h.xfersend C:ARLib.h.werr C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.xferrecv C:ARLib.h.msgs
o.xfersend: c.xfersend C:ARLib.h.os C:ARLib.h.bbc C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.wimpt C:ARLib.h.win C:ARLib.h.xfersend C:ARLib.h.fileicon C:ARLib.h.werr C:ARLib.h.menu C:ARLib.h.event C:ARLib.h.msgs C:ARLib.h.strfn
o.wimpasm: s.wimpasm Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS ^.Hdr.CRegs ^.Hdr.Macros2 Hdr:ModHand Hdr:Wimp Hdr:Proc
o.plotsprite: c.plotsprite C:ARLib.h.coords C:ARLib.h.wimp C:ARLib.h.res_spr C:ARLib.h.spr C:ARLib.h.memalloc C:ARLib.h.os C:ARLib.h.screenmode C:h.swis C:h.kernel C:ARLib.h.werr C:ARLib.h.wimpx C:ARLib.h.macros C:ARLib.h.plotsprite
o.taskwindow: c.taskwindow C:h.kernel C:ARLib.h.msgs C:ARLib.h.werr C:ARLib.h.os C:ARLib.h.wimpt C:ARLib.h.wimp C:ARLib.h.wimpx C:ARLib.h.win C:ARLib.h.macros C:ARLib.h.taskwindow
o.wmiscquit: c.wmiscquit C:ARLib.h.akbd C:ARLib.h.wimpx C:ARLib.h.wimp C:ARLib.h.os C:ARLib.h.werr C:ARLib.h.win C:ARLib.h.macros C:ARLib.h.wmisc
o.wmisc: c.wmisc C:ARLib.h.dbox C:ARLib.h.help C:ARLib.h.wimp C:ARLib.h.event C:ARLib.h.menu C:ARLib.h.msgs C:ARLib.h.res C:ARLib.h.screenmode C:ARLib.h.os C:ARLib.h.werr C:ARLib.h.wimpx C:ARLib.h.wimpt C:ARLib.h.win C:ARLib.h.file C:ARLib.h.macros C:ARLib.h.wmisc
