# Makefile for KeyWatch
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
INCLUDES = -I<Lib$Dir>.tboxlibs. -IC:

LIBS     =

DFLAGS   =

#
# Program specific options:
#
COMPONENT = KeyWatch
TARGET    = aof.KeyWatch
OBJS      = o.KeyWatchHd o.KeyWatch
EXPORTS   = 

#
# Rule patterns
#
SUFFIXES=   .cmhg

.c.o:;      ${CC} ${CFLAGS} -o $@ $<
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

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
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

rm.KeyWatch: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.KeyWatch:	c.KeyWatch
o.KeyWatch:	C:h.kernel
o.KeyWatch:	C:h.swis
o.KeyWatch:	<Lib$Dir>.tboxlibs.h.wimp
o.KeyWatch:	h.KeyWatchHd
o.KeyWatch:	VersionNum
o.KeyWatch:	c.KeyWatch
o.KeyWatch:	C:h.kernel
o.KeyWatch:	C:h.swis
o.KeyWatch:	<Lib$Dir>.tboxlibs.h.wimp
o.KeyWatch:	h.KeyWatchHd
o.KeyWatch:	VersionNum
