# Makefile for VideoCD Player App
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 20-Sep-95  SR           Created.
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
COMPONENT   = VideoCD
APP         = !VideoCD
APPLICATION = abs.!RunImage
TARGET      = aof.VideoCD
LDIR        = Resources.${LOCALE}

OBJS        =	\
 o.Main		\
 o.Initialise   \
 o.Messages     \
 o.Display      \
 o.Pointer      \
 o.Memory       \
 o.Help         \
 o.window       \
 o.swicall 

OBJSZ      =	\
 oz.Main	\
 oz.Initialise  \
 oz.Messages    \
 oz.Display     \
 oz.Pointer     \
 oz.Help        \
 oz.window      \
 o.swicall

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
	${CP} ${LDIR}.Messages  ${RESDIR}.Messages      ${CPFLAGS}
	${CP} ${LDIR}.!Boot     ${RESAPP}.!Boot         ${CPFLAGS}
	${CP} ${LDIR}.!Run      ${RESAPP}.!Run          ${CPFLAGS}
	${CP} ${LDIR}.Arrow     ${RESAPP}.Arrow         ${CPFLAGS}
	${CP} ${LDIR}.Backdrop  ${RESAPP}.Backdrop      ${CPFLAGS}
	${CP} ${LDIR}.Help      ${RESAPP}.Help          ${CPFLAGS}
	${CP} ${LDIR}.icons     ${RESAPP}.icons         ${CPFLAGS}
	${CP} ${LDIR}.Player    ${RESAPP}.Player        ${CPFLAGS}
	${CP} ${LDIR}.PlayerUI  ${RESAPP}.PlayerUI      ${CPFLAGS}
	${CP} ${LDIR}.Up        ${RESAPP}.Up            ${CPFLAGS}
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

rm.VideoCD: oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}
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
o.Main:	c.Main
o.Main:	h.Common
o.Main:	C:h.stdlib
o.Main:	h.Initialise
o.Main:	h.Display
o.Main:	h.Pointer
o.Main:	h.Help
o.Main:	h.window
o.Main:	h.Messages
o.Main:	C:h.stdio
o.Main:	C:h.stdlib
o.Main:	C:h.string
o.Main:	C:h.signal
o.Main:	C:h.time
o.Main:	h.swicall
o.Main:	C:h.kernel
o.Main:	C:h.swis
o.Initialise:	c.Initialise
o.Initialise:	h.Common
o.Initialise:	C:h.stdlib
o.Initialise:	h.Initialise
o.Initialise:	C:h.stdlib
o.Initialise:	C:h.stdio
o.Initialise:	C:h.signal
o.Initialise:	C:h.string
o.Initialise:	C:h.ctype
o.Initialise:	h.swicall
o.Initialise:	C:h.kernel
o.Initialise:	C:h.swis
o.Messages:	c.Messages
o.Messages:	C:h.stdlib
o.Messages:	C:h.stdio
o.Messages:	C:h.string
o.Messages:	h.swicall
o.Messages:	C:h.kernel
o.Messages:	C:h.swis
o.Messages:	h.Messages
o.Messages:	h.Common
o.Messages:	C:h.stdlib
o.Display:	c.Display
o.Display:	h.common
o.Display:	C:h.stdlib
o.Display:	h.display
o.Display:	C:h.stdio
o.Display:	C:h.string
o.Display:	h.swicall
o.Display:	C:h.kernel
o.Display:	C:h.swis
o.Pointer:	c.Pointer
o.Pointer:	h.Common
o.Pointer:	C:h.stdlib
o.Pointer:	h.Display
o.Pointer:	h.Pointer
o.Pointer:	C:h.stdlib
o.Pointer:	h.swicall
o.Pointer:	C:h.kernel
o.Pointer:	C:h.swis
o.Memory:	c.Memory
o.Memory:	h.Common
o.Memory:	C:h.stdlib
o.Memory:	h.Memory
o.Memory:	C:h.stdio
o.Memory:	h.swicall
o.Memory:	C:h.kernel
o.Memory:	C:h.swis
o.Help:	c.Help
o.Help:	h.Common
o.Help:	C:h.stdlib
o.Help:	h.Help
o.Help:	h.swicall
o.Help:	C:h.kernel
o.Help:	C:h.swis
o.window:	c.window
o.window:	h.common
o.window:	C:h.stdlib
o.window:	h.swicall
o.window:	C:h.kernel
o.window:	C:h.swis
o.window:	h.wimp
o.window:	C:h.kernel
o.window:	C:h.string
o.window:	h.display
o.window:	h.window
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
oz.Main:	c.Main
oz.Main:	h.Common
oz.Main:	C:h.stdlib
oz.Main:	h.Initialise
oz.Main:	h.Display
oz.Main:	h.Pointer
oz.Main:	h.Help
oz.Main:	h.window
oz.Main:	h.Messages
oz.Main:	C:h.stdio
oz.Main:	C:h.stdlib
oz.Main:	C:h.string
oz.Main:	C:h.signal
oz.Main:	C:h.time
oz.Main:	h.swicall
oz.Main:	C:h.kernel
oz.Main:	C:h.swis
oz.Initialise:	c.Initialise
oz.Initialise:	h.Common
oz.Initialise:	C:h.stdlib
oz.Initialise:	h.Initialise
oz.Initialise:	C:h.stdlib
oz.Initialise:	C:h.stdio
oz.Initialise:	C:h.signal
oz.Initialise:	C:h.string
oz.Initialise:	C:h.ctype
oz.Initialise:	h.swicall
oz.Initialise:	C:h.kernel
oz.Initialise:	C:h.swis
oz.Messages:	c.Messages
oz.Messages:	C:h.stdlib
oz.Messages:	C:h.stdio
oz.Messages:	C:h.string
oz.Messages:	h.swicall
oz.Messages:	C:h.kernel
oz.Messages:	C:h.swis
oz.Messages:	h.Messages
oz.Messages:	h.Common
oz.Messages:	C:h.stdlib
oz.Display:	c.Display
oz.Display:	h.common
oz.Display:	C:h.stdlib
oz.Display:	h.display
oz.Display:	C:h.stdio
oz.Display:	C:h.string
oz.Display:	h.swicall
oz.Display:	C:h.kernel
oz.Display:	C:h.swis
oz.Pointer:	c.Pointer
oz.Pointer:	h.Common
oz.Pointer:	C:h.stdlib
oz.Pointer:	h.Display
oz.Pointer:	h.Pointer
oz.Pointer:	C:h.stdlib
oz.Pointer:	h.swicall
oz.Pointer:	C:h.kernel
oz.Pointer:	C:h.swis
oz.Help:	c.Help
oz.Help:	h.Common
oz.Help:	C:h.stdlib
oz.Help:	h.Help
oz.Help:	h.swicall
oz.Help:	C:h.kernel
oz.Help:	C:h.swis
oz.window:	c.window
oz.window:	h.common
oz.window:	C:h.stdlib
oz.window:	h.swicall
oz.window:	C:h.kernel
oz.window:	C:h.swis
oz.window:	h.wimp
oz.window:	C:h.kernel
oz.window:	C:h.string
oz.window:	h.display
oz.window:	h.window
o.swicall: s.swicall
