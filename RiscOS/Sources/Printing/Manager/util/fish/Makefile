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
