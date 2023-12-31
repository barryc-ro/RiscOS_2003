# Makefile for Chrontel
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
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS} ${THROWBACK}
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

#DFLAGS   = -DDEBUG
DFLAGS   = -D${Machine}

#
# Program specific options:
#
COMPONENT = Chrontel
TARGET    = aof.Ctel${TYPE}
BASEOBJS  = header.o module.o chrontel.o debugit.o
EXPORTS   = 

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
.cmhg.h:;   ${CMHG} -p -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Other objects
#
fsNone.o: fsynth.c
	${CC} ${CFLAGS} -o $@ fsynth.c -DFSYNTH_NONE

fsCtel.o: fsynth.c
	${CC} ${CFLAGS} -o $@ fsynth.c -DFSYNTH_CHRONTEL

fsICS.o: fsynth.c
	${CC} ${CFLAGS} -o $@ fsynth.c -DFSYNTH_ICS

fsICS2.o: fsynth.c
	${CC} ${CFLAGS} -o $@ fsynth.c -DFSYNTH_ICS2

module.o: header.h

#
# build a relocatable module:
#
all: rm.CtelNone rm.CtelCtel rm.CtelICS rm.CtelICS2

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
	@echo ${COMPONENT}: rom module built

ram: rm.Ctel${TYPE}
	@echo ${COMPONENT}: ram module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} h.header
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
aof.CtelNone: ${BASEOBJS} fsNone.o ${ROMCSTUBS}
        ${LD} -o $@ -aof ${BASEOBJS} fsNone.o ${ROMCSTUBS}

aof.CtelCtel: ${BASEOBJS} fsCtel.o ${ROMCSTUBS}
        ${LD} -o $@ -aof ${BASEOBJS} fsCtel.o ${ROMCSTUBS}

aof.CtelICS: ${BASEOBJS} fsICS.o ${ROMCSTUBS}
        ${LD} -o $@ -aof ${BASEOBJS} fsICS.o ${ROMCSTUBS}

aof.CtelICS2: ${BASEOBJS} fsICS2.o ${ROMCSTUBS}
        ${LD} -o $@ -aof ${BASEOBJS} fsICS2.o ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.CtelNone: ${BASEOBJS} fsNone.o ${CLIB}
        ${LD} -o $@ -module ${BASEOBJS} fsNone.o ${CLIB}

rm.CtelCtel: ${BASEOBJS} fsCtel.o ${CLIB}
        ${LD} -o $@ -module ${BASEOBJS} fsCtel.o ${CLIB}

rm.CtelICS: ${BASEOBJS} fsICS.o ${CLIB}
        ${LD} -o $@ -module ${BASEOBJS} fsICS.o ${CLIB}

rm.CtelICS2: ${BASEOBJS} fsICS2.o ${CLIB}
        ${LD} -o $@ -module ${BASEOBJS} fsICS2.o ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
