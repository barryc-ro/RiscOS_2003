#	Project Post (769)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.14 for RISC OS
#
#	OVERVIEW
#	========
#	Makefile for AcornPOP3 relocatable module
#	
#	HISTORY
#	=======
#	19/02/97	0.02	RDW	Initial version
#	03/07/97	0.03	RDW	EXTRAS acornpop3.o added
#	11/07/97	0.04	RDW	Completely replaced with RISC OS build env. makefile
#	08/10/97	0.05	RDW	get.o added callbacks.o added
#	17/12/97	0.06	RDW	changes from BAL put in
#	27/01/98	0.07	RDW	Support added to output preprocessed source for McCabe tool
#	12/03/98	0.08	RDW	elib_priv obect removed, replaced with EmailLib
#	17/03/98	0.09	RDW	instrument target added and OBJSCI for building instrumented
#					version
#					-ffah added back to CFLAGS.  Got lost at some point in the past
#	25/03/98	0.10	RDW	Tidy up MemCheck build so it can be done cleaning from amu
#					command line.  DEBUG def. removed so it's now defined on amu
#					command line.  -C++ option added to .cp.o suffix rule to get
#					round fault in CC.  CC takes cp.filename and trys to use
#					cp.c.filename!
#	24/08/98	0.11	RDW	System build environment def. passed to source now for
#					errortable.c to select correct report string
#					analyse target (McCabe) changed to preprocess.  OBJSCP now OBJSI
#					OBJSCI now OBJSINST.  .i.o rule changed to .inst.o  OBJSINST
#					changed from o.xxx to inst.xxx
#	17/09/98	0.12	RDW	-Fn removed from CFLAGS
#	18/11/98	0.13	RDW	queue.o added to OBJS
#	23/02/99	0.14	RDW	reset.o added to OBJS
#					added s option to -W flags on CFLAGS and C++FLAGS

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
LIBS      = ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${CALLX} ${DOBJS} ${EMAILLIB} ${C++LIB}z ${EXTRALIBS}

#
# Program specific options:
#
COMPONENT = AcornPOP3
TARGET = rm.AcornPOP3
ROMTARGET = aof.AcornPOP3


STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

COMPAT=		-DCOMPAT_INET4

CFLAGS=		-ffah -zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -Wps -depend !Depend -c ${THROWBACK} \
		-D${SYSTEM} ${EXTRACCFLAGS}
C++FLAGS=	-ffa -zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -Wps -depend !Depend -c ${THROWBACK} \
		-D${SYSTEM} ${EXTRAC++FLAGS}


CMHGFLAGS = -p -IC: -d h.header ${THROWBACK}

ASMFLAGS=       

OBJS=		swihndlers.o handlers.o logon.o logout.o pop_utils.o module.o header.o \
                errortable.o get.o send.o cmdresp.o getnummsgs.o \
                getlistmsg.o getmsgfile.o deletemsg.o getuniquei.o getlistuid.o queue.o \
		reset.o ${LIBS}
		
OBJSI=		i.swihndlers i.handlers i.logon i.logout i.pop_utils i.module o.header \
                i.errortable i.get i.send i.cmdresp i.getnummsgs \
                i.getlistmsg i.getmsgfile i.deletemsg i.getuniquei i.getlistuid i.queue \
		i.reset ${LIBS}

OBJSINST=	inst.swihndlers inst.handlers inst.logon inst.logout inst.pop_utils inst.module \
		o.header \
                inst.errortable inst.get inst.send inst.cmdresp inst.getnummsgs \
                inst.getlistmsg inst.getmsgfile inst.deletemsg inst.getuniquei inst.getlistuid \
		inst.queue inst.reset \
		${LIBS}

EXTRAS=         acornpop3.o

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
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

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
	$(LD) -c++ -rmf -s link/sym -o rm.AcornPOP3u $(OBJSINST) inst.instlib $(STUBS)
	ModSqz rm.AcornPOP3u rm.AcornPOP3
	@echo ${COMPONENT}: instrument build complete

export:	export_${PHASE}

export_hdrs: h.AcornPOP3 local_dirs
	${CP}	h.AcornPOP3	${EXP_C_H}.AcornPOP3	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: o.AcornPOP3 local_dirs
	${CP}	o.AcornPOP3	${EXP_C_O}.AcornPOP3	${CPFLAGS}
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
		$(LD) -c++ -rmf -s link/sym -o rm.AcornPOP3u $(OBJS) $(STUBS)
		ModSqz rm.AcornPOP3u $@

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
