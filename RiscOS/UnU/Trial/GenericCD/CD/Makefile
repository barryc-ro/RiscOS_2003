# Makefile for CD Player App
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-Aug-95  JRH          Created.
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
COMPONENT   = CD
APP         = !CD
APPLICATION = abs.!RunImage
TARGET      = aof.CD
LDIR        = Resources.${LOCALE}

OBJS        =	\
 o.main		\
 o.messages	\
 o.display	\
 o.cd_type	\
 o.init		\
 o.launch	\
 o.cdpresent?	\
 o.window	\
 o.screens      \
 o.memory       \
 o.swicall 

OBJSZ      =	\
 oz.main	\
 oz.messages	\
 oz.display	\
 oz.cd_type	\
 oz.init	\
 oz.launch	\
 oz.cdpresent?	\
 oz.window	\
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
	${CP} ${LDIR}.Splash    ${RESAPP}.Splash        ${CPFLAGS}
	${CP} ${LDIR}.UnknownCD ${RESAPP}.UnknownCD     ${CPFLAGS}
	${CP} ${LDIR}.FaultyCD  ${RESAPP}.FaultyCD      ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied

install: ${RESFILES} ${APPLICATION}
	${MKDIR} ${INSTDIR}
	${CP} ${LDIR}.!Boot     ${INSTDIR}.!Boot        ${CPFLAGS}
	${CP} ${LDIR}.!Run      ${INSTDIR}.!Run         ${CPFLAGS}
	${CP} ${LDIR}.Splash    ${INSTDIR}.Splash       ${CPFLAGS}
	${CP} ${LDIR}.UnknownCD ${INSTDIR}.UnknownCD    ${CPFLAGS}
	${CP} ${LDIR}.FaultyCD  ${INSTDIR}.FaultyCD     ${CPFLAGS}
	@echo ${COMPONENT}: installed
	
#
# Static dependencies:
#
${APPLICATION}: ${OBJS} ${CLIB}
	${LD} -o $@ ${OBJS} ${CLIB}

rm.CD: oz.ModuleWrap ${OBJSZ} ${RLIB} ${CLIB}
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
o.main:	h.main
o.main:	h.common
o.main:	C:h.stdlib
o.main:	C:h.stdlib
o.main:	C:h.stdio
o.main:	C:h.stdlib
o.main:	C:h.string
o.main:	C:h.signal
o.main:	C:h.time
o.main:	h.swicall
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.wimp
o.main:	C:h.kernel
o.main:	h.common
o.main:	C:h.stdlib
o.main:	h.init
o.main:	h.cd_type
o.main:	h.launch
o.main:	h.cdpresent?
o.main:	h.display
o.main:	h.messages
o.main:	h.window
o.messages:	c.messages
o.messages:	h.messages
o.messages:	C:h.stdlib
o.messages:	C:h.stdio
o.messages:	C:h.string
o.messages:	h.common
o.messages:	C:h.stdlib
o.messages:	h.display
o.messages:	h.swicall
o.messages:	C:h.kernel
o.messages:	C:h.swis
o.messages:	h.window
o.display:	c.display
o.display:	h.common
o.display:	C:h.stdlib
o.display:	h.display
o.display:	C:h.stdio
o.display:	C:h.stdlib
o.display:	h.swicall
o.display:	C:h.kernel
o.display:	C:h.swis
o.cd_type:	c.cd_type
o.cd_type:	h.cd_type
o.cd_type:	h.common
o.cd_type:	C:h.stdlib
o.cd_type:	C:h.stdio
o.cd_type:	C:h.string
o.cd_type:	C:h.ctype
o.cd_type:	h.swicall
o.cd_type:	C:h.kernel
o.cd_type:	C:h.swis
o.init:	c.init
o.init:	h.common
o.init:	C:h.stdlib
o.init:	h.init
o.init:	C:h.stdio
o.init:	C:h.signal
o.init:	h.swicall
o.init:	C:h.kernel
o.init:	C:h.swis
o.launch:	c.launch
o.launch:	h.launch
o.launch:	h.common
o.launch:	C:h.stdlib
o.launch:	h.swicall
o.launch:	C:h.kernel
o.launch:	C:h.swis
o.launch:	h.messages
o.launch:	h.display
o.launch:	h.main
o.launch:	h.common
o.launch:	C:h.stdlib
o.launch:	C:h.stdlib
o.launch:	C:h.stdio
o.launch:	C:h.stdlib
o.launch:	C:h.string
o.cdpresent?:	c.cdpresent?
o.cdpresent?:	h.cdpresent?
o.cdpresent?:	h.common
o.cdpresent?:	C:h.stdlib
o.cdpresent?:	h.main
o.cdpresent?:	h.common
o.cdpresent?:	C:h.stdlib
o.cdpresent?:	C:h.stdlib
o.cdpresent?:	h.display
o.cdpresent?:	h.window
o.cdpresent?:	C:h.time
o.cdpresent?:	h.swicall
o.cdpresent?:	C:h.kernel
o.cdpresent?:	C:h.swis
o.window:	c.window
o.window:	h.swicall
o.window:	C:h.kernel
o.window:	C:h.swis
o.window:	h.wimp
o.window:	C:h.kernel
o.window:	C:h.string
o.window:	h.display
o.window:	h.common
o.window:	C:h.stdlib
o.screens:	c.screens
o.screens:	h.common
o.screens:	C:h.stdlib
o.screens:	h.swicall
o.screens:	C:h.kernel
o.screens:	C:h.swis
o.screens:	C:h.stdio
o.memory:	c.memory
o.memory:	h.Common
o.memory:	C:h.stdlib
o.memory:	C:h.stdio
o.memory:	h.swicall
o.memory:	C:h.kernel
o.memory:	C:h.swis
o.swicall: s.swicall
