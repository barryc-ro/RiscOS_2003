# Makefile for AcornURI
# =====================
#

# ------------------------------------------------------------------------------
# Change list
#

# Date		Name	Description
# ====		====	===========
# 1997-06-23	BAL	Created
# 2000-04-14	ADH	Merged in URILib export stuff, modified style to aid
#			folding text editors.

# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR	=	<export$dir>
EXP_C_H	=	<CExport$dir>.h
EXP_C_O	=	<CExport$dir>.o

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
LIBFILE		= libfile
RM		= remove
MODSQZ		= modsqz
WIPE		= -wipe

ASMFLAGS	= -depend !Depend -Stamp -quit
CFLAGS		= -c -depend !Depend -throwback -zM ${INCLUDES} ${DFLAGS}
CMHGFLAGS	= -depend !Depend -p -throwback ${INCLUDES}
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v

# ------------------------------------------------------------------------------
# Libraries
#

CLIB =		CLIB:o.stubs
RLIB =		RISCOSLIB:o.risc_oslib
RSTUBS =	RISCOSLIB:o.rstubs
ROMSTUBS =	RISCOSLIB:o.romstubs
ROMCSTUBS =	RISCOSLIB:o.romcstubs
ABSSYM  =	RISC_OSLib:o.c_AbsSym
LIBS =		C:tboxlibs.o.eventlibm C:tboxlibs.o.wimplib

# ------------------------------------------------------------------------------
# Include files
#
INCLUDES =	-Itbox:,TCPIPLibs:,C:

# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	=	URI
RAMTARGET	=	rm.URI
ROMTARGET	=	aof.URI
LIBTARGET	=	l.URI
EXPORTS		=	{EXP_DIR}.URI
DIRS		=	o._dirs_

# ------------------------------------------------------------------------------
# Export Paths for Messages module
#

#RESDIR = 	<resource$dir>.Resources2.URI

# ------------------------------------------------------------------------------
# Object lists
#

OBJS =		\
 o.cstart	\
 o.globals	\
 o.ModHdr	\
 o.module	\
 o.ralloc	\
 o.robase	\
 o.task		\
 o.URIswis

LIBOBJS =	\
 o.veneers

HDRS =	h.ModHdr

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .s .c .h
.c.o:;		${CC} ${CFLAGS} -o $@ $<
.s.o:;		${AS} ${ASMFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} $< -o $@ -d h.$*
.cmhg.h:;	${CMHG} ${CMHGFLAGS} $< -d $@ -o o.$*

# ------------------------------------------------------------------------------
# Main rules:
#

${DIRS}:
	@echo
	${MKDIR} aof
	${MKDIR} l
	${MKDIR} linked
	${MKDIR} map
	${MKDIR} o
	${MKDIR} rm
	create ${DIRS}
	@echo

clean:
	${RM} ${HDRS}
	${RM} syms
	${WIPE} aof	${WFLAGS}
	${WIPE} l	${WFLAGS}
	${WIPE} linked	${WFLAGS}
	${WIPE} map	${WFLAGS}
	${WIPE} o	${WFLAGS}
	${WIPE} rm	${WFLAGS}
	@echo
	@echo ${COMPONENT}: cleaned

ram: ${RAMTARGET}
	@echo
	@echo ${COMPONENT}: all complete

rom: ${ROMTARGET}
	@echo
	@echo ${COMPONENT}: rom module built

export: export_${PHASE}

export_hdrs: h.${COMPONENT}
	${CP} h.${COMPONENT} ${EXP_C_H}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBTARGET}
	${CP} ${LIBTARGET} ${EXP_C_O}.${COMPONENT} ${CPFLAGS}

install_rom: ${ROMTARGET}
	${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: rom module installed

resources:
	@echo
	@echo ${COMPONENT}: nothing done

#resources:
#        ${MKDIR} ${RESDIR}.^.^.URI
#        ${MKDIR} ${RESDIR}
#        ${CP} ${LDIR}.Messages ${RESDIR}.Messages ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

# ------------------------------------------------------------------------------
# Targets
#

${RAMTARGET}: ${OBJS} ${HDRS} ${CLIB} ${DIRS}
	@echo
	${MKDIR} rm
	${LD} -rmf -symbols syms -o $@ ${OBJS} ${LIBS} ${CLIB}
	${MODSQZ} $@
	Access $@ WR/R

${ROMTARGET}: ${OBJS} ${HDRS} ${ROMCSTUBS} ${DIRS}
	@echo
	${MKDIR} aof
	${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}
	Access $@ WR/R

${LIBTARGET}: ${LIBOBJS} ${DIRS}
	@echo
	${LIBFILE} -c -o ${LIBTARGET} ${LIBOBJS}
	Access $@ WR/R

# ------------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#

rom_link: ${DIRS} ${ROM_MODULE}
	@echo
	${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

# ------------------------------------------------------------------------------
# Dynamic dependencies:
