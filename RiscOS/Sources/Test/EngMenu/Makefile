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

DEFINES = ${DEBUG} ${THROWBACK} ${McCabe}
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
CLIB   = CLIB:o.stubs CLIB:o.c++lib <Lib$Dir>.RISC_OSLib.o.risc_oslib TCPIPLibs:o.unixlib
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
I_TARGET  = !EngMenu.!RunImagI

OBJS     = 	\
o.main 		\
o.graphics	\
o.message	\
o.parser	\
o.menuhier	\
o.interface	\
o.menu

d_OBJS     = 	\
d_o.main 	\
d_o.graphics	\
d_o.message	\
d_o.parser	\
d_o.menuhier	\
d_o.interface	\
d_o.menu

OBJSI     =	\
i.main 		\
i.graphics	\
i.message	\
i.parser	\
i.menuhier	\
i.interface	\
i.menu

OBJSINST   =	\
io.instlib	\
io.main		\
io.graphics	\
io.message	\
io.parser	\
io.menuhier	\
io.interface	\
io.menu

#
# Rule patterns
#
.SUFFIXES:	.d_o .o .s .c .c++ .i .cmhg .inst .io

.c++.o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c++.d_o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.d_o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.c++.i:;    ${C++} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.c.i:;      ${CC} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.inst.io:;  ${C++} ${CFLAGS} ${DFLAGS} -W -o $@ $<

#
# Build
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete

d_all: ${D_TARGET} dirs
        @echo ${COMPONENT}: all complete
        
instrument: ${I_TARGET} dirs
	@echo ${COMPONENT}: instrument build complete
	
preprocess: ${OBJSI} dirs
	@echo ${COMPONENT}: preprocess build complete

dirs:
        ${MKDIR} o
        ${MKDIR} d_o
        ${MKDIR} i
        ${MKDIR} io
        ${MKDIR} inst

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} d_o ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} io ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        ${WIPE} ${d_TARGET} ${WFLAGS}
        ${WIPE} ${I_TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned
        
#
# Target
#
${I_TARGET}: ${OBJSINST}
	${LD} ${Linkflags} ${OBJSINST} ${CLIB}

${TARGET}: ${OBJS}
        ${LD} ${Linkflags} ${OBJS} ${CLIB}

${D_TARGET}: ${d_OBJS}
        ${LD} ${Linkflags} ${d_OBJS} ${CLIB} ${DLIBS}

#
# Dynamic dependencies:
