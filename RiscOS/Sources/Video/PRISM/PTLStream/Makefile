# Makefile for PTLStream
# =========================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 26-Feb-2001	ADH	Created from Video Control makefile

# -----------------------------------------------------------------------------
# Component specific options
#

COMPONENT	= PTLStream
CMHG_HDR	= PTLSHdr
SWI_PREFIX	= PTLStream
ROM_MODULE	= aof.${COMPONENT}
RAM_MODULE	= rm.${COMPONENT}
DBG_MODULE	= rm.${COMPONENT}-tr
EXPORTS		= <CExport$Dir>.h.${COMPONENT}

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR		= <resource$dir>.Resources2.PRISM

# -----------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
TLD		= tlink
RM		= remove
WIPE		= -wipe
MODSQZ		= modsqz
AWK		= awk

CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v

CFLAGS		= -c -depend !Depend ${INCLUDES} -zM -Wpc -ffah -zps1 ${DFLAGS}
DFLAGS		= -D${SYSTEM} -DCOMPAT_INET4 -UDIRECTSWI
ROMFLAGS	= -DROM
RAMFLAGS	= -UROM
INCLUDES	= -ITCPIPLibs:,C:

DIRS		= o._dirs_

# -----------------------------------------------------------------------------
# Libraries and objects lists
#

ANSILIB		= CLib:o.ansilib
CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.abssym
TCPIPLIBS	= TCPIPLibs:o.inetlibzm TCPIPLIBS:o.socklibzm
DEBUGLIB	= <Lib$Dir>.Debuglib.o.debuglibzm
ASMUTILS	= <Lib$Dir>.AsmUtils.o.AsmUtilszm

LIBS = ${DEBUGLIB} \
       ${TCPIPLIBS} \
       ${ASMUTILS}

ROM_OBJS =	\
 o.CallOut	\
 o.Generic	\
 o.Module	\
 o.Protocol	\
 o.URLUtils	\
 o.Utils	\
 o.${CMHG_HDR}

# oSL -> Soft Load

RAM_OBJS =	\
 oSL.CallOut	\
 oSL.Generic	\
 oSL.Module	\
 oSL.Protocol	\
 oSL.URLUtils	\
 oSL.Utils	\
 o.${CMHG_HDR}

# oSLd -> Soft Load deug

DBG_OBJS =	\
 oSLd.CallOut	\
 oSLd.Generic	\
 oSLd.Module	\
 oSLd.Protocol	\
 oSLd.URLUtils	\
 oSLd.Utils	\
 o.${CMHG_HDR}

# Defining TRACE just does debuglib based message outputting. Defining
# TRACE2 defines TRACE for you and also enables various extra bits and
# pieces such as the clients list command (a CMHG header alteration is
# needed for this too).

MSGSF		= o.msgs
DBGFLAGS	= -DTRACE2

# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:
.SUFFIXES: .o .oSL .oSLd .i .s .c .h .cmhg

.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS} ${ROMFLAGS}
		@${CC} ${CFLAGS} ${DFLAGS} ${ROMFLAGS} -o $@ $<

.c.oSL:;	@echo
		@echo Compiling $<
		@echo ${DFLAGS} ${RAMFLAGS}
		@${CC} ${CFLAGS} ${DFLAGS} ${RAMFLAGS} -o $@ $<

.c.oSLd:;	@echo
		@echo Compiling $<
		@echo ${DFLAGS} ${RAMFLAGS} ${DBGFLAGS}
		@${CC} ${CFLAGS} ${DFLAGS} ${RAMFLAGS} ${DBGFLAGS} -o $@ $<

.c.i:;		@echo
		@echo Creating $<
		@echo ${DFLAGS} ${RAMFLAGS} -C -E
		@${CC} ${CFLAGS} ${DFLAGS} ${RAMFLAGS} -C -E $< >$@

.s.o .s.oSL .s.oSLd:; \
		@echo
		@echo Assembling $<
		@echo -depend !Depend -throwback
		@objasm -depend !Depend -throwback -o $@ $<

.cmhg.o .cmhg.oSL .cmhg.oSLd:; \
		@echo
		@echo CMHG generating $@
		@echo -p -o
		@${CMHG} -p -o $@ $<

.cmhg.h:;	@echo
		@echo CMHG generating $@
		@echo -p -d
		@${CMHG} -p -d $@ $<

# -----------------------------------------------------------------------------
# Main rules
#

all: ${RAM_MODULE} ${DIRS}
	@echo
	@echo ${COMPONENT}: Module built (RAM)
	@echo

install: ${RAM_MODULE}
	${MKDIR} ${INSTDIR}.310.Modules.PRISM
	${CP} ${RAM_MODULE} ${INSTDIR}.310.Modules.PRISM.${COMPONENT}
	@echo
	@echo ${COMPONENT}: Module installed (Disc)
	@echo

ramtrace: ${DBG_MODULE} ${DIRS}
	@echo
	@echo ${COMPONENT}: Module built (RAM debug)
	@echo

rom: ${ROM_MODULE} ${DIRS}
	@echo
	@echo ${COMPONENT}: Module built (ROM)
	@echo

export: ${EXPORTS}
	@echo
	@echo ${COMPONENT}: Export complete
	@echo

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Module installed (ROM)
	@echo

resources: LocalRes:Messages
	@echo
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
	${CP} LocalRes:Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied to Messages module
	@echo

${DIRS}:
	@echo
	${MKDIR} o
	${MKDIR} oSL
	${MKDIR} oSLd
	${MKDIR} i
	${MKDIR} linked
	create ${DIRS}

clean:
	${WIPE} o		${WFLAGS}
	${WIPE} oSL		${WFLAGS}
	${WIPE} oSLd		${WFLAGS}
	${WIPE} i		${WFLAGS}
	${WIPE} linked		${WFLAGS}
	${WIPE} aof		${WFLAGS}
	${WIPE} rm		${WFLAGS}
	${WIPE} h.${CMHG_HDR}	${WFLAGS}
	@echo
	@echo ${COMPONENT}: cleaned
	@echo

${ROM_MODULE}: ${ROM_OBJS} ${ROMCSTUBS} ${DIRS}
	@echo
	${MKDIR} aof
	${LD} -o $@ -aof ${ROM_OBJS} ${LIBS} ${ROMCSTUBS}

# -----------------------------------------------------------------------------
# Final link for ROM Image (using given base address)
#

rom_link: ${DIRS} ${ROM_MODULE}
	@echo
	${MKDIR} aof
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

${EXPORTS}: h.${CMHG_HDR} h.PTLStream
	${CP} h.PTLStream $@ ${CPFLAGS}
	Do ${AWK} -- "/.define ${SWI_PREFIX}\_00/,/endif/" h.${CMHG_HDR} >>$@

${MSGSF}: LocalRes:Messages
	TokenCheck LocalRes:Messages
	resgen messages_file ${MSGSF} LocalRes:Messages Resources.PRISM.${COMPONENT}.Messages

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${LIBS} ${CLIB}
	@echo
	${MKDIR} rm
	${LD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${LIBS} ${CLIB}
	${MODSQZ} $@
	Access $@ RW/R

${DBG_MODULE}: ${DBG_OBJS} ${MSGSF} ${LIBS} ${CLIB}
	@echo
	${MKDIR} rm
	${LD} -o $@ -module ${DBG_OBJS} ${MSGSF} ${LIBS} ${CLIB}
	Access $@ RW/R

# -----------------------------------------------------------------------------
# Specialised static dependencies
#

o.PTLSHdr h.PTLSHdr: VersionNum

o.Module oSL.Module oSLd.Module: PTLSHdr.h

# -----------------------------------------------------------------------------
# Dynamic dependencies:
