#	Project Jedi (003)
#	Element 14 Ltd.
#	Copyright © 1999. All Rights Reserved.
#
#	FILE:		Makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.01 for RISC OS
#
#	OVERVIEW
#	========
#	Makefile for relocatable module
#	
#	HISTORY
#	=======
#	1999-01-20	0.01	RDW	Initial version

#
# Paths
#
EXP_C_H = <CExport$dir>.h
EXP_C_O = <CExport$dir>.o
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm -tb
CC      = $(MemCheckCC) cc
C++	= $(MemCheckCC) c++
CMHG    = cmhg
CP      = copy
LD      = $(MemCheckLD) link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

#
# Include files
#
INCLUDES=	TCPIPLibs:,C:,<Lib$Dir>.C++Lib,<Lib$Dir>

INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm
CALLX     = <Lib$Dir>.callx.o.callx
EMAILLIB  = <Lib$Dir>.Email.EmailLib.o.EmailLibzm
C++LIB    = <Lib$Dir>.C++Lib.o.c++lib
LIBS      = ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${CALLX} ${DOBJS} ${EXTRALIBS}

#
# Program specific options:
#
COMPONENT = Net2TransportStreamer
TARGET = rm.Net2TS
ROMTARGET = aof.Net2TS


STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

COMPAT=		-DCOMPAT_INET4

CFLAGS=		-ffah -zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -Wp -Ws -depend !Depend -c ${THROWBACK} \
		-D${SYSTEM} ${EXTRACCFLAGS}
C++FLAGS=	-ffa -zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -Wp -depend !Depend -c ${THROWBACK} \
		-D${SYSTEM} ${EXTRAC++FLAGS}


CMHGFLAGS = -p -IC: -d h.header ${THROWBACK}

ASMFLAGS=       

OBJS=		handlers.o module.o socket.o swihndlers.o errortable.o header.o tstreamer.o \
		timeout.o ${LIBS}
		
OBJSI=		i.handlers i.module i.socket i.swihndlers errortable.o header.o tstreamer.o \
		o.timeout ${LIBS}

OBJSINST=	inst.handlers inst.module inst.socket inst.swihndlers inst.errortable header.o \
		inst.tstreamer.o inst.timeout ${LIBS}

#
# Rule patterns
#
.SUFFIXES:	.o .s .c .c++ .i .cmhg .inst
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.c++.o:;	$(C++) $(C++FLAGS) -o $@ $<
.c++.i:;	$(C++) $(C++FLAGS) -c -C -E $< >> $@
.c.i:;		$(CC) $(CFLAGS) -c -C -E $< >> $@
.i.inst:;	$(CC) $(CFLAGS) -C++ -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) ${DEBUG} -o $@ $<

all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

preprocess: ${OBJSI} local_dirs
	@echo ${COMPONENT}: preprocess build complete

instrument: ${OBJSINST} inst.instlib local_dirs 
	$(LD) -c++ -rmf -s link/sym -o rm.Net2TSu $(OBJSINST) inst.instlib $(STUBS)
	ModSqz rm.Net2TSu rm.Net2TS
	@echo ${COMPONENT}: instrument build complete

export:	export_${PHASE}

export_hdrs: local_dirs
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: local_dirs
	@echo ${COMPONENT}: export complete (libs)

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
		$(WIPE) o.*      $(WFLAGS)
		$(WIPE) i.*     $(WFLAGS)
		$(WIPE) inst.*   $(WFLAGS)
                $(WIPE) rm.*     $(WFLAGS)
                $(WIPE) aof.*    $(WFLAGS)
		$(RM) link/sym
		$(RM) rm
		$(RM) aof
		$(RM) linked
		$(RM) map
		$(RM) i
		$(RM) inst
		$(RM) o

local_dirs:
	${MKDIR} o
	${MKDIR} aof
	${MKDIR} rm
	${MKDIR} i
	${MKDIR} inst

#
# Target 
#
${TARGET}:	$(OBJS) $(EXTRAS) local_dirs
		$(LD) -c++ -rmf -s link/sym -o rm.Net2TSu $(OBJS) $(STUBS)
		ModSqz rm.Net2TSu $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} $(EXTRAS) local_dirs
        ${LD} -c++ -s link/sym -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -c++ -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Some rules that need explicitly declaring
#


# Dynamic dependencies:
