# Skeleton ASM Makefile
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 12 Nov 1997	RLT		Created

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
LD	= link
CP      = copy
RM      = remove
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = SystemTest
SOURCE     = s.systemtest
OBJ	   = o.systemtest
TARGET     = ${COMPONENT}
EXPORTS    = ${EXP_HDR}.${COMPONENT}
OM	   = o

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

export: ${EXPORTS}
        ${CP} Hdr.${COMPONENT} $@ ${CPFLAGS}
        @echo ${COMPONENT}: export complete

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

all: ${TARGET}
        @echo ${COMPONENT}: built

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}
	${LD} -o $@ -aif ${OBJ}

${EXP_HDR}.${COMPONENT}: Hdr.${COMPONENT}
        ${CP} Hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
