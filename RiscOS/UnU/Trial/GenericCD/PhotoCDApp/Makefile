# Makefile for PhotoCD Player App
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
#

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -throwback -depend !Depend ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
Datatoaofflags = -d data -l len -dp -s -p

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
WRAPPER   = RISCOSLIB:s.ModuleWrap

#
# Include files
#
INCLUDES = -IC:

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESAPP = <resource$dir>.Apps.${APP}

#
# Program specific options:
#
COMPONENT   = PhotoCDApp
APP         = !PhotoCD
APPLICATION = abs.!RunImage
TARGET      = aof.PhotoCDApp
LDIR        = Resources.${LOCALE}

OBJS        = o.copy o.fx o.swicall o.main o.message o.render o.screen \
              o.spots o.utils o.vdu o.muldiv o.Scroll o.shuffle o.PCD \
              o.drawbase o.edgebase o.heap o.mainwindow libjpeg

OBJSZ      =  o.copy o.fx o.swicall oz.main oz.message oz.render oz.screen \
              oz.spots oz.utils oz.vdu o.muldiv oz.Scroll o.shuffle oz.PCD \
              o.drawbase o.edgebase oz.heap o.mainwindow libjpeg

#
# Rule patterns
#
.SUFFIXES: .oz .d
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -ff -zM -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.d.o:;      datatoaof -f $< -o $@ $(Datatoaofflags)

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
	${WIPE} o.* ${WFLAGS}
	${WIPE} oz.* ${WFLAGS}
	${WIPE} linked.* ${WFLAGS}
	${WIPE} map.* ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${APPLICATION}
	@echo ${COMPONENT}: cleaned

resources:
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESDIR}
	${CP} ${LDIR}.!Boot     ${RESAPP}.!Boot         ${CPFLAGS}
	${CP} ${LDIR}.!Run      ${RESAPP}.!Run          ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied

install: ${RESFILES} ${APPLICATION}
	${MKDIR} ${INSTDIR}
	${CP} ${LDIR}.!Boot     ${INSTDIR}.!Boot        ${CPFLAGS}
	${CP} ${LDIR}.!Run      ${INSTDIR}.!Run         ${CPFLAGS}
	@echo ${COMPONENT}: installed
	
#
# Static dependencies:
#
${APPLICATION}: ${OBJS} ${CLIB}
	${LD} -o $@ ${OBJS} ${CLIB}

rm.PhotoCD: oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}
	${LD} -o $@ -module oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: oz.ModuleWrap ${OBJSZ} ${ROMSTUBS}
	${LD} -o $@ -aof oz.ModuleWrap ${OBJSZ} ${ROMSTUBS}

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


oz.ModuleWrap: s.ModuleWrap s.AppName
	${AS} ${AFLAGS} s.ModuleWrap $@

s.ModuleWrap: ${WRAPPER}
        ${CP} ${WRAPPER} $@ ${CPFLAGS}

# Dynamic dependencies:
oz.ModuleWrap: s.ModuleWrap
oz.ModuleWrap: s.AppName
oz.ModuleWrap: Hdr:ListOpts
oz.ModuleWrap: Hdr:Macros
oz.ModuleWrap: Hdr:System
oz.ModuleWrap: Hdr:SWIs
oz.ModuleWrap: Hdr:CPU.Generic26
oz.ModuleWrap: Hdr:IO.GenericIO
oz.ModuleWrap: Hdr:RISCOS
oz.ModuleWrap: Hdr:ModHand
oz.ModuleWrap: Hdr:EnvNumbers
oz.ModuleWrap: Hdr:Proc
oz.ModuleWrap: Hdr:FSNumbers
oz.ModuleWrap: Hdr:NewErrors
oz.ModuleWrap: Hdr:Services
oz.ModuleWrap: Hdr:ResourceFS
oz.ModuleWrap: Hdr:MsgTrans
oz.ModuleWrap: Hdr:Variables
oz.ModuleWrap: Hdr:UpCall
oz.ModuleWrap: s.ResFiles
oz.main:	c.main
oz.main:	h.defs
oz.main:	C:h.kernel
oz.main:	h.objects
oz.main:	h.heap
oz.main:	C:h.kernel
oz.main:	h.inlines
oz.main:	h.local
oz.main:	C:h.swis
oz.main:	h.keys
oz.main:	h.local
oz.main:	h.message
oz.main:	h.PCD
oz.main:	h.render
oz.main:	h.screen
oz.main:	h.Defs
oz.main:	h.scroll
oz.main:	h.defs
oz.main:	h.spots
oz.main:	h.swicall
oz.main:	C:h.kernel
oz.main:	C:h.swis
oz.main:	h.utils
oz.main:	h.vdu
oz.main:	h.wimpstuff
oz.message:	c.message
oz.message:	h.swicall
oz.message:	C:h.kernel
oz.message:	C:h.swis
oz.message:	h.defs
oz.message:	C:h.kernel
oz.message:	h.objects
oz.message:	h.heap
oz.message:	C:h.kernel
oz.message:	h.inlines
oz.message:	h.local
oz.message:	C:h.swis
oz.message:	h.message
oz.message:	h.screen
oz.message:	h.Defs
oz.message:	h.utils
oz.render:	c.render
oz.render:	h.defs
oz.render:	C:h.kernel
oz.render:	h.objects
oz.render:	h.inlines
oz.render:	h.local
oz.render:	C:h.swis
oz.render:	h.local
oz.render:	h.PCD
oz.render:	h.render
oz.render:	h.screen
oz.render:	h.Defs
oz.render:	h.spots
oz.render:	h.swicall
oz.render:	C:h.kernel
oz.render:	C:h.swis
oz.render:	h.utils
oz.screen:	c.screen
oz.screen:	h.copy
oz.screen:	h.defs
oz.screen:	C:h.kernel
oz.screen:	h.objects
oz.screen:	h.filter
oz.screen:	h.fx
oz.screen:	h.heap
oz.screen:	C:h.kernel
oz.screen:	h.inlines
oz.screen:	h.local
oz.screen:	C:h.swis
oz.screen:	h.objects
oz.screen:	h.render
oz.screen:	h.spots
oz.screen:	h.swicall
oz.screen:	C:h.kernel
oz.screen:	C:h.swis
oz.screen:	h.utils
oz.screen:	h.vdu
oz.spots:	c.spots
oz.spots:	h.defs
oz.spots:	C:h.kernel
oz.spots:	h.objects
oz.spots:	C:h.kernel
oz.spots:	h.muldiv
oz.spots:	h.swicall
oz.spots:	C:h.kernel
oz.spots:	C:h.swis
oz.utils:	c.utils
oz.utils:	h.swicall
oz.utils:	C:h.kernel
oz.utils:	C:h.swis
oz.utils:	h.defs
oz.utils:	C:h.kernel
oz.utils:	h.objects
oz.utils:	h.heap
oz.utils:	C:h.kernel
oz.utils:	h.inlines
oz.utils:	h.local
oz.utils:	C:h.swis
oz.utils:	h.screen
oz.utils:	h.Defs
oz.utils:	h.keys
oz.vdu:	c.vdu
oz.vdu:	h.swicall
oz.vdu:	C:h.kernel
oz.vdu:	C:h.swis
oz.vdu:	h.defs
oz.vdu:	C:h.kernel
oz.vdu:	h.objects
oz.vdu:	h.inlines
oz.vdu:	h.local
oz.vdu:	C:h.swis
oz.Scroll:	c.Scroll
oz.Scroll:	h.Defs
oz.Scroll:	C:h.kernel
oz.Scroll:	h.objects
oz.Scroll:	h.inlines
oz.Scroll:	h.local
oz.Scroll:	C:h.swis
oz.Scroll:	h.render
oz.Scroll:	h.screen
oz.Scroll:	h.Defs
oz.Scroll:	h.scroll
oz.Scroll:	h.defs
oz.Scroll:	h.swicall
oz.Scroll:	C:h.kernel
oz.Scroll:	C:h.swis
oz.Scroll:	C:h.swis
oz.Scroll:	h.utils
oz.PCD:	c.PCD
oz.PCD:	h.Defs
oz.PCD:	C:h.kernel
oz.PCD:	h.objects
oz.PCD:	h.Heap
oz.PCD:	C:h.kernel
oz.PCD:	h.inlines
oz.PCD:	h.local
oz.PCD:	C:h.swis
oz.PCD:	h.jerror
oz.PCD:	h.jinclude
oz.PCD:	h.jconfig
oz.PCD:	h.jpeglib
oz.PCD:	h.jmorecfg
oz.PCD:	h.local
oz.PCD:	h.screen
oz.PCD:	h.Defs
oz.PCD:	h.swicall
oz.PCD:	C:h.kernel
oz.PCD:	C:h.swis
oz.PCD:	h.utils
oz.heap:	c.heap
oz.heap:	h.Defs
oz.heap:	C:h.kernel
oz.heap:	h.objects
oz.heap:	h.swicall
oz.heap:	C:h.kernel
oz.heap:	C:h.swis
oz.heap:	h.utils
o.copy: s.copy
o.fx: s.fx
o.swicall: s.swicall
o.main:	c.main
o.main:	h.defs
o.main:	C:h.kernel
o.main:	h.objects
o.main:	h.heap
o.main:	C:h.kernel
o.main:	h.inlines
o.main:	h.local
o.main:	C:h.swis
o.main:	h.keys
o.main:	h.local
o.main:	h.message
o.main:	h.PCD
o.main:	h.render
o.main:	h.screen
o.main:	h.Defs
o.main:	h.scroll
o.main:	h.defs
o.main:	h.spots
o.main:	h.swicall
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.utils
o.main:	h.vdu
o.main:	h.wimpstuff
o.message:	c.message
o.message:	h.swicall
o.message:	C:h.kernel
o.message:	C:h.swis
o.message:	h.defs
o.message:	C:h.kernel
o.message:	h.objects
o.message:	h.heap
o.message:	C:h.kernel
o.message:	h.inlines
o.message:	h.local
o.message:	C:h.swis
o.message:	h.message
o.message:	h.screen
o.message:	h.Defs
o.message:	h.utils
o.render:	c.render
o.render:	h.defs
o.render:	C:h.kernel
o.render:	h.objects
o.render:	h.inlines
o.render:	h.local
o.render:	C:h.swis
o.render:	h.local
o.render:	h.PCD
o.render:	h.render
o.render:	h.screen
o.render:	h.Defs
o.render:	h.spots
o.render:	h.swicall
o.render:	C:h.kernel
o.render:	C:h.swis
o.render:	h.utils
o.screen:	c.screen
o.screen:	h.copy
o.screen:	h.defs
o.screen:	C:h.kernel
o.screen:	h.objects
o.screen:	h.filter
o.screen:	h.fx
o.screen:	h.heap
o.screen:	C:h.kernel
o.screen:	h.inlines
o.screen:	h.local
o.screen:	C:h.swis
o.screen:	h.objects
o.screen:	h.render
o.screen:	h.spots
o.screen:	h.swicall
o.screen:	C:h.kernel
o.screen:	C:h.swis
o.screen:	h.utils
o.screen:	h.vdu
o.spots:	c.spots
o.spots:	h.defs
o.spots:	C:h.kernel
o.spots:	h.objects
o.spots:	C:h.kernel
o.spots:	h.muldiv
o.spots:	h.swicall
o.spots:	C:h.kernel
o.spots:	C:h.swis
o.utils:	c.utils
o.utils:	h.swicall
o.utils:	C:h.kernel
o.utils:	C:h.swis
o.utils:	h.defs
o.utils:	C:h.kernel
o.utils:	h.objects
o.utils:	h.heap
o.utils:	C:h.kernel
o.utils:	h.inlines
o.utils:	h.local
o.utils:	C:h.swis
o.utils:	h.screen
o.utils:	h.Defs
o.utils:	h.keys
o.vdu:	c.vdu
o.vdu:	h.swicall
o.vdu:	C:h.kernel
o.vdu:	C:h.swis
o.vdu:	h.defs
o.vdu:	C:h.kernel
o.vdu:	h.objects
o.vdu:	h.inlines
o.vdu:	h.local
o.vdu:	C:h.swis
o.muldiv: s.muldiv
o.Scroll:	c.Scroll
o.Scroll:	h.Defs
o.Scroll:	C:h.kernel
o.Scroll:	h.objects
o.Scroll:	h.inlines
o.Scroll:	h.local
o.Scroll:	C:h.swis
o.Scroll:	h.render
o.Scroll:	h.screen
o.Scroll:	h.Defs
o.Scroll:	h.scroll
o.Scroll:	h.defs
o.Scroll:	h.swicall
o.Scroll:	C:h.kernel
o.Scroll:	C:h.swis
o.Scroll:	C:h.swis
o.Scroll:	h.utils
o.shuffle: s.shuffle
o.shuffle: h.regdefs
o.shuffle: h.swinos
o.shuffle: h.macros
o.PCD:	c.PCD
o.PCD:	h.Defs
o.PCD:	C:h.kernel
o.PCD:	h.objects
o.PCD:	h.Heap
o.PCD:	C:h.kernel
o.PCD:	h.inlines
o.PCD:	h.local
o.PCD:	C:h.swis
o.PCD:	h.jerror
o.PCD:	h.jinclude
o.PCD:	h.jconfig
o.PCD:	h.jpeglib
o.PCD:	h.jmorecfg
o.PCD:	h.local
o.PCD:	h.screen
o.PCD:	h.Defs
o.PCD:	h.swicall
o.PCD:	C:h.kernel
o.PCD:	C:h.swis
o.PCD:	h.utils
o.heap:	c.heap
o.heap:	h.Defs
o.heap:	C:h.kernel
o.heap:	h.objects
o.heap:	h.swicall
o.heap:	C:h.kernel
o.heap:	C:h.swis
o.heap:	h.utils
