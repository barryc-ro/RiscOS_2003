# Makefile for LVFGeneric
# =======================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 08-Nov-1999	ADH	Created from VideoControl Makefile
# 09-Nov-1999	ADH	Reworking for cleaner compile-time messages and
#			independent ROM / RAM builds
# 05-Apr-2000	ADH	Corrected path and filenames in ROM resources
#			export rule

# -----------------------------------------------------------------------------
# Component specific options
#

COMPONENT	= LVFGeneric
ROM_MODULE	= aof.${COMPONENT}
RAM_MODULE	= rm.${COMPONENT}
DBG_MODULE	= rm.${COMPONENT}-tr

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

CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v

CFLAGS		= -c -depend !Depend ${INCLUDES} -zM -Wp -ffca -zps1
DFLAGS		= -D${SYSTEM} -DCOMPAT_INET4 -UDIRECTSWI ${EXTRAFLAGS}
ROMFLAGS	= -DROM
RAMFLAGS	= -UROM
INCLUDES	= -ITCPIPLibs:,C:

DIRS		= o._dirs_

# Extra compilation options:
# ALWAYS_CLAIM_EVENTV		The module will sit on EventV from the moment
#				it starts to the moment it dies if this is
#				defined. Otherwise, it'll only claim it for
#				as long as any sessions exist (which should
#				be more efficient).
# FAST				Use the direct call interfaces to the MPEG
#				system (requires new MPEG modules).

EXTRAFLAGS	= -UALWAYS_CLAIM_EVENTV -DFAST

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

ROM_OBJS =		\
 o.Buffers		\
 o.CallOut		\
 o.Generic		\
 o.Headers		\
 o.LVFCtrl		\
 o.Module		\
 o.MoreInfo		\
 o.Options		\
 o.Parameters		\
 o.Play			\
 o.Response		\
 o.Setup		\
 o.Session		\
 o.Status		\
 o.TearDown		\
 o.URLutils		\
 o.Utils		\
 o.LVFGHdr

# oSL -> Soft Load

RAM_OBJS =		\
 oSL.Buffers		\
 oSL.CallOut		\
 oSL.Generic		\
 oSL.Headers		\
 oSL.LVFCtrl		\
 oSL.Module		\
 oSL.MoreInfo		\
 oSL.Options		\
 oSL.Parameters		\
 oSL.Play		\
 oSL.Response		\
 oSL.Setup		\
 oSL.Session		\
 oSL.Status		\
 oSL.TearDown		\
 oSL.URLutils		\
 oSL.Utils		\
 o.LVFGHdr

# oSLd -> Soft Load deug

DBG_OBJS =		\
 oSLd.Buffers		\
 oSLd.CallOut		\
 oSLd.Generic		\
 oSLd.Headers		\
 oSLd.LVFCtrl		\
 oSLd.Module		\
 oSLd.MoreInfo		\
 oSLd.Options		\
 oSLD.Parameters	\
 oSLd.Play		\
 oSLd.Response		\
 oSLd.Setup		\
 oSLd.Session		\
 oSLd.Status		\
 oSLd.TearDown		\
 oSLd.URLutils		\
 oSLd.Utils		\
 o.LVFGHdr

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
	${MKDIR} ${INSTDIR}.310.Modules.Networking.LVFGeneric
	${CP} ${RAM_MODULE} ${INSTDIR}.310.Modules.Networking.LVFGeneric.${COMPONENT}
	@echo
	@echo ${COMPONENT}: Module installed (Disc)

ramtrace: ${DBG_MODULE} ${DIRS}
	@echo
	@echo ${COMPONENT}: Module built (RAM debug)

rom: ${ROM_MODULE} ${DIRS}
	@echo
	@echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages
	@echo
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
	${CP} LocalRes:Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Resource files copied to Messages module

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
	${WIPE} h.LVFGHdr	${WFLAGS}
	@echo ${COMPONENT}: cleaned

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

${MSGSF}: LocalRes:Messages
	TokenCheck LocalRes:Messages
	resgen messages_file ${MSGSF} LocalRes:Messages Resources.VideoCtrl.${COMPONENT}.Messages

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

o.LVFGHdr h.LVFGHdr: VersionNum

o.CallOut oSL.CallOut oSLd.CallOut: LVFGHdr.h

o.LVFCtrl oSL.LVFCtrl oSLd.LVFCtrl: LVFGHdr.h

o.Module oSL.Module oSLd.Module: LVFGHdr.h

o.Status oSL.Status oSLd.Status: LVFGHdr.h

# -----------------------------------------------------------------------------
# Dynamic dependencies:
