# Makefile for STBFilter
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
CCFLAGS = -c ${INCLUDES} ${DFLAGS} -zM -depend !Depend
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = STBFilter
TARGET     = aof.STBFilter
MODULE     = rm.STBFilter

#
# Libraries
#
CLIB      = C:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ABSSYM    = RISCOSLIB:o.abssym

#
# Include files
#
INCLUDES = -IC:

OBJS =\
 o.mheader\
 o.module\
 o.filter

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@
.cmhg.o:;   ${CMHG} $< $@


#
# Build rules:
#
all: ${MODULE}
        @echo ${COMPONENT}: all built (disc)

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${LD} -o $@ -Module ${OBJS} ${CLIB}


#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

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


${MODULE}: ${OBJS} ${CLIB}
        ${LD} -o $@ -Module ${OBJS} ${CLIB}


# Dynamic dependencies:
o.module:	c.module
o.module:	h.module
o.module:	C:h.kernel
o.module:	C:h.kernel
o.module:	h.filter
o.filter: s.filter
