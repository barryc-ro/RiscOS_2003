# Makefile for Territory Manager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 21-Nov-00  SBF          Added StripDepnd support.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
LD      = link
CP      = copy
RM      = remove
WIPE    = -wipe
STRIP   = -stripdepnd
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit ${OPTS} -To $@ -From
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = TerrMgr
OBJECT    = o.TerrMgr
SOURCE    = s.TerrMgr
TARGET    = rm.TerrMgr
EXPORTS   = ${EXP_HDR}.Territory

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
        ${RM} ${TARGET}
	${WIPE} o ${WFLAGS}
        ${RM} z.SystemDisc
        ${STRIP}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${OBJECT}: ${SOURCE} z.SystemDisc
	${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
	${LD} ${LDFLAGS} ${OBJECT}
	SetType $@ &FFA

z.SystemDisc: SystemDisc
        @echo Compressing sprite
        MakeSprite
        stamp $@

${EXP_HDR}.Territory: hdr.Territory
        ${CP} hdr.Territory $@ ${CPFLAGS}

# Dynamic dependencies:
