COMPONENT	= TServer

RAM_OBJS	=	\
TServer.o		\
modhead.o

RAM_LIBS	=	\
c:o.stubs		\
tbox:o.eventlibm 	\
tbox:o.toolboxlibs 	\
tbox:o.wimplib 		\
c:debuglib.o.debuglibzm	\
tcpiplibs:o.unixlibzm	\
tcpiplibs:o.socklib5zm	\
tcpiplibs:o.inetlibzm	\

CFLAGS 		+= -zM -wp -ffah
CINCLUDES	= -Itbox:,c:tboxlibint,C:,TCPIPLibs:,Unixlib:
CMHGINCLUDES	= -IC:
#CDEFINES	= -DDEBUGLIB

include Makefiles:StdTools
include Makefiles:ModStdRule
include Makefiles:RAMCMODULE

all: o._dirs

clean:
	${WIPE} o ${WFLAGS} 
	${RM} h.modhead
	${WIPE} rm ${WFLAGS}

o._dirs:
	${MKDIR} o
	${MKDIR} h
	${MKDIR} rm
	${TOUCH} o._dirs

TServer.o:	modhead.h

# Static dependancies

# Dynamic dependencies:
o.modhead: cmhg.modhead C:Global.h.RISCOS C:Global.h.Services
o.modhead: cmhg.modhead C:Global.h.RISCOS C:Global.h.Services
o.cmodule: c.cmodule h.TServer h.modhead C:h.kernel TCPIPLibs:sys.h.types TCPIPLibs:sys.h.cdefs TCPIPLibs:machine.h.endian TCPIPLibs:machine.h.ansi TCPIPLibs:machine.h.types TCPIPLibs:sys.h.socket TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.ioctl TCPIPLibs:sys.h.ttycom TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.filio TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.sockio TCPIPLibs:sys.h.ioccom TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.errno C:h.kernel TCPIPLibs:netinet.h.in TCPIPLibs:h.netdb TCPIPLibs:sys.h.cdefs TCPIPLibs:h.unixlib TCPIPLibs:h.socklib TCPIPLibs:sys.h.types TCPIPLibs:sys.h.socket TCPIPLibs:sys.h.stat TCPIPLibs:sys.h.time TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.cdefs TCPIPLibs:sys.h.time TCPIPLibs:sys.h.uio TCPIPLibs:sys.h.cdefs C:h.kernel TCPIPLibs:arpa.h.telnet C:Global.h.RISCOS tbox:h.event tbox:h.wimp tbox:h.toolbox tbox:h.wimp tbox:h.wimplib C:h.swis C:DebugLib.h.DebugLib
