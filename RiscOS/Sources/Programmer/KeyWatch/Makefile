# Makefile for KeyWatch
#

#
# Paths
#
EXP_HDR = <Cexport$dir>

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -fah ${INCLUDES} ${DFLAGS}
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

DFLAGS   =

#
# Program specific options:
#
COMPONENT = KeyWatch
TARGET    = aof.KeyWatch

RM	  = rm.KeyWatch
RMD	  = rm.KeyWatchD

OBJS      = o.KeyWatchHd o.KeyWatch
OBJSD     = o.KeyWatchHd od.KeyWatch

LIBS     =
LIBSD    = C:debug.o.remotezm

#
# Rule patterns
#
.SUFFIXES:  .od

.c.o:;      ${CC} ${CFLAGS} -ff -o $@ $<
.c.od:;     ${CC} ${CFLAGS} -DDEBUG -DREMOTE_DEBUG -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
.cmhg.h:;   ${CMHG} -p -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.KeyWatch h.KeyWatchHd

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

export_hdrs: h.${COMPONENT}
        ${CP} h.${COMPONENT} ${EXP_HDR}.h.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: export complete (headers)

export_libs: o.filewatch
        @echo ${COMPONENT}: export complete (libs)

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} od.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} h.KeyWatchHd
        @echo ${COMPONENT}: cleaned

resources:
#        ${MKDIR} ${RESDIR}.KeyWatch
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.KeyWatch.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

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

${RM}: ${OBJS} ${LIBSD} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${RMD}: ${OBJSD} ${LIBSD} ${CLIB}
        ${LD} -o $@ -module ${OBJSD} ${LIBSD} ${CLIB}

KeyWatch.c: KeyWatchHd.h

# Dynamic dependencies:
