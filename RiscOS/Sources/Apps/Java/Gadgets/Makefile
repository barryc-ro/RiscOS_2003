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

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
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
.c.o:;		${CC}   ${CFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;	${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;		${AS}   ${AFLAGS} $< $@


# ------------------------------------------------------------------------------
# build a relocatable module:
#

all: ${RAMTARGET}
	@echo ${COMPONENT}: all complete
	

# ------------------------------------------------------------------------------
# RISC OS ROM build rules:
#

rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.*    ${WFLAGS}
	${RM} ${HDR}
        ${RM} ${RAMTARGET}
        ${RM} ${ROMTARGET}
        @echo ${COMPONENT}: cleaned
        

# ------------------------------------------------------------------------------
# ROM target (re-linked at ROM Image build time)
#

${ROMTARGET}: ${OBJS} ${HDR} ${LIBS} ${ROMCSTUBS} 
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}


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

${RAMTARGET}: ${OBJS} ${HDR} ${LIBS}
        ${LD} -rmf -o $@ ${OBJS} ${LIBS} ${CLIB}
        ${CHMOD} rm.${COMPONENT} ${CHMODFLAGS}

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.TextGadget:	c.TextGadget
o.TextGadget:	C:h.string
o.TextGadget:	C:h.kernel
o.TextGadget:	C:h.swis
o.TextGadget:	tbox:h.toolbox
o.TextGadget:	tbox:h.wimp
o.TextGadget:	tbox:h.window
o.TextGadget:	tbox:h.gadgets
o.TextGadget:	tbox:h.wimplib
o.TextGadget:	h.TextGadgets
o.TextGadget:	h.ModHdr
o.TextGadget:	h.glib
o.TextGadget:	C:h.kernel
o.TextGadget:	h.rmensure
o.TextGadget:	h.TextArea
o.TextGadget:	h.TextAreaP
o.TextGadget:	h.modmem
o.TextGadget:	h.TextArea
o.TextGadget:	h.TextMan
o.TextGadget:	h.MemMan
o.TextGadget:	C:h.kernel
o.TextGadget:	h.ScrollList
o.TextGadget:	h.ScrollLisP
o.TextGadget:	h.modmem
o.TextGadget:	h.ScrollList
o.TextGadget:	h.Scrollbar
o.TextGadget:	h.ScrollbarP
o.TextGadget:	h.modmem
o.TextGadget:	h.Scrollbar
o.TextGadget:	h.riscos_utils
o.TextGadget:	tbox:h.toolbox
o.TextGadget:	h.modmem
o.TextArea:	c.TextArea
o.TextArea:	C:h.kernel
o.TextArea:	C:h.swis
o.TextArea:	tbox:h.toolbox
o.TextArea:	tbox:h.wimp
o.TextArea:	tbox:h.window
o.TextArea:	tbox:h.gadgets
o.TextArea:	tbox:h.wimplib
o.TextArea:	h.glib
o.TextArea:	C:h.kernel
o.TextArea:	h.string32
o.TextArea:	h.riscos_utils
o.TextArea:	tbox:h.toolbox
o.TextArea:	h.TextGadgets
o.TextArea:	h.TextArea
o.TextArea:	h.TextAreaP
o.TextArea:	h.modmem
o.TextArea:	h.TextArea
o.TextArea:	h.TextMan
o.TextArea:	h.MemMan
o.TextArea:	C:h.kernel
o.TextArea:	h.Font
o.TextArea:	h.Text
o.TextArea:	h.Scrollbar
o.TextArea:	h.ScrollbarP
o.TextArea:	h.modmem
o.TextArea:	h.Scrollbar
o.TextArea:	h.ModHdr
o.TextArea:	h.utils
o.ScrollList:	c.ScrollList
o.ScrollList:	C:h.kernel
o.ScrollList:	C:h.swis
o.ScrollList:	tbox:h.toolbox
o.ScrollList:	tbox:h.wimp
o.ScrollList:	tbox:h.window
o.ScrollList:	tbox:h.gadgets
o.ScrollList:	tbox:h.wimplib
o.ScrollList:	h.glib
o.ScrollList:	C:h.kernel
o.ScrollList:	h.string32
o.ScrollList:	h.riscos_utils
o.ScrollList:	tbox:h.toolbox
o.ScrollList:	h.Font
o.ScrollList:	h.TextGadgets
o.ScrollList:	h.ScrollList
o.ScrollList:	h.ScrollLisP
o.ScrollList:	h.modmem
o.ScrollList:	h.ScrollList
o.ScrollList:	h.ScrollLisS
o.ScrollList:	h.modmem
o.ScrollList:	h.ScrollList
o.ScrollList:	h.debug
o.ScrollList:	h.ModHdr
o.ScrollList:	h.utils
o.Scrollbar:	c.Scrollbar
o.Scrollbar:	C:h.kernel
o.Scrollbar:	C:h.swis
o.Scrollbar:	tbox:h.toolbox
o.Scrollbar:	tbox:h.wimp
o.Scrollbar:	tbox:h.window
o.Scrollbar:	tbox:h.gadgets
o.Scrollbar:	tbox:h.wimplib
o.Scrollbar:	h.glib
o.Scrollbar:	C:h.kernel
o.Scrollbar:	h.string32
o.Scrollbar:	h.riscos_utils
o.Scrollbar:	tbox:h.toolbox
o.Scrollbar:	h.TextGadgets
o.Scrollbar:	h.Scrollbar
o.Scrollbar:	h.ScrollbarP
o.Scrollbar:	h.modmem
o.Scrollbar:	h.Scrollbar
o.Scrollbar:	h.debug
o.Scrollbar:	h.ModHdr
o.Scrollbar:	h.Utils
o.riscos_uti:	c.riscos_uti
o.riscos_uti:	C:h.kernel
o.riscos_uti:	C:h.swis
o.riscos_uti:	tbox:h.wimp
o.riscos_uti:	tbox:h.wimplib
o.riscos_uti:	tbox:h.toolbox
o.riscos_uti:	tbox:h.window
o.riscos_uti:	tbox:h.gadgets
o.riscos_uti:	h.riscos_utils
o.riscos_uti:	tbox:h.toolbox
o.riscos_gra: s.riscos_gra
o.riscos_gra: hdr.SWINames
o.Font: s.Font
o.Font: hdr.SWINames
o.debug: s.debug
o.debug: hdr.SWINames
o.TextMan:	c.TextMan
o.TextMan:	C:h.kernel
o.TextMan:	C:h.swis
o.TextMan:	h.TextMan
o.TextMan:	h.MemMan
o.TextMan:	C:h.kernel
o.MemMan:	c.MemMan
o.MemMan:	C:h.swis
o.MemMan:	C:h.kernel
o.MemMan:	C:h.kernel
o.MemMan:	h.MemMan
o.MemMan:	C:h.kernel
o.Text: s.Text
o.Text: hdr.SWINames
o.ScrollLisS: s.ScrollLisS
o.ScrollLisS: hdr.SWINames
o.Utils: s.Utils
o.Utils: hdr.SWINames
o.Utils: hdr.CVars
o.TAsel_ven: s.TAsel_ven
o.glib:	c.glib
o.glib:	C:h.kernel
o.glib:	C:h.swis
o.glib:	h.glib
o.glib:	C:h.kernel
o.glib:	tbox:h.wimp
o.glib:	tbox:h.toolbox
o.glib:	tbox:h.window
o.glib:	tbox:h.gadgets
o.glib:	h.glibP
o.glib:	tbox:h.toolbox
o.glib:	tbox:h.window
o.glib:	tbox:h.wimplib
o.glib3:	c.glib3
o.glib3:	C:h.kernel
o.glib3:	C:h.swis
o.glib3:	h.glib
o.glib3:	C:h.kernel
o.glib3:	tbox:h.wimp
o.glib3:	tbox:h.toolbox
o.glib3:	tbox:h.window
o.glib3:	tbox:h.gadgets
o.glib3:	h.glibP
o.glib3:	tbox:h.toolbox
o.glib3:	tbox:h.window
o.glib3:	tbox:h.wimplib
o.rmensure:	c.rmensure
o.rmensure:	C:h.kernel
o.rmensure:	C:h.swis
o.rmensure:	h.rmensure
o.string32:	c.string32
o.string32:	h.string32
