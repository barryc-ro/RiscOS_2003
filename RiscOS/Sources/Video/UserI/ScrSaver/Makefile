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
.cmhg.o:;   ${CMHG} -p -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: ${RAMTARGET} ${APPTARGET}

# Binary Build Environment augmenting rules
BBETYPE   = local
bbe-local: bbe-generic-resources-get-alias
  ${MKDIR} ${BBEDIR}.Resources
  BBE_Export_Dir abs
  BBE_Export_File Resources.!Run
  BBE_Export_File Resources.Templates
  BBE_Export_Dir  Resources.${LOCALE}.${USERIF}.${UI_VARIANT}
  BBE_Export_Dir  Resources.UK.${USERIF}

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
        ${RM} ${TARGET}
        ${RM} ${RAMTARGET}
        ${RM} ${APPTARGET}
        @echo ${COMPONENT}: cleaned

resources: ${APPTARGET}
        Set ScrSaverRes$Path Resources.${Locale}.${UserIF}.${UI_VARIANT}.,Resources.{Locale}.{UserIF}.,Resources.UK.{UserIF}.${UI_VARIANT}.,Resources.UK.{UserIF}.
        ${MKDIR} ${RESDIR}.ScrSaver
        ${CP} ${APPTARGET}                    ${RESDIR}.ScrSaver.!RunImage  ${CPFLAGS}
        ${CP} Resources.!Run                  ${RESDIR}.ScrSaver.!Run       ${CPFLAGS}
        ${CP} ScrSaverRes:Messages            ${RESDIR}.ScrSaver.Messages   ${CPFLAGS}
        ${CP} ScrSaverRes:Sprites             ${RESDIR}.ScrSaver.Sprites    ${CPFLAGS}
        ${CP} Resources.Templates             ${RESDIR}.ScrSaver.Templates  ${CPFLAGS}
        UnSet ScrSaverRes$Path
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

${APPTARGET}: ${APPOBJS}
	${MKDIR} abs
        ${LD} -aif -o $@ ${APPOBJS} ${CLIB} ${RLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${RAMTARGET}: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
