
# Project:   VSPHyperms

COMPONENT = VSPHyperms
DEBUG_COMPONENT = VSPHyperDM

#
# Paths
#
EXP_HDR = <export$dir>

# Libraries
#
TCPIPLib   = TCPIPLibs:o.unixlibzm TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklib5zm
DebugLib   = <Lib$Dir>.DebugLib.o.DebugLibzm
CLIB       = CLIB:o.stubs
CPPLIB	   = CLIB:o.c++libz
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
TOOLBOXLIB = C:tboxlibs.o.toolboxlib
EVENTLIB   = C:tboxlibs.o.eventlib
WIMPLIB	   = C:tboxlibs.o.wimplib

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
AWK	= awk

# Debug flags
#THROWBACK = -Throwback
#DEFINES = ${DEBUG} -DCOMPAT_INET4 ${THROWBACK}
DEFINES = ${DEBUG} ${THROWBACK}
COMMONFLAGS = -zM -c -Wp -ws -depend !Depend ${INCLUDES} ${DEFINES}

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = ${COMMONFLAGS}
C++FLAGS  = ${COMMONFLAGS}
Linkflags = -c++ -o $@ -rmf
ObjAsmflags = ${THROWBACK} -depend !Depend
CMHGFLAGS = -p
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
# Include files
#
INCLUDES = -ITCPIPLibs:,C:

TARGET    = rm.${COMPONENT}
TARGETDB  = rm.${DEBUG_COMPONENT}

OBJS      =	\
o.VSPHdr	\
o.main		\
o.Interface	\
o.List		\
o.MessageQ	\
o.Stream	\
o.Source	\
o.SourceMem	\
o.ServCtrl	\
o.ServerATM	\
o.Sink		\
o.Error         \
o.vspSocket	\
o.NECstruct	\
o.htonstruct

d_OBJS      =	\
o.VSPHdr	\
d_o.main	\
d_o.Interface	\
d_o.List	\
d_o.MessageQ	\
d_o.Stream	\
d_o.Source	\
d_o.SourceMem	\
d_o.ServCtrl	\
d_o.ServerATM	\
d_o.Sink	\
d_o.Error       \
d_o.vspSocket	\
d_o.NECstruct	\
d_o.htonstruct

OBJSI     =	\
i.main		\
i.Interface	\
i.List		\
i.MessageQ	\
i.Stream	\
i.Source	\
i.SourceMem	\
i.ServCtrl	\
i.ServerATM	\
i.Sink		\
i.Error         \
i.vspSocket	\
i.htonstruct	\
i.NECstruct

OBJSINST   =	\
oi.VSPHdr	\
oi.main		\
oi.Interface	\
oi.List		\
oi.MessageQ	\
oi.Stream	\
oi.Source	\
oi.SourceMem	\
oi.ServCtrl	\
oi.ServerATM	\
oi.Sink		\
oi.Error        \
oi.vspSocket	\
oi.htonstruct	\
oi.NECstruct	\
oi.instlib

#
# Rule patterns
#
.SUFFIXES:	.d_o .o .oi .s .c .c++ .i .cmhg .inst
.c++.d_o:;  ${C++} ${C++FLAGS} -o $@ $<
.c.d_o:;    ${CC} ${CFLAGS} -o $@ $<
.c++.o:;    ${C++} ${C++FLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c++.i:;    ${CC} ${C++FLAGS} -c -C -E $< > $@
.c.i:;      ${CC} ${CFLAGS} -c -C -E $< > $@
.inst.oi:;  ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.VSPHdr
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET} test local_dirs
        @echo ${COMPONENT}: all complete

debug: ${TARGETDB} test local_dirs
        @echo ${DEBUG_COMPONENT}: all complete

local_dirs:
	${MKDIR} o
	${MKDIR} d_o
	${MKDIR} oi
	${MKDIR} aof
	${MKDIR} rm
	${MKDIR} i
	${MKDIR} inst

preprocess: ${OBJSI} local_dirs
	@echo ${COMPONENT}: preprocess build complete

instrument: ${OBJSINST} local_dirs 
	${LD} -rmf -s link/sym -o ${TARGET}u ${OBJSINST} ${STUBS}
	ModSqz ${TARGET}u ${TARGET}
	@echo ${COMPONENT}: instrument build complete

clean:
        ${WIPE} ${TARGET} ${WFLAGS}
        ${RM} c++.NECstruct
        ${RM} h.VSPHdr
        ${WIPE} o ${WFLAGS}
        ${WIPE} d_o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} oi ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Final targets:
${TARGET}: ${OBJS} o.VSPHdr
        ${LD} ${Linkflags} -s syms ${OBJS} ${COBJ} ${CLIB} ${CPPLIB} ${TCPIPLib}

${TARGETDB}: ${d_OBJS} o.VSPHdr
        ${LD} ${Linkflags} -s syms ${d_OBJS} ${COBJ} ${CLIB} ${CPPLIB} ${TCPIPLib} ${DebugLib}
        
h.VSPHdr:	cmhg.VSPHdr
		${CMHG} ${CMHGFLAGS} -o o.$* $? $@
		@stamp $@

test: testassert
	@|

testassert: testassert.o
	link -o testassert testassert.o c:o.stubs
	run @.testassert

testassert.o: c++.NECstruct
	cc -DTEST -c -o o.testassert c++.NECstruct

o.NECstruct: c++.NECstruct
	${C++} ${C++FLAGS} -o $@ c++.NECstruct

c++.NECstruct: h.NECstruct
	${AWK} -f awk.mapstructs < $? > $@

# Dynamic dependencies:
