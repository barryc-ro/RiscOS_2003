# Makefile for UnSqzAIF

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
LD      = link
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
LDFLAGS = -bin -base 0

#
# Program specific options:
#
COMPONENT = UnSqzAIF
SOURCE    = s.ModHead
TARGET    = rm.UnSqzAIF
OBJECT    = o.ModHead

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
        ${RM} o
        @echo ${COMPONENT}: cleaned

${TARGET}: ${OBJECT}
        ${LD} ${LDFLAGS} -o $@ ${OBJECT}
        settype $@ Module

${OBJECT}: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} -o $@ ${SOURCE}

# Dynamic dependencies:
