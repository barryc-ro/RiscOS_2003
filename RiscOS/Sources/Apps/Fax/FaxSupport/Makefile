# Makefile for FaxSupport
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
#
# Date		Name		Description
# ----		----		-----------
# 1997-07-18	BAL		Created
# 1998-01-28	MGR		Beautified and made child of Fax


# ------------------------------------------------------------------------------
# Generic options:
#

MKDIR		= cdir
AS		= objasm
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
MODSQZ		= @echo Not doing a modsqz
RESGEN		= resgen
WIPE		= -wipe

FEATURES        = -wp -zM -zps1 -fah ${DFLAGS}
ASMFLAGS	= ${THROWBACK} -depend !Depend
CFLAGS		= ${THROWBACK} -depend !Depend -c ${FEATURES} ${INCLUDES}
CMHGFLAGS	= ${THROWBACK} -depend !Depend -p ${DFLAGS}
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v

DFLAGS		= #-DDEBUG #-DTEST


# ------------------------------------------------------------------------------
# Libraries
#

CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM 		= RISC_OSLib:o.AbsSym

MGRLIB		= <Lib$Dir>.mgrlib.o.mgrlib

LIBS		= ${MGRLIB}


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -Itbox:,C:


# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= FaxSupport
RAMTARGET	= rm.${COMPONENT}
ROMTARGET	= aof.${COMPONENT}

OBJS = \
 o.cmhg\
 o.main\
 o.preveneer\

OBJSSA = \
 o.cmhg\
 oSA.main\
 o.preveneer\

HDRS = h.cmhg


# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .oSA .h .s .c .cmhg

.c.o:;		@echo
		@echo Compiling $< ( ${FEATURES})
		@${CC} ${CFLAGS} -o $@ $<

.c.oSA:;	@echo
		@echo Compiling $< ( ${FEATURES}-DSTANDALONE )
		@${CC} ${CFLAGS} -DSTANDALONE -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@$(AS) ${ASMFLAGS} -o $@ $<

.cmhg.o:;	@echo
		@echo Generating module C veneers ( -p ${DFLAGS})
		@${CMHG} ${CMHGFLAGS} $< -o $@

.cmhg.h:;	@echo
		@echo Generating module C veneers' header file ( -p ${DFLAGS})
		@${CMHG} ${CMHGFLAGS} $< -d $@


# ------------------------------------------------------------------------------
# Main rules
#

install: ram

ram: ${RAMTARGET}
	@echo
	@echo ${COMPONENT}: Module built (RAM) in ${RAMTARGET}

rom: ${ROMTARGET}
	@echo
	@echo ${COMPONENT}: Module built (ROM) in ${ROMTARGET}

export: ${EXPORTS}
	@echo
	@echo ${COMPONENT}: No exports

install_rom: ${ROMTARGET}
	@echo
	${CP} ${ROMTARGET} ^.${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Module installed (ROM)

clean_all: clean

clean:
	@echo
	${RM}		${HDRS}
	${WIPE} o	${WFLAGS}
	${WIPE} oSA	${WFLAGS}
	${WIPE} linked	${WFLAGS}
	${WIPE} map	${WFLAGS}
	${RM} syms
	${RM} ${RAMTARGET}
	${RM} ${ROMTARGET}
	${WIPE} aof	${WFLAGS}
	${WIPE} rm	${WFLAGS}
	@echo
	@echo ${COMPONENT}: Cleaned

resources:
	@echo
        @echo ${COMPONENT}: No resources

o.dirs:
	@echo
	@echo Creating directories
	${MKDIR} aof
	${MKDIR} linked
	${MKDIR} map
	${MKDIR} o
	${MKDIR} oSA
	${MKDIR} rm
	@create o.dirs
	@stamp  o.dirs


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#


# Target 

${RAMTARGET}: ${OBJSSA} ${HDRS} ${LIBS} ${CLIB} o.dirs
	@echo
	@echo Linking $@ ( ${OBJSSA} )
	@${LD} -rmf -symbols @.syms -o $@ ${OBJSSA} ${LIBS} ${CLIB}
	@echo
	${MODSQZ} $@

o.res: Resources.<Locale>.Messages
	${RESGEN} resources_area o.res -via Res.List


# ROM Target (re-linked at ROM Image build time)

${ROMTARGET}: ${OBJS} ${HDRS} ${LIBS} ${ROMCSTUBS} o.dirs
	@echo
	@echo Linking $@ ( ${OBJS} )
	@${LD} -aof -symbols @.syms -o $@ ${OBJS} ${LIBS} ${ROMCSTUBS}

# Final link for the ROM Image (using given base address)

rom_link:
	@echo
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} \
		${ROMTARGET} ${ABSSYM} -map > map.${COMPONENT}
	@echo
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo
	@echo ${COMPONENT}: rom_link complete

# ------------------------------------------------------------------------------
# Dynamic dependencies:
