# Makefile for IZipFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 04-Apr-1997 Rich Buckley  Created
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = IZipFS
TARGET     = rm.${COMPONENT}
SRC        = s.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET} dirs
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} hdr.${COMPONENT} ${EXP_HDR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources: resources-${TYPE}

resources-install: ${TARGET} dirs
        ${CP} ${TARGET} ${RESDIR}.^.${RES}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied into ResourceFS at ${COMPONENT}

resources-:
        @echo ${COMPONENT}: resource files copied

dirs:
        ${MKDIR} rm

${TARGET}: ${SRC}
        ${AS} ${ASFLAGS} ${SRC}

# Dynamic dependencies:
