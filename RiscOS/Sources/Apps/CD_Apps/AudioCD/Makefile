# Makefile for AudioCD Player App
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-Aug-95  JRH          Created.
# 11-Sep-95  SR           Added inclusion of window.c
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
COMPONENT   = AudioCD
APP         = !AudioCD
APPLICATION = abs.!RunImage
TARGET      = aof.AudioCD
LDIR        = Resources.${LOCALE}

OBJS        =	\
 o.main		\
 o.Initialise	\
 o.Messages	\
 o.ButtonOps	\
 o.DriveOps	\
 o.Display	\
 o.Random       \
 o.Help         \
 o.window       \
 o.Memory       \
 o.swicall 

OBJSZ      =	\
 oz.main	\
 oz.Initialise	\
 oz.Messages	\
 oz.ButtonOps	\
 oz.DriveOps	\
 oz.Display	\
 oz.Random      \
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
	${CP} ${LDIR}.Buttons   ${RESAPP}.Buttons       ${CPFLAGS}
	${CP} ${LDIR}.Player    ${RESAPP}.Player        ${CPFLAGS}
	${CP} ${LDIR}.Help      ${RESAPP}.Help          ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied

install: ${RESFILES} ${APPLICATION}
	${MKDIR} ${INSTDIR}
	${CP} ${LDIR}.!Boot     ${INSTDIR}.!Boot        ${CPFLAGS}
	${CP} ${LDIR}.!Run      ${INSTDIR}.!Run         ${CPFLAGS}
	${CP} ${LDIR}.Buttons   ${INSTDIR}.Buttons      ${CPFLAGS}
	${CP} ${LDIR}.Player    ${INSTDIR}.Player       ${CPFLAGS}
	${CP} ${LDIR}.Help      ${INSTDIR}.Help         ${CPFLAGS}
	@echo ${COMPONENT}: installed
	
#
# Static dependencies:
#
${APPLICATION}: ${OBJS} ${CLIB}
	${LD} -o $@ ${OBJS} ${CLIB}

rm.AudioCD: oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}
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
o.main:	c.main
o.main:	h.Main
o.main:	h.Common
o.main:	C:h.stdlib
o.main:	h.Initialise
o.main:	h.ButtonOps
o.main:	h.DriveOps
o.main:	h.Display
o.main:	h.Random
o.main:	h.Help
o.main:	h.Messages
o.main:	C:h.ctype
o.main:	C:h.time
o.main:	C:h.signal
o.main:	C:h.stdlib
o.main:	C:h.stdio
o.main:	h.window
o.main:	h.swicall
o.main:	C:h.kernel
o.main:	C:h.swis
o.Initialise:	c.Initialise
o.Initialise:	h.Initialise
o.Initialise:	h.Common
o.Initialise:	C:h.stdlib
o.Initialise:	h.Display
o.Initialise:	h.Messages
o.Initialise:	h.ButtonOps
o.Initialise:	C:h.signal
o.Initialise:	h.swicall
o.Initialise:	C:h.kernel
o.Initialise:	C:h.swis
o.Messages:	c.Messages
o.Messages:	h.Common
o.Messages:	C:h.stdlib
o.Messages:	h.Messages
o.Messages:	C:h.stdio
o.Messages:	C:h.string
o.Messages:	h.swicall
o.Messages:	C:h.kernel
o.Messages:	C:h.swis
o.ButtonOps:	c.ButtonOps
o.ButtonOps:	h.ButtonOps
o.ButtonOps:	h.Common
o.ButtonOps:	C:h.stdlib
o.ButtonOps:	h.DriveOps
o.ButtonOps:	h.Display
o.ButtonOps:	C:h.time
o.ButtonOps:	h.swicall
o.ButtonOps:	C:h.kernel
o.ButtonOps:	C:h.swis
o.DriveOps:	c.DriveOps
o.DriveOps:	h.DriveOps
o.DriveOps:	h.Common
o.DriveOps:	C:h.stdlib
o.DriveOps:	h.Display
o.DriveOps:	h.swicall
o.DriveOps:	C:h.kernel
o.DriveOps:	C:h.swis
o.Display:	c.Display
o.Display:	h.Display
o.Display:	h.Common
o.Display:	C:h.stdlib
o.Display:	h.DriveOps
o.Display:	h.Main
o.Display:	h.ButtonOps
o.Display:	C:h.stdio
o.Display:	C:h.string
o.Display:	C:h.time
o.Display:	h.swicall
o.Display:	C:h.kernel
o.Display:	C:h.swis
o.Random:	c.Random
o.Random:	h.Common
o.Random:	C:h.stdlib
o.Random:	h.Random
o.Random:	C:h.time
o.Help:	c.Help
o.Help:	h.Common
o.Help:	C:h.stdlib
o.Help:	h.Help
o.Help:	h.Initialise
o.Help:	h.Display
o.Help:	h.ButtonOps
o.Help:	h.swicall
o.Help:	C:h.kernel
o.Help:	C:h.swis
o.window:	c.window
o.window:	h.swicall
o.window:	C:h.kernel
o.window:	C:h.swis
o.window:	h.wimp
o.window:	C:h.kernel
o.window:	C:h.string
o.window:	h.Initialise
o.window:	h.common
o.window:	C:h.stdlib
o.window:	h.Display
o.window:	C:h.stdio
o.Memory:	c.Memory
o.Memory:	h.Common
o.Memory:	C:h.stdlib
o.Memory:	C:h.stdio
o.Memory:	h.swicall
o.Memory:	C:h.kernel
o.Memory:	C:h.swis
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
oz.main:	h.Main
oz.main:	h.Common
oz.main:	C:h.stdlib
oz.main:	h.Initialise
oz.main:	h.ButtonOps
oz.main:	h.DriveOps
oz.main:	h.Display
oz.main:	h.Random
oz.main:	h.Help
oz.main:	h.Messages
oz.main:	C:h.ctype
oz.main:	C:h.time
oz.main:	C:h.signal
oz.main:	C:h.stdlib
oz.main:	C:h.stdio
oz.main:	h.window
oz.main:	h.swicall
oz.main:	C:h.kernel
oz.main:	C:h.swis
oz.Initialise:	c.Initialise
oz.Initialise:	h.Initialise
oz.Initialise:	h.Common
oz.Initialise:	C:h.stdlib
oz.Initialise:	h.Display
oz.Initialise:	h.Messages
oz.Initialise:	h.ButtonOps
oz.Initialise:	C:h.signal
oz.Initialise:	h.swicall
oz.Initialise:	C:h.kernel
oz.Initialise:	C:h.swis
oz.Messages:	c.Messages
oz.Messages:	h.Common
oz.Messages:	C:h.stdlib
oz.Messages:	h.Messages
oz.Messages:	C:h.stdio
oz.Messages:	C:h.string
oz.Messages:	h.swicall
oz.Messages:	C:h.kernel
oz.Messages:	C:h.swis
oz.ButtonOps:	c.ButtonOps
oz.ButtonOps:	h.ButtonOps
oz.ButtonOps:	h.Common
oz.ButtonOps:	C:h.stdlib
oz.ButtonOps:	h.DriveOps
oz.ButtonOps:	h.Display
oz.ButtonOps:	C:h.time
oz.ButtonOps:	h.swicall
oz.ButtonOps:	C:h.kernel
oz.ButtonOps:	C:h.swis
oz.DriveOps:	c.DriveOps
oz.DriveOps:	h.DriveOps
oz.DriveOps:	h.Common
oz.DriveOps:	C:h.stdlib
oz.DriveOps:	h.Display
oz.DriveOps:	h.swicall
oz.DriveOps:	C:h.kernel
oz.DriveOps:	C:h.swis
oz.Display:	c.Display
oz.Display:	h.Display
oz.Display:	h.Common
oz.Display:	C:h.stdlib
oz.Display:	h.DriveOps
oz.Display:	h.Main
oz.Display:	h.ButtonOps
oz.Display:	C:h.stdio
oz.Display:	C:h.string
oz.Display:	C:h.time
oz.Display:	h.swicall
oz.Display:	C:h.kernel
oz.Display:	C:h.swis
oz.Random:	c.Random
oz.Random:	h.Common
oz.Random:	C:h.stdlib
oz.Random:	h.Random
oz.Random:	C:h.time
oz.Help:	c.Help
oz.Help:	h.Common
oz.Help:	C:h.stdlib
oz.Help:	h.Help
oz.Help:	h.Initialise
oz.Help:	h.Display
oz.Help:	h.ButtonOps
oz.Help:	h.swicall
oz.Help:	C:h.kernel
oz.Help:	C:h.swis
oz.window:	c.window
oz.window:	h.swicall
oz.window:	C:h.kernel
oz.window:	C:h.swis
oz.window:	h.wimp
oz.window:	C:h.kernel
oz.window:	C:h.string
oz.window:	h.Initialise
oz.window:	h.common
oz.window:	C:h.stdlib
oz.window:	h.Display
oz.window:	C:h.stdio
o.swicall: s.swicall
