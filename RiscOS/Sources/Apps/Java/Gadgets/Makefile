# Makefile for TextGadgets
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 1997-12-16	BAL		Created

# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
CHMOD	= access

FEATURES   = -zM -zps1 -ffah ${DFLAGS}
AFLAGS     = ${THROWBACK} -depend !Depend
CFLAGS     = ${THROWBACK} -depend !Depend -c ${FEATURES} ${INCLUDES}
CMHGFLAGS  = ${THROWBACK} -depend !Depend -p ${DFLAGS}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R

DFLAGS  = -D${SYSTEM}_BUILD #-DUSE_TINY ${DEBUG}


# ------------------------------------------------------------------------------
# Libraries
#

CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = <Lib$Dir>.debug.o.remotezm

LIBS =\
 <Lib$Dir>.tboxlibs.o.toolboxlib\
 <Lib$Dir>.tboxlibs.o.wimplib\


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES = -Itbox:,C:,<Lib$Dir>.


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT = TextGadget
ROMTARGET = aof.${COMPONENT}
RAMTARGET = rm.${COMPONENT}
#EXPORTS   = ${EXP_C_H}.${COMPONENT}

OBJS      =	\
 o.Modhdr \
 o.TextGadget \
 o.TextArea \
 o.ScrollList \
 o.Scrollbar \
 o.riscos_uti \
 o.riscos_gra \
 o.Font \
 o.debug \
 o.TextMan \
 o.MemMan \
 o.Text \
 o.ScrollLisS \
 o.Utils \
 o.TAsel_ven \
 o.glib \
 o.glib3 \
 o.rmensure \
 o.string32 \


HDR = h.ModHdr


# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .h .s .c .cmhg

.c.o:;		@echo
		@echo Compiling $< ( ${FEATURES})
		@${CC} ${CFLAGS} -o $@ $<

.cmhg.o:;	@echo
		@echo Generating module C veneers ( -p ${DFLAGS})
		@${CMHG} ${CMHGFLAGS} $< -o $@

.cmhg.h:;	@echo
		@echo Generating module C veneers' header file ( -p ${DFLAGS})
		@${CMHG} ${CMHGFLAGS} $< -d $@

.s.o:;		@echo
		@echo Assembling $<
		@${AS} ${AFLAGS} -o $@ $<


# ------------------------------------------------------------------------------
# build a relocatable module:
#

all: ${RAMTARGET}
	@echo
	@echo ${COMPONENT}: Module built (RAM) in ${RAMTARGET}
	

# ------------------------------------------------------------------------------
# RISC OS ROM build rules:
#

rom: ${ROMTARGET}
	@echo
        @echo ${COMPONENT}: Module built (ROM) in ${ROMTARGET}

export: ${EXPORTS}
	@echo
        @echo ${COMPONENT}: No exports

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
	@echo
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.*    ${WFLAGS}
	${RM} ${HDR}
        ${RM} ${RAMTARGET}
        ${RM} ${ROMTARGET}
        @echo
        @echo ${COMPONENT}: Cleaned
        

# ------------------------------------------------------------------------------
# ROM target (re-linked at ROM Image build time)
#

${ROMTARGET}: ${OBJS} ${HDR} ${LIBS} ${ROMCSTUBS} 
	@echo
	@echo Linking $@ ( ${OBJS} )
        @${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}


# ------------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#

rom_link:
        ${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


# ------------------------------------------------------------------------------
# Relocatable module target
#

${RAMTARGET}: ${OBJS} ${HDR} ${LIBS} ${CLIB}
	@echo
	@echo Linking $@ ( ${OBJS} )
        @${LD} -rmf -o $@ ${OBJS} ${LIBS} ${CLIB}
        @echo
        ${CHMOD} rm.${COMPONENT} ${CHMODFLAGS}

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
