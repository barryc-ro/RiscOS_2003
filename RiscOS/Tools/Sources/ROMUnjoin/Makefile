# Project:   ROMUnjoin

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
STRIP   = stripdepnd

AFLAGS  = -depend !Depend -Stamp -quit
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
WRAPPER   = s.ModuleWrap

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Include files
#
INCLUDES = -IC:

#
# Program specific options:
#
COMPONENT   = ROMUnjoin
APPLICATION = ROMUnjoin
TARGET      = ROMUnjoin
OBJS = o.ROMUnjoin

#
# Build for application:
#
ROMUnjoin:	${OBJS} ${CLIB} dirs
		${LD} -o $@ ${OBJS} ${CLIB}

dirs:
        ${MKDIR} o

#
# build an application:
#
all:	${APPLICATION}
	@echo ${APPLICATION} built

clean:
        ${RM} ${TARGET}
        ${WIPE} ${OBJS} ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${STRIP}

# Dynamic dependencies:
