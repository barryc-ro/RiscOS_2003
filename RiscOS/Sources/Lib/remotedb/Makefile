# Makefile for remotedb
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 24 Apr 1997  RWB 	  Created
#

#
# Paths
#
#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
LIBFILE = libfile

SOCKLIBZM = TCPIPLibs:o.socklibzm
SOCKLIB   = TCPIPLibs:o.socklib
UNIXLIBZM = TCPIPLibs:o.unixlibzm
UNIXLIB   = TCPIPLibs:o.unixlib
INETLIBZM = TCPIPLibs:o.inetlibzm
INETLIB   = TCPIPLibs:o.inetlib

DEBUGLIB = <Lib$Dir>.debug

CFLAGS   = -c -depend !Depend -Wap -zps0 ${INCLUDES} -fah -Fn ${DFLAGS}
CFLAGSZM = -c -depend !Depend -Wap -zps0 ${INCLUDES} -fah -Fn ${DFLAGS} -zM
CPFLAGS  = ~cfr~v
WFLAGS   = ~c~v

CCZM=		${CC} ${CFLAGSZM}

#
# Libraries
#
CLIB     = CLIB:o.stubs
#
# Include files
#
INCLUDES = -ITCPIPLibs:,C:
DFLAGS   = -DCOMPAT_INET4

# Program specific options:
#
COMPONENT   = remote
COMPONENTZM = remotezm
LCOMPONENT  = lremote
LCOMPONENTZM= lremotezm

OBJ      = o.${COMPONENT}
OBJZM    = z.${COMPONENT}

#
# Rule patterns
#
.SUFFIXES: .z .o .c .s
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.z:;      ${CCZM} -c -o $@ $<

#
# Build
#
all: ${COMPONENT} ${COMPONENTZM} ${LCOMPONENT} ${LCOMPONENTZM} local_dirs
        @echo ${COMPONENT}: all complete

${COMPONENT}: ${OBJ}
	${LD} -o ${COMPONENT}   -aof ${OBJ} ${SOCKLIB} ${UNIXLIB} ${INETLIB}

${COMPONENTZM}: ${OBJZM}
	${LD} -o ${COMPONENTZM} -aof ${OBJZM} ${SOCKLIBZM} ${UNIXLIBZM} ${INETLIBZM}

${LCOMPONENT}: ${OBJ}
	${LIBFILE} -o ${LCOMPONENT}   -c ${OBJ}

${LCOMPONENTZM}: ${OBJZM}
	${LIBFILE} -o ${LCOMPONENTZM} -c ${OBJZM}

local_dirs:
        ${MKDIR} o
        ${MKDIR} z

clean:
	${WIPE} o.${COMPONENT} ${WFLAGS}
	${WIPE} z.${COMPONENT} ${WFLAGS}
	${WIPE} ${COMPONENT} ${WFLAGS}
	${WIPE} ${COMPONENTZM} ${WFLAGS}
	${WIPE} ${LCOMPONENT} ${WFLAGS}
	${WIPE} ${LCOMPONENTZM} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

export: export_${PHASE}

export_hdrs:
	${MKDIR} ${DEBUGLIB}
	${MKDIR} ${DEBUGLIB}.h
        ${CP} h.${COMPONENT} ${DEBUGLIB}.h.${COMPONENT}   ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${COMPONENT} ${COMPONENTZM} ${LCOMPONENT} ${LCOMPONENTZM} local_dirs
	${MKDIR} ${DEBUGLIB}
	${MKDIR} ${DEBUGLIB}.o
        ${CP} ${COMPONENT}   ${DEBUGLIB}.o.${COMPONENT}   ${CPFLAGS}
        ${CP} ${COMPONENTZM} ${DEBUGLIB}.o.${COMPONENTZM} ${CPFLAGS}
        ${CP} ${LCOMPONENT}   ${DEBUGLIB}.o.${LCOMPONENT}   ${CPFLAGS}
        ${CP} ${LCOMPONENTZM} ${DEBUGLIB}.o.${LCOMPONENTZM} ${CPFLAGS}


#
# Dynamic dependencies:
o.remote:	c.remote
o.remote:	TCPIPLibs:sys.h.types
o.remote:	TCPIPLibs:sys.h.cdefs
o.remote:	TCPIPLibs:machine.h.endian
o.remote:	TCPIPLibs:machine.h.ansi
o.remote:	TCPIPLibs:machine.h.types
o.remote:	TCPIPLibs:sys.h.socket
o.remote:	TCPIPLibs:sys.h.cdefs
o.remote:	TCPIPLibs:sys.h.time
o.remote:	TCPIPLibs:sys.h.cdefs
o.remote:	TCPIPLibs:h.netdb
o.remote:	TCPIPLibs:sys.h.cdefs
o.remote:	TCPIPLibs:netinet.h.in
o.remote:	TCPIPLibs:h.socklib
o.remote:	TCPIPLibs:sys.h.types
o.remote:	TCPIPLibs:sys.h.socket
o.remote:	TCPIPLibs:sys.h.stat
o.remote:	TCPIPLibs:sys.h.time
o.remote:	TCPIPLibs:sys.h.cdefs
o.remote:	TCPIPLibs:sys.h.time
o.remote:	TCPIPLibs:sys.h.uio
o.remote:	TCPIPLibs:sys.h.cdefs
o.remote:	C:h.kernel
o.remote:	h.remote
z.remote:	c.remote
z.remote:	TCPIPLibs:sys.h.types
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:machine.h.endian
z.remote:	TCPIPLibs:machine.h.ansi
z.remote:	TCPIPLibs:machine.h.types
z.remote:	TCPIPLibs:sys.h.socket
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:sys.h.time
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:h.netdb
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:netinet.h.in
z.remote:	TCPIPLibs:h.socklib
z.remote:	TCPIPLibs:sys.h.types
z.remote:	TCPIPLibs:sys.h.socket
z.remote:	TCPIPLibs:sys.h.stat
z.remote:	TCPIPLibs:sys.h.time
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:sys.h.time
z.remote:	TCPIPLibs:sys.h.uio
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	C:h.kernel
z.remote:	h.remote
z.remote:	c.remote
z.remote:	TCPIPLibs:sys.h.types
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:machine.h.endian
z.remote:	TCPIPLibs:machine.h.ansi
z.remote:	TCPIPLibs:machine.h.types
z.remote:	TCPIPLibs:sys.h.socket
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:sys.h.time
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:h.netdb
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:netinet.h.in
z.remote:	TCPIPLibs:h.socklib
z.remote:	TCPIPLibs:sys.h.types
z.remote:	TCPIPLibs:sys.h.socket
z.remote:	TCPIPLibs:sys.h.stat
z.remote:	TCPIPLibs:sys.h.time
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	TCPIPLibs:sys.h.time
z.remote:	TCPIPLibs:sys.h.uio
z.remote:	TCPIPLibs:sys.h.cdefs
z.remote:	C:h.kernel
z.remote:	h.remote
