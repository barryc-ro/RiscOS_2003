# Makefile for ScrSaver
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
SHRINK  = ShrinkWrapZipCompress

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS}
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

#
# Include files
#
INCLUDES = -IC:

LIBS     =

DFLAGS   =

#
# Program specific options:
#
COMPONENT = ScrSaver
TARGET    = aof.${COMPONENT}
APPTARGET = abs.!RunImage
RAMTARGET = rm.${COMPONENT}
OBJS      = o.header o.module
APPOBJS   = o.app
EXPORTS   = 

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: ${RAMTARGET} ${APPTARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${RAMTARGET}
        ${RM} ${APPTARGET}
        @echo ${COMPONENT}: cleaned

resources: ${APPTARGET}
        ${MKDIR} ${RESDIR}.ScrSaver
        ${CP} ${APPTARGET}                     ${RESDIR}.ScrSaver.!RunImage  ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Run         ${RESDIR}.ScrSaver.!Run       ${CPFLAGS}
        ${CP} Resources.${LOCALE}.Templates    ${RESDIR}.ScrSaver.Templates  ${CPFLAGS}
        ${SHRINK} Resources.${LOCALE}.Sprites  ${RESDIR}.ScrSaver.Sprites
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

${APPTARGET}: ${APPOBJS}
        ${LD} -aif -o $@ ${APPOBJS} ${CLIB} ${RLIB}

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

${RAMTARGET}: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.module:	c.module
o.module:	C:h.kernel
o.module:	h.module
o.app:	c.app
o.app:	C:h.swis
o.app:	C:h.kernel
o.app:	C:h.time
o.app:	C:h.math
o.app:	C:h.float
o.app:	C:h.os
o.app:	C:h.wimp
o.app:	C:h.sprite
o.app:	C:h.wimpt
o.app:	C:h.win
o.app:	C:h.res
o.app:	C:h.template
o.app:	C:h.event
o.app:	C:h.menu
o.app:	C:h.sprite
o.app:	C:h.bbc
o.app:	C:h.colourtran
o.app:	C:h.font
o.app:	C:h.drawmod
