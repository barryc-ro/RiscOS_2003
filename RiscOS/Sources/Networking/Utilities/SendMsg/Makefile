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
LIBS      = TCPIPLibs:o.inetlibzm TCPIPLIBS:o.socklibzm TCPIPLIBS:o.unixlib
DIRS      = local_dirs
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

local_dirs:
        ${MKDIR} o
        ${MKDIR} h

clean:
        ${RM} h.ModHdr
        IfThere aof    Then ${WIPE} aof ${WFLAGS}
        IfThere o      Then ${WIPE} o ${WFLAGS}
        IfThere linked Then ${WIPE} linked ${WFLAGS}
        IfThere rm     Then ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned


# Dynamic dependencies:
