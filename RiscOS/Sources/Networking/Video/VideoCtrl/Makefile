# Makefile for VideoControl
# =========================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 09-Sep-1999	ADH	Created from URL_Fetcher Makefile
# 05-Nov-1999	ADH	Formatting changed to make it foldable
# 08-Nov-1999	ADH	Now uses DebugLib
# 09-Nov-1999	ADH	Reworking for cleaner compile-time messages and
#			independent ROM / RAM builds. Exports header file.

# -----------------------------------------------------------------------------
# Component specific options
#

COMPONENT	= VideoCtrl
CMHG_HDR	= VCHdr
SWI_PREFIX	= VideoControl
ROM_MODULE	= aof.${COMPONENT}
RAM_MODULE	= rm.${COMPONENT}
DBG_MODULE	= rm.${COMPONENT}-tr
EXPORTS		= <CExport$Dir>.h.${COMPONENT}

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR		= <resource$dir>.Resources2.VideoCtrl

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
 o.Enumerate	\
 o.Generic	\
 o.Module	\
 o.MoreInfo	\
 o.MPEGCtrl	\
 o.Options	\
 o.ParseURL	\
 o.Play		\
 o.Protocol	\
 o.Register	\
 o.Setup	\
 o.Status	\
 o.TearDown	\
 o.URLutils	\
 o.Utils	\
 o.directswi	\
 o.${CMHG_HDR}

# oSL -> Soft Load

RAM_OBJS =	\
 oSL.Enumerate	\
 oSL.Generic	\
 oSL.Module	\
 oSL.MoreInfo	\
 oSL.MPEGCtrl	\
 oSL.Options	\
 oSL.ParseURL	\
 oSL.Play	\
 oSL.Protocol	\
 oSL.Register	\
 oSL.Setup	\
 oSL.Status	\
 oSL.TearDown	\
 oSL.URLutils	\
 oSL.Utils	\
 o.directswi	\
 o.${CMHG_HDR}

# oSLd -> Soft Load deug

DBG_OBJS =	\
 oSLd.Enumerate	\
 oSLd.Generic	\
 oSLd.Module	\
 oSLd.MoreInfo	\
 oSLd.MPEGCtrl	\
 oSLd.Options	\
 oSLd.ParseURL	\
 oSLd.Play	\
 oSLd.Protocol	\
 oSLd.Register	\
 oSLd.Setup	\
 oSLd.Status	\
 oSLd.TearDown	\
 oSLd.URLutils	\
 oSLd.Utils	\
 o.directswi	\
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

install: ${RAM_MODULE}
	${MKDIR} ${INSTDIR}.310.Modules.Networking
	${MKDIR} ${INSTDIR}.310.Modules.Networking.VideoCtrl
	${CP} ${RAM_MODULE} ${INSTDIR}.310.Modules.Networking.VideoCtrl.${COMPONENT}
	@echo
	@echo ${COMPONENT}: Module installed (Disc)

ramtrace: ${DBG_MODULE} ${DIRS}
	@echo
	@echo ${COMPONENT}: Module built (RAM debug)

rom: ${ROM_MODULE} ${DIRS}
	@echo
	@echo ${COMPONENT}: Module built (ROM)

export: ${EXPORTS}
	@echo
	@echo ${COMPONENT}: Export complete

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages LocalRes:ResList
	@echo
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
	${CP} LocalRes:* ${RESDIR}.${COMPONENT}.* ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: resource files copied to Messages module

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

${ROM_MODULE}: ${ROM_OBJS} ${ROMCSTUBS}
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

${EXPORTS}: h.${CMHG_HDR} h.VideoCtrl
	${CP} h.VideoCtrl $@ ${CPFLAGS}
	Do ${AWK} -- "/.define ${SWI_PREFIX}\_00/,/endif/" h.${CMHG_HDR} >>$@

${MSGSF}: LocalRes:Messages LocalRes:ResList
	TokenCheck LocalRes:Messages
	resgen messages_file ${MSGSF} -via LocalRes:ResList

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

o.Module oSL.Module oSLd.Module: VCHdr.h

o.Protocol oSL.Protocol oSLd.Protocol: VCHdr.h

# -----------------------------------------------------------------------------
# Dynamic dependencies:
