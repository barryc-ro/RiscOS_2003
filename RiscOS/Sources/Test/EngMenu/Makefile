# Makefile for EngMenu
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
#  AR	   4/2/2000   included resources target

#
# Program specific options:
#

COMPONENT   = EngMenu
APP	    = !EngMenu
RDIR        = Resources.${USERIF}
LDIR	    = EngMenuRes:
TARGET      = ${RDIR}.!RunImage
UTIL_TARGET = ${RDIR}.CheckStrt
D_TARGET    = ${RDIR}.!RunImaDB
I_TARGET    = ${RDIR}.!RunImagI

# Resources paths
#
RESAPP = <resource$dir>.Apps.${APP}
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
CLIB   = C:o.stubs <Lib$Dir>.c++lib.o.c++lib TCPIPLibs:o.unixlib
DLIBS  = <LIB$Dir>.DebugLib.o.debuglib TCPIPLibs:o.socklib TCPIPLibs:o.inetlib
#
# Include files
#
INCLUDES =  -IC:,TCPIPLibs:

UTILOBJS =	\
o.CheckStrt

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
.c++.d_o:;  ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.d_o:;    ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.c++.i:;    ${C++} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.c.i:;      ${CC} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.inst.io:;  ${C++} ${CFLAGS} ${DFLAGS} -W -o $@ $<

#
# Build
#
all: ${TARGET} dirs
	access ${TARGET} RW/r
        @echo ${COMPONENT}: all complete

d_all: ${D_TARGET} dirs
	access ${D_TARGET} RW/r
        @echo ${COMPONENT}: all complete

util:  ${UTIL_TARGET} dirs
	access ${UTIL_TARGET} RW/r
	SetType ${UTIL_TARGET} Utility
	@echo ${COMPONENT}: utility complete
        
instrument: ${I_TARGET} dirs
	access ${I_TARGET} RW/r
	@echo ${COMPONENT}: instrument build complete
	
preprocess: ${OBJSI} dirs
	@echo ${COMPONENT}: preprocess build complete

dirs:
        ${MKDIR} o
        ${MKDIR} d_o
        ${MKDIR} i
        ${MKDIR} io
        ${MKDIR} inst
        ${MKDIR} ${RESAPP}
        
resources: ${TARGET} dirs
	squeeze ${TARGET}
	set EngMenuRes$Path Resources.${USERIF}.${LOCALE}., Resources.${USERIF}.UK.
        ${CP} ${RDIR}.!Run      ${RESAPP}.!Run ${CPFLAGS}
        access ${RESAPP}.!Run WR/r
        ${CP} ${RDIR}.Pace	${RESAPP}.Pace ${CPFLAGS}
        access ${RESAPP}.Pace WR/r
        ${CP} ${RDIR}.CheckStrt	${RESAPP}.CheckStrt ${CPFLAGS}
        access ${RESAPP}.CheckStrt WR/r
        ${CP} ${LDIR}Messages   ${RESAPP}.Messages ${CPFLAGS}
        access ${RESAPP}.Messages WR/r
        ${CP} ${LDIR}ConfigNTSC ${RESAPP}.ConfigNTSC ${CPFLAGS}
        access ${RESAPP}.ConfigNTSC WR/r
        ${CP} ${LDIR}ConfigPAL ${RESAPP}.ConfigPAL ${CPFLAGS}
        access ${RESAPP}.ConfigPAL WR/r
        ${CP} ${RDIR}.!RunImage ${RESAPP}.!RunImage ${CPFLAGS}
        access ${RESAPP}.!RunImage WR/r
        access ${RESAPP} /r
        unset EngMenuRes$Path
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} ${TARGET} ${WFLAGS}
        ${WIPE} ${d_TARGET} ${WFLAGS}
        ${WIPE} ${I_TARGET} ${WFLAGS}
	${WIPE} ${UTIL_TARGET} ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} d_o ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} io ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${WIPE} ${RESAPP}.!Run ${WFLAGS}
        ${WIPE} ${RESAPP}.Pace ${WFLAGS}
        ${WIPE} ${RESAPP}.CheckStrt ${WFLAGS}
        ${WIPE} ${RESAPP}.!RunImage ${WFLAGS}
        ${WIPE} ${RESAPP}.Messages ${WFLAGS}
        ${WIPE} ${RESAPP}.ConfigNTSC ${WFLAGS}
        ${WIPE} ${RESAPP}.ConfigPAL ${WFLAGS}
        ${WIPE} ${RESAPP} ${WFLAGS}	
        @echo ${COMPONENT}: cleaned
        
#
# Target
#
${UTIL_TARGET}: ${UTILOBJS}
	${LD} -o $@ -bin ${UTILOBJS}
	
	
${I_TARGET}: ${OBJSINST}
	${LD} ${Linkflags} ${OBJSINST} ${CLIB}

${TARGET}: ${OBJS}
        ${LD} ${Linkflags} ${OBJS} ${CLIB}

${D_TARGET}: ${d_OBJS}
        ${LD} ${Linkflags} ${d_OBJS} ${CLIB} ${DLIBS}

#
# Dynamic dependencies:
