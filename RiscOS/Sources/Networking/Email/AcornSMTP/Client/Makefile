#	Project Post (769)
#	Acorn Risc Technologies
#	Copyright © 1997. All Rights Reserved.
#
#	FILE:		makefile
#	AUTHOR:		Richard D. Warren
#	VERSION:	0.05 for RISC OS
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
COMPONENT = AcornSMTP
TARGET = rm.AcornSMTP
ROMTARGET = aof.AcornSMTP


OTHERDEFS=	#-g


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
		${OTHERDEFS}

CMHGFLAGS = -p -IC: -d h.header -throwback

ASMFLAGS=       

DOBJS=          #MemCheck:o.MC_Mtml_podule.o debuglib.o trace_PL.o 

OBJS=		swihndlers.o handlers.o smtp_utils.o module.o sendmail.o header.o \
                errortable.o get.o sendline.o send.o cmdresp.o elib_priv.o \
                ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${CALLX} ${DOBJS}

OBJSCP=		cp.swihndlers cp.handlers cp.smtp_utils cp.module cp.sendmail o.header \
                cp.errortable cp.get cp.sendline cp.send cp.cmdresp cp.elib_priv \
                ${SOCKLIB} ${INETLIB} ${UNIXLIB} ${CALLX} ${DOBJS}

EXTRAS=         acornsmtp.o

#
# Rule patterns
#
.SUFFIXES:	.o .s .c .cp .cmhg
.c.o:;		$(MemCheckCC) $(CC) $(CFLAGS) -o $@ $<
.c.cp:;		$(CC) $(CFLAGS) -c -C -E $< >> $@
.s.o:;		$(AS) ${ASMFLAGS} -o $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

analyse: ${OBJSCP}
	@echo ${COMPONENT}: analysis build complete

export:	export_${PHASE}

export_hdrs: h.AcornSMTP
	${CP}	h.AcornSMTP	${EXP_C_H}.AcornSMTP	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: o.AcornSMTP
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

#
# Target 
#
${TARGET}:	$(OBJS) $(EXTRAS)
		$(MemCheckLD) $(LD) -rmf -s link/sym -o rm.AcornSMTPu $(OBJS) $(STUBS)
		ModSqz rm.AcornSMTPu $@

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
