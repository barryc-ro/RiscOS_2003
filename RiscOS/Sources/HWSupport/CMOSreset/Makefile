# Makefile for CMOSreset
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 10-Mar-95  JRH          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
WIPE    = -wipe
AS      = objasm 
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
WFLAGS  = fr~c~v

#
# Program specific options:
#
COMPONENT = CMOSreset
SOURCE    = s.Module
OBJECT    = o.Module
TARGET    = rm.${MACHINE}.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${OBJECT}
        cdir rm.${MACHINE}
        link -o $@ -bin -base 0 ${OBJECT}
        settype $@ Module

${OBJECT}:
	${MKDIR} o
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

#
# Extra Development rules:
#
install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ${TARGET} installed

# Dynamic dependencies:
