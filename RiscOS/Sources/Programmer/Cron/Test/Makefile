# Makefile for CronTest
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Mar-1997  RWB 	  New for CSFS
# 07-May-1997  NDT        Stolen for CronTest

#
# Paths
#

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
CD	= dir
DEFMOD  = DefMod
AR	= LibFile

AFLAGS = -depend !Depend -Stamp -quit ${THROWBACK}
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS}
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES} ${THROWBACK}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
DFLAGS  = ${DEBUG} ${MESSAGES}
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
#
# Include files
#
INCLUDES = -IC:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = CronTest
TARGET    = aof.${COMPONENT}

OBJS      =	\
o.crontest	\
o.header	\

#
# Rule patterns
#

.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} $< $@ h.$*
.s.o:;      ${AS} ${AFLAGS}  $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT} dirs
        @echo ${COMPONENT}: all complete
#
# RISC OS ROM build rules:
#
rom: ${TARGET} dirs
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        @echo ${COMPONENT}: resource files copied

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned
dirs:
	${MKDIR} rm
	${MKDIR} o
	${MKDIR} linked
	${MKDIR} map
	${MKDIR} aof

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}: ${OBJS} ${CLIB}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        Access rm.${COMPONENT} RW/R

#
# Dynamic dependencies:
o.crontest:	c.crontest
o.crontest:	h.header
o.crontest:	C:h.kernel
o.crontest:	C:h.swis
