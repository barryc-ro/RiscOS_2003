# Project:   ROMinfo

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
ROMinfoRESS = squash

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
COMPONENT   = ROMinfo
APPLICATION = ROMinfo
TARGET      = ROMinfo
OBJS = o.ROMinfo

#
# Build for application:
#
ROMinfo:	${OBJS} ${CLIB} dirs
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

# Dynamic dependencies:
o.ROMinfo:	c.ROMinfo
o.ROMinfo:	C:h.swis
o.ROMinfo:	C:h.kernel
