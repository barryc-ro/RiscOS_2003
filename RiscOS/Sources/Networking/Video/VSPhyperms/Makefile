
# Project:   VSPHyperms

COMPONENT = VSPHyperms

#
# Paths
#
EXP_HDR = <export$dir>

# Libraries
#
TCPIPLib   = TCPIPLibs:o.unixlibzm TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklibzm
DebugLib   = 
#<Lib$Dir>.DebugLib.o.DebugLib
CLIB       = CLIB:o.stubs
CPPLIB	   = CLIB:o.c++lib
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
THROWBACK = -Throwback
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
.SUFFIXES:	.o .oi .s .c .c++ .i .cmhg .inst
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

local_dirs:
	${MKDIR} o
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
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} oi ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Final targets:
${TARGET}: ${OBJS} o.VSPHdr
        ${LD} ${Linkflags} -s syms ${OBJS} ${COBJ} ${CLIB} ${CPPLIB} ${TCPIPLib} ${DebugLib}
        
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
o.Sink:	c++.Sink
o.Sink:	h.Sink
o.Sink:	h.Error
o.Sink:	C:h.kernel
o.Sink:	h.VSP_types
o.Sink:	h.VSPHdr
o.Sink:	C:h.stdio
o.Sink:	C:h.swis
o.Sink:	h.vspSocket
o.Sink:	TCPIPLibs:h.netdb
o.Sink:	TCPIPLibs:sys.h.cdefs
o.Sink:	TCPIPLibs:sys.h.errno
o.Sink:	C:h.kernel
o.Sink:	TCPIPLibs:h.unixlib
o.Sink:	TCPIPLibs:h.socklib
o.Sink:	TCPIPLibs:sys.h.types
o.Sink:	TCPIPLibs:sys.h.cdefs
o.Sink:	TCPIPLibs:machine.h.endian
o.Sink:	TCPIPLibs:machine.h.ansi
o.Sink:	TCPIPLibs:machine.h.types
o.Sink:	TCPIPLibs:sys.h.socket
o.Sink:	TCPIPLibs:sys.h.cdefs
o.Sink:	TCPIPLibs:sys.h.stat
o.Sink:	TCPIPLibs:sys.h.time
o.Sink:	TCPIPLibs:sys.h.cdefs
o.Sink:	TCPIPLibs:sys.h.cdefs
o.Sink:	TCPIPLibs:sys.h.time
o.Sink:	TCPIPLibs:sys.h.uio
o.Sink:	TCPIPLibs:sys.h.cdefs
o.Sink:	C:h.kernel
o.Sink:	TCPIPLibs:h.inetlib
o.Sink:	TCPIPLibs:sys.h.types
o.Sink:	TCPIPLibs:netinet.h.in
o.Sink:	TCPIPLibs:sys.h.uio
o.Sink:	C:h.swis
o.Sink:	C:h.kernel
o.Error:	c++.Error
o.Error:	C:h.kernel
o.Error:	C:h.swis
o.Error:	h.VSPHdr
o.Error:	h.Error
o.Error:	C:h.kernel
o.Error:	h.VSP_types
o.Error:	h.VSPHdr
o.vspSocket:	c++.vspSocket
o.vspSocket:	h.vspSocket
o.vspSocket:	TCPIPLibs:h.netdb
o.vspSocket:	TCPIPLibs:sys.h.cdefs
o.vspSocket:	TCPIPLibs:sys.h.errno
o.vspSocket:	C:h.kernel
o.vspSocket:	TCPIPLibs:h.unixlib
o.vspSocket:	TCPIPLibs:h.socklib
o.vspSocket:	TCPIPLibs:sys.h.types
o.vspSocket:	TCPIPLibs:sys.h.cdefs
o.vspSocket:	TCPIPLibs:machine.h.endian
o.vspSocket:	TCPIPLibs:machine.h.ansi
o.vspSocket:	TCPIPLibs:machine.h.types
o.vspSocket:	TCPIPLibs:sys.h.socket
o.vspSocket:	TCPIPLibs:sys.h.cdefs
o.vspSocket:	TCPIPLibs:sys.h.stat
o.vspSocket:	TCPIPLibs:sys.h.time
o.vspSocket:	TCPIPLibs:sys.h.cdefs
o.vspSocket:	TCPIPLibs:sys.h.cdefs
o.vspSocket:	TCPIPLibs:sys.h.time
o.vspSocket:	TCPIPLibs:sys.h.uio
o.vspSocket:	TCPIPLibs:sys.h.cdefs
o.vspSocket:	C:h.kernel
o.vspSocket:	TCPIPLibs:h.inetlib
o.vspSocket:	TCPIPLibs:sys.h.types
o.vspSocket:	TCPIPLibs:netinet.h.in
o.vspSocket:	TCPIPLibs:sys.h.uio
o.vspSocket:	C:h.swis
o.vspSocket:	C:h.kernel
o.vspSocket:	TCPIPLibs:arpa.h.inet
o.vspSocket:	TCPIPLibs:sys.h.cdefs
o.vspSocket:	TCPIPLibs:sys.h.ioctl
o.vspSocket:	TCPIPLibs:sys.h.ttycom
o.vspSocket:	TCPIPLibs:sys.h.ioccom
o.vspSocket:	TCPIPLibs:sys.h.ioccom
o.vspSocket:	TCPIPLibs:sys.h.filio
o.vspSocket:	TCPIPLibs:sys.h.ioccom
o.vspSocket:	TCPIPLibs:sys.h.sockio
o.vspSocket:	TCPIPLibs:sys.h.ioccom
o.vspSocket:	TCPIPLibs:sys.h.cdefs
o.vspSocket:	TCPIPLibs:sys.h.sysctl
o.vspSocket:	TCPIPLibs:netinet.h.in_systm
o.vspSocket:	TCPIPLibs:netinet.h.ip
o.vspSocket:	TCPIPLibs:netinet.h.ip_var
o.vspSocket:	TCPIPLibs:netinet.h.udp
o.vspSocket:	TCPIPLibs:netinet.h.udp_var
o.vspSocket:	TCPIPLibs:netinet.h.tcp_timer
o.vspSocket:	TCPIPLibs:netinet.h.tcp
o.vspSocket:	TCPIPLibs:netinet.h.tcp_var
o.NECstruct:	c++.NECstruct
o.NECstruct:	h.NECstruct
o.NECstruct:	h.VSP_types
o.htonstruct:	c++.htonstruct
o.htonstruct:	h.NECstruct
o.htonstruct:	h.VSP_types
o.htonstruct:	h.Error
o.htonstruct:	C:h.kernel
o.htonstruct:	h.VSP_types
o.htonstruct:	h.VSPHdr
o.htonstruct:	TCPIPLibs:sys.h.types
o.htonstruct:	TCPIPLibs:sys.h.cdefs
o.htonstruct:	TCPIPLibs:machine.h.endian
o.htonstruct:	TCPIPLibs:machine.h.ansi
o.htonstruct:	TCPIPLibs:machine.h.types
