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

DEFINES = ${DEBUG} ${THROWBACK}
COMMONFLAGS = -zM -c -Wp -ws -depend !Depend ${INCLUDES} ${DEFINES}

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${COMMONFLAGS} -Throwback
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CPFLAGS = ~cfr~v 
WFLAGS  = ~c~v


#
# Libraries
#
CLIB       = CLIB:o.stubs CLIB:o.c++lib <Lib$Dir>.RISC_OSLib.o.risc_oslib TCPIPLibs:o.unixlib
DLIBS  = <LIB$Dir>.DebugLib.o.debuglib TCPIPLibs:o.socklib TCPIPLibs:o.inetlib
#
# Include files
#
INCLUDES =  -ICLIB:,TCPIPLibs:

# Program specific options:
#
COMPONENT = EngMenu
TARGET    = !EngMenu.!RunImage
D_TARGET  = !EngMenu.!RunImaDB

OBJS     = 	\
o.main 		\
o.graphics	\
o.message	\
o.parser	\
o.menuhier	\
o.menu

d_OBJS     = 	\
d_o.main 	\
d_o.graphics	\
d_o.message	\
d_o.parser	\
d_o.menuhier	\
d_o.menu

OBJSI     =	\
i.main 		\
i.graphics	\
i.message	\
i.parser	\
i.menuhier	\
i.menu

OBJSINST   =	\
oi.main		\
oi.graphics	\
oi.message	\
oi.parser	\
oi.menuhier	\
oi.menu		\
oi.instlib

#
# Rule patterns
#
.SUFFIXES:	.d_o .o .oi .s .c .c++ .i .cmhg .inst
.c++.o:;    ${C++} ${CFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c++.d_o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.d_o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.c++.i:;    ${CC} ${C++FLAGS} -c -C -E $< >> $@
.c.i:;      ${CC} ${CFLAGS} -c -C -E $< >> $@
.inst.oi:;  ${CC} ${CFLAGS} -C++ -W -o $@ $<

#
# Build
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete

d_all: ${D_TARGET} dirs
        @echo ${COMPONENT}: all complete
dirs:
        ${MKDIR} o
        ${MKDIR} d_o
        ${MKDIR} i
        ${MKDIR} oi
        ${MKDIR} inst

preprocess: ${OBJSI} dirs
	@echo ${COMPONENT}: preprocess build complete

instrument: ${OBJSINST} dirs 
	${LD} -rmf -s link/sym -o ${TARGET}u ${OBJSINST} ${STUBS}
	@echo ${COMPONENT}: instrument build complete

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} d_o ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} oi ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        ${WIPE} ${d_TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned
        
#
# Target
#
${TARGET}: ${OBJS}
        ${LD} ${Linkflags} ${OBJS} ${CLIB}

${D_TARGET}: ${d_OBJS}
        ${LD} ${Linkflags} ${d_OBJS} ${CLIB} ${DLIBS}

#
# Dynamic dependencies:
