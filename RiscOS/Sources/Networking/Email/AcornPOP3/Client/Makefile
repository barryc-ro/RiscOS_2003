#	Project Post (769)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.09 for RISC OS
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
INCLUDES=	TCPIPLibs:,C:,<Lib$Dir>

INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm
CALLX     = <Lib$Dir>.callx.o.callx
EMAILLIB  = <Lib$Dir>.Email.EmailLib.o.EmailLibzm
LIBS      = ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${CALLX} ${DOBJS} ${EMAILLIB} ${EXTRALIBS}

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
		-zps1 -Wp -depend !Depend -Fn -c ${THROWBACK} \
		${EXTRACCFLAGS}

CMHGFLAGS = -p -IC: -d h.header ${THROWBACK}

ASMFLAGS=       

OBJS=		swihndlers.o handlers.o logon.o logout.o pop_utils.o module.o header.o \
                errortable.o get.o send.o cmdresp.o callbacks.o getnummsgs.o \
                getlistmsg.o getmsgfile.o deletemsg.o getuniquei.o getlistuid.o \
		${LIBS}
		
OBJSCP=		cp.swihndlers cp.handlers cp.logon cp.logout cp.pop_utils cp.module o.header \
                cp.errortable cp.get cp.send cp.cmdresp cp.callbacks cp.getnummsgs \
                cp.getlistmsg cp.getmsgfile cp.deletemsg cp.getuniquei cp.getlistuid \
		${LIBS}

OBJSCI=		o.swihndlers o.handlers o.logon o.logout o.pop_utils o.module o.header \
                o.errortable o.get o.send o.cmdresp o.callbacks o.getnummsgs \
                o.getlistmsg o.getmsgfile o.deletemsg o.getuniquei o.getlistuid \
                o.instlib \
		${LIBS}

EXTRAS=         acornpop3.o

#
# Rule patterns
#
.SUFFIXES:	.o .s .c .cp .cmhg
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.c.cp:;		$(CC) $(CFLAGS) -c -C -E $< >> $@
.cp.o:;		$(CC) $(CFLAGS) -C++ -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

analyse: ${OBJSCP} local_dirs
	@echo ${COMPONENT}: analysis build complete

instrument: ${OBJSCI} local_dirs
	$(LD) -rmf -s link/sym -o rm.AcornPOP3u $(OBJSCI) $(STUBS)
	ModSqz rm.AcornPOP3u rm.AcornPOP3

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
		$(WIPE) cp.*      $(WFLAGS)
                $(WIPE) rm.*     $(WFLAGS)
                $(WIPE) aof.*    $(WFLAGS)
		$(RM) link/sym
		$(RM) rm
		$(RM) aof
		$(RM) linked
		$(RM) map
		$(RM) cp
		$(RM) o

local_dirs:
	${MKDIR} o
	${MKDIR} aof
	${MKDIR} rm
	${MKDIR} cp

#
# Target 
#
${TARGET}:	$(OBJS) $(EXTRAS) local_dirs
		$(LD) -rmf -s link/sym -o rm.AcornPOP3u $(OBJS) $(STUBS)
		ModSqz rm.AcornPOP3u $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} $(EXTRAS) local_dirs
        ${LD} -s link/sym -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Some rules that need explicitly declaring
#


# Dynamic dependencies:
