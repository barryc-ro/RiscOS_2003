#	Project Post (769)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.06 for RISC OS
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
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir
MemCheckCC	= #MemCheck.CC
MemCheckLD	= #MemCheck.DDTModLink

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

#
# Program specific options:
#
COMPONENT = AcornPOP3
TARGET = rm.AcornPOP3
ROMTARGET = aof.AcornPOP3


OTHERCCDEFS=	#-g

# You may want to link with TinyStubs. if you do, set LD to tlink and
# uncomment this TINYSTUBS, and comment out the STUBS

TINYSTUBS=	#-DTINYSTUBS
STUBS=		C:o.Stubs

ROMCSTUBS=      RISC_OSLib:o.romcstubs
ABSSYM=         RISC_OSLib:o.AbsSym

DEBUG=		#-DMEMCHECK #-DDEBUG -DTRACE

COMPAT=		-DCOMPAT_INET4

# CFLAGS=		-fnah -zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}
CFLAGS=		-zM -I${INCLUDES} -DINET -DCOMPAT_INET4 ${DEBUG}\
		-zps1 -Wp -depend !Depend -Fn -c -throwback ${TINYSTUBS}\
		${OTHERCCDEFS}

CMHGFLAGS = -p -IC: -d h.header -throwback

ASMFLAGS=       

DOBJS=          #MemCheck:o.MC_M #tml_podule.o debuglib.o trace_PL.o

OBJS=		swihndlers.o handlers.o logon.o logout.o pop_utils.o module.o header.o \
                errortable.o get.o send.o cmdresp.o callbacks.o elib_priv.o getnummsgs.o \
                getlistmsg.o getmsgfile.o deletemsg.o getuniquei.o getlistuid.o \
		${SOCKLIB} ${INETLIB} ${UNIXLIB} ${CALLX} ${DOBJS}

EXTRAS=         acornpop3.o

#
# Rule patterns
#
.SUFFIXES:	.o .s .c
.c.o:;		$(MemCheckCC) $(CC) $(CFLAGS) -o $@ $<
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export:	export_${PHASE}

export_hdrs: h.AcornPOP3
	${CP}	h.AcornPOP3	${EXP_C_H}.AcornPOP3	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: o.AcornPOP3
	${CP}	o.AcornPOP3	${EXP_C_O}.AcornPOP3	${CPFLAGS}
	@echo ${COMPONENT}: export complete (libs)

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
		$(WIPE) o.*      $(WFLAGS)
                $(WIPE) rm.*     $(WFLAGS)
                $(WIPE) aof.*    $(WFLAGS)
		$(RM) link/sym

#
# Target 
#
${TARGET}:	$(OBJS) $(EXTRAS)
		$(MemCheckLD) $(LD) -rmf -s link/sym -o rm.AcornPOP3u $(OBJS) $(STUBS)
		ModSqz rm.AcornPOP3u $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS}
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
