
# Project:   VSPHyperms

COMPONENT = VSPHyperms

#
# Paths
#
EXP_HDR = <export$dir>

# Libraries
#
TCPIPLib   = TCPIPLibs:o.unixlibzm TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklibzm
DebugLib   = <Lib$Dir>.DebugLib.o.DebugLib
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
o.List:	c++.List
o.List:	h.List
o.List:	h.Error
o.List:	C:h.kernel
o.List:	h.VSP_types
o.List:	h.VSPHdr
o.List:	C:h.stdio
o.MessageQ:	c++.MessageQ
o.MessageQ:	h.MessageQ
o.MessageQ:	h.Error
o.MessageQ:	C:h.kernel
o.MessageQ:	h.VSP_types
o.MessageQ:	h.VSPHdr
o.MessageQ:	h.VSPHdr
o.MessageQ:	C:h.swis
o.MessageQ:	C:h.string
o.SourceMem:	c++.SourceMem
o.SourceMem:	C:h.kernel
o.SourceMem:	C:h.swis
o.SourceMem:	h.SourceP
o.SourceMem:	h.VSP_types
o.SourceMem:	C:h.kernel
o.SourceMem:	h.SourceMem
o.SourceMem:	h.Error
o.SourceMem:	C:h.kernel
o.SourceMem:	h.VSP_types
o.SourceMem:	h.VSPHdr
o.SourceMem:	h.SourceP
o.SourceMem:	h.NECstruct
o.SourceMem:	h.VSP_types
o.Error:	c++.Error
o.Error:	C:h.kernel
o.Error:	C:h.swis
o.Error:	h.VSPHdr
o.Error:	h.Error
o.Error:	C:h.kernel
o.Error:	h.VSP_types
o.Error:	h.VSPHdr
o.Error:	C:h.stdarg
o.Error:	C:DebugLib.h.DebugLib
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
o.ServCtrl:	c++.ServCtrl
o.ServCtrl:	h.NECstruct
o.ServCtrl:	h.VSP_types
o.ServCtrl:	h.ServCtrl
o.ServCtrl:	h.Error
o.ServCtrl:	C:h.kernel
o.ServCtrl:	h.VSP_types
o.ServCtrl:	h.VSPHdr
o.ServCtrl:	h.List
o.ServCtrl:	h.Error
o.ServCtrl:	h.NECStruct
o.ServCtrl:	h.vspSocket
o.ServCtrl:	TCPIPLibs:h.netdb
o.ServCtrl:	TCPIPLibs:sys.h.cdefs
o.ServCtrl:	TCPIPLibs:sys.h.errno
o.ServCtrl:	C:h.kernel
o.ServCtrl:	TCPIPLibs:h.unixlib
o.ServCtrl:	TCPIPLibs:h.socklib
o.ServCtrl:	TCPIPLibs:sys.h.types
o.ServCtrl:	TCPIPLibs:sys.h.cdefs
o.ServCtrl:	TCPIPLibs:machine.h.endian
o.ServCtrl:	TCPIPLibs:machine.h.ansi
o.ServCtrl:	TCPIPLibs:machine.h.types
o.ServCtrl:	TCPIPLibs:sys.h.socket
o.ServCtrl:	TCPIPLibs:sys.h.cdefs
o.ServCtrl:	TCPIPLibs:sys.h.stat
o.ServCtrl:	TCPIPLibs:sys.h.time
o.ServCtrl:	TCPIPLibs:sys.h.cdefs
o.ServCtrl:	TCPIPLibs:sys.h.cdefs
o.ServCtrl:	TCPIPLibs:sys.h.time
o.ServCtrl:	TCPIPLibs:sys.h.uio
o.ServCtrl:	TCPIPLibs:sys.h.cdefs
o.ServCtrl:	C:h.kernel
o.ServCtrl:	TCPIPLibs:h.inetlib
o.ServCtrl:	TCPIPLibs:sys.h.types
o.ServCtrl:	TCPIPLibs:netinet.h.in
o.ServCtrl:	TCPIPLibs:sys.h.uio
o.ServCtrl:	C:h.swis
o.ServCtrl:	C:h.kernel
o.ServCtrl:	h.MessageQ
o.ServCtrl:	h.Error
o.ServCtrl:	C:h.kernel
o.ServCtrl:	C:h.swis
o.Source:	c++.Source
o.Source:	C:h.kernel
o.Source:	C:h.swis
o.Source:	h.SourceP
o.Source:	h.VSP_types
o.Source:	C:h.kernel
o.Source:	h.Source
o.Source:	h.Error
o.Source:	C:h.kernel
o.Source:	h.VSP_types
o.Source:	h.VSPHdr
o.Source:	h.ServCtrl
o.Source:	h.Error
o.Source:	h.List
o.Source:	h.Error
o.Source:	h.NECStruct
o.Source:	h.VSP_types
o.Source:	h.vspSocket
o.Source:	TCPIPLibs:h.netdb
o.Source:	TCPIPLibs:sys.h.cdefs
o.Source:	TCPIPLibs:sys.h.errno
o.Source:	C:h.kernel
o.Source:	TCPIPLibs:h.unixlib
o.Source:	TCPIPLibs:h.socklib
o.Source:	TCPIPLibs:sys.h.types
o.Source:	TCPIPLibs:sys.h.cdefs
o.Source:	TCPIPLibs:machine.h.endian
o.Source:	TCPIPLibs:machine.h.ansi
o.Source:	TCPIPLibs:machine.h.types
o.Source:	TCPIPLibs:sys.h.socket
o.Source:	TCPIPLibs:sys.h.cdefs
o.Source:	TCPIPLibs:sys.h.stat
o.Source:	TCPIPLibs:sys.h.time
o.Source:	TCPIPLibs:sys.h.cdefs
o.Source:	TCPIPLibs:sys.h.cdefs
o.Source:	TCPIPLibs:sys.h.time
o.Source:	TCPIPLibs:sys.h.uio
o.Source:	TCPIPLibs:sys.h.cdefs
o.Source:	C:h.kernel
o.Source:	TCPIPLibs:h.inetlib
o.Source:	TCPIPLibs:sys.h.types
o.Source:	TCPIPLibs:netinet.h.in
o.Source:	TCPIPLibs:sys.h.uio
o.Source:	C:h.swis
o.Source:	C:h.kernel
o.Source:	h.MessageQ
o.Source:	h.Error
o.Source:	h.vspSocket
o.Source:	h.SourceMem
o.Source:	h.Error
o.Source:	h.SourceP
o.Source:	h.NECstruct
o.Source:	h.NECstruct
o.Source:	TCPIPLibs:sys.h.ioctl
o.Source:	TCPIPLibs:sys.h.ttycom
o.Source:	TCPIPLibs:sys.h.ioccom
o.Source:	TCPIPLibs:sys.h.ioccom
o.Source:	TCPIPLibs:sys.h.filio
o.Source:	TCPIPLibs:sys.h.ioccom
o.Source:	TCPIPLibs:sys.h.sockio
o.Source:	TCPIPLibs:sys.h.ioccom
o.Source:	TCPIPLibs:sys.h.cdefs
o.Source:	TCPIPLibs:netinet.h.udp
o.Stream:	c++.Stream
o.Stream:	h.Stream
o.Stream:	h.Error
o.Stream:	C:h.kernel
o.Stream:	h.VSP_types
o.Stream:	h.VSPHdr
o.Stream:	h.List
o.Stream:	h.Error
o.Stream:	h.ServCtrl
o.Stream:	h.Error
o.Stream:	h.List
o.Stream:	h.NECStruct
o.Stream:	h.VSP_types
o.Stream:	h.vspSocket
o.Stream:	TCPIPLibs:h.netdb
o.Stream:	TCPIPLibs:sys.h.cdefs
o.Stream:	TCPIPLibs:sys.h.errno
o.Stream:	C:h.kernel
o.Stream:	TCPIPLibs:h.unixlib
o.Stream:	TCPIPLibs:h.socklib
o.Stream:	TCPIPLibs:sys.h.types
o.Stream:	TCPIPLibs:sys.h.cdefs
o.Stream:	TCPIPLibs:machine.h.endian
o.Stream:	TCPIPLibs:machine.h.ansi
o.Stream:	TCPIPLibs:machine.h.types
o.Stream:	TCPIPLibs:sys.h.socket
o.Stream:	TCPIPLibs:sys.h.cdefs
o.Stream:	TCPIPLibs:sys.h.stat
o.Stream:	TCPIPLibs:sys.h.time
o.Stream:	TCPIPLibs:sys.h.cdefs
o.Stream:	TCPIPLibs:sys.h.cdefs
o.Stream:	TCPIPLibs:sys.h.time
o.Stream:	TCPIPLibs:sys.h.uio
o.Stream:	TCPIPLibs:sys.h.cdefs
o.Stream:	C:h.kernel
o.Stream:	TCPIPLibs:h.inetlib
o.Stream:	TCPIPLibs:sys.h.types
o.Stream:	TCPIPLibs:netinet.h.in
o.Stream:	TCPIPLibs:sys.h.uio
o.Stream:	C:h.swis
o.Stream:	C:h.kernel
o.Stream:	h.MessageQ
o.Stream:	h.Error
o.Stream:	h.Sink
o.Stream:	h.Error
o.Stream:	h.Source
o.Stream:	h.Error
o.Stream:	h.ServCtrl
o.Stream:	h.vspSocket
o.Stream:	h.SourceMem
o.Stream:	h.Error
o.Stream:	h.SourceP
o.Stream:	h.VSP_types
o.Stream:	C:h.kernel
o.Stream:	h.NECstruct
o.Stream:	h.NECstruct
o.Stream:	C:h.swis
o.Stream:	C:h.string
o.main:	c++.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.VSPHdr
o.main:	h.Interface
o.main:	h.Error
o.main:	C:h.kernel
o.main:	h.VSP_types
o.main:	h.VSPHdr
o.main:	h.Stream
o.main:	h.Error
o.main:	h.List
o.main:	h.Error
o.main:	h.ServCtrl
o.main:	h.Error
o.main:	h.List
o.main:	h.NECStruct
o.main:	h.VSP_types
o.main:	h.vspSocket
o.main:	TCPIPLibs:h.netdb
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:sys.h.errno
o.main:	C:h.kernel
o.main:	TCPIPLibs:h.unixlib
o.main:	TCPIPLibs:h.socklib
o.main:	TCPIPLibs:sys.h.types
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:machine.h.endian
o.main:	TCPIPLibs:machine.h.ansi
o.main:	TCPIPLibs:machine.h.types
o.main:	TCPIPLibs:sys.h.socket
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:sys.h.stat
o.main:	TCPIPLibs:sys.h.time
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	TCPIPLibs:sys.h.time
o.main:	TCPIPLibs:sys.h.uio
o.main:	TCPIPLibs:sys.h.cdefs
o.main:	C:h.kernel
o.main:	TCPIPLibs:h.inetlib
o.main:	TCPIPLibs:sys.h.types
o.main:	TCPIPLibs:netinet.h.in
o.main:	TCPIPLibs:sys.h.uio
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	h.MessageQ
o.main:	h.Error
o.main:	h.Sink
o.main:	h.Error
o.main:	h.Source
o.main:	h.Error
o.main:	h.ServCtrl
o.main:	h.vspSocket
o.main:	h.SourceMem
o.main:	h.Error
o.main:	h.SourceP
o.main:	h.VSP_types
o.main:	C:h.kernel
o.main:	h.NECstruct
o.main:	h.NECstruct
o.main:	C:h.kernel
o.main:	h.Error
o.Interface:	c++.Interface
o.Interface:	C:h.kernel
o.Interface:	C:h.swis
o.Interface:	h.VSPHdr
o.Interface:	h.Interface
o.Interface:	h.Error
o.Interface:	C:h.kernel
o.Interface:	h.VSP_types
o.Interface:	h.VSPHdr
o.Interface:	h.Stream
o.Interface:	h.Error
o.Interface:	h.List
o.Interface:	h.Error
o.Interface:	h.ServCtrl
o.Interface:	h.Error
o.Interface:	h.List
o.Interface:	h.NECStruct
o.Interface:	h.VSP_types
o.Interface:	h.vspSocket
o.Interface:	TCPIPLibs:h.netdb
o.Interface:	TCPIPLibs:sys.h.cdefs
o.Interface:	TCPIPLibs:sys.h.errno
o.Interface:	C:h.kernel
o.Interface:	TCPIPLibs:h.unixlib
o.Interface:	TCPIPLibs:h.socklib
o.Interface:	TCPIPLibs:sys.h.types
o.Interface:	TCPIPLibs:sys.h.cdefs
o.Interface:	TCPIPLibs:machine.h.endian
o.Interface:	TCPIPLibs:machine.h.ansi
o.Interface:	TCPIPLibs:machine.h.types
o.Interface:	TCPIPLibs:sys.h.socket
o.Interface:	TCPIPLibs:sys.h.cdefs
o.Interface:	TCPIPLibs:sys.h.stat
o.Interface:	TCPIPLibs:sys.h.time
o.Interface:	TCPIPLibs:sys.h.cdefs
o.Interface:	TCPIPLibs:sys.h.cdefs
o.Interface:	TCPIPLibs:sys.h.time
o.Interface:	TCPIPLibs:sys.h.uio
o.Interface:	TCPIPLibs:sys.h.cdefs
o.Interface:	C:h.kernel
o.Interface:	TCPIPLibs:h.inetlib
o.Interface:	TCPIPLibs:sys.h.types
o.Interface:	TCPIPLibs:netinet.h.in
o.Interface:	TCPIPLibs:sys.h.uio
o.Interface:	C:h.swis
o.Interface:	C:h.kernel
o.Interface:	h.MessageQ
o.Interface:	h.Error
o.Interface:	h.Sink
o.Interface:	h.Error
o.Interface:	h.Source
o.Interface:	h.Error
o.Interface:	h.ServCtrl
o.Interface:	h.vspSocket
o.Interface:	h.SourceMem
o.Interface:	h.Error
o.Interface:	h.SourceP
o.Interface:	h.VSP_types
o.Interface:	C:h.kernel
o.Interface:	h.NECstruct
o.Interface:	h.NECstruct
o.Interface:	C:h.kernel
o.Interface:	h.ServCtrl
