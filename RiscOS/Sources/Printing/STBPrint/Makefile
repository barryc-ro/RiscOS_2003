# Makefile for STBPrint
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 13-Apr-95  JRH          Created.
# 20-Jun-95  RWB	  Added build to disc based application.
# 12-Feb-96  JRH          Fixed up with new ver.2 palettes from !Printers 1.52

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
COMPRESS = squash
STRIP   = stripdepnd

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
WRAPPER   = s.ModuleWrap

#
# Include files
#
INCLUDES = -IC:

DIRS     = o._dirs

# DFLAGS should be NULL for any non-debugging release, for old key mappings, key OLDKEYS flag
DFLAGS    = -DOLDKEYS
#DFLAGS   =  -DDEBUG

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESAPP = <resource$dir>.Apps.${APP}

#
# Program specific options:
#
COMPONENT   = STBPrint
APP         = !STBPrint
APPLICATION = !RunImage
TARGET      = aof.STBPrint
RDIR        = Resources
RSRC        = ResSrc
LDIR        = ${RDIR}.${LOCALE}.${MACHINE}
OBJS        =	\
 o.cmos		\
 o.config	\
 o.db		\
 o.display	\
 o.error	\
 o.file		\
 o.font		\
 o.keybd	\
 o.main		\
 o.msg		\
 o.process	\
 o.text

OBJSZ      =	\
 oz.cmos	\
 oz.config	\
 oz.db		\
 oz.display	\
 oz.error	\
 oz.file	\
 oz.font	\
 oz.keybd	\
 oz.main	\
 oz.msg		\
 oz.process	\
 oz.text

# Include these in the ROM module
RESFILES =			\
 ${RDIR}.PaperSizes		\
 ${RDIR}.Printers.JP-150	\
 ${RDIR}.Printers.BubbleJet	\
 ${RDIR}.Printers.BJC600_BW	\
 ${RDIR}.Printers.BJC600_Col	\
 ${RDIR}.Printers.DJ500C_BW	\
 ${RDIR}.Printers.DJ500C_Col	\
 ${RDIR}.Printers.LasJet4   	\
 ${RDIR}.Printers.FX-80		\
 ${RDIR}.Printers.FX-85		\
 ${RDIR}.Printers.Stylus-800	\
 ${RDIR}.Printers.StyCol_Col	\
 ${RDIR}.Printers.StyCol_BW	\
 ${RDIR}.Printers.StyColH_Co	\
 ${RDIR}.Printers.StyColH_BW	\
 ${RDIR}.Printers.LC-200_BW	\
 ${RDIR}.Printers.LC-200_Col	\
 ${RDIR}.Printers.Swift24_BW	\
 ${RDIR}.Printers.Swift24_Co	\
 ${RDIR}.Printers.PS_BW		\
 ${RDIR}.Printers.PS_Col

#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -ff -zM -zps1 -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a !RunImage module:
#
all: ${APPLICATION}
	@echo ${APPLICATION} built

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
	@echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom module installed

clean:
	${WIPE} abs ${WFLAGS}
	${WIPE} aof ${WFLAGS}
	${WIPE} o ${WFLAGS}
	${WIPE} oz ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${WIPE} map ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${APPLICATION}
	${STRIP}
	@echo ${COMPONENT}: cleaned

${DIRS}:
	@${MKDIR} abs
	@${MKDIR} aof
	@${MKDIR} linked
	@${MKDIR} map
	@${MKDIR} o
	@${MKDIR} oz
	@${MKDIR} rm
	create ${DIRS}
	stamp  ${DIRS}

resources:
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESDIR}.Palettes
	${MKDIR} ${RESDIR}.Printers
	${MKDIR} ${RESDIR}.ps
	${MKDIR} ${RESDIR}.ps.Printers
	${MKDIR} ${RESDIR}.ps.PSfiles
	${CP} ${LDIR}.Messages             ${RESDIR}.Messages             ${CPFLAGS}
	${CP} ${LDIR}.Supported	           ${RESDIR}.Supported            ${CPFLAGS}
	${CP} ${RDIR}.!Run	           ${RESDIR}.!Run                 ${CPFLAGS}
	${CP} ${RDIR}.Palettes.0           ${RESDIR}.Palettes.0	          ${CPFLAGS}
	${CP} ${RDIR}.Palettes.1           ${RESDIR}.Palettes.1           ${CPFLAGS}
	${CP} ${RDIR}.Palettes.2           ${RESDIR}.Palettes.2           ${CPFLAGS}
	${CP} ${RDIR}.Palettes.3           ${RESDIR}.Palettes.3	          ${CPFLAGS}
	${CP} ${RDIR}.Palettes.4           ${RESDIR}.Palettes.4	          ${CPFLAGS}
	${CP} ${RDIR}.Palettes.5           ${RESDIR}.Palettes.5	          ${CPFLAGS}
	${CP} ${RDIR}.Palettes.6           ${RESDIR}.Palettes.6	          ${CPFLAGS}
	${CP} ${RDIR}.Palettes.7           ${RESDIR}.Palettes.7	          ${CPFLAGS}
	${CP} ${RDIR}.Palettes.8           ${RESDIR}.Palettes.8	          ${CPFLAGS}
	${CP} ${RDIR}.Palettes.9           ${RESDIR}.Palettes.9	          ${CPFLAGS}
	${CP} ${RDIR}.Palettes.10          ${RESDIR}.Palettes.10          ${CPFLAGS}
	${CP} ${RDIR}.ps.Printers.0        ${RESDIR}.ps.Printers.0        ${CPFLAGS}
	${CP} ${RDIR}.ps.PSfiles.PSepilog  ${RESDIR}.ps.PSfiles.PSepilog  ${CPFLAGS}
	${CP} ${RDIR}.ps.PSfiles.PSprolog2 ${RESDIR}.ps.PSfiles.PSprolog2 ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied

install: ${RESFILES} ${APPLICATION}
	${MKDIR} ${INSTDIR}.Palettes
	${MKDIR} ${INSTDIR}.Printers
	${MKDIR} ${INSTDIR}.ps
	${MKDIR} ${INSTDIR}.ps.Printers
	${MKDIR} ${INSTDIR}.ps.PSfiles
	${CP} ${LDIR}.Messages             ${INSTDIR}.Messages             ${CPFLAGS}
	${CP} ${LDIR}.Supported	           ${INSTDIR}.Supported            ${CPFLAGS}
	${CP} ${RDIR}.!Boot	           ${INSTDIR}.!Boot                ${CPFLAGS}
	${CP} ${RDIR}.!Run2	           ${INSTDIR}.!Run                 ${CPFLAGS}
	${CP} ${APPLICATION}	           ${INSTDIR}.${APPLICATION}       ${CPFLAGS}
	${CP} ${RDIR}.!Sprites	           ${INSTDIR}.!Sprites             ${CPFLAGS}
	${CP} ${RDIR}.Palettes.0           ${INSTDIR}.Palettes.0	   ${CPFLAGS}
	${CP} ${RDIR}.Palettes.1           ${INSTDIR}.Palettes.1           ${CPFLAGS}
	${CP} ${RDIR}.Palettes.2           ${INSTDIR}.Palettes.2           ${CPFLAGS}
	${CP} ${RDIR}.Palettes.3           ${INSTDIR}.Palettes.3	   ${CPFLAGS}
	${CP} ${RDIR}.Palettes.4           ${INSTDIR}.Palettes.4	   ${CPFLAGS}
	${CP} ${RDIR}.Palettes.5           ${INSTDIR}.Palettes.5	   ${CPFLAGS}
	${CP} ${RDIR}.Palettes.6           ${INSTDIR}.Palettes.6	   ${CPFLAGS}
	${CP} ${RDIR}.Palettes.7           ${INSTDIR}.Palettes.7	   ${CPFLAGS}
	${CP} ${RDIR}.Palettes.8           ${INSTDIR}.Palettes.8	   ${CPFLAGS}
	${CP} ${RDIR}.Palettes.9           ${INSTDIR}.Palettes.9	   ${CPFLAGS}
	${CP} ${RDIR}.Palettes.10          ${INSTDIR}.Palettes.10	   ${CPFLAGS}
	${CP} ${RDIR}.ps.Printers.0        ${INSTDIR}.ps.Printers.0        ${CPFLAGS}
	${CP} ${RDIR}.ps.PSfiles.PSepilog  ${INSTDIR}.ps.PSfiles.PSepilog  ${CPFLAGS}
	${CP} ${RDIR}.ps.PSfiles.PSprolog2 ${INSTDIR}.ps.PSfiles.PSprolog2 ${CPFLAGS}
	${CP} ${RDIR}.PaperSizes	   ${INSTDIR}.PaperSizes	   ${CPFLAGS}
	${CP} ${RDIR}.Printers.*	   ${INSTDIR}.Printers.*	   ${CPFLAGS}
	@echo ${COMPONENT}: installed
	
#
# Static dependencies:
#
abs.!RunImage: ${OBJS} ${RLIB} ${CLIB} ${DIRS}
	${LD} -o $@ ${OBJS} ${RLIB} ${CLIB}

rm.STBPrint: oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB} ${DIRS}
	${LD} -o $@ -module oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: oz.ModuleWrap ${OBJSZ} ${ROMSTUBS} ${DIRS}
	${LD} -o $@ -aof oz.ModuleWrap ${OBJSZ} ${ROMSTUBS}

#
# Build for !RunImage
#
${APPLICATION}: ${OBJS} ${CLIB} ${DIRS}
	${LD} -o $@ ${OBJS} ${CLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${MKDIR} linked
	${MKDIR} map
	${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete


oz.ModuleWrap: s.ModuleWrap ${RESFILES}
	${AS} ${AFLAGS} s.ModuleWrap $@

#
# Squash resource files
#
${RDIR}.PaperSizes:		${RSRC}.A4.PaperSizes
	${COMPRESS}	${RSRC}.A4.PaperSizes		${RDIR}.PaperSizes

${RDIR}.Printers.JP-150:	${RSRC}.Printers.JP-150
	${COMPRESS}	${RSRC}.Printers.JP-150		${RDIR}.Printers.JP-150

${RDIR}.Printers.BubbleJet:	${RSRC}.Printers.BubbleJet
	${COMPRESS}	${RSRC}.Printers.BubbleJet	${RDIR}.Printers.BubbleJet

${RDIR}.Printers.BJC600_BW:	${RSRC}.Printers.BJC600_BW
	${COMPRESS}	${RSRC}.Printers.BJC600_BW	${RDIR}.Printers.BJC600_BW

${RDIR}.Printers.BJC600_Col:	${RSRC}.Printers.BJC600_Col
	${COMPRESS}	${RSRC}.Printers.BJC600_Col	${RDIR}.Printers.BJC600_Col

${RDIR}.Printers.DJ500C_BW:	${RSRC}.Printers.DJ500C_BW
	${COMPRESS}	${RSRC}.Printers.DJ500C_BW	${RDIR}.Printers.DJ500C_BW

${RDIR}.Printers.DJ500C_Col:	${RSRC}.Printers.DJ500C_Col
	${COMPRESS}	${RSRC}.Printers.DJ500C_Col	${RDIR}.Printers.DJ500C_Col

${RDIR}.Printers.LasJet4:	${RSRC}.Printers.LasJet4
	${COMPRESS}	${RSRC}.Printers.LasJet4	${RDIR}.Printers.LasJet4

${RDIR}.Printers.FX-80:		${RSRC}.Printers.FX-80
	${COMPRESS}	${RSRC}.Printers.FX-80		${RDIR}.Printers.FX-80

${RDIR}.Printers.FX-85:		${RSRC}.Printers.FX-85
	${COMPRESS}	${RSRC}.Printers.FX-85		${RDIR}.Printers.FX-85

${RDIR}.Printers.Stylus-800:	${RSRC}.Printers.Stylus-800
	${COMPRESS}	${RSRC}.Printers.Stylus-800	${RDIR}.Printers.Stylus-800

${RDIR}.Printers.LC-200_BW:	${RSRC}.Printers.LC-200_BW
	${COMPRESS}	${RSRC}.Printers.LC-200_BW	${RDIR}.Printers.LC-200_BW

${RDIR}.Printers.LC-200_Col:	${RSRC}.Printers.LC-200_Col
	${COMPRESS}	${RSRC}.Printers.LC-200_Col	${RDIR}.Printers.LC-200_Col

${RDIR}.Printers.Swift24_BW:	${RSRC}.Printers.Swift24_BW
	${COMPRESS}	${RSRC}.Printers.Swift24_BW	${RDIR}.Printers.Swift24_BW

${RDIR}.Printers.Swift24_Co:	${RSRC}.Printers.Swift24_Co
	${COMPRESS}	${RSRC}.Printers.Swift24_Co	${RDIR}.Printers.Swift24_Co

${RDIR}.Printers.PS_BW:		${RSRC}.Printers.PS_BW
	${COMPRESS}	${RSRC}.Printers.PS_BW		${RDIR}.Printers.PS_BW

${RDIR}.Printers.PS_Col:	${RSRC}.Printers.PS_Col
	${COMPRESS}	${RSRC}.Printers.PS_Col		${RDIR}.Printers.PS_Col

${RDIR}.Printers.StyCol_Col:	${RSRC}.Printers.StyCol_Col
	${COMPRESS}	${RSRC}.Printers.StyCol_Col	${RDIR}.Printers.StyCol_Col

${RDIR}.Printers.StyCol_BW:	${RSRC}.Printers.StyCol_BW
	${COMPRESS}	${RSRC}.Printers.StyCol_BW	${RDIR}.Printers.StyCol_BW

${RDIR}.Printers.StyColH_Co:	${RSRC}.Printers.StyColH_Co
	${COMPRESS}	${RSRC}.Printers.StyColH_Co	${RDIR}.Printers.StyColH_Co

${RDIR}.Printers.StyColH_BW:	${RSRC}.Printers.StyColH_BW
	${COMPRESS}	${RSRC}.Printers.StyColH_BW	${RDIR}.Printers.StyColH_BW

#---------------------------------------------------------------------------
# Dynamic dependencies:
