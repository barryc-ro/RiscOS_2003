# Project:   crc

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
crcRESS = squash

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
COMPONENT   = crc
APPLICATION = crc
TARGET      = crc
OBJS = o.prog o.calc

#
# Build for application:
#
crc:	${OBJS} ${CLIB}
	${LD} -o $@ ${OBJS} ${CLIB}

#
# build an application:
#
all:	${APPLICATION}
	@echo ${APPLICATION} built

# Dynamic dependencies:
o.crc:	c.crc
o.crc:	h.crc
o.prog:	c.prog
o.prog:	h.crc
o.calc:	c.calc
o.calc:	h.crc
