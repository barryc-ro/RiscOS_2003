# Project:   !PrintTool
# Whose sole purpose in life is to allow palettes to be tuned
# to an acceptable level.

#
# Paths
#
EXP_HDR = <export$dir>

# Libraries
#
CLIB       = CLIB:o.stubs
ABCLIB     = <LIB$Dir>.ABClib.o.ABCLib
DLIB       = <LIB$Dir>.DebugLib.o.debuglib TCPIPLibs:o.socklib TCPIPLibs:o.inetlib
CPPLIB	   = Cpp:o.c++lib
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
TOOLBOXLIB = TBox:o.toolboxlib
EVENTLIB   = TBox:o.eventlib
WIMPLIB	   = TBox:o.wimplib

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS} -Throwback
C++FLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS} -Throwback
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
# Include files
#
INCLUDES = -I<LIB$Dir>.ABCLib.,C:,,tbox:

COMPONENT = Printtool
TARGET    = !RunImage
DTARGET	  = !RunImaDB

OBJS      =	\
o.main		\
o.calib		\
o.display	\
o.drawfile	\
o.tboxclass	\
o.walkth	\
o.error		\
o.palchoose	\
o.prtablegen

d_OBJS	  = 	\
d_o.main	\
d_o.calib	\
d_o.display	\
d_o.drawfile	\
d_o.tboxclass	\
d_o.walkth	\
d_o.error	\
d_o.palchoose	\
d_o.prtablegen

#
# Rule patterns
#
.SUFFIXES:	.d_o .o .s .c .c++ .i .cmhg .inst .io

.c++.o:;    ${C++} ${C++FLAGS} ${DFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c++.d_o:;  ${C++} ${C++FLAGS} ${DFLAGS} -o $@ $<
.c.d_o:;    ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.c++.i:;    ${C++} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.c.i:;      ${CC} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.inst.io:;  ${C++} ${CFLAGS} ${DFLAGS} -W -o $@ $<

#
# Build
#
all:    ${TARGET} dirs
        @echo ${COMPONENT}: all complete

d_all:  ${DTARGET} dirs
        @echo ${COMPONENT}: debug version complete

dirs:
        ${MKDIR} o
	${MKDIR} d_o

clean:
        ${WIPE} o.* ${WFLAGS}
	${WIPE} d_o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        ${WIPE} ${DTARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Final targets:
${TARGET}: ${OBJS} ${EVENTLIB} ${ABCLIB} ${WIMPLIB}
        ${LD} ${Linkflags} ${OBJS} ${COBJ} ${CLIB} ${CPPLIB} ${EVENTLIB} ${ABCLIB} ${WIMPLIB}

${DTARGET}: ${d_OBJS} ${EVENTLIB} ${DLIB} ${WIMPLIB}
        ${LD} ${Linkflags} ${d_OBJS} ${COBJ} ${CLIB} ${CPPLIB} ${EVENTLIB} ${ABCLIB} ${DLIB} ${WIMPLIB}


# Dynamic dependencies:
