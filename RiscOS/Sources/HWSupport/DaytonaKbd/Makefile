# Makefile for Daytona Keyboard driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-May-97  BAL          Created
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Program specific options:
#
COMPONENT = DaytonaKbd
TARGET    = rm.DaytonaKbd
OBJS = DaytonaKbd.o

#
# Rule patterns
#
.s.o:;      ${AS} ${ASFLAGS} -o $@ $<

#
# Generic rules:
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
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${OBJS}
	${LD} -o $@ -RMF ${OBJS}

# Dynamic dependencies:
