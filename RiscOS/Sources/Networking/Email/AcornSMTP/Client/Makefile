#	Project Post (769)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.08 for RISC OS
#
#	OVERVIEW
#	========
#	Makefile for AcornSMTP relocatable module
#	
#	HISTORY
#	=======
#	07/02/97	0.01	RDW	Initial version
#	22/09/97	0.02	RDW	Completely replaced with RISC OS build env. makefile
#	07/11/97	0.03	RDW	changed when AcornSMTP code replaced with AcornPOP3 code base
#	17/12/97	0.04	RDW	changed so it exports correctly and uses callx from the Lib$Dir
#					Changes from BAL.
#	27/01/98	0.05	RDW	Support added to output preprocessed source for McCabe tool
#	17/03/98	0.06	RDW	Now creates and deletes dirs. aof, o and rm
#	18/03/98	0.07	RDW	Forgot to add local_dir dependancy to export rules.
#					-ffah added back to CFLAGS.  Got lost at some point in the past
#	06/04/98	0.08	RDW	instrument target added and OBJSCI for building instrumented
#					version
#					Tidy up MemCheck build so it can be done cleaning from amu
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
COMPONENT = AcornSMTP
TARGET = rm.AcornSMTP
ROMTARGET = aof.AcornSMTP


STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

COMPAT=		-DCOMPAT_INET4

CFLAGS=		-ffah -zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -Wp -depend !Depend -Fn -c -throwback ${TINYSTUBS}\
		${EXTRACCFLAGS}

CMHGFLAGS = -p -IC: -d h.header ${THROWBACK}

ASMFLAGS=       

OBJS=		swihndlers.o handlers.o smtp_utils.o module.o sendmail.o header.o \
                errortable.o get.o sendline.o send.o cmdresp.o \
                ${LIBS}

OBJSCP=		cp.swihndlers cp.handlers cp.smtp_utils cp.module cp.sendmail o.header \
                cp.errortable cp.get cp.sendline cp.send cp.cmdresp \
                ${LIBS}
                
OBJSCI=		o.swihndlers o.handlers o.smtp_utils o.module o.sendmail o.header \
                o.errortable o.get o.sendline o.send o.cmdresp \
                o.instlib \
                ${LIBS}

EXTRAS=         acornsmtp.o

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
	$(LD) -rmf -s link/sym -o rm.AcornSMTPu $(OBJSCI) $(STUBS)
	ModSqz rm.AcornSMTPu rm.AcornSMTP

export:	export_${PHASE}

export_hdrs: h.AcornSMTP local_dirs
	${CP}	h.AcornSMTP	${EXP_C_H}.AcornSMTP	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: o.AcornSMTP local_dirs
	${CP}	o.AcornSMTP	${EXP_C_O}.AcornSMTP	${CPFLAGS}
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
		$(LD) -rmf -s link/sym -o rm.AcornSMTPu $(OBJS) $(STUBS)
		ModSqz rm.AcornSMTPu $@

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
