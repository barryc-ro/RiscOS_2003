# Makefile for pm

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
C++	= c++
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir

# Debug flags
DFLAGS = ${DEBUG}

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} -Throwback
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v


#
# Libraries
#
CLIB       = CLIB:o.stubs
CPPLIB	   = Cpp:o.c++lib
DLIB       = <Lib$Dir>.DebugLib.o.debuglib TCPIPLibs:o.socklib TCPIPLibs:o.inetlib
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
EVENTLIB   = TBox:o.eventlib
TOOLBOXLIB = TBox:o.toolboxlib
WIMPLIB	   = TBox:o.wimplib
#
# Include files
#
INCLUDES = -ITbox:,C:

# Program specific options:
#
COMPONENT = fish
TARGET    = !RunImage
dTARGET   = !RunImaDB

OBJS      =	\
o.error		\
o.file		\
o.module	\
o.events	\
o.main		\
o.printer	\
o.fwindow	\
o.file_op	\
o.WimpJob

d_OBJS      =	\
do.error	\
do.file		\
do.module	\
do.events	\
do.main		\
do.printer	\
do.fwindow	\
do.file_op	\
do.WimpJob

#
# Rule patterns
#
.SUFFIXES:	.do .o .s .c .c++ .i .cmhg .inst .io

.c++.o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c++.do:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.do:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.c++.i:;    ${C++} ${CFLAGS} -c -C -E $< >> $@
.c.i:;      ${CC} ${CFLAGS} -c -C -E $< >> $@
.inst.io:;  ${C++} ${CFLAGS} -W -o $@ $<

#
# Build
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete

debug: ${dTARGET} dirs
	@echo ${d_COMPONENT}: debug complete
dirs:
        ${MKDIR} o
        ${MKDIR} do

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} do ${WFLAGS}
        ${WIPE} ${dTARGET} ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Target
#
${TARGET}: ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}
        ${LD} ${Linkflags} ${OBJS} ${COBJ} ${CLIB} ${CPPLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}

${dTARGET}: ${d_OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}
        ${LD} ${Linkflags} ${d_OBJS} ${COBJ} ${CLIB} ${CPPLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${DLIB}

#
# Dynamic dependencies:
do.error: c.error C:h.kernel C:h.swis h.error C:h.kernel h.main Tbox:h.wimp Tbox:h.wimplib Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits
do.file: c.file C:h.kernel C:h.swis h.file C:h.kernel h.error C:h.kernel h.main Tbox:h.wimp Tbox:h.wimplib Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits
do.module: c.module C:h.kernel C:h.swis h.module C:h.kernel
do.events: c++.events h.events h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.WimpJob h.main h.file_op h.main h.WimpJob h.error C:h.kernel h.printer h.main h.fwindow h.WimpJob h.file_op
do.main: c++.main h.WimpJob h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.main h.events h.main h.WimpJob h.printer h.main h.fwindow h.WimpJob h.file_op h.main h.WimpJob h.error C:h.kernel <Lib$Dir>.DebugLib.h.DebugLib C:debug.h.remote
do.printer: c++.printer h.printer h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.fwindow h.WimpJob h.main h.file_op h.main h.WimpJob h.error C:h.kernel h.module C:h.kernel
do.fwindow: c++.fwindow h.fwindow h.WimpJob h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits
do.file_op: c++.file_op h.file C:h.kernel h.error C:h.kernel h.file_op h.main Tbox:h.wimp Tbox:h.wimplib Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.WimpJob h.main
do.WimpJob: c++.WimpJob h.WimpJob h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.error C:h.kernel
o.error: c.error C:h.kernel C:h.swis h.error C:h.kernel h.main Tbox:h.wimp Tbox:h.wimplib Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits
o.file: c.file C:h.kernel C:h.swis h.file C:h.kernel h.error C:h.kernel h.main Tbox:h.wimp Tbox:h.wimplib Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits
o.module: c.module C:h.kernel C:h.swis h.module C:h.kernel
o.events: c++.events h.events h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.WimpJob h.main h.file_op h.main h.WimpJob h.error C:h.kernel h.printer h.main h.fwindow h.WimpJob h.file_op
o.main: c++.main h.WimpJob h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.main h.events h.main h.WimpJob h.printer h.main h.fwindow h.WimpJob h.file_op h.main h.WimpJob h.error C:h.kernel <Lib$Dir>.DebugLib.h.DebugLib
o.printer: c++.printer h.printer h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.fwindow h.WimpJob h.main h.file_op h.main h.WimpJob h.error C:h.kernel h.module C:h.kernel
o.fwindow: c++.fwindow h.fwindow h.WimpJob h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits
o.file_op: c++.file_op h.file C:h.kernel h.error C:h.kernel h.file_op h.main Tbox:h.wimp Tbox:h.wimplib Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.WimpJob h.main
o.WimpJob: c++.WimpJob h.WimpJob h.main Tbox:h.wimp Tbox:h.wimplib C:h.kernel Tbox:h.toolbox Tbox:h.gadgets Tbox:h.saveas Tbox:h.window Tbox:h.menu Tbox:h.window Tbox:h.event C:h.kernel C:h.swis C:h.limits h.error C:h.kernel
