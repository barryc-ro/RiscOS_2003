# Makefile for SendMessage module.
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name    Description
# ----         ----    -----------
# 15-Oct-2001  SAR     Created.

COMPONENT = SendMessage
TARGET    = SendMsg
OBJS      = ModHdr.o Module.o Message.o MsgRes.o
LIBS      = TCPIPLibs:o.inetlibzm TCPIPLIBS:o.socklib5zm TCPIPLIBS:o.unixlib
DIRS      = o._dirs
CFLAGS    = -ffah -Wp -zM -iC:,TCPIPLibs:

include Makefiles:StdTools
include Makefiles:ModuleLibs
include Makefiles:ModStdRule
include Makefiles:RAMCModule
include Makefiles:ROMCModule

Module.o: Module.c ModHdr.h
        ${CC} ${CFLAGS} -o $@ Module.c

Message.o: Message.c
        ${CC} ${CFLAGS} -o $@ Message.c

MsgRes.o: LocalUserIFRes:Messages
	TokenCheck LocalUserIFRes:Messages
	resgen messages_file o.MsgRes LocalUserIFRes:Messages Resources.${COMPONENT}.Messages

o._dirs:
        ${MKDIR} o
        ${MKDIR} h
        ${TOUCH} o._dirs

clean:
        ${RM} h.ModHdr
        IfThere aof    Then Wipe aof ${WFLAGS}
        IfThere o      Then Wipe o ${WFLAGS}
        IfThere linked Then Wipe linked ${WFLAGS}
        IfThere rm     Then Wipe rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned


# Dynamic dependencies:
